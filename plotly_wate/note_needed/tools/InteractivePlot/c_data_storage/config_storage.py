# Dictionary of signal patterns with their aliases and plot types
Gen7V1_V2 = {
"DETECTION_STREAM": {
    "ran": {
        "aliases": ["ran", "detection_range"],
        "call": ["Range"],
        "plot_types": [
            "histogram_with_count",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
        "unit": "meters",
        "range_to_be_accepted": [0, 500],
        "grp": "Vse_stream",
    },
    "rdd_idx": {"grp": "DETECTION_STREAM"},
    "num_af_det": {
        "aliases": ["num_af_det", "af_det"],
        "call": [
            "num_af_det",
        ],
        "plot_types": [
            "scatter_plot",
            "bar_mismatch_plots_all",
            "box_num_af_det",
            "scatter_num_af_det",
        ],
        "unit": "meters",
        "range_to_be_accepted": [0, 500],
    },
    "theta": {
        "aliases": ["theta", "azimuth"],
        "call": ["Azimuth"],
        "plot_types": [
            "histogram_with_radtodeg",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
        "unit": "radiance",
        "range_to_be_accepted": [-80, 80],
    },
    "phi": {
        "aliases": ["phi", "elevation"],
        "call": ["elevation"],
        "plot_types": [
            "histogram_with_radtodeg",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
        "unit": "radiance",
        "range_to_be_accepted": [-120, 120],
    },
    "amplitude": {
        "aliases": ["amplitude", "amplitude_val"],
        "call": ["amplitude"],
        "plot_types": ["scatter_plot", "scatter_with_mismatch"],
        "unit": "meters",
        "range_to_be_accepted": [0, 1000],
    },
    "snr": {
        "aliases": ["snr", "signal_to_noise_ratio"],
        "call": ["signal_to_noise_ratio"],
        "plot_types": [
            "histogram_with_count",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
        "unit": "DB",
        "range_to_be_accepted": [0, 100],
    },
    "rcs": {
        "aliases": ["rcs", "radar_cross_section"],
        "call": ["radar_cross_section"],
        "plot_types": [
            "histogram_with_count",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
    },
    "f_bistatic": {
        "aliases": ["f_bistatic", "bistatic_count"],
        "call": ["bistatic_count", "scatter_plot"],
        "plot_types": [
            "scatter_plot",
            "bar_plots_fbi_sup_sig",
            "scatter_plot_bs_si_sr",
        ],
    },
    "f_superres_target": {
        "aliases": ["f_superres", "super_res_target"],
        "call": ["super_res_target"],
        "plot_types": [
            "scatter_plot",
            "bar_plots_fbi_sup_sig",
            "scatter_plot_bs_si_sr",
        ],
    },
    "f_single_target": {
        "aliases": ["single_target"],
        "call": ["single_target"],
        "plot_types": [
            "scatter_plot",
            "bar_plots_fbi_sup_sig",
            "scatter_plot_bs_si_sr",
        ],
        "unit": "meters",
        "range_to_be_accepted": [0, 500],
    }
    },
"VSE_STREAM": {
    "vel": {
        "aliases": ["velocity"],
        "call": ["Rangerate"],
        "plot_types": [
            "histogram_with_count",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
    },
    "veh_yaw": {"aliases": ["yaw"], "call": ["Yaw"], "plot_types": ["scatter_plot"]},
    "veh_speed": {
        "aliases": ["speed"],
        "call": ["Speed"],
        "plot_types": ["scatter_plot_mstokmh"],
    },
    "veh_steering_angle": {
        "aliases": ["steering_angle"],
        "call": ["Steering_angle"],
        "plot_types": ["scatter_plot"],
    },
    "veh_pitch": {
        "aliases": ["pitch"],
        "call": ["Veh_pitch"],
        "plot_types": ["scatter_plot"],
    },
    "veh_roll": {"aliases": ["Roll"], "call": ["Roll"], "plot_types": ["scatter_plot"]},
    "status_of_plate": {"plot_types": ["scatter_plot"]}
    },

"DOWN_SELECTION_STREAM":{
    "num_af_det": {
        "aliases": ["num_af_det", "af_det"],
        "call": [
            "num_af_det",
        ],
        "plot_types": [
            "scatter_plot",
            "bar_mismatch_plots_all",
            "box_num_af_det",
            "scatter_num_af_det",
        ],
        "unit": "meters",
        "range_to_be_accepted": [0, 500],
    },
    "theta": {
        "aliases": ["theta", "azimuth"],
        "call": ["Azimuth"],
        "plot_types": [
            "histogram_with_radtodeg",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
        "unit": "radiance",
        "range_to_be_accepted": [-80, 80],
    },
    "phi": {
        "aliases": ["phi", "elevation"],
        "call": ["elevation"],
        "plot_types": [
            "histogram_with_radtodeg",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
        "unit": "radiance",
        "range_to_be_accepted": [-120, 120],
    },
    "amplitude": {
        "aliases": ["amplitude", "amplitude_val"],
        "call": ["amplitude"],
        "plot_types": ["scatter_plot", "scatter_with_mismatch"],
        "unit": "meters",
        "range_to_be_accepted": [0, 1000],
    },
    "snr": {
        "aliases": ["snr", "signal_to_noise_ratio"],
        "call": ["signal_to_noise_ratio"],
        "plot_types": [
            "histogram_with_count",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
        "unit": "DB",
        "range_to_be_accepted": [0, 100],
    },
    "rcs": {
        "aliases": ["rcs", "radar_cross_section"],
        "call": ["radar_cross_section"],
        "plot_types": [
            "histogram_with_count",
            "scatter_plot",
            "scatter_with_mismatch",
            "bar_mismatch_plots_all",
        ],
    }
    }
}




sequence_of_plot = {
    "scatter": [
        "all_signal;bar_mismatch_plots_all",
        "fbi_sup_sig;bar_plots_fbi_sup_sig",
        "Range;scatter_plot",
        "Range;scatter_with_mismatch",
        "Rangerate;scatter_plot",
        "Rangerate;scatter_with_mismatch",
        "Azimuth;scatter_plot",
        "Azimuth;scatter_with_mismatch",
        "elevation;scatter_plot",
        "elevation;scatter_with_mismatch",
        "amplitude;scatter_plot",
        "amplitude;scatter_with_mismatch",
        "radar_cross_section;scatter_plot",
        "radar_cross_section;scatter_with_mismatch"
    ],
    "scatter_mismatch": [
        "f_superres;scatter_plot",
        "f_superres;scatter_with_mismatch",
        "f_single_target;scatter_plot",
        "f_single_target;scatter_with_mismatch",
        "num_af_det;scatter_plot",
        "f_bistatic;scatter_plot",
        "signal_to_noise_ratio;scatter_plot"
    ],
    "histogram": [
        "Range;histogram_with_count",
        "Rangerate;histogram_with_count",
        "Azimuth;histogram_with_radtodeg",
        "elevation;histogram_with_radtodeg",
        "amplitude;histogram_with_count",
        "radar_cross_section;histogram_with_count",
        "signal_to_noise_ratio;histogram_with_count",
        "Range;scatter_with_scanindex"
    ]
}
