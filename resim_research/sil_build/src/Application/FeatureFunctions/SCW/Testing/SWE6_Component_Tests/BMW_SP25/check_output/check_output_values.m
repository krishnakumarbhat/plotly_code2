% SCW alert on both sides check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'scw_alert_right', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'SCW Alert Right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Apr-2021 16:28:51', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_acc_x_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_acc_y_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_age_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_exist_prob_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_heading_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_length_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_pos_x_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_pos_y_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_type_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_10 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_vel_x_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_vel_y_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_12 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_width_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_13 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9, h_block_1_10, h_block_1_11, h_block_1_12, h_block_1_13]);");

operator = operator_block_not_empty(testcase, 'SCW Alert Left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Apr-2021 16:28:51', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW, enum_bin.SCW], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_acc_x_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_acc_y_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_age_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_exist_prob_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_heading_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_length_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_pos_x_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_pos_y_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_type_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_10 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_vel_x_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_vel_y_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_12 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_width_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_13 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.SCW, 'SCW_critical_obj_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9, h_block_1_10, h_block_1_11, h_block_1_12, h_block_1_13]);");
