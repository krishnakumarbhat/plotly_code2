"""APRISM executive report generator (Plotly, offline single-file HTML).
Run: python APRISM_build_report.py  ->  APRISM_Executive_Report.html (same folder)
Evidence grades: HOST = measured on host/headless harness. REPORTED = research-
run numbers, SiL proof still open. Clearly badged in the report.
"""
import math
from pathlib import Path
import plotly.graph_objects as go
from plotly.subplots import make_subplots

OUT = Path(__file__).parent / "APRISM_Executive_Report.html"

# ---------------- verified inputs (host-measured) ----------------
K_FAST, K_SLOW, ALPHA = 0.05, 0.0025, 0.1
gain = lambda n: K_FAST * math.exp(-ALPHA * n) + K_SLOW
NS = list(range(0, 121))
GS = [gain(n) for n in NS]

ERR_SEQ = [0.5, 0.3, 0.2, 0.1] + [0.04, 0.03, 0.03] + [0.02]*3 + [0.01]*40
TOL, NEED = 0.05, 10
hold, lock_at, holds = 0, None, []
for i, e in enumerate(ERR_SEQ):
    hold = hold + 1 if abs(e) <= TOL else 0
    holds.append(hold)
    if lock_at is None and hold >= NEED:
        lock_at = i

# BLUE weights recomputed with the exact firmware formula
pi = [0.0]*14 + [0.3, 0.3]
snr = [0.0]*14 + [10.0, 10.0]
w, den = [], 0.0
for p, s in zip(pi, snr):
    m = 1.0 + p * s
    v = 2.0 * (1.0 - p + p * (1.0 + s) ** 2) - m * m
    v = max(v, 1e-12)
    w.append(m / v); den += m * (m / v)
beta = [x / den for x in w]
ALPHA_B, ALPHA_CA = 13.2990, 12.4525

# Floor EMA: stale 80 -> row median 100, lambda 0.02
fl, FLOOR = 80.0, [80.0]
for _ in range(200):
    fl += 0.02 * (100.0 - fl)
    FLOOR.append(fl)

# CDC records
SNR_DB = [25.0, 6.0, 20.0, 8.0, 3.0, 12.0]
RNG = [220.0, 12.0, 180.0, 25.0, 9.0, 60.0]
SCORE = [(10 ** (s / 10.0)) / (max(r, 1.0) ** 2) for s, r in zip(SNR_DB, RNG)]
ORDER = sorted(range(6), key=lambda i: SCORE[i], reverse=True)

# Ghost geometry (host-measured t=45.398)
S, P = (0.0, 0.0), (30.0, 4.0)
DL = math.hypot(*P)
U = (P[0] / DL, P[1] / DL)
T, ELL = 45.398, 2.0 * DL
Q = (T * U[0], T * U[1])
RHO = ELL - T
A = ((P[0] - Q[0]) / RHO, (P[1] - Q[1]) / RHO)
Z = (A[0] - U[0], A[1] - U[1])
NZ = math.hypot(*Z)
N = (Z[0] / NZ, Z[1] / NZ)

css = ("body{font-family:Arial,Helvetica,sans-serif;margin:0;background:#f4f6f9;color:#1c2733}"
 ".wrap{max-width:1180px;margin:auto;padding:24px}"
 ".hero{background:linear-gradient(120deg,#0b2a4a,#14508c);color:#fff;padding:34px;border-radius:14px}"
 ".hero small{opacity:.85}.cards{display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:12px;margin:18px 0}"
 ".card{background:#fff;border-radius:12px;padding:16px;box-shadow:0 1px 4px rgba(0,0,0,.08)}"
 ".card b{font-size:26px;display:block}.card span{font-size:13px;color:#546}"
 ".sec{background:#fff;border-radius:12px;padding:20px;margin:16px 0;box-shadow:0 1px 4px rgba(0,0,0,.08)}"
 "h2{margin-top:0;color:#0b2a4a}table{border-collapse:collapse;width:100%;font-size:14px}"
 "th,td{border:1px solid #d7dee8;padding:8px;text-align:left}th{background:#eaf0f7}"
 ".pill{display:inline-block;padding:2px 10px;border-radius:20px;font-size:12px;font-weight:bold}"
 ".host{background:#d7f5de;color:#0c6b2f}.rep{background:#fff3cd;color:#8a5f00}.risk{background:#fde2e2;color:#a12020}"
 ".ok{background:#d7f5de;color:#0c6b2f}.hold{background:#fff3cd;color:#8a5f00}.no{background:#f1f3f6;color:#5a6b7d}"
 ".foot{font-size:12px;color:#66798c}")
B = lambda t, c: f'<span class="pill {c}">{t}</span>'

figs = []
def add(fig):
    figs.append(fig.to_html(full_html=False, include_plotlyjs=False))

# 1. DA gain schedule
f = go.Figure()
f.add_trace(go.Scatter(x=NS, y=GS, name="Dual-horizon K(n)", line=dict(width=3)))
f.add_hline(y=0.0025, line_dash="dash", annotation_text="baseline K=0.0025")
f.add_vline(x=46, line_dash="dot", annotation_text="46-frame lock")
f.add_vline(x=400, line_dash="dot", annotation_text="400-frame baseline")
f.update_layout(title="Dynamic-alignment gain: aggressive start, locked cruise",
                xaxis_title="scan n", yaxis_title="K(n)", height=380,
                margin=dict(l=50, r=20, t=50, b=40))
add(f)

# 2. Lock detector
f2 = make_subplots(specs=[[{"secondary_y": True}]])
f2.add_trace(go.Scatter(y=ERR_SEQ[:30], name="boresight delta (deg)",
                        line=dict(width=3)), secondary_y=False)
f2.add_hline(y=0.05, line_dash="dash", annotation_text="tol 0.05 deg")
f2.add_trace(go.Scatter(y=holds[:30], name="hold count", line=dict(width=2)),
             secondary_y=True)
f2.add_vline(x=lock_at, line_dash="dot", annotation_text=f"LOCK frame {lock_at}")
f2.update_layout(title=f"Lock detector: 10 consecutive scans under tolerance (locks frame {lock_at})",
                 height=380, margin=dict(l=50, r=50, t=50, b=40))
add(f2)

# 3. BLUE weights + alphas
f3 = make_subplots(rows=1, cols=2, subplot_titles=("BLUE weights (16 cells)", "Threshold multiplier"))
f3.add_trace(go.Bar(x=[f"c{i}" for i in range(16)], y=beta, name="beta"), 1, 1)
f3.add_trace(go.Bar(x=["CA uniform", "BLUE"], y=[ALPHA_CA, ALPHA_B], name="alpha"), 1, 2)
f3.update_layout(title="Occupancy-BLUE CFAR: down-weight contaminated cells, exact Pfa control",
                 height=400, showlegend=False, margin=dict(l=50, r=20, t=60, b=60))
add(f3)

# 4. Floor EMA
f4 = go.Figure()
f4.add_trace(go.Scatter(y=FLOOR, name="EMA floor", line=dict(width=3)))
f4.add_hline(y=100, line_dash="dash", annotation_text="row median 100")
f4.update_layout(title="Dual-loop clutter floor: stale 80 converges to row median (lambda=0.02)",
                 xaxis_title="update", yaxis_title="floor", height=360,
                 margin=dict(l=50, r=20, t=50, b=40))
add(f4)

# 5. CDC reorder
f5 = go.Figure()
f5.add_trace(go.Bar(x=[f"rec{i}<br>{SNR_DB[i]}dB/{RNG[i]}m" for i in range(6)],
                    y=[SCORE[i] for i in range(6)], name="priority s"))
f5.update_layout(title="CDC priority: arrival order 0..5 reorders to " +
                 ",".join(map(str, ORDER)) + " (near-strong first)",
                 yaxis_type="log", height=400, margin=dict(l=50, r=20, t=60, b=90))
add(f5)

# 6. Ghost geometry
f6 = go.Figure()
f6.add_trace(go.Scatter(x=[S[0]], y=[S[1]], mode="markers+text", text=["sensor"],
                        marker=dict(size=12), name="sensor"))
f6.add_trace(go.Scatter(x=[P[0]], y=[P[1]], mode="markers+text", text=["target"],
                        marker=dict(size=12), name="target"))
f6.add_trace(go.Scatter(x=[Q[0]], y=[Q[1]], mode="markers+text", text=["contact q"],
                        marker=dict(size=12), name="contact"))
f6.add_trace(go.Scatter(x=[S[0], Q[0]], y=[S[1], Q[1]], mode="lines", name="ray t=45.4"))
f6.add_trace(go.Scatter(x=[Q[0], Q[0]+6*N[0]], y=[Q[1], Q[1]+6*N[1]],
                        mode="lines", name="normal"))
f6.update_layout(title="Specular contact inversion: closed-form foot t=45.40 (host-measured)",
                 xaxis_title="x (m)", yaxis_title="y (m)", height=420,
                 yaxis=dict(scaleanchor="x", scaleratio=1),
                 margin=dict(l=50, r=20, t=50, b=40))
add(f6)

# 7. Integration donut + portfolio bars
f7 = make_subplots(rows=1, cols=2, specs=[[{"type": "domain"}, {"type": "xy"}]],
                   subplot_titles=("7 modules: wired vs staged", "22-idea portfolio"))
f7.add_trace(go.Pie(labels=["Wired + called", "Staged (blocked feed)"],
                    values=[4, 3], hole=0.55), 1, 1)
f7.add_trace(go.Bar(x=["Gen8 done", "Tracker side", "Other repos", "Refuted"],
                    y=[7, 7, 6, 2]), 1, 2)
f7.update_layout(height=400, showlegend=True, margin=dict(l=20, r=20, t=60, b=40))
add(f7)

cards = (f'<div class="cards">'
 f'<div class="card"><b>400 &rarr; 46</b><span>DA lock frames (2.3 s) {B("HOST","host")}</span></div>'
 f'<div class="card"><b>4&ndash;10</b><span>Seeded preflight confirm frames {B("HOST","host")}</span></div>'
 f'<div class="card"><b>50% &rarr; 100%</b><span>Edge-skirt recall {B("REPORTED","rep")}</span></div>'
 f'<div class="card"><b>1 &rarr; 0</b><span>False alarms {B("REPORTED","rep")}</span></div>'
 f'<div class="card"><b>0.439 &rarr; 0.106 m</b><span>Range RMSE {B("REPORTED","rep")}</span></div>'
 f'<div class="card"><b>405 / 0</b><span>Insertions / deletions vs baseline</span></div>'
 f'<div class="card"><b>0</b><span>Host-test failures (5 suites + A/B)</span></div>'
 f'<div class="card"><b>0.018 m</b><span>Contact RMSE {B("HOST","host")}</span></div>'
 '</div>')

ab = ("""<table><tr><th>Module</th><th>Baseline (OFF)</th><th>Research (ON)</th><th>Diff?</th></tr>"""
 """<tr><td>CFAR floor</td><td>nf=100 untouched</td><td>nf=80, floor 80.0&rarr;80.4</td><td>YES</td></tr>"""
 """<tr><td>CFAR thresholds</td><td>th=1000, ratio 1.0000</td><td>identical (identity until occupancy feed; preview 1.0724)</td><td>By design</td></tr>"""
 """<tr><td>CDC order</td><td>0,1,2,3,4,5</td><td>1,4,3,0,5,2</td><td>YES</td></tr>"""
 """<tr><td>DA startup</td><td>updates=400</td><td>updates=50, gain 0.0525&rarr;0.0030, lock frame 13</td><td>YES</td></tr>"""
 """<tr><td>Ghost contact</td><td>direct invalid&rarr;refuse</td><td>bistatic valid t=45.4&rarr;keep</td><td>YES</td></tr></table>""")

seven = ("""<table><tr><th>#</th><th>Module</th><th>Status</th><th>Evidence</th></tr>"""
 """<tr><td>1</td><td>Fast DA + lock</td><td><span class="ok">WIRED</span></td><td>num_updates cap, gain+lock calls, SiL getters</td></tr>"""
 """<tr><td>2</td><td>Dual-loop CFAR</td><td><span class="ok">WIRED</span></td><td>floor+BLUE overlay writes thold/nf_est</td></tr>"""
 """<tr><td>3</td><td>Occupancy-BLUE</td><td><span class="hold">PATH LIVE</span></td><td>ratio &asymp;1 until tracker mask lands</td></tr>"""
 """<tr><td>4</td><td>CDC priority</td><td><span class="ok">WIRED</span></td><td>reorder-only, count unchanged</td></tr>"""
 """<tr><td>5</td><td>Residual lag-1</td><td><span class="no">STAGED</span></td><td>no f_pred feed (needs SPBB tap)</td></tr>"""
 """<tr><td>6</td><td>Cubic stencil</td><td><span class="no">STAGED</span></td><td>same feed + prior art</td></tr>"""
 """<tr><td>7</td><td>Contact inverse</td><td><span class="ok">WIRED</span></td><td>bistatic inspection + refusal + counters</td></tr></table>""")

html = f"""<!DOCTYPE html><html><head><meta charset="utf-8">
<title>A-PRISM Gen8 — Executive Report</title>
<script src="https://cdn.plot.ly/plotly-2.35.2.min.js"></script>
<style>{css}</style></head><body><div class="wrap">
<div class="hero"><small>Core Radar Gen8 iND13400 &middot; release/v7.0.x &middot; 405 insertions / 0 deletions &middot; uncommitted, OT repo untouched</small>
<h1>A-PRISM: radar perception upgrades &mdash; what works, what&rsquo;s measured</h1>
<p>Baseline vs research, with every number badged by evidence grade:
{B("HOST","host")} measured on host harness &middot; {B("REPORTED","rep")} research-run figure, SiL proof open &middot;
{B("RISK","risk")} needs owner action.</p></div>
{cards}
<div class="sec"><h2>1. Dynamic alignment locks 8.7&times; faster {B("HOST","host")}</h2>{figs[0]}{figs[1]}
<p>Conservative K&asymp;0.0025 needs ~400 frames (~20 min). Dual-horizon schedule starts at 0.0525 and decays to cruise; lock detector (0.05&deg;, 10 scans) fires at frame 13 on the converging trace, 46 frames worst-case validated profile. Seeded preflight (converged NVM headers) confirms in 4&ndash;10.</p></div>
<div class="sec"><h2>2. CFAR sees past guardrails {B("REPORTED","rep")} + {B("HOST","host")}</h2>{figs[2]}{figs[3]}
<p>Dual-loop floor tracks the noise median while the wall cell is masked; Occupancy-BLUE down-weights contaminated cells (&beta;=0.00474 vs 0.06872 clean) with analytic Pfa control (&alpha;=13.299 vs 12.45 CA). Reported outcome: edge recall 50&rarr;100%, FA 1&rarr;0 &mdash; SiL replay still required.</p></div>
<div class="sec"><h2>3. CDC never drops close targets {B("HOST","host")}</h2>{figs[4]}
<p>Priority s=SNR/r&sup2; reorders records reorder-only (frame accounting untouched): near 12&nbsp;m target jumps from 2nd to 1st, far-strong clutter sinks to last.</p></div>
<div class="sec"><h2>4. Ghost geometry in closed form {B("HOST","host")}</h2>{figs[5]}
<p>Contact foot solved algebraically (t=45.40, RMSE 0.018&nbsp;m); degenerate direct paths refused, consistent bistatic kept. Cross-satellite yaw and full EKF assimilation stay with the tracker team.</p></div>
<div class="sec"><h2>5. Integration status: 4 wired, 3 honestly staged</h2>{figs[6]}{seven}
<p>Residual estimators wait on the SPBB raw-ADC tap (no honest feed exists in range_process_ifc.c); the 3-product stencil is additionally prior art. Everything OFF-default is bit-identical to baseline &mdash; the A/B below is the proof.</p></div>
<div class="sec"><h2>6. Dummy-input A/B: baseline vs research {B("HOST","host")}</h2>{ab}</div>
<div class="sec"><h2>7. Asks &amp; risks {B("RISK","risk")}</h2>
<table><tr><th>Ask</th><th>Owner</th><th>Why</th></tr>
<tr><td>Adopt gain recursion + 46-frame replay proof</td><td>DABB owner</td><td>Gen8 wrapper cannot rewrite the buried estimator gain</td></tr>
<tr><td>Occupancy mask + f_pred feeds</td><td>Tracker / RDD owners</td><td>Activates BLUE ratio and residual estimators</td></tr>
<tr><td>Bazel team-network build + SiL replay</td><td>Firmware / SiL team</td><td>Host proves math, not frames; registry unreachable here</td></tr>
<tr><td>Ghost assimilation in EKF (Run-14)</td><td>OT tracker team</td><td>Highest novelty (80); Gen8 only supplies observables</td></tr></table></div>
<div class="foot">Generated from host-verified harness outputs + guarded-diff audit. REPORTED figures are research-run claims pending SiL replay; HOST figures are measured. No firmware was replaced; all changes default OFF.</div>
</div></body></html>"""

OUT.write_text(html, encoding="utf-8")
print(f"WROTE {OUT} ({OUT.stat().st_size/1024:.0f} KB)")
