% TA True Positive check:
testcase = class_testcase(h_suite, h_archive, [], [], 'ta_alert_no_alert', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_inactive(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.TA enum_bin.TA], 'rivian_ta_alert_right rivian_ta_alert_left', NaN, NaN, [1 1], enum_signal_operation.OR);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
