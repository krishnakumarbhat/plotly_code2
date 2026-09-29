% CTA Alert on left side 
testcase = class_testcase(h_suite, h_archive, [], [], 'cta_events', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare_any(testcase, 'alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '20-Dec-2024 15:69:81', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'honda_srr6_f_cta_alert_left', 18.464, 21.414, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare_any(testcase, 'warn', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '20-Dec-2024 15:69:81', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'honda_srr6_f_cta_warn_left', 18.464, 21.414, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);
