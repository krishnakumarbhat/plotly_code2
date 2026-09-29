testcase = class_testcase(h_suite, h_archive, [], [], 'HostSpeed Hys Low', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_active(testcase, "Expect ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 0.2, NaN, [1]);

% EXPECT TARGET ID 1 BEFORE LANE SHIFT 

operator = operator_signal_value_compare(testcase, 'ESA crit object Left Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_Left_id', 0.2, 1.7, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% EXPECT TARGET ID 2 AFTER SECOND TARGET LANE SHIFT

operator = operator_signal_value_compare(testcase, 'ESA crit object Left Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_Left_id', 1.75, 2.4, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);

% EXPECT TARGET ID 1 AFTER SECOND TARGET PASS HOST REAR BUMPER

operator = operator_signal_value_compare(testcase, 'ESA crit object Left Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_Left_id', 2.45, NaN, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);