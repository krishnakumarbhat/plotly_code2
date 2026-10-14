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
        "num_af_det": {"aliases": ["num_af_det", "af_dets_num_af_det", "af_ds_dets_num_af_det"], "call": ["num_af_det"], "kpi_required": True},
        "rdd_idx": {"aliases": ["rdd_idx", "af_dets_rdd_idx", "af_ds_dets_rdd_idx", "rdd_index"], "call": ["rdd_idx"], "kpi_required": True},
        "ran": {"aliases": ["ran", "range", "af_dets_ran", "af_ds_dets_ran", "detection_range"], "call": ["ran"], "unit": "m", "kpi_required": True},
        "vel": {"aliases": ["vel", "velocity", "af_dets_vel", "af_ds_dets_vel", "detectionvelocity", "rr", "rangerate"], "call": ["vel"], "unit": "m/s", "kpi_required": True},
        "theta": {"aliases": ["theta", "azimuth", "af_dets_theta", "af_ds_dets_theta", "azi", "az"], "call": ["theta"], "unit": "rad", "kpi_required": True},
        "phi": {"aliases": ["phi", "elevation", "af_dets_phi", "af_ds_dets_phi", "eli", "el"], "call": ["phi"], "unit": "rad", "kpi_required": True},
        "f_single_target": {"aliases": ["f_single_target", "af_dets_f_single_target", "af_ds_dets_f_single_target", "single_target"], "call": ["f_single_target"], "kpi_required": True},
        "f_superres_target": {"aliases": ["f_superres_target", "af_dets_f_superres_target", "af_ds_dets_f_superres_target", "f_superres", "super_res_target"], "call": ["f_superres_target"], "kpi_required": True},
        "f_bistatic": {"aliases": ["f_bistatic", "af_dets_f_bistatic", "af_ds_dets_f_bistatic", "bistatic_count"], "call": ["f_bistatic"], "kpi_required": True},
        "pow": {"aliases": ["pow", "af_dets_pow", "af_ds_dets_pow", "amplitude"], "call": ["pow"], "kpi_required": True},
        "snr": {"aliases": ["snr", "af_dets_snr", "af_ds_dets_snr", "signal_to_noise_ratio"], "call": ["snr"], "kpi_required": True},
        "rcs": {"aliases": ["rcs", "af_dets_rcs", "af_ds_dets_rcs", "radar_cross_section"], "call": ["rcs"], "kpi_required": True},
        "az_conf": {"aliases": ["az_conf", "af_dets_az_conf", "af_ds_dets_az_conf"], "call": ["az_conf"], "kpi_required": True},
        "el_conf": {"aliases": ["el_conf", "af_dets_el_conf", "af_ds_dets_el_conf"], "call": ["el_conf"], "kpi_required": True},
        "spec_res": {"aliases": ["spec_res", "af_dets_spec_res", "af_ds_dets_spec_res"], "call": ["spec_res"], "kpi_required": True},
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
        "trkID": {"aliases": ["trkID", "track_id", "object_trkID", "object_unique_id", "object_reducedID", "detection_objTrkID"], "call": ["trkID"], "kpi_required": True},
        "vcs_xposn": {"aliases": ["vcs_xposn", "x_position", "object_xposn", "detection_position_x"], "call": ["vcs_xposn"], "unit": "m", "kpi_required": True},
        "vcs_yposn": {"aliases": ["vcs_yposn", "y_position", "object_yposn", "detection_position_y"], "call": ["vcs_yposn"], "unit": "m", "kpi_required": True},
        "vcs_xvel": {"aliases": ["vcs_xvel", "x_velocity", "object_xvel", "object_xvel_relative", "detection_rngrate_comp", "detection_rngrate_dealiased"], "call": ["vcs_xvel"], "unit": "m/s", "kpi_required": True},
        "vcs_yvel": {"aliases": ["vcs_yvel", "y_velocity", "object_yvel", "object_yvel_relative"], "call": ["vcs_yvel"], "unit": "m/s", "kpi_required": True},
        "vcs_heading": {"aliases": ["vcs_heading", "object_heading", "object_orientation"], "call": ["vcs_heading"], "unit": "rad", "kpi_required": True},
        "len1": {"aliases": ["len1", "object_length"], "call": ["len1"], "unit": "m", "kpi_required": True},
        "len2": {"aliases": ["len2", "object_length"], "call": ["len2"], "unit": "m", "kpi_required": True},
        "wid1": {"aliases": ["wid1", "object_width"], "call": ["wid1"], "unit": "m", "kpi_required": True},
        "wid2": {"aliases": ["wid2", "object_width"], "call": ["wid2"], "unit": "m", "kpi_required": True},
        "f_moving": {"aliases": ["f_moving", "object_f_moving", "object_f_moveable"], "call": ["f_moving"], "kpi_required": True}
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

KPI_STREAM_ALIASES = {"OBJECT_LIST_STREAM": "TRACKER_STREAM"}
KPI_TRACKER_PHYSICAL_STREAMS = ["OBJECT_LIST_STREAM", "TRACKER_STREAM"] 
