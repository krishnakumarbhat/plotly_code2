% testcase file (inputs: <none>, outputs: h_input)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], '', [enum_project.FF_CORE], [enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'INTERSECT ([RADAR_TRACKER:pa_guardrail_active_flag:1]==1, [RADAR_TRACKER:pa_guardrail_active_flag:2]==1, [RADAR_TRACKER:pa_guardrail_age:1]==10, [RADAR_TRACKER:pa_guardrail_age:2]==10, [RADAR_TRACKER:pa_guardrail_existence_probability:1]==1, [RADAR_TRACKER:pa_guardrail_existence_probability:2]==1, [RADAR_TRACKER:pa_guardrail_lateral_position:1]==-4, [RADAR_TRACKER:pa_guardrail_lateral_position:2]==4, [RADAR_TRACKER:pa_guardrail_present_flag:1]==1, [RADAR_TRACKER:pa_guardrail_present_flag:2]==1, [RADAR_TRACKER:pa_guardrail_status:1]==1, [RADAR_TRACKER:pa_guardrail_status:2]==1)', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '30-Jun-2021 07:43:10', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_active_flag', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_active_flag', 2, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_age', 1, 10).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_age', 2, 10).cook(h_data_synced, class_blocks.empty());
h_block_1_5 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_existence_probability', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_6 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_existence_probability', 2, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_7 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_lateral_position', 1, -4).cook(h_data_synced, class_blocks.empty());
h_block_1_8 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_lateral_position', 2, 4).cook(h_data_synced, class_blocks.empty());
h_block_1_9 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_present_flag', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_10 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_present_flag', 2, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_11 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_status', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_12 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_guardrail_status', 2, 1).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4, h_block_1_5, h_block_1_6, h_block_1_7, h_block_1_8, h_block_1_9, h_block_1_10, h_block_1_11, h_block_1_12]);");

operator = operator_block_not_empty(testcase, 'INTERSECT ([VEHICLE:pa_host_speed:1]!=0, [VEHICLE:pa_yawrate:1]==0, [VEHICLE:pa_host_length:1]==4.65, [VEHICLE:pa_host_width:1]==1.83)', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '30-Jun-2021 07:43:10', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.VEHICLE, enum_bin.VEHICLE, enum_bin.VEHICLE, enum_bin.VEHICLE], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.UNEQUALS, enum_bin.VEHICLE, 'pa_host_speed', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.VEHICLE, 'pa_yawrate', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN_OR_EQUALS, enum_bin.VEHICLE, 'pa_host_length', 1, 4).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN_OR_EQUALS, enum_bin.VEHICLE, 'pa_host_width', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4]);");

operator = operator_block_not_empty(testcase, 'INTERSECT ([RADAR_TRACKER:status:1]==2, [RADAR_TRACKER:curvi_long_posn:1]<=-5, [RADAR_TRACKER:curvi_long_vel:1]>=30)', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '30-Jun-2021 07:43:10', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER, enum_bin.RADAR_TRACKER], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.RADAR_TRACKER, 'pa_status', 1, 2).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.SMALLER_THAN_OR_EQUALS, enum_bin.RADAR_TRACKER, 'pa_curvi_long_posn', 1, -5).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN_OR_EQUALS, enum_bin.RADAR_TRACKER, 'pa_curvi_long_vel', 1, 30).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3]);");

operator = operator_block_not_empty(testcase, 'INTERSECT ([LCDA:Lcda_out_f_bsw_enabled:1]==1, [LCDA:Lcda_out_f_cvw_enabled:1]==1, [LCDA:f_elc_is_enabled:1]==1, [LCDA:Lcda_out_f_slc_enabled:1]==1)', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '30-Jun-2021 07:54:56', 'sjq8k1', ''));
operator.input_block = class_input_block(operator, [enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA, enum_bin.LCDA], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'Lcda_out_f_bsw_enabled', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'Lcda_out_f_cvw_enabled', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'f_elc_is_enabled', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block_1_4 = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.LCDA, 'Lcda_out_f_slc_enabled', 1, 1).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3, h_block_1_4]);");

