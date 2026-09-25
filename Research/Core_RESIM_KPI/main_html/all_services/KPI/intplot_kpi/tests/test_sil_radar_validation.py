import numpy as np
import pandas as pd

from InteractivePlot.kpi_client.sil_radar_validation import (
    MatchOutput,
    RadarComparisonBundle,
    RadarSideBundle,
    compute_kpis,
    compute_loaded_kpis,
    match_points,
)


def test_match_points_uses_4_fields_and_excludes_exponential_values():
    veh_df = pd.DataFrame(
        {
            "scan_index": [1, 1, 2],
            "ran": [10.0, 20.0, 30.0],
            "vel": [1.0, 1.0, 2.0],
            "azimuth": [2.0, 2.0, 3.0],
            "elevation": [3.0, 2.5e5, 4.0],
        }
    )
    resim_df = pd.DataFrame(
        {
            "scan_index": [1, 1, 2],
            "ran": [10.0, 20.0, 30.0],
            "vel": [1.0, 1.0, 2.0],
            "azimuth": [2.0, 2.0, 3.0],
            "elevation": [3.0, 3.0, 2.0e5],
        }
    )

    match = match_points(veh_df, resim_df, gate_threshold=0.01, metric="euclidean")

    np.testing.assert_array_equal(match.matched_veh_idx, np.array([0], dtype=np.int64))
    np.testing.assert_array_equal(match.matched_resim_idx, np.array([0], dtype=np.int64))
    np.testing.assert_array_equal(np.sort(match.orphan_veh_idx), np.array([1], dtype=np.int64))
    np.testing.assert_array_equal(np.sort(match.orphan_resim_idx), np.array([1, 2], dtype=np.int64))


def test_match_points_requires_elevation_for_matching():
    veh_df = pd.DataFrame(
        {
            "scan_index": [1, 2],
            "ran": [10.0, 20.0],
            "vel": [1.0, 2.0],
            "azimuth": [2.0, 3.0],
        }
    )
    resim_df = pd.DataFrame(
        {
            "scan_index": [1, 2],
            "ran": [10.0, 20.0],
            "vel": [1.0, 2.0],
            "azimuth": [2.0, 3.0],
        }
    )

    match = match_points(veh_df, resim_df, gate_threshold=0.01, metric="euclidean")

    assert match.matched_veh_idx.size == 0
    assert match.matched_resim_idx.size == 0
    np.testing.assert_array_equal(np.sort(match.orphan_veh_idx), np.array([0, 1], dtype=np.int64))
    np.testing.assert_array_equal(np.sort(match.orphan_resim_idx), np.array([0, 1], dtype=np.int64))


def test_match_points_reports_scan_level_totals():
    veh_df = pd.DataFrame(
        {
            "scan_index": [1, 1, 2],
            "ran": [10.0, 20.0, 40.0],
            "vel": [1.0, 2.0, 4.0],
            "azimuth": [3.0, 4.0, 5.0],
            "elevation": [0.5, 0.7, 0.9],
        }
    )
    resim_df = pd.DataFrame(
        {
            "scan_index": [1, 3],
            "ran": [10.0, 60.0],
            "vel": [1.0, 6.0],
            "azimuth": [3.0, 7.0],
            "elevation": [0.5, 1.1],
        }
    )

    match = match_points(veh_df, resim_df, gate_threshold=1.0, metric="euclidean")

    np.testing.assert_array_equal(match.common_scan_indices, np.array([1], dtype=np.int64))
    np.testing.assert_array_equal(match.input_only_scan_indices, np.array([2], dtype=np.int64))
    np.testing.assert_array_equal(match.output_only_scan_indices, np.array([3], dtype=np.int64))
    np.testing.assert_allclose(match.scan_match_percentages, np.array([50.0], dtype=np.float64))
    assert match.total_veh_points == 3
    assert match.total_resim_points == 2


def _empty_side_bundle() -> RadarSideBundle:
    return RadarSideBundle(
        scan_index=np.array([], dtype=np.int64),
        points=np.empty((0, 4), dtype=np.float64),
        optional={},
        scan_to_indices={},
        valid_mask=np.array([], dtype=bool),
    )


def test_compute_kpis_uses_input_scan_count_for_scan_match_pct():
    match = MatchOutput(
        matched_veh_idx=np.array([], dtype=np.int64),
        matched_resim_idx=np.array([], dtype=np.int64),
        orphan_veh_idx=np.array([], dtype=np.int64),
        orphan_resim_idx=np.array([], dtype=np.int64),
        dropped_scan_indices=np.array([], dtype=np.int64),
        distance_values=np.array([], dtype=np.float64),
        common_scan_indices=np.array([2681, 2682, 2683], dtype=np.int64),
        input_only_scan_indices=np.array([2684], dtype=np.int64),
        output_only_scan_indices=np.array([478, 479, 480, 481], dtype=np.int64),
        scan_match_percentages=np.array([50.0, 50.0, 50.0], dtype=np.float64),
    )

    kpis = compute_kpis(pd.DataFrame(), pd.DataFrame(), match)

    assert kpis["avg_scan_match_pct"] == 75.0


def test_compute_loaded_kpis_uses_input_scan_count_for_scan_match_pct():
    bundle = RadarComparisonBundle(
        veh=_empty_side_bundle(),
        resim=_empty_side_bundle(),
        common_scan_indices=np.array([2681, 2682, 2683], dtype=np.int64),
        input_only_scan_indices=np.array([2684], dtype=np.int64),
        output_only_scan_indices=np.array([478, 479, 480, 481], dtype=np.int64),
        scan_input_counts={2681: 10, 2682: 10, 2683: 10, 2684: 10},
        scan_output_counts={478: 10, 479: 10, 480: 10, 481: 10, 2681: 10, 2682: 10, 2683: 10},
    )
    match = MatchOutput(
        matched_veh_idx=np.array([], dtype=np.int64),
        matched_resim_idx=np.array([], dtype=np.int64),
        orphan_veh_idx=np.array([], dtype=np.int64),
        orphan_resim_idx=np.array([], dtype=np.int64),
        dropped_scan_indices=np.array([], dtype=np.int64),
        distance_values=np.array([], dtype=np.float64),
        common_scan_indices=np.array([2681, 2682, 2683], dtype=np.int64),
        input_only_scan_indices=np.array([2684], dtype=np.int64),
        output_only_scan_indices=np.array([478, 479, 480, 481], dtype=np.int64),
        scan_match_percentages=np.array([50.0, 50.0, 50.0], dtype=np.float64),
        total_veh_points=40,
        total_resim_points=70,
    )

    kpis = compute_loaded_kpis(bundle, match)

    assert kpis["avg_scan_match_pct"] == 75.0
