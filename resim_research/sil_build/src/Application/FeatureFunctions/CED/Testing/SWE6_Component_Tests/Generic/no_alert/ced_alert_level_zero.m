% CED True Negative check:
testcase = class_testcase(h_suite, h_archive, [], [], 'ced_alert_level_zero', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_inactive(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_alert_left ced_core_out_alert_right', NaN, NaN, [1 1], enum_signal_operation.OR);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
