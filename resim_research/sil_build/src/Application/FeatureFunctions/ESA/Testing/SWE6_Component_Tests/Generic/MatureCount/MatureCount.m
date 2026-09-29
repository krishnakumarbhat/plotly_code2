testcase = class_testcase(h_suite, h_archive, [], [], 'HostSpeed Hys Low', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT NO VALID OBJECTS THUS NO ALERT 

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', NaN, 1.0, [1]);

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Right", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_right', NaN, 1.0, [1]);


% EXPECT PRESENT VALID OBJECTS ON EACH SIDE AND ALERT RISED

operator = operator_signal_active(testcase, "Expect ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 1.05, 3.55, [1]);

operator = operator_signal_active(testcase, "Expect ESA Alert Right", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_right', 1.05, 3.55, [1]);

