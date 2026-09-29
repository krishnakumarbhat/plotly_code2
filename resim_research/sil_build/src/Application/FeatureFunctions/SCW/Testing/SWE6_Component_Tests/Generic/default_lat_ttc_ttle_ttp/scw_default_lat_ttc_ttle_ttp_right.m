% SCW - default values calculation check:
testcase = class_testcase(h_suite, h_archive, [], [], 'scw_default_values', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Check lateral ttc
operator = operator_signal_value_compare(testcase, 'scw_default_lat_ttc', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.SCW], 'SCW_critical_obj_lateral_ttc_right', 0.5, 1.5, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 10);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.00001);

% Check ttle
operator = operator_signal_value_compare(testcase, 'scw_default_ttle', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.SCW], 'SCW_critical_obj_ttle_right', 0.5, 1.5, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 10);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.00001);

% Check ttp
operator = operator_signal_value_compare(testcase, 'scw_default_ttp', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.SCW], 'SCW_critical_obj_ttp_right', 0.5, 1.5, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.00001);
