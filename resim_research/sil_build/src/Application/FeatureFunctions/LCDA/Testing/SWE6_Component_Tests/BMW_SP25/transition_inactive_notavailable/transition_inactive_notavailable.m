% LCDA Transition from inactive to not-available Check:
testcase = class_testcase(h_suite, h_archive, [], [], 'LCDA_transition_inactive_to_notavailable', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Checking if the state was inactive till 2.5s
operator = operator_signal_value_compare(testcase, 'Check_LCDA_State_Machine_Output_Start', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'bmw_sp25_lcda_output_bus_signals_lcda_function_state', 0.05, 2.45, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% Checking if the state was not-available after 2.5s
operator = operator_signal_value_compare(testcase, 'Check_LCDA_State_Machine_Output_End', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'bmw_sp25_lcda_output_bus_signals_lcda_function_state', 2.55, NaN, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
