testcase = class_testcase(h_suite, h_archive, [], [], 'cvw_ttc_hysteresis_test', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Check is cvw alert is init with tcc value lower than setup TTC value without hysteresis
operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'Lcda_out_cvw_ttc_s_right', 0.3, 1.45, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 3.5);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);

% Check is cvw tcc hysteresis is added
operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'Lcda_out_cvw_ttc_s_right', 1.55, 1.65, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 3.5);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);

operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'Lcda_out_cvw_ttc_s_right', 1.55, 1.65, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 5.0);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);

% Check is cvw alert is rise 
operator = operator_signal_edge_count(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, [enum_bin.LCDA], 'Lcda_out_cvw_alert_right', NaN, NaN, [1], enum_signal_operation.OR);
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);