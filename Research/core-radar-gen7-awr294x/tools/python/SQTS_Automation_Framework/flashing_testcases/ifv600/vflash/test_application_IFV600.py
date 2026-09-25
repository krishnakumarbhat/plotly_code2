import time
import logging
from pathlib import Path

THIS_DIR = Path(__file__).parent

logger = logging.getLogger(__name__)


def test_application_flashing_vflash(
        vFlash,
        CANoe_Session_Minimal,
        # Power_Session,
        sqts_config,
        execution_timestamp
        ):
    canoe = CANoe_Session_Minimal

    logger.info('Starting application flashing...')
    logger.info('Verifying vflash project path...')
    source_vflash_project = sqts_config['vFlash']['application_project']

    if not source_vflash_project:
        logger.error('Path to vflash project for application was empty.')
        raise FileNotFoundError('Path to vflash project for application needs to be specified in pyproject.toml')

    source_vflash_project_path = Path(source_vflash_project).absolute()
    destination_vflashpack_path = THIS_DIR / f'{source_vflash_project_path.stem}.vflashpack'

    reports_dir = sqts_config['Reports']['reports_dir']
    project_name = sqts_config['Reports']['project_name']
    team_name = sqts_config['Reports']['team_name']

    report_path = Path(reports_dir).absolute() / fr'{project_name}_{team_name}_Application_Flashing_Report_{source_vflash_project_path.stem}_{execution_timestamp}.txt'

    logger.info('Creating vflashpack...')
    vFlash.convert_to_vflashpack(source_vflash_project_path, destination_vflashpack_path)

    # logger.info('Starting power cycle...')

    # Power_Session.switch_off()
    # time.sleep(10)
    # Power_Session.switch_on()
    # Power_Session.set_output_voltage(13.5)

    # logger.info('Power cycle finished.')

    logger.info('Loading ADA unlock CANoe configuration...')
    canoe.load_config(
        cfg_file=sqts_config['CANoe']['ada_unlock_config_path'],
        fdx_ip_address=sqts_config['CANoe']['FDX']['ip_address'],
        fdx_port=sqts_config['CANoe']['FDX']['port'],
        fdx_transport_layer=sqts_config['CANoe']['FDX']['transport_layer']
        )

    canoe.start_measurement()

    time.sleep(10)

    logger.info('Sending unlock sequence through CANoe...')

    canoe.press_key('3')
    time.sleep(5)
    canoe.press_key('0')
    time.sleep(5)
    canoe.press_key('9')
    time.sleep(5)

    logger.info('Opening vflashpack...')
    with vFlash.open(destination_vflashpack_path) as vf_project:
        logger.info(f'Setting report path to {report_path}...')
        vf_project.activate_reporting(report_path)
        logger.info('Flashing application...')
        vf_project.flash()

    logger.info('Flashing completed successfully.')

    logger.info('Removing created vflashpack file...')
    destination_vflashpack_path.unlink()
