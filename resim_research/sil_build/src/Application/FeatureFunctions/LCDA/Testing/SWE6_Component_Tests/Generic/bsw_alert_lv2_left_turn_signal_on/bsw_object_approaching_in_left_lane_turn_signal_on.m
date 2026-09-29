testcase = class_testcase(h_suite, h_archive, [], [], 'cvw_right_turn_signal_on', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_edge_count(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, [enum_bin.LCDA], 'Lcda_out_bsw_alert_left', 3.80, 4.5, [1], enum_signal_operation.OR);
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);
