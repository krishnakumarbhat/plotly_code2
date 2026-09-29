% PT Check the matching:
testcase = class_testcase(h_suite, h_archive, [], [], 'pt_matching', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'test_existing_match_value_for_obj_inside_of_creation_path', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.PATHTRACKING], 'path_out_track_match', 3.05, 4.4, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.2);
