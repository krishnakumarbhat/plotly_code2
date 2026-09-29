% SCW - TTLE calculation check:
testcase = class_testcase(h_suite, h_archive, [], [], 'ttle', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Check ttle
operator = operator_signal_value_compare(testcase, 'scw_ttle', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.SCW], 'SCW_critical_obj_ttle_left', 1.45, 1.45, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 5.52);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.01);
