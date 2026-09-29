# KPI Configuration Storage
# Based on InteractivePlot config_storage.py structure but optimized for KPI needs

# KPI-specific configuration for alignment data
KPI_ALIGNMENT_CONFIG = {
    "DYNAMIC_ALIGNMENT_STREAM": {
        "vacs_boresight_az_nominal_V": {
            "aliases": ["vacs_boresight_az_nominal", "az_nominal"],
            "call": ["vacs_boresight_az_nominal"],
            "plot_types": ["kpi_alignment_analysis"],
            "unit": "degrees",
            "range_to_be_accepted": [-180, 180],
            "kpi_required": True
        },
        "vacs_boresight_az_estimated": {
            "aliases": ["vacs_boresight_az_estimated", "az_estimated"],
            "call": ["vacs_boresight_az_estimated"],
            "plot_types": ["kpi_alignment_analysis"],
            "unit": "degrees",
            "range_to_be_accepted": [-180, 180],
            "kpi_required": True
        },
        "vacs_boresight_az_kf_internal_V": {
            "aliases": ["vacs_boresight_az_kf_internal", "az_kf_internal"],
            "call": ["vacs_boresight_az_kf_internal"],
            "plot_types": ["kpi_alignment_analysis"],
            "unit": "degrees",
            "range_to_be_accepted": [-180, 180],
            "kpi_required": True
        },
        "vacs_boresight_el_nominal": {
            "aliases": ["vacs_boresight_el_nominal", "el_nominal"],
            "call": ["vacs_boresight_el_nominal"],
            "plot_types": ["kpi_alignment_analysis"],
            "unit": "degrees",
            "range_to_be_accepted": [-90, 90],
            "kpi_required": True
        },
        "vacs_boresight_el_estimated": {
            "aliases": ["vacs_boresight_el_estimated", "el_estimated"],
            "call": ["vacs_boresight_el_estimated"],
            "plot_types": ["kpi_alignment_analysis"],
            "unit": "degrees",
            "range_to_be_accepted": [-90, 90],
            "kpi_required": True
        },
        "vacs_boresight_el_kf_internal": {
            "aliases": ["vacs_boresight_el_kf_internal", "el_kf_internal"],
            "call": ["vacs_boresight_el_kf_internal"],
            "plot_types": ["kpi_alignment_analysis"],
            "unit": "degrees",
            "range_to_be_accepted": [-90, 90],
            "kpi_required": True
        }
    },
}

# "ALIGNMENT_STREAM": {
#     "vacs_boresight_az_nominal":{},
#     "vacs_boresight_az_estimated":{},
#     "vacs_boresight_az_kf_internal":{},
#     "vacs_boresight_el_nominal":{},
#     "vacs_boresight_el_estimated":{},
#     "vacs_boresight_el_kf_internal":{}
#     },

# KPI-specific configuration for detection data
KPI_DETECTION_CONFIG = {
    "DETECTION_STREAM": {
        "num_af_det": {"aliases": ["num_af_det"], "call": ["num_af_det"], "kpi_required": True},
        "rdd_idx": {"aliases": ["rdd_idx", "af_dets_rdd_idx"], "call": ["rdd_idx"], "kpi_required": True},
        "ran": {"aliases": ["ran", "range", "af_dets_ran"], "call": ["ran"], "unit": "m", "kpi_required": True},
        "vel": {"aliases": ["vel", "velocity", "af_dets_vel"], "call": ["vel"], "unit": "m/s", "kpi_required": True},
        "theta": {"aliases": ["theta", "azimuth", "af_dets_theta"], "call": ["theta"], "unit": "rad", "kpi_required": True},
        "phi": {"aliases": ["phi", "elevation", "af_dets_phi"], "call": ["phi"], "unit": "rad", "kpi_required": True},
        "f_single_target": {"aliases": ["f_single_target", "af_dets_f_single_target"], "call": ["f_single_target"], "kpi_required": True},
        "f_superres_target": {"aliases": ["f_superres_target", "af_dets_f_superres_target"], "call": ["f_superres_target"], "kpi_required": True},
        "f_bistatic": {"aliases": ["f_bistatic", "af_dets_f_bistatic"], "call": ["f_bistatic"], "kpi_required": True},
        "scan_index": {"aliases": ["scan_index"], "call": ["scan_index"], "kpi_required": True}
    },
    "RDD_STREAM": {
        "rdd1_num_detect": {"aliases": ["rdd1_num_detect"], "call": ["rdd1_num_detect"], "kpi_required": True},
        "rdd1_rindx": {"aliases": ["rdd1_rindx"], "call": ["rdd1_rindx"], "kpi_required": True},
        "rdd1_dindx": {"aliases": ["rdd1_dindx"], "call": ["rdd1_dindx"], "kpi_required": True},
        "rdd2_range": {"aliases": ["rdd2_range"], "call": ["rdd2_range"], "unit": "m", "kpi_required": True},
        "rdd2_range_rate": {"aliases": ["rdd2_range_rate"], "call": ["rdd2_range_rate"], "unit": "m/s", "kpi_required": True},
        "scan_index": {"aliases": ["scan_index"], "call": ["scan_index"], "kpi_required": True}
    },
    "CDC_STREAM": {
        "num_cdc_records": {"aliases": ["num_cdc_records"], "call": ["num_cdc_records"], "unit": "m/s", "kpi_required": True}
    },
    "VSE_STREAM": {
        "veh_speed": {"aliases": ["veh_speed"], "call": ["veh_speed"], "unit": "m/s", "kpi_required": True}
    }
}

# KPI-specific configuration for tracker data
# NOTE: tracker logs exist in two layouts:
#   - classic "TRACKER_STREAM" with vcs_* signals, and
#   - AshokLeyland-style "OBJECT_LIST_STREAM/F360_Object_Log" with object_* signals.
# Both are kept backward compatible via STREAM_ALIASES + extended signal aliases below.
KPI_TRACKER_CONFIG = {
    "TRACKER_STREAM": {
        "trkID": {"aliases": ["trkID", "track_id", "object_trkID"], "call": ["trkID"], "kpi_required": True},
        "vcs_xposn": {"aliases": ["vcs_xposn", "x_position", "object_xposn"], "call": ["vcs_xposn"], "unit": "m", "kpi_required": True},
        "vcs_yposn": {"aliases": ["vcs_yposn", "y_position", "object_yposn"], "call": ["vcs_yposn"], "unit": "m", "kpi_required": True},
        "vcs_xvel": {"aliases": ["vcs_xvel", "x_velocity", "object_xvel"], "call": ["vcs_xvel"], "unit": "m/s", "kpi_required": True},
        "vcs_yvel": {"aliases": ["vcs_yvel", "y_velocity", "object_yvel"], "call": ["vcs_yvel"], "unit": "m/s", "kpi_required": True},
        "vcs_heading": {"aliases": ["vcs_heading", "heading", "object_heading"], "call": ["vcs_heading"], "unit": "rad", "kpi_required": True},
        "len1": {"aliases": ["len1", "object_length"], "call": ["len1"], "unit": "m", "kpi_required": True},
        "len2": {"aliases": ["len2", "object_length"], "call": ["len2"], "unit": "m", "kpi_required": True},
        "wid1": {"aliases": ["wid1", "object_width"], "call": ["wid1"], "unit": "m", "kpi_required": True},
        "wid2": {"aliases": ["wid2", "object_width"], "call": ["wid2"], "unit": "m", "kpi_required": True},
        "f_moving": {"aliases": ["f_moving", "object_f_moving", "object_f_moveable"], "call": ["f_moving"], "kpi_required": True}
    }
}


# Canonical stream registry for backward-compatible log layouts.
# Key = canonical name used across the KPI pipeline (wrapper, factory, reports).
# Value = HDF stream names accepted for that canonical stream (first = preferred).
# This is intentionally better than scattered `if a == X or a == Y` checks:
# every layer resolves through canonical_stream_name()/resolve_actual_stream().
STREAM_ALIASES = {
    "DYNAMIC_ALIGNMENT_STREAM": ["DYNAMIC_ALIGNMENT_STREAM", "Dyn_Align_STREAM"],
    "DETECTION_STREAM": ["DETECTION_STREAM"],
    "RDD_STREAM": ["RDD_STREAM"],
    "CDC_STREAM": ["CDC_STREAM"],
    "VSE_STREAM": ["VSE_STREAM"],
    "TRACKER_STREAM": ["TRACKER_STREAM", "OBJECT_LIST_STREAM"],
}


def canonical_stream_name(name):
    """Map any known stream variant back to its canonical KPI name."""
    if not name:
        return name
    for canonical, variants in STREAM_ALIASES.items():
        if name == canonical or name in variants:
            return canonical
    lowered = str(name).lower()
    for canonical, variants in STREAM_ALIASES.items():
        if lowered == canonical.lower() or any(lowered == str(v).lower() for v in variants):
            return canonical
    return name


def resolve_actual_stream(available_names, canonical):
    """Pick the concrete HDF stream present for a canonical stream.

    Prefers the canonical/preferred spelling, then any known variant,
    then a case-insensitive match. Returns None when nothing matches.
    """
    if not available_names:
        return None
    available = list(available_names)
    variants = STREAM_ALIASES.get(canonical, [canonical])
    for candidate in variants:
        if candidate in available:
            return candidate
    lowered = {str(a).lower(): a for a in available}
    for candidate in variants:
        if str(candidate).lower() in lowered:
            return lowered[str(candidate).lower()]
    if str(canonical).lower() in lowered:
        return lowered[str(canonical).lower()]
    return None




# KPI validation rules and thresholds
KPI_VALIDATION_RULES = {
    "alignment_tolerance": 0.1,  # degrees
    "detection_threshold": 0.8,  # percentage
    "tracker_quality_min": 0.7,  # minimum quality score
    "data_completeness_threshold": 0.95,  # minimum data completeness
    # Detection matching thresholds
    "detection_thresholds": {
        "range_threshold": 2.5 + 0.0000001,  # m
        "velocity_threshold": 2.5 + 0.0000001,  # m/s
        "theta_threshold": 2.5 + 0.0000001,  # rad
        "phi_threshold": 2.5 + 0.0000001,  # rad
        "max_num_af_dets_front": 768,
        "max_num_af_dets_corner": 680,
        "max_num_rdd_dets": 512,
        "range_saturation_threshold_front": 135,  # m
        "range_saturation_threshold_corner": 135,  # m
        "max_cdc_records": 5016,
        "radar_cycle_s": 0.05
    },
    # Tracker matching thresholds
    "tracker_thresholds": {
        "max_number_of_data": 64,
        "max_valid_distance": 160,  # m
        "vcs_xposn_threshold": 0.01 + 0.0000001,  # m
        "vcs_yposn_threshold": 0.01 + 0.0000001,  # m
        "vcs_xvel_threshold": 0.02 + 0.0000001,  # m/s
        "vcs_yvel_threshold": 0.02 + 0.0000001   # m/s
    },
    # Alignment matching thresholds
    "alignment_thresholds": {
        "az_misalign_threshold": 1.0,  # degrees
        "el_misalign_threshold": 1.0   # degrees
    }
} 
