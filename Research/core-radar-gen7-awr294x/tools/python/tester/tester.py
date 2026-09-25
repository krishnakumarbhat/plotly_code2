"""Tester.

This module contains all the classes and methods to do testing and comparisons. It can generate
a HTML report listing cases passed and failed. An XML file can also be generated to import
into Jenkins for continuous reporting and tracking.
"""
import sys
import datetime
import unittest
import time
import os
import junitparser
from os.path import abspath, join, dirname
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

#####################################
# Insert local directories into path
#####################################
BASE_PATH = dirname(abspath(__file__))
sys.path.insert(0, abspath(join(BASE_PATH, "..", "print_trace")))
sys.path.insert(0, abspath(join(BASE_PATH, "..", "build_html_file")))
sys.path.insert(0, abspath(join(BASE_PATH, "..", "junit")))

#####################################
# CUSTOM LIBRARIES
#####################################
from print_trace import Trace  # noqa: E402
from build_html_file import BuildHTMLFile, Row  # noqa: E402

# Create python script header
__author__ = "Devin Jaenicke"
__version__ = "1.0.0"
__email__ = "devin.k.jaenicke@aptiv.com"
__copyright__ = "Copyright 2018 Aptiv, All Rights Reserved."


#####################################
# CLASSES
#####################################
class __Tester_Unit_Tests(unittest.TestCase):
    # pylint: disable-msg=line-too-long, missing-docstring, too-many-public-methods, invalid-name
    def test_is_equal_string_pass(self):
        self.assertEqual(
            test_obj.is_equal("Tester.is_equal() - string - PASS test case", "string", "string"),
            "PASS",
            "Tester.is_equal() - string - PASS test case",
        )

    def test_is_equal_string_fail(self):
        self.assertEqual(
            test_obj.is_equal("Tester.is_equal() - string - FAIL test case", "string1", "string2"),
            "FAIL",
            "Tester.is_equal() - string - FAIL test case",
        )

    def test_is_equal_float_pass(self):
        self.assertEqual(
            test_obj.is_equal("Tester.is_equal() - float - PASS test case", 1.0005, 1.0005),
            "PASS",
            "Tester.is_equal() - float - PASS test case",
        )

    def test_is_equal_float_fail(self):
        self.assertEqual(
            test_obj.is_equal("Tester.is_equal() - float - FAIL test case", 1.0007, 1.0005),
            "FAIL",
            "Tester.is_equal() - float - FAIL test case",
        )

    def test_is_equal_int_pass(self):
        self.assertEqual(
            test_obj.is_equal("Tester.is_equal() - integer - PASS test case", 50000, 50000),
            "PASS",
            "Tester.is_equal() - integer - PASS test case",
        )

    def test_is_equal_int_fail(self):
        self.assertEqual(
            test_obj.is_equal("Tester.is_equal() - integer - FAIL test case", 50000, 49000),
            "FAIL",
            "Tester.is_equal() - integer - FAIL test case",
        )

    def test_is_almost_equal_float_pass(self):
        self.assertEqual(
            test_obj.is_almost_equal(
                "Tester.is_almost_equal() - float - PASS test case", 1.0005, 0.05, 1.00051
            ),
            "PASS",
            "Tester.is_almost_equal() - float - PASS test case",
        )

    def test_is_almost_equal_float_fail(self):
        self.assertEqual(
            test_obj.is_almost_equal(
                "Tester.is_almost_equal() - float - FAIL test case", 1.0007, 0.0001, 1.0005
            ),
            "FAIL",
            "Tester.is_almost_equal() - float - FAIL test case",
        )

    def test_is_almost_equal_int_pass(self):
        self.assertEqual(
            test_obj.is_almost_equal(
                "Tester.is_almost_equal() - integer - PASS test case", 1, 5, 4
            ),
            "PASS",
            "Tester.is_almost_equal() - integer - PASS test case",
        )

    def test_is_almost_equal_int_fail(self):
        self.assertEqual(
            test_obj.is_almost_equal(
                "Tester.is_almost_equal() - integer - FAIL test case", 1, 1, 3
            ),
            "FAIL",
            "Tester.is_almost_equal() - integer - FAIL test case",
        )

    def test_is_less_than_float_pass(self):
        self.assertEqual(
            test_obj.is_less_than(
                "Tester.is_less_than() - float - PASS test case", 1.0005, 1.0003
            ),
            "PASS",
            "Tester.is_less_than() - float - PASS test case",
        )

    def test_is_less_than_float_fail(self):
        self.assertEqual(
            test_obj.is_less_than(
                "Tester.is_less_than() - float - FAIL test case", 1.0005, 1.0005
            ),
            "FAIL",
            "Tester.is_less_than() - float - FAIL test case",
        )

    def test_is_less_than_int_pass(self):
        self.assertEqual(
            test_obj.is_less_than("Tester.is_less_than() - integer - PASS test case", 5, 4),
            "PASS",
            "Tester.is_less_than() - integer - PASS test case",
        )

    def test_is_less_than_int_fail(self):
        self.assertEqual(
            test_obj.is_less_than("Tester.is_less_than() - integer - FAIL test case", 7, 8),
            "FAIL",
            "Tester.is_less_than() - integer - FAIL test case",
        )

    def test_is_not_equal_string_pass(self):
        self.assertEqual(
            test_obj.is_not_equal(
                "Tester.is_not_equal() - string - PASS test case", "string1", "string2"
            ),
            "PASS",
            "Tester.is_not_equal() - string - PASS test case",
        )

    def test_is_not_equal_string_fail(self):
        self.assertEqual(
            test_obj.is_not_equal(
                "Tester.is_not_equal() - string - FAIL test case", "string", "string"
            ),
            "FAIL",
            "Tester.is_not_equal() - string - FAIL test case",
        )

    def test_is_not_equal_float_pass(self):
        self.assertEqual(
            test_obj.is_not_equal(
                "Tester.is_not_equal() - float - PASS test case", 1.0006, 1.0005
            ),
            "PASS",
            "Tester.is_not_equal() - float - PASS test case",
        )

    def test_is_not_equal_float_fail(self):
        self.assertEqual(
            test_obj.is_not_equal(
                "Tester.is_not_equal() - float - FAIL test case", 1.0005, 1.0005
            ),
            "FAIL",
            "Tester.is_not_equal() - float - FAIL test case",
        )

    def test_is_not_equal_int_pass(self):
        self.assertEqual(
            test_obj.is_not_equal(
                "Tester.is_not_equal() - integer - PASS test case", 50001, 50000
            ),
            "PASS",
            "Tester.is_not_equal() - integer - PASS test case",
        )

    def test_is_not_equal_int_fail(self):
        self.assertEqual(
            test_obj.is_not_equal(
                "Tester.is_not_equal() - integer - FAIL test case", 49000, 49000
            ),
            "FAIL",
            "Tester.is_not_equal() - integer - FAIL test case",
        )

    def test_is_greater_than_float_pass(self):
        self.assertEqual(
            test_obj.is_greater_than(
                "Tester.is_greater_than() - float - PASS test case", 1.0003, 1.0005
            ),
            "PASS",
            "Tester.is_greater_than() - float - PASS test case",
        )

    def test_is_greater_than_float_fail(self):
        self.assertEqual(
            test_obj.is_greater_than(
                "Tester.is_greater_than() - float - FAIL test case", 1.0005, 1.0005
            ),
            "FAIL",
            "Tester.is_greater_than() - float - FAIL test case",
        )

    def test_is_greater_than_int_pass(self):
        self.assertEqual(
            test_obj.is_greater_than("Tester.is_greater_than() - integer - PASS test case", 3, 4),
            "PASS",
            "Tester.is_greater_than() - integer - PASS test case",
        )

    def test_is_greater_than_int_fail(self):
        self.assertEqual(
            test_obj.is_greater_than("Tester.is_greater_than() - integer - FAIL test case", 9, 8),
            "FAIL",
            "Tester.is_greater_than() - integer - FAIL test case",
        )

    def test_is_greater_than_or_equal_float_pass(self):
        self.assertEqual(
            test_obj.is_greater_than_or_equal(
                "Tester.is_greater_than_or_equal() - float - PASS test case", 1.0003, 1.0005
            ),
            "PASS",
            "Tester.is_greater_than_or_equal() - float - PASS test case",
        )

    def test_is_greater_than_or_equal_float_fail(self):
        self.assertEqual(
            test_obj.is_greater_than_or_equal(
                "Tester.is_greater_than_or_equal() - float - FAIL test case", 1.0006, 1.0005
            ),
            "FAIL",
            "Tester.is_greater_than_or_equal() - float - FAIL test case",
        )

    def test_is_greater_than_or_equal_int_pass(self):
        self.assertEqual(
            test_obj.is_greater_than_or_equal(
                "Tester.is_greater_than_or_equal() - integer - PASS test case", 4, 4
            ),
            "PASS",
            "Tester.is_greater_than_or_equal() - integer - PASS test case",
        )

    def test_is_greater_than_or_equal_int_fail(self):
        self.assertEqual(
            test_obj.is_greater_than_or_equal(
                "Tester.is_greater_than_or_equal() - integer - FAIL test case", 9, 8
            ),
            "FAIL",
            "Tester.is_greater_than_or_equal() - integer - FAIL test case",
        )

    def test_is_less_than_or_equal_float_pass(self):
        self.assertEqual(
            test_obj.is_less_than_or_equal(
                "Tester.is_less_than_or_equal() - float - PASS test case", 1.0005, 1.0003
            ),
            "PASS",
            "Tester.is_less_than_or_equal() - float - PASS test case",
        )

    def test_is_less_than_or_equal_float_fail(self):
        self.assertEqual(
            test_obj.is_less_than_or_equal(
                "Tester.is_less_than_or_equal() - float - FAIL test case", 1.0004, 1.0005
            ),
            "FAIL",
            "Tester.is_less_than_or_equal() - float - FAIL test case",
        )

    def test_is_less_than_or_equal_int_pass(self):
        self.assertEqual(
            test_obj.is_less_than_or_equal(
                "Tester.is_less_than_or_equal() - integer - PASS test case", 4, 4
            ),
            "PASS",
            "Tester.is_less_than_or_equal() - integer - PASS test case",
        )

    def test_is_less_than_or_equal_int_fail(self):
        self.assertEqual(
            test_obj.is_less_than_or_equal(
                "Tester.is_less_than_or_equal() - integer - FAIL test case", 7, 8
            ),
            "FAIL",
            "Tester.is_less_than_or_equal() - integer - FAIL test case",
        )

    def test_is_true_int_pass(self):
        self.assertEqual(
            test_obj.is_true("Tester.is_true() - integer - PASS test case", -1),
            "PASS",
            "Tester.is_true() - integer - PASS test case",
        )

    def test_is_true_int_fail(self):
        self.assertEqual(
            test_obj.is_true("Tester.is_true() - integer - FAIL test case", 0),
            "FAIL",
            "Tester.is_true() - integer - FAIL test case",
        )

    def test_is_true_float_pass(self):
        self.assertEqual(
            test_obj.is_true("Tester.is_true() - float - PASS test case", 7.4),
            "PASS",
            "Tester.is_true() - float - PASS test case",
        )

    def test_is_true_float_fail(self):
        self.assertEqual(
            test_obj.is_true("Tester.is_true() - float - FAIL test case", 0.0),
            "FAIL",
            "Tester.is_true() - float - FAIL test case",
        )

    def test_is_true_bool_pass(self):
        self.assertEqual(
            test_obj.is_true("Tester.is_true() - boolean - PASS test case", True),
            "PASS",
            "Tester.is_true() - boolean - PASS test case",
        )

    def test_is_true_bool_fail(self):
        self.assertEqual(
            test_obj.is_true("Tester.is_true() - boolean - FAIL test case", False),
            "FAIL",
            "Tester.is_true() - boolean - FAIL test case",
        )

    def test_is_false_int_pass(self):
        self.assertEqual(
            test_obj.is_false("Tester.is_false() - integer - PASS test case", 0),
            "PASS",
            "Tester.is_false() - integer - PASS test case",
        )

    def test_is_false_int_fail(self):
        self.assertEqual(
            test_obj.is_false("Tester.is_false() - integer - FAIL test case", -1),
            "FAIL",
            "Tester.is_false() - integer - FAIL test case",
        )

    def test_is_false_float_pass(self):
        self.assertEqual(
            test_obj.is_false("Tester.is_false() - float - PASS test case", 0.0),
            "PASS",
            "Tester.is_false() - float - PASS test case",
        )

    def test_is_false_float_fail(self):
        self.assertEqual(
            test_obj.is_false("Tester.is_false() - float - FAIL test case", 0.2),
            "FAIL",
            "Tester.is_false() - float - FAIL test case",
        )

    def test_is_false_bool_pass(self):
        self.assertEqual(
            test_obj.is_false("Tester.is_false() - boolean - PASS test case", False),
            "PASS",
            "Tester.is_false() - boolean - PASS test case",
        )

    def test_is_false_bool_fail(self):
        self.assertEqual(
            test_obj.is_false("Tester.is_false() - boolean - FAIL test case", True),
            "FAIL",
            "Tester.is_false() - boolean - FAIL test case",
        )

    def test_timer_class(self):
        test_obj.timer.start()
        time.sleep(2)
        test_obj.timer.stop()
        self.assertEqual(
            test_obj.is_equal(
                "Tester.timer() - start to stop", 2, test_obj.timer.elapsed.seconds, "sec"
            ),
            "PASS",
            "Tester.timer() - start to stop",
        )


class TestTimer(object):
    """
    Internal Class used by the Tester class to time the total test execution time.
    """

    def __init__(self):
        """Initialize the test timer."""
        self.__start = None
        self.elapsed = None
        self.status = "not_started"

    def start(self):
        """
        Start the test timer.
        """
        if self.status == "not_started":
            self.__start = datetime.datetime.now().replace(microsecond=0)
            self.status = "running"
        else:
            Trace.print_error("Timer already running! Ignoring start request.")

    def stop(self):
        """
        Stop the test timer and compute the elapsed time.
        """
        if self.status == "running":
            self.elapsed = datetime.datetime.now().replace(microsecond=0) - self.__start
            self.status = "stopped"
        else:
            Trace.print_error("Timer not running! Ignoring stop request.")


class TestResult(object):
    # pylint: disable-msg=too-few-public-methods
    """
    Internal Class used by the Tester class to store each test's results.

    :type test_id: integer
    :param test_id: number associated with the current test

    :type description: string
    :param description: short description of the current test

    :type expected: string | int | long | float
    :param expected: expected result of the current test

    :type actual: string | int | long | float
    :param actual: actual result of the current test

    :type result: string
    :param result: PASS or FAIL

    :type units: string
    :param units: units of the expected and actual values (defaults to '-')

    """

    def __init__(
        self, test_id, description, expected, actual, result, wi="-", units="-", bg_selector=0
    ):
        """Initialize the test result."""
        if result == "PASS":
            row_colors = ["black", "black", "black", "black", "black", "black", "green"]
        elif result == "FAIL":
            row_colors = ["black", "black", "black", "black", "black", "black", "red"]
        else:
            row_colors = ["black"] * 7
        row_align = ["center", "center", "left", "center", "center", "center", "center"]
        self.html_row = Row(
            [str(test_id), str(wi), description, str(expected), str(actual), units, result],
            row_colors,
            row_align,
        )
        self.description = description
        self.expected = expected
        self.actual = actual
        self.result = result
        self.bg_selector = bg_selector


class Tester(object):
    """
    Class to be used by all integration test Python scripts for collecting results and generating a test report.

    :type test_name: string
    :param test_name: overall test name, e.g. MRR_If_Integration_Test

    :type test_summary: string
    :param test_summary: short summary of what the overall test
                         accomplishes, e.g. verifies the MRR interface

    :type program: string
    :param program: optional parameter to specify the Aptiv
                    program, default is blank

    :type test_group_name: string
    :param test_group_name: (Optional argument) If the same test_group_name is specified for
        multiple Tester class instances, they will be grouped together when reported in Jenkins.

    """

    def __init__(
        self,
        test_name,
        test_summary,
        test_group_name="",
        program="",
        comments="",
        part_number="",
        powerCycles=1,
    ):
        """Initialize the tester."""
        self.HTML_TABLE_NUM_COLUMNS = 7  # pylint: disable-msg=invalid-name

        self.name = test_name
        self.test_summary = test_summary
        self.part_number = part_number
        self.program = program
        self.timer = TestTimer()
        self.powerCycle = powerCycles
        self.comments = comments
        self.aggregate_test_result = "PASS"
        self.results = []
        self.num_tests = 0
        self.num_passes = 0
        self.num_failures = 0
        self.classname = ""
        self.images = []
        self.html_report_title = "{0} Report".format(test_name)
        self.html_header = []
        self.html_header.append("{0} Report".format(test_name))
        self.junit_xml = []
        self.tester_group_name = test_group_name

        self.num_or_str_types = [float, int, str]
        self.num_or_bool_types = [float, int, bool]
        self.num_types = [float, int]

    def __verify_arg_types(self, args_list, supported_types_list):
        for arg in args_list:
            if type(arg) not in supported_types_list:  # pylint: disable-msg=unidiomatic-typecheck
                Trace.print_error("Invalid argument data type given!", True)

    def add_part_number(self, part_number):
        """Add the part number."""
        self.part_number = part_number

    def add_info_row(self, info_str):
        """
        Add a test information step into the html report.

        :type info_str: string
        :param info_str: the info string to be added into the test description

        """
        self.num_tests += 1
        self.results.append(TestResult(self.num_tests, info_str, "-", "-", "-", "-", "-", 3))

    def add_result_row(self, description, actual):
        """
        Add a test information step into the html report.

        :type info_str: string
        :param info_str: the info string to be added into the test description

        """
        self.num_tests += 1
        self.results.append(TestResult(self.num_tests, description, "-", actual, "-", "-", "-"))

    def is_equal(self, description, expected, actual, units="-", wi="-", bg_selector=0):
        """
        Compare an actual result with an expected result.

        :type description: string
        :param description: a short description of the current test
                            (will appear in the html report table)

        :type expected: string | int | long | float
        :param expected: expected result of the current test

        :type actual: string | int | long | float
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the expected and actual values (defaults to '-')

        """
        self.__verify_arg_types([expected, actual], self.num_or_str_types)

        if isinstance(expected, str) and type(expected) is not type(actual):
            Trace.print_error(
                "Data types for expected and actual \
                              arguments must be the same!",
                True,
            )

        if expected == actual:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "{0}".format(expected)
        self.num_tests += 1
        self.results.append(
            TestResult(
                self.num_tests, description, expected_str, actual, result, wi, units, bg_selector
            )
        )
        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def is_not_equal(self, description, expected, actual, units="-", wi="-", bg_selector=0):
        """
        Compare an actual result with an expected result.

        :type description: string
        :param description: a short description of the current test
                            (will appear in the html report table)

        :type expected: string | int | long | float
        :param expected: expected result of the current test

        :type actual: string | int | long | float
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the expected and actual values (defaults to '-')

        """
        self.__verify_arg_types([expected, actual], self.num_or_str_types)

        if isinstance(expected, str) and type(expected) is not type(actual):
            Trace.print_error(
                "Data types for expected and actual \
                              arguments must be the same!",
                True,
            )

        if expected != actual:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "!{0}".format(expected)
        self.num_tests += 1
        self.results.append(
            TestResult(
                self.num_tests, description, expected_str, actual, result, wi, units, bg_selector
            )
        )

        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def is_almost_equal(
        self, description, expected, tolerance, actual, units="-", wi="-", bg_selector=0
    ):
        """
        Compare an actual result with an expected result +/- tolerance.

        :type description: string
        :param description: short description of the current test
                            (will appear in the html report table)

        :type expected: int | long | float
        :param expected: expected result of the current test

        :type tolerance: int | long | float
        :param tolerance: amount added and subtracted from the expected result

        :type actual: int | long | float
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the expected and actual values (defaults to '-')

        """
        self.__verify_arg_types([expected, tolerance, actual], self.num_types)

        low = expected - tolerance
        high = expected + tolerance

        if actual >= low and actual <= high:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "{0:.2f} <= Actual <= {1:.2f}".format(low, high)
        self.num_tests += 1
        self.results.append(
            TestResult(self.num_tests, description, expected_str, actual, result, wi, units)
        )
        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def is_less_than(self, description, less_than_value, actual, units="-", wi="-", bg_selector=0):
        """
        Compare that an actual result is less than expected result.

        :type description: string
        :param description: a short description of the current test
                            (will appear in the html report table)

        :type less_than_value: int | long | float
        :param expected: value that the actual result should be less than

        :type actual: int | long | float
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the expected and actual values (defaults to '-')

        """
        self.__verify_arg_types([less_than_value, actual], self.num_types)

        if actual < less_than_value:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "Actual < {0}".format(less_than_value)
        self.num_tests += 1
        self.results.append(
            TestResult(
                self.num_tests,
                description,
                expected_str,
                actual,
                result,
                wi=wi,
                units=units,
                bg_selector=bg_selector,
            )
        )
        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def is_greater_than(self, description, greater_than_value, actual, units="-", wi="-"):
        """
        Compare that an actual result is greater than expected result.

        :type description: string
        :param description: a short description of the current test
                            (will appear in the html report table)

        :type greater_than_value: int | long | float
        :param expected: value that the actual result should be greater than

        :type actual: int | long | float
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the expected and actual values (defaults to '-')

        """
        self.__verify_arg_types([greater_than_value, actual], self.num_types)

        if actual > greater_than_value:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "Actual > {0}".format(greater_than_value)
        self.num_tests += 1
        self.results.append(
            TestResult(self.num_tests, description, expected_str, actual, result, wi, units)
        )
        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def is_less_than_or_equal(
        self, description, less_than_or_equal_value, actual, units="-", wi="-"
    ):
        """
        Compare that an actual result is less than or equal to expected result.

        :type description: string
        :param description: a short description of the current test
                            (will appear in the html report table)

        :type less_than_value: int | long | float
        :param expected: value that the actual result should be less than or equal to

        :type actual: int | long | float
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the expected and actual values (defaults to '-')

        """
        self.__verify_arg_types([less_than_or_equal_value, actual], self.num_types)

        if actual <= less_than_or_equal_value:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "Actual <= {0}".format(less_than_or_equal_value)
        self.num_tests += 1
        self.results.append(
            TestResult(self.num_tests, description, expected_str, actual, result, wi, units)
        )
        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def is_greater_than_or_equal(
        self, description, greater_than_or_equal_value, actual, units="-", wi="-"
    ):
        """
        Compare that an actual result is greater than or equal to expected result.

        :type description: string
        :param description: a short description of the current test
                            (will appear in the html report table)

        :type less_than_value: int | long | float
        :param expected: value that the actual result should be greater than or equal to

        :type actual: int | long | float
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the expected and actual values (defaults to '-')

        """
        self.__verify_arg_types([greater_than_or_equal_value, actual], self.num_types)

        if actual >= greater_than_or_equal_value:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "Actual >= {0}".format(greater_than_or_equal_value)
        self.num_tests += 1
        self.results.append(
            TestResult(self.num_tests, description, expected_str, actual, result, wi, units)
        )
        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def is_true(self, description, actual, units="-", wi="-"):
        """
        Compare that an actual result is True.

        :type description: string
        :param description: a short description of the current test
                            (will appear in the html report table)

        :type actual: int | long | float | bool
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the actual value (defaults to '-')

        """
        self.__verify_arg_types([actual], self.num_or_bool_types)

        if actual:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "{0}".format(True)
        self.num_tests += 1
        self.results.append(
            TestResult(self.num_tests, description, expected_str, actual, result, wi, units)
        )
        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def is_false(self, description, actual, units="-", wi="-"):
        """
        Compare that an actual result is False.

        :type description: string
        :param description: a short description of the current test
                            (will appear in the html report table)

        :type actual: int | long | float | bool
        :param actual: actual result of the current test

        :type units: string
        :param units: units of the actual value (defaults to '-')

        """
        self.__verify_arg_types([actual], self.num_or_bool_types)

        if not actual:
            result = "PASS"
            self.num_passes += 1
        else:
            result = "FAIL"
            self.aggregate_test_result = "FAIL"
            self.num_failures += 1

        expected_str = "{0}".format(False)
        self.num_tests += 1
        self.results.append(
            TestResult(self.num_tests, description, expected_str, actual, result, wi, units)
        )
        self.__add_test_to_junit_xml(description, expected_str, actual, result)

        return result

    def add_image(self, data, file, title="", xlabel="", ylabel=""):
        """
        Add an image to the tester.
        """
        fig, ax = plt.subplots(nrows=1, ncols=1)  # create figure & 1 axis
        # default 6.4 and 4.8
        # WIDTH_600_CNT = 36
        # WIDTH_100_CNT = 24
        WIDTH_50_CNT = 12
        # MYPCWIDTH = 18

        fig.set_figwidth(WIDTH_50_CNT)
        ax.plot(data)
        ax.set_title(title)
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        fig.savefig(file)  # save the figure to file
        plt.close(fig)  # close the figure window
        self.images.append(file)

    def add_image_compare5(
        self, data1, data2, label1, label2, file, title="", xlabel="", ylabel=""
    ):
        """
        Plot two inputs on the same graph.

        On seperate subplot below it plots the difference between the two
        """
        # Make an example plot with two subplots...
        fig = plt.figure()
        # default 6.4 and 4.8
        # WIDTH_600_CNT = 36
        # WIDTH_100_CNT = 24
        WIDTH_50_CNT = 12
        # MYPCWIDTH = 18

        fig.set_figwidth(WIDTH_50_CNT)
        ax1 = fig.add_subplot(2, 1, 1)
        ax1.set_title(title)
        ax1.plot(data1, "tab:blue", label=label1)
        ax1.plot(data2, "tab:orange", label=label2)
        ax1.set_ylabel(ylabel)
        ax1.legend()
        diff = []
        for i in range(0, len(data1)):
            diff.append(data1[i] - data2[i])
        ax1.yaxis.set_major_formatter(mtick.FormatStrFormatter("%.1f"))
        ax2 = fig.add_subplot(2, 1, 2)
        ax2.plot(diff, label="Difference")
        ax2.set_title("\nDifference")
        ax2.set_xlabel(xlabel)
        ax2.set_ylabel(ylabel)
        ax2.yaxis.set_major_formatter(mtick.FormatStrFormatter("%.1f"))

        fig.savefig(file)  # save the figure to file
        plt.close(fig)  # close the figure window
        self.images.append(file)

    def add_image_compare4(
        self,
        file,
        data1,
        data2,
        label1,
        label2,
        data3=None,
        data4=None,
        label3=None,
        label4=None,
        title="",
        xlabel="",
        ylabel="",
    ):
        """
        Plot 2-4 inputs on the same graph that can overlap.
        """
        fig, ax = plt.subplots(nrows=1, ncols=1)  # create figure & 1 axis

        # default 6.4 and 4.8
        # WIDTH_600_CNT = 36
        # WIDTH_100_CNT = 24
        WIDTH_50_CNT = 12
        # MYPCWIDTH = 18

        fig.set_figwidth(WIDTH_50_CNT)

        ax.plot(data1, label=label1)
        ax.plot(data2, label=label2)
        if data3 is not None:
            ax.plot(data3, label=label3)
        if data4 is not None:
            ax.plot(data4, label=label4)
        ax.set_title(title)
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        ax.legend()
        fig.savefig(file)  # save the figure to file
        plt.close(fig)  # close the figure window
        self.images.append(file)

    def add_image_compare3(
        self, data1, data2, label1, label2, file, title="", xlabel="", ylabel=""
    ):
        """
        Plot 2-4 inputs on the same graph.
        """
        # Make an example plot with two subplots...
        fig = plt.figure()

        ax1 = fig.add_subplot(1, 2, 1)
        ax1.set_title(title)
        ax1.plot(data1, label=label1)
        ax1.set_ylabel(ylabel)
        ax2 = fig.add_subplot(1, 2, 2)
        ax2.plot(data2, label=label2)

        ax2.set_xlabel(xlabel)
        ax2.set_ylabel(ylabel)

        fig.savefig(file)  # save the figure to file
        plt.close(fig)  # close the figure window
        self.images.append(file)

    def add_image_compare2(
        self, data1, data2, label1, label2, file, title="", xlabel="", ylabel=""
    ):
        """
        Plot 2 inputs on seperate subplots.
        """
        # Make an example plot with two subplots...
        fig = plt.figure()
        # default 6.4 and 4.8
        # WIDTH_600_CNT = 36
        # WIDTH_100_CNT = 24
        WIDTH_50_CNT = 12
        # MYPCWIDTH = 18

        fig.set_figwidth(WIDTH_50_CNT)
        ax1 = fig.add_subplot(2, 1, 1)
        ax1.set_title(title)
        ax1.plot(data1, "tab:orange", label=label1)
        ax1.set_ylabel(ylabel)

        ax1.yaxis.set_major_formatter(mtick.FormatStrFormatter("%.1f"))
        ax2 = fig.add_subplot(2, 1, 2)
        ax2.plot(data2, label=label2)

        ax2.set_xlabel(xlabel)
        ax2.set_ylabel(ylabel)
        ax2.yaxis.set_major_formatter(mtick.FormatStrFormatter("%.1f"))

        fig.savefig(file)  # save the figure to file
        plt.close(fig)  # close the figure window
        self.images.append(file)

    def add_image_compare(
        self, data1, data2, label1, label2, file, title="", xlabel="", ylabel=""
    ):
        """
        Plot 2 inputs on the same graph that can overlap.
        """
        fig, ax = plt.subplots(nrows=1, ncols=1)  # create figure & 1 axis

        # default 6.4 and 4.8
        # WIDTH_600_CNT = 36
        # WIDTH_100_CNT = 24
        WIDTH_50_CNT = 12
        # MYPCWIDTH = 18

        fig.set_figwidth(WIDTH_50_CNT)

        ax.plot(data1, label=label1)
        ax.plot(data2, label=label2)
        ax.set_title(title)
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        ax.legend()
        fig.savefig(file)  # save the figure to file
        plt.close(fig)  # close the figure window
        self.images.append(file)

    def __add_test_to_junit_xml(self, description, expected_str, actual, result):
        if "" == self.tester_group_name:
            self.classname = self.name + "." + self.name
        else:
            self.classname = self.tester_group_name + "." + self.name

        junit_case = junitparser.TestCase(name=description)
        junit_case.classname = self.classname
        if "PASS" == result:
            self.junit_xml.append(junit_case)
        else:
            junit_case.result = [
                junitparser.Failure(
                    "Expected: {0}, but got {1}".format(expected_str, actual), "Output Mismatch"
                )
            ]
            self.junit_xml.append(junit_case)

    def generate_html_report(self, output_path, name, open_report=False):
        """
        Generate the html report.

        :type output_path: string
        :param output_path: path where the html file will be generated

        :type name: string
        :param name: html report name (file extension .html not required)

        :type open_report: boolean
        :param open_report: Flag to indicate whether to open the HTML report after generation
            Defaults to False

        """
        # Verify the output path exists
        if not os.path.exists(output_path):
            Trace.print_error("Output path does not exist!", True)

        html = BuildHTMLFile(os.path.join(output_path, name + ".html"))

        # Add the defined Title
        html.add_title(self.html_report_title)

        # Add the defined Header
        html.add_header(self.html_header)

        # Start the Body
        html.start_body()
        html.add_hor_line()

        basic_info = ""
        basic_info += "Program: {0}<br>".format(self.program)
        if self.part_number != "":
            basic_info += "Part Number(s): {0}<br>".format(self.part_number)
        basic_info += "Summary: {0}<br>".format(self.test_summary)
        basic_info += "Tester Net Id: {0}<br>".format(os.getenv("username"))
        basic_info += "Bench Name: {0}<br>".format(os.getenv("computername"))
        basic_info += "Date Performed: {0}<br>".format(datetime.date.today())

        if self.comments != "":
            basic_info += "Comments: {0}<br>".format(self.comments)

        html.add_text(basic_info)

        html.add_text("Overall Test Status: {0}<br>".format(self.aggregate_test_result), bold=True)
        html.add_text(
            "Totals: Passing = {0} | Failing = {1}".format(self.num_passes, self.num_failures)
        )

        if self.timer.elapsed:
            html.add_text("Test execution time = {0} (hr:min:sec)".format(self.timer.elapsed))

        if self.powerCycle > 1:
            html.add_text("Power Cycles: {0}".format(self.powerCycle))

        html.create_table(self.HTML_TABLE_NUM_COLUMNS)
        html.add_header_row_to_table(
            Row(
                [
                    "Test Id",
                    "WI",
                    "Test Description",
                    "Expected Result",
                    "Actual Result",
                    "Units",
                    "Status",
                ],
                ["black"] * self.HTML_TABLE_NUM_COLUMNS,
            )
        )
        for result in self.results:
            html.add_row_to_table(result.html_row, result.bg_selector)
        html.close_table()

        html.add_hor_line()

        for img in self.images:
            html.add_image(img)

        html.add_hor_line()
        # End the Body
        html.end_body()

        # Write the HTML and XML file
        html.write_html_to_file()
        self.__generate_xml_report(output_path, name)

        if open_report is True:
            html.open_html_file()

    def __generate_xml_report(self, output_path, name):
        junit_suite = junitparser.TestSuite(self.classname)
        fail_count = 0
        for junit_test in self.junit_xml:
            if junit_test.result:
                fail_count += 1
            junit_suite.add_testcase(junit_test)
        junit_suite.tests = len(self.junit_xml)
        junit_suite.failures = fail_count
        xmlreport = junitparser.JUnitXml(self.classname)
        xmlreport.add_testsuite(junit_suite)
        xmlreport.failures = fail_count
        xmlreport.tests = len(self.junit_xml)
        if self.timer.elapsed:
            junit_suite.time = self.timer.elapsed.total_seconds()
            xmlreport.time = self.timer.elapsed.total_seconds()

        xmlreport.write(os.path.join(output_path, name + "_jenkins.xml"), pretty=True)

        # Write the plot data needed by Jenkins
        plot_report = ["<PlotReport>"]
        for result in self.results:
            plot_report.append("<" + result.description.replace(" ", "_") + ">")
            plot_report.append(str(result.actual))
            plot_report.append("</" + result.description.replace(" ", "_") + ">")
        plot_report.append("</PlotReport>")

        plot_file = open(os.path.join(output_path, name + "_plot_info.xml"), "w")
        str_plot_xml = "\n".join(plot_report)
        plot_file.write(str_plot_xml)
        plot_file.close()


#####################################
# Script begins here
#####################################
if __name__ == "__main__":  # noqa: F401

    test_obj = Tester(
        "Tester Class Unit Tests",
        "Used to demonstrate the test report as well \
                       as display the results of the Tester class unit tests.",
    )
    suite = unittest.TestLoader().loadTestsFromTestCase(__Tester_Unit_Tests)
    unittest.TextTestRunner(verbosity=2).run(suite)
    test_obj.add_info_row("Information row for adding test context or setup step info")
    test_obj.generate_html_report(
        os.path.join(os.getcwd(), "unit_test"), "Tester_Class_Unit_Test_Results"
    )

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/       JIRA AAA-####
#             Initials    Explanation of changes done here.
#    Date        By             Description
# ----------  ---------   -----------------------
# 04/20/2018  Devin J.    APS-4249 Initial creation
# 06/05/2018  Luke W.     APS-6333 Added is_less_than method
# 06/13/2018  Rachel M.   APS-6422 Added is_grater_than, is_greater_than_or_equal,
#                                  is_less_than_or_equal, is_not_equal,
#                                  is_true, and is_false methods
# 07/02/2018  Luke W.     APS-6809 Added XML generation needed by Jenkins
#                                  Cleaned up code to pylint and sonarqube standards
# 08/14/2018  Luke W.     APS-7331 Python Code Clean-up
# 09/27/2018  Tim B.      BVH-5    Remove junit non-standard library and replace with
#                                  junitparser.
# 10/12/2018  Tim B.      BVH-0016 Only add elapsed time to junit if time was recorded
# 03/11/2019  Luke W.     BVH-0022 Removed default of Ford DAT 2 for the project
###############################################################################
