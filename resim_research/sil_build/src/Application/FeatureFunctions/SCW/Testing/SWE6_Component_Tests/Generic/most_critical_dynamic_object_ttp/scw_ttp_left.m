% SCW - TTP calculation check:
testcase = class_testcase(h_suite, h_archive, [], [], 'ttp', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Check ttp
operator = operator_signal_value_compare(testcase, 'scw_ttp', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.SCW], 'SCW_critical_obj_ttp_left', 0.35, 0.35, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.49);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.01);
