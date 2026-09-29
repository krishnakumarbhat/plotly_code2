% LCDA Default Core Output Check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'lcda_default_core_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_empty(testcase, 'BSW Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_ttp_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_ttp_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_ttle_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_ttle_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9]);");

operator = operator_block_empty(testcase, 'CVW Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttc_left', 1, 25).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttc_right', 1, 25).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttp_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttp_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttle_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttle_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_10 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_12 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_distance_left', 1, -1000).cook(h_data_synced, class_blocks.empty());
h_block_1_13 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_distance_right', 1, -1000).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9, h_block_1_10, h_block_1_11, h_block_1_12, h_block_1_13]);");

operator = operator_block_empty(testcase, 'SLC Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_lon_ttc_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_lon_ttc_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_lat_ttc_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_lat_ttc_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_10 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_ttp_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_ttp_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_12 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_lane_change_prob_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_13 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'slc_lane_change_prob_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9, h_block_1_10, h_block_1_11, h_block_1_12, h_block_1_13]);");

operator = operator_block_empty(testcase, 'ELC Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_ttc_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_ttc_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_decel_to_reach_host_speed_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_decel_to_reach_host_speed_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9]);");
