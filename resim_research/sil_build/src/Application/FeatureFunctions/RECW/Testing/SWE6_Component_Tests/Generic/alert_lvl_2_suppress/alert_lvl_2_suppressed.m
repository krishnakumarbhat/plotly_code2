% Alert lvl 2 suppressed for pedestrian. Alert lvl 1 only triggered:
testcase = class_testcase(h_suite, h_archive, [], [], 'RECW_alert_lvl', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'verify_that_lvl_2_is_suppressed', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.RECW], 'RECW_output_alert_level', 4.0, 5.1, 1, enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);