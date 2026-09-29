% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, [], [], '', [enum_project.FF_CORE], [enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'Warning without existing target', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '25-Jun-2021 11:06:26', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.RADAR_TRACKER], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'Lcda_out_slc_alert_left', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_status', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2]);");

