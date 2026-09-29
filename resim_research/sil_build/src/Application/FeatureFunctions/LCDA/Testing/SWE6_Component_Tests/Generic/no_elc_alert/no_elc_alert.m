% LCDA True Negative check:
testcase = class_testcase(h_suite, h_archive, [], [], 'no_elc_alert', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_inactive(testcase, 'no_elc_alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA enum_bin.LCDA], 'elc_alert_left elc_alert_right', NaN, NaN, [1 1], enum_signal_operation.AND);
