% CTA Host Speed Interesaction Point Extension
testcase = class_testcase(h_suite, h_archive, [], [], 'CTA_host_speed_based_intersection_threshold_extension', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Checking if alert state is level 0
operator = operator_signal_value_compare(testcase, 'check_cta_core_out_rcta_crit_level_left_ZERO', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_out_rear_left_alert_level', 0.1, 1.8, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% Checking if alert state is level 2
operator = operator_signal_value_compare(testcase, 'check_cta_core_out_rcta_crit_level_left_TWO', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_out_rear_left_alert_level', 1.9,4.6, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
