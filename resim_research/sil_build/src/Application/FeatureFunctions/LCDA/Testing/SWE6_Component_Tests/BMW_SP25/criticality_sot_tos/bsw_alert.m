% LCDA - BSW SOT and TOS criticality check:
testcase = class_testcase(h_suite, h_archive, [], [], 'bsw_alert', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_active(testcase, 'TOS', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'bmw_sp25_bsw_alert_left', 1.20, 3.85, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.01);

operator = operator_signal_active(testcase, 'Middle', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'bmw_sp25_bsw_alert_left', 3.9, 6.3, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.01);

operator = operator_signal_active(testcase, 'SOT', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'bmw_sp25_bsw_alert_left', 6.35, 9.0, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.01);
