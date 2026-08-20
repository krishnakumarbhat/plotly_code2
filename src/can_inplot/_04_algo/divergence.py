"""Information divergence metrics for radar false-alarm suppression.

Purpose: evaluate KL, JS and Wasserstein divergences between detection
distributions; derive a divergence-gated measurement gate that suppresses
false alarms while preserving true tracks.
Inputs : empirical or parametric distributions (histogram bin edges + counts,
        or mean/cov pairs).
Outputs: divergence values and gate verdicts, plus numeric verification runs.

Equation ledger (see equations.md):
    #1  D_KL(P||Q) = sum_i P_i ln(P_i/Q_i)          — asymmetric, unbounded
    #2  D_JS(P||Q) = 1/2 D_KL(P||M) + 1/2 D_KL(Q||M) — bounded [0, ln2]
    #3  W_1(P,Q)  = sum_i |CDF_P(i) - CDF_Q(i)| * bin  — metric, full support
"""

import logging
from dataclasses import dataclass
from typing import Any, Dict, Optional

import numpy as np

logger = logging.getLogger(__name__)


def kl_divergence(p: np.ndarray, q: np.ndarray, eps: float = 1e-12) -> float:
    """Purpose: Kullback-Leibler divergence D_KL(P||Q).
    Inputs : normalized probability vectors p, q; eps for zero support.
    Outputs: scalar divergence (unbounded, asymmetric)."""
    p = np.asarray(p, dtype=np.float64)
    q = np.asarray(q, dtype=np.float64)
    p = p / p.sum() if p.sum() > 0 else p
    q = q / q.sum() if q.sum() > 0 else q
    mask = p > 0
    if not np.any(mask):
        return 0.0
    q_safe = np.where(mask & (q <= 0), eps, q)
    return float(np.sum(p[mask] * np.log(p[mask] / q_safe[mask])))


def js_divergence(p: np.ndarray, q: np.ndarray, eps: float = 1e-12) -> float:
    """Purpose: Jensen-Shannon divergence (bounded [0, ln 2]).
    Inputs : normalized probability vectors p, q.
    Outputs: JS divergence; sqrt(JS) is a metric."""
    p = np.asarray(p, dtype=np.float64)
    q = np.asarray(q, dtype=np.float64)
    p = p / p.sum() if p.sum() > 0 else p
    q = q / q.sum() if q.sum() > 0 else q
    m = 0.5 * (p + q)
    return 0.5 * kl_divergence(p, m, eps) + 0.5 * kl_divergence(q, m, eps)


def wasserstein_1d(
    p: np.ndarray, q: np.ndarray, bin_width: float = 1.0
) -> float:
    """Purpose: 1D Wasserstein-1 distance between histogram distributions.
    Inputs : probability vectors p, q and histogram bin width.
    Outputs: W1 = sum_i |CDF_P(i) - CDF_Q(i)| * bin_width."""
    p = np.asarray(p, dtype=np.float64)
    q = np.asarray(q, dtype=np.float64)
    p = p / p.sum() if p.sum() > 0 else p
    q = q / q.sum() if q.sum() > 0 else q
    cdf_diff = np.abs(np.cumsum(p) - np.cumsum(q))
    return float(np.sum(cdf_diff) * bin_width)


@dataclass
class DivergenceGateResult:
    """Gate verdict for a candidate detection distribution."""

    accepted: bool
    kl: float
    js: float
    w1: float
    gate_metric: float
    threshold: float


def divergence_gate(
    reference: np.ndarray,
    candidate: np.ndarray,
    metric: str = "js",
    threshold: Optional[float] = None,
    n_bins: int = 32,
) -> DivergenceGateResult:
    """Purpose: gate a candidate measurement distribution against a reference.
    Inputs : reference and candidate histograms (unnormalized counts),
            metric name ('kl'|'js'|'w1'), optional threshold, bin count.
    Outputs: DivergenceGateResult with accept/reject verdict.

    Variation (equations.md #4): the *symmetric bounded* JS metric replaces the
    raw Mahalanobis distance in measurement gating; candidates whose JS
    divergence from the predicted measurement distribution exceeds the gate are
    classified as false alarms without track loss.
    """
    ref = np.asarray(reference, dtype=np.float64)
    cand = np.asarray(candidate, dtype=np.float64)
    if ref.sum() <= 0 or cand.sum() <= 0:
        return DivergenceGateResult(
            accepted=False, kl=float("inf"), js=float("inf"),
            w1=float("inf"), gate_metric=float("inf"),
            threshold=threshold if threshold is not None else 0.0,
        )
    p = ref / ref.sum()
    q = cand / cand.sum()
    kl = kl_divergence(p, q)
    js = js_divergence(p, q)
    w1 = wasserstein_1d(p, q, bin_width=1.0 / n_bins)
    if metric == "kl":
        gate_metric = kl
        default_threshold = 1.0
    elif metric == "w1":
        gate_metric = w1
        default_threshold = 0.15
    else:
        gate_metric = js
        default_threshold = 0.20
    thr = threshold if threshold is not None else default_threshold
    return DivergenceGateResult(
        accepted=gate_metric <= thr,
        kl=kl,
        js=js,
        w1=w1,
        gate_metric=gate_metric,
        threshold=thr,
    )


def verify_divergence_metrics() -> dict:
    """Purpose: numeric verification of divergence identities (equations.md #1-#3).
    Inputs : none.
    Outputs: dict of verification results."""
    results: Dict[str, Any] = {}

    p = np.array([0.5, 0.5])
    q = np.array([0.5, 0.5])
    results["kl_identical"] = kl_divergence(p, q)  # expect 0.0
    results["js_identical"] = js_divergence(p, q)  # expect 0.0
    results["w1_identical"] = wasserstein_1d(p, q)  # expect 0.0

    p = np.array([1.0, 0.0])
    q = np.array([0.5, 0.5])
    results["kl_delta_vs_uniform"] = kl_divergence(p, q)  # expect ln 2 ~ 0.693
    results["js_delta_vs_uniform"] = js_divergence(p, q)  # expect 0.5 ln 2 ~ 0.347
    results["kl_asymmetry"] = (
        kl_divergence(p, q),
        kl_divergence(q, p),
    )  # expect different
    results["js_bound"] = js_divergence(np.array([1.0, 0.0]), np.array([0.0, 1.0]))  # expect ln 2
    results["w1_delta"] = wasserstein_1d(np.array([1.0, 0.0]), np.array([0.0, 1.0]))  # expect 1.0

    rng = np.random.default_rng(7)
    g1 = rng.normal(0.0, 1.0, 2000)
    g2 = rng.normal(0.5, 1.0, 2000)
    h1, _ = np.histogram(g1, bins=32, range=(-4, 4))
    h2, _ = np.histogram(g2, bins=32, range=(-4, 4))
    results["js_shift05"] = js_divergence(h1, h2)
    results["w1_shift05"] = wasserstein_1d(h1, h2, bin_width=8.0 / 32.0)
    results["w1_near_mean_shift"] = abs(results["w1_shift05"] - 0.5) < 0.1

    return results