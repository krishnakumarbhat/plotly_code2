% LCDA -  default TTP left check:
testcase = class_testcase(h_suite, h_archive, [], [], 'ttp', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Check ttp for CVW
operator = operator_signal_value_compare(testcase, 'bsw_ttp', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'cvw_ttp_left', 3.4, 4.0, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% Check ttp for BSW
operator = operator_signal_value_compare(testcase, 'cvw_ttp', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'bsw_ttp_left', 3.9, 4.0, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
