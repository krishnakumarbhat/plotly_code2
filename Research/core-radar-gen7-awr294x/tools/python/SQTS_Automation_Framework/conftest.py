"""Define the tools that need initialized for testing."""

from py.xml import html
import pytest
import time
import subprocess
import string
import xml.etree.ElementTree as ET
import os
import re
import tomli
import logging
import dlipower
import mss
import matplotlib.pyplot as plt
import numpy as np
from src import Trace32, VectorExe, PowerCtrl, VFlash, ReportMaker, com_sleep
from typing import Literal
from dataclasses import dataclass
from pathlib import Path
from datetime import datetime

logging.getLogger('matplotlib').setLevel(logging.WARNING)
logging.getLogger('PIL').setLevel(logging.WARNING)


logging.getLogger('').setLevel(logging.WARNING)

logger = logging.getLogger(__name__)
test_logger = logging.getLogger("Testcases")

TIME_STR = os.environ['SQTS_EXECUTION_TIMESTAMP']

# Load the configuration
with Path('pyproject.toml').open('rb') as fp:
    g_sqts_config = tomli.load(fp)


@pytest.fixture(scope='session')
def execution_timestamp():
    """
    Get the execution timestamp of the testrun.
    """
    return TIME_STR


@pytest.fixture(scope='session')
def sqts_config():
    """
    Fixture to get access to the contents of the pyproject.toml file.
    """
    logger.debug('Getting config')
    return g_sqts_config


@pytest.fixture(scope='session')
def config():
    """
    Fixture to get access to the contents of the config.xml file.

    WARNING: Deprecated fixture, please use sqts_config instead.
    """
    def _config():
        ...

    setattr(_config, 'xml_tree', ET.parse('config.xml'))      # noqa: B010
    setattr(_config, 'xml_root', _config.xml_tree.getroot())  # noqa: B010
    for child in _config.xml_root:
        setattr(_config, child.tag, child.text)
    return _config


@pytest.fixture(scope='function')
def Report(request):
    """
    Fixture to add formatted information to the reports.
    """
    reporter = ReportMaker(test_logger)
    request.node.sqts_reporter = reporter
    return reporter


@pytest.fixture(scope='session')
def utils():
    """
    Fixture that exposes some utility functions for the test cases.
    """
    class UtilityFunctions:
        """Dummy class to store attributes in."""

        sleep = com_sleep

    return UtilityFunctions


@pytest.fixture(scope='session')
def Trace32_Session_Minimal(sqts_config, request):
    """Fixture for a minimal T32 config.

    Fixture for minimal setup and teardown of Trace32, it just opens Trace32 and connects
    to it with the RCL interface, and once it's no longer being used it closes Trace32.
    """
    eth_switch_config = sqts_config['EthSwitch']
    dbg_port_name = eth_switch_config['debug_port_name']

    if dbg_port_name:
        Ethernet_Switch = request.getfixturevalue('Ethernet_Switch')

        port = Ethernet_Switch.determine_outlet(dbg_port_name) - 1
        if Ethernet_Switch[port].off():
            logger.error(f'Failed to turn off {dbg_port_name}')
            raise Exception(f'Not able to turn off {dbg_port_name}')
        else:
            logger.info(f'Successfully turned off {dbg_port_name}')

        time.sleep(5)

        if Ethernet_Switch[port].on():
            logger.error(f'Failed to turn on {dbg_port_name}')
            raise Exception(f'Not able to turn on {dbg_port_name}')
        else:
            logger.info(f'Successfully turned on {dbg_port_name}')

        time.sleep(5)

    logger.debug('Trace32_Session_Minimal fixture requested, starting setup...')
    config = sqts_config['Trace32']['connection'][0]

    # Code to run before the first test case that requests this fixture
    executable = config['executable']
    t32api = Trace32(executable)

    # First close any Trace32 windows
    try:
        subprocess.check_call([f"taskkill /im {Path(executable).name}"], stdout=subprocess.DEVNULL)
        time.sleep(3)
    except Exception:
        pass

    try:
        t32api.start_exe(
            t32_config=config['t32_config'],
            t32_config_arguments=config['t32_config_arguments']
        )
        t32api.connect(
            connection_port=config['port'],
            connection_protocol=config['protocol']
        )

    except Exception as exc:
        logger.error(f'Exception occured while trying to setup Trace32: {exc}')

        t32api.exit_exe()
        raise ConnectionError('Unable to connect to Trace32') from exc

    startup_cmm = config['startup_cmm']

    if startup_cmm:
        timeout_s = config['startup_cmm_timeout_s']
        timeout_s = None if timeout_s < 0 else timeout_s
        t32api.run_cmm(Path(startup_cmm).absolute(), timeout_s=timeout_s)

    logger.debug('Setup for Trace32_Session_Minimal finished.')

    yield t32api

    # Code to run after all test cases have been executed
    logger.debug('Starting Trace32_Session_Minimal teardown...')

    t32api.exit_exe()

    logger.debug('Teardown for Trace32_Session_Minimal finished.')


@pytest.fixture(scope="session")
def Trace32_Session_Global(Trace32_Session_Minimal, sqts_config):
    """Fixture for a globla T32 config.

    Fixture to configure the opened Trace32 instance with the parameters
    specified in pyproject.toml
    """
    t32_api: Trace32 = Trace32_Session_Minimal
    logger.debug('Trace32_Session_Global requested, starting setup...')

    t32_config = sqts_config['Trace32']

    t32_api.set_processor_type(t32_config['processor_type'])

    t32_setup = t32_config['setup']

    for cmm in t32_setup['cmm']:
        specified_path = cmm['path']
        if not specified_path:
            continue

        cmm_path = Path(specified_path).absolute()
        if cmm.get('change_dir', True):
            cmm_dir = cmm_path.parent
            t32_api.cmd(f'CD "{cmm_dir}"')

        t32_api.run_cmm(cmm_path, timeout_s=60)

    if t32_setup['hsm_verify']:
        tc397x_config = t32_config['TC397x']

        t32_api.set_hsm(tc397x_dir=tc397x_config['HSM_dir'])

        flashing_files_paths = tc397x_config['flashing_files']

        flashing_files = [
            {'C': True, 'V': True, 'file': flashing_files_paths['application']},
            {'S': True, 'file': flashing_files_paths['symbols']}
        ]

        t32_api.tc397x.configure(
            flashing_files=flashing_files,
            source_code_dirs=tc397x_config['source_code_dirs'],
            options=tc397x_config['options']
        )

        t32_api.tc397x.verify()

        time.sleep(2)

    mem_accs_cmd = 'B:: SYStem.Option.DUALPORT.ON'
    t32_api.cmd(mem_accs_cmd)

    time.sleep(2)

    t32_api.power_on()
    logger.debug('Setup for Trace32_Session_Global finished.')

    yield t32_api  # What the test cases will have access to

    logger.debug('Staring teardown for Trace32_Session_Global...')

    logger.debug('Teardown for Trace32_Session_Global finished.')


@pytest.fixture(scope='function')
def Trace32_Session(Trace32_Session_Global):
    """Fixture to access Trace32.

    Setup and teardown steps used in all the test cases should go here.
    """
    t32_api: Trace32 = Trace32_Session_Global
    logger.debug('Trace32_Session fixture requested, starting setup...')

    t32_api.delete_all_breakpoints()

    logger.debug('Finished setup for Trace32_Session fixture.')

    yield t32_api

    logger.debug('Starting teardown for Trace32_Session...')

    logger.debug('Teardown for Trace32_Session finished.')


@pytest.fixture(scope='session')
def Trace32_Session_Minimal2(sqts_config):
    """Fixture for minimal setup and teardown of Trace32.

    It just opens Trace32 and connects to it with the RCL interface,
    and once it's no longer being used it closes Trace32.
    """
    logger.debug('Trace32_Session_Minimal2 fixture requested, starting setup...')
    config = sqts_config['Trace32']['connection'][1]

    # Code to run before the first test case that requests this fixture
    executable = config['executable']
    t32api = Trace32(executable)

    # First close any Trace32 windows
    try:
        subprocess.check_call([f"taskkill /im {Path(executable).name}"], stdout=subprocess.DEVNULL)
        time.sleep(3)
    except Exception:
        pass

    try:
        t32api.start_exe(
            t32_config=config['t32_config'],
            t32_config_arguments=config['t32_config_arguments']
        )
        t32api.connect(
            connection_port=config['port'],
            connection_protocol=config['protocol']
        )

    except Exception as exc:
        logger.error(f'Exception occured while trying to setup Trace32: {exc}')

        t32api.exit_exe()
        raise ConnectionError('Unable to connect to Trace32') from exc

    startup_cmm = config['startup_cmm']

    if startup_cmm:
        timeout_s = config['startup_cmm_timeout_s']
        timeout_s = None if timeout_s < 0 else timeout_s
        t32api.run_cmm(Path(startup_cmm).absolute(), timeout_s=timeout_s)

    logger.debug('Setup for Trace32_Session_Minimal2 finished.')

    yield t32api

    # Code to run after all test cases have been executed
    logger.debug('Starting Trace32_Session_Minimal2 teardown...')

    t32api.exit_exe()

    logger.debug('Teardown for Trace32_Session_Minimal2 finished.')


@pytest.fixture(scope='session')
def CANoe_Session_Minimal(sqts_config):
    """
    Fixture for minimal setup and teardown of CANoe.
    """
    logger.debug('CANoe_Session_Minimal requested, starting setup')
    canoe = VectorExe("CANoe")
    canoe.start_exe()

    test_logs_path = Path(fr'logs\canoe_logs\logs_{TIME_STR}')
    canoe._pytest_data['test_logs_path'] = test_logs_path
    test_paths = sqts_config['tool']['pytest']['ini_options']['testpaths']
    canoe._pytest_data['test_paths'] = [Path(test_path).absolute() for test_path in test_paths]

    logger.debug('Finished setup for CANoe_Session_Minimal.')

    yield canoe

    logger.debug('Starting teardown for CANoe_Session_Minimal...')

    canoe.exit_exe()

    logger.debug('Teardown for CANoe_Session_Minimal finished.')


@pytest.fixture(scope='session')
def CANoe_Session_Global(CANoe_Session_Minimal, sqts_config):
    """Canoe fixture.

    Fixture that starts CANoe, loads the configuration defined as CANoeConfPath
    in the pyproject.toml file, deletes created temporary logs,
    and starts the measurement.
    """
    canoe: VectorExe = CANoe_Session_Minimal
    logger.debug('CANoe_Session_Global requested, starting setup...')

    # configure everything
    canoe.load_config(
        cfg_file=sqts_config['CANoe']['config_path'],
        fdx_ip_address=sqts_config['CANoe']['FDX']['ip_address'],
        fdx_port=sqts_config['CANoe']['FDX']['port'],
        fdx_transport_layer=sqts_config['CANoe']['FDX']['transport_layer']
    )

    canoe.start_measurement()

    # Request data through FDX to verify that it was connected correctly
    try:
        canoe.fdx.request_data(0x8000)
    except Exception as exc:
        logger.error(f'FDX ping failed: {exc}.')
        raise ConnectionError('Error pinging CANoe through FDX, please check that FDX is properly configured.') from exc

    logger.debug('Finished setup for CANoe_Session_Global.')

    yield canoe  # What the test cases will have access to

    logger.debug('Starting teardown for CANoe_Sesssion_Global...')

    # Code to run after all test cases have been executed
    canoe.stop_measurement()

    logger.debug('Teardown for CANoe_Session_Global finished.')


@pytest.fixture(scope='function')
def CANoe_Session(CANoe_Session_Global, request):
    """Camoe Session.

    Fixture to that adds relevant data from the test case that makes use of VectorExe
    in the form of attributes that are used internally.
    """
    canoe: VectorExe = CANoe_Session_Global
    logger.debug('CANoe_Session fixture requested, starting setup...')
    # Test case name
    canoe._pytest_data['current_sqts_test_case'] = request.node.name

    # Location of the module that contains the test case
    module_path = Path(request.node.parent.fspath)
    canoe._pytest_data['current_sqts_test_module'] = (module_path.parent / module_path.stem).absolute()

    logger.debug('Finished setup for CANoe_Session.')

    yield canoe

    logger.debug('Starting teardown for CANoe_Session...')
    # Code to run after every test case that requests this fixture
    # Reset attributes
    canoe._pytest_data['current_sqts_test_case'] = None
    canoe._pytest_data['current_sqts_test_module'] = None

    logger.debug('Teardown for CANoe_Session finished.')


@pytest.fixture(scope='session')
def Ethernet_Switch(sqts_config):
    """Ethernet switch."""
    eth_switch_config = sqts_config['EthSwitch']

    switch_ip = eth_switch_config['ip']
    user_id = eth_switch_config['user_id']
    passw = eth_switch_config['password']
    switch = dlipower.PowerSwitch(hostname=switch_ip, userid=user_id, password=passw)

    if not switch.verify():
        logger.error('Either Web switch is not connected or configured properly')
        raise Exception('Not able to connect to the switch')

    logger.info('Successfully connected to the Ethernet Switch')

    return switch


@pytest.fixture(scope='session')
def Power_Session(sqts_config, request):
    """Power Supply Session.

    Fixture to control programmable power supplies.
    """
    eth_switch_config = sqts_config['EthSwitch']
    power_supply_port_name = eth_switch_config['power_supply_port_name']

    if power_supply_port_name:
        Ethernet_Switch = request.getfixturevalue('Ethernet_Switch')

        port = Ethernet_Switch.determine_outlet(power_supply_port_name) - 1
        if Ethernet_Switch[port].off():
            logger.error(f'Failed to turn off {power_supply_port_name}')
            raise Exception(f'Not able to turn off {power_supply_port_name}')
        else:
            logger.info(f'Successfully turned off {power_supply_port_name}')

        time.sleep(5)

        if Ethernet_Switch[port].on():
            logger.error(f'Failed to turn on {power_supply_port_name}')
            raise Exception(f'Not able to turn on {power_supply_port_name}')
        else:
            logger.info(f'Successfully turned on {power_supply_port_name}')

        time.sleep(5)
    power_ctrl = PowerCtrl()
    yield power_ctrl


@pytest.fixture(scope='session')
def vFlash(sqts_config):
    """Vector vflash fixture.

    Fixture to initialize the vFlash C API.
    """
    logger.info('vFlash requested.')
    executable = sqts_config['vFlash']['executable']
    if executable:
        logger.debug(f'vFlash executable explicitely specified: {executable}.')
        executable_path = Path(executable).absolute()
        if not executable_path.exists():
            logger.error(f'Specified vFlash executable does not exist: {executable_path}')
            raise FileNotFoundError(f'Specified vFlash executable does not exist: {executable_path}')
    else:
        logger.debug('vFlash executable not specified, searching for it...')
        possible_executable_paths = list(Path(r'C:\Program Files (x86)').glob('Vector vFlash ?/Bin/vFlash.exe'))
        if not possible_executable_paths:
            logger.error('Could not find executable for vFlash')
            raise FileNotFoundError('Could not find executable for vFlash')

        executable_path = possible_executable_paths[0].absolute()
        logger.debug(f'Found vFlash executable: {executable_path}.')

    dll = sqts_config['vFlash']['dll']
    if dll:
        logger.debug(f'vFlash dll explicitely specified: {dll}.')
        dll_path = Path(dll).absolute()
        if not dll_path.exists():
            logger.error(f'Specified vFlash dll does not exist: {dll}')
            raise FileNotFoundError(f'Specified vFlash dll does not exist: {dll}')
    else:
        logger.debug('vFlash dll not specified, searching for it...')
        possible_dll_paths = list(Path(r'C:\Program Files (x86)').glob('Vector vFlash ?/Bin/VFlashAutomation64.dll'))
        if not possible_dll_paths:
            logger.error('Could not find dll for vFlash')
            raise FileNotFoundError('Could not find dll for vFlash')

        dll_path = possible_dll_paths[0].absolute()
        logger.debug(f'Found vFlash dll: {dll_path}.')

    # First close vFlash if it's open
    try:
        subprocess.check_call([f"taskkill /im {executable_path.name}"], stdout=subprocess.DEVNULL)
        time.sleep(3)
    except Exception:
        pass

    vflash = VFlash(executable_path=executable_path, dll_path=dll_path)

    yield vflash

    vflash.close_api()

# -------------------- Pytest configuration  --------------------


def log_filter(record: logging.LogRecord):
    """Log filter.

    Filter function to attach status attribute to LogRecords.

    Args:
        record: The current record being filtered.
    """
    if not hasattr(record, 'status'):
        match record.levelno:
            case logging.INFO:
                record.status = 'INFO'
            case logging.WARNING:
                record.status = 'WARN'
            case logging.ERROR | logging.CRITICAL:
                record.status = 'ERROR'


def pytest_sessionstart(session):
    """Pytest session start.

    Pytest hook implementation to configure the logs
    at the start of the session.
    """
    project_name = g_sqts_config['Reports']['project_name']
    team_name = g_sqts_config['Reports']['team_name']

    logging_plugin = session.config.pluginmanager.get_plugin("logging-plugin")
    logging_plugin.set_log_path(fr'logs\debug_logs\{project_name}_{team_name}_dbg_log_{TIME_STR}.txt')
    logging_plugin.caplog_handler.addFilter(log_filter)

# -------------------- Error handling  --------------------


def pytest_exception_interact(node, call):
    """Pytest exception interact.

    Pytest hook implementation to take screenshots and log
    the traceback whenever an Exception is detected.
    """
    excinfo = call.excinfo
    logger.error(f'Exception occurred in {node.name}:\n{excinfo.getrepr()}')

    if g_sqts_config['Reports']['screenshots']:
        test_name = node.name
        ss_file_name = f'{test_name}_{TIME_STR}.png'

        folder_name = str(node.parent)
        folder_name = folder_name[8:-4].split('/')[-1].strip()
        this_dir = Path(__file__).parent.absolute()
        ss_folder = this_dir / f'logs/screenshots/{folder_name}'
        ss_folder.mkdir(parents=True, exist_ok=True)

        ss_file = ss_folder / ss_file_name
        with mss.mss() as mss_obj:
            mss_obj.shot(output=str(ss_file))
        logger.info('Screenshot taken due to an Exception or Assertion Error')


def pytest_internalerror(excinfo):
    """
    Pytest hook implementation to deal with internal errors.
    """
    logger.error(f'Internal error occured:\n{excinfo.getrepr()}')


def pytest_keyboard_interrupt(excinfo):
    """Pytest keyboard interrupt.

    Pytest hook implementation to deal with keyboard interrupts.
    """
    logger.error(f'Keyboard interruption detected:\n{excinfo.getrepr()}')

# -------------------- Reports configuration --------------------


def pytest_assertion_pass(item, orig):
    """Pytest assertion pass check.

    Pytest hook implementation to log everytime an assertion passes.
    """
    if hasattr(item, 'sqts_reporter'):
        item.sqts_reporter.info(f'Assertion passed: {orig}', extra={'status': 'PASS'})


@pytest.hookimpl(tryfirst=True)
def pytest_metadata(metadata):
    """Make metadata for pytest.

    Pytest hook implementation to add information to the Environment table in the html report.
    """
    # Remove entries
    metadata.pop('Plugins')
    metadata.pop('Packages')

    # Get environment information about the bench from the sqts_config file
    for key, value in g_sqts_config['Bench'].items():
        metadata[key] = value

    # Get login information
    metadata['User'] = os.getlogin()


def pytest_html_results_table_header(cells):
    """Make html result table header.

    Pytest hook implementation to reorder the results table header
    in the HTML report.
    """
    cells[:] = [
        html.th('Test Case Name', class_='sortable name', col='name'),
        html.th('Work Item ID', class_='sortable', col='wiid'),
        html.th('Test Description', class_='sortable', col='description'),
        html.th("Time", class_="sortable time", col="time"),
        html.th("Duration", class_="sortable dur", col="duration"),
        html.th("Results", class_="sortable result", col="results")
    ]


def pytest_html_results_table_row(report, cells):
    """Make html results table row.

    Pytest hook implementation to reorder the results table columns
    in the HTML report.
    """
    result_val, tc_name, dur = cells[:3]

    cells[:] = [
        tc_name,
        html.td(getattr(report, 'WI', 'NA')),
        html.td(getattr(report, 'description', 'NA')),
        html.td(datetime.utcnow(), class_="col-time"),
        dur,
        result_val,
    ]


def pytest_html_results_summary(prefix, summary):
    """Make html results summary.

    Pytest hook implementation to add a pie-chart in the summary section
    of the HTML report.
    """
    info_for_pie = {
        "passed": 0,
        "failed": 0,
        "skipped": 0,
        "expected failures": 0,
        "unexpected passes": 0,
        "errors": 0
    }
    for item in summary:
        if "span" in str(item):
            vals = re.sub("</span", "", str(item).split(">")[1])
            xinfo = vals.split(" ", 1)
            info_for_pie[xinfo[1]] = int(xinfo[0])
    create_pie_chart(info_for_pie)
    prefix.append(
        html.div(
            html.img(src=f'./pie_{TIME_STR}.png', style='height:250px;', display='block'),
            style='text-align:center;'
        )
    )


@pytest.hookimpl(hookwrapper=True)
def pytest_runtest_makereport(item):
    """Make pytest report.

    Pytest hook implementation to add the descriptions defined in the
    'description' marker to the test results.
    """
    outcome = yield
    report = outcome.get_result()
    setattr(report, "duration_formatter", "%H:%M:%S.%f")      # noqa: B010

    if not hasattr(report, 'description'):
        report.description = ['NA']
        if item.iter_markers(name='description'):
            for mark in item.iter_markers(name='description'):
                report.description = list(mark.args)
    if not hasattr(report, 'WI'):
        report.WI = ['NA']
        if item.iter_markers(name='WI'):
            for mark in item.iter_markers(name='WI'):
                report.WI = list(mark.args)


def create_pie_chart(values: dict[str, int]) -> None:
    """Create pie chart.

    Function to create a pie chart given the results of the test cases.

    Args:
        values: A dictionary with the frequency of results for the test cases, e.g. "passed": 4.
    """
    v = []
    colors = {
        "Passed": "green",
        "Failed": "red",
        "Skipped": "black",
        "Expected Failures": "magenta",
        "Unexpected Passes": "yellow",
        "Errors": "orange"
    }
    mycolors = []
    mylabels = []
    for value in values:
        if values[value] != 0:
            v.append(values[value])
            mylabels.append(string.capwords(value))
            mycolors.append(colors[string.capwords(value)])
    v = np.array(v)
    plt.pie(v, labels=mylabels, colors=mycolors)

    reports_dir = g_sqts_config['Reports']['reports_dir']
    plt.savefig(f'{reports_dir}/pie_{TIME_STR}.png', dpi=300, bbox_inches='tight')

# -------------------- User-defined fixtures  --------------------


@pytest.fixture(scope='session')
def T32_R5A(Trace32_Session_Minimal, T32_C66, sqts_config, Power_Session) -> Trace32:
    """T32 Instance for R5A."""
    t32 = Trace32_Session_Minimal
    files = sqts_config['Trace32']['files']
    flags = sqts_config['Trace32']['files']['flags']
    autoflash_cmm = Path(files['auto_flash_cmm']).absolute()
    pbl_hex = Path(files['pbl_hex']).absolute()
    hsm_hex = Path(files['hsm_hex']).absolute()
    pbl_elf = Path(files['pbl_elf']).absolute()
    app_hex = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['app_hex'])).absolute()
    mss_elf = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['mss_elf'])).absolute()
    dss_elf = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['dss_elf'])).absolute()
    smc_ptp = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['smc_ptp'])).absolute()
    usc_ptp = Path(str(files['Base_dir']) + str(flags['variant']) + str(files['usc_ptp'])).absolute()
    time.sleep(10)
    t32.print("Before_Reset")
    t32.cmd("SYStem.mode.Down")
    try:
        t32.cmd("SYStem.mode.Up")
    except BaseException:
        None
    t32.cmd("SYStem.mode.Attach")
    t32.cmd("Break")
    t32.print("After_Reset")
    t32.run_cmm(
        f'"{autoflash_cmm}"',
        sqts_config['Trace32']['pre_erase'],
        pbl_hex,
        hsm_hex,
        pbl_elf,
        app_hex,
        mss_elf,
        dss_elf,
        smc_ptp,
        usc_ptp,
        timeout_s=60
    )
    t32.delete_all_breakpoints()
    t32.cmd("SYStem.mode.Down")
    time.sleep(2)
    try:
        t32.cmd("SYStem.mode.Up")
    except BaseException:
        None
    time.sleep(5)
    attachAttempts = 0
    while t32.get_run_state() != 3:
        t32.cmd("SYStem.Attach")
        attachAttempts += 1
        time.sleep(10)
        t32.print(f'-------- State: {t32.get_run_state()} --------')
        if (attachAttempts > 5) :
            break
    T32_C66.cmd("SYStem.mode.NoDebug")
    time.sleep(2)
    try:
        T32_C66.cmd("SYStem.mode.Attach")
    except BaseException:
        None
    time.sleep(5)
    # Checking Radar Status
    T_1 = []
    T_1.append("RADAR_CTL_INIT_FAIL")
    T_1.append("RADAR_CTL_INIT_NOT_STARTED")
    T_1.append("RADAR_CTL_INIT_STARTED")
    T_1.append("RADAR_CTL_INIT_SUCCESS")
    t = "Radar_Ctl_Data.init_status"
    t32.add_var_watch(t)
    T = t32.read_var(t)
    print("")
    print(f"{t} = {T_1[T]}.")
    assert (T == 3), 'Radar Is not Successful'
    t32.print("End ----> Flashing and Check")
    time.sleep(10)
    yield t32


@pytest.fixture(scope='session')
def T32_C66(Trace32_Session_Minimal2) -> Trace32:
    """T32 Instance for C66."""
    t32 = Trace32_Session_Minimal2
    t32.delete_all_breakpoints()
    yield t32


@pytest.fixture(scope="session")
def Power(Power_Session, utils):
    """Power Supply Control."""
    Power_Session.switch_off()
    utils.sleep(3)
    Power_Session.switch_on()
    utils.sleep(3)
    yield Power_Session
    Power_Session.switch_off()


@dataclass
class InterfaceData:
    """Class to hold test case variable and functions."""

    provider_interface_variable: str
    provider_core: Literal['C66', 'R5A']
    receiver_interface_variable: str
    receiver_core: Literal['R5A', 'C66']
