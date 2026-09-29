% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.FF_CORE], [enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'INTERSECT ([LCDA:Lcda_out_cvw_alert_left:1]==1, [RADAR_TRACKER:vcs_lat_posn:1]<=-7)', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '02-Jul-2021 07:52:33', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.RADAR_TRACKER], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'Lcda_out_cvw_alert_left', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.SMALLER_THAN_OR_EQUALS, enum_bin.RADAR_TRACKER, 'pa_vcs_lat_pos', 1, -7).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2]);");

