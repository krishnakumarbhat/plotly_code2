% LCDA - BSW alert, check if holding works as expected - the total duration of the alert is tested.

testcase = class_testcase(h_suite, h_archive, [], [], 'bsw_sot_long_short_object', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% Case 1 BSW LEFT(object outside zone)
operator = operator_signal_edge_count(testcase, 'bsw_left_1', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'stla_thunder_bsw_alert_left',5420.69,5423.62,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);


% Case 2 BSW LEFT (object outside zone)
operator = operator_signal_edge_count(testcase, 'bsw_left_2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'stla_thunder_bsw_alert_left',5443.29,5453.22,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% Case 1 BSW RIGHT (object outside zone)
operator = operator_signal_edge_count(testcase, 'bsw_right_1', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'stla_thunder_bsw_alert_right',5421.73,5425.12,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);

% Case 2 BSW RIGHT (host speed < 1kph)
operator = operator_signal_edge_count(testcase, 'bsw_right_2', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_edge_signal = class_input_signal(operator, enum_bin.LCDA, 'stla_thunder_bsw_alert_right',5441.93,5455.28,1, enum_signal_operation.NONE);
operator.input_threshold_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_threshold_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);