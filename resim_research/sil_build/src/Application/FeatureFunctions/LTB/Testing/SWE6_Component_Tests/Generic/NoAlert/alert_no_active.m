% NO ACTIVE ALERT LTB

% testcase 1:

testcase = class_testcase(h_suite, h_archive, [], [], 'ltb_alert_no_active', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_inactive(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LTB enum_bin.LTB], 'LTB_alert_level_left LTB_obj_alert_level_right', NaN, NaN, [1 1], enum_signal_operation.OR);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);


