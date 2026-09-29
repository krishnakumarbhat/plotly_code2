% LCDA True Negative check:
testcase = class_testcase(h_suite, h_archive, [], [], 'lcda_no_alert', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_inactive(testcase, 'lcda_no_alert', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA enum_bin.LCDA], 'Lcda_out_bsw_alert_left Lcda_out_bsw_alert_right Lcda_out_cvw_alert_left Lcda_out_cvw_alert_right elc_alert_left elc_alert_right Lcda_out_slc_alert_left Lcda_out_slc_alert_right', NaN, NaN, [1 1 1 1 1 1 1 1], enum_signal_operation.OR);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% CVW default TTC
operator = operator_signal_value_compare(testcase, 'cvw_default_ttc_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_cvw_ttc_s_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 25);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'cvw_default_ttc_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_cvw_ttc_s_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 25);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% ELC default TTC
operator = operator_signal_value_compare(testcase, 'elc_default_ttc_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'elc_ttc_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'elc_default_ttc_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'elc_ttc_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% SLC default TTC
operator = operator_signal_value_compare(testcase, 'slc_default_lat_ttc_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_slc_ttc_s_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'slc_default_lat_ttc_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_slc_ttc_s_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'slc_default_lon_ttc_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'slc_lon_ttc_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'slc_default_lon_ttc_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'slc_lon_ttc_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
