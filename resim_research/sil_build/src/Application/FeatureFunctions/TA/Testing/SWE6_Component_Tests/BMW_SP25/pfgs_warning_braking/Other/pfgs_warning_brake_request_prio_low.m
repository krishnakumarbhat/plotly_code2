% PFGS Warning and Brake request check:
testcase = class_testcase(h_suite, h_archive, [], [], 'pfgs_warning_and_brake_request', [enum_project.SRR5_BMW], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_LOW], enum_test_type.UNIT_TEST);

operator = operator_signal_edge_count(testcase, 'pfgs_warning_active', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.TA, 'fta_alert_level');
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);

operator = operator_signal_edge_count(testcase, 'pfgs_brake_request_active', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.TA, 'fta_brake_deceleration_request');
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
