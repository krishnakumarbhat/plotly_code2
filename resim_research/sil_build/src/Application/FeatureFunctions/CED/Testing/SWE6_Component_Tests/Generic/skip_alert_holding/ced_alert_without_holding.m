% CED alert without holding:
testcase = class_testcase(h_suite, h_archive, enum_enable.ENABLED, [], 'ced_alert_without_holding', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_block_empty(testcase, '[CED:ced_core_out_ttp_right:1]>0.3', enum_sensor.ARTIFICIAL_LOGFILE);
operator.set_approval_status(class_approval_status(enum_approval.PENDING, '26-Aug-2022 15:54:43', 'fj8y3r', ''));
operator.input_block = class_input_block(operator, [enum_bin.CED], "h_block = class_ingredient_compare_signal_to_value(enum_compare.SMALLER_THAN_OR_EQUALS, enum_bin.CED, 'ced_core_out_ttp_right', 1, 0.3).cook(h_data_synced, class_blocks.empty());");
