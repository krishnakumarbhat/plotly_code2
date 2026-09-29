% LCDA - Alert state = 1
testcase = class_testcase(suite, archive, enum_enable.DISABLED, [], '', [enum_project.FF_CORE], [enum_tags.ENV_CITY], enum_test_type.DEFAULT);

operator = operator_signal_value_compare_any(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '24-Apr-2023 16:35:01', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'honda_srr6_alert_state', NaN, NaN, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
