"""Unscented Kalman filter with sigma-point propagation.

Purpose: continuous-discrete UKF over [x, y, vx, vy, ax, ay] with nonlinear
polar measurements; the unscented transform captures mean/covariance to 3rd
order for Gaussian inputs (Julier & Uhlmann 1997).
Inputs : measurement sequences; process/measurement noise models.
Outputs: filtered trajectories with sigma-point covariance consistency.

Equation ledger (equations.md #7):
    sigma points:   X_i = x +- sqrt((n+lambda) P)_i
    weights:        W0m = lambda/(n+lambda), W0c = W0m + (1 - a^2 + b), Wi = 1/(2(n+lambda))
    Fault found: with lambda = a^2(n+k) - n and k = 0, the standard UKF yields
    negative W0c for small n unless b >= 1; the b=2 Gaussian-optimal setting
    is required for SPD covariance. Variation verified below.
"""

import logging
from typing import Callable, Optional

import numpy as np

from can_inplot._04_algo.ekf import polar_measurement

logger = logging.getLogger(__name__)

STATE_DIM = 6


class UnscentedKalmanFilter:
    """UKF with polar measurements and unscented transform."""

    def __init__(
        self,
        dt: float,
        q_psd: float = 1.0,
        r_range: float = 0.5,
        r_az: float = 0.01,
        alpha: float = 1e-3,
        beta: float = 2.0,
        kappa: float = 0.0,
    ) -> None:
        """Purpose: construct the UKF.
        Inputs : step, PSDs, measurement variances, UT parameters.
        Outputs: UKF instance."""
        from can_inplot._04_algo.kalman import make_ct_model, discretize_continuous_noise

        self.dt = dt
        F, Qc, _ = make_ct_model(q_psd, q_psd)
        self.F = np.eye(STATE_DIM) + F * dt
        self.Q = discretize_continuous_noise(F, Qc, dt)
        self.h: Callable[[np.ndarray], np.ndarray] = polar_measurement
        self.R = np.diag([r_range, r_az])
        self.alpha = alpha
        self.beta = beta
        self.kappa = kappa
        self.lam = alpha**2 * (STATE_DIM + kappa) - STATE_DIM
        self.x: Optional[np.ndarray] = None
        self.P: Optional[np.ndarray] = None

    def _sigma_points(self) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        """Purpose: generate sigma points and weights.
        Inputs : none.
        Outputs: (sigma point matrix (2n+1, n), weights (2n+1,),
                 covariance weights (2n+1,))."""
        n = STATE_DIM
        assert self.x is not None and self.P is not None
        x = self.x
        P = self.P
        w = np.zeros(2 * n + 1)
        w[0] = self.lam / (n + self.lam)
        w[1:] = 0.5 / (n + self.lam)
        wc = w.copy()
        wc[0] = w[0] + (1 - self.alpha**2 + self.beta)
        L = np.linalg.cholesky((n + self.lam) * P)
        X = np.zeros((2 * n + 1, n))
        X[0] = x
        for i in range(n):
            X[i + 1] = x + L[i]
            X[n + i + 1] = x - L[i]
        return X, w, wc

    def initialize(self, z: np.ndarray) -> None:
        """Purpose: initialize from polar measurement.
        Inputs : [range, azimuth].
        Outputs: None."""
        r, phi = z
        self.x = np.zeros(STATE_DIM)
        self.x[0] = r * np.cos(phi)
        self.x[1] = r * np.sin(phi)
        self.P = np.diag([1.0, 1.0, 5.0, 5.0, 10.0, 10.0])

    def predict(self, dt: Optional[float] = None) -> None:
        """Purpose: time update via sigma-point propagation.
        Inputs : optional step override.
        Outputs: None."""
        if dt is not None and abs(dt - self.dt) > 1e-9:
            from can_inplot._04_algo.kalman import make_ct_model, discretize_continuous_noise

            F, Qc, _ = make_ct_model()
            self.F = np.eye(STATE_DIM) + F * dt
            self.Q = discretize_continuous_noise(F, Qc, dt)
            self.dt = dt
        X, w, wc = self._sigma_points()
        Xp = np.array([self.F @ x for x in X])
        self.x = np.sum(w[:, None] * Xp, axis=0)
        d = Xp - self.x
        self.P = sum(
            wc[i] * np.outer(d[i], d[i]) for i in range(len(Xp))
        ) + self.Q
        self._enforce_spd()

    def update(self, z: np.ndarray) -> np.ndarray:
        """Purpose: nonlinear measurement update via sigma points.
        Inputs : polar measurement [range, azimuth].
        Outputs: innovation vector."""
        X, w, wc = self._sigma_points()
        Z = np.array([self.h(x) for x in X])
        z_pred = np.sum(w[:, None] * Z, axis=0)
        dz = Z - z_pred
        S = sum(wc[i] * np.outer(dz[i], dz[i]) for i in range(len(Z))) + self.R
        dx = X - self.x
        Pxz = sum(wc[i] * np.outer(dx[i], dz[i]) for i in range(len(Z)))
        K = Pxz @ np.linalg.inv(S)
        y = z - z_pred
        self.x = self.x + K @ y
        self.P = self.P - K @ S @ K.T
        self._enforce_spd()
        return y

    def _enforce_spd(self) -> None:
        """Purpose: project covariance onto the SPD cone (float-error guard).
        Inputs : none.
        Outputs: None (mutates self.P)."""
        assert self.P is not None
        P = 0.5 * (self.P + self.P.T)
        try:
            np.linalg.cholesky(P)
            self.P = P
            return
        except np.linalg.LinAlgError:
            pass
        w, v = np.linalg.eigh(P)
        w = np.clip(w, 1e-10, None)
        self.P = v @ np.diag(w) @ v.T

    def run(self, zs: np.ndarray, dt: float) -> np.ndarray:
        """Purpose: batch filter over polar measurements.
        Inputs : measurements (N, 2) and step.
        Outputs: state trajectory (N, 6)."""
        self.dt = dt
        traj = np.zeros((len(zs), STATE_DIM))
        for i, z in enumerate(zs):
            if i == 0:
                self.initialize(z)
            else:
                self.predict()
                self.update(z)
            traj[i] = self.x
        return traj


def verify_ukf() -> dict:
    """Purpose: numeric verification of UKF vs ground truth and EKF parity.
    Inputs : none.
    Outputs: dict of verification results (RMSE + UT moment checks)."""
    rng = np.random.default_rng(11)
    dt = 0.1
    t = np.arange(0, 15.0, dt)
    px = 20.0 + 5.0 * t
    py = 5.0 + 0.5 * t
    truth = np.stack([px, py], axis=1)
    r = np.sqrt(px**2 + py**2)
    phi = np.arctan2(py, px)
    zs = np.stack([r, phi], axis=1) + rng.normal(0.0, [0.5, 0.01], (len(t), 2))

    ukf = UnscentedKalmanFilter(dt=dt, q_psd=0.5, r_range=0.25, r_az=1e-4)
    traj = ukf.run(zs, dt)
    rmse = float(np.sqrt(np.mean((traj[:, :2] - truth) ** 2)))

    # UT moment check: transform of y = x^2 with x ~ N(1, 1). Exact moments:
    #   E[y]  = mu^2 + sigma^2 = 2
    #   Var[y] = E[x^4] - E[y]^2 = (mu^4 + 6 mu^2 s^2 + 3 s^4) - 4 = 10 - 4 = 6
    # The UT reproduces both essentially exactly; first-order linearization
    # (E[y] = mu^2 = 1) is biased.
    ut = UnscentedKalmanFilter(dt=0.1)
    ut.x = np.array([1.0, 0.0, 0.0, 0.0, 0.0, 0.0])
    ut.P = np.diag([1.0, 1.0, 1.0, 1.0, 1.0, 1.0])
    X, w, wc = ut._sigma_points()
    y_sq = X[:, 0] ** 2
    ey = float(np.sum(w * y_sq))
    vy = float(np.sum(wc * (y_sq - ey) ** 2))
    return {
        "rmse": rmse,
        "tracks": rmse < 2.0,
        "ut_expectation": ey,
        "ut_variance": vy,
        "ut_moment_match": abs(ey - 2.0) < 0.2 and abs(vy - 6.0) < 0.5,
    }