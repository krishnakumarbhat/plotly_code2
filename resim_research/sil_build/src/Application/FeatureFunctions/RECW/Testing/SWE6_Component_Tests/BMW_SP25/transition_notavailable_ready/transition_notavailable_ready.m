% RECW Transition from Not available to Ready
testcase = class_testcase(h_suite, h_archive, [], [], 'RECW_transition_not_available_to_ready', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Checking if the state was Not available till 15s
operator = operator_signal_value_compare(testcase, 'check_bmw_sp25_recw_bus_signals_sm_state_start', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.RECW], 'RECW_BMW_SP25_sm_state', 0.05, 2.45, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% Checking if the state was Ready after 15s
operator = operator_signal_value_compare(testcase, 'check_bmw_sp25_recw_bus_signals_sm_state_end', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.RECW], 'RECW_BMW_SP25_sm_state', 2.55, 4.95, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);