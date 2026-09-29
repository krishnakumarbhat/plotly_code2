% CTA True Positive check:
testcase = class_testcase(h_suite, h_archive, [], [], 'CTA_true_positive_narrow_scene', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'check_alert_from_left_in_narrowing_scene', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_crit_level_left', 7.2, 10.2, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);
