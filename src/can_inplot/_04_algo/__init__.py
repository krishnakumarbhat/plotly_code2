"""Layer 04_algo — tracking filters and divergence metrics.

Purpose: research-grade algorithms: KL/JS/Wasserstein divergence metrics for
false-alarm suppression, clutter separation, Doppler distribution shift
detection, and continuous-discrete Kalman/EKF/UKF multi-target tracking.
Inputs : detection streams from 02_kpi (or synthetic experiments).
Outputs: verified metric/tracking results with numeric validation.
"""

from can_inplot._04_algo.divergence import (
    kl_divergence,
    js_divergence,
    wasserstein_1d,
    divergence_gate,
    DivergenceGateResult,
    verify_divergence_metrics,
)
from can_inplot._04_algo.kalman import (
    KalmanFilter,
    discretize_continuous_noise,
    verify_kalman,
)
from can_inplot._04_algo.ekf import ExtendedKalmanFilter, verify_ekf
from can_inplot._04_algo.ukf import UnscentedKalmanFilter, verify_ukf
from can_inplot._04_algo.tracker import (
    MultiTargetTracker,
    TrackState,
    run_tracking_experiment,
)

__all__ = [
    "kl_divergence",
    "js_divergence",
    "wasserstein_1d",
    "divergence_gate",
    "DivergenceGateResult",
    "verify_divergence_metrics",
    "KalmanFilter",
    "discretize_continuous_noise",
    "verify_kalman",
    "ExtendedKalmanFilter",
    "verify_ekf",
    "UnscentedKalmanFilter",
    "verify_ukf",
    "MultiTargetTracker",
    "TrackState",
    "run_tracking_experiment",
]