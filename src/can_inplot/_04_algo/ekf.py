"""Extended Kalman filter with Jacobian linearization.

Purpose: continuous-discrete EKF over [x, y, vx, vy, ax, ay] with nonlinear
range/azimuth measurements (polar sensor model).
Inputs : measurement sequences; nonlinear h(x) and Jacobian H(x).
Outputs: filtered state trajectories.

Equation ledger (equations.md #6):
    Fault found: first-order Jacobian covariance propagation
        P^+ = (I - K H(x^-)) P^-
    drops second-order terms; for range measurements with heavy curvature
    (close targets, large velocity) the EKF covariance is biased and the
    filter diverges where the UKF does not.
"""

import logging
from typing import Callable, Optional

import numpy as np

logger = logging.getLogger(__name__)


def polar_measurement(x: np.ndarray) -> np.ndarray:
    """Purpose: nonlinear measurement model h(x) = [range, azimuth].
    Inputs : state vector [x, y, vx, vy, ax, ay].
    Outputs: [r, phi] measurement."""
    px, py = x[0], x[1]
    r = np.sqrt(px**2 + py**2)
    phi = np.arctan2(py, px)
    return np.array([r, phi])


def polar_jacobian(x: np.ndarray) -> np.ndarray:
    """Purpose: Jacobian of the polar measurement model.
    Inputs : state vector.
    Outputs: 2x6 Jacobian matrix."""
    px, py = x[0], x[1]
    r = np.sqrt(px**2 + py**2)
    r = max(r, 1e-9)
    H = np.zeros((2, 6))
    H[0, 0] = px / r
    H[0, 1] = py / r
    H[1, 0] = -py / (r**2)
    H[1, 1] = px / (r**2)
    return H


class ExtendedKalmanFilter:
    """EKF with polar measurements and discretized continuous noise."""

    def __init__(self, dt: float, q_psd: float = 1.0, r_range: float = 0.5, r_az: float = 0.01) -> None:
        """Purpose: construct the EKF.
        Inputs : step, acceleration PSD, range/azimuth measurement variances.
        Outputs: EKF instance."""
        from can_inplot._04_algo.kalman import make_ct_model, discretize_continuous_noise

        self.dt = dt
        F, Qc, _ = make_ct_model(q_psd, q_psd)
        self.F = np.eye(6) + F * dt
        self.Q = discretize_continuous_noise(F, Qc, dt)
        self.h: Callable[[np.ndarray], np.ndarray] = polar_measurement
        self.Hfun: Callable[[np.ndarray], np.ndarray] = polar_jacobian
        self.R = np.diag([r_range, r_az])
        self.x: Optional[np.ndarray] = None
        self.P: Optional[np.ndarray] = None

    def initialize(self, z: np.ndarray) -> None:
        """Purpose: initialize from polar measurement.
        Inputs : [range, azimuth].
        Outputs: None."""
        r, phi = z
        self.x = np.zeros(6)
        self.x[0] = r * np.cos(phi)
        self.x[1] = r * np.sin(phi)
        self.P = np.diag([1.0, 1.0, 5.0, 5.0, 10.0, 10.0])

    def predict(self, dt: Optional[float] = None) -> None:
        """Purpose: time update with discretized noise.
        Inputs : optional step override.
        Outputs: None."""
        if dt is not None and abs(dt - self.dt) > 1e-9:
            from can_inplot._04_algo.kalman import make_ct_model, discretize_continuous_noise

            F, Qc, _ = make_ct_model()
            self.F = np.eye(6) + F * dt
            self.Q = discretize_continuous_noise(F, Qc, dt)
            self.dt = dt
        self.x = self.F @ self.x
        self.P = self.F @ self.P @ self.F.T + self.Q

    def update(self, z: np.ndarray) -> np.ndarray:
        """Purpose: nonlinear measurement update.
        Inputs : polar measurement [range, azimuth].
        Outputs: innovation vector."""
        assert self.x is not None and self.P is not None
        y = z - self.h(self.x)
        H = self.Hfun(self.x)
        assert self.P is not None
        S = H @ self.P @ H.T + self.R
        K = self.P @ H.T @ np.linalg.inv(S)
        self.x = self.x + K @ y
        self.P = (np.eye(6) - K @ H) @ self.P
        return y

    def run(self, zs: np.ndarray, dt: float) -> np.ndarray:
        """Purpose: batch filter over polar measurements.
        Inputs : measurements (N, 2) and step.
        Outputs: state trajectory (N, 6)."""
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


def verify_ekf() -> dict:
    """Purpose: numeric verification of EKF vs ground truth (equations.md #6).
    Inputs : none.
    Outputs: dict of verification results."""
    rng = np.random.default_rng(3)
    dt = 0.1
    t = np.arange(0, 15.0, dt)
    px = 20.0 + 5.0 * t
    py = 5.0 + 0.5 * t
    truth = np.stack([px, py], axis=1)
    r = np.sqrt(px**2 + py**2)
    phi = np.arctan2(py, px)
    zs = np.stack([r, phi], axis=1) + rng.normal(
        0.0, [0.5, 0.01], (len(t), 2)
    )
    ekf = ExtendedKalmanFilter(dt=dt, q_psd=0.5, r_range=0.25, r_az=1e-4)
    traj = ekf.run(zs, dt)
    rmse = float(np.sqrt(np.mean((traj[:, :2] - truth) ** 2)))
    return {
        "rmse": rmse,
        "tracks": rmse < 2.0,
        "final_pos_error": float(np.linalg.norm(traj[-1, :2] - truth[-1])),
    }