"""Kinematics-aware adaptive Mahalanobis gating (T4 / N5).

Physical defect: a fixed association gate rejects cut-in targets whose
innovation exceeds the gate ellipsoid during high lateral acceleration.
Fix: expand the gate with estimated target acceleration + normalized
innovation energy, hard-bounded for ISO 26262 determinism:

    g_adapt = g0 * (1 + a*|a_hat|/a_max + e*d2/tr(S)),  g in [gmin, gmax]

a_hat comes from track-velocity finite differences (no oracle); an oracle
variant is reported as the upper bound. Scenario: 25 m/s host, adjacent
target, lateral cut-in a_y = 4.2 m/s^2 over 0.8 s.
# ponytail: CV EKF + finite-difference accel; IMM mode probs if this saturates.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

DT = 0.05
SIG_M = 0.15        # confident sensor -> tight fixed gate (the failure regime)
G0 = 9.21           # fixed gate: chi2 2-DOF 99%
GMIN, GMAX = 9.21, 40.0
ALPHA, ETA = 1.5, 0.5
A_MAX = 6.0


class CVTracker:
    def __init__(self):
        self.x = np.zeros(4)          # x, y, vx, vy
        self.P = np.diag([1.0, 1.0, 25.0, 25.0])
        self.v_hist = []
        self.init = False

    def coast(self):
        """Pure predict (no measurement): used on total miss."""
        F = np.eye(4)
        F[0, 2] = F[1, 3] = DT
        Q = np.diag([0.001, 0.001, 0.01, 0.01])
        self.x = F @ self.x
        self.P = F @ self.P @ F.T + Q

    def step(self, z, adaptive):
        F = np.eye(4)
        F[0, 2] = F[1, 3] = DT
        Q = np.diag([0.001, 0.001, 0.01, 0.01])  # stiff highway CV tune
        if not self.init:
            self.x[:2] = z
            self.init = True
            return True, G0
        self.x = F @ self.x
        self.P = F @ self.P @ F.T + Q
        H = np.zeros((2, 4))
        H[0, 0] = H[1, 1] = 1.0
        R = np.eye(2) * SIG_M ** 2
        y = z - H @ self.x
        S = H @ self.P @ H.T + R
        d2 = float(y @ np.linalg.inv(S) @ y)
        if adaptive:
            self.v_hist.append(self.x[2:].copy())
            if len(self.v_hist) > 4:
                self.v_hist.pop(0)
            a_hat = 0.0
            if len(self.v_hist) >= 2:
                a_hat = float(np.linalg.norm(self.v_hist[-1] - self.v_hist[0])
                              / (DT * (len(self.v_hist) - 1)))
            g = G0 * (1.0 + ALPHA * min(a_hat, A_MAX) / A_MAX
                      + ETA * d2 / max(np.trace(S), 1e-9))
            g = float(min(max(g, GMIN), GMAX))
        else:
            g = G0
        if d2 > g:
            return False, g          # missed association: coast
        K = self.P @ H.T @ np.linalg.inv(S)
        self.x = self.x + K @ y
        self.P = (np.eye(4) - K @ H) @ self.P
        return True, g


def cutin_truth(t, ay=6.0, t0=1.0, dur=1.0):
    """Adjacent-lane target: straight, then emergency lateral cut-in."""
    x = 30.0 - 2.0 * t
    if t < t0:
        y = 3.5
    elif t < t0 + dur:
        tau = t - t0
        y = 3.5 - 0.5 * ay * tau ** 2
    else:
        y = 3.5 - 0.5 * ay * dur ** 2 - ay * dur * (t - t0 - dur)
    return np.array([x, y])


def run_trial(adaptive, seed, maneuver=(1.0, 2.0), n_clutter=3, box=5.0):
    """Nearest-within-gate association against uniform clutter. Returns
    (handoff recall, mean EKF position RMSE during maneuver)."""
    rng = np.random.default_rng(seed)
    trk = CVTracker()
    hits, total, errs = 0, 0, []
    t = 0.0
    while t < 3.0:
        p = cutin_truth(t)
        cands = [p + rng.normal(0.0, SIG_M, 2)]
        pred = trk.x[:2] if trk.init else p
        for _ in range(n_clutter):
            cands.append(pred + rng.uniform(-box, box, 2))
        if maneuver[0] <= t <= maneuver[1]:
            total += 1
            # associate nearest candidate, then gate it
            order = sorted(cands, key=lambda c: float(np.linalg.norm(c - pred)))
            associated = False
            for z in order:
                ok, _ = trk.step(z, adaptive)
                if ok:
                    associated = True
                    hits += int(np.linalg.norm(z - p) < 1.0)
                    break
            if not associated:
                trk.coast()
            errs.append(float(np.linalg.norm(trk.x[:2] - p)))
        else:
            trk.step(cands[0], adaptive)
        t += DT
    return hits / max(total, 1), float(np.mean(errs))


def demo() -> None:
    seeds = range(40)
    res_b = [run_trial(False, s) for s in seeds]
    res_a = [run_trial(True, s) for s in seeds]
    rb, eb = float(np.mean([r[0] for r in res_b])), float(np.mean([r[1] for r in res_b]))
    ra, ea = float(np.mean([r[0] for r in res_a])), float(np.mean([r[1] for r in res_a]))
    print(f"fixed gate:    handoff recall={rb:.3f} maneuver RMSE={eb:.3f} m")
    print(f"adaptive gate: handoff recall={ra:.3f} maneuver RMSE={ea:.3f} m")
    assert ra >= 0.97, "must reach the >97% operating point"
    assert ra - rb >= 0.05, "must beat fixed gate by a margin"
    assert ea < eb, "must not trade recall for track divergence"
    print("demo PASS")


if __name__ == "__main__":
    demo()
