% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'Rcta_core_output_left', [enum_project.SRR5_BMW], [enum_tags.ENV_CITY], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '11-May-2021 11:09:44', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.CTA, enum_bin.CTA, enum_bin.CTA, enum_bin.CTA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.CTA, 'cta_core_out_rcta_id_left', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.SMALLER_THAN_OR_EQUALS, enum_bin.CTA, 'cta_core_out_rcta_ttc_left', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN_OR_EQUALS, enum_bin.CTA, 'cta_core_out_rcta_crit_level_left', 1, 2).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.CTA, 'cta_core_out_rcta_brake_qualifier_left', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4]);");

