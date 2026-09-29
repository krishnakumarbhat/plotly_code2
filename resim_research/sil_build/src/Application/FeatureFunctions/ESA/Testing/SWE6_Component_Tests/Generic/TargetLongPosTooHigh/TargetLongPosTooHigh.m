testcase = class_testcase(h_suite, h_archive, [], [], 'HostSpeed Hys Low', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT NO VALID OBJECTS THUS NO ALERT 

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', NaN, 2.65, [1]);

operator = operator_signal_inactive(testcase, "Expect NO ESA Alert Right", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_right', NaN, 2.65, [1]);

operator = operator_signal_value_compare(testcase, 'ESA crit object Left Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_id', NaN, 2.65, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_value_compare(testcase, 'ESA crit object Right Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_Right_id', NaN, 2.65, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% EXPECT PRESENT VALID OBJECTS ON EACH SIDE AND ALERT RISED

operator = operator_signal_active(testcase, "Expect NO ESA Alert Left", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_left', 2.75, 3.6, [1]);

operator = operator_signal_active(testcase, "Expect NO ESA Alert Right", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_alert_right', 2.75, 3.6, [1]);

operator = operator_signal_value_compare(testcase, 'ESA crit object Left Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_left_id', 2.75, 3.6, 1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);

operator = operator_signal_value_compare(testcase, 'ESA crit object Right Default', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_obj_Right_id', 2.75, 3.6, 1);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);

