testcase = class_testcase(h_suite, h_archive, [], [], 'CTA_crit_obj', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_edge_count(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, [enum_bin.CTA enum_bin.CTA enum_bin.CTA enum_bin.CTA], 'cta_core_out_rcta_crit_level_left cta_core_out_rcta_crit_level_right cta_core_out_fcta_crit_level_left cta_core_out_fcta_crit_level_right', 1.55, 1.65, [1 1 1 1], enum_signal_operation.OR);
operator.input_edge_type = class_input_edge(operator, enum_edge.FALLING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
