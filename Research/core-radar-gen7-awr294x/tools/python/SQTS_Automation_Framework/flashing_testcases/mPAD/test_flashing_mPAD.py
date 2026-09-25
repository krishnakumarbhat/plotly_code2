import pytest
import time
import logging
from pathlib import Path

logger = logging.getLogger(__name__)


@pytest.fixture(scope="session", autouse=True)
def dummy_fixture(CANoe_Session_Global):
    """
    Fixture used to ensure that CANoe will launch before Trace32
    """
    CANoe_api = CANoe_Session_Global
    CANoe_api.start_exe()
    CANoe_api.start_measurement()
    time.sleep(10)


@pytest.mark.description("Test case to Flash mPAD and if requested to check for resets for 5 minutes")
def test_application_flashing(Trace32_Session_Minimal, sqts_config, request):
    if 'power_check' in sqts_config['flags']:
        logger.info('Checking that power supply is ON and has the correct voltage...')
        Power_Session = request.getfixturevalue('Power_Session')

        Power_Session.switch_on()
        time.sleep(1)

        assert Power_Session.get_status().output_status == 'Output ON', 'Power supply could not be turned on'
        logger.info('Power supply is ON')

        expected_voltage = sqts_config['flags']['power_check']
        if expected_voltage == '':
            expected_voltage = 12
        else:
            expected_voltage = float(expected_voltage)

        # Loop that will try to set the voltage to the expected voltage three times
        for _ in range(3):
            Power_Session.set_output_voltage(expected_voltage)
            time.sleep(5)
            current_voltage = Power_Session.get_output_voltage()
            current_current = Power_Session.get_output_current()
            logger.info(f'Power supply voltage is: {current_voltage}V')
            logger.info(f'Power supply drawn current is: {current_current}A')
            if abs(current_voltage - expected_voltage) <= 0.1:
                break
            else:
                logger.info('Power Supply voltage is not the expected one, retrying...')
        else:
            raise Exception(f'After three attempts the voltage has not been set to {expected_voltage}')

        logger.info('Power supply voltage is the one expected.')

    t32_api = Trace32_Session_Minimal
    t32_config = sqts_config['Trace32']
    t32_setup = t32_config['setup']

    cmm = t32_setup['cmm'][0]

    specified_path = cmm['path']
    if not specified_path:
        raise FileNotFoundError("Please specify the path of the start.cmm file")

    cmm_path = Path(specified_path).absolute()

    if not Path(cmm_path).exists():
        raise FileNotFoundError('Specified cmm file does not exist.')

    logger.info('Starting Flashing procedure...')
    cmm_dir = cmm_path.parent
    t32_api.cmd(f'CD "{cmm_dir}"')

    t32_api.run_cmm(cmm_path)
    reset_behavior = 'B:: SYStem.Option.RESetBehavior RunRestore'
    Trace32_Session_Minimal.cmd(reset_behavior)
    time.sleep(5)
    t32_api.in_target_reset()
    time.sleep(5)
    t32_api.power_on()
    time.sleep(10)
    logger.info('Flashing has been completed')

    if 'power_check' in sqts_config['flags']:
        current_current = Power_Session.get_output_current()
        logger.info(f'Current drawn after flashing is: {current_current}A.')

    try:
        cmd_sts = t32_api.read_var('SwcPwrMgr_CmdIgnSts')
        if cmd_sts != 4:
            logger.warning(f'SwcPwrMgr_CmdIgnSts is not 4 (Run), it is {cmd_sts}')
        else:
            logger.info('SwcPwrMgr_CmdIgnSts is 4 and flashing process was successful')
    except Trace32_Session_Minimal.exceptions.VariableError:
        logger.warning('Unable to read the value of SwcPwrMgr_CmdIgnSts variable')

    if 'verify_reset' in sqts_config['flags']:
        logger.info('Checking for reset')
        timer_10sec = 0
        total_time = 0
        elapsed = 0
        no_reset_cnt_past = 0
        sample_period = 0.02
        while total_time < 300:
            start = time.time()
            total_time = total_time + elapsed
            timer_10sec = timer_10sec + elapsed
            if timer_10sec > sample_period:
                timer_10sec = 0
                sample_period = sample_period + 0.02
                if sample_period > 10:
                    sample_period = 10
                no_reset_cnt = t32_api.variable.read('SwcPwrMgr_5msCnt').value
                if no_reset_cnt <= no_reset_cnt_past:
                    raise Exception(f"A Reset has been detected at approximately {total_time:.3f} seconds")
                else:
                    no_reset_cnt_past = no_reset_cnt
            elapsed = time.time() - start

        logger.info('No resets have been detected during the test')
