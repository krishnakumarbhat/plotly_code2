% CVW dynamic ttc calculation for RANGE_STT_DEFAULT
testcase = class_testcase(h_suite, h_archive, [], [], 'dynamic_ttc', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare_any(testcase, 'default_value_1', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '24-Apr-2023 16:35:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_ttc_right', 0, 36.589, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 7.875);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare_any(testcase, 'non_default_1', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '24-Apr-2023 16:35:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_ttc_right', 36.639, 42.639, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 5.0);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare_any(testcase, 'default_value_2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '24-Apr-2023 16:35:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_ttc_right', 42.689, 47.038, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 7.875);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare_any(testcase, 'non_default_2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '24-Apr-2023 16:35:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_ttc_right', 47.088, 53.738, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 5.0);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare_any(testcase, 'default_value_3', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '24-Apr-2023 16:35:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_ttc_right', 53.788, 55.138, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 7.875);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare_any(testcase, 'non_default_3', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '24-Apr-2023 16:35:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_cvw_ttc_right', 55.188, 60.788, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 5.0);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);
