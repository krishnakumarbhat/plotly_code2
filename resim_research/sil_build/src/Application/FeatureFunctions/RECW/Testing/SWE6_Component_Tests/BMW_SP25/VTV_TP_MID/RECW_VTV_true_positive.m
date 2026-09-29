% RECW True Positive check:
testcase = class_testcase(h_suite, h_archive, [], [], 'RECW_true_positive', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_LOW, enum_tags.VARIANT_ECU], enum_test_type.UNIT_TEST);

operator = operator_recw_bmw_sp25_check_precrash(testcase, 'RECW Precrash', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.RECW, 'RECW_BMW_SP25_status_precrash');

operator = operator_recw_bmw_sp25_check_warning(testcase, 'RECW Warning', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.RECW, 'RECW_BMW_SP25_status_collision_warning');
