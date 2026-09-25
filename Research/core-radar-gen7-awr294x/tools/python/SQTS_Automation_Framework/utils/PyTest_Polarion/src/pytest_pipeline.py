from typing import Optional, Iterable
import logging
import argparse
from getpass import getpass
import toml
import sys
import re
from polarion import polarion
from polarion.record import Record
from polarion.testrun import Testrun
from bs4 import BeautifulSoup

logging.basicConfig(level=logging.INFO, format='%(levelname)s - %(message)s')
logger = logging.getLogger(__name__)
ResultType = Record.ResultType


def extract_sit_info(file_path):
    """
        Function to extract information from SIT style HTML reports

        Args:
           file_path: The path to the html report file.

        Ret: A dictionary containing the necessary information for updating test results, Keys are work item IDs.
    """
    info = {}

    HTMLFile = open(file_path, "r")
    index = HTMLFile.read()
    Parse = BeautifulSoup(index, 'html.parser')
    header = Parse.find("h2")
    idd = re.sub("Test Cases: ", "", header.text)
    result = Parse.find_all("p")
    for r in result:
        if "Overall Result" in r.text:
            info[idd] = {"name": idd,"result": r.text.split()[-1]}
            break
    return info


def extract_sqts_info(file_path):
    """
        Function to extract information from SQTS style HTML reports

        Args:
           file_path: The path to the html report file.

        Ret: A dictionary containing the necessary information for updating test results, Keys are work item IDs.
    """
    info = {}

    HTMLFile = open(file_path, "r")
    index = HTMLFile.read()
    Parse = BeautifulSoup(index, 'html.parser')
    ele = Parse.find("table",{"id":"results-table"})
    tmp = ele.find_all("tr")
    for vals in tmp:
        no_class = vals.find_all("td", {"class": ""})
        if len(no_class) == 0:
            continue
        name = vals.find("td",{"class":"col-name"})
        wid = no_class[0]
        res = vals.find("td",{"class":"col-result"})
        to_add = {}
        if name:
            to_add["name"] = name.text
        if res:
            to_add["result"] = res.text
        if to_add != {}:
            info[wid.text] = to_add
    return info


def get_config(config_filename: Optional[str]) -> dict[str, str]:
    """
    Function to get the configuration for the script.

    Args:
       config_filename: The path to the toml config file.

    Ret: A dictionary with the configurarion for the script.
    """
    config_filename = 'Polarion_Config.toml' if config_filename is None else config_filename
    try:
        with open(config_filename, 'r') as f:
            config = toml.load(f)
    except Exception as e:
        logger.error(f'Error trying to open configuration file {config_filename}: {e}')
        sys.exit(1)

    return config


def get_testrun_available_name_polarion(project: polarion.Project, basename: str) -> str:
    """
    Function to query the existing testrun names using basename and find one that is available.

    Args:
       project: The polarion project object to query the testrun names.
       basename: The base name to use when querying for used names and for creating a new one.

    Ret: An available testrun name.
    """
    config = get_config(None)
    prefix = config['polarion']['testrun_prefix']
    testrun_name = prefix + basename
    # If no testrun with that name exists, return that name
    if not project.searchTestRuns(testrun_name):
        return testrun_name

    # If it exists, keep trying with different numbers until one is available.
    i = 1
    new_testrun_name = f'{testrun_name}_{i}'
    while project.searchTestRuns(new_testrun_name):
        i += 1
        new_testrun_name = f'{testrun_name}_{i}'
    return new_testrun_name


def create_testrun_polarion(project: polarion.Project, wi_ids: Iterable[str], testrun_name: str) -> Testrun:
    """
    Function to create a new testrun in polarion that includes all the specified workitems.

    Args:
       project: The polarion project object to create the testrun.
       wi_ids: The IDs of the workitems to include in the testrun.
       testrun_name: The basename of the testrun to create.

    Ret: The created testrun.
    """
    config = get_config(None)
    template = config['polarion']['testrun_template']
    testrun_name = get_testrun_available_name_polarion(project, testrun_name)
    testrun = project.createTestRun(testrun_name, testrun_name, template)
    for wi_id in wi_ids:
        print(wi_id)
        test_case = project.getWorkitem(wi_id)
        testrun.addTestcase(test_case)
    logger.info(f'Testrun {testrun.title} created.')
    return testrun


def update_record_polarion(record: Record, test_result):
    """
    Function to update the status of a polarion test record.

    Args:
       record: The polarion record to update.
       test_result: The test result that the record will be updated with.

    Ret: None
    """
    config = get_config(None)
    name = test_result['name']
    result = test_result['result']
    logger.info(f'Updating record {record} for test {name}, with result {result}')
    comment = config['polarion']['record_comment']

    if "pass" in result.lower():
        record.setResult(ResultType.PASSED, comment=comment)
    else:
        record.setResult(ResultType.FAILED, comment=comment)


def update_testrun_polarion(project, test_results, testrun_name, create, attachment):
    """
    Function to update the records of a testrun in polarion using the extracted test results.

    Args:
       project: polarion project object to create the testrun
       test_results: The results of the testing in a dict where keys are the work ID and the values are the information
       testrun_name: Name of the testrun to be grabbed/created
       create: Boolean of whether or not to create a new testrun
       attachment: Report to attach to the testrun

    Ret: None
    """
    logger.info(f'Updating testrun: {testrun_name}')
    wi_ids = test_results.keys()
    if create:
        testrun = create_testrun_polarion(project, wi_ids, testrun_name)
    else:
        testrun = project.searchTestRuns(testrun_name)[0]
    print(testrun)
    if testrun.status.id != 'open':
        testrun.status.id = 'open'
        testrun.save()
    for record in testrun.records:
        if record.testcase_id in test_results:
            test_result = test_results[record.testcase_id]
            update_record_polarion(record, test_result)

    if testrun.hasAttachment():
        try:
            testrun.deleteAttachment(attachment.split('\\')[-1])
        except Exception as e:
            print("No attachment to delete")
    testrun.addAttachment(attachment, attachment.split('\\')[-1])

    if any("fail" in test_result['result'].lower() for test_result in test_results.values()):
        logger.info(f'Testrun {testrun.title} failed')
        testrun.status.id = 'failed'
    else:
        logger.info(f'Testrun {testrun.title} passed')
        testrun.status.id = 'passed'

    testrun.save()


def get_polarion_project(server: str, project_name: str) -> polarion.Project:
    """
    Function to get a polarion project object to interact with the server through the script.

    Args:
       server: The name of the server, eg.'https://polariondev1.aptiv.com/polarion/'
       project_name: The name of the project.

    Ret: The polarion project object.
    """
    while True:
        # Ask for login information
        print("Please enter your polarion credentials to update the testrun with the extracted results")
        user = input('Polarion user: ')
        password = getpass('Polarion password: ')
        try:
            pol = polarion.Polarion(server, user, password)
            project = pol.getProject(project_name)
            print()
            return project
        except Exception as e:
            print(f"Error trying to login: {e}\n")


def main(config: dict[str, str]):
    """
    Main function of the script to read the test execution results from a file and then update the corresponding testrun using
    the extracted information.

    Args:
       config: A dictionary with the script configuration.

    Ret: None
    """
    results_filename = config['results_filename']
    if config['report_type'] == "SQTS" or config['report_type'] == "SQT":
        test_results = extract_sqts_info(results_filename)
    else:
        test_results = extract_sit_info(results_filename)
    print('Extracted test results:')
    for result in test_results:
        print(result)
    print()

    testrun_name = config['testrun_name']

    server = config['polarion']['server']
    project_name = config['polarion']['project']

    project = get_polarion_project(server, project_name)

    update_testrun_polarion(project, test_results, testrun_name, config['create_tr'], results_filename)

def command_line_application():
    """
    Function to run a command line application.

    Args:
       None
    Ret: None
    """
    cli_args_parser = argparse.ArgumentParser(prog='update_testrun_results',
                                              description='Find the status of the executed test cases and update the testruns that contain them all.')

    cli_args_parser.add_argument(
        'ResultsFilename',
        metavar='results_filename',
        nargs='?',
        type=str,
        default=None,
        help='the path to the results txt file'
    )
    cli_args_parser.add_argument(
        'TestRunName',
        metavar='testrun_name',
        nargs='?',
        type=str,
        default=None,
        help='The Testrun that either exists or needs to be created'
    )
    cli_args_parser.add_argument(
        'ReportType',
        metavar="report_type",
        nargs='?',
        type=str,
        default='SQTS',
        help='Type of report that is being built (SQTS or SIT)'
    )

    cli_args_parser.add_argument(
        '-t',
        action='store_true',
        help='If included, this indicates to create a new testrun. If excluded, the script will search '
             'for a previously created testrun of the same name'
    )

    args = cli_args_parser.parse_args()

    config = get_config(args.config)
    if args.ResultsFilename:
        config['results_filename'] = args.ResultsFilename
    if args.TestRunName:
        config['testrun_name'] = args.TestRunName
    config['report_type'] = args.ReportType
    config['create_tr'] = args.t

    try:
        main(config)
    except Exception as e:
        logger.error(f'Unexpected error occured running the program:\n', exc_info=True)
        sys.exit(1)


if __name__ == '__main__':
    command_line_application()
