% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_signal_edge_count(testcase, 'RECW basic true positive check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.RECW, 'RECW_output_alert_level', NaN, NaN, 1);
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_block_empty(testcase, 'UNION ([RADAR_TRACKER:pa_guardrail_active_flag:1]!=1, [RADAR_TRACKER:pa_guardrail_active_flag:2]!=1, [RADAR_TRACKER:pa_guardrail_age:1]!=10, [RADAR_TRACKER:pa_guardrail_age:2]!=10, [RADAR_TRACKER:pa_guardrail_existence_probability:1]!=0.5, [RADAR_TRACKER:pa_guardrail_existence_probability:2]!=0.5, [RADAR_TRACKER:pa_guardrail_lateral_position:1]!=-2, [RADAR_TRACKER:pa_guardrail_lateral_position:2]!=2, [RADAR_TRACKER:pa_guardrail_present_flag:1]!=1, [RADAR_TRACKER:pa_guardrail_present_flag:2]!=1, [RADAR_TRACKER:pa_guardrail_status:1]!=2, [RADAR_TRACKER:pa_guardrail_status:2]!=2)', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '18-Jun-2021 11:55:45', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_active_flag', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_active_flag', 2, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_age', 1, 10).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_age', 2, 10).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_existence_probability', 1, 0.5).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_existence_probability', 2, 0.5).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_lateral_position', 1, -2).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_lateral_position', 2, 2).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_present_flag', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_10 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_present_flag', 2, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_status', 1, 2).cook(h_data_synced, class_blocks.empty());
h_block_1_12 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_status', 2, 2).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9, h_block_1_10, h_block_1_11, h_block_1_12]);");

