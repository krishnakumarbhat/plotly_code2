% RECW True Negative check:
testcase = class_testcase(h_suite, h_archive, enum_enable.DISABLED, [], 'RECW_true_negative', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_inactive(testcase, 'RECW basic true negative check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.RECW, 'RECW_core_output_alert_level', NaN, NaN, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_inactive(testcase, 'BMW SRR5 RECW basic true negative check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.RECW, 'RECW_BMW_SP25_status_collision_warning', NaN, NaN, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);

operator = operator_signal_inactive(testcase, 'BMW SRR5 PCR basic true negative check', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.RECW, 'RECW_BMW_SP25_status_precrash', NaN, NaN, 1);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
