% ESA Active Core Output Check

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'esa_active_core_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'ESA Output Right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '11-Nov-2023 13:23:00', 'sznwmq', ''));
operator.input_block = class_input_block(operator, [enum_bin.ESA, enum_bin.ESA, enum_bin.ESA, enum_bin.ESA, enum_bin.ESA, enum_bin.ESA, enum_bin.ESA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.ESA, 'f_esa_enabled', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.ESA, 'esa_alert_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.ESA, 'esa_id_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.ESA, 'esa_long_distance_right', 1, 1000).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_3, h_block_1_5, h_block_1_7]);");
