% Check that the output remains default:
testcase = class_testcase(h_suite, h_archive, enum_enable.ENABLED, [], 'ltb_default_output_expected_no_target', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Check input enable flag
operator = operator_signal_value_compare(testcase, 'ltb_core_in_f_ltb_enable', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_in_f_ltb_enable', NaN, NaN, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% check most critical side
operator = operator_signal_value_compare(testcase, 'ltb_core_out_most_critical_side', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB], 'ltb_core_out_most_critical_side', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% Check default output signal values [LEFT, RIGHT]
operator = operator_signal_value_compare(testcase, 'ltb_core_out_waypoint_at_collision_x', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_waypoint_at_collision_x', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_waypoint_at_collision_y', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_waypoint_at_collision_y', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_ttc', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_ttc', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_ttb', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_ttb', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_decel_estimate', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_decel_estimate', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_distance', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_distance', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_id', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_id', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_index', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_index', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ltb_core_out_f_obj_in_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'ltb_core_out_f_obj_in_zone', NaN, NaN, [1 2], enum_signal_operation.OR);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
