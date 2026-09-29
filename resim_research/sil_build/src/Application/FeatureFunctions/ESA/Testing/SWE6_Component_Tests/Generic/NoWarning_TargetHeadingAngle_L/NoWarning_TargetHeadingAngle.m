testcase = class_testcase(h_suite, h_archive, [], [], 'HostSpeed Hys Low', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT ESA STATUS ENABLED AND ESA ALERT LEFT SIDE
operator = operator_signal_active(testcase, "Expect  ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 0.2, 1.55, [1]);

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Right", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_right', 0.2, 1.55, [1]);

% EXPECT ESA STATUS ENABLED AND NO DEFAULT OUTPUT SINCE OBJECT WILL NOT BE CLASIFIED AS VALID

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 1.6, NaN, [1]);

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Right", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_right', 1.6, NaN, [1]);

operator = operator_signal_value_compare(testcase, 'esa_ttp_left Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttp_left', 1.6, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'esa_ttc_left Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttc_left', 1.6, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);

operator = operator_signal_value_compare(testcase, 'esa_ttp_right Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttp_right', 1.6, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'esa_ttc_right Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttc_right', 1.6, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
