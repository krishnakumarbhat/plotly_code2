% LCDA - CVW STLA THUNDER right check:
testcase = class_testcase(h_suite, h_archive, [], [], 'stla_thunder_bsw_cvw_alert_held', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_active(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA enum_bin.LCDA], 'stla_thunder_bsw_alert_right stla_thunder_cvw_alert_right', 2.8, 5.1, [1 1], enum_signal_operation.OR);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.5);

