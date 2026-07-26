# Website Latency Reduction - Recommendations

> **Context:** This project (Flask + Jinja2, no bundler/minifier) serves raw JS/CSS. Cluster disk is at 96% capacity. `Slurm` inventory/queue lookups add latency.

---

## 1. Enable Flask-Compress (Gzip/Brotli)

Flask-Compress is already in `requirements.txt` but gated behind `HPCC_ENABLE_COMPRESS=1`. Enable it in production to compress HTML, JS, CSS, and JSON responses on-the-fly (reduces transfer size by 60-80%).

**File:** `simg_zmq/main_html/config.py` — set `COMPRESS_ENABLED = True` unconditionally in ProductionConfig.

---

## 2. HTTP Cache Headers on Static Assets

Static files (`/static/css/style.css`, `/static/js/*`) have no aggressive cache headers. Set `Cache-Control: public, max-age=31536000, immutable` and version URLs with a content hash or query string (e.g., `style.css?v=HASH`). This eliminates repeat downloads for returning users.

Already partially done in hyperlink routes (`max_age=3600`) — extend to main static routes.

---

## 3. Bundle & Minify JS/CSS (No-Build Approach)

Since there is no Node.js toolchain, use existing Python tools to concatenate and minify:

- Use `cssmin` and `jsmin` (Python packages) or `rjsmin`/`rcssmin` (faster C extensions).
- Add a simple build script (`build_static.py`) that concatenates `main.js` + `file_browser.js` into one minified file, and minifies `style.css`.
- Serve the bundled versions in production; keep originals for development.

This reduces multiple HTTP requests and shrinks file sizes significantly.

---

## 4. Move CDN Assets to Self-Hosted Bundles

Bootstrap 5.3 CSS, Bootstrap Icons, and Google Fonts are loaded from CDNs (3-5 additional DNS lookups + TCP connections). Download these files and bundle them with your minified CSS/JS. This eliminates DNS resolution time and external dependency risks.

Alternative: Use `dns-prefetch` and `preconnect` on CDN origins as a lighter fix.

---

## 5. Lazy-Load Below-the-Fold Content

Large portions of the dashboard (job history tables, chat sessions, runtime maps) are rendered server-side and sent in full on every page load. Instead:

- Load critical content (navigation, status summary) server-side.
- Fetch heavy sections (job history table, broker health) via JavaScript `fetch()` calls after page render.
- Use `IntersectionObserver` to lazy-load sections as the user scrolls.

This reduces initial HTML payload size and Time to First Byte (TTFB).

---

## 6. Add `async`/`defer` to Script Tags

All `<script>` tags (including Bootstrap JS) are at the end of `<body>` (good), but scripts that don't need DOM at load time should use `defer`. The inline chat/refresh scripts can be extracted to a deferred external file. This prevents render-blocking.

---

## 7. Database Query Optimization

The cluster/`Slurm` inventory queries (`sacct`, `sinfo`, etc.) are likely blocking and slow, especially with 96% disk usage. Investigate:

- Cache `Slurm` command outputs with a TTL (e.g., 30-60 seconds) in Redis or in-memory.
- Use database connection pooling (already available via SQLAlchemy).
- Add indexes to frequently queried columns in `JobHistory`, `ChatSession`, etc.
- Move from SQLite to PostgreSQL in production (already planned — ensure it's active).

---

## 8. Reduce SQLite / Disk I/O Contention

At 96% disk capacity, even reads slow down due to fragmentation and lack of free space for temp files. Mitigations:

- Free up disk space (archive/delete old job data, logs, completed reports).
- Move SQLite database to a faster disk (SSD/NVMe) or to PostgreSQL (which handles high I/O better).
- Set `PRAGMA journal_mode=WAL` on SQLite for better concurrent read performance.
- Consider in-memory caching for frequently accessed data.

---

## 9. Optimize Gunicorn Worker Configuration

Current gunicorn config uses `gthread` workers. For an I/O-bound Flask app (file reads, DB queries, Slurm calls):

- Increase `workers` to `(2 × CPU cores) + 1`.
- Increase `threads` per worker (e.g., 4-8) to handle concurrent requests during slow I/O.
- Enable `keepalive` connections to reuse TCP connections.
- Ensure `timeout` is reasonable (not too short, causing worker restarts).

---

## 10. Use HTTP/2 or HTTP/3

HTTP/2 (multiplexing, server push) and HTTP/3 (QUIC, faster connection setup) dramatically improve perceived load time for sites with many assets. If using a reverse proxy (Nginx, Caddy, Traefik) in front of Gunicorn:

- Enable HTTP/2 (requires TLS).
- Enable HTTP/3 if the proxy supports it.
- Turn on static file serving at the reverse proxy level (bypass Python for static assets entirely).

Caddy is the simplest option — it enables HTTP/2 and HTTP/3 by default with automatic TLS.

---

## 11. CDN or Reverse Proxy for Static Assets

Serve `/static/` files from Nginx (or a CDN like Cloudflare) instead of through Gunicorn/Flask. This:

- Avoids Python process overhead for every JS/CSS request.
- Allows aggressive caching at the edge.
- Offloads TLS termination.

Even without a full CDN, a local Nginx reverse proxy handling static files + Flask for dynamic content is a major win.

---

## 12. Preload Critical Resources

Use `<link rel="preload">` in `<head>` for:

- `style.css` (render-critical CSS)
- `main.js` (if it's needed for above-the-fold interactivity)
- Key API endpoints the page fetches on load

This signals the browser to start downloading these resources immediately.

---

## 13. Add a Service Worker for Offline/Instant Load

A simple service worker can cache static assets (JS, CSS, Bootstrap) and API responses after the first visit. Subsequent page loads serve from cache instantly, even if the server is slow due to disk I/O.

Use the Workbox library (or write a minimal ~50-line service worker).

---

## 14. Audit and Optimize Large Jinja2 Templates

`dashboard.html` and `base.html` load many includes, macros, and blocks. Each DB call within a template (e.g., job history query) adds to TTFB. Move heavy data queries out of template context pre-processing and into async JavaScript endpoints. Profile template rendering time using Flask debug toolbar or manual timestamps.

---

## 15. Implement Database Read Replicas (PostgreSQL)

If the production database is PostgreSQL, set up a read replica for dashboard/report queries and keep writes (job submission, status updates) on the primary. This distributes I/O load and avoids contention.

---

## Priority Matrix

| # | Recommendation | Impact | Effort |
|---|----------------|--------|--------|
| 1 | Enable Flask-Compress | High | Low |
| 8 | Free disk space / fix I/O | High | Medium |
| 7 | Cache Slurm queries | High | Medium |
| 2 | Cache headers on static | Medium | Low |
| 4 | Preconnect to CDNs | Medium | Low |
| 3 | Bundle/minify JS/CSS | Medium | Medium |
| 9 | Tune Gunicorn workers | Medium | Low |
| 10 | Reverse proxy + HTTP/2 | High | Medium |
| 5 | Lazy-load sections | Medium | High |
| 6 | Async/defer scripts | Low | Low |

---

## Quick Wins (this week)

1. Enable Flask-Compress
2. Free up disk space (lowest-hanging fruit — directly fixes the 96% I/O bottleneck)
3. Add cache headers + versioned static URLs
4. Tune Gunicorn worker count
5. Preconnect to Bootstrap/Google CDNs in `<head>`

## Medium Term (this sprint)

6. Bundle and minify JS/CSS with a Python build script
7. Cache `Slurm` inventory/queue output with a 30s TTL
8. Move to PostgreSQL if not already done
9. Put Nginx/Caddy in front for HTTP/2 + static serving

## Long Term (next quarter)

10. Service worker for offline/instant load
11. Lazy-load dashboard sections
12. Database read replicas
