% CED True Positive check:
testcase = class_testcase(h_suite, h_archive, [], [], 'CED_true_positive', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'Honda_Alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'honda_srr6_ced_alert_right honda_srr6_ced_hold_alert', 24.94, 25.64, [1,2], enum_signal_operation.AND);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
