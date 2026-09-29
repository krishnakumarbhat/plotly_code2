testcase = class_testcase(h_suite, h_archive, [], [], 'slc_alert_right', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'slc_obj_f_ttc_below_threshold', 3.6, 4.85, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);