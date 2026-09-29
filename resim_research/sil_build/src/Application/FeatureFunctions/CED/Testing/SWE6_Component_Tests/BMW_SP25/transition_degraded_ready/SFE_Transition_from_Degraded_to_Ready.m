% CED Transition to Ready from Degraded Check:
testcase = class_testcase(h_suite, h_archive, [], [], 'SFE_transition_degraded_to_ready', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Checking if the state was Degraded until 2.5s
operator = operator_signal_value_compare(testcase, 'check_SFE_CED_State_Machine_Output_start', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'SFE_CED_State_Machine_Output', 0.05, 2.45, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 3);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);


% Checking if the state was Ready after 2.5s
operator = operator_signal_value_compare(testcase, 'check_SFE_CED_State_Machine_Output_end', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CED], 'SFE_CED_State_Machine_Output', 2.55, NaN, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
