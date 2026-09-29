% CED test for alert due object in lateral prediction hysteresis
testcase = class_testcase(h_suite, h_archive, enum_enable.ENABLED, [], 'CED_alert', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Right side
operator = operator_signal_value_compare(testcase, 'right_check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'CED_Gen_out_alert_right',0.75,2.4,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);

operator = operator_signal_value_compare(testcase, 'right_closest_object_check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'ced_object_closest_lat_dist_predicted',1.55,3.55,1);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
