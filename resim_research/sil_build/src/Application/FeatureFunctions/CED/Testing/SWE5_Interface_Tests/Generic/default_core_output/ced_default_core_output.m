% CED Default core output check:
testcase = class_testcase(h_suite, h_archive, [], [], 'ced_default_core_output', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

%CED ID
operator = operator_signal_value_compare(testcase, 'ced_core_out_id_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_id_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_id_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_id_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

%CED INDEX
operator = operator_signal_value_compare(testcase, 'ced_core_out_index_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_index_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_index_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_index_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% CED TTC
operator = operator_signal_value_compare(testcase, 'ced_core_out_ttc_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_ttc_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_ttc_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_ttc_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% CED TTP
operator = operator_signal_value_compare(testcase, 'ced_core_out_ttp_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_ttp_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_ttp_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_ttp_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_alert_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_alert_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_alert_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_alert_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_object_direction_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_direction_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_object_direction_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_direction_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_object_path_match_index_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_path_match_index_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_object_path_match_index_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_path_match_index_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 255);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_object_predicted_lat_pos_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_predicted_lat_pos_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_object_predicted_lat_pos_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_predicted_lat_pos_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% CED ced_front_bumper_pos_long
operator = operator_signal_value_compare(testcase, 'ced_core_out_object_front_bumper_pos_long_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_front_bumper_pos_long_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_object_front_bumper_pos_long_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_front_bumper_pos_long_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% CED ced_vcs_vel_rel_x
operator = operator_signal_value_compare(testcase, 'ced_core_out_object_vcs_vel_rel_x_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_vcs_vel_rel_x_left');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'ced_core_out_object_vcs_vel_rel_x_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED enum_bin.CED], 'ced_core_out_object_vcs_vel_rel_x_right');
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);