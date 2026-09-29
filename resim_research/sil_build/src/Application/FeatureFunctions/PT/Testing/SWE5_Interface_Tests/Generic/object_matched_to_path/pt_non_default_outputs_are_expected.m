% Check that the output remains default:
testcase = class_testcase(h_suite, h_archive, [], [], 'pt_non_default_outputs_are_expected', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'check_diff_nearest_path', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_path_range_vcs_proj_to_path_segment_nearest_path', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 90);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_segment_heading_diff_nearest_path', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_segment_heading_diff_nearest_path', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_idx_nearest_path', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_track_idx_nearest_path', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_heading', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_path_heading',5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_out_track_match', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_track_match', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_out_track_match_last_cycle', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_track_match_last_cycle', 5.95, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_out_track_match_age', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_track_match_age', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_out_range_at_zero', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_range_at_zero', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 90);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_out_range_at_host_edge', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_range_at_host_edge', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 90);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_out_range_to_current_path_part', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_range_to_current_path_part', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 90);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_out_length_of_trajectory', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_length_of_trajectory', 5.9, 13.1, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, -1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'check_path_out_direction', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_path_direction', 5.9, 19.0, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);