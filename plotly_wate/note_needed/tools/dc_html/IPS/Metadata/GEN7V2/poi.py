"""
Description:
This module hold dictionary where point of interested signal for
plotting to updated for the same.
"""

datasource = None
poi_data = {
    "project": "GEN7V2",
    "stream_version": "v15",
    "streams": ["DETECTION_STREAM/AF_Det", "DETECTION_STREAM/Stream_Hdr",
                '04_OLP', 'DOWN_SELECTION_STREAM/AF_DS_Det', 'DOWN_SELECTION_STREAM/Stream_Hdr'
                ],

    "sensors": ["RL", "RR", "FL", "FR"],

    "DETECTION_STREAM/AF_Det": [
        {"name": "ran", "pname": "range", "unit": "m", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "vel", "pname": "rangerate", "unit": "mps", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "phi", "pname": "phi", "unit": "deg", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "theta", "pname": "theta", "unit": "deg", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "snr", "pname": "snr", "unit": "dB", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "rcs", "pname": "rcs", "unit": "m2", "plots": ["SC", "HIS"]}  # SC_MM, Bar_MM removed

    ],

    "DETECTION_STREAM/Stream_Hdr": [
        {"name": "scan_index", "pname": "scan_index", "unit": "index", "plots": ["SC"]},

    ],

    '04_OLP': [
        # Position signals (adjacent)
        {'name': 'vcs_pos_x', 'pname': 'vcs_pos_x', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'vcs_pos_y', 'pname': 'vcs_pos_y', 'unit': 'meter', 'plots': ['SC']},
        # Velocity signals (adjacent)
        {'name': 'vcs_vel_x', 'pname': 'vcs_vel_x', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'vcs_vel_y', 'pname': 'vcs_vel_y', 'unit': 'meter/second', 'plots': ['SC']},
        # Acceleration signals (adjacent)
        {'name': 'vcs_accel_x', 'pname': 'vcs_accel_x', 'unit': 'meter/second²', 'plots': ['SC']},
        {'name': 'vcs_accel_y', 'pname': 'vcs_accel_y', 'unit': 'meter/second²', 'plots': ['SC']},
        {'name': 'f_moveable', 'pname': 'f_moveable', 'unit': '', 'plots': ['SC']},
        {'name': 'f_stationary', 'pname': 'f_stationary', 'unit': '', 'plots': ['SC']},
        # Curvilinear position signals (adjacent)
        {'name': 'curvi_pos_x', 'pname': 'curvi_pos_x', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'curvi_pos_y', 'pname': 'curvi_pos_y', 'unit': 'meter', 'plots': ['SC']},
        # Curvilinear velocity signals (adjacent)
        {'name': 'curvi_vel_x', 'pname': 'curvi_vel_x', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'curvi_vel_y', 'pname': 'curvi_vel_y', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'age', 'pname': 'age', 'unit': '', 'plots': ['SC']},
        {'name': 'class_prob_2wheel', 'pname': 'class_prob_2wheel', 'unit': '', 'plots': ['SC']},
        {'name': 'class_prob_car', 'pname': 'class_prob_car', 'unit': '', 'plots': ['SC']},
        {'name': 'class_prob_pedestrian', 'pname': 'class_prob_pedestrian', 'unit': '', 'plots': ['SC']},
        {'name': 'class_prob_truck', 'pname': 'class_prob_truck', 'unit': '', 'plots': ['SC']},
        {'name': 'curvi_heading', 'pname': 'curvi_heading', 'unit': 'degree', 'plots': ['SC']},
        {'name': 'eclipse_value', 'pname': 'eclipse_value', 'unit': '', 'plots': ['SC']},

    ],

    "DOWN_SELECTION_STREAM/AF_DS_Det": [
        {"name": "ran", "pname": "range", "unit": "meter", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "vel", "pname": "rangerate", "unit": "meter/second", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "phi", "pname": "phi", "unit": "deg", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "theta", "pname": "theta", "unit": "deg", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "snr", "pname": "snr", "unit": "dB", "plots": ["SC", "HIS"]},  # SC_MM, Bar_MM removed
        {"name": "rcs", "pname": "rcs", "unit": "m2", "plots": ["SC", "HIS"]}  # SC_MM, Bar_MM removed

    ],

    "DOWN_SELECTION_STREAM/Stream_Hdr": [
        {"name": "scan_index", "pname": "scan_index", "unit": "index", "plots": ["SC"]},

    ],

}

poi_data_DC = {

    'streams': ['02_Input_DC_Data/Detections/Detection_Info', '04_OLP', '03_TrackerInfo/Tracker_Input/VSE',
                '03_TrackerInfo/Tracker_Output/ROT', '00_metadata', '03_TrackerInfo/Tracker_Output/AllObjects',
                '05_FeatureFunctions/CED', '05_FeatureFunctions/CTA', '05_FeatureFunctions/ESA',
                '05_FeatureFunctions/LCDA', '05_FeatureFunctions/LTB', '05_FeatureFunctions/RECW',
                '05_FeatureFunctions/SCW', '05_FeatureFunctions/TA'],

    '02_Input_DC_Data/Detections/Detection_Info': [

        {'name': 'range', 'pname': 'range', 'unit': 'meter', 'plots': ['SC', 'HS', 'BPMP']},
        {'name': 'range_rate', 'pname': 'range_rate', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'snr', 'pname': 'range_rate', 'unit': 'decibel', 'plots': ['SC']},
        {'name': 'azimuth', 'pname': 'range_rate', 'unit': 'radian', 'plots': ['SC']},
        {'name': 'elevation', 'pname': 'range_rate', 'unit': 'radian', 'plots': ['SC']},
        {'name': 'amplitude', 'pname': 'range_rate', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'valid', 'pname': 'range_rate', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'azimuth_confidence', 'pname': 'range_rate', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'elevation_confidence', 'pname': 'range_rate', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'std_range', 'pname': 'std_range', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'std_range_rate', 'pname': 'std_range_rate', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'std_rcs', 'pname': 'std_range', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'std_azimuth', 'pname': 'std_range_rate', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'std_elevation', 'pname': 'std_range_rate', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'bistatic', 'pname': 'std_range_rate', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'existence_probability', 'pname': 'std_range_rate', 'unit': 'meter', 'plots': ['SC']}

    ],

    '04_OLP': [
        # Position signals (adjacent)
        {'name': 'vcs_pos_x', 'pname': 'vcs_pos_x', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'vcs_pos_y', 'pname': 'vcs_pos_y', 'unit': 'meter', 'plots': ['SC']},
        # Velocity signals (adjacent)
        {'name': 'vcs_vel_x', 'pname': 'vcs_vel_x', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'vcs_vel_y', 'pname': 'vcs_vel_y', 'unit': 'meter/second', 'plots': ['SC']},
        # Acceleration signals (adjacent)
        {'name': 'vcs_accel_x', 'pname': 'vcs_accel_x', 'unit': 'meter/second²', 'plots': ['SC']},
        {'name': 'vcs_accel_y', 'pname': 'vcs_accel_y', 'unit': 'meter/second²', 'plots': ['SC']},
        {'name': 'f_moveable', 'pname': 'f_moveable', 'unit': '', 'plots': ['SC']},
        {'name': 'f_stationary', 'pname': 'f_stationary', 'unit': '', 'plots': ['SC']},
        # Curvilinear position signals (adjacent)
        {'name': 'curvi_pos_x', 'pname': 'curvi_pos_x', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'curvi_pos_y', 'pname': 'curvi_pos_y', 'unit': 'meter', 'plots': ['SC']},
        # Curvilinear velocity signals (adjacent)
        {'name': 'curvi_vel_x', 'pname': 'curvi_vel_x', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'curvi_vel_y', 'pname': 'curvi_vel_y', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'age', 'pname': 'age', 'unit': '', 'plots': ['SC']},
        {'name': 'class_prob_2wheel', 'pname': 'class_prob_2wheel', 'unit': '', 'plots': ['SC']},
        {'name': 'class_prob_car', 'pname': 'class_prob_car', 'unit': '', 'plots': ['SC']},
        {'name': 'class_prob_pedestrian', 'pname': 'class_prob_pedestrian', 'unit': '', 'plots': ['SC']},
        {'name': 'class_prob_truck', 'pname': 'class_prob_truck', 'unit': '', 'plots': ['SC']},
        {'name': 'curvi_heading', 'pname': 'curvi_heading', 'unit': 'degree', 'plots': ['SC']},
        {'name': 'eclipse_value', 'pname': 'eclipse_value', 'unit': '', 'plots': ['SC']},

    ],

    '03_TrackerInfo/Tracker_Input/VSE': [
        {'name': 'raw_speed_mps', 'pname': 'raw_speed_mps', 'unit': 'kilometer/hour', 'plots': ['SC']},
        {'name': 'raw_speed_qf', 'pname': 'raw_speed_qf', 'unit': '', 'plots': ['SC']},
        {'name': 'raw_steering_angle_deg', 'pname': 'raw_steering_angle_deg', 'unit': 'degree', 'plots': ['SC']},
        {'name': 'raw_steering_angle_qf', 'pname': 'raw_steering_angle_qf', 'unit': '', 'plots': ['SC']},
        {'name': 'raw_yaw_rate_qf', 'pname': 'raw_yaw_rate_qf', 'unit': '', 'plots': ['SC']},
        {'name': 'raw_yaw_rate_rps', 'pname': 'raw_yaw_rate_rps', 'unit': 'degree/second', 'plots': ['SC']},
        {'name': 'road_wheel_angle_deg', 'pname': 'road_wheel_angle_deg', 'unit': 'degree', 'plots': ['SC']},
        {'name': 'road_wheel_angle_qf', 'pname': 'road_wheel_angle_qf', 'unit': '', 'plots': ['SC']},
        {'name': 'sensor_sideslip', 'pname': 'sensor_sideslip', 'unit': 'degree', 'plots': ['SC']},

    ],

    '03_TrackerInfo/Tracker_Output/ROT': [
        {'name': 'object_class', 'pname': 'object_class', 'unit': '', 'plots': ['SC']},
        {'name': 'length', 'pname': 'length', 'unit': 'meter', 'plots': ['SC']},
        # ISO position signals
        {'name': 'iso_x_posn', 'pname': 'iso_x_posn', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'iso_y_posn', 'pname': 'iso_y_posn', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'iso_x_posn_var', 'pname': 'iso_x_posn_var', 'unit': 'meter²', 'plots': ['SC']},
        {'name': 'iso_y_posn_var', 'pname': 'iso_y_posn_var', 'unit': 'meter²', 'plots': ['SC']},
        {'name': 'iso_xy_posn_cov', 'pname': 'iso_xy_posn_cov', 'unit': 'meter²', 'plots': ['SC']},
        # ISO velocity signals
        {'name': 'iso_x_vel', 'pname': 'iso_x_vel', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'iso_y_vel', 'pname': 'iso_y_vel', 'unit': 'meter/second', 'plots': ['SC']},
        {'name': 'iso_x_vel_var', 'pname': 'iso_x_vel_var', 'unit': '(meter/second)²', 'plots': ['SC']},
        {'name': 'iso_y_vel_var', 'pname': 'iso_y_vel_var', 'unit': '(meter/second)²', 'plots': ['SC']},
        {'name': 'iso_xy_vel_cov', 'pname': 'iso_xy_vel_cov', 'unit': '(meter/second)²', 'plots': ['SC']},
        {'name': 'iso_relative_y_vel', 'pname': 'iso_relative_y_vel', 'unit': 'meter/second', 'plots': ['SC']},
        # ISO acceleration signals
        {'name': 'iso_x_acc', 'pname': 'iso_x_acc', 'unit': 'meter/second²', 'plots': ['SC']},
        {'name': 'iso_y_acc', 'pname': 'iso_y_acc', 'unit': 'meter/second²', 'plots': ['SC']},
        {'name': 'iso_x_acc_var', 'pname': 'iso_x_acc_var', 'unit': '(meter/second²)²', 'plots': ['SC']},
        {'name': 'iso_y_acc_var', 'pname': 'iso_y_acc_var', 'unit': '(meter/second²)²', 'plots': ['SC']},
        {'name': 'iso_xy_acc_cov', 'pname': 'iso_xy_acc_cov', 'unit': '(meter/second²)²', 'plots': ['SC']},
        # Object status signals
        {'name': 'number_of_objects', 'pname': 'number_of_objects', 'unit': '', 'plots': ['SC']},
        {'name': 'object_class', 'pname': 'object_class', 'unit': '', 'plots': ['SC']},
        {'name': 'object_status', 'pname': 'object_status', 'unit': '', 'plots': ['SC']},
        {'name': 'occlusion_status', 'pname': 'occlusion_status', 'unit': '', 'plots': ['SC']},

    ],
    '00_metadata': [
        {'name': 'Created_datetime', 'pname': 'length', 'unit': 'm', 'plots': ['SC']},
        {'name': 'DC_version', 'pname': 'length', 'unit': 'm', 'plots': ['SC']},
        {'name': 'OCG_version', 'pname': 'length', 'unit': 'm', 'plots': ['SC']},
        {'name': 'OLP_version', 'pname': 'length', 'unit': 'm', 'plots': ['SC']},
        {'name': 'SFL_version', 'pname': 'length', 'unit': 'm', 'plots': ['SC']},
        {'name': 'Tracker_version', 'pname': 'length', 'unit': 'm', 'plots': ['SC']},

    ],

    '03_TrackerInfo/Tracker_Output/AllObjects': [
        {'name': 'object_class', 'pname': 'object_class', 'unit': '', 'plots': ['SC']},
        # Position signals (adjacent)
        {'name': 'vcs_xposn', 'pname': 'vcs_xposn', 'unit': 'meter', 'plots': ['SC']},
        {'name': 'vcs_yposn', 'pname': 'vcs_yposn', 'unit': 'meter', 'plots': ['SC']},
        # Velocity signals (adjacent)
        {'name': 'vcs_xvel', 'pname': 'vcs_xvel', 'unit': 'mps', 'plots': ['SC']},
        {'name': 'vcs_yvel', 'pname': 'vcs_yvel', 'unit': 'mps', 'plots': ['SC']},
        # Acceleration signals (adjacent)
        {'name': 'vcs_xaccel', 'pname': 'vcs_xaccel', 'unit': 'm/s^2', 'plots': ['SC']},
        {'name': 'vcs_yaccel', 'pname': 'vcs_yaccel', 'unit': 'm/s^2', 'plots': ['SC']},
    ],

    # Feature Function streams - CED (Collision Evasion by Distance)
    '05_FeatureFunctions/CED': [
        {'name': 'ced_alert_left', 'pname': 'ced_alert_left', 'unit': '', 'plots': ['SC']},
        {'name': 'ced_alert_right', 'pname': 'ced_alert_right', 'unit': '', 'plots': ['SC']},
        {'name': 'f_ced_enable', 'pname': 'f_ced_enable', 'unit': '', 'plots': ['SC']},
    ],

    # Feature Function streams - CTA (Cross Traffic Alert)
    '05_FeatureFunctions/CTA': [
        {'name': 'f_cta_enabled', 'pname': 'f_cta_enabled', 'unit': '', 'plots': ['SC']},
        {'name': 'most_critical_object_by_sides_alert_level_left', 'pname': 'alert_level_left', 'unit': '', 'plots': ['SC']},
        {'name': 'most_critical_object_by_sides_alert_level_right', 'pname': 'alert_level_right', 'unit': '', 'plots': ['SC']},
    ],

    # Feature Function streams - ESA (Emergency Steering Assist)
    '05_FeatureFunctions/ESA': [
        {'name': 'esa_object_id_left', 'pname': 'esa_object_id_left', 'unit': '', 'plots': ['SC']},
        {'name': 'esa_object_id_right', 'pname': 'esa_object_id_right', 'unit': '', 'plots': ['SC']},
        {'name': 'esa_object_existence_prob_left', 'pname': 'esa_existence_prob_left', 'unit': '', 'plots': ['SC']},
        {'name': 'f_esa_alert_left', 'pname': 'f_esa_alert_left', 'unit': '', 'plots': ['SC']},
        {'name': 'f_esa_alert_right', 'pname': 'f_esa_alert_right', 'unit': '', 'plots': ['SC']},
        {'name': 'esa_object_ttc_s_left', 'pname': 'esa_ttc_left', 'unit': 's', 'plots': ['SC']},
        {'name': 'esa_object_ttc_s_right', 'pname': 'esa_ttc_right', 'unit': 's', 'plots': ['SC']},
    ],

    # Feature Function streams - LCDA (Lane Change Decision Aid / BSW, CVW, SLC)
    '05_FeatureFunctions/LCDA': [
        {'name': 'bsw_alert_left', 'pname': 'bsw_alert_left', 'unit': '', 'plots': ['SC']},
        {'name': 'bsw_alert_right', 'pname': 'bsw_alert_right', 'unit': '', 'plots': ['SC']},
        {'name': 'bsw_id_left', 'pname': 'bsw_id_left', 'unit': '', 'plots': ['SC']},
        {'name': 'bsw_id_right', 'pname': 'bsw_id_right', 'unit': '', 'plots': ['SC']},
        {'name': 'cvw_alert_left', 'pname': 'cvw_alert_left', 'unit': '', 'plots': ['SC']},
        {'name': 'cvw_alert_right', 'pname': 'cvw_alert_right', 'unit': '', 'plots': ['SC']},
        {'name': 'cvw_ttc_s_left', 'pname': 'cvw_ttc_s_left', 'unit': 's', 'plots': ['SC']},
        {'name': 'cvw_ttc_s_right', 'pname': 'cvw_ttc_s_right', 'unit': 's', 'plots': ['SC']},
        {'name': 'slc_alert_left', 'pname': 'slc_alert_left', 'unit': '', 'plots': ['SC']},
        {'name': 'slc_alert_right', 'pname': 'slc_alert_right', 'unit': '', 'plots': ['SC']},
    ],

    # Feature Function streams - LTB (Lead Tracking Brake)
    '05_FeatureFunctions/LTB': [
        {'name': 'ltb_alert_level_left', 'pname': 'ltb_alert_level_left', 'unit': '', 'plots': ['SC']},
        {'name': 'ltb_alert_level_right', 'pname': 'ltb_alert_level_right', 'unit': '', 'plots': ['SC']},
        {'name': 'ltb_most_critical_side', 'pname': 'ltb_most_critical_side', 'unit': '', 'plots': ['SC']},
    ],

    # Feature Function streams - RECW (Rear End Collision Warning)
    '05_FeatureFunctions/RECW': [
        {'name': 'recw_alert_level', 'pname': 'recw_alert_level', 'unit': '', 'plots': ['SC']},
        {'name': 'recw_crash_probability', 'pname': 'recw_crash_probability', 'unit': '', 'plots': ['SC']},
        {'name': 'recw_ttc_s', 'pname': 'recw_ttc_s', 'unit': 's', 'plots': ['SC']},
    ],

    # Feature Function streams - SCW (Side Collision Warning)
    '05_FeatureFunctions/SCW': [
        {'name': 'f_scw_enabled', 'pname': 'f_scw_enabled', 'unit': '', 'plots': ['SC']},
        {'name': 'scw_object_acceleration_mps2_x_left', 'pname': 'scw_accel_x_left', 'unit': 'm/s^2', 'plots': ['SC']},
        {'name': 'scw_object_acceleration_mps2_x_right', 'pname': 'scw_accel_x_right', 'unit': 'm/s^2', 'plots': ['SC']},
        {'name': 'scw_object_alert_level_left', 'pname': 'scw_alert_left', 'unit': '', 'plots': ['SC']},
        {'name': 'scw_object_alert_level_right', 'pname': 'scw_alert_right', 'unit': '', 'plots': ['SC']},
        {'name': 'scw_object_lateral_ttc_s_left', 'pname': 'scw_ttc_left', 'unit': 's', 'plots': ['SC']},
        {'name': 'scw_object_lateral_ttc_s_right', 'pname': 'scw_ttc_right', 'unit': 's', 'plots': ['SC']},
    ],

    # Feature Function streams - TA (Traffic Alert)
    '05_FeatureFunctions/TA': [
        {'name': 'f_ta_enable', 'pname': 'f_ta_enable', 'unit': '', 'plots': ['SC']},
        {'name': 'ta_alert_level_left', 'pname': 'ta_alert_level_left', 'unit': '', 'plots': ['SC']},
        {'name': 'ta_alert_level_right', 'pname': 'ta_alert_level_right', 'unit': '', 'plots': ['SC']},
    ],

}
poi_data_MCIP_CAN = {
    "project": "GEN7V2",
    "stream_version": "v15",
    "streams": ["SRR_FR_DETECTION", "SRR_FL_DETECTION", "SRR_RR_DETECTION", "SRR_RL_DETECTION", "FLR_DETECTION"

                ],

    "sensors": ["MCIP_FR", "MCIP_FL", "MCIP_RR", "MCIP_RL", "MCIP_FLR"],

    "SRR_FR_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_SRR_FR_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},

    ],
    "SRR_FL_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_SRR_FL_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},

    ],
    "SRR_RR_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_SRR_RR_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},

    ],
    "SRR_RL_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_SRR_RL_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
    ],
    "FLR_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_FLR_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},

    ]

}

poi_data_CEER_CAN = {
    "project": "GEN7V2",
    "stream_version": "v15",
    "streams": ["SRR_FR_DETECTION", "SRR_FL_DETECTION", "SRR_RR_DETECTION", "SRR_RL_DETECTION", "FLR_DETECTION"

                ],

    "sensors": ["CEER_FR", "CEER_FL", "CEER_RR", "CEER_RL", "CEER_FLR"],

    "SRR_FR_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_SRR_FR_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},

    ],
    "SRR_FL_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_SRR_FL_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},

    ],
    "SRR_RR_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_SRR_RR_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},

    ],
    "SRR_RL_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_SRR_RL_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
    ],
    "FLR_DETECTION": [
        {"name": "DET_RANGE", "pname": "Range", "unit": "index", "plots": ["SC", "SC_MM", "Bar_MM"]},
        {"name": "DET_RCS", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_AZIMUTH", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_ELEVATION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_RANGE_VELOCITY", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "DET_SNR", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},
        {"name": "timestamp_FLR_DETECTION", "pname": "steering angle", "unit": "deg", "plots": ["SC"]},

    ]

}
