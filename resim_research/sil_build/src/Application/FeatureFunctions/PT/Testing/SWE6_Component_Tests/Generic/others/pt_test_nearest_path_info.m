% PFGS Warning and Brake request check:
testcase = class_testcase(h_suite, h_archive, [], [], 'pt_nearest_path_info', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_LOW], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'test_nearest_path_info', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_track_match', 3.85, 4.1, [2 3 5 6 7 8], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 90);
operator.input_compare = class_input_compare(operator, enum_compare.LESS_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);