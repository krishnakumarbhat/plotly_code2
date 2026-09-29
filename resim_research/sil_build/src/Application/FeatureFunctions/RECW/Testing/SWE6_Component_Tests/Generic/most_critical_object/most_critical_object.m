% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, '[RECW:RECW_output_id:1]==1', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '18-Jun-2021 14:49:49', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.RECW], "h_block = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RECW, 'RECW_output_id', 1, 1).cook(h_data_synced, class_blocks.empty());");

operator = operator_block_empty(testcase, '[RECW:RECW_output_id:1]>=2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '18-Jun-2021 14:49:54', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.RECW], "h_block = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN_OR_EQUALS, enum_bin.RECW, 'RECW_output_id', 1, 2).cook(h_data_synced, class_blocks.empty());");

