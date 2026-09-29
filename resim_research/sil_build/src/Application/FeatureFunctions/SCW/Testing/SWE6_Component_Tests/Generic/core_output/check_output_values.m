% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.SRR5_BMW], [enum_tags.ENV_CITY], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'INTERSECT ([SCW:SCW_alert_level_left:1]==1, [SCW:SCW_alert_level_right:1]==1, [SCW:SCW_critical_obj_id_left:1]==2, [SCW:SCW_critical_obj_id_right:1]==1, [SCW:SCW_critical_obj_type_left:1]==1, [SCW:SCW_critical_obj_type_right:1]==1)', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '06-May-2021 10:41:31', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_alert_level_left', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_alert_level_right', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_critical_obj_id_left', 1, 2).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_critical_obj_id_right', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_critical_obj_type_left', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_critical_obj_type_right', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6]);");

