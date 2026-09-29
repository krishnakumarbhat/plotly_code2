% CTA Transition Ready to Active Check:
testcase = class_testcase(h_suite, h_archive, [], [], 'CTA_transition_ready_to_active', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Checking if the state was Ready till 15s
operator = operator_signal_value_compare(testcase, 'check_bmw_sp25_ctb_bus_signals_qualifier_function_ctb_start', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'bmw_sp25_ctb_bus_signals_qualifier_function_ctb', 0.05, 14.95, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% Checking if the state was Active after 15s
operator = operator_signal_value_compare(testcase, 'check_bmw_sp25_ctb_bus_signals_qualifier_function_ctb_end', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.CTA], 'bmw_sp25_ctb_bus_signals_qualifier_function_ctb', 15.05, NaN, [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 9);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);