from pathlib import Path
from typing import Literal
import pytest
import argparse
import sys
import os
import tomlkit
from enum import IntEnum

from utils import file_time

# Get timestamp
TESTURN_TIMESTAMP = file_time()

# Set environment variable to access the TESTURN_TIMESTAMP variable in conftest
os.environ['SQTS_EXECUTION_TIMESTAMP'] = TESTURN_TIMESTAMP

PYPROJECT_PATH = Path(r'.\pyproject.toml')
TEMP_CONFIG_PATH = Path(r'.\logs\temp_config.toml')


class ExitCode(IntEnum):
    """
    Exit return codes for the execution of the test cases.
    """
    PYTEST_OK = 0
    PYTEST_TESTS_FAILED = 1
    PYTEST_INTERRUPTED = 2
    PYTEST_INTERNAL_ERROR = 3
    PYTEST_USAGE_ERROR = 4
    PYTEST_NO_TESTS_COLLECTED = 5
    PYTHON_EXCEPTION = 6


def get_cli_arguments() -> argparse.Namespace:
    """
    Function to parse arguments passed to the script.

    Returns:
        A namespace whose attributes are the parsed contents
        of the passed arguments.
    """
    cli_args_parser = argparse.ArgumentParser(
        prog='SQTS Automation Framework',
        description='Run the test cases and generate the reports.'
    )

    cli_args_parser.add_argument(
        '--project_name',
        metavar='PROJECT_NAME',
        type=str,
        action='store',
        default=None,
        help='The name of the project. If not provided, the value in the [Reports] section in pyproject.toml will be used.'
    )

    cli_args_parser.add_argument(
        '--team_name',
        metavar='TEAM_NAME',
        type=str,
        action='store',
        default=None,
        help='The name of the team running the test cases. If not provided, the value in the [Reports] section in pyproject.toml will be used.'
    )

    cli_args_parser.add_argument(
        '-b',
        '--bench_attribute',
        metavar=('BENCH_ATTRIBUTE', 'ATTRIBUTE_VALUE'),
        type=str,
        nargs=2,
        default=[],
        action='append',
        help='Attribute of the bench. Equivalent to adding an entry to the [Bench] section in pyproject.toml.'
    )

    cli_args_parser.add_argument(
        '--ci_cd_environment',
        action='store_true',
        help='Use this flag if the test cases are being run from a CI/CD environment. It avoids asking for user input to continue execution.'
    )

    cli_args_parser.add_argument(
        '--no_reports',
        action='store_true',
        help='Use this flag to not generate any kind of report for the executed test cases.'
    )

    cli_args_parser.add_argument(
        '--flag',
        type=str,
        nargs='+',
        default=[],
        action='append',
        help='Add any optional needed flags to pyproject.toml'
    )

    execution_commands = cli_args_parser.add_mutually_exclusive_group()

    execution_commands.add_argument(
        '--tests_dirs',
        metavar='PATH_TO_TESTS_DIR',
        type=str,
        nargs='+',
        action='store',
        default=[],
        help='Paths to look in for test cases. This argument adds paths to the testpaths entry in the [tool.pytest.ini_options] section in pyproject.toml.'
    )

    execution_commands.add_argument(
        '--flash_bootloader',
        action='store_true',
        help="If this argument is passed, only the test case to flash bootloader will be executed."
    )

    execution_commands.add_argument(
        '--flash_application',
        action='store_true',
        help="If this argument is passed, only the test case to flash bootloader will be executed."
    )

    return cli_args_parser.parse_args()


def undo_previous_pyproject_changes():
    """
    Function to check if temp_config.toml is present
    and undo changes in pyproject.toml if it is.
    """
    # Check that a previous run was not interrupted
    if TEMP_CONFIG_PATH.exists():
        print('Found temp_config.toml file, a previous run must have been interrupted.')
        if TEMP_CONFIG_PATH.stat().st_size > 0:
            print('Reverting pyproject.toml to state before last run...\n')
            PYPROJECT_PATH.unlink()
            TEMP_CONFIG_PATH.rename(PYPROJECT_PATH)
        else:
            print('temp_config.toml was empty, removing it...')
            TEMP_CONFIG_PATH.unlink()


def undo_current_pyproject_changes() -> None:
    """
    Function to undo the changes done to pyproject.toml for this run.
    """
    # Revert changes done to the configuration
    if TEMP_CONFIG_PATH.exists():
        with PYPROJECT_PATH.open('wt', encoding='utf-8') as fp, TEMP_CONFIG_PATH.open('rt', encoding='utf-8') as temp:
            sqts_config = tomlkit.load(temp)
            tomlkit.dump(sqts_config, fp)

        TEMP_CONFIG_PATH.unlink(missing_ok=True)


def get_report_format_arguments(
        sqts_config: dict,
        execution_type: Literal['Test', 'Bootloader_Flashing', 'Application_Flashing']
        ) -> list[str]:
    """
    Function to select which reports to generate for the test run.

    Args:
        sqts_config: A dictionary with the contents defined in pyproject.toml.

    Returns:
        A list with pytest arguments needed to generate the selected report formats.
    """
    # Create report filenames
    reports_options = sqts_config['Reports']
    reports_dir = reports_options['reports_dir']
    project_name = reports_options['project_name']
    team_name = reports_options['team_name']
    test_type = reports_options['test_type']

    report_file_name = f'{project_name}_{team_name}_{test_type}_{execution_type}_Report_{TESTURN_TIMESTAMP}'
    report_name_root = fr"{reports_dir}\{report_file_name}"

    html_filename = f"{report_name_root}.html"
    xlsx_filename = f"{report_name_root}.xlsx"
    junit_filename = f"{report_name_root}.xml"

    # Find out which report options were selected
    # Available options
    report_options_arguments = {
        'HTML': [f"--html={html_filename}", "--self-contained-html"],
        'XLSX': [f"--excelreport={xlsx_filename}"],
        'JUNIT': [f"--junitxml={junit_filename}"]
    }

    # Get selected report options from config file

    # Find out selected format options
    specified_report_options = {option.upper() for option in sqts_config['Reports']['report_formats']}

    selected_report_options = []
    for option in specified_report_options:
        if option in report_options_arguments:
            selected_report_options += report_options_arguments[option]
        else:
            print(f'Unrecognized report format: {option}')

    return selected_report_options


def modify_pyproject(args: argparse.Namespace) -> None:
    """
    Function to modify the pyproject.toml file with the given command-line arguments.

    Args:
        args: The namespace that contains the arguments information.
    """
    # Load current configuration and save it in a temporary file
    with PYPROJECT_PATH.open('rt', encoding='utf-8') as fp, TEMP_CONFIG_PATH.open('wt', encoding='utf-8') as temp:
        sqts_config = tomlkit.load(fp)
        tomlkit.dump(sqts_config, temp)

    execution_type = 'Test'

    # Add report name attributes
    reports_options = sqts_config['Reports']
    if args.project_name:
        reports_options['project_name'] = args.project_name

    if args.team_name:
        reports_options['team_name'] = args.team_name

    # Add bench attributes
    bench_config = sqts_config['Bench']
    for key, value in args.bench_attribute:
        bench_config[key] = value

    # Add optional flags for specific functions
    flags = {}
    for flag in args.flag:
        flag_name, *flag_params = flag
        if len(flag_params) == 0:
            flags[flag_name] = ''
        elif len(flag_params) == 1:
            flags[flag_name] = flag_params[0]
        else:
            flags[flag_name] = flag_params

    sqts_config['flags'] = flags

    # Make changes to the configuration
    ini_options = sqts_config['tool']['pytest']['ini_options']

    # Replace test paths if they were specified
    tests_paths = args.tests_dirs
    if tests_paths:
        ini_options['testpaths'] = tests_paths

    if args.flash_bootloader:
        bootloader_tc_path = sqts_config['flashing']['bootloader_tc_path']
        ini_options['testpaths'] = [bootloader_tc_path]
        t32_config = sqts_config['Trace32']
        tc397x_config = t32_config['TC397x']
        execution_type = 'Bootloader_Flashing'

        print('Flashing Bootloader Selected')
        print(f'Testcase for bootloader selected is {bootloader_tc_path}\n')
        print('Make sure the following entries in pyproject.toml are correct:')
        print(f'* Trace32.processor_type: {t32_config["processor_type"]}')
        print(f'* Trace32.TC397x.HSM_dir: {tc397x_config["HSM_dir"]}')
        print(f'* Trace32.TC397x.hw_variant: {tc397x_config["hw_variant"]}')
        print(f'* Trace32.TC397x.certificate_type: {tc397x_config["certificate_type"]}')
        print(f'* Trace32.TC397x.flashing_files.bootloader: {tc397x_config["flashing_files"]["bootloader"]}')
        print(f'* Trace32.TC397x.flashing_files.symbols: {tc397x_config["flashing_files"]["symbols"]}')
        print(f'* Trace32.TC397x.options: {tc397x_config["options"]}\n')

        print(r'WARNING: If the fields are NOT correct, close this CMD window and then delete SQTS_Automation_Framework\logs\temp_config.toml BEFORE making changes to pyproject.toml. '
              'Otherwise the changes will be undone the next time you run this BAT script.', end='\n\n')

    if args.flash_application:
        application_tc_path = sqts_config['flashing']['application_tc_path']
        ini_options['testpaths'] = [application_tc_path]
        t32_config = sqts_config['Trace32']
        tc397x_config = t32_config['TC397x']
        vflash_config = sqts_config['vFlash']
        execution_type = 'Application_Flashing'
        t32_setup = t32_config['setup']
        CANoe_config = sqts_config['CANoe']

        print('Flashing application selected')
        print(f'Testcase for application selected is {application_tc_path}\n')
        print('Make sure the following entries in pyproject.toml are correct:\n')

        print('For flashing IFV600 with Trace32:')
        print(f'* Trace32.processor_type: {t32_config["processor_type"]}')
        print(f'* Trace32.TC397x.flashing_files.application: {tc397x_config["flashing_files"]["application"]}')
        print(f'* Trace32.TC397x.flashing_files.symbols: {tc397x_config["flashing_files"]["symbols"]}\n')

        print('For flashing with vFlash:')
        print(f'* vFlash.application_project: {vflash_config["application_project"]}\n')

        print('For flashing mPAD with Trace32:')
        print(f'* Trace32.setup.cmm[0].path: {t32_setup["cmm"][0]}')
        print(f'* CANoe.config_path: {CANoe_config["config_path"]}')

        print(r'WARNING: If the fields are NOT correct, close this CMD window and then delete SQTS_Automation_Framework\logs\temp_config.toml BEFORE making changes to pyproject.toml. '
              'Otherwise the changes will be undone the next time you run this BAT script.', end='\n\n')

    # Show confirmation prompt if not disabled in pyproject.toml
    if (args.flash_bootloader or args.flash_application) and not args.ci_cd_environment:
        input('Please press ENTER to proceed with flashing')

    # Add arguments to generate the selected reports
    if not args.no_reports:
        ini_options['addopts'] += get_report_format_arguments(sqts_config, execution_type)

    # Save changes to the configuration
    with PYPROJECT_PATH.open('wt', encoding='utf-8') as fp:
        tomlkit.dump(sqts_config, fp)


def run_test_cases(args: argparse.Namespace) -> ExitCode:
    """
    Main function that runs pytest.
    """
    # Undo previous changes in the configuration in case the testrun was interrupted
    undo_previous_pyproject_changes()

    # Modify configuration with information from arguments
    modify_pyproject(args)

    print("......................starting test.............................")
    try:
        sys.argv = sys.argv[:1]  # Consume arguments so that they don't reach pytest directly
        pytest_retcode = pytest.main()
        retcode = ExitCode(pytest_retcode)
    except Exception as exc:
        retcode = ExitCode.PYTHON_EXCEPTION
        print("Test Case Failed, Stopping Tests")
        print(f'Exception occured: {exc}', file=sys.stderr)

    print(retcode.name)
    print("........................completed...............................")

    undo_current_pyproject_changes()

    return retcode


if __name__ == '__main__':
    # Remove autogenerated types library for COM interface with CANoe

    # Get passed arguments
    args = get_cli_arguments()

    try:
        retcode = run_test_cases(args)
    except Exception as exc:
        print(f'Exception occured: {exc}', file=sys.stderr)
        retcode = ExitCode.PYTHON_EXCEPTION
    finally:
        # if not args.ci_cd_environment:
            # input('Press ENTER to close the CMD window...')
        sys.exit(retcode)
