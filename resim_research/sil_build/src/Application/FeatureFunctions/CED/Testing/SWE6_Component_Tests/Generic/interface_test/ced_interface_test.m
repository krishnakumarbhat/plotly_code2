% CED True Positive both sides check:
testcase = class_testcase(h_suite, h_archive, [], [], 'ced_interface_test', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% GENERIC INPUT
operator = operator_signal_value_compare(testcase, 'CED_Gen_in_f_ced_enable', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_in_f_ced_enable',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'CED_Gen_in_f_ced_front_mode', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_in_f_ced_front_mode',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'CED_Gen_in_f_ced_rear_mode', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_in_f_ced_rear_mode',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% GENERIC OUTPUT, LEFT SIDE
operator = operator_signal_value_compare(testcase, 'CED_Gen_out_alert_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_alert_left',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_direction', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_direction',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_heading_rad', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_heading_rad',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_id', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_id',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_lat_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_lat_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_length_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_length_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 4.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_long_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_long_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, -18.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_predicted_lat_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_predicted_lat_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_speed_mps', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_speed_mps',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 13.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_ttc_s', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_ttc_s',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_ttp_s', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_ttp_s',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_type', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_type',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 3);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_left_width_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_left_width_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1.5);


% GENERIC OUTPUT, RIGHT SIDE
operator = operator_signal_value_compare(testcase, 'CED_Gen_out_alert_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_alert_right',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_direction', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_direction',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_heading_rad', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_heading_rad',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_id', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_id',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_lat_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_lat_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_length_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_length_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 4.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_long_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_long_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, -18.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_predicted_lat_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_predicted_lat_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_speed_mps', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_speed_mps',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 13.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_ttc_s', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_ttc_s',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_ttp_s', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_ttp_s',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1.0);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_type', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_type',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 3);

operator = operator_signal_value_compare(testcase, 'CED_Gen_out_obj_right_width_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'CED_Gen_out_obj_right_width_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1.5);
