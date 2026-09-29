% BSW ALERT LEFT ACTIVE

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.FF_CORE], [enum_tags.ENV_CITY], enum_test_type.DEFAULT);

operator = operator_signal_edge_count(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '13-Apr-2023 14:12:27', 'xjs9y9', ''));
operator.input_edge_signal = class_input_signal(operator, [enum_bin.LCDA], 'bsw_alert_left', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_f_all_edges = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

