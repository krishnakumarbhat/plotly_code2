% LCDA - alert state lvl 3 front entry:
testcase = class_testcase(h_suite, h_archive, [], [], 'honda_srr6_alert_state', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare_any(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_alert_state', 2.6, 6.8, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 3);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);


