"""Runner Health Monitor.

Production-ready replacement for gitHubRunner.py.

Outputs (to OUTPUT_DIR, default="."):
  report.html      — Interactive Plotly dashboard (for GitHub Pages)
  summary.md       — GitHub Step Summary markdown
  email_body.html  — Email-safe HTML summary for stakeholders
  metrics.json     — Machine-readable structured snapshot

Configuration via environment variables (all overridable via CLI):
  GITHUB_TOKEN       Required. PAT or Actions token with actions:read.
  GITHUB_API_URL     e.g. https://aptv.ghe.com/api/v3  (auto-set in Actions)
  GITHUB_SERVER_URL  e.g. https://aptv.ghe.com          (auto-set in Actions)
  GITHUB_REPOSITORY  e.g. GPO/Core_Radar_Gen8_iND13400  (auto-set in Actions)
  RUNNER_GROUP       Runner group name. Default: sh-advancedradar-lab
  DAYS_BACK          Look-back window in days. Default: 2
  MAX_RUNS           Cap on workflow runs to scan. Default: unlimited
  FAIL_THRESHOLD     Failure rate above which a runner is "unhealthy". Default: 0.25
  MAX_WORKERS        Thread pool size. Default: 20
  TIMEZONE           Display timezone. Default: America/Indiana/Indianapolis
  OUTPUT_DIR         Output directory. Default: .
  PAGES_URL          GitHub Pages URL injected into email/summary links.
"""

import argparse
import itertools
import json
import os
import random
import re
import sys
import threading
import time
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime, timedelta, timezone
from zoneinfo import ZoneInfo

import requests

# ============================================================
# CONFIG — resolved once at module load; overridden in main()
# ============================================================


def _resolve_base_url() -> str:
    if os.getenv("GITHUB_API_URL"):
        return os.getenv("GITHUB_API_URL").rstrip("/")
    if os.getenv("GITHUB_SERVER_URL"):
        return os.getenv("GITHUB_SERVER_URL").rstrip("/") + "/api/v3"
    return "https://api.github.com"


def _resolve_org_repo() -> tuple[str, str]:
    repo_env = os.getenv("GITHUB_REPOSITORY", "GPO/Core_Radar_Gen8_iND13400")
    parts = repo_env.split("/", 1)
    return parts[0], parts[1] if len(parts) > 1 else "Core_Radar_Gen8_iND13400"


TOKEN: str = os.getenv("GITHUB_TOKEN", "")
BASE_URL: str = _resolve_base_url()
ORG: str
REPO: str
ORG, REPO = _resolve_org_repo()
RUNNER_GROUP: str = os.getenv("RUNNER_GROUP", "sh-advancedradar-lab")
DAYS_BACK: int = int(os.getenv("DAYS_BACK", "2"))
MAX_RUNS: int | None = int(os.getenv("MAX_RUNS", "0")) or None
FAIL_THRESHOLD: float = float(os.getenv("FAIL_THRESHOLD", "0.25"))
MAX_WORKERS: int = int(os.getenv("MAX_WORKERS", "20"))
OUTPUT_DIR: str = os.getenv("OUTPUT_DIR", ".")
PAGES_URL: str = os.getenv("PAGES_URL", "")
LOCAL_TIMEZONE = ZoneInfo(os.getenv("TIMEZONE", "America/Indiana/Indianapolis"))
LOCAL_TIMEZONE_LABEL: str = os.getenv("TIMEZONE", "America/Indiana/Indianapolis")

# ============================================================
# API — paginated GET with exponential back-off + rate-limit handling
# ============================================================


def _headers() -> dict:
    if not TOKEN:
        raise RuntimeError("GITHUB_TOKEN is not set.")
    return {"Authorization": f"Bearer {TOKEN}", "Accept": "application/vnd.github+json"}


def github_get(url: str, params: dict | None = None, max_retries: int = 6):
    """Yield pages (JSON objects) from a paginated GitHub API endpoint."""
    while url:
        for attempt in range(max_retries):
            try:
                r = requests.get(url, headers=_headers(), params=params, timeout=30)
            except requests.exceptions.RequestException as exc:
                if attempt >= max_retries - 1:
                    raise
                _backoff(attempt, f"request error ({exc})")
                continue

            if r.status_code == 429 or (
                r.status_code == 403 and r.headers.get("X-RateLimit-Remaining") == "0"
            ):
                reset_ts = int(r.headers.get("X-RateLimit-Reset", time.time() + 60))
                wait = max(1.0, reset_ts - time.time()) + random.uniform(0.5, 2.0)
                print(f"\nRate-limited — waiting {wait:.0f}s before retry…", flush=True)
                time.sleep(min(wait, 180))
                continue

            if r.status_code >= 500 and attempt < max_retries - 1:
                _backoff(attempt, f"HTTP {r.status_code}")
                continue

            if r.status_code >= 400:
                msg = ""
                try:
                    msg = r.json().get("message", "")
                except Exception:
                    pass
                print(f"\nHTTP {r.status_code} for {url}" + (f" — {msg}" if msg else ""))
                if r.status_code in (401, 403):
                    print("Verify GITHUB_TOKEN is valid and has actions:read scope.")
                r.raise_for_status()

            break  # success

        yield r.json()
        url = r.links.get("next", {}).get("url")
        params = None  # params only apply to first request


def _backoff(attempt: int, reason: str) -> None:
    wait = min(2**attempt + random.uniform(0, 1), 60)
    print(f"\n{reason} — retrying in {wait:.1f}s (attempt {attempt + 1})…", flush=True)
    time.sleep(wait)


# ============================================================
# RUNNER INVENTORY — live status from the runner group
# ============================================================


def get_runner_inventory() -> list[dict]:
    """Return live runner records from the configured runner group."""
    try:
        group_id = _find_runner_group_id()
        if group_id is None:
            print(f"Warning: runner group '{RUNNER_GROUP}' not found in org '{ORG}'.")
            return []
        runners = []
        for page in github_get(
            f"{BASE_URL}/orgs/{ORG}/actions/runner-groups/{group_id}/runners",
            {"per_page": 100},
        ):
            runners.extend(page.get("runners", []))
        return runners
    except Exception as exc:
        print(f"Warning: could not fetch runner inventory — {exc}")
        return []


def _find_runner_group_id() -> int | None:
    for page in github_get(f"{BASE_URL}/orgs/{ORG}/actions/runner-groups", {"per_page": 100}):
        for group in page.get("runner_groups", []):
            if group.get("name") == RUNNER_GROUP:
                return group["id"]
    return None


# ============================================================
# STEP / VARIANT CLASSIFICATION
# ============================================================


def classify_step(name: str) -> str:
    """Return the category of a CI step: infra, test, or other."""
    n = name.lower()
    if any(
        x in n
        for x in [
            "setup",
            "install",
            "checkout",
            "init",
            "download",
            "upload",
            "artifact",
            "flash",
            "quickflash",
            "build",
            "environment",
            "docker",
        ]
    ):
        return "infra"
    if any(x in n for x in ["test", "smoke", "pytest", "validation", "verify"]):
        return "test"
    return "other"


def extract_variant(job_name: str) -> str:
    """Extract a job variant label from the job name."""
    name = (job_name or "").strip()
    m = re.search(r"\(([^()]+)\)\s*$", name)
    if m:
        return m.group(1).strip() or "unknown"
    if " / " in name:
        tail = name.rsplit(" / ", 1)[-1].strip()
        if tail:
            return tail
    return "unknown"


# ============================================================
# SPINNER
# ============================================================


def _spinner(stop_event: threading.Event, msg: str) -> None:
    for c in itertools.cycle("|/-\\"):
        if stop_event.is_set():
            break
        sys.stdout.write(f"\r{msg}... {c}")
        sys.stdout.flush()
        time.sleep(0.1)


def _run_with_spinner(func, msg: str):
    ev = threading.Event()
    t = threading.Thread(target=_spinner, args=(ev, msg))
    t.start()
    try:
        result = func()
        return result
    except Exception:
        print(f"\r{msg}... failed")
        raise
    finally:
        ev.set()
        t.join()
        print(f"\r{msg} ✅          ")


# ============================================================
# DATA FETCHING
# ============================================================


def get_runs(days_back: int, max_runs: int | None = None) -> list[dict]:
    """Fetch workflow runs created within the last days_back days."""
    url = f"{BASE_URL}/repos/{ORG}/{REPO}/actions/runs"
    since = (datetime.now(timezone.utc) - timedelta(days=days_back)).isoformat()
    params = {"per_page": 100, "created": f">{since}"}
    runs: list[dict] = []
    for page in github_get(url, params):
        runs.extend(page.get("workflow_runs", []))
        if max_runs and len(runs) >= max_runs:
            return runs[:max_runs]
    return runs


def get_jobs(run_id: int) -> list[dict]:
    """Fetch all jobs for a given workflow run."""
    url = f"{BASE_URL}/repos/{ORG}/{REPO}/actions/runs/{run_id}/jobs"
    jobs: list[dict] = []
    for page in github_get(url):
        jobs.extend(page.get("jobs", []))
    return jobs


def parse_iso_utc(ts: str) -> datetime:
    """Parse an ISO 8601 UTC timestamp string to an aware datetime."""
    return datetime.fromisoformat(ts.replace("Z", "+00:00"))


def to_local(dt: datetime) -> datetime:
    """Convert a UTC-aware datetime to the configured local timezone."""
    return dt.astimezone(LOCAL_TIMEZONE)


# ============================================================
# ANALYSIS
# ============================================================


def analyze(days_back: int, max_runs: int | None = None) -> tuple:
    """Fetch and aggregate job statistics for the given look-back window."""
    runs = _run_with_spinner(lambda: get_runs(days_back, max_runs=max_runs), "Fetching runs")

    runner_stats = defaultdict(lambda: {"success": 0, "failure": 0, "total": 0})
    daily_stats = defaultdict(lambda: {"success": 0, "failure": 0, "total": 0})
    step_failures = defaultdict(int)
    step_daily = defaultdict(lambda: defaultdict(int))
    runner_step_fail = defaultdict(lambda: defaultdict(int))
    runner_step_variant_fail = defaultdict(lambda: defaultdict(lambda: defaultdict(int)))
    runner_daily_fail = defaultdict(lambda: defaultdict(int))
    runner_hourly = defaultdict(lambda: defaultdict(lambda: {"success": 0, "failure": 0}))
    runner_events: list[dict] = []
    coverage = {
        "runs_scanned": len(runs),
        "jobs_fetched": 0,
        "jobs_self_hosted": 0,
        "jobs_group_matched": 0,
    }
    hours_cutoff = datetime.now(timezone.utc) - timedelta(days=days_back)

    start_time = time.time()
    total_runs = len(runs)
    done_count = 0
    lock = threading.Lock()

    def _failure_reason(job: dict) -> str:
        failed_steps = [
            f"{st.get('name', '?')} ({st.get('conclusion')})"
            for st in job.get("steps", [])
            if st.get("conclusion") in ("failure", "cancelled", "timed_out", "startup_failure")
        ]
        if failed_steps:
            return "; ".join(failed_steps[:2])
        return job.get("conclusion") or "unknown"

    def _process_run(run: dict):
        r = defaultdict(lambda: {"success": 0, "failure": 0, "total": 0})
        d = defaultdict(lambda: {"success": 0, "failure": 0, "total": 0})
        s = defaultdict(int)
        sd = defaultdict(lambda: defaultdict(int))
        rs = defaultdict(lambda: defaultdict(int))
        rv = defaultdict(lambda: defaultdict(lambda: defaultdict(int)))
        rd = defaultdict(lambda: defaultdict(int))
        rh = defaultdict(lambda: defaultdict(lambda: {"success": 0, "failure": 0}))
        events: list[dict] = []
        local_cov = {"jobs_fetched": 0, "jobs_self_hosted": 0, "jobs_group_matched": 0}

        run_number = run.get("run_number")
        run_attempt = run.get("run_attempt")
        run_id = run.get("id")
        workflow = run.get("name")
        run_url = run.get("html_url")

        for job in get_jobs(run["id"]):
            local_cov["jobs_fetched"] += 1
            if "self-hosted" not in job.get("labels", []):
                continue
            local_cov["jobs_self_hosted"] += 1
            if job.get("runner_group_name") != RUNNER_GROUP:
                continue
            local_cov["jobs_group_matched"] += 1

            runner = job.get("runner_name", "unknown")
            job_id = job.get("id")
            job_name = job.get("name")
            variant = extract_variant(job_name)
            job_url = job.get("html_url")
            created_at = job.get("created_at")
            if not created_at:
                continue

            dt = parse_iso_utc(created_at)
            dt_local = to_local(dt)
            day = dt_local.date().isoformat()
            hour_key = dt_local.replace(minute=0, second=0, microsecond=0).isoformat()
            conclusion = job.get("conclusion")

            r[runner]["total"] += 1
            d[day]["total"] += 1

            if conclusion == "success":
                r[runner]["success"] += 1
                d[day]["success"] += 1
                if dt >= hours_cutoff:
                    rh[runner][hour_key]["success"] += 1
                    events.append(
                        {
                            "runner": runner,
                            "ts": dt_local.isoformat(),
                            "result": "success",
                            "run_number": run_number,
                            "run_attempt": run_attempt,
                            "run_id": run_id,
                            "workflow": workflow,
                            "job_id": job_id,
                            "job_name": job_name,
                            "run_url": run_url,
                            "job_url": job_url,
                            "failure_reason": "",
                        }
                    )
            else:
                r[runner]["failure"] += 1
                d[day]["failure"] += 1
                rd[runner][day] += 1
                reason = _failure_reason(job)
                if dt >= hours_cutoff:
                    rh[runner][hour_key]["failure"] += 1
                    events.append(
                        {
                            "runner": runner,
                            "ts": dt_local.isoformat(),
                            "result": "failure",
                            "run_number": run_number,
                            "run_attempt": run_attempt,
                            "run_id": run_id,
                            "workflow": workflow,
                            "job_id": job_id,
                            "job_name": job_name,
                            "run_url": run_url,
                            "job_url": job_url,
                            "failure_reason": reason,
                        }
                    )
                for st in job.get("steps", []):
                    if st.get("conclusion") == "failure":
                        step = st["name"]
                        s[step] += 1
                        sd[step][day] += 1
                        rs[runner][step] += 1
                        rv[runner][step][variant] += 1

        return r, d, s, sd, rs, rv, rd, rh, events, local_cov

    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
        futures = [executor.submit(_process_run, run) for run in runs]
        for fut in as_completed(futures):
            rr, dd, ss, sd, rs, rv, rd, rh, events, local_cov = fut.result()
            with lock:
                for k, v in rr.items():
                    for key in v:
                        runner_stats[k][key] += v[key]
                for k, v in dd.items():
                    for key in v:
                        daily_stats[k][key] += v[key]
                for k, v in ss.items():
                    step_failures[k] += v
                for step, days in sd.items():
                    for day, cnt in days.items():
                        step_daily[step][day] += cnt
                for runner, steps in rs.items():
                    for step, cnt in steps.items():
                        runner_step_fail[runner][step] += cnt
                for runner, steps in rv.items():
                    for step, variants in steps.items():
                        for variant, cnt in variants.items():
                            runner_step_variant_fail[runner][step][variant] += cnt
                for runner, days in rd.items():
                    for day, cnt in days.items():
                        runner_daily_fail[runner][day] += cnt
                for runner, hours in rh.items():
                    for h, cd in hours.items():
                        runner_hourly[runner][h]["success"] += cd.get("success", 0)
                        runner_hourly[runner][h]["failure"] += cd.get("failure", 0)
                runner_events.extend(events)
                coverage["jobs_fetched"] += local_cov["jobs_fetched"]
                coverage["jobs_self_hosted"] += local_cov["jobs_self_hosted"]
                coverage["jobs_group_matched"] += local_cov["jobs_group_matched"]
                done_count += 1

            elapsed = time.time() - start_time
            rate = done_count / elapsed if elapsed else 0
            eta = (total_runs - done_count) / rate if rate else 0
            sys.stdout.write(
                f"\rProcessed {done_count}/{total_runs} | {rate:.2f}/s | ETA {int(eta)}s   "
            )
            sys.stdout.flush()

    print()
    return (
        runner_stats,
        daily_stats,
        step_failures,
        step_daily,
        runner_step_fail,
        runner_step_variant_fail,
        runner_daily_fail,
        runner_hourly,
        runner_events,
        coverage,
    )


# ============================================================
# UNHEALTHY RUNNER DETECTION
# ============================================================

MIN_JOBS_FOR_RATE_THRESHOLD = 3  # avoid false positives on low-volume runners


def identify_unhealthy(
    runner_stats: dict,
    runner_inventory: list[dict],
    fail_threshold: float = 0.25,
) -> list[dict]:
    """Return runners that are offline or exceed the failure rate threshold."""
    inventory_map = {r["name"]: r for r in runner_inventory}
    unhealthy: list[dict] = []
    seen: set[str] = set()

    for rname, data in runner_stats.items():
        total = data["total"]
        fail = data["failure"]
        rate = fail / total if total else 0.0
        inv = inventory_map.get(rname, {})
        status = inv.get("status", "unknown")
        busy = inv.get("busy", False)

        reasons = []
        if status == "offline":
            reasons.append("offline")
        if total >= MIN_JOBS_FOR_RATE_THRESHOLD and rate > fail_threshold:
            reasons.append(f"failure rate {rate:.1%} > {fail_threshold:.0%} threshold")

        if reasons:
            unhealthy.append(
                {
                    "name": rname,
                    "reason": " + ".join(reasons),
                    "status": status,
                    "busy": busy,
                    "total_jobs": total,
                    "failures": fail,
                    "failure_rate": rate,
                }
            )
        seen.add(rname)

    # Runners present in inventory but never matched a job in the period
    for r in runner_inventory:
        if r["name"] not in seen and r.get("status") == "offline":
            unhealthy.append(
                {
                    "name": r["name"],
                    "reason": "offline — no jobs matched in period",
                    "status": "offline",
                    "busy": r.get("busy", False),
                    "total_jobs": 0,
                    "failures": 0,
                    "failure_rate": 0.0,
                }
            )

    return sorted(unhealthy, key=lambda x: (-x["failures"], x["name"]))


# ============================================================
# TREND INTELLIGENCE
# ============================================================


def compute_trends(sd: dict) -> dict:
    """Compute growth rates for each failed step across the period."""
    trends: dict = {}
    for step, data in sd.items():
        days = sorted(data)
        counts = [data[d] for d in days]
        if len(counts) < 2:
            continue
        mid = len(counts) // 2
        prev = sum(counts[:mid])
        recent = sum(counts[mid:])
        growth = (recent - prev) / prev if prev else (float("inf") if recent else 0.0)
        trends[step] = {"recent": recent, "previous": prev, "growth": growth}
    return trends


def summarize_trends(trends: dict) -> list[str]:
    """Return up to five human-readable trend messages."""
    msgs: list[str] = []
    for step, t in trends.items():
        if t["growth"] == float("inf"):
            msgs.append(f"NEW issue: {step}")
        elif t["growth"] > 1.0:
            msgs.append(f"SPIKE: {step} (+{t['growth']:.1f}x)")
        elif t["growth"] < -0.5:
            msgs.append(f"Improving: {step}")
    return msgs[:5]


# ============================================================
# INTERACTIVE PLOTS (Plotly)
# ============================================================


def _plotly_available() -> bool:
    try:
        import plotly  # noqa: F401

        return True
    except ImportError:
        return False


def render_runner_hourly_interactive(rh: dict, days_back: int) -> str | None:
    """Return a Plotly HTML div for per-runner hourly pass/fail, or None."""
    try:
        import plotly.graph_objects as go
        from plotly.offline import plot as plotly_plot
    except ImportError:
        return None

    totals = {
        k: sum(v.get("success", 0) + v.get("failure", 0) for v in vh.values())
        for k, vh in rh.items()
    }
    top = sorted(totals, key=totals.get, reverse=True)[:6]
    days = sorted({h for r in top for h in rh[r]}, key=lambda h: datetime.fromisoformat(h))
    if not days:
        return None
    dt_labels = [datetime.fromisoformat(h) for h in days]

    fig = go.Figure()
    for runner in top:
        successes = [rh[runner].get(h, {}).get("success", 0) for h in days]
        failures = [rh[runner].get(h, {}).get("failure", 0) for h in days]
        fig.add_trace(
            go.Scatter(x=dt_labels, y=successes, mode="lines+markers", name=f"{runner} ✓")
        )
        fig.add_trace(
            go.Scatter(
                x=dt_labels,
                y=failures,
                mode="lines+markers",
                name=f"{runner} ✗",
                line={"dash": "dash"},
            )
        )

    fig.update_layout(
        title=f"Last {days_back}d: Per-runner Passed vs Failed (hourly)",
        xaxis_title="Time",
        yaxis_title="Count",
        legend={"orientation": "h"},
    )
    return plotly_plot(fig, include_plotlyjs="cdn", output_type="div")


def render_runner_timeline_interactive(events: list[dict], days_back: int) -> str | None:
    """Return a Plotly HTML div for the runner event timeline, or None."""
    try:
        import plotly.express as px
        from plotly.offline import plot as plotly_plot
        import pandas as pd
    except ImportError:
        return None

    if not events:
        return None

    cnt = defaultdict(int)
    for ev in events:
        cnt[ev.get("runner", "unknown")] += 1
    top = sorted(cnt, key=cnt.get, reverse=True)[:12]

    rows = []
    for ev in events:
        r = ev.get("runner", "unknown")
        if r not in top:
            continue
        ts = ev.get("ts")
        if not ts:
            continue
        dt = datetime.fromisoformat(ts.replace("Z", "+00:00"))
        try:
            is_rerun = int(ev.get("run_attempt", 1)) > 1
        except (TypeError, ValueError):
            is_rerun = False
        rows.append(
            {
                "runner": r,
                "ts": dt,
                "result": ev.get("result", "unknown"),
                "run_number": ev.get("run_number"),
                "run_attempt": ev.get("run_attempt"),
                "rerun": "rerun" if is_rerun else "first-run",
                "run_id": ev.get("run_id"),
                "workflow": ev.get("workflow"),
                "job_id": ev.get("job_id"),
                "job_name": ev.get("job_name"),
                "run_url": ev.get("run_url"),
                "job_url": ev.get("job_url"),
                "failure_reason": ev.get("failure_reason", ""),
            }
        )
    if not rows:
        return None

    df = pd.DataFrame(rows)
    fig = px.scatter(
        df,
        x="ts",
        y="runner",
        color="result",
        symbol="rerun",
        symbol_map={"first-run": "circle", "rerun": "diamond"},
        color_discrete_map={"success": "green", "failure": "red"},
        title=f"Last {days_back}d: Runner Event Timeline",
        custom_data=["run_url", "job_url"],
        hover_data={
            "run_number": True,
            "run_attempt": True,
            "rerun": True,
            "job_name": True,
            "failure_reason": True,
            "run_url": False,
            "job_url": False,
            "run_id": False,
            "workflow": False,
            "job_id": False,
            "ts": "|%b %d, %Y, %H:%M:%S",
        },
    )
    fig.update_traces(marker={"size": 7})
    height = max(220, min(360, 40 * max(len(top), 1) + 120))
    fig.update_layout(
        yaxis={"categoryorder": "array", "categoryarray": sorted(top, reverse=True)},
        height=height,
    )
    return plotly_plot(fig, include_plotlyjs="cdn", output_type="div")


def render_step_trends_interactive(sd: dict) -> str | None:
    """Return a Plotly HTML div for step failure trends, or None."""
    try:
        import plotly.graph_objects as go
        from plotly.offline import plot as plotly_plot
    except ImportError:
        return None

    totals = {k: sum(v.values()) for k, v in sd.items()}
    top = sorted(totals, key=totals.get, reverse=True)[:6]
    days = sorted({d for s in top for d in sd[s]})
    if not days:
        return None
    x = [datetime.fromisoformat(d + "T00:00").replace(tzinfo=LOCAL_TIMEZONE) for d in days]

    fig = go.Figure()
    for step in top:
        fig.add_trace(
            go.Scatter(
                x=x,
                y=[sd[step].get(d, 0) for d in days],
                mode="lines+markers",
                name=step,
            )
        )
    fig.update_layout(
        title="Step Trends",
        xaxis_title="Date",
        yaxis_title="Count",
        legend={"orientation": "h"},
    )
    return plotly_plot(fig, include_plotlyjs="cdn", output_type="div")


def render_runner_trends_interactive(rd: dict) -> str | None:
    """Return a Plotly HTML div for runner failure trends, or None."""
    try:
        import plotly.graph_objects as go
        from plotly.offline import plot as plotly_plot
    except ImportError:
        return None

    totals = {k: sum(v.values()) for k, v in rd.items()}
    top = sorted(totals, key=totals.get, reverse=True)[:6]
    days = sorted({d for r in top for d in rd[r]})
    if not days:
        return None
    x = [datetime.fromisoformat(d + "T00:00").replace(tzinfo=LOCAL_TIMEZONE) for d in days]

    fig = go.Figure()
    for runner in top:
        fig.add_trace(
            go.Scatter(
                x=x,
                y=[rd[runner].get(d, 0) for d in days],
                mode="lines+markers",
                name=runner,
            )
        )
    fig.update_layout(
        title="Runner Failed Jobs Per Day",
        xaxis_title="Date",
        yaxis_title="Failures",
        legend={"orientation": "h"},
    )
    return plotly_plot(fig, include_plotlyjs="cdn", output_type="div")


def render_daily_jobs_interactive(daily: dict, days_back: int) -> str | None:
    """Return a Plotly HTML div for the daily job stacked bar chart, or None."""
    try:
        import plotly.graph_objects as go
        from plotly.offline import plot as plotly_plot
    except ImportError:
        return None

    if days_back <= 0:
        return None

    end_day = datetime.now(LOCAL_TIMEZONE).date()
    start_day = end_day - timedelta(days=days_back - 1)
    days = [(start_day + timedelta(days=i)).isoformat() for i in range(days_back)]
    x = [datetime.fromisoformat(d + "T00:00").replace(tzinfo=LOCAL_TIMEZONE) for d in days]
    success = [daily.get(d, {}).get("success", 0) for d in days]
    failure = [daily.get(d, {}).get("failure", 0) for d in days]
    total = [s + f for s, f in zip(success, failure)]
    frate = [(f / t) if t else 0 for f, t in zip(failure, total)]

    fig = go.Figure()
    fig.add_trace(
        go.Bar(
            x=x,
            y=success,
            name="Success",
            marker_color="#2fb344",
            customdata=[[t, f, fr] for t, f, fr in zip(total, failure, frate)],
            hovertemplate="Date: %{x|%Y-%m-%d}<br>Success: %{y}<br>Total: %{customdata[0]}<br>Failure: %{customdata[1]} (%{customdata[2]:.1%})<extra></extra>",
        )
    )
    fig.add_trace(
        go.Bar(
            x=x,
            y=failure,
            name="Failure",
            marker_color="#e03131",
            customdata=[[t, s, fr] for t, s, fr in zip(total, success, frate)],
            hovertemplate="Date: %{x|%Y-%m-%d}<br>Failure: %{y} (%{customdata[2]:.1%})<br>Total: %{customdata[0]}<br>Success: %{customdata[1]}<extra></extra>",
        )
    )
    fig.update_layout(
        title=f"Last {days_back}d: Jobs Per Day (Stacked Success/Failure)",
        xaxis_title="Date",
        yaxis_title="Jobs",
        barmode="stack",
        legend={"orientation": "h"},
        height=320,
    )
    return plotly_plot(fig, include_plotlyjs="cdn", output_type="div")


# ============================================================
# HELPER
# ============================================================


def _runner_sort_key(name: str) -> tuple:
    m = re.search(r"^(.*?)(\d+)$", name)
    if m:
        return (m.group(1), int(m.group(2)), name)
    return (name, 0, name)


def _out(filename: str) -> str:
    return os.path.join(OUTPUT_DIR, filename)


# ============================================================
# OUTPUT 1: report.html  (Interactive Plotly Dashboard)
# ============================================================


def generate_html(stats: tuple, days_back: int, unhealthy: list[dict]) -> None:
    """Write the interactive Plotly dashboard to report.html in OUTPUT_DIR."""
    runner, daily, steps, sd, rs, rsv, rd, rh, events, coverage = stats

    trends = compute_trends(sd)
    trend_msgs = summarize_trends(trends)
    last_updated = datetime.now(LOCAL_TIMEZONE).strftime("%Y-%m-%d %H:%M:%S %Z")

    total_jobs_window = sum(v.get("total", 0) for v in daily.values())
    active_days = len([d for d, v in daily.items() if v.get("total", 0) > 0])
    avg_jobs_per_day = (total_jobs_window / active_days) if active_days else 0
    display_days = min(days_back, active_days) if active_days else days_back
    total_failures_window = sum(v.get("failure", 0) for v in daily.values())
    overall_frate = total_failures_window / total_jobs_window if total_jobs_window else 0
    online_count = sum(1 for v in runner.values() if v.get("total", 0) > 0)
    unhealthy_count = len(unhealthy)
    healthy_count = max(0, online_count - unhealthy_count)

    top_failing = [
        (rn, d)
        for rn, d in sorted(runner.items(), key=lambda x: x[1].get("failure", 0), reverse=True)
        if d.get("failure", 0) > 0
    ][:15]
    top_steps = sorted(steps.items(), key=lambda x: x[1], reverse=True)[:15]

    # ── CSS ──────────────────────────────────────────────────────────────────
    css = """
:root{--bg:#f0f2f5;--surface:#fff;--border:#dde3ea;--text:#111827;--muted:#6b7280;
      --primary:#1a56db;--danger:#dc2626;--warn:#d97706;--ok:#16a34a;
      --danger-bg:#fef2f2;--warn-bg:#fffbeb;--ok-bg:#f0fdf4;
      --shadow:0 1px 4px rgba(0,0,0,.08);}
*{box-sizing:border-box;margin:0;padding:0;}
body{font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif;
     font-size:14px;background:var(--bg);color:var(--text);line-height:1.5;}
/* ── sticky header ── */
.top-bar{position:sticky;top:0;z-index:100;background:var(--primary);color:#fff;
         padding:10px 28px;display:flex;justify-content:space-between;align-items:center;
         box-shadow:0 2px 8px rgba(0,0,0,.18);}
.top-bar-title{font-size:17px;font-weight:700;letter-spacing:.02em;}
.top-bar-meta{font-size:12px;opacity:.85;text-align:right;}
/* ── layout ── */
.layout{max-width:1360px;margin:0 auto;padding:22px 24px;}
.card{background:var(--surface);border-radius:8px;border:1px solid var(--border);
      margin-bottom:20px;box-shadow:var(--shadow);overflow:hidden;}
.card-head{padding:13px 20px;border-bottom:1px solid var(--border);
           display:flex;justify-content:space-between;align-items:center;}
.card-title{font-size:14px;font-weight:700;color:var(--text);}
.card-sub{font-size:12px;color:var(--muted);}
.card-body{padding:16px 20px;}
/* ── KPI grid ── */
.kpi-row{display:grid;grid-template-columns:repeat(auto-fit,minmax(140px,1fr));gap:14px;
         margin-bottom:4px;}
.kpi{padding:14px 18px;border-radius:6px;border:1px solid var(--border);
     background:var(--surface);border-top-width:4px;}
.kpi-val{font-size:26px;font-weight:800;line-height:1.15;}
.kpi-label{font-size:11px;color:var(--muted);margin-top:3px;text-transform:uppercase;
           letter-spacing:.04em;}
.kpi-blue{border-top-color:var(--primary);} .kpi-blue .kpi-val{color:var(--primary);}
.kpi-red{border-top-color:var(--danger);} .kpi-red .kpi-val{color:var(--danger);}
.kpi-orange{border-top-color:var(--warn);} .kpi-orange .kpi-val{color:var(--warn);}
.kpi-green{border-top-color:var(--ok);} .kpi-green .kpi-val{color:var(--ok);}
/* ── tables ── */
table{width:100%;border-collapse:collapse;font-size:13px;}
thead tr{background:#f8fafc;}
th{padding:9px 12px;text-align:left;font-weight:600;color:var(--muted);
   border-bottom:2px solid var(--border);white-space:nowrap;}
td{padding:8px 12px;border-bottom:1px solid #f1f4f8;vertical-align:top;}
tr:last-child td{border-bottom:none;}
tbody tr:hover td{background:#f8fafc;}
/* ── badges ── */
.badge{display:inline-block;padding:2px 9px;border-radius:12px;font-size:11px;font-weight:600;}
.badge-red{background:var(--danger-bg);color:var(--danger);}
.badge-orange{background:var(--warn-bg);color:var(--warn);}
.badge-green{background:var(--ok-bg);color:var(--ok);}
.badge-gray{background:#f1f5f9;color:#475569;}
.badge-blue{background:#eff6ff;color:var(--primary);}
/* ── status dot ── */
.dot{display:inline-block;width:8px;height:8px;border-radius:50%;
     margin-right:5px;vertical-align:middle;}
.dot-red{background:var(--danger);} .dot-orange{background:var(--warn);}
.dot-green{background:var(--ok);} .dot-gray{background:#9ca3af;}
/* ── health bar ── */
.hbar-wrap{min-width:160px;}
.hbar-labels{display:flex;justify-content:space-between;font-size:11px;
             color:var(--muted);margin-bottom:3px;}
.hbar{height:13px;border-radius:3px;overflow:hidden;background:#e5e7eb;display:flex;}
.hbar-pass{background:#16a34a;} .hbar-fail{background:#dc2626;}
.hbar-zero{background:#d1d5db;width:100%;}
/* ── trends ── */
.trend-spike{color:var(--danger);font-weight:600;}
.trend-new{color:var(--warn);font-weight:600;}
.trend-good{color:var(--ok);font-weight:600;}
ul.trend-list{padding-left:18px;} ul.trend-list li{margin-bottom:4px;}
/* ── meta grid ── */
.meta-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:8px;}
.meta-item{display:flex;justify-content:space-between;padding:7px 12px;
           background:#f8fafc;border-radius:5px;border:1px solid var(--border);font-size:13px;}
.meta-key{color:var(--muted);}
.meta-val{font-weight:600;}
/* ── variants list ── */
.var-list{list-style:none;padding:0;margin:3px 0 0 0;}
.var-list li{font-size:12px;color:var(--muted);}
"""

    # ── HTML skeleton ─────────────────────────────────────────────────────────
    parts = [
        "<!DOCTYPE html><html lang='en'>",
        "<head><meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width,initial-scale=1'>",
        f"<title>Runner Health — {ORG}/{REPO}</title>",
        f"<style>{css}</style></head><body>",
    ]

    # ── Top bar ───────────────────────────────────────────────────────────────
    parts.append(
        "<div class='top-bar'>"
        f"<div class='top-bar-title'>Runner Health Dashboard</div>"
        "<div class='top-bar-meta'>"
        f"{ORG}/{REPO} &nbsp;|&nbsp; Group: {RUNNER_GROUP}<br>"
        f"Updated: {last_updated} &nbsp;|&nbsp; Last {display_days}d"
        "</div></div>"
    )
    parts.append("<div class='layout'>")

    def _frate_cls(rate: float) -> str:
        if rate > 0.3:
            return "kpi-red"
        if rate > 0.15:
            return "kpi-orange"
        return "kpi-green"

    # ── KPI cards ─────────────────────────────────────────────────────────────
    parts.append("<div class='kpi-row'>")
    kpi_data = [
        (str(online_count), "Active Runners", "kpi-blue"),
        (str(total_jobs_window), "Total Jobs", "kpi-blue"),
        (str(total_failures_window), "Total Failures", _frate_cls(overall_frate)),
        (f"{overall_frate:.1%}", "Overall Failure Rate", _frate_cls(overall_frate)),
        (str(active_days), "Active Days", "kpi-blue"),
        (f"{avg_jobs_per_day:.0f}", "Avg Jobs / Day", "kpi-blue"),
        (str(unhealthy_count), "Unhealthy Runners", "kpi-red" if unhealthy_count else "kpi-green"),
        (str(healthy_count), "Healthy Runners", "kpi-green"),
    ]
    for val, label, cls in kpi_data:
        parts.append(
            f"<div class='kpi {cls}'>"
            f"<div class='kpi-val'>{val}</div>"
            f"<div class='kpi-label'>{label}</div>"
            "</div>"
        )
    parts.append("</div>")

    # ── Unhealthy runners ─────────────────────────────────────────────────────
    if unhealthy:
        parts.append("<div class='card'>")
        parts.append(
            "<div class='card-head'>"
            f"<span class='card-title'>Unhealthy Runners ({unhealthy_count})</span>"
            "<span class='card-sub'>Runners that are offline or exceed the failure rate threshold</span>"
            "</div>"
        )
        parts.append("<div class='card-body'>")
        parts.append(
            "<table><thead><tr>"
            "<th>Runner</th><th>Status</th><th>Reason</th>"
            "<th>Total Jobs</th><th>Failures</th><th>Failure Rate</th>"
            "</tr></thead><tbody>"
        )
        for u in unhealthy:
            status = u["status"]
            dot_cls = "dot-red" if status == "offline" else "dot-orange"
            rate_badge = "badge-red" if u["failure_rate"] > 0.3 else "badge-orange"
            parts.append(
                "<tr>"
                f"<td><strong>{u['name']}</strong></td>"
                f"<td><span class='dot {dot_cls}'></span>{status}</td>"
                f"<td><span class='badge badge-red'>{u['reason']}</span></td>"
                f"<td>{u['total_jobs']}</td>"
                f"<td>{u['failures']}</td>"
                f"<td><span class='badge {rate_badge}'>{u['failure_rate']:.1%}</span></td>"
                "</tr>"
            )
        parts.append("</tbody></table></div></div>")

    # ── Per-runner breakdown ──────────────────────────────────────────────────
    parts.append("<div class='card'>")
    parts.append(
        "<div class='card-head'>"
        "<span class='card-title'>Per Runner Breakdown</span>"
        f"<span class='card-sub'>{online_count} runners — last {display_days}d</span>"
        "</div><div class='card-body'>"
    )
    parts.append(
        "<table><thead><tr>"
        "<th style='width:14%'>Runner</th>"
        "<th style='width:40%'>Pass / Fail Distribution</th>"
        "<th style='text-align:right;width:8%'>Jobs</th>"
        "<th style='text-align:right;width:8%'>Failures</th>"
        "<th style='width:10%'>Fail Rate</th>"
        "<th>Top Failing Steps</th>"
        "</tr></thead><tbody>"
    )
    for rname, data in sorted(runner.items(), key=lambda item: _runner_sort_key(item[0])):
        total = data["total"]
        fail = data["failure"]
        success = total - fail
        frate = fail / total if total else 0
        srate = success / total if total else 0

        if frate > 0.3:
            name_cls = "badge badge-red"
        elif frate > 0.15:
            name_cls = "badge badge-orange"
        else:
            name_cls = "badge badge-green"

        hbar = "<div class='hbar-wrap'>"
        hbar += (
            "<div class='hbar-labels'>"
            f"<span>Pass {srate:.0%} ({success})</span>"
            f"<span>{total} total</span>"
            f"<span>Fail {frate:.0%} ({fail})</span>"
            "</div>"
        )
        hbar += "<div class='hbar'>"
        if total == 0:
            hbar += "<div class='hbar-zero'></div>"
        else:
            if srate > 0:
                hbar += f"<div class='hbar-pass' style='width:{srate * 100:.1f}%'></div>"
            if frate > 0:
                hbar += f"<div class='hbar-fail' style='width:{frate * 100:.1f}%'></div>"
        hbar += "</div></div>"

        step_html = ""
        if rname in rs:
            top3 = sorted(rs[rname].items(), key=lambda x: x[1], reverse=True)[:3]
            for sname, cnt in top3:
                variants = rsv.get(rname, {}).get(sname, {})
                var_items = "".join(
                    f"<li>{c} ({v})</li>"
                    for v, c in sorted(variants.items(), key=lambda x: x[1], reverse=True)[:3]
                )
                var_html = f"<ul class='var-list'>{var_items}</ul>" if var_items else ""
                step_html += f"<div style='margin-bottom:4px'><strong>{sname}</strong>: {cnt}{var_html}</div>"

        parts.append(
            "<tr>"
            f"<td><span class='{name_cls}'>{rname}</span></td>"
            f"<td>{hbar}</td>"
            f"<td style='text-align:right'>{total}</td>"
            f"<td style='text-align:right'><strong>{fail}</strong></td>"
            f"<td><span class='badge {name_cls}'>{frate:.1%}</span></td>"
            f"<td>{step_html}</td>"
            "</tr>"
        )
    parts.append("</tbody></table></div></div>")

    # ── Top failing runners table ─────────────────────────────────────────────
    if top_failing:
        parts.append("<div class='card'>")
        parts.append(
            "<div class='card-head'>"
            "<span class='card-title'>Top Failing Runners</span>"
            "<span class='card-sub'>Ranked by failure count</span>"
            "</div><div class='card-body'>"
        )
        parts.append(
            "<table><thead><tr>"
            "<th>#</th><th>Runner</th><th>Failures</th><th>Total Jobs</th>"
            "<th>Failure Rate</th><th>Top Failing Step</th>"
            "</tr></thead><tbody>"
        )
        for rank, (rname, data) in enumerate(top_failing, 1):
            total = data["total"]
            fail = data["failure"]
            rate = fail / total if total else 0
            badge = "badge-red" if rate > 0.3 else "badge-orange"
            top_step = ""
            if rname in rs:
                s_items = sorted(rs[rname].items(), key=lambda x: x[1], reverse=True)
                if s_items:
                    top_step = f"{s_items[0][0]} ({s_items[0][1]})"
            parts.append(
                "<tr>"
                f"<td style='color:var(--muted)'>{rank}</td>"
                f"<td><strong>{rname}</strong></td>"
                f"<td style='text-align:right'><strong>{fail}</strong></td>"
                f"<td style='text-align:right'>{total}</td>"
                f"<td><span class='badge {badge}'>{rate:.1%}</span></td>"
                f"<td style='color:var(--muted);font-size:12px'>{top_step}</td>"
                "</tr>"
            )
        parts.append("</tbody></table></div></div>")

    # ── Top failed steps table ────────────────────────────────────────────────
    if top_steps:
        parts.append("<div class='card'>")
        parts.append(
            "<div class='card-head'>"
            "<span class='card-title'>Top Failed Steps</span>"
            "<span class='card-sub'>Steps most frequently failing across all runners</span>"
            "</div><div class='card-body'>"
        )
        parts.append(
            "<table><thead><tr>"
            "<th>#</th><th>Step Name</th><th>Failures</th>"
            "<th>Category</th><th>Affected Runners</th><th>Trend</th>"
            "</tr></thead><tbody>"
        )
        for rank, (step, cnt) in enumerate(top_steps, 1):
            cat = classify_step(step)
            cat_badge = (
                "badge-orange"
                if cat == "infra"
                else ("badge-blue" if cat == "test" else "badge-gray")
            )
            affected = sum(1 for rn in rs if step in rs[rn])
            t = trends.get(step, {})
            if t.get("growth") == float("inf"):
                trend_cell = "<span class='badge badge-orange'>NEW</span>"
            elif t.get("growth", 0) > 1.0:
                trend_cell = f"<span class='badge badge-red'>+{t['growth']:.0%}</span>"
            elif t.get("growth", 0) < -0.5:
                trend_cell = "<span class='badge badge-green'>Improving</span>"
            else:
                trend_cell = "<span class='badge badge-gray'>Stable</span>"
            parts.append(
                "<tr>"
                f"<td style='color:var(--muted)'>{rank}</td>"
                f"<td><strong>{step}</strong></td>"
                f"<td style='text-align:right'><strong>{cnt}</strong></td>"
                f"<td><span class='badge {cat_badge}'>{cat}</span></td>"
                f"<td style='text-align:right'>{affected}</td>"
                f"<td>{trend_cell}</td>"
                "</tr>"
            )
        parts.append("</tbody></table></div></div>")

    # ── Runner event timeline ─────────────────────────────────────────────────
    parts.append("<div class='card'>")
    parts.append(
        "<div class='card-head'>"
        f"<span class='card-title'>Runner Event Timeline</span>"
        f"<span class='card-sub'>Last {display_days}d — click a point to open the job</span>"
        "</div><div class='card-body'>"
    )
    timeline_div = render_runner_timeline_interactive(events, display_days)
    if timeline_div:
        parts.append(timeline_div)
        parts.append(
            "<script>"
            "document.addEventListener('DOMContentLoaded',function(){"
            "var plots=document.querySelectorAll('.plotly-graph-div');"
            "for(var i=0;i<plots.length;i++){"
            "var gd=plots[i];"
            "var title=gd.layout&&gd.layout.title&&gd.layout.title.text?gd.layout.title.text:'';"
            "if(title.indexOf('Runner Event Timeline')===-1)continue;"
            "if(gd.__timelineClickBound)continue;"
            "gd.__timelineClickBound=true;"
            "gd.on('plotly_click',function(ev){"
            "if(!ev||!ev.points||!ev.points.length)return;"
            "var cd=ev.points[0].customdata||[];"
            "var target=cd[1]||cd[0]||'';"
            "if(target)window.open(target,'_blank','noopener,noreferrer');"
            "});"
            "}"
            "});</script>"
        )
    else:
        parts.append(
            "<p style='color:var(--muted)'>Install plotly and pandas to enable this chart.</p>"
        )
    parts.append("</div></div>")

    # ── Daily jobs chart ──────────────────────────────────────────────────────
    parts.append("<div class='card'>")
    parts.append(
        "<div class='card-head'>"
        f"<span class='card-title'>Jobs Per Day</span>"
        f"<span class='card-sub'>Last {display_days}d &nbsp;|&nbsp; "
        f"Total: {total_jobs_window} &nbsp;|&nbsp; "
        f"Avg: {avg_jobs_per_day:.1f}/day &nbsp;|&nbsp; "
        f"Active days: {active_days}</span>"
        "</div><div class='card-body'>"
    )
    daily_div = render_daily_jobs_interactive(daily, display_days)
    parts.append(
        daily_div
        if daily_div
        else "<p style='color:var(--muted)'>No daily chart data available.</p>"
    )
    parts.append("</div></div>")

    # ── Key insights & trends ─────────────────────────────────────────────────
    if trend_msgs:
        parts.append("<div class='card'>")
        parts.append(
            "<div class='card-head'>"
            "<span class='card-title'>Key Insights &amp; Failure Trends</span>"
            "<span class='card-sub'>Detected change patterns over the period</span>"
            "</div><div class='card-body'>"
        )
        parts.append("<ul class='trend-list'>")
        for msg in trend_msgs:
            if "SPIKE" in msg or "NEW" in msg:
                parts.append(f"<li class='trend-spike'>{msg}</li>")
            elif "Improving" in msg:
                parts.append(f"<li class='trend-good'>{msg}</li>")
            else:
                parts.append(f"<li>{msg}</li>")
        parts.append("</ul></div></div>")

    # ── Hourly pass/fail chart ────────────────────────────────────────────────
    parts.append("<div class='card'>")
    parts.append(
        "<div class='card-head'>"
        "<span class='card-title'>Hourly Pass vs Fail per Runner</span>"
        f"<span class='card-sub'>Top 6 runners by activity — last {display_days}d</span>"
        "</div><div class='card-body'>"
    )
    hourly_div = render_runner_hourly_interactive(rh, display_days)
    parts.append(
        hourly_div if hourly_div else "<p style='color:var(--muted)'>Install plotly to enable.</p>"
    )
    parts.append("</div></div>")

    # ── Step failure trends chart ─────────────────────────────────────────────
    parts.append("<div class='card'>")
    parts.append(
        "<div class='card-head'>"
        "<span class='card-title'>Step Failure Trends</span>"
        "<span class='card-sub'>Top 6 failing steps over time</span>"
        "</div><div class='card-body'>"
    )
    step_div = render_step_trends_interactive(sd)
    parts.append(
        step_div if step_div else "<p style='color:var(--muted)'>No step trend data available.</p>"
    )
    parts.append("</div></div>")

    # ── Runner failure trends chart ───────────────────────────────────────────
    parts.append("<div class='card'>")
    parts.append(
        "<div class='card-head'>"
        "<span class='card-title'>Runner Failure Trends</span>"
        "<span class='card-sub'>Top 6 runners by failure count over time</span>"
        "</div><div class='card-body'>"
    )
    runner_div = render_runner_trends_interactive(rd)
    parts.append(
        runner_div
        if runner_div
        else "<p style='color:var(--muted)'>No runner trend data available.</p>"
    )
    parts.append("</div></div>")

    # ── Variant-level failure breakdown ──────────────────────────────────────
    variant_rows = []
    for rname in sorted(rsv, key=_runner_sort_key):
        for step in sorted(rsv[rname], key=lambda s: sum(rsv[rname][s].values()), reverse=True)[
            :5
        ]:
            for variant, cnt in sorted(rsv[rname][step].items(), key=lambda x: x[1], reverse=True):
                variant_rows.append((rname, step, variant, cnt))
    if variant_rows:
        parts.append("<div class='card'>")
        parts.append(
            "<div class='card-head'>"
            "<span class='card-title'>Failure Breakdown by Variant</span>"
            "<span class='card-sub'>Runner / step / variant detail</span>"
            "</div><div class='card-body'>"
        )
        parts.append(
            "<table><thead><tr>"
            "<th>Runner</th><th>Step</th><th>Variant</th><th>Failures</th>"
            "</tr></thead><tbody>"
        )
        for rname, step, variant, cnt in variant_rows[:60]:
            parts.append(
                "<tr>"
                f"<td>{rname}</td>"
                f"<td>{step}</td>"
                f"<td><span class='badge badge-gray'>{variant}</span></td>"
                f"<td style='text-align:right'>{cnt}</td>"
                "</tr>"
            )
        if len(variant_rows) > 60:
            parts.append(
                f"<tr><td colspan='4' style='color:var(--muted);font-style:italic'>"
                f"... and {len(variant_rows) - 60} more rows</td></tr>"
            )
        parts.append("</tbody></table></div></div>")

    # ── Coverage metadata ─────────────────────────────────────────────────────
    parts.append("<div class='card'>")
    parts.append(
        "<div class='card-head'>"
        "<span class='card-title'>Coverage &amp; Scan Metadata</span>"
        "</div><div class='card-body'>"
    )
    meta_items = [
        ("Runs scanned", coverage.get("runs_scanned", 0)),
        ("Jobs fetched (all)", coverage.get("jobs_fetched", 0)),
        ("Self-hosted jobs", coverage.get("jobs_self_hosted", 0)),
        ("Jobs matched group", coverage.get("jobs_group_matched", 0)),
        ("Runner group", RUNNER_GROUP),
        ("Repository", f"{ORG}/{REPO}"),
        ("Look-back window", f"{days_back}d"),
        ("Timezone", LOCAL_TIMEZONE_LABEL),
        ("Generated at", last_updated),
    ]
    parts.append("<div class='meta-grid'>")
    for key, val in meta_items:
        parts.append(
            "<div class='meta-item'>"
            f"<span class='meta-key'>{key}</span>"
            f"<span class='meta-val'>{val}</span>"
            "</div>"
        )
    parts.append("</div></div></div>")

    parts.append("</div></body></html>")  # close .layout and body

    html = "".join(parts)
    path = _out("report.html")
    with open(path, "w", encoding="utf-8") as f:
        f.write(html)
    print(f"report.html -> {path}")


# ============================================================
# OUTPUT 2: summary.md  (GitHub Step Summary)
# ============================================================


def generate_summary_md(
    stats: tuple,
    days_back: int,
    unhealthy: list[dict],
    page_url: str = "",
) -> str:
    """Write summary.md and return its content as a string."""
    runner, daily, steps, sd, rs, _, rd, _, _, coverage = stats

    trends = compute_trends(sd)
    trend_msgs = summarize_trends(trends)
    report_date = datetime.now(LOCAL_TIMEZONE).strftime("%Y-%m-%d %H:%M %Z")

    total_jobs = sum(v.get("total", 0) for v in daily.values())
    total_failures = sum(v.get("failure", 0) for v in daily.values())
    frate = total_failures / total_jobs if total_jobs else 0.0
    active_runners = len(runner)

    lines = [
        f"# 🏃 Runner Health Report — {report_date}",
        "",
        f"> **Repo**: `{ORG}/{REPO}` &nbsp;|&nbsp; **Group**: `{RUNNER_GROUP}` &nbsp;|&nbsp; **Period**: last {days_back} day(s)",
        "",
        "## Summary",
        "",
        "| Metric | Value |",
        "|--------|-------|",
        f"| Runs scanned | {coverage.get('runs_scanned', 0)} |",
        f"| Jobs matched | {coverage.get('jobs_group_matched', 0)} |",
        f"| Active runners | {active_runners} |",
        f"| Total jobs | {total_jobs} |",
        f"| Total failures | {total_failures} |",
        f"| Overall failure rate | **{frate:.1%}** |",
        f"| Unhealthy runners | **{len(unhealthy)}** |",
        "",
    ]

    if unhealthy:
        lines += [
            "## ⚠️ Unhealthy Runners",
            "",
            "| Runner | Status | Reason | Failure Rate |",
            "|--------|--------|--------|-------------|",
        ]
        for u in unhealthy:
            lines.append(
                f"| `{u['name']}` | {u['status']} | {u['reason']} | {u['failure_rate']:.1%} |"
            )
        lines.append("")

    # Top failing runners
    top_failing = sorted(runner.items(), key=lambda x: x[1].get("failure", 0), reverse=True)[:10]
    if top_failing:
        lines += [
            "## 🔴 Top Failing Runners",
            "",
            "| Runner | Failures | Total | Failure Rate |",
            "|--------|----------|-------|-------------|",
        ]
        for rname, data in top_failing:
            if data["failure"] == 0:
                break
            lines.append(
                f"| `{rname}` | {data['failure']} | {data['total']} | {data['failure'] / data['total']:.1%} |"
            )
        lines.append("")

    # Top failed steps
    top_steps = sorted(steps.items(), key=lambda x: x[1], reverse=True)[:10]
    if top_steps:
        lines += [
            "## 🪜 Top Failed Steps",
            "",
            "| Step | Failures | Category |",
            "|------|----------|----------|",
        ]
        for step, cnt in top_steps:
            lines.append(f"| `{step}` | {cnt} | {classify_step(step)} |")
        lines.append("")

    if trend_msgs:
        lines += ["## 📈 Failure Trends", ""]
        for msg in trend_msgs:
            lines.append(f"- {msg}")
        lines.append("")

    if page_url:
        lines += ["---", f"📊 **[View Full Interactive Dashboard]({page_url})**", ""]

    md = "\n".join(lines)
    path = _out("summary.md")
    with open(path, "w", encoding="utf-8") as f:
        f.write(md)
    print(f"✅ summary.md      → {path}")
    return md


# ============================================================
# OUTPUT 3: email_body.html  (Email-safe, inline-CSS only)
# ============================================================


def generate_email_body(
    stats: tuple,
    days_back: int,
    unhealthy: list[dict],
    page_url: str = "",
) -> None:
    """Write the Outlook-safe email summary to email_body.html in OUTPUT_DIR."""
    runner, daily, steps, sd, rs, _, rd, _, _, coverage = stats

    trends = compute_trends(sd)
    trend_msgs = summarize_trends(trends)
    report_date = datetime.now(LOCAL_TIMEZONE).strftime("%Y-%m-%d %H:%M %Z")

    total_jobs = sum(v.get("total", 0) for v in daily.values())
    total_failures = sum(v.get("failure", 0) for v in daily.values())
    frate = total_failures / total_jobs if total_jobs else 0.0
    frate_color = "#c92a2a" if frate > 0.2 else ("#f76707" if frate > 0.1 else "#2b8a3e")

    top_failing = sorted(runner.items(), key=lambda x: x[1].get("failure", 0), reverse=True)[:10]
    top_steps = sorted(steps.items(), key=lambda x: x[1], reverse=True)[:10]

    def td(content, extra="") -> str:
        return f'<td style="padding:8px 12px;border:1px solid #dee2e6;{extra}">{content}</td>'

    def th(content) -> str:
        return f'<th style="padding:8px 12px;border:1px solid #dee2e6;background:#f1f3f5;font-weight:600;text-align:left;">{content}</th>'

    def section(title) -> str:
        return (
            f'<tr><td colspan="10" style="padding:16px 0 6px 0;">'
            f'<span style="font-size:15px;font-weight:700;color:#1a1a2e;">{title}</span>'
            f"</td></tr>"
        )

    html = (
        '<!DOCTYPE html><html><head><meta charset="UTF-8"></head>'
        '<body style="font-family:Arial,Helvetica,sans-serif;font-size:14px;color:#1a1a2e;background:#f8f9fa;margin:0;padding:0;">'
        '<table width="100%" cellpadding="0" cellspacing="0" style="background:#f8f9fa;padding:20px 0;">'
        '<tr><td align="center">'
        '<table width="620" cellpadding="0" cellspacing="0" style="background:#ffffff;border-radius:8px;overflow:hidden;box-shadow:0 2px 8px rgba(0,0,0,0.1);">'
        # Header
        '<tr><td style="background:#1a1a2e;padding:20px 24px;">'
        '<span style="color:#ffffff;font-size:20px;font-weight:700;">🏃 Runner Health Report</span><br>'
        f'<span style="color:#a0aec0;font-size:13px;">{report_date} &nbsp;|&nbsp; {ORG}/{REPO} &nbsp;|&nbsp; {RUNNER_GROUP}</span>'
        "</td></tr>"
        # Body
        '<tr><td style="padding:20px 24px;">'
        '<table width="100%" cellpadding="0" cellspacing="0">'
    )

    # KPI row
    html += "<tr>"
    kpi_items = [
        (str(coverage.get("runs_scanned", 0)), "Runs Scanned", "#1971c2"),
        (str(coverage.get("jobs_group_matched", 0)), "Jobs Matched", "#1971c2"),
        (f"{frate:.1%}", "Failure Rate", frate_color),
        (
            str(len(unhealthy)),
            "Unhealthy Runners",
            "#c92a2a" if unhealthy else "#2b8a3e",
        ),
    ]
    for val, label, color in kpi_items:
        html += (
            f'<td width="25%" style="padding:4px;">'
            f'<table width="100%" cellpadding="0" cellspacing="0" style="border:1px solid #dee2e6;border-top:4px solid {color};border-radius:6px;">'
            f'<tr><td style="padding:12px;text-align:center;">'
            f'<div style="font-size:22px;font-weight:700;color:{color};">{val}</div>'
            f'<div style="font-size:11px;color:#6c757d;margin-top:4px;">{label}</div>'
            f"</td></tr></table>"
            f"</td>"
        )
    html += "</tr>"
    html += '<tr><td colspan="4" style="padding:12px 0;"></td></tr>'

    # Unhealthy runners section
    if unhealthy:
        html += f'<tr><td colspan="4"><p style="font-size:15px;font-weight:700;color:#c92a2a;margin:8px 0 6px 0;">⚠️ Unhealthy Runners ({len(unhealthy)})</p></td></tr>'
        html += '<tr><td colspan="4"><table width="100%" cellpadding="0" cellspacing="0" style="border-collapse:collapse;">'
        html += f'<thead><tr>{th("Runner")}{th("Status")}{th("Reason")}{th("Fail Rate")}</tr></thead><tbody>'
        for u in unhealthy:
            status_icon = "🔴 offline" if u["status"] == "offline" else "🟡 " + u["status"]
            name_td = td("<strong>" + u["name"] + "</strong>")
            ufrate_pct = f"{u['failure_rate']:.1%}"
            rate_td = td('<strong style="color:#c92a2a;">' + ufrate_pct + "</strong>")
            html += "<tr>" + name_td + td(status_icon) + td(u["reason"]) + rate_td + "</tr>"
        html += "</tbody></table></td></tr>"
        html += '<tr><td colspan="4" style="padding:8px 0;"></td></tr>'

    # Top failing runners
    failing_rows = [(rn, d) for rn, d in top_failing if d.get("failure", 0) > 0]
    if failing_rows:
        html += '<tr><td colspan="4"><p style="font-size:15px;font-weight:700;color:#1a1a2e;margin:8px 0 6px 0;">🔴 Top Failing Runners</p></td></tr>'
        html += '<tr><td colspan="4"><table width="100%" cellpadding="0" cellspacing="0" style="border-collapse:collapse;">'
        html += f'<thead><tr>{th("Runner")}{th("Failures")}{th("Total")}{th("Fail Rate")}</tr></thead><tbody>'
        for rname, data in failing_rows[:8]:
            rate = data["failure"] / data["total"] if data["total"] else 0
            color = "#c92a2a" if rate > 0.3 else "#f76707"
            rate_pct = f"{rate:.1%}"
            fail_td = td(str(data["failure"]), "color:" + color + ";font-weight:600;")
            strong_rate_td = td('<strong style="color:' + color + ';">' + rate_pct + "</strong>")
            html += (
                "<tr>" + td(rname) + fail_td + td(str(data["total"])) + strong_rate_td + "</tr>"
            )
        html += "</tbody></table></td></tr>"
        html += '<tr><td colspan="4" style="padding:8px 0;"></td></tr>'

    # Top failed steps
    if top_steps:
        html += '<tr><td colspan="4"><p style="font-size:15px;font-weight:700;color:#1a1a2e;margin:8px 0 6px 0;">🪜 Top Failed Steps</p></td></tr>'
        html += '<tr><td colspan="4"><table width="100%" cellpadding="0" cellspacing="0" style="border-collapse:collapse;">'
        html += f'<thead><tr>{th("Step Name")}{th("Failures")}{th("Category")}</tr></thead><tbody>'
        for step, cnt in top_steps[:8]:
            html += (
                f"<tr>"
                f"{td(step)}"
                f"{td(str(cnt), 'font-weight:600;')}"
                f"{td(classify_step(step))}"
                f"</tr>"
            )
        html += "</tbody></table></td></tr>"
        html += '<tr><td colspan="4" style="padding:8px 0;"></td></tr>'

    # Trends
    if trend_msgs:
        html += '<tr><td colspan="4"><p style="font-size:15px;font-weight:700;color:#1a1a2e;margin:8px 0 6px 0;">📈 Failure Trends</p><ul style="margin:0 0 8px 18px;padding:0;">'
        for msg in trend_msgs:
            color = (
                "#c92a2a"
                if ("SPIKE" in msg or "NEW" in msg)
                else ("#2b8a3e" if "Improving" in msg else "#1a1a2e")
            )
            html += f'<li style="color:{color};margin-bottom:4px;">{msg}</li>'
        html += "</ul></td></tr>"

    # Dashboard link
    if page_url:
        html += (
            '<tr><td colspan="4" style="padding:12px 0 4px 0;">'
            f'<a href="{page_url}" style="display:inline-block;background:#1971c2;color:#fff;padding:10px 20px;border-radius:6px;text-decoration:none;font-weight:600;">'
            "📊 View Full Interactive Dashboard →"
            "</a>"
            "</td></tr>"
        )

    html += (
        "</table>"
        "</td></tr>"
        # Footer
        '<tr><td style="background:#f1f3f5;padding:12px 24px;text-align:center;">'
        '<span style="color:#6c757d;font-size:12px;">'
        f"Runner Health Monitor &nbsp;|&nbsp; {ORG}/{REPO} &nbsp;|&nbsp; Auto-generated {report_date}"
        "</span>"
        "</td></tr>"
        "</table>"
        "</td></tr>"
        "</table>"
        "</body></html>"
    )

    path = _out("email_body.html")
    with open(path, "w", encoding="utf-8") as f:
        f.write(html)
    print(f"✅ email_body.html → {path}")


# ============================================================
# OUTPUT 4: metrics.json  (Machine-readable snapshot)
# ============================================================


def generate_metrics_json(
    stats: tuple,
    days_back: int,
    unhealthy: list[dict],
    runner_inventory: list[dict],
) -> None:
    """Write a structured JSON snapshot to metrics.json in OUTPUT_DIR."""
    runner, daily, steps, sd, rs, _, rd, _, _, coverage = stats

    total_jobs = sum(v.get("total", 0) for v in daily.values())
    total_failures = sum(v.get("failure", 0) for v in daily.values())
    trends = compute_trends(sd)

    top_failing = [
        {
            "runner": rname,
            "total_jobs": data["total"],
            "failures": data["failure"],
            "failure_rate": (round(data["failure"] / data["total"], 4) if data["total"] else 0),
        }
        for rname, data in sorted(
            runner.items(), key=lambda x: x[1].get("failure", 0), reverse=True
        )
        if data.get("failure", 0) > 0
    ][:20]

    top_steps = [
        {"step": step, "failures": cnt, "category": classify_step(step)}
        for step, cnt in sorted(steps.items(), key=lambda x: x[1], reverse=True)
    ][:20]

    trend_list = [
        {
            "step": step,
            "recent": t["recent"],
            "previous": t["previous"],
            "growth": round(t["growth"], 4) if t["growth"] != float("inf") else None,
            "label": (
                "new"
                if t["growth"] == float("inf")
                else (
                    "spike"
                    if t["growth"] > 1.0
                    else ("improving" if t["growth"] < -0.5 else "stable")
                )
            ),
        }
        for step, t in sorted(
            trends.items(), key=lambda x: (x[1].get("growth", 0) or 0), reverse=True
        )
    ]

    payload = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "period_days": days_back,
        "repo": f"{ORG}/{REPO}",
        "runner_group": RUNNER_GROUP,
        "coverage": coverage,
        "summary": {
            "total_jobs": total_jobs,
            "total_failures": total_failures,
            "overall_failure_rate": (round(total_failures / total_jobs, 4) if total_jobs else 0),
            "active_runners": len(runner),
            "unhealthy_runners": len(unhealthy),
        },
        "runner_inventory": runner_inventory,
        "unhealthy_runners": unhealthy,
        "top_failing_runners": top_failing,
        "top_failed_steps": top_steps,
        "failure_trends": trend_list,
        "daily_stats": {day: {**data} for day, data in sorted(daily.items())},
    }

    path = _out("metrics.json")
    with open(path, "w", encoding="utf-8") as f:
        json.dump(payload, f, indent=2, default=str)
    print(f"✅ metrics.json    → {path}")


# ============================================================
# WRITE GITHUB STEP SUMMARY
# ============================================================


def write_step_summary(summary_md: str) -> None:
    """Append summary markdown to GITHUB_STEP_SUMMARY when running in Actions."""
    step_summary_file = os.getenv("GITHUB_STEP_SUMMARY")
    if step_summary_file:
        try:
            with open(step_summary_file, "a", encoding="utf-8") as f:
                f.write(summary_md)
            print("✅ Written to GITHUB_STEP_SUMMARY")
        except OSError as exc:
            print(f"Warning: could not write to GITHUB_STEP_SUMMARY — {exc}")


# ============================================================
# ENTRY POINT
# ============================================================


def report(days_back: int, max_runs: int | None = None, page_url: str = "") -> None:
    """Run the full monitoring pipeline and write all output files."""
    if not TOKEN:
        print("ERROR: GITHUB_TOKEN is not set.", file=sys.stderr)
        sys.exit(1)

    os.makedirs(OUTPUT_DIR, exist_ok=True)

    print(f"Config: {ORG}/{REPO} | group={RUNNER_GROUP} | days={days_back} | output={OUTPUT_DIR}")
    print(f"API: {BASE_URL}")

    runner_inventory = _run_with_spinner(get_runner_inventory, "Fetching runner inventory")

    stats = analyze(days_back, max_runs=max_runs)

    runner_stats = stats[0]
    unhealthy = identify_unhealthy(runner_stats, runner_inventory, FAIL_THRESHOLD)

    if unhealthy:
        print(
            f"\n⚠️  {len(unhealthy)} unhealthy runner(s): {', '.join(u['name'] for u in unhealthy)}"
        )
    else:
        print("\n✅ All runners healthy")

    print()
    generate_html(stats, days_back, unhealthy)
    summary_md = generate_summary_md(stats, days_back, unhealthy, page_url)
    generate_email_body(stats, days_back, unhealthy, page_url)
    generate_metrics_json(stats, days_back, unhealthy, runner_inventory)
    write_step_summary(summary_md)

    print(f"\n✅ All outputs written to: {OUTPUT_DIR}/")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Runner Health Monitor")
    parser.add_argument(
        "--days",
        type=int,
        default=DAYS_BACK,
        help="Days to look back (default: %(default)s)",
    )
    parser.add_argument(
        "--max-runs", type=int, default=MAX_RUNS, help="Cap on workflow runs to scan"
    )
    parser.add_argument(
        "--page-url",
        type=str,
        default=PAGES_URL,
        help="GitHub Pages URL embedded in email/summary",
    )
    parser.add_argument(
        "--output-dir",
        type=str,
        default=OUTPUT_DIR,
        help="Output directory (default: %(default)s)",
    )
    args = parser.parse_args()

    # Allow CLI to override module-level globals
    if args.output_dir != OUTPUT_DIR:
        OUTPUT_DIR = args.output_dir  # noqa: F811

    try:
        report(days_back=args.days, max_runs=args.max_runs, page_url=args.page_url)
    except requests.exceptions.HTTPError:
        sys.exit(1)
    except Exception as exc:
        print(f"\nError: {exc}", file=sys.stderr)
        sys.exit(1)
