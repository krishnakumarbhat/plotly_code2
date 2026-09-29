% Honda_Srr6 CED: Testing the e-ratch door signals. For each elatch zone input signal (0-1-2) two objects traveling at different lateral distance are created

testcase = class_testcase(h_suite, h_archive, [], [], 'eratch_zone', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Case 1: e-latch signal =0, standard zone, left object is outside the zone, no eratch alert expected
operator = operator_signal_edge_count(testcase, 'elatch_lvl_0_out_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.CED, 'honda_srr6_ced_eratch_alert_left',2.1,4.6,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% Case 2: e-latch signal =0, standard zone, right object is inside the zone,  eratch alert expected
operator = operator_signal_edge_count(testcase, 'elatch_lvl_0_in_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.CED, 'honda_srr6_ced_eratch_alert_right',2.1,4.6,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% Case 3: e-latch signal =1, short zone, left object is outside the zone, no eratch alert expected
operator = operator_signal_edge_count(testcase, 'elatch_lvl_1_out_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.CED, 'honda_srr6_ced_eratch_alert_left',8.1,10.6,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% Case 4: e-latch signal =1, short zone, right object is inside the zone,  eratch alert expected
operator = operator_signal_edge_count(testcase, 'elatch_lvl_1_in_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.CED, 'honda_srr6_ced_eratch_alert_right',8.1,10.6,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% Case 5: e-latch signal =2, standard zone, left object is outside the zone, no eratch alert expected
operator = operator_signal_edge_count(testcase, 'elatch_lvl_2_out_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.CED, 'honda_srr6_ced_eratch_alert_left',14.1,16.6,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% Case 6: e-latch signal =2, standard zone, right object is inside the zone,  eratch alert expected
operator = operator_signal_edge_count(testcase, 'elatch_lvl_2_in_zone', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.CED, 'honda_srr6_ced_eratch_alert_right',14.1,16.6,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

