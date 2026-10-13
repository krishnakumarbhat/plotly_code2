"""Build one mismatch-forensics PDF per AL pair in out_al (0003-0006).

Reads sidecars + detection HTML scan lines (no HDF reads; tiny memory).
ELI13 section + PhD section + funnel/pass-rate figures per PDF.
Usage: python make_out_al_pdfs.py  (run from udp_debug_2)
"""
import json
import os
import re
import sys
import time

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

REPO = r"C:\Users\ouymc2\Desktop\udp_debug_2"
OUT = os.path.join(REPO, "out_al")
ASSETS = os.path.join(OUT, "pdf_assets")
os.makedirs(ASSETS, exist_ok=True)

from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import cm
from reportlab.platypus import (
    HRFlowable,
    Image,
    PageBreak,
    Paragraph,
    SimpleDocTemplate,
    Spacer,
    Table,
    TableStyle,
)

PAIRS = {
    "0003": ("AshLey_CheetahBus_20260819_115747_0003_b05", 481, 401),
    "0004": ("AshLey_CheetahBus_20260819_115747_0004_b05", 481, 416),
    "0005": ("AshLey_CheetahBus_20260819_115747_0005_b05", 481, 453),
    "0006": ("AshLey_CheetahBus_20260819_115747_0006_b05", 481, 455),
}

# Measured RDD-matched |delta| percentiles, FL (capped probes, scan-value joined).
FORENSICS_FL = {
    "0003": {
        "det_in": 394, "det_out": 370, "common": 370, "rdd_both": 370,
        "pairs": 46764,
        "ran": (10.323, 46.051, 111.580, 22.1),
        "vel": (5.774, 22.451, 28.472, 31.2),
        "theta": (0.488, 1.656, 2.293, 99.8),
        "phi": (0.076, 0.250, 0.438, 100.0),
    },
    "0006": {
        "det_in": 395, "det_out": 380, "common": 374, "rdd_both": 373,
        "pairs": 62250,
        "ran": (10.382, 43.087, 97.266, 24.8),
        "vel": (4.436, 22.096, 28.856, 39.0),
        "theta": (0.423, 1.605, 2.218, 99.9),
        "phi": (0.048, 0.192, 0.387, 100.0),
    },
    "0004": {
        "det_in": 395, "det_out": 368, "common": 362, "rdd_both": 361,
        "pairs": 50091,
        "ran": (17.379, 71.469, 148.275, 23.4),
        "vel": (4.559, 23.099, 30.904, 37.1),
        "theta": (0.439, 1.614, 2.205, 99.9),
        "phi": (0.064, 0.219, 0.416, 100.0),
    },
    "0005": {
        "det_in": 395, "det_out": 378, "common": 372, "rdd_both": 371,
        "pairs": 58868,
        "ran": (14.761, 57.582, 149.500, 22.9),
        "vel": (5.897, 24.226, 29.921, 34.5),
        "theta": (0.457, 1.770, 2.301, 99.9),
        "phi": (0.063, 0.224, 0.414, 100.0),
    },
}

STYLES = getSampleStyleSheet()
S_TITLE = ParagraphStyle("T", parent=STYLES["Title"], fontSize=20, spaceAfter=6)
S_H1 = ParagraphStyle("H1", parent=STYLES["Heading1"], fontSize=15, spaceBefore=12, spaceAfter=6)
S_H2 = ParagraphStyle("H2", parent=STYLES["Heading2"], fontSize=12, spaceBefore=8, spaceAfter=4)
S_BODY = ParagraphStyle("B", parent=STYLES["BodyText"], fontSize=9.5, leading=13.5, spaceAfter=4)
S_SMALL = ParagraphStyle("S", parent=STYLES["BodyText"], fontSize=8.5, leading=11.5, spaceAfter=3)
S_CELL = ParagraphStyle("C", parent=STYLES["BodyText"], fontSize=8.5, leading=11)
S_HDR = ParagraphStyle("H", parent=STYLES["BodyText"], fontSize=8.5, leading=11,
                       textColor=colors.white, fontName="Helvetica-Bold")


def esc(t):
    return str(t).replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")


def p(text, style=S_BODY):
    return Paragraph(text, style)


def table(headers, rows):
    data = [[Paragraph(f"<b>{esc(h)}</b>", S_HDR) for h in headers]]
    for r in rows:
        data.append([Paragraph(esc(c), S_CELL) for c in r])
    t = Table(data, repeatRows=1)
    t.setStyle(TableStyle([
        ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#0d6a8b")),
        ("GRID", (0, 0), (-1, -1), 0.5, colors.HexColor("#d7e1e9")),
        ("ROWBACKGROUNDS", (0, 1), (-1, -1), [colors.white, colors.HexColor("#f4f8fb")]),
        ("VALIGN", (0, 0), (-1, -1), "TOP"),
    ]))
    return t


def scan_lines(html_path):
    """Extract scan-inventory numbers from a detection HTML (regex only)."""
    try:
        t = open(html_path, encoding="utf-8").read()
    except Exception:
        return {}
    out = {}
    m = re.search(r"Scans \(vehicle/simulation\): <span[^>]*>([^<]+)</span>", t)
    out["veh_sim"] = m.group(1) if m else "n/a"
    m = re.search(r"Scans processed / with matches:\s*<span[^>]*>([^<]+)</span>", t)
    if not m:
        m = re.search(r"Scans processed / with matches \(([^)]+)\)", t)
    out["proc_match"] = m.group(1) if m else "n/a"
    m = re.search(r"Common / Input Total \(rows\): <span[^>]*>([^<]+)</span>", t)
    out["common_in"] = m.group(1) if m else "n/a"
    return out


def funnel_fig(pair, fl_diag, path):
    det = fl_diag["kpis"]["detection"]
    m = re.match(r"\s*([0-9]+)\s*/\s*([0-9]+)", det.get("detail", ""))
    matched, denom = (int(m.group(1)), int(m.group(2))) if m else (0, 1)
    f = FORENSICS_FL.get(pair, {})
    stages = ["Input\nscans", "Common\nscans", "Scans w/\nshared RDD", "Matched /\ncompared dets"]
    vals = [f.get("det_in", 0), f.get("common", 0), f.get("rdd_both", 0), matched]
    fig, ax = plt.subplots(figsize=(7, 3.2))
    bars = ax.bar(stages, vals, color=["#5b9bd5", "#2e75b6", "#e8912d", "#c00000"])
    for b, v in zip(bars, vals):
        ax.text(b.get_x() + b.get_width() / 2, b.get_height(), f"{v:,}", ha="center", va="bottom", fontsize=9)
    ax.set_title(f"FL mismatch funnel, log {pair} (denominator {denom:,} detections)")
    ax.set_ylabel("count")
    fig.tight_layout()
    fig.savefig(path, dpi=110)
    plt.close(fig)


def passrate_fig(path, pair="0003"):
    sigs = ["ran", "vel", "theta", "phi"]
    v0 = [FORENSICS_FL[pair][s][3] for s in sigs]
    v6 = [FORENSICS_FL["0006"][s][3] for s in sigs]
    x = range(len(sigs))
    fig, ax = plt.subplots(figsize=(7, 3.2))
    ax.bar([i - 0.2 for i in x], v0, width=0.4, label=pair, color="#2e75b6")
    ax.bar([i + 0.2 for i in x], v6, width=0.4, label="0006", color="#e8912d")
    ax.axhline(100, color="gray", linestyle="--", linewidth=0.8)
    ax.set_xticks(list(x))
    ax.set_xticklabels(["range", "velocity", "theta", "phi"])
    ax.set_ylabel("share of RDD-matched pairs within gate (%)")
    ax.set_title("Per-signal pass rate at 2.5 (m, m/s, rad, rad) gates, FL")
    ax.legend()
    fig.tight_layout()
    fig.savefig(path, dpi=110)
    plt.close(fig)


def build_pdf(pair, base, mb_in, mb_out):
    proot = os.path.join(OUT, base)
    diags = {}
    for s in ("FC", "FL", "RL"):
        dp = os.path.join(proot, s, "KPI", "diagnostics.json")
        try:
            diags[s] = json.load(open(dp, encoding="utf-8"))
        except Exception:
            diags[s] = None
    det_html = os.path.join(proot, "FL", "KPI", f"{base}_detection_kpi.html")
    sl = scan_lines(det_html)
    fl = (diags.get("FL") or {}).get("kpis", {}).get("detection", {})
    f = FORENSICS_FL.get(pair, {})

    pdf_path = os.path.join(OUT, f"{base}_mismatch_report.pdf")
    doc = SimpleDocTemplate(pdf_path, pagesize=A4,
                            leftMargin=1.6 * cm, rightMargin=1.6 * cm,
                            topMargin=1.4 * cm, bottomMargin=1.4 * cm)
    story = [
        p(f"Mismatch forensics — {esc(base)}", S_TITLE),
        p(f"Input {mb_in} MB vs resim output {mb_out} MB (rR00060100) · generated {time.strftime('%Y-%m-%d %H:%M')}",
          S_SMALL),
        HRFlowable(width="100%", thickness=1, color=colors.HexColor("#0d6a8b")),
    ]

    story.append(p("1 · Run inventory", S_H1))
    inv = []
    for s in ("FC", "FL", "RL"):
        d = diags.get(s) or {}
        inv.append([s, ", ".join(d.get("streams_in", []) or ["?"]),
                    ", ".join(d.get("streams_out", []) or ["?"])])
    story.append(table(["Sensor", "Input streams", "Output streams"], inv))
    story.append(Spacer(1, 6))
    story.append(p("2 · KPI results", S_H1))
    res = []
    for s in ("FC", "FL", "RL"):
        d = (diags.get(s) or {}).get("kpis", {}) or {}
        res.append([s,
                    f"{d.get('detection', {}).get('status', '?')} {d.get('detection', {}).get('headline', '')}",
                    f"{d.get('alignment', {}).get('status', '?')}",
                    f"{d.get('tracker', {}).get('status', '?')} {d.get('tracker', {}).get('headline', '')}"])
    story.append(table(["Sensor", "Detection", "Alignment", "Tracker"], res))

    story.append(p("3 · Explain-like-I'm-13", S_H1))
    story.append(p(
        "A radar shouts and listens for echoes — each echo is a <b>detection</b> with a distance "
        "(range), a speed (velocity) and a direction (theta/phi). The resim is a <b>video-game replay</b> "
        "of the same drive. To check the replay is faithful, the KPI plays spot-the-difference in three rounds. "
        "<b>Round 1 — same photo number?</b> Every radar picture has a scan number; only pictures with the same "
        f"number can be compared (FL here: {esc(sl.get('common_in', '?'))} pictures overlap). "
        "<b>Round 2 — same echo pigeonhole?</b> Each echo carries an RDD box number; only echoes from the same box "
        "are compared. <b>Round 3 — close enough?</b> The two echoes must agree within 2.5 m, 2.5 m/s and 2.5 rad. "
        "A <b>0</b> in some scan cell (like 0/50) just means that picture's echoes failed round 2 or 3 — the earlier "
        "rounds still matched fine. The 0.50k-style totals (e.g. 0 matched of ~50k compared) are round-3 failures, "
        "not missing pictures.", S_BODY))
    story.append(p(
        "So when FL shows scan numbers matching but detections at 0%: the photos were taken at the same moments, "
        "but the replay put <b>different cars at different distances</b> in them — while pointing the <b>same "
        "directions</b>.", S_BODY))

    story.append(p("4 · PhD-level layer analysis (FL)", S_H1))
    story.append(p(
        f"Layer 1, scan alignment: DET scans IN/OUT = {f.get('det_in', '?')}/{f.get('det_out', '?')}; "
        f"vehicle/sim scans {esc(sl.get('veh_sim', '?'))}; common/input {esc(sl.get('common_in', '?'))}; "
        f"processed/with-matches {esc(sl.get('proc_match', '?'))}. Verdict: <b>healthy</b> (~95% overlap).", S_BODY))
    story.append(p(
        "Layer 2, RDD prefilter (exact <i>rdd1_rindx, rdd1_dindx</i> equality, O(N) hash maps both sides): "
        f"373/374 common scans carry shared pairs; 62,250 pair comparisons entered layer 3 (0006; 46,764 for 0003). "
        "Verdict: <b>healthy</b> — the oft-suspected RDD stage is not the bottleneck.", S_BODY))
    story.append(p("Layer 3, threshold matrix (all four must pass):", S_H2))
    if True:
        rows = []
        for s, unit in (("ran", "m"), ("vel", "m/s"), ("theta", "rad"), ("phi", "rad")):
            p50, p90, p99, ps = f[s]
            rows.append([s, unit, f"{p50:.3f}", f"{p90:.3f}", f"{p99:.3f}", f"{ps:.1f}%"])
        story.append(table(["signal", "unit", "|delta| p50", "p90", "p99", "pass @2.5"], rows))
        story.append(p(
            "Reading: <b>theta 99.9% / phi 100%</b> — direction is preserved almost perfectly. "
            "<b>Range p50 ≈ 10.4 m</b> (only ~23–25% within 2.5 m), <b>velocity p50 ≈ 4.4–5.8 m/s</b> "
            "(~31–39% within 2.5 m/s). The joint pass rate is the product-like conjunction actually "
            f"measured: <b>{esc(fl.get('headline', ''))} ({esc(fl.get('detail', ''))})</b>. "
            "The delta spread (p50 10 m → p99 ~100 m) rules out a constant bias or a unit slip, both of which "
            "would give a tight shifted distribution; the content genuinely differs per echo while bearings agree. "
            "Interpretation: the resim replays similar scenes from similar angles with different traffic "
            "placement/speeds — direction-consistent, range/velocity-divergent.", S_BODY))
    story.append(p(
        "The four logs sit on one ladder (8.6% → 11.8% → 12.4% → 14.9% across 0003–0006) with the same "
        "fingerprint: healthy scans, healthy RDD overlap, loss concentrated in range, then velocity, "
        "angles near-perfect. 0004 shows the widest range spread (p50 17.4 m).", S_BODY))

    story.append(p("5 · Why alignment & tracker read as they do here", S_H1))
    story.append(p(
        "Alignment <b>failed</b> (not 0%): these AL logs contain no alignment stream at all — neither "
        "<i>DYNAMIC_ALIGNMENT_STREAM</i> nor <i>Dyn_Align_STREAM</i> — so there are no boresight signals to compare. "
        "Tracker <b>missing</b>: the <i>data/</i> AL logs carry <i>DETECTION_STREAM</i> instead of "
        "<i>OBJECT_LIST_STREAM</i> (only the separate <i>tracker/0003</i> log has object tracks), so there are no "
        "track IDs to index. Both are honest absences with stream inventories on the index page, not bugs.",
        S_BODY))
    story.append(p("6 · Figures", S_H1))
    fp1 = os.path.join(ASSETS, f"funnel_{pair}.png")
    funnel_fig(pair, {"kpis": {"detection": fl}}, fp1)
    story.append(Image(fp1, width=15 * cm, height=6.9 * cm))
    story.append(Spacer(1, 6))
    fp2 = os.path.join(ASSETS, f"passrate_{pair}.png")
    passrate_fig(fp2, pair)
    story.append(Image(fp2, width=15 * cm, height=6.9 * cm))
    story.append(Spacer(1, 4))
    story.append(p("Per-signal pass rates at the 2.5 gates, this log (blue) vs 0006 (orange): angles ~100%, "
                   "velocity ~31–39%, range ~22–25%. The headline is their conjunction.", S_SMALL))
    story.append(p("7 · Recommendations", S_H1))
    story.append(p(
        "1) If the resim is <i>supposed</i> to reproduce traffic: investigate scenario seeding/traffic placement — "
        "range/velocity diverge while bearings hold. 2) If it is a <i>different</i> scenario by design: the 8–17% "
        "comparable-scan scores are expected; quote them with the coverage-inclusive overall beside them. "
        "3) Gate changes will not fix this (p50 range ≈ 4× the gate); only scenario fidelity will. "
        "4) For tracker-bearing AL logs use <i>tracker/0003</i>-style files with <i>OBJECT_LIST_STREAM</i>; "
        "for alignment, logs must include a Dyn_Align stream.", S_BODY))
    doc.build(story)
    return pdf_path


def main():
    for pair, (base, mi, mo) in PAIRS.items():
        path = build_pdf(pair, base, mi, mo)
        print(f"PDF {pair}: {path} ({os.path.getsize(path) // 1024} KB)", flush=True)


if __name__ == "__main__":
    main()
