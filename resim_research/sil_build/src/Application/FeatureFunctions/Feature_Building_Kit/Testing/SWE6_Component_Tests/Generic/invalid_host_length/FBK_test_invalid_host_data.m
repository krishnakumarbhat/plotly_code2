% FBK host length outside of range - sfl_status = 2 - SFL_STATUS_VEH_DATA_ERROR = (2)  /**< Vehicle Data Error */:
testcase = class_testcase(h_suite, h_archive, [], [], 'FBK_test_sfl_status', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);


operator = operator_signal_value_compare(testcase, 'verify_that_sfl_status_return_veh_data_error', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, [enum_bin.FBK], 'sfl_status', 0.0, 1.0, 1, enum_signal_operation.NONE);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
