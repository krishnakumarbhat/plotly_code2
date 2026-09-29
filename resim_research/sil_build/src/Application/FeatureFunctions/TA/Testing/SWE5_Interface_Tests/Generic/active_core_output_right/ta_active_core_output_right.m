% TA - Check active core output values

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'ta_active_core_output_right', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'Check Core Output Most Critical Side Right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Apr-2021 12:23:08', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.TA], "h_block = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.TA, 'ta_core_out_most_critical_side', 1, 1).cook(h_data_synced, class_blocks.empty());");

operator = operator_block_not_empty(testcase, 'Check Core Output Valid Counter', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Oct-2021 10:00:00', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.TA], "h_block = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.TA, 'ta_core_out_n_valid_objects', 1, 1).cook(h_data_synced, class_blocks.empty());");

operator = operator_block_not_empty(testcase, 'Check Core Output Relevant Counter', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Oct-2021 10:00:00', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.TA], "h_block = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.TA, 'ta_core_out_n_relevant_objects', 1, 1).cook(h_data_synced, class_blocks.empty());");

operator = operator_block_not_empty(testcase, 'Check Core Output Critical Counter', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Oct-2021 10:00:00', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.TA], "h_block = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.TA, 'ta_core_out_n_critical_objects', 1, 1).cook(h_data_synced, class_blocks.empty());");

operator = operator_block_not_empty(testcase, 'Check Core Output Right TTP Alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Apr-2021 12:23:08', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.TA, 'ta_core_out_alert_level', 2, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.TA, 'ta_core_out_f_obj_in_info_zone', 2, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_id', 2, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_index', 2, 255).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_ttp', 2, 100).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5]);");

operator = operator_block_not_empty(testcase, 'Check Core Output Right TTC Alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '08-Apr-2021 12:23:08', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA, enum_bin.TA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_alert_level', 2, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_decel_estimate', 2, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_distance', 2, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_f_obj_in_danger_zone', 2, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_f_obj_in_wing_zone', 2, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_id', 2, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_index', 2, 255).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_ttb', 2, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_ttc', 2, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_10 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_waypoint_at_collision_x', 2, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.TA, 'ta_core_out_waypoint_at_collision_y', 2, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9, h_block_1_10, h_block_1_11]);");
