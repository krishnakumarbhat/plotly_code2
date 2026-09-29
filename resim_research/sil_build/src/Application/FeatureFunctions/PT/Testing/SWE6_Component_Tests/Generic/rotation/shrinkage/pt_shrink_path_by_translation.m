% PT Check the matching:
testcase = class_testcase(h_suite, h_archive, [], [], 'pt_test_rotation', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'pt_test_initial_border', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_last_p', 8.15, 12, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 14);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);

operator = operator_signal_value_compare(testcase, 'pt_test_shrinkage_by_translation', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_last_p', 12, 25.95, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 14);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);
