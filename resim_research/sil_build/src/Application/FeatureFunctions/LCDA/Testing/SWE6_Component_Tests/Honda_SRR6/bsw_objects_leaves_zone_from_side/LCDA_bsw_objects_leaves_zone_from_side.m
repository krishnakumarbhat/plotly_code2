% LCDA Warning check:
testcase = class_testcase(h_suite, h_archive, [], [], 'bsw_alert', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.DEFAULT); 

operator = operator_signal_active(testcase, 'honda_srr6_bsw_alert_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'honda_srr6_bsw_alert_right',  0.95,    2.1, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_active(testcase, 'honda_srr6_bsw_alert_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'honda_srr6_bsw_alert_left',  0.95,    2.1, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);