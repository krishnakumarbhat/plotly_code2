testcase = class_testcase(h_suite, h_archive, [], [], 'HostSpeed Hys Low', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT ESA STATUS ENABLED AND NO ALERT FOR COASTED OBJECTS

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 0.3, 1.35, [1]);

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Right", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_right', 0.3, 1.35, [1]);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_esa_status_Enabled', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_esa_status', 0.3, 1.35, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
