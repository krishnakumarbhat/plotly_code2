% PT Check the matching:
testcase = class_testcase(h_suite, h_archive, [], [], 'pt_matching', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'test_that_match_is_created', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_track_match', 3.45, 3.6, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);

operator = operator_signal_value_compare(testcase, 'test_that_match_is_created', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_reset_reason', 3.65, 3.65, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 6);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);

operator = operator_signal_value_compare(testcase, 'test_that_match_is_created', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_track_match', 3.6, 10, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);