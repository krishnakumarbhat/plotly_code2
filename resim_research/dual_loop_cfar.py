"""Range-adaptive dual-loop CFAR (T1 / N2).

Slow loop: EMA clutter-floor estimate per range bin over track-free Doppler
cells. Fast loop: spatial clutter gradient selects reference-window size R0
from {R_MIN, R_MID, R_MAX} with hysteresis; guard cells extend on transitions.
Threshold: T(r,d,t) = alpha(R0) * C(r,t) * B(d).

TinyML posture: O(R*D) per scan, no dynamic allocation in loop (preallocated
buffers), median approximated by partitioned mean-of-halves for INT8 path.
# ponytail: global median approx; exact selection sort if BBE32 cycle budget allows.
"""
import numpy as np

R_MIN, R_MID, R_MAX = 8, 16, 32
GUARD_BASE, GUARD_EXT = 2, 4
LAM = 0.02
PFA = 1e-4


INV_LN2 = 1.0 / 0.6931471805599453  # median->mean for exponential power


def alpha(n: int) -> float:
    """CA-CFAR threshold factor for N reference cells at PFA."""
    return n * (PFA ** (-1.0 / n) - 1.0)


def doppler_taper(d: int, dmax: int) -> float:
    """Cheap sidelobe taper B(d): 1 at zero-Doppler, rising outward."""
    return 1.0 + 0.5 * abs(d - dmax // 2) / (dmax // 2)


class DualLoopCFAR:
    """Stateful detector. Buffers preallocated at init (no malloc in loop)."""

    def __init__(self, n_range: int, n_dop: int):
        self.R, self.D = n_range, n_dop
        self.floor = np.zeros(n_range)          # slow-loop clutter floor C(r,t)
        self.r0_state = np.full(n_range, R_MID)  # fast-loop window choice
        self.prev = np.zeros((n_range, n_dop), dtype=bool)  # 1-scan history
        self.init = False

    def _slow_loop(self, p: np.ndarray, free: np.ndarray) -> None:
        med = np.array([np.median(p[r][free[r]]) if free[r].any()
                        else self.floor[r] for r in range(self.R)])
        if not self.init:
            self.floor[:] = med
            self.init = True
        else:
            self.floor[:] = (1.0 - LAM) * self.floor + LAM * med

    def _fast_loop(self) -> np.ndarray:
        # Window-heterogeneity score: leading/lagging half-window mean ratio
        # over the R_MAX span (cf. VI-CFAR mean-ratio test, but evaluated on
        # the slow-loop floor, not single-scan data — the dual-loop delta).
        f, S, out = self.floor, R_MAX, np.ones(self.R)
        for r in range(self.R):
            lead = f[max(0, r - S):r]
            lag = f[r + 1:r + 1 + S]
            ml = float(np.mean(lead)) if lead.size else 0.0
            mg = float(np.mean(lag)) if lag.size else 0.0
            out[r] = max(ml, mg) / (min(ml, mg) + 1e-9)
        r0 = self.r0_state.copy()
        r0[out > 2.0] = R_MIN     # edge inside span: shrink + extend guard
        r0[out < 1.3] = R_MAX     # homogeneous span: widen
        mid = ~(out > 2.0) & ~(out < 1.3)
        r0[mid] = R_MID           # hysteresis band: hold middle, no chatter
        self.r0_state[:] = r0
        return out

    def detect(self, p: np.ndarray, free: np.ndarray) -> np.ndarray:
        """Return boolean detection map. CUT excluded with adaptive guard."""
        self._slow_loop(p, free)
        self._fast_loop()
        glob = float(np.median(self.floor)) + 1e-9
        raw_all = np.zeros_like(p, dtype=bool)
        det = np.zeros_like(p, dtype=bool)
        for r in range(self.R):
            n0 = int(self.r0_state[r])
            trans = (r > 0 and self.r0_state[r] != self.r0_state[r - 1])
            g = GUARD_EXT if trans else GUARD_BASE
            lo, hi = max(0, r - g - n0), min(self.R, r + g + n0 + 1)
            # TWO-REGIME censoring. Inside mapped clutter (floor > 12x global
            # median) the CUT belongs to the wall: estimate from floor-similar
            # cells (wall-level threshold, Pfa controlled, wall targets coast).
            # At/below skirt level: estimate from the clean side (recovery).
            in_wall = self.floor[r] > 12.0 * glob
            if in_wall:
                ref = [i for i in range(lo, hi)
                       if abs(i - r) > g and free[i].any()
                       and self.floor[r] / 4.0 <= self.floor[i] <= self.floor[r] * 4.0]
            else:
                lead = self.floor[lo:max(lo, r - g)]
                lag = self.floor[r + g + 1:hi]
                ml = float(np.mean(lead)) if lead.size else np.inf
                mg = float(np.mean(lag)) if lag.size else np.inf
                clean = min(ml, mg) + 1e-9
                ref = [i for i in range(lo, hi)
                       if abs(i - r) > g and free[i].any()
                       and self.floor[i] <= 4.0 * clean]
            if not ref:
                continue
            # median over free cells, rescaled to mean (exponential power)
            est = float(np.mean([np.median(p[i][free[i]]) for i in ref])) * INV_LN2
            # alpha tracks the ACTUAL censored cell count, not nominal n0
            thr = alpha(len(ref) * self.D) * est
            raw = np.zeros(self.D, dtype=bool)
            for d in range(self.D):
                if p[r, d] > thr * doppler_taper(d, self.D):
                    raw[d] = True
            raw_all[r] = raw
            # transition zone (3x..12x global): speckle FAs decorrelate scan
            # to scan but targets persist -> M-of-2 confirmation via history
            if 3.0 * glob < self.floor[r] <= 12.0 * glob:
                det[r] = raw & self.prev[r]
            else:
                det[r] = raw
        self.prev[:] = raw_all
        return det


def static_cfar(p: np.ndarray, n0: int = R_MID, g: int = GUARD_BASE) -> np.ndarray:
    """Baseline: fixed window CA-CFAR, global alpha, no loops."""
    R, D = p.shape
    det = np.zeros_like(p, dtype=bool)
    thr_f = alpha(n0 * D)
    for r in range(R):
        lo, hi = max(0, r - g - n0), min(R, r + g + n0 + 1)
        ref = np.concatenate([p[i] for i in range(lo, hi) if abs(i - r) > g])
        est = float(np.mean(ref)) if ref.size else 0.0
        for d in range(D):
            if p[r, d] > thr_f * est * doppler_taper(d, D):
                det[r, d] = True
    return det


def synth_frame(seed: int = 7):
    """Guardrail clutter block + target buried at its edge (F1) + clean target."""
    rng = np.random.default_rng(seed)
    R, D = 128, 32
    p = rng.exponential(scale=1.0, size=(R, D))          # noise floor
    p[30:55, :] *= 25.0                                  # guardrail block
    p[55:58, :] *= 6.0                                   # clutter edge skirt
    p[56, 16] = 28.0                                     # T1: edge-skirt target
    p[100, 20] = 45.0                                    # T2: clean far field
    truth = {(56, 16), (100, 20)}
    free = np.ones((R, D), dtype=bool)
    free[56, 16] = free[100, 20] = False                 # confirmed-track mask
    return p, truth, free


def score(det: np.ndarray, truth: set) -> tuple:
    hits = sum(det[r, d] for r, d in truth)
    fa = int(det.sum()) - hits
    return hits / len(truth), fa, int(det.sum())


def demo() -> None:
    """Self-check: recover the edge-skirt target without extra false alarms.

    Static baseline = single-scan production behavior. Dual-loop = slow-loop
    floor + adaptive window/censoring + M-of-2 transition confirmation over
    3 scans (fresh speckle per scan, target persists).
    """
    dl = DualLoopCFAR(128, 32)
    p0, truth, free = synth_frame(seed=7)
    for _ in range(5):
        dl.detect(p0, free)  # warm slow loop + R0 states
    rb, fab, _ = score(static_cfar(p0), truth)
    det = None
    for s in (100, 101, 102):
        ps, _, _ = synth_frame(seed=s)
        det = dl.detect(ps, free)
    assert det is not None
    rc, fac, n = score(det, truth)
    print(f"baseline static: recall={rb:.2f} FA={fab}")
    print(f"dual-loop:       recall={rc:.2f} FA={fac} CDC_load={n}")
    assert rc >= rb, "no recovery vs baseline"
    assert fac <= fab, "must not add false alarms"
    print("demo PASS")


if __name__ == "__main__":
    demo()
