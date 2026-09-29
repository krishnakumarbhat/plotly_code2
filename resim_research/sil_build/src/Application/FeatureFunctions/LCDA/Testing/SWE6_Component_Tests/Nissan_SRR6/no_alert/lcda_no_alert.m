% LCDA True Negative check:
testcase = class_testcase(h_suite, h_archive, [], [], 'lcda_no_alert', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_inactive(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA], 'bsw_alert_left bsw_alert_right cvw_alert_left cvw_alert_right elc_alert_left elc_alert_right slc_alert_left slc_alert_right', NaN, NaN, [1 1 1 1 1 1 1 1], enum_signal_operation.OR);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
