% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.FF_CORE], [enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, '[LCDA:slc_obj_lc_prob:1]>=0.8', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '30-Jun-2021 08:33:33', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA], "h_block = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN_OR_EQUALS, enum_bin.LCDA, 'slc_obj_lc_prob', 1, 0.8).cook(h_data_synced, class_blocks.empty());");

