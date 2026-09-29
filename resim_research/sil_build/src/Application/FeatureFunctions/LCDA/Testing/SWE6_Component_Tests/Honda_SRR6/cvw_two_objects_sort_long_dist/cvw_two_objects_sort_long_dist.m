% LCDA - CVW object sorting by longitudinal distance:
testcase = class_testcase(h_suite, h_archive, [], [], 'cvw_two_objects_sort_long_dist', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare_any(testcase, 'test_alert_level', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'cvw_alert_left', 3.3, 9.0, 1, enum_signal_operation.NONE);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare_any(testcase, 'test_most_crit_obj_1', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'cvw_id_left', 3.3, 6.75, 1, enum_signal_operation.NONE);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare_any(testcase, 'test_most_crit_obj_2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'cvw_id_left', 6.8, 9, 1, enum_signal_operation.NONE);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);


