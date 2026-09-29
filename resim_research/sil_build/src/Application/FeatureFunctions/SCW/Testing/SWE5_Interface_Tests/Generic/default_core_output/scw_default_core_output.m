% SCW default core output check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'scw_default_core_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_empty(testcase, 'SCW Default Core Output', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Apr-2021 15:21:15', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'scw_core_output_alert_level_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'scw_core_output_alert_level_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'scw_core_output_obj_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'scw_core_output_obj_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'scw_core_output_obj_index_left', 1, 255).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'scw_core_output_obj_index_right', 1, 255).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'scw_core_output_obj_type_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'scw_core_output_obj_type_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8]);");
