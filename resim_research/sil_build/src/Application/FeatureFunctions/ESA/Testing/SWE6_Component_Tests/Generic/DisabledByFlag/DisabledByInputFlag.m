% ESA True Positive both sides check:
testcase = class_testcase(h_suite, h_archive, [], [], 'esa_disabled_from_calibration', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT NO ALERT

operator = operator_signal_inactive(testcase, "Expect no alert", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_f_esa_alert_right', NaN, NaN, 1);

operator = operator_signal_inactive(testcase, "Expect no alert", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_f_esa_alert_left', NaN, NaN, 1);

% EXPECT ESA STATUS DISABLED

operator = operator_signal_inactive(testcase, "Expect ESA Status disabled", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'f_esa_enabled', NaN, NaN, [1]);

% EXPECT ESA CORE STATUS DISABLED FROM FLAG

operator = operator_signal_value_compare(testcase, 'ESA Status disabled from calibratrion', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_esa_status', NaN, NaN, [1]);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 3);

% EXPECT ESA DEFALUT TTC

operator = operator_signal_value_compare(testcase, 'ESA ttc left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttc_left', NaN, NaN, [1]);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);

operator = operator_signal_value_compare(testcase, 'ESA ttc right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttc_right', NaN, NaN, [1]);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);

% EXPECT ESA DEFATULT TTP

operator = operator_signal_value_compare(testcase, 'ESA ttp left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttp_left', NaN, NaN, [1]);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA ttp right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttp_right', NaN, NaN, [1]);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% EXPECT ESA DEFAULT DECELERATION

operator = operator_signal_value_compare(testcase, 'ESA decel left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_decel_to_reach_host_speed_left', NaN, NaN, [1]);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA decel right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_decel_to_reach_host_speed_right', NaN, NaN, [1]);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% EXPECT DEFAULT OBJECT ID PER SIDE

operator = operator_signal_value_compare(testcase, 'ESA_Object_ID', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA enum_bin.ESA], 'ESA_Gen_obj_left_id', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Object_ID', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA enum_bin.ESA], 'ESA_Gen_obj_right_id', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% EXPECT DEFAULT LENGTH AND WIDTH OF OBJECT PER SIDE

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Width_Left ', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_width_m', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Width_Right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_width_m', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Length_Left ', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_length_m', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Length_Right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_length_m', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% EXPECT DEFAULT OBJECT SPEED

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Speed_lon_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_long_speed_mps', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Speed_lon_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_long_speed_mps', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Speed_lat_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_lat_speed_mps', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Speed_lat_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_lat_speed_mps', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% EXPECT OBJECT DEFALUT DISTANCE TO HOST

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Dist_host_left', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_long_distance_m', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1000);

operator = operator_signal_value_compare(testcase, 'ESA_Obj_Dist_host_lat_right', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_right_long_distance_m', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1000);

% EXPECT OBJECT DEFAULT TTC

operator = operator_signal_value_compare(testcase, 'ESA_TTC_L', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttc_left', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);

operator = operator_signal_value_compare(testcase, 'ESA_TTC_R', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttc_right', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 100);

% EXPECT OBJECT DEFAULT TTP

operator = operator_signal_value_compare(testcase, 'ESA_TTP_L', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttp_left', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA_TTP_R', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_ttp_right', NaN, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);