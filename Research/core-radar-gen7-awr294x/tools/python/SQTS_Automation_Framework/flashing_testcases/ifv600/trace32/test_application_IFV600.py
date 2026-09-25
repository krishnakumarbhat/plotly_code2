import logging
import time

logger = logging.getLogger(__name__)


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
    RunState = t32_api.types.RunState

    t32_config = sqts_config['Trace32']
    tc397x_config = t32_config['TC397x']
    flashing_files_paths = tc397x_config['flashing_files']
    application_path = flashing_files_paths['application']

    if not application_path:
        raise Exception('Application file must be specified in pyproject.toml')

    # Select source code file
    symbols_path = flashing_files_paths['symbols']
    if not symbols_path:
        raise Exception('Symbols path must be specified in pyproject.toml')

    flashing_files = [
        {'C': True, 'V': True, 'file': application_path},
        {'S': True, 'file': symbols_path}
        ]

    processor_type = t32_config['processor_type']
    logger.info(f'Setting processor type to {processor_type}...')
    t32_api.set_processor_type(processor_type)

    cmm_dir = tc397x_config['HSM_dir']

    logger.info('Specifying type of HSM wrapper to use...')
    t32_api.set_hsm(project='IFV600')

    logger.info(f'Setting cmm dir to {cmm_dir}...')
    t32_api.tc397x.set_cmm_dir(cmm_dir)

    t32_api.tc397x.configure(
        flashing_files=flashing_files,
        source_code_dirs=[],
        options=tc397x_config['options']
    )

    t32_api.tc397x.run_cmm()

    state = t32_api.get_run_state()
    if state == RunState.RUNNING:
        t32_api.dbg.break_()

    # flashing steps
    t32_api.tc397x.prog_selected()
    time.sleep(5)

    logger.info('Verifying...')
    t32_api.tc397x.verify()
    time.sleep(5)

    logger.info('Executing in-target reset...')
    t32_api.in_target_reset()
    time.sleep(5)

    logger.info('Starting up the ECU...')
    t32_api.power_on()
    time.sleep(10)

    if 'power_check' in sqts_config['flags']:
        current_current = Power_Session.get_output_current()
        logger.info(f'Current drawn after flashing is: {current_current}A.')

    logger.info('Adding variable to watch window')
    t32_api.add_var_watch('CMDIGNSTS_RUN')

    time.sleep(5)
    logger.info('Reading the value of variable')

    try:
        cmd_sts = t32_api.read_var('CMDIGNSTS_RUN')
        if cmd_sts != 4:
            logger.warning(f'CMDIGNSTS_RUN is not 4 (Run), it is {cmd_sts}')
        else:
            logger.info('CMDIGNSTS_RUN is 4 and flashing process was successful')
    except Trace32_Session_Minimal.exceptions.VariableError:
        logger.warning('Unable to read the value of CMDIGNSTS_RUN variable')
