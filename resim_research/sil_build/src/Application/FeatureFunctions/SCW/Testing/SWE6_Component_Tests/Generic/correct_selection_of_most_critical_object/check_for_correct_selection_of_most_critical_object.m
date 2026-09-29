% check that a guardrail alert is raised before the dynamic object is critical and a dynamic object alert is raised subsequently.

testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'correct_selection_of_most_critical_object', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_not_empty(testcase, 'dynamic_obj_alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '18-Jun-2021 15:03:36', 'fj8y3r', ''));
operator.input_block = class_input_block(operator, [enum_bin.SCW], "h_block = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_critical_obj_type_right', 1, 1).cook(h_data_synced, class_blocks.empty());");

operator = operator_block_not_empty(testcase, 'guardrail_alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '18-Jun-2021 15:03:54', 'fj8y3r', ''));
operator.input_block = class_input_block(operator, [enum_bin.SCW], "h_block = class_ingredient_compare_signal_to_value(enum_compare.EQUALS, enum_bin.SCW, 'SCW_critical_obj_type_right', 1, 2).cook(h_data_synced, class_blocks.empty());");
