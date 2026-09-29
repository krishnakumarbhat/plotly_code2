% LCDA Default Generic Output Check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'lcda_default_generic_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_empty(testcase, 'BSW Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_f_bsw_enabled', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_bsw_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_bsw_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_bsw_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_bsw_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5]);");

operator = operator_block_empty(testcase, 'CVW Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_f_cvw_enabled', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_cvw_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_cvw_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_cvw_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_cvw_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_cvw_ttc_s_left', 1, 25).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_cvw_ttc_s_right', 1, 25).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7]);");

operator = operator_block_empty(testcase, 'SLC Output Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_f_slc_enabled', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_slc_alert_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_slc_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_slc_id_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_slc_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_slc_ttc_s_left', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_slc_ttc_s_right', 1, 100).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_slc_lane_change_probability_left', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.LCDA, 'Lcda_out_slc_lane_change_probability_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_union().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9]);");
