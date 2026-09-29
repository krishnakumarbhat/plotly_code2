% Min BSW CVW speed (hys)
% Monitor the LCDA's behaviour when crossing and falling below the minimum speed

% testcase 1:
testcase = class_testcase(h_suite, h_archive, [], [], 'Min_BSW_CVW_speed', [enum_project.FF_CORE], [enum_tags.ENV_TESTING_GROUND, enum_tags.PRIO_HIGH], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'LCDA_BSW_Diabled_flag', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'f_bsw_is_enabled',[0],[0.4],[1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare(testcase, 'LCDA_CVW_Disabled_flag', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'f_cvw_is_enabled',[0],[0.4],[1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);

operator = operator_signal_value_compare(testcase, 'LCDA_BSW_Enabled_flag', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'f_bsw_is_enabled',[0.45],[3.20],[1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare(testcase, 'LCDA_CVW_Enabled_flag', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'f_cvw_is_enabled',[0.45],[3.20],[1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 1);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);

operator = operator_signal_value_compare(testcase, 'LCDA_BSW_Hys_Disabled_flag', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'f_bsw_is_enabled',[3.25],[4],[1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.05);

operator = operator_signal_value_compare(testcase, 'LCDA_CVW_Hys_Disabled_flag', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'f_cvw_is_enabled',[3.25],[4],[1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
operator.input_compare = class_input_compare(operator, enum_compare.EQUALS);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0.1);
