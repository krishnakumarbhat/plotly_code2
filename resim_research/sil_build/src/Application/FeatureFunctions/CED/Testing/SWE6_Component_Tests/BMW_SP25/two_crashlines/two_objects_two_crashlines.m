% CED test for different rear and front crashline. 
testcase = class_testcase(h_suite, h_archive, enum_enable.ENABLED, [], 'two_crash_lines', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Left side
operator = operator_signal_value_compare(testcase, 'left_alert_check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'ced_core_out_alert_left',1.6,3.6,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);

operator = operator_signal_value_compare(testcase, 'left_direction_check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'ced_core_out_object_direction_left',1.6,3.6,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'left_alert_drop_check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'ced_core_out_alert_left',3.65,4.65,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% Right side
operator = operator_signal_value_compare(testcase, 'right_alert_check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'ced_core_out_alert_right',1.55,3.55,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);

operator = operator_signal_value_compare(testcase, 'right_direction_check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'ced_core_out_object_direction_right',1.55,3.55,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'right_alert_drop_check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'ced_core_out_alert_right',3.65,4.6,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);