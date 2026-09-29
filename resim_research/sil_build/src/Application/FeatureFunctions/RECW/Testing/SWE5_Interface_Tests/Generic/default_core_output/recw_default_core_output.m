% RECW Default core output check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'recw_default_core_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'RECW Core Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 09:17:15', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_alert_level', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_crash_prob_braking', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_crash_prob_combined', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_crash_prob_steering', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_id', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_index', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_ttc', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_ttc_alert_level_1_threshold', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_core_output_ttc_alert_level_2_threshold', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9]);");
