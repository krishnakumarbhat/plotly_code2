#!/usr/bin/env python3
from __future__ import annotations

import argparse
import gzip
import os
import hashlib
import io
import json
import math
import sqlite3
import time
import urllib.error
import urllib.request
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

import h5py
import numpy as np
import plotly.graph_objects as go
from scipy.optimize import linear_sum_assignment
from scipy.spatial.distance import cdist

SENSOR_TOKENS = {"FL", "FR", "RL", "RR", "FC", "RC", "MRR", "SRR"}
JIRA_THRESHOLD = 60.0
LLAMA_SERVER_URL = os.environ.get("LLAMA_SERVER_BASE_URL", os.environ.get("RAG_LLAMA_URL", "http://127.0.0.1:8081")).rstrip("/")
RAG_INGEST_URL = (os.environ.get("RAG_SERVICE_URL", "http://127.0.0.1:5100").rstrip("/") + "/ingest")


def _open_hdf(path: Path) -> h5py.File:
    if path.name.lower().endswith(".gz"):
        with gzip.open(path, "rb") as gz:
            payload = gz.read()
        return h5py.File(io.BytesIO(payload), "r")
    return h5py.File(path, "r")


def _utcnow() -> str:
    return datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")


def list_hdf_files(input_dir: Path) -> List[Path]:
    return sorted(
        p for p in input_dir.iterdir()
        if p.is_file() and (p.name.lower().endswith(".h5") or p.name.lower().endswith(".h5.gz"))
    )


def filename_stem(path: Path) -> str:
    name = path.name
    for ext in (".h5.gz", ".h5"):
        if name.lower().endswith(ext):
            return name[: -len(ext)]
    return path.stem


def infer_sensor(stem: str) -> str:
    import re
    upper = stem.upper()
    parts = re.split(r"[^A-Z0-9]+", upper)
    for part in parts:
        if part in SENSOR_TOKENS:
            return part
    return "UNKNOWN"


def is_resim(stem: str) -> bool:
    import re
    upper = stem.upper()
    return bool(re.search(r"(^|[-_])R(R)?[0-9]+($|[-_])", upper)) or bool(
        re.search(r"(^|[-_])OUTPUT($|[-_])", upper)
    )


def normalize_key(stem: str) -> str:
    import re
    key = re.sub(r"(?i)(^|[-_])r[0-9a-z]*($|[-_])", r"\1", stem)
    key = re.sub(r"(?i)(^|[-_])(input|output|vehicle|veh)($|[-_])", r"\1", key)
    key = re.sub(r"[-_]{2,}", "_", key).strip("_-")
    return key


def auto_pair_files(input_dir: Path) -> List[dict]:
    files = list_hdf_files(input_dir)
    buckets: Dict[tuple, Dict[str, List[Path]]] = {}
    for fp in files:
        stem = filename_stem(fp)
        sensor = infer_sensor(stem)
        key = (sensor, normalize_key(stem))
        g = buckets.setdefault(key, {"veh": [], "resim": []})
        (g["resim"] if is_resim(stem) else g["veh"]).append(fp)
    pairs = []
    for (sensor, base_key), g in sorted(buckets.items(), key=lambda x: (x[0][0], x[0][1])):
        if not g["veh"] or not g["resim"]:
            continue
        pairs.append({
            "sensor": sensor,
            "base_key": base_key,
            "veh": sorted(g["veh"])[0],
            "resim": sorted(g["resim"])[0],
        })
    return pairs


def sensors_in_file(path: Path) -> List[str]:
    with _open_hdf(path) as h:
        return sorted(set(k.upper() for k in h.keys() if k.upper() in SENSOR_TOKENS))


def _safe_read(h: h5py.File, paths: list) -> Optional[np.ndarray]:
    for p in paths:
        if p in h:
            try:
                return np.asarray(h[p][...])
            except Exception:
                pass
    return None


def load_sensor_data(path: Path, sensor: str) -> Optional[dict]:
    with _open_hdf(path) as h:
        af = f"{sensor}/DETECTION_STREAM/AF_Det"
        if af not in h:
            return None
        scan = _safe_read(h, [
            f"{sensor}/DETECTION_STREAM/Stream_Hdr/scan_index",
            f"{sensor}/RDD_STREAM/Look_Data/scan_index",
        ])
        ran = _safe_read(h, [f"{af}/ran", f"{af}/range"])
        vel = _safe_read(h, [f"{af}/vel", f"{af}/velocity"])
        az = _safe_read(h, [f"{af}/azimuth", f"{af}/phi"])
        rcs = _safe_read(h, [f"{af}/rcs"])
        snr = _safe_read(h, [f"{af}/snr"])
        if ran is None or vel is None or az is None or scan is None:
            return None
        return {"scan_index": scan, "ran": ran, "vel": vel, "azimuth": az, "rcs": rcs, "snr": snr}


def _to_long(data: dict):
    import pandas as pd
    scan = np.ravel(data["scan_index"])
    ran = np.asarray(data["ran"], dtype=np.float64)
    vel = np.asarray(data["vel"], dtype=np.float64)
    az = np.asarray(data["azimuth"], dtype=np.float64)
    if ran.ndim == 2:
        n = min(ran.shape[0], vel.shape[0], az.shape[0])
        rows = []
        for i in range(n):
            si = int(scan[i]) if i < len(scan) else i
            for j in range(ran.shape[1]):
                if not (np.isfinite(ran[i, j]) and np.isfinite(vel[i, j]) and np.isfinite(az[i, j])):
                    continue
                rows.append({"scan_index": si, "ran": ran[i, j], "vel": vel[i, j], "azimuth": az[i, j]})
        if not rows:
            return None
        return pd.DataFrame(rows)
    n = min(len(scan), len(ran), len(vel), len(az))
    return pd.DataFrame({
        "scan_index": scan[:n].astype(np.int64),
        "ran": ran[:n],
        "vel": vel[:n],
        "azimuth": az[:n],
    })


def match_points(veh: dict, resim: dict, gate: float = 1.0) -> dict:
    import pandas as pd
    veh_df = _to_long(veh)
    resim_df = _to_long(resim)
    if veh_df is None or resim_df is None:
        return {"matched": 0, "total_veh": 0, "total_resim": 0, "recall": 0.0, "precision": 0.0, "f1": 0.0}

    veh_g = veh_df.groupby("scan_index").indices
    resim_g = resim_df.groupby("scan_index").indices
    common = sorted(set(veh_g.keys()) & set(resim_g.keys()))
    matched = 0
    total_veh = len(veh_df)
    total_resim = len(resim_df)

    for scan in common:
        vi = np.asarray(veh_g[scan], dtype=np.int64)
        ri = np.asarray(resim_g[scan], dtype=np.int64)
        if vi.size == 0 or ri.size == 0:
            continue
        vp = veh_df.loc[vi, ["ran", "vel", "azimuth"]].to_numpy(dtype=np.float64)
        rp = resim_df.loc[ri, ["ran", "vel", "azimuth"]].to_numpy(dtype=np.float64)
        d = cdist(vp, rp, metric="euclidean")
        row, col = linear_sum_assignment(d)
        ok = d[row, col] <= gate
        matched += int(np.count_nonzero(ok))

    recall = matched / total_veh if total_veh > 0 else 0.0
    precision = matched / total_resim if total_resim > 0 else 0.0
    f1 = 2 * recall * precision / (recall + precision) if (recall + precision) > 0 else 0.0
    return {"matched": matched, "total_veh": total_veh, "total_resim": total_resim, "recall": recall, "precision": precision, "f1": f1}


def generate_html(pair: dict, sensor_results: List[dict], kpi_overall: dict) -> str:
    sensor_divs = []
    for idx, sr in enumerate(sensor_results):
        k = sr["kpi"]
        rows = "".join(
            f"<tr><td>{key}</td><td>{val:.4f}</td></tr>"
            for key, val in k.items() if isinstance(val, (int, float)) and not (isinstance(val, float) and math.isnan(val))
        )
        fig = go.Figure()
        if sr.get("data") and sr["data"].get("ran") is not None and sr["data"].get("vel") is not None:
            r = np.asarray(sr["data"]["ran"]).ravel()[:5000]
            v = np.asarray(sr["data"]["vel"]).ravel()[:5000]
            fig.add_trace(go.Scatter(x=r, y=v, mode="markers", marker=dict(size=2, color="#3b82f6"), name=sr["sensor"]))
            fig.update_layout(title=f"{sr['sensor']} Range vs Velocity", xaxis_title="Range", yaxis_title="Velocity", template="plotly_dark")
        plot_div = fig.to_html(full_html=False, include_plotlyjs="cdn" if idx == 0 else False)
        sensor_divs.append(f"""
        <div class="panel">
          <h2>{sr['sensor']}</h2>
          <table><tbody>{rows}</tbody></table>
          {plot_div}
        </div>""")

    overall_rows = "".join(
        f"<tr><td>{key}</td><td>{val:.4f}</td></tr>"
        for key, val in kpi_overall.items() if isinstance(val, (int, float)) and not (isinstance(val, float) and math.isnan(val))
    )
    return f"""<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8"/>
  <meta name="viewport" content="width=device-width,initial-scale=1"/>
  <title>CAN KPI Report - {pair['base_key']}</title>
  <style>
    body {{ font-family:Arial,sans-serif; margin:20px; background:#0f172a; color:#e2e8f0; }}
    .panel {{ background:#111827; border:1px solid #334155; border-radius:10px; padding:14px; margin-bottom:18px; }}
    table {{ border-collapse:collapse; width:100%; }}
    th,td {{ border:1px solid #334155; padding:8px; text-align:left; }}
    th {{ background:#1f2937; }}
    h1,h2 {{ color:#f8fafc; }}
  </style>
</head>
<body>
  <h1>CAN KPI Report: {pair['base_key']}</h1>
  <div class="panel">
    <p><b>Vehicle:</b> {pair['veh'].name}</p>
    <p><b>Resimulation:</b> {pair['resim'].name}</p>
  </div>
  <div class="panel">
    <h2>Overall KPI</h2>
    <table><tbody>{overall_rows}</tbody></table>
  </div>
  {''.join(sensor_divs)}
</body>
</html>"""


# ---------------------------------------------------------------------------
# SQLite store for processing metadata + Jira tickets
# ---------------------------------------------------------------------------

class KpiRagStore:
    def __init__(self, db_path: Path):
        self.db_path = db_path
        db_path.parent.mkdir(parents=True, exist_ok=True)
        self._conn = sqlite3.connect(str(db_path))
        self._conn.row_factory = sqlite3.Row
        self._conn.execute("PRAGMA journal_mode=WAL")
        self._conn.execute("PRAGMA foreign_keys=ON")
        self._init_schema()

    def _init_schema(self):
        self._conn.executescript("""
            CREATE TABLE IF NOT EXISTS kpi_index (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                base_key TEXT NOT NULL,
                sensor TEXT,
                veh_path TEXT,
                resim_path TEXT,
                recall REAL,
                precision REAL,
                f1 REAL,
                matched INTEGER,
                total_veh INTEGER,
                total_resim INTEGER,
                html_path TEXT,
                rag_ingested INTEGER DEFAULT 0,
                content_hash TEXT,
                processed_at TEXT NOT NULL
            );
            CREATE TABLE IF NOT EXISTS jira_tickets (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                ticket_key TEXT NOT NULL,
                base_key TEXT NOT NULL,
                accuracy REAL,
                summary TEXT,
                description TEXT,
                hdf_path TEXT,
                log_path TEXT,
                created_at TEXT NOT NULL
            );
            CREATE INDEX IF NOT EXISTS idx_kpi_base_key ON kpi_index(base_key);
            CREATE INDEX IF NOT EXISTS idx_jira_base_key ON jira_tickets(base_key);
        """)
        self._conn.commit()

    def is_processed(self, base_key: str, sensor: str, content_hash: str) -> bool:
        row = self._conn.execute(
            "SELECT 1 FROM kpi_index WHERE base_key=? AND sensor=? AND content_hash=?",
            (base_key, sensor, content_hash),
        ).fetchone()
        return row is not None

    def insert_kpi(self, base_key: str, sensor: str, veh_path: str, resim_path: str,
                    recall: float, precision: float, f1: float, matched: int,
                    total_veh: int, total_resim: int, html_path: str,
                    content_hash: str, rag_ingested: int = 0) -> int:
        cur = self._conn.execute("""
            INSERT INTO kpi_index
                (base_key, sensor, veh_path, resim_path, recall, precision, f1,
                 matched, total_veh, total_resim, html_path, rag_ingested,
                 content_hash, processed_at)
            VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)
        """, (base_key, sensor, veh_path, resim_path, recall, precision, f1,
              matched, total_veh, total_resim, html_path, 1 if rag_ingested else 0,
              content_hash, _utcnow()))
        self._conn.commit()
        return int(cur.lastrowid)

    def insert_jira_ticket(self, ticket_key: str, base_key: str, accuracy: float,
                            summary: str, description: str, hdf_path: str, log_path: str) -> int:
        cur = self._conn.execute("""
            INSERT INTO jira_tickets
                (ticket_key, base_key, accuracy, summary, description, hdf_path, log_path, created_at)
            VALUES (?,?,?,?,?,?,?,?)
        """, (ticket_key, base_key, accuracy, summary, description, hdf_path, log_path, _utcnow()))
        self._conn.commit()
        return int(cur.lastrowid)

    def get_pending_rag_ingest(self) -> List[dict]:
        rows = self._conn.execute(
            "SELECT * FROM kpi_index WHERE rag_ingested=0 ORDER BY id"
        ).fetchall()
        return [dict(r) for r in rows]

    def mark_rag_ingested(self, kpi_id: int):
        self._conn.execute("UPDATE kpi_index SET rag_ingested=1 WHERE id=?", (kpi_id,))
        self._conn.commit()

    def summary(self) -> dict:
        kpi_count = self._conn.execute("SELECT COUNT(*) FROM kpi_index").fetchone()[0]
        jira_count = self._conn.execute("SELECT COUNT(*) FROM jira_tickets").fetchone()[0]
        return {"kpi_processed": kpi_count, "jira_tickets": jira_count}

    def close(self):
        self._conn.close()


# ---------------------------------------------------------------------------
# RAG ingest
# ---------------------------------------------------------------------------

def _rag_ingest(html_dir: Path, rag_url: str) -> bool:
    payload = json.dumps({"html_root": str(html_dir.resolve()), "reset_index": False}).encode()
    req = urllib.request.Request(
        rag_url,
        data=payload,
        headers={"Content-Type": "application/json"},
    )
    try:
        with urllib.request.urlopen(req, timeout=300) as resp:
            body = json.loads(resp.read().decode())
            print(f"  RAG ingest OK: {body.get('status', 'done')}")
            return True
    except (urllib.error.URLError, urllib.error.HTTPError, OSError, json.JSONDecodeError) as e:
        print(f"  RAG ingest failed (will retry later): {e}")
        return False


# ---------------------------------------------------------------------------
# Jira ticket with AI HDF5 debug analysis
# ---------------------------------------------------------------------------

def _call_llm(prompt: str, llama_url: str) -> str:
    payload = json.dumps({
        "model": "local",
        "messages": [
            {
                "role": "system",
                "content": (
                    "You are a radar/KPI debug analyst. Analyze the HDF5 data summary "
                    "and identify root causes for low accuracy. Be concise and technical."
                ),
            },
            {"role": "user", "content": prompt},
        ],
        "temperature": 0.1,
        "max_tokens": 512,
        "stream": False,
    }).encode()
    req = urllib.request.Request(
        f"{llama_url}/v1/chat/completions",
        data=payload,
        headers={"Content-Type": "application/json"},
    )
    try:
        with urllib.request.urlopen(req, timeout=120) as resp:
            body = json.loads(resp.read().decode())
            choices = body.get("choices") or []
            if choices:
                msg = choices[0].get("message") or {}
                return (msg.get("content") or "").strip()
    except Exception as e:
        print(f"  LLM call failed: {e}")
    return ""


def _summarize_hdf5(hdf_path: str) -> str:
    import numpy as np
    path = Path(hdf_path)
    if not path.exists():
        return f"HDF5 not found: {hdf_path}"
    try:
        f = h5py.File(str(path), "r")
    except Exception as e:
        return f"Cannot open HDF5: {e}"
    lines = [f"File: {path.name}", f"Size: {path.stat().st_size / 1024 / 1024:.1f} MB", ""]

    def walk(g, depth=0):
        indent = "  " * depth
        for key in g:
            try:
                item = g[key]
            except Exception:
                continue
            if isinstance(item, h5py.Group):
                lines.append(f"{indent}[{key}]")
                walk(item, depth + 1)
            elif isinstance(item, h5py.Dataset):
                shape = str(item.shape) if item.shape is not None else "scalar"
                lines.append(f"{indent}{key}: shape={shape}")
                if item.ndim == 1 and 0 < item.size <= 12:
                    try:
                        vals = item[:]
                        if np.issubdtype(item.dtype, np.number):
                            lines.append(f"{indent}  vals={vals.tolist()}")
                    except Exception:
                        pass
    try:
        walk(f)
    except Exception as e:
        lines.append(f"(walk error: {e})")
    f.close()
    return "\n".join(lines[:80])


def _create_jira_ticket(accuracy: float, base_key: str, hdf_path: str, log_path: str,
                         llama_url: str, jira_config: dict) -> Optional[str]:
    summary_text = _summarize_hdf5(hdf_path)
    if summary_text:
        prompt = (
            f"A CAN KPI run for {base_key} achieved {accuracy:.1f}% accuracy.\n\n"
            f"HDF5 summary:\n{summary_text}\n\n"
            f"Log: {log_path}\n\n"
            "Analyze possible causes: empty groups, low scan count, abnormal ranges, "
            "missing sensors, alignment issues. Be specific."
        )
        debug_analysis = _call_llm(prompt, llama_url)
    else:
        debug_analysis = ""

    now = _utcnow()
    description = (
        f"KPI Accuracy Alert\n\n"
        f"Accuracy: {accuracy:.1f}%\n"
        f"Log: {base_key}\n"
        f"HDF: {hdf_path}\n"
        f"Generated: {now}\n\n"
    )
    if debug_analysis:
        description += f"AI Debug Analysis:\n{debug_analysis}\n\n"
    description += "Accuracy below 60% threshold. Investigate."

    jira_url = jira_config.get("url", "").rstrip("/")
    if not jira_url:
        print("  Jira not configured (no JIRA_URL)")
        return None

    token = jira_config.get("token", "")
    user = jira_config.get("user", "")
    api_token = jira_config.get("api_token", "")
    project = jira_config.get("project", "")

    if not (jira_url and (token or (user and api_token))):
        print("  Jira not configured (missing auth)")
        return None

    headers = {"Content-Type": "application/json"}
    if token:
        headers["Authorization"] = f"Basic {token}"
    elif user and api_token:
        import base64
        headers["Authorization"] = f"Basic {base64.b64encode(f'{user}:{api_token}'.encode()).decode()}"

    payload = json.dumps({
        "fields": {
            "project": {"key": project},
            "summary": f"[KPI Alert] Low accuracy: {accuracy:.1f}% - {base_key}",
            "description": description,
            "issuetype": {"name": "Task"},
        }
    }).encode()

    req = urllib.request.Request(
        f"{jira_url}/rest/api/2/issue",
        data=payload,
        headers=headers,
    )
    try:
        with urllib.request.urlopen(req, timeout=30) as resp:
            body = json.loads(resp.read().decode())
            ticket_key = body.get("key", "")
            print(f"  Jira ticket created: {ticket_key}")
            return ticket_key
    except Exception as e:
        print(f"  Jira ticket creation failed: {e}")
        return None


# ---------------------------------------------------------------------------
# Main processing
# ---------------------------------------------------------------------------

def process_log(pair: dict, output_dir: Path, gate: float, store: KpiRagStore,
                llama_url: str, jira_config: dict) -> Tuple[bool, float, dict]:
    sensors = sorted(set(sensors_in_file(pair["veh"])) & set(sensors_in_file(pair["resim"])))
    if not sensors:
        sensors = [pair["sensor"]] if pair["sensor"] != "UNKNOWN" else ["UNKNOWN"]

    results = []
    total_matched = 0
    total_veh = 0
    total_resim = 0

    for sensor in sensors:
        veh = load_sensor_data(pair["veh"], sensor)
        resim = load_sensor_data(pair["resim"], sensor)
        if veh is None or resim is None:
            continue
        kpi = match_points(veh, resim, gate=gate)
        total_matched += kpi["matched"]
        total_veh += kpi["total_veh"]
        total_resim += kpi["total_resim"]
        results.append({"sensor": sensor, "kpi": kpi, "data": veh})

    if not results:
        return False, 0.0, {}

    overall = {
        "matched": total_matched,
        "total_veh": total_veh,
        "total_resim": total_resim,
        "recall": total_matched / total_veh if total_veh > 0 else 0.0,
        "precision": total_matched / total_resim if total_resim > 0 else 0.0,
        "f1": 2 * total_matched / (total_veh + total_resim) if (total_veh + total_resim) > 0 else 0.0,
    }

    html = generate_html(pair, results, overall)
    out_name = f"{pair['base_key']}_can_kpi_report.html"
    out_path = output_dir / out_name
    output_dir.mkdir(parents=True, exist_ok=True)
    out_path.write_text(html, encoding="utf-8")

    content_hash = hashlib.sha256(html.encode()).hexdigest()[:16]

    for sr in results:
        if store.is_processed(pair["base_key"], sr["sensor"], content_hash):
            print(f"  already processed: {pair['base_key']}/{sr['sensor']}")
            continue
        store.insert_kpi(
            base_key=pair["base_key"],
            sensor=sr["sensor"],
            veh_path=str(pair["veh"]),
            resim_path=str(pair["resim"]),
            recall=sr["kpi"]["recall"],
            precision=sr["kpi"]["precision"],
            f1=sr["kpi"]["f1"],
            matched=sr["kpi"]["matched"],
            total_veh=sr["kpi"]["total_veh"],
            total_resim=sr["kpi"]["total_resim"],
            html_path=str(out_path),
            content_hash=content_hash,
            rag_ingested=0,
        )

    accuracy_pct = overall["recall"] * 100.0
    if accuracy_pct < JIRA_THRESHOLD:
        ticket_key = _create_jira_ticket(
            accuracy=accuracy_pct,
            base_key=pair["base_key"],
            hdf_path=str(pair["veh"]),
            log_path=str(pair["veh"]),
            llama_url=llama_url,
            jira_config=jira_config,
        )
        if ticket_key:
            store.insert_jira_ticket(
                ticket_key=ticket_key,
                base_key=pair["base_key"],
                accuracy=accuracy_pct,
                summary=f"Low accuracy {accuracy_pct:.1f}% - {pair['base_key']}",
                description=f"Auto-created from KPI RAG bridge.",
                hdf_path=str(pair["veh"]),
                log_path=str(pair["veh"]),
            )

    return True, accuracy_pct, overall


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="CAN KPI RAG bridge: generate per-log HTML, store in RAG vector DB + SQLite"
    )
    p.add_argument("input_dir", type=Path, help="Directory with .h5/.h5.gz files")
    p.add_argument("--output-dir", type=Path, default=None, help="Output dir (default: input_dir/kpi_rag_reports)")
    p.add_argument("--gate", type=float, default=1.0, help="Matching gate threshold")
    p.add_argument("--rag-url", type=str, default=RAG_INGEST_URL, help="RAG ingest endpoint")
    p.add_argument("--llama-url", type=str, default=LLAMA_SERVER_URL, help="llama-server base URL")
    p.add_argument("--jira-url", type=str, default="", help="Jira base URL (optional)")
    p.add_argument("--jira-token", type=str, default="", help="Jira PAT or base64 user:token")
    p.add_argument("--jira-user", type=str, default="", help="Jira username")
    p.add_argument("--jira-api-token", type=str, default="", help="Jira API token")
    p.add_argument("--jira-project", type=str, default="", help="Jira project key")
    p.add_argument("--max-pairs", type=int, default=0, help="Cap on pairs (0=all)")
    return p.parse_args()


def main() -> int:
    args = parse_args()
    start = time.time()

    if not args.input_dir.exists():
        print(f"Input dir not found: {args.input_dir}")
        return 1

    output_dir = args.output_dir or (args.input_dir / "kpi_rag_reports")
    db_path = output_dir / "kpi_rag.db"
    store = KpiRagStore(db_path)

    jira_config = {
        "url": args.jira_url,
        "token": args.jira_token,
        "user": args.jira_user,
        "api_token": args.jira_api_token,
        "project": args.jira_project,
    }

    pairs = auto_pair_files(args.input_dir)
    if not pairs:
        print("No HDF5 pairs found")
        store.close()
        return 1
    if args.max_pairs > 0:
        pairs = pairs[: args.max_pairs]

    print(f"Found {len(pairs)} pairs. DB: {db_path}")
    success = 0
    for i, pair in enumerate(pairs, 1):
        print(f"  [{i}/{len(pairs)}] {pair['base_key']} ({pair['sensor']})...", end=" ", flush=True)
        try:
            ok, acc, overall = process_log(pair, output_dir, args.gate, store, args.llama_url, jira_config)
            if ok:
                print(f"OK recall={acc:.1f}%")
                success += 1
            else:
                print("SKIP (no sensor data)")
        except Exception as e:
            print(f"FAIL: {e}")
            import traceback
            traceback.print_exc()

    # RAG ingest: compulsory for all generated HTML
    html_files = list(output_dir.glob("*.html"))
    if html_files:
        print(f"Ingesting {len(html_files)} HTML reports into RAG vector DB...")
        ok = _rag_ingest(output_dir, args.rag_url)
        if ok:
            for row in store.get_pending_rag_ingest():
                store.mark_rag_ingested(row["id"])

    summary = store.summary()
    elapsed = time.time() - start
    print(f"Done: {success}/{len(pairs)} in {elapsed:.1f}s")
    print(f"  KPI records: {summary['kpi_processed']}, Jira tickets: {summary['jira_tickets']}")
    print(f"  SQLite DB: {db_path}")

    store.close()
    return 0 if success > 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
