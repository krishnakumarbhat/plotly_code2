% Check that the output remains default in the negative integration test:
testcase = class_testcase(h_suite, h_archive, [], [], 'cta_non_default_output_for_left_sided_outputs_expected', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'rcta_check_crit_level_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_crit_level_right', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_crit_level_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_crit_level_left', 1.15, 3.25, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_edge_count(testcase, 'rcta_check_brake_qualifier_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_brake_qualifier_left');
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_brake_qualifier_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_brake_qualifier_right', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_id_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_id_right', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_edge_count(testcase, 'rcta_check_id_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_id_left');
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_index_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_index_right', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_core_out_index_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_index_left', 1.15, 3.25, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'check_status', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_status', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_cta_ttc_for_right_obj', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_ttc_right', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_cta_ttc_for_left_obj', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_ttc_left', 1.15, 3.25, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'rcta_check_cta_heading_for_right_obj', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_cta_heading_right', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_cta_heading_for_left_obj', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_cta_heading_left', 1.15, 3.25, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'rcta_check_cta_intersection_for_point_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_long_isect_right', NaN, NaN, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'rcta_check_cta_intersection_for_point_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'cta_core_out_rcta_long_isect_left', 1.15, 3.25, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);
