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
        "rdd_idx": {"aliases": ["rdd_idx"], "call": ["rdd_idx"], "kpi_required": True},
        "ran": {"aliases": ["ran", "range"], "call": ["ran"], "unit": "m", "kpi_required": True},
        "vel": {"aliases": ["vel", "velocity"], "call": ["vel"], "unit": "m/s", "kpi_required": True},
        "theta": {"aliases": ["theta", "azimuth"], "call": ["theta"], "unit": "rad", "kpi_required": True},
        "phi": {"aliases": ["phi", "elevation"], "call": ["phi"], "unit": "rad", "kpi_required": True},
        "f_single_target": {"aliases": ["f_single_target"], "call": ["f_single_target"], "kpi_required": True},
        "f_superres_target": {"aliases": ["f_superres_target"], "call": ["f_superres_target"], "kpi_required": True},
        "f_bistatic": {"aliases": ["f_bistatic"], "call": ["f_bistatic"], "kpi_required": True},
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
KPI_TRACKER_CONFIG = {
    "TRACKER_STREAM": {
        "trkID": {"aliases": ["trkID", "track_id"], "call": ["trkID"], "kpi_required": True},
        "vcs_xposn": {"aliases": ["vcs_xposn", "x_position"], "call": ["vcs_xposn"], "unit": "m", "kpi_required": True},
        "vcs_yposn": {"aliases": ["vcs_yposn", "y_position"], "call": ["vcs_yposn"], "unit": "m", "kpi_required": True},
        "vcs_xvel": {"aliases": ["vcs_xvel", "x_velocity"], "call": ["vcs_xvel"], "unit": "m/s", "kpi_required": True},
        "vcs_yvel": {"aliases": ["vcs_yvel", "y_velocity"], "call": ["vcs_yvel"], "unit": "m/s", "kpi_required": True},
        "vcs_heading": {"aliases": ["vcs_heading"], "call": ["vcs_heading"], "unit": "rad", "kpi_required": True},
        "len1": {"aliases": ["len1"], "call": ["len1"], "unit": "m", "kpi_required": True},
        "len2": {"aliases": ["len2"], "call": ["len2"], "unit": "m", "kpi_required": True},
        "wid1": {"aliases": ["wid1"], "call": ["wid1"], "unit": "m", "kpi_required": True},
        "wid2": {"aliases": ["wid2"], "call": ["wid2"], "unit": "m", "kpi_required": True},
        "f_moving": {"aliases": ["f_moving"], "call": ["f_moving"], "kpi_required": True}
    }
}




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