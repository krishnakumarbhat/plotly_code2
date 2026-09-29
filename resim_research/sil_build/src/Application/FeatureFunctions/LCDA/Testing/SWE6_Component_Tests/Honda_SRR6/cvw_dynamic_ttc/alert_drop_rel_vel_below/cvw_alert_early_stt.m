% LCDA - CVW left check, alert dropped when object rel vel below threshold:
testcase = class_testcase(h_suite, h_archive, [], [], 'honda_srr6_cvw_alert_drop', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'honda_srr6_cvw_alert_left_on', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_alert_left', 1.2, 3.65, [1], enum_signal_operation.NONE);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'honda_srr6_cvw_alert_left_off', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_alert_left', 3.7, 5.4, [1], enum_signal_operation.NONE);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'honda_srr6_cvw_ttc_left_below_default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_ttc_left', 1.2, 3.65, [1], enum_signal_operation.NONE);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 7.875);

operator = operator_signal_value_compare(testcase, 'honda_srr6_cvw_ttc_left_default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_ttc_left', 3.7, 5.4, [1], enum_signal_operation.NONE);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 7.875);
