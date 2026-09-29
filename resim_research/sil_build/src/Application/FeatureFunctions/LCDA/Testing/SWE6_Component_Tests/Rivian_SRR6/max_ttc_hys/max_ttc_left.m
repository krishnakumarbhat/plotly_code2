% Max TTC value
% Check if TTC is between 4.5 - 5sec (customer specific value)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'max_ttc_left', [enum_project.FF_CORE], [enum_tags.ENV_CITY], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, 'cvw_ttc_left_value', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.LCDA, 'cvw_ttc_left', [5.2], [5.3], [1]);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 4.5);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN_OR_EQUALS);
