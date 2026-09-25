"""
This module contains tools to interact with HSM cmm scripts
used for flashing and verifying with Trace32.
"""

from pathlib import Path
from typing import TypeVar
from dataclasses import dataclass
import inspect
import time
import logging

logger = logging.getLogger(__name__)

Trace32 = TypeVar('Trace32')

THIS_DIR = Path(__file__).parent

NUM_FLASHING_FILES = 6
NUM_SOURCE_DIRS = 4
HSM_LOAD_TIMEOUT_S = 5

INI_OPTIONS_MAPPING = {
    'force_single_core_debug': 'Force_Single_Core',
    'force_multi_core_debug': 'Force_Multi_Core',
    'load_default_windows': 'Load_Default_Windows',
    'on_chip_MCDS_off': 'McdsOff',
    'UCBs_unlock': 'DebugUCBunlock',
    'use_jtag_pw': 'UseJtagPassword',
}


@dataclass(kw_only=True)
class FlashingFileSpecification:
    """
    Class to specify a selected file in the HSM script.

    Attributes:
        C: Whether the Code checkbox is selected.
        S: Whether the Source checkbox is selected.
        V: Whether the Verify checkbox is selected.
        file: The path to the selected file.
    """
    C: bool = False
    S: bool = False
    V: bool = False
    file: str = ''

    def __post_init__(self) -> None:
        """
        Post-init method for FlashingFileSpecification.
        """
        if self.file:
            file_path = Path(self.file).absolute()
            if not file_path.exists():
                logger.error(f'Specified flashing file does not exist: {file_path}')
                raise FileNotFoundError(f'Specified flashing file does not exist: {file_path}')
            self.file = str(file_path)

        self._temp_C = False
        self._temp_S = False
        self._temp_V = False

    def disable(self) -> None:
        """
        Method to disable a selected file.
        """
        self._temp_C = self.C
        self._temp_S = self.S
        self._temp_V = self.V

        self.C = False
        self.S = False
        self.V = False

    def enable(self) -> None:
        """
        Method to enable a selected file.
        """
        self.C = self._temp_C
        self.S = self._temp_S
        self.V = self._temp_V

        self._temp_C = False
        self._temp_S = False
        self._temp_V = False


@dataclass(kw_only=True)
class FlashingOptions:
    """
    Class to specify selected HSM options.
    """
    force_single_core_debug: bool = False
    force_multi_core_debug: bool = True
    load_default_windows: bool = True
    on_chip_MCDS_off: bool = False
    UCBs_unlock: bool = False
    use_jtag_pw: bool = False

    def attrs(self) -> set[str]:
        """
        Method to get all the available options that can be specified.

        Returns:
            A set with all the available options.
        """
        return {attr_name for attr_name, attr in inspect.getmembers(self)
                if not attr_name.startswith('__') and not inspect.ismethod(attr)}


class HSMWrapper:
    """
    Class to interact with an HSM cmm script.

    Attributes:
        t32: Trace32 object to interact with Trace32.
        _open: Whether the HSM script is open.
        _hsm_enabled: Whether HSM is enabled.
        _dir: The path to the directory containing the HSM_TC39x.cmm script.
        _init_path: The path to the ini file used to configure the HSM script.
        _cmm: The path to the HSM_TC39x.cmm script.
        _flashing_files: The selected files for flashing/verification.
        _source_code_dirs: The paths to the directories that contain the application source code (.c and .h files).
        _options: The selected options for the HSM script.
    """
    def __init__(
            self,
            t32: Trace32,
            cmm_dir: str = '',
            flashing_files: list[dict] = [],
            source_code_dirs: list[str] = [],
            options: dict[str, bool] = {}
            ) -> None:
        """
        Init method for HSMWrapper.

        Args:
            t32: Trace32 object to interact with Trace32.
            cmm_dir: The path to the directory containing the HSM_TC39x.cmm script.
            flashing_files: A list with specified flashing files, each element should be a dictionary with the following format:
                {'C': bool, 'S': bool, 'V': bool, 'file': str}
                'C', 'S' and 'V' may be omitted, their default value is False.
            source_code_dirs: A list with paths to the directories that contain the application source code (.c and .h files).
            options: A dictionary that specifies which options are selected or not. It should have the following format:
                {option1_name: bool, option2_name: bool, ...}
                The valid option names are the same as the attributes of the FlashingOptions class.
        """
        self.t32 = t32

        self._is_open: bool = False
        self._hsm_enabled = True

        self._dir: Path
        self._ini_path: Path
        self._cmm: Path

        self._flashing_files: list[FlashingFileSpecification]
        self._source_code_dirs: list[str]
        self._options: FlashingOptions

        self._base_stack_depth = self.t32.fnc("PRACTICE.SD()")

        if not cmm_dir:
            cmm_dir = THIS_DIR / 'cmm_scripts/CADM_LO'

        self.set_cmm_dir(cmm_dir)

        self.configure(
            flashing_files=flashing_files,
            source_code_dirs=source_code_dirs,
            options=options
        )

    @property
    def cmm_dir(self) -> Path:
        """Path to the directory containing the HSM_TC39x.cmm script."""
        return self._dir

    @property
    def is_open(self) -> bool:
        return self._is_open

    def set_cmm_dir(self, cmm_dir: str) -> None:
        """
        Method to change the path of the directory containing the HSM_TC39x.cmm script.

        Args:
            cmm_dir: The new path of the directory that contains the cmm.
        """
        logger.debug(f'Setting cmm dir to {cmm_dir}...')
        self._dir = Path(cmm_dir).absolute()
        self._ini_path = self._dir / 'TC39x/ini/TC39x_flash_dialog.ini'
        self._cmm = self._dir / 'HSM_TC39x.cmm'

        if not self._dir.exists():
            logger.error(f'Specified HSM directory does not exist: {self._dir}.')
            raise FileNotFoundError(f'Specified HSM directory does not exist: {self._dir}.')
        if not self._ini_path.exists():
            logger.error(f'Specified path to ini file does not exist: {self._ini_path}.')
            raise FileNotFoundError(f'Specified path to ini file does not exist: {self._ini_path}.')
        if not self._cmm.exists():
            logger.error(f'Specified HSM cmm script does not exist: {self._cmm}.')
            raise FileNotFoundError(f'Specified HSM cmm script does not exist: {self._cmm}.')

    def configure(
            self,
            flashing_files: list[dict] = [],
            source_code_dirs: list[str] = [],
            options: dict[str, bool] = {}
            ) -> None:
        """
        Method to configure the HSM script options and update the ini file with them.

        Args:
            flashing_files: A list with specified flashing files, each element should be a dictionary with the following format:
                {'C': bool, 'S': bool, 'V': bool, 'file': str}
                'C', 'S' and 'V' may be omitted, their default value is False.
            source_code_dirs: A list with paths to the directories that contain the application source code (.c and .h files).
            options: A dictionary that specifies which options are selected or not. It should have the following format:
                {option1_name: bool, option2_name: bool, ...}
                The valid option names are the same as the attributes of the FlashingOptions class.
        """
        logger.debug('Configuring hsm wrapper...')

        if self.is_open:
            logger.error('Tried to configure hsm wrapper while HSM cmm script is currently open in Trace32.')
            raise Exception('HSM cmm script is currently open in Trace32, please close it first before changing the configuration.')

        self._flashing_files = [FlashingFileSpecification(**file_specification) for file_specification in flashing_files[:NUM_FLASHING_FILES]]
        for _ in range(NUM_FLASHING_FILES-len(self._flashing_files)):
            self._flashing_files.append(FlashingFileSpecification())

        logger.debug(f'Flashing files specified: {self._flashing_files}.')

        source_code_dirs_paths = [Path(source_code_dir).absolute() for source_code_dir in source_code_dirs[:NUM_SOURCE_DIRS]]
        for source_code_dir_path in source_code_dirs_paths:
            if not source_code_dir_path.exists():
                logger.error(f'Specified source code directory does not exist: {source_code_dir_path}.')
                raise FileNotFoundError(f'Specified source code directory does not exist: {source_code_dir_path}.')

        self._source_code_dirs = [str(source_code_dir_path) for source_code_dir_path in source_code_dirs]
        logger.debug(f'Source code directories specified: {self._source_code_dirs}.')

        self._options = FlashingOptions(**options)
        logger.debug(f'Options specified: {self._options}.')

        self.update_ini()

    def read_ini_contents(self) -> dict:
        """
        TODO
        Method to read the contents of the ini file.
        """
        ...

    def update_ini(self) -> None:
        """
        Method to update the ini that the HSM script uses to read the
        selected configuration options.
        """
        logger.debug(f'Updating ini file {self._ini_path}...')
        with self._ini_path.open('w') as ini_file:
            # Set flashing files
            for i, file_specification in enumerate(self._flashing_files, start=1):
                file_path = file_specification.file
                if file_path:
                    print(f'File_Line0x{i} {file_path}', file=ini_file)

                code = 'ON' if file_specification.C else 'OFF'
                symbol = 'ON' if file_specification.S else 'OFF'
                verify = 'ON' if file_specification.V else 'OFF'

                print(f'Code_Line0x{i} {code}', file=ini_file)
                print(f'Symbol_Line0x{i} {symbol}', file=ini_file)
                print(f'Verify_Line0x{i} {verify}', file=ini_file)

            # Set source code dirs
            for i, src_code_dir in enumerate(self._source_code_dirs, start=1):
                if src_code_dir:
                    print(f'Source_Line0x{i} ON', file=ini_file)
                    print(f'Sourcepath0x{i} {src_code_dir}', file=ini_file)
                else:
                    print(f'Source_Line0x{i} OFF', file=ini_file)

            # Set other options
            for option in self._options.attrs():
                if option in INI_OPTIONS_MAPPING:
                    state = 'ON' if getattr(self._options, option) else 'OFF'
                    print(f'{INI_OPTIONS_MAPPING[option]} {state}', file=ini_file)
                else:
                    logger.warning(f'No mapping to ini file entry found for option {option}.')

    def run_cmm(self) -> None:
        """
        Method to run the HSM script.
        """
        logger.debug('Opening hsm cmm...')

        if self.is_open:
            logger.warning('Tried to open cmm while it was already open, waiting for it to be closed before opening again...')
            self.wait_for_cmm_to_finish()

        logger.debug(f'Changing working directory in Trace32 to {self._dir}...')
        self.t32.cmd(f'CD {self._dir}')

        logger.debug(f'Opening hsm cmm {self._cmm}...')
        self._base_stack_depth = self.t32.fnc("PRACTICE.SD()")
        self.t32.run_cmm(self._cmm, timeout_s=0)

        logger.debug('Waiting for cmm to load...')
        time.sleep(HSM_LOAD_TIMEOUT_S)

        self._is_open = True

        logger.debug('Wait time for cmm to load is over.')

    def wait_for_cmm_to_finish(self, timeout_s: float = 120) -> None:
        last_message = self.t32.dbg.get_message()
        if last_message:
            logger.debug(f'Trace32 print area: {last_message}')
        start = time.time()
        while time.time() - start < timeout_s:
            if self.t32.fnc("PRACTICE.SD()") == self._base_stack_depth:
                self._is_open = False
                break
            current_message = self.t32.dbg.get_message()
            if current_message != last_message:
                last_message = current_message
                logger.debug(f'Trace32 print area: {last_message}')
            time.sleep(0.01)
        else:
            logger.error('Timeout expired waiting for HSM cmm script to finish.')
            raise TimeoutError('Timeout expired waiting for HSM cmm script to finish.')

    def select_cmm_option(self, option: str) -> None:
        """
        Method to set a macro to true and continue program execution.
        This emulates the behavior of most button clicks in the dialogs
        window of the HSM script.

        Args:
            option: The name of the macro to set to True.
        """
        logger.debug(f'Setting {option} to true and resuming cmm execution...')
        if not self.is_open:
            logger.debug('HSM was not open, opening it...')
            self.run_cmm()

        self.t32.cmd(f'&{option}=(0==0)')
        self.t32.cmd('CONTINUE')

        self.wait_for_cmm_to_finish()

    def disable_hsm(self) -> None:
        """
        Method to disable HSM. Equivalent to pressing the 'DISABLE HSM' button
        in the dialog window.
        """
        self.select_cmm_option('disable_hsm')
        self.t32.print('SQTS - disable_hsm finished.')
        self._hsm_enabled = False

    def enable_hsm(self) -> None:
        """
        Method to enable HSM. Equivalent to pressing the 'ENABLE HSM' button
        in the dialog window.
        """
        self.select_cmm_option('enable_hsm')
        self.t32.print('SQTS - enable_hsm finished.')
        self._hsm_enabled = True

    def erase_all(self) -> None:
        """
        Method to erase all. Equivalent to pressing the 'ERASE ALL' button
        in the dialog window.
        """
        self.select_cmm_option('erase_all')
        self.t32.print('SQTS - erase_all finished.')

    def prog_ucb(self) -> None:
        """
        Method to program the UCBs. Equivalent to pressing the 'PROG UCBs' button
        in the dialog window.
        """
        self.select_cmm_option('prog_ucb')
        self.t32.print('SQTS - prog_ucb finished.')

    def prog_selected(self) -> None:
        """
        Method to program the selected files. Equivalent to pressing the 'PROG SELECTED' button
        in the dialog window.
        """
        self.select_cmm_option('prog_selected')
        self.t32.print('SQTS - prog_selected finished.')

    def verify(self) -> None:
        """
        Method to verify flashing with the selected files. Equivalent to pressing the 'VERIFY' button
        in the dialog window.
        """
        self.select_cmm_option('verify')
        self.t32.print('SQTS - verify finished.')

    def cancel(self) -> None:
        """
        Method to exit the HSM script. Equivalent to pressing the 'CANCEL' button
        in the dialog window.
        """
        self.select_cmm_option('cancel')
        self.t32.print('SQTS - cancel finished.')


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 05/31/2023  ABPA       FKU-921   Initial creation
