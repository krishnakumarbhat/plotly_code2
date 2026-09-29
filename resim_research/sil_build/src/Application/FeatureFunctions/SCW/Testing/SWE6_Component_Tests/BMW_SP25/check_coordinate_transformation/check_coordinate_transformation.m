% Check that coordinates in SCW output are transformed for alerted object on right side

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'check_coordinate_transformation', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'check_coordinate_transformation', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '18-Jun-2021 14:07:41', 'fj8y3r', ''));
operator.input_block = class_input_block(operator, [enum_bin.SCW, enum_bin.SCW, enum_bin.SCW], "h_block_1_1 = class_ingredient_compare_signal_to_value(enum_compare.SMALLER_THAN, enum_bin.SCW, 'SCW_critical_obj_pos_y_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_2 = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN, enum_bin.SCW, 'SCW_critical_obj_vel_y_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block_1_3 = class_ingredient_compare_signal_to_value(enum_compare.GREATER_THAN, enum_bin.SCW, 'SCW_critical_obj_heading_right', 1, 0).cook(h_data_synced, class_blocks.empty());
h_block = class_ingredient_intersect().cook(h_data_synced, [h_block_1_1, h_block_1_2, h_block_1_3]);");
