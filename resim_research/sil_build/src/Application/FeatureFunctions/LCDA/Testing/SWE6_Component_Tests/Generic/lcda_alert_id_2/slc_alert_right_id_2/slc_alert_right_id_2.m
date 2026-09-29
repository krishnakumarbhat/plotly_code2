% LCDA SLC 2nd object check:
testcase = class_testcase(h_suite, h_archive, [], [], 'slc_alert_right_id_2', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_edge_count(testcase, 'slc_alert_right_gt_zero', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_slc_alert_right');
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);

operator = operator_signal_value_compare_any(testcase, 'slc_id_right_eq_2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_slc_id_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);
