% ESA True Positive both sides check:
testcase = class_testcase(h_suite, h_archive, [], [], 'esa_disabled_from_calibration', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% EXPECT NO ALERT

operator = operator_signal_inactive(testcase, "Expect no alert", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_f_esa_alert_right', NaN, NaN, 1);

operator = operator_signal_inactive(testcase, "Expect no alert", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_f_esa_alert_left', NaN, NaN, 1);

% EXPECT ESA STATUS ENABLED

operator = operator_signal_active(testcase, "Expect ESA Status disabled", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'f_esa_enabled', NaN, NaN, [1]);

% EXPECT ESA CORE STATUS DISABLED LOW HOST SPEED VALUE

operator = operator_signal_value_compare(testcase, 'ESA Status disabled from calibratrion', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'ESA_Gen_esa_status', NaN, NaN, [1]);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 4);

% EXPECT ESA DISABLED FROM HOST SPEED FLAG ENABLED

operator = operator_signal_inactive(testcase, "Expect disabled flag speed activation range", enum_sensor.ARTIFICIAL_LOGFILE)
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'f_esa_host_speed_in_activation_range', NaN, NaN, 1);

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