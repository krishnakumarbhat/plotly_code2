testcase = class_testcase(h_suite, h_archive, [], [], 'HostSpeed Hys Low', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT CONTINOUS ESA LEFT ALERT

operator = operator_signal_active(testcase, "Expect ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 0.2, NaN, [1]);
