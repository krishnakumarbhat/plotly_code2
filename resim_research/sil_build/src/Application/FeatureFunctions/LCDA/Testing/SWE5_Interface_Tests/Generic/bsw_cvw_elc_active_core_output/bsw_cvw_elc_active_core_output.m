% LCDA BSW, CVW, ELC Active Core Output Check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'bsw_cvw_elc_active_core_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'BSW Output Right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'f_bsw_is_enabled', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_ttp_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_ttle_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_3, h_block_1_5, h_block_1_7, h_block_1_9]);");

operator = operator_block_not_empty(testcase, 'CVW Output Right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'f_cvw_is_enabled', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttc_right', 1, 25).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttp_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttle_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_3, h_block_1_5, h_block_1_7, h_block_1_9, h_block_1_11]);");

operator = operator_block_not_empty(testcase, 'ELC Output Right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'f_elc_is_enabled', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_ttc_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'elc_decel_to_reach_host_speed_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_3, h_block_1_5, h_block_1_7, h_block_1_9]);");
