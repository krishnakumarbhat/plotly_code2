% ESA True Positive both sides check:
testcase = class_testcase(h_suite, h_archive, [], [], 'esa_interface_test_car_overtake_curvi', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% GENERIC INPUT, BOTH SIDE

operator = operator_signal_value_compare(testcase, 'ESA_Gen_in_f_esa_enabled', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_in_f_esa_enabled',0.1,1.8,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% GENERIC OUTPUT, BOTH SIDE

operator = operator_signal_value_compare(testcase, 'ESA_Gen_esa_status', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_esa_status',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% GENERIC OUTPUT, LEFT SIDE

operator = operator_signal_value_compare(testcase, 'ESA_Gen_f_esa_alert_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_f_esa_alert_left',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_id', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_id',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_index', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_index',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_width_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_width_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1.8);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_length_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_length_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 4.5);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_long_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_long_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, -8.5);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_lat_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_lat_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.SMALLER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, -3.3);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_long_speed_mps', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_long_speed_mps',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 28);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_lat_speed_mps', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_lat_speed_mps',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, -0.1);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_ttc_s', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_ttc_s',1.1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_ttp_s', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_ttp_s',0.8,1.0,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.3);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_decel_to_reach_host_speed', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_decel_to_reach_host_speed',0.8,1.0,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 50.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_long_distance_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_long_distance_m',0.8,1.0,1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_left_existence_prob', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_existence_prob',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% GENERIC OUTPUT, RIGHT SIDE

operator = operator_signal_value_compare(testcase, 'ESA_Gen_f_esa_alert_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_f_esa_alert_right',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_id', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_id',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_index', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_index',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_width_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_width_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_length_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_length_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_long_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_long_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_lat_pos_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_lat_pos_m',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_long_speed_mps', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_long_speed_mps',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_lat_speed_mps', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_lat_speed_mps',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_ttc_s', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_ttc_s',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_ttp_s', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_ttp_s',0.8,1.0,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_decel_to_reach_host_speed', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_decel_to_reach_host_speed',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_long_distance_m', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_long_distance_m',0.8,1.0,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1000);

operator = operator_signal_value_compare(testcase, 'ESA_Gen_obj_right_existence_prob', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_existence_prob',1,1.2,1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
