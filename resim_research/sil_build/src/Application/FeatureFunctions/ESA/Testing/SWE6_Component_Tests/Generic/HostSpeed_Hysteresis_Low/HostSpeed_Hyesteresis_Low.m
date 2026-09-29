testcase = class_testcase(h_suite, h_archive, [], [], 'HostSpeed Hys Low', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT ESA STATUS AND ALERT IN FIRST PART OF SCENARIO

operator = operator_signal_active(testcase, "Expect ESA Status disabled", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 0.25, 3.55, [1]);

operator = operator_signal_active(testcase, "Expect ESA acitvation speed in range", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'f_esa_host_speed_in_activation_range', 0.25, 3.55, [1]);

% EXPECT ESA STATUS AND ALERT IN FIRST PART OF SCENARIO

operator = operator_signal_inactive(testcase, "Expect ESA Status disabled", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 3.65, NaN, [1]);

operator = operator_signal_inactive(testcase, "Expect ESA acitvation speed in range", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'f_esa_host_speed_in_activation_range', 3.65, NaN, [1]);