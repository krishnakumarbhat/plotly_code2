% CTA alert level test for speed criticality logic:
testcase = class_testcase(h_suite, h_archive, enum_enable.ENABLED, [], 'speed_criticality_level', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare_any(testcase, 'speed_criticality_level_1', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CTA, 'cta_out_alert_level_right',5.6,8.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare_any(testcase, 'speed_criticality_level_2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.CTA, 'cta_out_alert_level_right',16.8,18.8,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);