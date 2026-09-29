% LCDA - SLC right check:
testcase = class_testcase(h_suite, h_archive, [], [], 'slc_alert_right', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Check object presence in zone before hysteresis apply
operator = operator_signal_edge_count(testcase, 'slc_object_in_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'slc_obj_f_in_zone', 2.9, 3.1, [1]);
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);

% Check object presence in zone after hysteresis apply
operator = operator_signal_edge_count(testcase, 'slc_object_in_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'slc_obj_f_in_zone', 5.8, 6.0, [1]);
operator.input_edge_type = class_input_edge(operator, enum_edge.NONE);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
