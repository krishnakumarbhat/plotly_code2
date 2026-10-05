"""IMM acceleration-covariance gate vs fixed and N5 energy gates (N15).

N15 recipe: r_gate = sqrt(CHI2 + k*sigma_a*|a_IMM|) from a CV/CA IMM pair;
expands only on accel spike (model-probability grounded). Same cut-in
scene + clutter + seeds as Run 6 (`adaptive_gating`); N5 gate reused
verbatim via CVTracker for a fair comparison.
# ponytail: 2-model IMM, shared trial harness, k frozen on warm seeds.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

from resim_research.adaptive_gating import (
    CVTracker, cutin_truth, DT, SIG_M, G0, GMIN, GMAX, ALPHA, ETA, A_MAX,
)

CHI2 = G0
PI = np.array([[0.95, 0.05], [0.05, 0.95]])  # CV->CV, CV->CA; CA->CV, CA->CA
Q_CA = 9.0  # CA process-noise intensity (m/s^2)^2/Hz


def ca_mats():
    """CA transition + process noise (white jerk)."""
    F = np.eye(6)
    F[0, 2] = F[1, 3] = DT
    F[0, 4] = F[1, 5] = 0.5 * DT ** 2
    F[2, 4] = F[3, 5] = DT
    G = np.zeros((6, 2))
    G[0, 0] = G[1, 1] = DT ** 2 / 2.0
    G[2, 0] = G[3, 1] = DT
    G[4, 0] = G[5, 1] = 1.0
    return F, G @ G.T * Q_CA


F_CA, Q_CA_M = ca_mats()
F_CV = np.eye(4)
F_CV[0, 2] = F_CV[1, 3] = DT
Q_CV = np.diag([0.001, 0.001, 0.01, 0.01])
H_CV = np.zeros((2, 4))
H_CV[0, 0] = H_CV[1, 1] = 1.0
H_CA = np.zeros((2, 6))
H_CA[0, 0] = H_CA[1, 1] = 1.0
R_M = np.eye(2) * SIG_M ** 2


class IMMTracker:
    def __init__(self, k):
        self.k = k
        self.x_cv = np.zeros(4)
        self.P_cv = np.diag([1.0, 1.0, 25.0, 25.0])
        self.x_ca = np.zeros(6)
        self.P_ca = np.diag([1.0, 1.0, 25.0, 25.0, 36.0, 36.0])
        self.mu = np.array([0.9, 0.1])
        self.init = False

    def _mix(self):
        c = PI.T @ self.mu
        mu_mix = (PI * self.mu[:, None]).T / np.maximum(c, 1e-12)
        # CV mixed <- CV only (states differ in dim; CA->CV via projection)
        W = np.zeros((4, 6))
        W[:4, :4] = np.eye(4)
        x_cv_m = mu_mix[0, 0] * self.x_cv + mu_mix[0, 1] * (W @ self.x_ca)
        P_cv_m = mu_mix[0, 0] * (self.P_cv + np.outer(self.x_cv - x_cv_m, self.x_cv - x_cv_m)) \
            + mu_mix[0, 1] * (W @ self.P_ca @ W.T + np.outer(W @ self.x_ca - x_cv_m, W @ self.x_ca - x_cv_m))
        E = np.zeros((6, 4))
        E[:4, :4] = np.eye(4)
        x_ca_m = mu_mix[1, 0] * (E @ self.x_cv) + mu_mix[1, 1] * self.x_ca
        P_ca_m = mu_mix[1, 0] * (E @ self.P_cv @ E.T + np.outer(E @ self.x_cv - x_ca_m, E @ self.x_cv - x_ca_m)) \
            + mu_mix[1, 1] * (self.P_ca + np.outer(self.x_ca - x_ca_m, self.x_ca - x_ca_m))
        return (x_cv_m, P_cv_m), (x_ca_m, P_ca_m), c

    def step(self, z, gate_on):
        if not self.init:
            self.x_cv[:2] = z
            self.x_ca[:2] = z
            self.init = True
            return True, CHI2
        (x_cv_m, P_cv_m), (x_ca_m, P_ca_m), c = self._mix()
        # predict
        x_cv_p = F_CV @ x_cv_m
        P_cv_p = F_CV @ P_cv_m @ F_CV.T + Q_CV
        x_ca_p = F_CA @ x_ca_m
        P_ca_p = F_CA @ P_ca_m @ F_CA.T + Q_CA_M
        # combined prediction for association
        w = c  # predicted model weights
        xp = w[0] * x_cv_p + w[1] * x_ca_p[[0, 1, 2, 3]]
        Pp = w[0] * P_cv_p + w[1] * P_ca_p[np.ix_([0, 1, 2, 3], [0, 1, 2, 3])]
        S = H_CV @ Pp @ H_CV.T + R_M
        y = z - H_CV @ xp
        d2 = float(y @ np.linalg.inv(S) @ y)
        # IMM accel belief (prior, no oracle): model-spread covariance
        a_ca = x_ca_p[4:6]
        a_imm = w[1] * a_ca
        a_cv0 = np.zeros(2)
        abar = w[0] * a_cv0 + w[1] * a_ca
        Pa = w[0] * (np.zeros((2, 2)) + np.outer(a_cv0 - abar, a_cv0 - abar)) \
            + w[1] * (P_ca_p[4:6, 4:6] + np.outer(a_ca - abar, a_ca - abar))
        siga = float(np.sqrt(max(np.trace(Pa) / 2.0, 0.0)))
        if gate_on == "imm":
            g = CHI2 + self.k * siga * float(np.linalg.norm(a_imm))
        elif gate_on == "hybrid":
            # N5 energy structure, IMM accel replaces finite differences
            g = G0 * (1.0 + ALPHA * min(float(np.linalg.norm(a_imm)), A_MAX) / A_MAX
                      + ETA * d2 / max(float(np.trace(S)), 1e-9))
            g = float(min(max(g, GMIN), GMAX))
        else:
            g = CHI2
        if d2 > g:
            # coast: keep prediction, freeze weights
            self.x_cv, self.P_cv = x_cv_p, P_cv_p
            self.x_ca, self.P_ca = x_ca_p, P_ca_p
            return False, float(g)
        # update both models
        K_cv = P_cv_p @ H_CV.T @ np.linalg.inv(H_CV @ P_cv_p @ H_CV.T + R_M)
        K_ca = P_ca_p @ H_CA.T @ np.linalg.inv(H_CA @ P_ca_p @ H_CA.T + R_M)
        y_cv = z - H_CV @ x_cv_p
        y_ca = z - H_CA @ x_ca_p
        self.x_cv = x_cv_p + K_cv @ y_cv
        self.P_cv = (np.eye(4) - K_cv @ H_CV) @ P_cv_p
        self.x_ca = x_ca_p + K_ca @ y_ca
        self.P_ca = (np.eye(6) - K_ca @ H_CA) @ P_ca_p
        # likelihoods -> model probs
        def gauss(y_, S_):
            return float(np.exp(-0.5 * y_ @ np.linalg.inv(S_) @ y_)
                         / np.sqrt((2 * np.pi) ** 2 * max(np.linalg.det(S_), 1e-18)))
        L = np.array([gauss(y_cv, H_CV @ P_cv_p @ H_CV.T + R_M),
                      gauss(y_ca, H_CA @ P_ca_p @ H_CA.T + R_M)])
        self.mu = c * L / max(float(c @ L), 1e-300)
        return True, float(g)

    def coast(self):
        self.x_cv = F_CV @ self.x_cv
        self.P_cv = F_CV @ self.P_cv @ F_CV.T + Q_CV
        self.x_ca = F_CA @ self.x_ca
        self.P_ca = F_CA @ self.P_ca @ F_CA.T + Q_CA_M

    @property
    def pos(self):
        w = self.mu
        return w[0] * self.x_cv[:2] + w[1] * self.x_ca[:2]


def run_trial_imm(seed, k, mode="imm", maneuver=(1.0, 2.0), n_clutter=3, box=5.0):
    """Same harness as Run 6; mode in {imm, hybrid}. Returns (recall, RMSE)."""
    rng = np.random.default_rng(seed)
    trk = IMMTracker(k)
    hits, total, errs = 0, 0, []
    t = 0.0
    while t < 3.0:
        p = cutin_truth(t)
        cands = [p + rng.normal(0.0, SIG_M, 2)]
        pred = trk.pos if trk.init else p
        for _ in range(n_clutter):
            cands.append(pred + rng.uniform(-box, box, 2))
        if maneuver[0] <= t <= maneuver[1]:
            total += 1
            order = sorted(cands, key=lambda c: float(np.linalg.norm(c - pred)))
            associated = False
            for z in order:
                ok, _ = trk.step(z, mode)
                if ok:
                    associated = True
                    hits += int(np.linalg.norm(z - p) < 1.0)
                    break
            if not associated:
                trk.coast()
            errs.append(float(np.linalg.norm(trk.pos - p)))
        else:
            trk.step(cands[0], mode)
        t += DT
    return hits / max(total, 1), float(np.mean(errs))


def demo() -> None:
    from resim_research.adaptive_gating import run_trial
    warm = [0, 1, 2]
    for k in (0.25, 0.5, 1.0):  # k-insensitivity documents the onset problem
        r = float(np.mean([run_trial_imm(s, k)[0] for s in warm]))
        print(f"[warm] k={k:.2f} recall={r:.3f}")
    print("[warm] k frozen at 0.5 (sweep is flat: gate never opens at onset)")
    seeds = range(100, 140)  # fresh seeds (disjoint from Run 6's 0-39)
    res_f = [run_trial(False, s) for s in seeds]
    res_n5 = [run_trial(True, s) for s in seeds]
    res_i = [run_trial_imm(s, 0.5, "imm") for s in seeds]
    res_h = [run_trial_imm(s, 0.5, "hybrid") for s in seeds]
    rf, ef = float(np.mean([r[0] for r in res_f])), float(np.mean([r[1] for r in res_f]))
    rn, en = float(np.mean([r[0] for r in res_n5])), float(np.mean([r[1] for r in res_n5]))
    ri, ei = float(np.mean([r[0] for r in res_i])), float(np.mean([r[1] for r in res_i]))
    rh, eh = float(np.mean([r[0] for r in res_h])), float(np.mean([r[1] for r in res_h]))
    print(f"fixed gate: handoff recall={rf:.3f} RMSE={ef:.3f} m")
    print(f"N5 energy:  handoff recall={rn:.3f} RMSE={en:.3f} m")
    print(f"IMM pure:   handoff recall={ri:.3f} RMSE={ei:.3f} m")
    print(f"IMM hybrid: handoff recall={rh:.3f} RMSE={eh:.3f} m")
    assert 0.83 <= rf <= 0.90, "Run-6 regime reproduced"
    assert rn >= 0.97 and rn - rf >= 0.05 and en < ef, "N5 reproduced"
    assert ri < rn - 0.03, "pure IMM loses: onset chicken-and-egg (REFUTED standalone)"
    assert rh >= 0.97 and eh < ef, "hybrid reaches operating point"
    print("demo PASS (N15 standalone REFUTED; hybrid == N5-class, grounding adds nothing)")


if __name__ == "__main__":
    demo()
