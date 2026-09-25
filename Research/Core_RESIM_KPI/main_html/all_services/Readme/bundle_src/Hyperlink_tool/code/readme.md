# Hyperlink_tool (Log Viewer) - Cloud/Cluster Web UI

This is the **online** (Flask) log viewer used in the cloud. It lets a user:
- Connect to a cluster via SFTP
- Provide an input path that contains both `html/` and `video/`
- Optionally provide an output path (cache location)
- View matched HTML/video content in the browser

## Install

From `tools/Hyperlink_tool/code`:

```bash
pip install -r requirements.txt
```

## Run (cloud/server)

```bash
cd tools/Hyperlink_tool/code
set LOGVIEW_HOST=0.0.0.0
set LOGVIEW_PORT=5000
set LOGVIEW_NO_BROWSER=1
python main.py
```

Then open: `http://<server-ip>:5000/`

## Using the UI

1. Click **Connect**
2. Enter cluster credentials (server/username/password)
3. Enter **Remote Path** that contains `html/` and `video/` subfolders
4. (Optional) Enter **Output Path** (where the tool stores the downloaded cache on the server)
5. Click **Scan Logs** and then click a log to download+view

## Caching behavior

If **Output Path** is empty, the tool uses the default cache directory:
- Linux: `~/.cache_html`
- Windows: `C:\\.cache_html`

You can override the default cache root with:
- `CACHE_HTML_DIR=/some/path`

## PostgreSQL logging (no SQLite)

Set one of these env vars to enable event logging:
- `HYPERLINK_DATABASE_URL=postgresql://user:pass@host:5432/dbname`
- or `DATABASE_URL=postgresql://...`

The app auto-creates a table named `hyperlink_events` and logs:
- cluster connect/disconnect
- per-log downloads (includes remote paths + output path)

## VLM video->txt (placeholder)

There is a `/api/vlm/process` endpoint and `html_online/vlm.py`, but **Slurm `srun` GPU execution is not integrated yet**.
When you’re ready, we can refactor VLM execution to submit `srun` jobs on the NVIDIA nodes and store results in PostgreSQL.
