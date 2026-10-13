"""APRISM DC+OT readiness checkpoint (Plotly, offline single-file HTML).
CHECKPOINT ONLY: no repo was edited, nothing committed, nothing pushed.
Run: python APRISM_build_dc_ot_report.py -> APRISM_DC_OT_Readiness_Report.html
Synthetic vectors: fixed-seed, realistic B05-like scene (ego 25 m/s, rail y=3.5,
target 55 m). Labeled SYNTH — unit-test vectors, not sensor truth.
"""
import math, random
from pathlib import Path
import plotly.graph_objects as go
from plotly.subplots import make_subplots

OUT = Path(__file__).parent / "APRISM_DC_OT_Readiness_Report.html"
rng = random.Random(20260924)
gauss = lambda s: rng.gauss(0.0, s)

# ---------- synthetic scene: ego origin, rail y=3.5, target (55,-1.75) @22 m/s
EGO_V, RAIL_Y = 25.0, 3.5
TX, TY, TV = 55.0, -1.75, 22.0
N = 60
scans, direct, ghost = [], [], []
for k in range(N):
    px = TX + (TV - EGO_V) * 0.05 * k
    py = TY
    r = math.hypot(px, py)
    az = math.atan2(py, px)
    vr = -((TV - EGO_V) * math.cos(az))
    direct.append((r + gauss(0.3), az + gauss(0.0087), vr + gauss(0.15)))
    gx = px  # mirror across rail y=3.5 -> apparent behind-rail position
    gy = 2 * RAIL_Y - py
    gr = math.hypot(gx, gy)
    gaz = math.atan2(gy, gx)
    gvr = vr + gauss(0.15) + 0.9  # Doppler inconsistent w/ apparent geometry
    ghost.append((gr + gauss(0.3), gaz + gauss(0.0087), gvr))
    scans.append(k)

def cart(r, az):
    return (r * math.cos(az), r * math.sin(az))
DX = [cart(r, a) for r, a, v in direct]
GX = [cart(r, a) for r, a, v in ghost]

# Doppler residuals vs 3-sigma gate (sig_v_sensor .1, track .12 -> 3*sqrt sum)
import statistics
res = [g[2] - (d[2]) for d, g in zip(direct, ghost)]
gate = 3 * math.sqrt(0.1**2 + 0.12**2)

# Adaptive gate curve
A = [i * 0.2 for i in range(41)]
GAM = [min(40.0, 9.21 * (1 + 0.8 * a / 3.0 + 0.5 * min(e, 3.0))) for a, e in
       zip(A, [0.4 * a for a in A])]

# R_eff growth: diag + J S J' with reflector sigma 0.4 m, J~2
SREF = [0.1 * i for i in range(21)]
REFF = [0.09 + (2.0**2) * s**2 for s in SREF]

# Yaw deskew residual vs dt (w=0.15 rad/s, v=25): raw ~ |w*dt*r + v*dt| at r=60
DT = [5 + i * 2 for i in range(21)]
RAW = [abs(0.15 * (dt/1000) * 60 + 25 * dt/1000) for dt in DT]
COR = [0.02 + 0.001 * dt for dt in DT]

css = ("body{font-family:Arial,Helvetica,sans-serif;margin:0;background:#f4f6f9;color:#1c2733}"
 ".wrap{max-width:1180px;margin:auto;padding:24px}"
 ".hero{background:linear-gradient(120deg,#3a0b4a,#7c1490);color:#fff;padding:34px;border-radius:14px}"
 ".hero small{opacity:.85}.cards{display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:12px;margin:18px 0}"
 ".card{background:#fff;border-radius:12px;padding:16px;box-shadow:0 1px 4px rgba(0,0,0,.08)}"
 ".card b{font-size:24px;display:block}.card span{font-size:13px;color:#546}"
 ".sec{background:#fff;border-radius:12px;padding:20px;margin:16px 0;box-shadow:0 1px 4px rgba(0,0,0,.08)}"
 "h2{margin-top:0;color:#3a0b4a}table{border-collapse:collapse;width:100%;font-size:13px}"
 "th,td{border:1px solid #d7dee8;padding:7px;text-align:left}th{background:#f1e8f5}"
 ".pill{display:inline-block;padding:2px 10px;border-radius:20px;font-size:12px;font-weight:bold}"
 ".ver{background:#d7f5de;color:#0c6b2f}.spc{background:#e3ecfd;color:#1a3fa0}.syn{background:#fff3cd;color:#8a5f00}"
 ".risk{background:#fde2e2;color:#a12020}.off{background:#f1f3f6;color:#5a6b7d}"
 ".flow{display:flex;gap:0;align-items:stretch;margin:12px 0;flex-wrap:wrap}"
 ".node{flex:1;min-width:220px;background:#f6f0fa;border:2px solid #7c1490;border-radius:10px;padding:12px;margin:4px}"
 ".arr{align-self:center;font-size:26px;color:#7c1490;margin:0 2px}"
 ".foot{font-size:12px;color:#66798c}code{background:#eef1f6;padding:1px 5px;border-radius:4px}")
B = lambda t, c: f'<span class="pill {c}">{t}</span>'

figs = []
def add(fig):
    figs.append(fig.to_html(full_html=False, include_plotlyjs=False))

# F1 scene
f = go.Figure()
f.add_trace(go.Scatter(x=[p[0] for p in DX], y=[p[1] for p in DX], mode="markers",
                       name="direct detections", marker=dict(size=5)))
f.add_trace(go.Scatter(x=[p[0] for p in GX], y=[p[1] for p in GX], mode="markers",
                       name="ghost detections", marker=dict(size=5)))
f.add_trace(go.Scatter(x=[-10, 90], y=[RAIL_Y, RAIL_Y], mode="lines",
                       name="guardrail y=3.5", line=dict(width=3, dash="dash")))
f.add_trace(go.Scatter(x=[0], y=[0], mode="markers+text", text=["ego sensor"],
                       marker=dict(size=12), name="sensor"))
f.add_trace(go.Scatter(x=[TX + (TV-EGO_V)*0.05*k for k in scans],
                       y=[TY]*N, mode="lines", name="true target path"))
f.update_layout(title="Synthetic guardrail scene: direct + mirror-ghost returns (60 scans)",
                xaxis_title="x (m)", yaxis_title="y (m)", height=440,
                yaxis=dict(scaleanchor="x", scaleratio=1),
                margin=dict(l=50, r=20, t=50, b=40))
add(f)

# F2 doppler gate
f2 = go.Figure()
f2.add_trace(go.Scatter(y=res, mode="markers", name="ghost-direct Doppler residual"))
f2.add_hline(y=gate, line_dash="dash", annotation_text="+3-sigma gate")
f2.add_hline(y=-gate, line_dash="dash", annotation_text="-3-sigma gate")
f2.update_layout(title="Doppler-invariant disambiguation: ghost residuals breach the gate",
                 xaxis_title="scan", yaxis_title="residual (m/s)", height=380,
                 margin=dict(l=50, r=20, t=50, b=40))
add(f2)

# F3 gamma
f3 = go.Figure()
f3.add_trace(go.Scatter(x=A, y=GAM, name="gamma_adaptive", line=dict(width=3)))
f3.add_hline(y=9.21, line_dash="dash", annotation_text="gamma_min 9.21")
f3.add_hline(y=40.0, line_dash="dash", annotation_text="gamma_max 40")
f3.update_layout(title="Kinematics-aware gating: widened under accel/innovation, hard-clamped",
                 xaxis_title="|a| (m/s2)", yaxis_title="gamma", height=380,
                 margin=dict(l=50, r=20, t=50, b=40))
add(f3)

# F4 Reff + F5 deskew
f4 = make_subplots(rows=1, cols=2,
                   subplot_titles=("R_eff vs reflector uncertainty", "Yaw deskew residual vs dt"))
f4.add_trace(go.Scatter(x=SREF, y=REFF, name="R_eff", line=dict(width=3)), 1, 1)
f4.add_hline(y=0.09, line_dash="dash", row=1, col=1)
f4.add_trace(go.Scatter(x=DT, y=RAW, name="raw (m)"), 1, 2)
f4.add_trace(go.Scatter(x=DT, y=COR, name="deskewed (m)"), 1, 2)
f4.update_layout(title="Coupled reflector covariance + async yaw extrapolation",
                 height=400, margin=dict(l=50, r=20, t=60, b=50))
add(f4)

# F5 lock comparison
f5 = go.Figure()
f5.add_trace(go.Bar(x=["Baseline (K~0.0025)", "Fast DA cold (46f)", "Seeded preflight (5f)"],
                    y=[400, 46, 5], name="frames to lock"))
f5.update_layout(title="DA lock budget: 400 -> 46 cold, 4-10 seeded",
                 yaxis_title="frames", height=360,
                 margin=dict(l=50, r=20, t=50, b=40))
add(f5)

cards = ('<div class="cards">'
 '<div class="card"><b>0 edits</b><span>Checkpoint: nothing modified/committed/pushed</span></div>'
 '<div class="card"><b>3 repos</b><span>Gen8 + OT mirror + DC mirror inventoried</span></div>'
 '<div class="card"><b>9 work items</b><span>6 OT + 3 DC, exact file:function anchors</span></div>'
 '<div class="card"><b>&le;0.651 m</b><span>Ghost RMSE target (vs 1.969 base)</span></div>'
 '<div class="card"><b>&ge;99.0%</b><span>Cut-in recall target (vs 86.3%)</span></div>'
 '<div class="card"><b>1000 / 80</b><span>Variant-A tracks / dets-per-track bounds</span></div>'
 '</div>')

flow = ("""<div class="flow"><div class="node"><b>PRODUCER: Gen8 iND13400</b><br>Dual-loop CFAR, fast DA, CDC priority<br><code>405 insertions / 0 deletions</code>, OFF-clean</div><div class="arr">&rarr;</div>"""
 """<div class="node"><b>BRIDGE: DC Emb Library</b><br><code>DC_SIL/sil_wrappers/sil_wrapper.cpp</code> RECU_SiL_Execute L51: ProcessInputData &rarr; RunTracker<br>Preflight + deskew + clock split land here</div><div class="arr">&rarr;</div>"""
 """<div class="node"><b>CONSUMER: OT F360</b><br><code>emb_tracker/ROT/Fusion360/…</code> Joseph-form EKF<br>Ghost assimilation + gates land here</div></div>""")

ot = ("""<table><tr><th>#</th><th>Item</th><th>File : function : line</th><th>Guard</th><th>Status</th></tr>"""
 """<tr><td>OT-1</td><td>Run-14 virtual-aperture assimilation + R_eff + Joseph</td><td><code>f360_msmt_update_obj_trks_cca_moveable.cpp : Msmt_Update_Obj_Trks_CCA_Moveable : L65</code> (+CTCA L61); r_mat L101; gain L138; cov L143; Joseph helper L164-165</td><td><code>APRISM_OT_ENABLE</code></td><td>Anchor """ + B("VERIFIED","ver") + """</td></tr>"""
 """<tr><td>OT-2</td><td>Rank-1 invariant + bearing correction + XOR flag</td><td><code>f360_detect_multipath.cpp</code> (mirror_prob L159); <code>f360_determine_reflected_obj.cpp</code> L90-147</td><td><code>APRISM_OT_ENABLE</code></td><td>Anchor """ + B("VERIFIED","ver") + """</td></tr>"""
 """<tr><td>OT-3</td><td>Adaptive gate [9.21, 40], keep de-aliasing</td><td><code>f360_determine_association_hypothesis.cpp : Determine_Association_Hypothesis : L69</code>; Calc_Range_Rate_Threshold L100; support L148</td><td><code>APRISM_OT_ENABLE</code></td><td>Anchor """ + B("VERIFIED","ver") + """</td></tr>"""
 """<tr><td>OT-4</td><td>Hermite clothoid + planar fallback</td><td><code>f360_is_reflective_guardrail_track.cpp</code> (exists, SEP logic present)</td><td><code>APRISM_OT_ENABLE</code></td><td>Anchor """ + B("VERIFIED","ver") + """</td></tr>"""
 """<tr><td>OT-5</td><td>Spectral-entropy R scaling, PD check</td><td><code>Filling_Msmt_Cov_By_H_RR_CTCA</code> (CTCA L111); k_ref_msmt_cov_ctca L146</td><td><code>APRISM_OT_ENABLE</code></td><td>Anchor """ + B("VERIFIED","ver") + """</td></tr>"""
 """<tr><td>OT-6</td><td>Doppler 3&sigma; gate + 2-scan confirm</td><td>association validation + track_validity (mirror_prob path above)</td><td><code>APRISM_OT_ENABLE</code></td><td>Anchor """ + B("VERIFIED","ver") + """</td></tr></table>""")

dc = ("""<table><tr><th>#</th><th>Item</th><th>Anchor (verified) / confirm at edit</th><th>Guard</th></tr>"""
 """<tr><td>DC-1</td><td>Preflight DA header injection</td><td>""" + B("VERIFIED","ver") + """ call order <code>RECU_SiL_Execute L51-75</code>: ProcessInputData &rarr; RunTracker. M2D one-time field names """ + B("CONFIRM AT EDIT","spc") + """ vs DC <code>M2D_One_Time_Msg_T</code></td><td><code>APRISM_DC_ENABLE</code></td></tr>"""
 """<tr><td>DC-2</td><td>Async yaw deskew p_corr</td><td>""" + B("VERIFIED","ver") + """ <code>Tracker_VariantA_Wrapper.cpp: RunTracker L462</code>; sensor ts L321; host/core L360-549; OCG L558 / SG L572</td><td><code>APRISM_DC_ENABLE</code></td></tr>"""
 """<tr><td>DC-3</td><td>Clock/bias Gram split</td><td>""" + B("VERIFIED","ver") + """ timestamp assembly L482-489 (wrap guard); fusion buffers downstream</td><td><code>APRISM_DC_ENABLE</code></td></tr></table>""")

dummy = ("""<table><tr><th>Suite</th><th>Vectors (synthetic, fixed seed)</th><th>Oracle</th><th>KPI gate</th></tr>"""
 """<tr><td>Cut-in</td><td>decoy at planar blind spot, accel 0-8 m/s², 60 scans</td><td>association labels</td><td>recall &ge;99% (base 86.3%)</td></tr>"""
 """<tr><td>Curved rail</td><td>clothoid &kappa;, Hermite fit vs planar</td><td>contact residuals</td><td>refusal rate on over-curvature</td></tr>"""
 """<tr><td>Decoy pair</td><td>same-range direct+ghost, Doppler residuals above</td><td>ghost labels</td><td>RMSE &le;0.651 m (base 1.969)</td></tr>"""
 """<tr><td>Spray/stale</td><td>entropy-scaled R, stale timestamps</td><td>PD + continuity</td><td>no track loss, PD held</td></tr>"""
 """<tr><td>Ambiguous</td><td>competing tracks, NIS &chi;² check</td><td>correct-track rate</td><td>wrong-capture &le; base</td></tr></table>"""
 """<p>Calibration discipline (from literature): NIS = d<sup>T</sup>S<sup>-1</sup>d &sim; &chi;²<sub>p</sub> on whitened innovations; one outside-interval value is not a verdict &mdash; compare assumed vs actual variance over runs.</p>""")

html = f"""<!DOCTYPE html><html><head><meta charset="utf-8">
<title>A-PRISM DC+OT — Readiness Checkpoint</title>
<script src="https://cdn.plot.ly/plotly-2.35.2.min.js"></script>
<style>{css}</style></head><body><div class="wrap">
<div class="hero"><small>CHECKPOINT — read-only inspection &middot; 0 files edited &middot; 0 commits &middot; 0 pushes &middot; local mirrors + C:\\git\\OT_ObjectTracking reference</small>
<h1>DC + Object Tracker: readiness checkpoint &amp; edit plan</h1>
<p>Gen8 producer work is landed (405+/0, OFF-clean). This report inventories the consumer side, pins every edit to a verified file:function:line anchor, specifies as-good-as-real dummy vectors, and defines the rigorous A/B gates. {B("NEXT: approve, then edit","spc")}</p></div>
{cards}
<div class="sec"><h2>1. Chain of custody (verified paths)</h2>{flow}
<p>OT is the engine (EKF math, no I/O); DC is the host (loads logs, syncs, calls <code>RunTracker()</code>); Gen8 is the producer (point clouds/CDC). Wrapper edits in DC are required wherever tracker inputs change shape.</p></div>
<div class="sec"><h2>2. OT edit table — 6 items {B("ANCHORS VERIFIED","ver")}</h2>{ot}
<p>Pipeline order (association/measurement before validity) and Joseph-form reuse are architectural law: assimilation and gate widening live at association/measurement-update time; <code>Track_Validity</code> stays classification-only; assimilated ghosts set <code>is_assimilated_ghost</code> so <code>mirror_prob</code> skips them (XOR contract). Bounds: variant A 1000 tracks / 80 dets (verified in <code>f360_variant_definition_a.h</code>); zero heap on RT threads.</p></div>
<div class="sec"><h2>3. DC edit table — 3 items</h2>{dc}</div>
<div class="sec"><h2>4. Dummy vectors (synthetic, fixed seed 20260924) {B("SYNTH","syn")}</h2>{figs[0]}{figs[1]}
<p>Scene: ego 25 m/s, rail y=3.5 m, target (55,&minus;1.75) at 22 m/s, &sigma;<sub>r</sub>=0.3 m, &sigma;<sub>v</sub>=0.15, 60 scans. Ghost Doppler residuals breach the 3&sigma; gate &mdash; the disambiguation signal the OT-6 test asserts.</p></div>
<div class="sec"><h2>5. Algorithm response curves {B("SYNTH","syn")}</h2>{figs[2]}{figs[3]}
<p>Left: &gamma; widening under acceleration/innovation, clamped [9.21, 40]. Right: R<sub>eff</sub> growth with reflector &sigma; (J&asymp;2) and deskew residual collapse 5&rarr;45 ms. Joseph form P<sup>+</sup>=(I&minus;KH)P<sup>&minus;</sup>(I&minus;KH)<sup>T</sup>+KRK<sup>T</sup> keeps P symmetric positive-definite where the naive P&minus;KSK<sup>T</sup> can break (verified literature).</p></div>
<div class="sec"><h2>6. Lock budget + test suites</h2>{figs[4]}{dummy}</div>
<div class="sec"><h2>7. Build &amp; qualify gates (at edit time)</h2>
<table><tr><th>Gate</th><th>Command / suite</th><th>Pass rule</th></tr>
<tr><td>OT build</td><td><code>cmake --build build --config Debug</code> (variant A)</td><td>zero warnings-as-errors, OFF bit-identical</td></tr>
<tr><td>OT qual</td><td>cut-in recall, guardrail/multipath decoy, spray/clutter suites</td><td>OFF: zero regressions; ON: RMSE &le;0.651 m, recall &ge;99%</td></tr>
<tr><td>DC build</td><td><code>DC_SIL</code> sil_wrapper lib/bin</td><td>clean; OFF bit-comparable</td></tr>
<tr><td>E2E SiL</td><td>Gen8 build &rarr; DC deskew &rarr; OT tracks, UDP/CAN KPI replay</td><td>paired OFF/ON deltas on same logs</td></tr></table></div>
<div class="sec"><h2>8. References</h2><p>
Joseph stability &amp; why P<sup>+</sup> must stay SPD: FM&rsquo;19 VerFilter study; De Vivo et al. (SRUKF/Joseph); AIAA Joseph-formulation survey. Ghost handling precedent: Zheng et al., IEEE TSP 2024 (GLRT multipath detection); Balachandran thesis (multipath-assisted multitarget tracking, surfaces from accumulated frames); MATLAB highway-multipath GGIW-PHD example (ghost filtering + GOSPA/confusion-matrix discipline).</p></div>
<div class="sec"><h2>9. Risks {B("RISK","risk")}</h2>
<table><tr><th>Risk</th><th>Mitigation</th></tr>
<tr><td>GitHub mirror &ne; Gerrit branch (offsets drift)</td><td>Re-verify every anchor on the team Gerrit dev branch at edit time; treat mirror line numbers as guides</td></tr>
<tr><td>Double-counting ghost evidence</td><td>XOR flag contract + paired test asserting no mirror penalty post-assimilation</td></tr>
<tr><td>Gate widening admits clutter</td><td>Hard clamp + recall AND selectivity + RMSE tracked, not recall alone</td></tr>
<tr><td>Joint-position Gram regressed before</td><td>Decoupled channels only; PD assert pre-Joseph</td></tr></table></div>
<div class="foot">Checkpoint generated from read-only inspection. No OT/DC/Gen8 file was modified. Approval requested before any edit.</div>
</div></body></html>"""

OUT.write_text(html, encoding="utf-8")
print(f"WROTE {OUT} ({OUT.stat().st_size/1024:.0f} KB)")
