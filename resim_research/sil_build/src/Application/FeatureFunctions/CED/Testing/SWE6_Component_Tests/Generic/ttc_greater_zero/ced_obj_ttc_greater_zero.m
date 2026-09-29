% CED Object TTC Check:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'ced_obj_ttc_greater_zero', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT);

operator = operator_block_empty(testcase, 'ced_object_ttc below zero', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '20-Aug-2021 12:13:01', 'qj76x7', ''));
operator.input_block = class_input_block(operator, [enum_bin.CED], "h_block = class_ingredient_compare_signal_to_value(enum_compare.SMALLER_THAN, enum_bin.CED, 'ced_object_ttc', 1, 0).cook(h_data_synced, class_blocks.empty());");

