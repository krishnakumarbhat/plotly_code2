% CED Alert level 1 check:
testcase = class_testcase(h_suite, h_archive, [], [], 'ced_alert_left_eq_one', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare_any(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CED, 'ced_core_out_alert_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
