% CTA True Positive check - alert is triggered due to shifting rcta interesection crashline to host side - k_cta_intersection_line_host_width_percentage
testcase = class_testcase(h_suite, h_archive, [], [], 'CTA_true_positive', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_edge_count(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, [enum_bin.CTA enum_bin.CTA enum_bin.CTA enum_bin.CTA], 'cta_out_front_left_alert_level cta_out_front_right_alert_level cta_out_rear_left_alert_level cta_out_rear_right_alert_level', NaN, NaN, [1 1 1 1], enum_signal_operation.OR);
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
