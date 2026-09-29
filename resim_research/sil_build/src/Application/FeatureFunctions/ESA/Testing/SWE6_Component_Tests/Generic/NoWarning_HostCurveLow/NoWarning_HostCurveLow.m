testcase = class_testcase(h_suite, h_archive, [], [], 'HostSpeed Hys Low', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT ESA STATUS AND ALERT IN FIRST PART OF SCENARIO

operator = operator_signal_active(testcase, "Expect ESA Alert", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 0.3, 1.25, [1]);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_esa_status_Enabled', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_esa_status', 0.3, 1.25, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% EXPECT NO ALERT AND DISABLED BY LOW CURVE ESA STATUS IN SECOND PART OF SCENARIO

operator = operator_signal_inactive(testcase, "Expect No ESA Alert", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 1.3, 4.5, [1]);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_esa_status_Disabled', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_esa_status', 1.3, 4.5, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 6);

% EXPECT ALERT IN LAST PART OF SCENARIO
operator = operator_signal_active(testcase, "Expect ESA Status disabled", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 4.7, NaN, [1]);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_esa_status_Enabled', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_esa_status', 4.7, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);