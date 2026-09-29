% CTA True Positive check:
testcase = class_testcase(h_suite, h_archive, [], [], 'CTA_true_negative', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_inactive(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA enum_bin.CTA enum_bin.CTA enum_bin.CTA], 'cta_rivian_out_fcta_alert_level_right cta_rivian_out_fcta_alert_level_left cta_rivian_out_rcta_alert_level_right cta_rivian_out_rcta_alert_level_left', NaN, NaN, [1 1 1 1], enum_signal_operation.AND);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);