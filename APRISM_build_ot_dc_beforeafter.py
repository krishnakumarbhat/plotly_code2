"""APRISM OT+DC before/after report (Plotly, offline single-file HTML).
All numbers come from APRISM_ot_dc_ab.exe (PASS) + git diff --stat.
Badges: HOST = harness-measured. REPORTED = qual-target, pending team suites.
Run: python APRISM_build_ot_dc_beforeafter.py
"""
import math
from pathlib import Path
import plotly.graph_objects as go
from plotly.subplots import make_subplots

OUT = Path(__file__).parent / "APRISM_OT_DC_BeforeAfter_Report.html"
figs = []
def add(fig):
    figs.append(fig.to_html(full_html=False, include_plotlyjs=False))

# 1. gamma surface
A = [i * 0.25 for i in range(33)]
f1 = go.Figure()
for innov, nm in [(0.0, "innov 0"), (1.0, "innov 1"), (2.0, "innov 2"), (3.0, "innov 3")]:
    f1.add_trace(go.Scatter(x=A, y=[min(40.0, 9.21 * (1 + 0.8 * a / 3.0 + 0.5 * innov)) for a in A], name=nm))
f1.add_hline(y=9.21, line_dash="dash"); f1.add_hline(y=40.0, line_dash="dash")
f1.update_layout(title="OT-3 adaptive gate surface (clamped [9.21, 40])",
                 xaxis_title="|a| (m/s2)", yaxis_title="gamma", height=400,
                 margin=dict(l=50, r=20, t=50, b=40))
add(f1)

# 2. confirm timeline
pat = [1, 1, 0, 1, 1, 1, 0, 1, 1]
seen, conf, out = 0, [], []
for g in pat:
    if g:
        if seen: conf.append(1); seen = 0
        else: conf.append(0); seen = 1
    else:
        conf.append(0); seen = 0
f2 = go.Figure()
f2.add_trace(go.Scatter(y=pat, mode="lines+markers", name="gated"))
f2.add_trace(go.Scatter(y=[c * 0.9 + 0.05 for c in conf], mode="lines+markers", name="confirmed"))
f2.update_layout(title="OT-6 two-scan confirmation (ghost associates only on 2nd consecutive gate)",
                 xaxis_title="scan", height=360, margin=dict(l=50, r=20, t=50, b=40))
add(f2)

# 3. hermite fit vs planar + SEP samples
xs = [10.0, 40.0]; ys = [3.5, 4.1]; ms = [0.02, 0.05]
h = 30.0
a = (2*(ys[0]-ys[1]) + h*(ms[0]+ms[1])) / h**3
b = (3*(ys[1]-ys[0]) - h*(2*ms[0]+ms[1])) / h**2
c3, c2 = a, b - 3*a*xs[0]
c1 = ms[0] - 2*b*xs[0] + 3*a*xs[0]**2
c0 = ys[0] - ms[0]*xs[0] + b*xs[0]**2 - a*xs[0]**3
X = [8 + i for i in range(35)]
YH = [((c3*x + c2)*x + c1)*x + c0 for x in X]
YP = [ys[0] + ms[0]*(x - xs[0]) for x in X]
f3 = go.Figure()
f3.add_trace(go.Scatter(x=X, y=YH, name="Hermite cubic (exact thru both)"))
f3.add_trace(go.Scatter(x=X, y=YP, name="planar tangent", line=dict(dash="dash")))
f3.add_trace(go.Scatter(x=xs, y=ys, mode="markers", marker=dict(size=10), name="SEP points"))
f3.update_layout(title="OT-4 Hermite curved rail vs planar tangent (det=810000)",
                 xaxis_title="x (m)", yaxis_title="y (m)", height=400,
                 margin=dict(l=50, r=20, t=50, b=40))
add(f3)

# 4. deskew quiver
GX, GY, GU, GV = [], [], [], []
for x in range(10, 71, 10):
    for y in (-20, -5, 5, 20):
        th = 0.15 * 0.02
        rx, ry = math.cos(th)*x - math.sin(th)*y, math.sin(th)*x + math.cos(th)*y
        ox, oy = rx - 25*0.02, ry
        GX.append(x); GY.append(y); GU.append(ox - x); GV.append(oy - y)
f4 = go.Figure()
f4.add_trace(go.Scatter(x=GX, y=GY, mode="markers", marker=dict(size=6), name="measured"))
import plotly.figure_factory as ff
q = ff.create_quiver(GX, GY, GU, GV, scale=8, arrow_scale=0.25, name="correction x8", line=dict(width=2))
f4.add_trace(q.data[0])
f4.update_layout(title="DC-2 deskew field dt=20ms, w=0.15, v=25 (vectors x8)",
                 xaxis_title="x (m)", yaxis_title="y (m)", height=420,
                 margin=dict(l=50, r=20, t=50, b=40))
add(f4)

# 5. Reff + entropy
f5 = make_subplots(rows=1, cols=2, subplot_titles=("R_eff constituents", "Entropy scale vs H"))
f5.add_trace(go.Bar(x=["R base", "+ (J s)^2", "= R_eff"], y=[0.9, 0.64, 1.54]), 1, 1)
H = [i * 0.25 for i in range(17)]
f5.add_trace(go.Scatter(x=H, y=[math.exp(h / 10.0) for h in H], name="exp(H/10)"), 1, 2)
f5.update_layout(title="OT-1/OT-5 covariance handling", height=400, showlegend=False,
                 margin=dict(l=50, r=20, t=60, b=40))
add(f5)

# 6. NIS chi2
import math as _m
X6 = [i * 0.1 for i in range(1, 120)]
Y6 = [0.5 * _m.exp(-x / 2) for x in X6]
f6 = go.Figure()
f6.add_trace(go.Scatter(x=X6, y=Y6, name="chi2 df=2 pdf"))
f6.add_vline(x=1.37, annotation_text="synthetic NIS 1.37")
f6.add_vline(x=5.99, line_dash="dash", annotation_text="95% = 5.99")
f6.update_layout(title="NIS calibration discipline (whitened innovations)",
                 xaxis_title="NIS", height=360, margin=dict(l=50, r=20, t=50, b=40))
add(f6)

# 7. edit inventory
files = ["cca_moveable", "hypothesis", "host_reflected", "guardrail_track",
         "ctca", "detect_mp", "refl_obj", "ctca_sup", "VArunTracker", "sil_wrapper"]
adds = [51, 32, 54, 38, 22, 16, 11, 8, 25, 10]
f7 = go.Figure()
f7.add_trace(go.Bar(x=files, y=adds, name="+lines (incl. new headers)"))
f7.update_layout(title="Edit inventory: OT 214+/18-, DC 35+/0 (OFF-identical via guards)",
                 height=400, margin=dict(l=50, r=20, t=60, b=110))
add(f7)

css = ("body{font-family:Arial,Helvetica,sans-serif;margin:0;background:#f4f6f9;color:#1c2733}"
 ".wrap{max-width:1180px;margin:auto;padding:24px}"
 ".hero{background:linear-gradient(120deg,#0b3a2a,#0e7c4a);color:#fff;padding:34px;border-radius:14px}"
 ".hero small{opacity:.85}.cards{display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:12px;margin:18px 0}"
 ".card{background:#fff;border-radius:12px;padding:16px;box-shadow:0 1px 4px rgba(0,0,0,.08)}"
 ".card b{font-size:24px;display:block}.card span{font-size:13px;color:#546}"
 ".sec{background:#fff;border-radius:12px;padding:20px;margin:16px 0;box-shadow:0 1px 4px rgba(0,0,0,.08)}"
 "h2{margin-top:0;color:#0b3a2a}table{border-collapse:collapse;width:100%;font-size:13px}"
 "th,td{border:1px solid #d7dee8;padding:7px;text-align:left}th{background:#e6f3ec}"
 ".pill{display:inline-block;padding:2px 10px;border-radius:20px;font-size:12px;font-weight:bold}"
 ".host{background:#d7f5de;color:#0c6b2f}.rep{background:#fff3cd;color:#8a5f00}.risk{background:#fde2e2;color:#a12020}"
 ".foot{font-size:12px;color:#66798c}code{background:#eef1f6;padding:1px 5px;border-radius:4px}")
B = lambda t, c: f'<span class="pill {c}">{t}</span>'

ba = ("""<table><tr><th>Item</th><th>BEFORE (OFF)</th><th>AFTER (ON)</th><th>Evidence</th></tr>"""
 """<tr><td>OT-1 ghost row</td><td>direct-only update, ghost discounted</td><td>+1 virtual RR row, R 0.9&rarr;1.54, XOR sentinel -1</td><td>""" + B("HOST","host") + """ helper math; firmware qual pending</td></tr>"""
 """<tr><td>OT-2 mirror penalty</td><td>mirror_prob assigned 0.9/1.0</td><td>skipped when sentinel set (5 sites) + rank-1 necessary condition in SEP check</td><td>""" + B("HOST","host") + """ logic; qual pending</td></tr>"""
 """<tr><td>OT-3 gate</td><td>static 9.21-equivalent thresholds</td><td>9.21&rarr;16.58 @1g, clamp 40; N24 rank factor neutral v0</td><td>""" + B("HOST","host") + """ curve</td></tr>"""
 """<tr><td>OT-4 rail fit</td><td>planar SEP only</td><td>Hermite exact (det 810000), counted, non-deciding</td><td>""" + B("HOST","host") + """ fit</td></tr>"""
 """<tr><td>OT-5 R scale</td><td>bin-weight R</td><td>x1.0 neutral; x1.22 @H=2/SNR=10 ready</td><td>""" + B("HOST","host") + """; feed pending</td></tr>"""
 """<tr><td>OT-6 ghost assoc</td><td>immediate on gate</td><td>2nd consecutive gate required (0,1 then miss&rarr;0)</td><td>""" + B("HOST","host") + """ sequence</td></tr>"""
 """<tr><td>DC-1 preflight</td><td>cold nominal start</td><td>seed -1.145/-0.018 validated, stored, applied as boresight offset in UpdateSensorVariables</td><td>""" + B("HOST","host") + """; replay-header sourcing pending</td></tr>"""
 """<tr><td>DC-2 deskew</td><td>single epoch</td><td>identity today (dt=0 verified), path armed, max-dt recorded</td><td>""" + B("HOST","host") + """; async epochs pending</td></tr>"""
 """<tr><td>DC-3 clock</td><td>coupled yaw/clock</td><td>Gram split tau=-0.5 exact on synthetic pair</td><td>""" + B("HOST","host") + """; fusion transport pending</td></tr>"""
 """<tr><td>Ghost RMSE</td><td>1.969 m</td><td>&le;0.651 m target</td><td>""" + B("REPORTED","rep") + """ team qual suites</td></tr>"""
 """<tr><td>Cut-in recall</td><td>86.3%</td><td>&ge;99.0% target</td><td>""" + B("REPORTED","rep") + """ team qual suites</td></tr></table>""")

html = f"""<!DOCTYPE html><html><head><meta charset="utf-8">
<title>A-PRISM OT+DC — Before/After</title>
<script src="https://cdn.plot.ly/plotly-2.35.2.min.js"></script>
<style>{css}</style></head><body><div class="wrap">
<div class="hero"><small>CHECKPOINT + EDITS &middot; OT mirror 214+/18- &middot; DC mirror 35+/0 &middot; 0 commits &middot; 0 pushes &middot; harness <b>APRISM_OT_DC_AB PASS</b></small>
<h1>OT + DC before / after — measured deltas, honest pendings</h1>
<p>Every ON-behavior below was executed in the standalone harness against the real production headers. OFF paths are guard-identical to baseline. {B("HOST","host")} measured &middot; {B("REPORTED","rep")} qual-target pending &middot; {B("RISK","risk")} owner action.</p></div>
<div class="cards">
<div class="card"><b>9 / 9</b><span>Work items landed (6 OT + 3 DC)</span></div>
<div class="card"><b>16.58</b><span>Gamma at 1g accel (was 9.21)</span></div>
<div class="card"><b>1.54</b><span>Ghost R_eff (was 0.9)</span></div>
<div class="card"><b>2nd scan</b><span>Ghost confirmation gate</span></div>
<div class="card"><b>810000</b><span>Hermite det (exact fit)</span></div>
<div class="card"><b>-0.5</b><span>Clock tau exact (synthetic)</span></div>
<div class="card"><b>PASS</b><span>Harness, 0 failures</span></div>
<div class="card"><b>1.37 &lt; 5.99</b><span>NIS inside 95% band</span></div>
</div>
<div class="sec"><h2>1. Before / after ledger</h2>{ba}</div>
<div class="sec"><h2>2. OT-3 gate surface + OT-6 confirmation</h2>{figs[0]}{figs[1]}</div>
<div class="sec"><h2>3. OT-4 Hermite vs planar + DC-2 deskew field</h2>{figs[2]}{figs[3]}
<p>Deskew vectors (x8) for dt=20 ms show the correction this path applies once async satellite epochs land; today dt=0 so firmware output is unchanged.</p></div>
<div class="sec"><h2>4. OT-1/OT-5 covariance + NIS discipline</h2>{figs[4]}{figs[5]}
<p>Joseph form P<sup>+</sup>=(I&minus;KH)P<sup>&minus;</sup>(I&minus;KH)<sup>T</sup>+KRK<sup>T</sup> is reused untouched; only R inputs change. PD asserted pre-update.</p></div>
<div class="sec"><h2>5. Edit inventory (git diff --stat)</h2>{figs[6]}
<p>OT: 8 files modified + <code>aprism_ot_hooks.h</code> new. DC: 2 files modified + <code>aprism_dc_hooks.h</code> new. Deletions are guard-<code>#else</code> branches preserving OFF bytes. No BUILD/CMakeLists changes needed (OT BUILD globs headers; DC headers are same-dir).</p></div>
<div class="sec"><h2>6. Compile verification (this workstation)</h2>
<table><tr><th>Check</th><th>Result</th></tr>
<tr><td>All 8 edited OT files, flag OFF</td><td>parse clean (real include graph, MSVC-STL workaround)</td></tr>
<tr><td>CCA/CTCA/hypothesis/guardrail, flag ON</td><td>parse clean incl. all APRISM blocks</td></tr>
<tr><td>DC Tracker_VariantA, OFF vs ON</td><td>identical pre-existing error sets (4 F360_Tracker API-skew errors, none in APRISM lines)</td></tr>
<tr><td>Standalone harness vs real headers</td><td>PASS, 0 failures (rank-1, gamma, Reff, gate, Hermite, XOR, seed, deskew, clock, N24, NIS)</td></tr></table>
<p>Caught live by this check: an <code>APRISM_OT_COUNT</code> macro invisible to OFF builds (fixed: unconditional include), and the mirror <code>f360_tracker.cpp</code> stage order verified Time_Update&rarr;PreAssoc&rarr;Assoc&rarr;MeasUpdate&rarr;Validity.</p></div>
<div class="sec"><h2>7. What remains (owners)</h2>
<table><tr><th>Pending</th><th>Owner</th></tr>
<tr><td>Re-verify anchors on Gerrit dev branch; run variant-A build + qual suites</td><td>OT team</td></tr>
<tr><td>M2D one-time field mapping for preflight transport</td><td>DC owner</td></tr>
<tr><td>Per-sensor async epochs for live deskew; paired-target fusion inputs for clock</td><td>DC/fusion owners</td></tr>
<tr><td>WSE spray-entropy feed for OT-5; reflector rank-1 refinement for OT-1 pairing</td><td>OT team</td></tr>
<tr><td>Hermite deciding vote after qual comparison vs planar classifier</td><td>OT team</td></tr></table></div>
<div class="foot">Dummy vectors: fixed-seed synthetic guardrail scene (ego 25 m/s, rail y=3.5, target 55 m). Harness compiles the real headers; firmware qual suites decide the REPORTED targets.</div>
</div></body></html>"""
OUT.write_text(html, encoding="utf-8")
print(f"WROTE {OUT} ({OUT.stat().st_size/1024:.0f} KB)")
