% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'INTERSECT ([RECW:RECW_BMW_SP25_status_precrash:1]==4, [RECW:RECW_BMW_SP25_status_collision_warning:1]==4, [RECW:RECW_BMW_SP25_obj_id:1]==1)', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '21-Jun-2021 09:23:50', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.RECW, enum_bin.RECW, enum_bin.RECW], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_status_precrash', 1, 4).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_status_collision_warning', 1, 4).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_id', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3]);");

operator = operator_block_empty(testcase, '[RECW:RECW_BMW_SP25_obj_id:1]>=2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '21-Jun-2021 09:24:13', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.RECW], "h_block = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN_OR_EQUALS, enum_bin.RECW, 'RECW_BMW_SP25_obj_id', 1, 2).cook(h_data_synced, class_blocks.empty());");

