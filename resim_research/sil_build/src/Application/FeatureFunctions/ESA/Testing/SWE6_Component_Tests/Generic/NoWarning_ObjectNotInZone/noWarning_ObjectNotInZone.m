% ESA True Positive both sides check:
testcase = class_testcase(h_suite, h_archive, [], [], 'esa_interface_test_car_overtake_curvi', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

% NO ALERT LEFT SIDE

operator = operator_signal_inactive(testcase, "Expect no alert", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA enum_bin.ESA], 'ESA_Gen_f_esa_alert_left ESA_Gen_f_esa_alert_right', NaN, NaN, [1 1], enum_signal_operation.AND);

% NO ALERT RIGHT SIDE

operator = operator_signal_inactive(testcase, "Expect no alert", enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.ESA], 'esa_obj_f_in_zone', NaN, NaN, [1]);
