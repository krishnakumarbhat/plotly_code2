% Check that the output remains default:
testcase = class_testcase(h_suite, h_archive, enum_enable.ENABLED, [], 'ltb_traget_alert_L', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Define timestamps to perform test
time_start = 0;
time_stop = 2.25;


% check most critical side = LEFT
operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_most_critical_side', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_most_critical_side', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_most_critical_side', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_most_critical_side', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.UNEQUALS);


% Check non-default signal output LEFT
operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_waypoint_at_collision_x', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_waypoint_at_collision_x', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.UNEQUALS);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_waypoint_at_collision_y', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_waypoint_at_collision_y', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.UNEQUALS);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_ttc', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_ttc', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 5);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN_OR_EQUALS);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_ttb', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_ttb', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 5);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN_OR_EQUALS);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_decel_estimate', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_decel_estimate', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.UNEQUALS);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_distance', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_distance', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.UNEQUALS);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_id', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_id', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_index', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_index', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);

operator = operator_signal_value_compare_any(testcase, 'ltb_core_out_f_obj_in_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_f_obj_in_zone', time_start, time_stop, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);


% Check default signal output RIGHT
operator = operator_signal_value_compare(testcase, 'ltb_core_out_waypoint_at_collision_x', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_waypoint_at_collision_x', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_waypoint_at_collision_y', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_waypoint_at_collision_y', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_ttc', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_ttc', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_ttb', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_ttb', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_decel_estimate', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_decel_estimate', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_distance', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_distance', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_id', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_id', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_index', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_index', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_f_obj_in_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_f_obj_in_zone', time_start, time_stop, [2]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
