% SFE Default output check:
testcase = class_testcase(h_suite, h_archive, [], [], 'OSE_default_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'OSE_CED_alert_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'OSE_CED_alert_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'OSE_CED_alert_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'OSE_CED_alert_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'OSE_CED_id_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'OSE_CED_id_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'OSE_CED_id_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'OSE_CED_id_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'OSE_CED_obj_pred_lat_pos_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'OSE_CED_obj_pred_lat_pos_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'OSE_CED_obj_pred_lat_pos_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'OSE_CED_obj_pred_lat_pos_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'OSE_CED_ttc_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'OSE_CED_ttc_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'OSE_CED_ttc_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'OSE_CED_ttc_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_in_f_ced_enable', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'ced_core_in_f_ced_enable');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
