% Max TTC value
% Check if Rivian TTC is activated below 2.5sec (customer specific value)

% testcase 1:
testcase = class_testcase(suite, archive, enum_enable.ENABLED, [], 'max_rivian_ttc', [enum_project.FF_CORE], [enum_tags.ENV_CITY], enum_test_type.UNIT_TEST);

operator = operator_signal_value_compare(testcase, '', enum_sensor.ARTIFICIAL_LOGFILE);
operator.input_signal = class_input_signal(operator, enum_bin.TA, 'rivian_ta_ttc_right', 0, 8.85);
operator.input_value = class_input_value(operator, enum_constant.CUSTOM_VALUE, 2.5);
operator.input_compare = class_input_compare(operator, enum_compare.GREATER_THAN);
operator.input_margin = class_input_value(operator, enum_constant.CUSTOM_VALUE, 0);
