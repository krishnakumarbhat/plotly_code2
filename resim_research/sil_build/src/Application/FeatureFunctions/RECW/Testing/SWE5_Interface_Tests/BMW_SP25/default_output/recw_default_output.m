% RECW Default output check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'recw_default_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'RECW Default Output', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '18-Aug-2023 13:41:23', 'zjq4fp', ''));
operator.input_block = class_input_block(operator, [enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW, enum_bin.RECW], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_status_precrash', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_status_collision_warning', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_crash_probability', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_class', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_class_cdc', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_distance', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_heading', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_id', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_lat_pos', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_10 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_long_pos', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_approach_speed', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_12 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_overlap', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_13 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_ttc', 1, 252).cook(h_data_synced, class_blocks.empty());
h_block_1_14 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_ttc_warning_threshold', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9, h_block_1_10, h_block_1_11, h_block_1_12, h_block_1_13, h_block_1_14]);");
