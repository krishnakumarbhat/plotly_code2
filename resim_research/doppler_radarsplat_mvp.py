"""Doppler-RadarSplat MVP (Frontier / N7).

Gap (swarm-verified): NeuRadar (CVPRW 2025) omits radial Doppler, RCS/SNR,
and CDC synthesis; RadarSplat variants synthesize RA maps or consume Doppler
but none project per-primitive velocity + R^4 power into Range-Doppler CDC
bins. This MVP does exactly that:

    v_r,i = (v_i - v_ego) . (mu_i - p_s) / ||mu_i - p_s||
    P_r,i = Pt G^2 l^2 sig_RCS,i / ((4pi)^3 . ||mu_i - p_s||^4)

then splats each primitive as a 2-D Gaussian into (range, doppler) CDC bins
(sr = range extent, sd = velocity spread -> detection flicker). Demo checks:
(1) R^4 law between two identical primitives at different ranges;
(2) peak-extracted (r, v_r) match analytic projections within one bin;
(3) primitive recovery rate on a 6-target scene with clutter primitives.
# ponytail: static primitives + single sensor; ego-motion streaking is next.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

C = 3e8
FC = 77e9
LAM = C / FC
PT, G = 1.0, 100.0          # EIRP-ish constants (relative power only)
RMIN, RMAX, DR = 5.0, 60.0, 0.2
VMIN, VMAX, DV = -30.0, 30.0, 0.1
NR = int((RMAX - RMIN) / DR)
ND = int((VMAX - VMIN) / DV)


class RadarPrimitive:
    """mu (3,), vel (3,), rcs (m^2), sr/sd splat widths, amp flicker."""
    def __init__(self, mu, vel, rcs, sr=0.3, sd=0.15, amp=1.0):
        self.mu = np.asarray(mu, float)
        self.vel = np.asarray(vel, float)
        self.rcs = float(rcs)
        self.sr, self.sd, self.amp = sr, sd, amp


def project(prim, p_sens, v_ego):
    """Analytic (range, radial Doppler, received power) for one primitive."""
    d = prim.mu - p_sens
    r = float(np.linalg.norm(d))
    u = d / max(r, 1e-9)
    vr = float((prim.vel - v_ego) @ u)
    pr = PT * G ** 2 * LAM ** 2 * prim.rcs / ((4 * np.pi) ** 3 * r ** 4)
    return r, vr, pr


def render(prims, p_sens, v_ego):
    """Splat all primitives into the Range-Doppler power map."""
    rd = np.zeros((NR, ND))
    r_axis = RMIN + (np.arange(NR) + 0.5) * DR
    v_axis = VMIN + (np.arange(ND) + 0.5) * DV
    for prim in prims:
        r, vr, pr = project(prim, p_sens, v_ego)
        if not (RMIN <= r <= RMAX and VMIN <= vr <= VMAX):
            continue
        gr = np.exp(-0.5 * ((r_axis - r) / prim.sr) ** 2)
        gd = np.exp(-0.5 * ((v_axis - vr) / prim.sd) ** 2)
        rd += prim.amp * pr * np.outer(gr, gd)
    return rd


def extract_peaks(rd, thresh_rel=0.05, min_sep_r=2, min_sep_d=2,
                  clean=False, sr=0.3, sd=0.15, noise_floor=1e-11,
                  max_iter=200):
    """Greedy local-max peak extraction with cell suppression. With
    clean=True, CLEAN-style successive cancellation: subtract each detected
    peak's modeled splat before re-searching (answers near-far masking).
    Terminates at the absolute noise floor (never tracks residuals to zero)."""
    work = rd.copy()
    r_axis = RMIN + (np.arange(NR) + 0.5) * DR
    v_axis = VMIN + (np.arange(ND) + 0.5) * DV
    peaks = []
    for _ in range(max_iter):
        thr = max(thresh_rel * work.max(), noise_floor)
        k = int(np.argmax(work))
        i, j = k // ND, k % ND
        if work[i, j] < thr:
            break
        rk, vk, ak = (RMIN + (i + 0.5) * DR, VMIN + (j + 0.5) * DV, work[i, j])
        peaks.append((rk, vk, ak))
        if clean:
            gr = np.exp(-0.5 * ((r_axis - rk) / sr) ** 2)
            gd = np.exp(-0.5 * ((v_axis - vk) / sd) ** 2)
            work -= ak * np.outer(gr, gd)
            work = np.maximum(work, 0.0)
        else:
            i0, i1 = max(0, i - min_sep_r), min(NR, i + min_sep_r + 1)
            j0, j1 = max(0, j - min_sep_d), min(ND, j + min_sep_d + 1)
            work[i0:i1, j0:j1] = 0.0
    return peaks


def demo() -> None:
    p_sens = np.zeros(3)
    v_ego = np.array([25.0, 0.0, 0.0])
    # --- (1) R^4 law: identical primitives at 10 m and 40 m
    a = RadarPrimitive([10.0, 0.0, 0.0], [10.0, 0.0, 0.0], rcs=10.0)
    b = RadarPrimitive([40.0, 0.0, 0.0], [10.0, 0.0, 0.0], rcs=10.0)
    _, _, pa = project(a, p_sens, v_ego)
    _, _, pb = project(b, p_sens, v_ego)
    ratio = pa / pb
    print(f"[R4] P(10m)/P(40m) = {ratio:.1f} (theory 256.0)")
    assert abs(ratio - 256.0) / 256.0 < 0.05, "range equation violated"
    # --- (1b) differentiability: analytic Jacobians vs finite differences
    t = RadarPrimitive([20.0, 5.0, 0.0], [8.0, 2.0, 0.0], rcs=10.0)
    d = t.mu - p_sens
    r = float(np.linalg.norm(d))
    u = d / r
    w = t.vel - v_ego
    g_vr = (w - float(w @ u) * u) / r
    C0 = PT * G ** 2 * LAM ** 2 * t.rcs / (4 * np.pi) ** 3
    g_pr = -4.0 * C0 * u / r ** 5
    h = 1e-6
    fd_vr = np.array([(project(RadarPrimitive(t.mu + h * e, t.vel, t.rcs),
                               p_sens, v_ego)[1]
                       - project(RadarPrimitive(t.mu - h * e, t.vel, t.rcs),
                                 p_sens, v_ego)[1]) / (2 * h)
                      for e in np.eye(3)])
    fd_pr = np.array([(project(RadarPrimitive(t.mu + h * e, t.vel, t.rcs),
                               p_sens, v_ego)[2]
                       - project(RadarPrimitive(t.mu - h * e, t.vel, t.rcs),
                                 p_sens, v_ego)[2]) / (2 * h)
                      for e in np.eye(3)])
    ev = float(np.linalg.norm(fd_vr - g_vr) / max(np.linalg.norm(g_vr), 1e-12))
    ep = float(np.linalg.norm(fd_pr - g_pr) / max(np.linalg.norm(g_pr), 1e-12))
    print(f"[grad] dvr/dmu relerr={ev:.2e} dPr/dmu relerr={ep:.2e}")
    assert ev < 1e-4 and ep < 1e-4, "projection must be differentiable"
    # --- (2) projection accuracy on a 6-target scene, no clutter
    rng = np.random.default_rng(0)
    prims = [RadarPrimitive([rng.uniform(8, 55), rng.uniform(-20, 20), 0.0],
                            [rng.uniform(-5, 15), rng.uniform(-6, 6), 0.0],
                            rcs=rng.uniform(1.0, 30.0))
             for _ in range(6)]
    rd = render(prims, p_sens, v_ego)
    peaks = extract_peaks(rd, clean=True)
    ana = [project(p, p_sens, v_ego)[:2] for p in prims]
    matched, errs_r, errs_v = 0, [], []
    for r0, v0 in ana:
        d = [abs(pk[0] - r0) + 3.0 * abs(pk[1] - v0) for pk in peaks]
        k = int(np.argmin(d))
        if abs(peaks[k][0] - r0) <= DR and abs(peaks[k][1] - v0) <= DV:
            matched += 1
            errs_r.append(abs(peaks[k][0] - r0))
            errs_v.append(abs(peaks[k][1] - v0))
    rec = matched / len(prims)
    print(f"[proj] recovery={rec:.3f} rangeRMSE={np.mean(errs_r):.3f} m "
          f"dopplerRMSE={np.mean(errs_v):.3f} m/s")
    assert rec >= 5.0 / 6.0, "must recover nearly all primitives"
    # --- (3) with 30 clutter primitives (low RCS, random velocity)
    cl = [RadarPrimitive([rng.uniform(8, 55), rng.uniform(-20, 20), 0.0],
                         [rng.uniform(-10, 10), rng.uniform(-10, 10), 0.0],
                         rcs=rng.uniform(0.05, 0.5)) for _ in range(30)]
    rd2 = render(prims + cl, p_sens, v_ego)
    peaks2 = extract_peaks(rd2)
    matched2 = 0
    for r0, v0 in ana:
        if any(abs(pk[0] - r0) <= 2 * DR and abs(pk[1] - v0) <= 2 * DV
               for pk in peaks2):
            matched2 += 1
    rec2 = matched2 / len(prims)
    print(f"[clutter] recovery={rec2:.3f} with 30 clutter prims")
    assert rec2 >= 5.0 / 6.0, "clutter must not hide real primitives"
    print("demo PASS")


if __name__ == "__main__":
    demo()
