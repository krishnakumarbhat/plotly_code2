% CVW NO ACTIVE ALERT HONDA LCDA

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.FF_CORE], [enum_tags.ENV_CITY], enum_test_type.DEFAULT);

operator = operator_signal_inactive(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '13-Apr-2023 14:51:02', 'xjs9y9', ''));
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'cvw_alert_left', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

