% LCDA Default Output Check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'lcda_default_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_empty(testcase, 'Alert Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, enum_bin.LCDA, "h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'nissan_srr6_f_lcda_enabled', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'nissan_srr6_f_bsw_enabled', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'nissan_srr6_f_cvw_enabled', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_2, h_block_1_3, h_block_1_4]);");

operator = operator_block_empty(testcase, 'BSW Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, enum_bin.LCDA, "h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'bsw_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5]);");

operator = operator_block_empty(testcase, 'CVW Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '09-Apr-2021 14:38:45', 'qj76x7', ''));
operator.input_block = class_input_block(operator, enum_bin.LCDA, "h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttc_left', 1, 25).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_ttc_right', 1, 25).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'cvw_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7]);");
