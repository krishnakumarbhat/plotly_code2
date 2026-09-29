% FBK object ageing check:
testcase = class_testcase(h_suite, h_archive, [], [], 'FBK_test_object_id_to_index_mapping', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);


operator = operator_signal_value_compare(testcase, 'verify_that_id_index_lut_is_set_correctly', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.FBK], 'fbk_idx_lut', 0.05, 3.65, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);


operator = operator_signal_value_compare(testcase, 'verify_that_id_index_lut_is_set_correctly', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.FBK], 'fbk_idx_lut', 0.05, 3.65, [3], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);