testcase = class_testcase(h_suite, h_archive, [], [], 'lcda_bsw_fallback', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);


% Check is signal of fallback is falling
operator = operator_signal_edge_count(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, [enum_bin.LCDA], 'bsw_fallback_state', 2.8, 3.05, [2], enum_signal_operation.OR);
operator.input_edge_type = class_input_edge(operator, enum_edge.FALLING);

operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% Check is relative curvi velocity is bellow treshold value with hysteresis
operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.RADAR_TRACKER], 'pa_curvi_long_vel_rel', 2.95, 3.05, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, -4.5);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
