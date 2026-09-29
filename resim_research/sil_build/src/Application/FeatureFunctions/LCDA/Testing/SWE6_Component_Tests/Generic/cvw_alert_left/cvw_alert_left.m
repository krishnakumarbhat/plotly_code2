% LCDA - CVW left check:
testcase = class_testcase(h_suite, h_archive, [], [], 'cvw_alert_left', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_edge_count(testcase, 'cvw_alert_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_cvw_alert_left');
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);

operator = operator_signal_edge_count(testcase, 'cvw_ttc_left_valid', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_cvw_alert_right');
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 25);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
