"""Unit tests for can_inplot layers."""

import os
import sys

import numpy as np
import pytest

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from can_inplot._04_algo.divergence import (
    kl_divergence,
    js_divergence,
    wasserstein_1d,
    divergence_gate,
    verify_divergence_metrics,
)
from can_inplot._04_algo.kalman import verify_kalman, discretize_continuous_noise, make_ct_model
from can_inplot._04_algo.ekf import verify_ekf
from can_inplot._04_algo.ukf import verify_ukf
from can_inplot._04_algo.tracker import run_tracking_experiment
from can_inplot._03_transport.frames import Frame, FrameCodec, MessageType


class TestDivergence:
    def test_identities(self):
        p = np.array([0.5, 0.5])
        assert kl_divergence(p, p) == pytest.approx(0.0, abs=1e-12)
        assert js_divergence(p, p) == pytest.approx(0.0, abs=1e-12)
        assert wasserstein_1d(p, p) == pytest.approx(0.0, abs=1e-12)

    def test_kl_asymmetry(self):
        p = np.array([1.0, 0.0])
        q = np.array([0.5, 0.5])
        assert kl_divergence(p, q) == pytest.approx(np.log(2), rel=1e-9)
        assert kl_divergence(p, q) != pytest.approx(kl_divergence(q, p), abs=1e-3)

    def test_js_bound(self):
        p = np.array([1.0, 0.0])
        q = np.array([0.0, 1.0])
        assert js_divergence(p, q) == pytest.approx(np.log(2), rel=1e-9)

    def test_w1_shift(self):
        rng = np.random.default_rng(7)
        g1 = rng.normal(0.0, 1.0, 2000)
        g2 = rng.normal(0.5, 1.0, 2000)
        h1, _ = np.histogram(g1, bins=32, range=(-4, 4))
        h2, _ = np.histogram(g2, bins=32, range=(-4, 4))
        w = wasserstein_1d(h1, h2, bin_width=8.0 / 32.0)
        assert w == pytest.approx(0.5, abs=0.1)

    def test_gate_rejects_disjoint(self):
        ref = np.ones(16)
        cand = np.zeros(16)
        cand[0] = 1.0
        result = divergence_gate(ref, cand, metric="js", threshold=0.2)
        assert not result.accepted

    def test_verify(self):
        results = verify_divergence_metrics()
        assert results["w1_near_mean_shift"]


class TestKalman:
    def test_verify(self):
        results = verify_kalman()
        assert results["tracks_line"]
        assert results["q_d_closed_form_match"]

    def test_qd_diagonal(self):
        F, Qc, _ = make_ct_model(1.0, 1.0)
        Qd = discretize_continuous_noise(F, Qc, 0.1)
        assert Qd.shape == (6, 6)
        assert Qd[4, 4] == pytest.approx(0.1, abs=1e-9)


class TestEKFUKF:
    def test_ekf(self):
        assert verify_ekf()["tracks"]

    def test_ukf(self):
        results = verify_ukf()
        assert results["tracks"]
        assert results["ut_moment_match"]


class TestTracker:
    def test_js_gate_beats_maha_on_cluttered_clumps(self):
        # Specular/multipath clutter regime: Mahalanobis gates get captured by
        # clumped false alarms inside the chi2 ellipsoid; the divergence gate
        # rejects distributionally-inconsistent clumps (equations.md #8).
        js = run_tracking_experiment(gate="js", seed=7, clumped=True)
        maha = run_tracking_experiment(gate="maha", seed=7, clumped=True)
        assert js["rmse"] < maha["rmse"]
        assert js["confirmed_tracks"] > maha["confirmed_tracks"]

    def test_js_gate_parity_on_diffuse_clutter(self):
        # Gaussian + Poisson clutter is Mahalanobis's optimal regime; the
        # divergence gate must stay within parity, not collapse.
        js = run_tracking_experiment(gate="js", seed=5)
        maha = run_tracking_experiment(gate="maha", seed=5)
        assert js["rmse"] < maha["rmse"] * 1.15


class TestTransport:
    def test_frame_roundtrip(self):
        frame = Frame(
            msg_type=MessageType.KPI_REQUEST,
            body={"sensor_id": "CEER_FL"},
            arrays=[np.array([1.0, 2.0, 3.0])],
        )
        parts = FrameCodec.encode(frame)
        decoded = FrameCodec.decode(parts, shapes=[(3,)])
        assert decoded.msg_type == "kpi_request"
        assert decoded.body["sensor_id"] == "CEER_FL"
        assert np.allclose(decoded.arrays[0], [1.0, 2.0, 3.0])