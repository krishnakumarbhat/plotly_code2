% LCDA - BSW heading hystereris check:
testcase = class_testcase(h_suite, h_archive, [], [], 'bsw_test_heading_hysteresis', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'target_turn_in_bsw_zone_cont_alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA enum_bin.LCDA], 'bsw_alert_right', 3.85, 5.15, 1);
operator.input_compare=class_input_compare(operator,enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value=class_input_value(operator,enum_constant.CUSTOM_VALUE,1);

operator = operator_signal_value_compare(testcase, 'oblique_target_no_alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA enum_bin.LCDA], 'bsw_alert_left bsw_alert_right', 0.5, 1.6, [1,1], enum_signal_operation.AND);
operator.input_compare=class_input_compare(operator,enum_compare.EQUALS);
operator.input_value=class_input_value(operator,enum_constant.CUSTOM_VALUE,0);
