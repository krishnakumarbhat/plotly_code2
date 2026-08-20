"""Linear Kalman filter with continuous-discrete process noise.

Purpose: baseline tracking filter over [x, y, vx, vy, ax, ay] with exact
discretization of the continuous white-noise acceleration model via the
Van Loan method.
Inputs : measurement sequences with timestamps; process noise PSD q_c.
Outputs: filtered state/covariance trajectories.

Equation ledger (equations.md #5):
    Q_d = int_0^T exp(F s) Q_c exp(F^T s) ds     (continuous-discrete noise)
    verified against the Van Loan block-matrix exponential.
"""

import logging
from typing import Optional

import numpy as np

logger = logging.getLogger(__name__)

STATE_DIM = 6  # [x, y, vx, vy, ax, ay]


def discretize_continuous_noise(F: np.ndarray, Qc: np.ndarray, dt: float) -> np.ndarray:
    """Purpose: exact process noise discretization via Van Loan (1978).
    Inputs : continuous dynamics F, continuous noise PSD Qc, step dt.
    Outputs: discrete Q_d matrix.

    Fault found (equations.md #5): the naive approximation Q_d ≈ Qc * dt
    under-estimates covariance for large dt because it ignores the coupling
    of position/velocity/acceleration terms through F.
    """
    n = F.shape[0]
    A = np.zeros((2 * n, 2 * n))
    A[:n, :n] = -F
    A[:n, n:] = Qc
    A[n:, n:] = F.T
    try:
        from scipy.linalg import expm

        M = expm(A * dt)
    except Exception:
        # Taylor fallback: M = exp(A*dt)
        M = np.eye(2 * n)
        term = np.eye(2 * n)
        for k in range(1, 40):
            term = term @ (A * dt) / k
            M = M + term
            if np.abs(term).max() < 1e-15:
                break
    # upper-right block: e^{-FT} * Q_d ; Q_d = e^{FT} @ block, e^{FT} = M[n:,n:].T
    return M[n:, n:].T @ M[:n, n:]


def make_ct_model(ax_noise_psd: float = 1.0, ay_noise_psd: float = 1.0):
    """Purpose: build continuous-time matrices for the constant-acceleration model.
    Inputs : acceleration noise PSDs in x and y.
    Outputs: (F, Qc, H) — 6D dynamics, 6D noise PSD, 2D position measurement."""
    F = np.zeros((6, 6))
    F[0, 2] = 1.0
    F[1, 3] = 1.0
    F[2, 4] = 1.0
    F[3, 5] = 1.0
    Qc = np.zeros((6, 6))
    Qc[4, 4] = ax_noise_psd
    Qc[5, 5] = ay_noise_psd
    H = np.zeros((2, 6))
    H[0, 0] = 1.0
    H[1, 1] = 1.0
    return F, Qc, H


class KalmanFilter:
    """Standard linear Kalman filter with discretized continuous noise."""

    def __init__(self, dt: float, q_psd: float = 1.0, r: float = 1.0) -> None:
        """Purpose: construct the filter.
        Inputs : nominal step dt, acceleration PSD, measurement variance.
        Outputs: filter instance."""
        self.dt = dt
        F, Qc, H = make_ct_model(q_psd, q_psd)
        self.F = np.eye(6) + F * dt
        self.Q = discretize_continuous_noise(F, Qc, dt)
        self.H = H
        self.R = np.eye(2) * r
        self.x: Optional[np.ndarray] = None
        self.P: Optional[np.ndarray] = None

    def initialize(self, z: np.ndarray, P0: Optional[np.ndarray] = None) -> None:
        """Purpose: initialize state from first measurement.
        Inputs : measurement [x, y], optional initial covariance.
        Outputs: None."""
        self.x = np.zeros(6)
        self.x[:2] = z
        self.P = (
            P0
            if P0 is not None
            else np.diag([1.0, 1.0, 5.0, 5.0, 10.0, 10.0])
        )

    def predict(self, dt: Optional[float] = None) -> None:
        """Purpose: time update.
        Inputs : optional step override.
        Outputs: None."""
        if dt is not None and abs(dt - self.dt) > 1e-9:
            F, Qc, H = make_ct_model()
            self.F = np.eye(6) + F * dt
            self.Q = discretize_continuous_noise(F, Qc, dt)
            self.dt = dt
        self.x = self.F @ self.x
        self.P = self.F @ self.P @ self.F.T + self.Q

    def update(self, z: np.ndarray) -> np.ndarray:
        """Purpose: measurement update.
        Inputs : measurement [x, y].
        Outputs: innovation vector."""
        y = z - self.H @ self.x
        S = self.H @ self.P @ self.H.T + self.R
        K = self.P @ self.H.T @ np.linalg.inv(S)
        self.x = self.x + K @ y
        self.P = (np.eye(6) - K @ self.H) @ self.P
        return y

    def run(self, zs: np.ndarray, dt: float) -> np.ndarray:
        """Purpose: batch filter over a measurement sequence.
        Inputs : measurements (N, 2) and fixed step.
        Outputs: filtered state trajectory (N, 6)."""
        self.dt = dt
        traj = np.zeros((len(zs), 6))
        for i, z in enumerate(zs):
            if i == 0:
                self.initialize(z)
            else:
                self.predict()
                self.update(z)
            traj[i] = self.x
        return traj


def verify_kalman() -> dict:
    """Purpose: numeric verification of the linear KF (equations.md #5).
    Inputs : none.
    Outputs: dict of verification results."""
    rng = np.random.default_rng(42)
    dt = 0.1
    t = np.arange(0, 20.0, dt)
    true_x = np.stack([2.0 * t, 1.5 * t], axis=1)  # constant velocity line
    zs = true_x + rng.normal(0.0, 1.0, true_x.shape)

    kf = KalmanFilter(dt=dt, q_psd=0.1, r=1.0)
    traj = kf.run(zs, dt)

    rmse = float(np.sqrt(np.mean((traj[:, :2] - true_x) ** 2)))
    # The filter must track the line to well below the measurement noise.
    F, Qc, _ = make_ct_model(1.0, 1.0)
    q_d_diag = discretize_continuous_noise(F, Qc, 0.1)[4, 4]
    return {
        "rmse": rmse,
        "tracks_line": rmse < 0.5,
        "q_d_diag": q_d_diag,
        # closed form Q_d[4,4] = qc * dt for pure acceleration noise
        "q_d_closed_form_match": abs(q_d_diag - 0.1) < 1e-9,
    }