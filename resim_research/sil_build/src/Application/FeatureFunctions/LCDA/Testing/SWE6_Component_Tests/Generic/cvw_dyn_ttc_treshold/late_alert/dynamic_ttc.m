% CVW dynamic ttc calculation for LATE HMI TTC
testcase = class_testcase(h_suite, h_archive, [], [], 'dynamic_ttc', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare_any(testcase, 'default_value', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '11-Dec-2024 11:10:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'cvw_ttc_right', 0, 5.1, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 25.0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare_any(testcase, 'non_default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '11-Dec-2024 11:10:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'cvw_ttc_right', 5.15, 7.9, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 3.0);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);
