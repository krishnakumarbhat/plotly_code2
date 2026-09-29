% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.SRR5_BMW], [], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, '[SCW:SCW_critical_obj_id_right:1]==1', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '06-May-2021 10:10:33', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.SCW], "h_block = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_critical_obj_id_right', 1, 1).cook(h_data_synced, class_blocks.empty());");

