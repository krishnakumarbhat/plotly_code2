testcase = class_testcase(h_suite, h_archive, [], [], 'lcda_bsw_Zone_Modification', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% In case of problems please contact Konrad

% Check is alert for BSW was raised
operator = operator_signal_edge_count(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'Lcda_out_bsw_alert_right');
operator.input_edge_type = class_input_edge(operator, enum_edge.RISING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% Check presence in zone for both targets at the begining of the simulation

% Presence in the zone for target object 1
operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'bsw_obj_f_in_zone', 1.3, 1.3, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
% Presence in the zone for target object 2
operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'bsw_obj_f_in_zone', 1.3, 1.3, [2], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

% Check drop in zone presence for terget object 2
operator = operator_signal_edge_count(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, [enum_bin.LCDA], 'bsw_obj_f_in_zone', 7.9, 8.4, [2], enum_signal_operation.AND);
operator.input_edge_type = class_input_edge(operator, enum_edge.FALLING);
operator.input_threshold_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

% Check position an presence in zone for both targets in the end of the simulation
% Presence in the zone for target object 1
operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.LCDA], 'bsw_obj_f_in_zone', 5.95, 5.95, [1], enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);