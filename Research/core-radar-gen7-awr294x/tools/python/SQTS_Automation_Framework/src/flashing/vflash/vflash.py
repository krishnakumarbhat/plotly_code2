"""
This module contains classes to interact with the vFlash Automation C API.
"""
import ctypes
from pathlib import Path
import logging
import time
import subprocess

from .vflash_types import FlashResult, FlashStatus, FlashReportingOptions

logger = logging.getLogger(__name__)

VFlashProjectHandle = ctypes.c_void_p
ProgressFunc = ctypes.WINFUNCTYPE(None, ctypes.c_uint, ctypes.c_uint)
StatusFunc = ctypes.WINFUNCTYPE(None, ctypes.c_int)

THIS_DIR = Path(__file__).parent


class VFlashError(Exception):
    """VFlash exception"""


class VFlashAttribute:
    """
    Class to store the values of a Flash attribute.

    Attributes:
        name: The name of the flash attribute.
        configured_value: The configured value for the flash attribute.
        last_run_value: The last run value of the flash attribute.
    """
    def __init__(self, attr_name: str, configured_value: str, last_run_value: str) -> None:
        """
        Init method for VFlashAttribute class.

        Args:
            attr_name: The name of the flash attribute.
            configured_value: The configured value for the flash attribute.
            last_run_value: The last run value of the flash attribute.
        """
        self.name = attr_name
        self.configured_value = configured_value
        self.last_run_value = last_run_value


class FlashAttributesHandler:
    """
    Class to handle getting and setting Flash attributes.
    """
    def __init__(self, project: 'VFlashProject') -> None:
        """
        Init method for FlashAttributesHandler class.

        Args:
            project: The VFlash project for which the flash attributes apply.
        """
        logger.debug('Setting up Flash Attributes Handler...')
        self.__project = project

        vflash_set_flash_attribute = self.__project._dll.vFlashSetFlashAttribute
        vflash_set_flash_attribute.restype = FlashResult

        vflash_get_flash_attribute_configured_value = self.__project._dll.vFlashGetFlashAttributeConfiguredValue
        vflash_get_flash_attribute_configured_value.restype = FlashResult

        vflash_get_flash_attribute_last_run_value = self.__project._dll.vFlashGetFlashAttributeLastRunValue
        vflash_get_flash_attribute_last_run_value.restype = FlashResult

        self.__set = vflash_set_flash_attribute
        self.__get_configured = vflash_get_flash_attribute_configured_value
        self.__get_last = vflash_get_flash_attribute_last_run_value

        logger.debug('Finished setting up Flash Attributes Handler.')

    def __getitem__(self, attr_name: str) -> VFlashAttribute:
        """
        Operator overloading of getitem dunder method.

        Args:
            attr_name: The name of the flash attribute to get.

        Returns:
            A VFlashAttribute object that contains the configured value and the last run value
            of the requested flash attribute.
        """
        logger.debug(f'Trying to get flash attribute {attr_name}...')
        logger.debug(f'Getting last configured value for flash attribute {attr_name}...')

        configured_value_buffer = ctypes.c_char_p()
        result = self.__get_configured(self.__project._project_handle, attr_name.encode(), ctypes.byref(configured_value_buffer), 1024)

        if result != FlashResult.SUCCESS:
            error_msg = f'Error getting last configured value for flash attribute {attr_name}: {result.name}\n{self.__project.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug(f'Got last configured value for flash attribute {attr_name}: {result}.')

        logger.debug(f'Getting last run value for flash attribute {attr_name}...')

        last_run_value_buffer = ctypes.c_char_p()
        result = self.__get_last(self.__project._project_handle, attr_name.encode(), ctypes.byref(last_run_value_buffer), 1024)

        if result != FlashResult.SUCCESS:
            error_msg = f'Error getting last run value for flash attribute {attr_name}: {result.name}\n{self.__project.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug(f'Got last run value for flash attribute {attr_name}: {result}.')

        return VFlashAttribute(
            attr_name=attr_name,
            configured_value=configured_value_buffer.value.decode(),
            last_run_value=last_run_value_buffer.value.decode()
        )

    def __setitem__(self, attr_name: str, value: str) -> None:
        """
        Operator overloading of setitem dunder method.

        Args:
            attr_name: The flash attribute to set.
            value: The value to set the flash attribute to.
        """
        logger.debug(f'Setting value of flash attribute {attr_name} to {value}...')
        result = self.__set(self.__project._project_handle, attr_name.encode(), value.encode())
        if result != FlashResult.SUCCESS:
            error_msg = f'Error setting flash attribute {attr_name} to {value}: {result.name}\n{self.__project.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug('Value of flash attribute set successfully.')


class CustomActionAttributesHandler:
    """
    Class to handle Custom Action attributes.
    """
    def __init__(self, project: 'VFlashProject') -> None:
        """
        Init method for CustomActionAttributesHandler class.

        Args:
            project: The VFlash project for which the custom action attributes apply.
        """
        logger.debug('Setting up Custom Action Attributes Handler...')
        self.__project = project

        vflash_set_custom_action_attribute = self.__project._dll.vFlashSetCustomActionAttribute
        vflash_set_custom_action_attribute.restype = FlashResult

        vflash_get_custom_action_attribute_configured_value = self.__project._dll.vFlashGetCustomActionAttributeConfiguredValue
        vflash_get_custom_action_attribute_configured_value.restype = FlashResult

        vflash_get_custom_action_attribute_last_run_value = self.__project._dll.vFlashGetCustomActionAttributeLastRunValue
        vflash_get_custom_action_attribute_last_run_value.restype = FlashResult

        self.__set = vflash_set_custom_action_attribute
        self.__get_configured = vflash_get_custom_action_attribute_configured_value
        self.__get_last = vflash_get_custom_action_attribute_last_run_value

        logger.debug('Finished setting up Custom Action Attributes Handler.')

    def __getitem__(self, attr_name: str) -> VFlashAttribute:
        """
        Operator overloading of getitem dunder method.

        Args:
            attr_name: The name of the custom action attribute to get.

        Returns:
            A VFlashAttribute object that contains the configured value and the last run value
            of the requested custom action attribute.
        """
        logger.debug(f'Trying to get custom action attribute {attr_name}...')
        logger.debug(f'Getting last configured value for custom action attribute {attr_name}...')

        configured_value_buffer = ctypes.c_char_p()
        result = self.__get_configured(self.__project._project_handle, attr_name.encode(), ctypes.byref(configured_value_buffer), 1024)

        if result != FlashResult.SUCCESS:
            error_msg = f'Error getting last configured value for custom action attribute {attr_name}: {result.name}\n{self.__project.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug(f'Got last configured value for custom action attribute {attr_name}: {result}.')

        logger.debug(f'Getting last run value for custom action attribute {attr_name}...')

        last_run_value_buffer = ctypes.c_char_p()
        result = self.__get_last(self.__project._project_handle, attr_name.encode(), ctypes.byref(last_run_value_buffer), 1024)

        if result != FlashResult.SUCCESS:
            error_msg = f'Error getting last configured value for custom action attribute {attr_name}: {result.name}\n{self.__project.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug(f'Got last run value for custom action attribute {attr_name}: {result}.')

        return VFlashAttribute(
            attr_name=attr_name,
            configured_value=configured_value_buffer.value.decode(),
            last_run_value=last_run_value_buffer.value.decode()
        )

    def __setitem__(self, attr_name: str, value: str) -> None:
        """
        Operator overloading of setitem dunder method.

        Args:
            attr_name: The custom action attribute to set.
            value: The value to set the custom action attribute to.
        """
        logger.debug(f'Setting value of custom action attribute {attr_name} to {value}...')
        result = self.__set(self.__project._project_handle, attr_name.encode(), value.encode())
        if result != FlashResult.SUCCESS:
            error_msg = f'Error setting custom action attribute {attr_name} to {value}: {result.name}\n{self.__project.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug('Value of custom action attribute set successfully.')


class CommunicationParameterHandler:
    """
    Class to handle communication parameters.
    """
    def __init__(self, project: 'VFlashProject') -> None:
        """
        Init method for CommunicationParameterHandler class.

        Args:
            project: The VFlash project for which the Communication Parameters apply.
        """
        logger.debug('Setting up Communication Parameter Handler...')
        self.__project = project

        try:
            vflash_update_com_param = self.__project._dll.vFlashUpdateComParam
            vflash_update_com_param.restype = FlashResult
            self.__set = vflash_update_com_param
            self.__valid = True
            logger.debug('Finished setting up Communication Parameter Handler.')
        except Exception:
            logger.warning('Could not get vFlashUpdateComParam function from dll.')
            self.__valid = False

    def __setitem__(self, param_qualifier: str, value: str) -> None:
        """
        Operator overloading of setitem dunder method.

        Args:
            param_qualifier: The communication parameter to set.
            value: The value to set the communication parameter to.
        """
        logger.debug(f'Setting value of communication parameter {param_qualifier} to {value}...')
        if not self.__valid:
            logger.error('Tried updating communication parameter when function not available in dll.')
            raise VFlashError('Updating Communication Parameters not supported for this version of vFlash.')
        result = self.__set(self.__project._project_handle, param_qualifier.encode(), value.encode())
        if result != FlashResult.SUCCESS:
            error_msg = f'Error updating communication parameter {param_qualifier} to {value}: {result.name}\n{self.__project.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug(f'Value of communication parameter {param_qualifier} set successfully.')


class VFlashProject:
    """
    Class to work with a vflashpack project.

    Attributes:
        name: The name of the loaded vflashpack project.
    """

    def __init__(self, api: 'VFlash', project_handle: VFlashProjectHandle, project_name: str) -> None:
        """
        Init method for VFlashProject class.

        Args:
            dll: The DLL object to communicate with the C API.
            project_handle: The handle used by the API to work with the loaded project.
            project_name: The name of the loaded vflashpack project.
        """
        logger.debug(f'Setting up vFlash project {project_name}...')
        self.__project_handle = project_handle
        self.__api = api
        self.__dll = api._dll
        self._project_name = project_name
        self.__flash_attribute = FlashAttributesHandler(self)
        self.__custom_action_attribute = CustomActionAttributesHandler(self)
        self.__communication_parameter = CommunicationParameterHandler(self)
        logger.debug(f'Finished setting up vFlash project {project_name}.')

    @property
    def _dll(self) -> ctypes.WinDLL:
        """DLL object used to communicate with the C API."""
        if self.__dll is not None:
            return self.__dll
        logger.error('DLL is not loaded')
        raise VFlashError('DLL is not loaded')

    @property
    def _project_handle(self) -> VFlashProjectHandle:
        """Handle used by the API to work with the loaded project."""
        if self.__project_handle is not None:
            return self.__project_handle
        logger.error('Invalid use, the project has already been closed.')
        raise VFlashError('Invalid use, the project has already been closed.')

    @property
    def name(self) -> str:
        """Name of the loaded vflashpack project."""
        return self._project_name

    @property
    def flash_attribute(self) -> FlashAttributesHandler:
        """Handler of the flash attributes for the project."""
        return self.__flash_attribute

    @property
    def custom_action_attribute(self) -> CustomActionAttributesHandler:
        """Handler of the custom action attributes for the project."""
        return self.__custom_action_attribute

    @property
    def communication_parameter(self) -> CommunicationParameterHandler:
        """Handler of the communication parameters for the project."""
        return self.__communication_parameter

    def activate_reporting(self, report_path: str | Path) -> None:
        """
        Method to activate reporting for the flashing process with the loaded project.

        Args:
            report_path: The path to save the report in.
        """
        report_path = str(Path(report_path).absolute())
        report_path_str = ctypes.create_string_buffer(report_path.encode('ascii'))
        logger.debug(f'Activating reporting for project {self.name} with report path {report_path}...')

        vflash_activate_reporting = self._dll.vFlashActivateReporting
        vflash_activate_reporting.argtypes = [VFlashProjectHandle, ctypes.c_char_p, ctypes.c_int]
        vflash_activate_reporting.restype = FlashResult

        result = vflash_activate_reporting(self._project_handle, report_path_str, FlashReportingOptions.DEFAULT.value)

        if result != FlashResult.SUCCESS:
            error_msg = f'Error activating reporting: {result.name}\n{self.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug('Reporting activated successfully.')

    def flash(self, timeout_s: float = 3600) -> None:
        """
        Method to flash with the files specified in the vflashpack project.

        Args:
            timeout_s: Timeout to finish flashing.

        Raises:
            VFlashError: There was an error flashing.
            TimeoutError: The timeout expired before the flashing was finished.
        """
        logger.debug(f'Flashing vflash project {self.name}...')
        vflash_start = self._dll.vFlashStart
        vflash_start.argtypes = [VFlashProjectHandle, ProgressFunc, StatusFunc]
        vflash_start.restype = FlashResult

        flashing_in_progress = True
        flashing_status = None

        @ProgressFunc
        def report_progress(current_progress: int, remaining_time_s: int) -> None:
            """
            Callback function used by vFlash API in vFlashStart function to get updates
            in the flashing process.

            Args:
                current_progress: The flashing progress in percentage.
                remaining_time_s: Remaining flashing time in seconds.
            """
            logger.info(f'Flashing progress: {current_progress}% - Remaining time: {remaining_time_s}s')

        @StatusFunc
        def report_status(status: int) -> None:
            """
            Callback function used by vFlash API in vFlashStart function to
            report when the flashing is done, whether successfully or because of
            an error

            Args:
                status: The flashing status
            """
            nonlocal flashing_status
            flashing_status = FlashStatus(status)

            nonlocal flashing_in_progress
            flashing_in_progress = False

        start = time.time()
        result = vflash_start(
            self._project_handle,
            report_progress,
            report_status
            )
        if result != FlashResult.SUCCESS:
            error_msg = f'Error starting flashing process: {result.name}\n{self.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug('Waiting for flashing to finish...')
        while time.time() - start < timeout_s:
            if not flashing_in_progress:
                logger.debug('Flashing no longer in progress.')
                if flashing_status != FlashStatus.SUCCESS:
                    error_msg = f'Error flashing: {flashing_status.name}\n{self.get_last_error()}'
                    logger.error(error_msg)
                    raise VFlashError(error_msg)
                logger.debug('Flashing finished successfully.')
                return
            time.sleep(0.5)

        # Stop flashing if timeout expired
        logger.debug('Timeout expired while flashing, attempting to stop...')
        vflash_stop = self._dll.vFlashStop
        vflash_stop.restype = FlashResult

        result = vflash_stop()
        if result == FlashResult.SUCCESS:
            logger.error('Flashing stopped successfully')
            raise TimeoutError('Flashing timed out, flashing process was stopped')
        else:
            error_msg = f'Flashing timeout expired. An error occured while trying to stop flashing: {result.name}\n{self.get_last_error()}'
            logger.error(error_msg)
            raise TimeoutError(error_msg)

    def __enter__(self) -> 'VFlashProject':
        """
        Enter method to use VFlashProject as a context manager.

        Returns:
            self
        """
        return self

    def __exit__(self, exc_type, exc_value, exc_tb) -> None:
        """
        Exit method to use VFlashProject as a context manager.

        Args:
            exc_type: Type of exception in case an exception was raised inside the context management block.
            exc_value: The exception value in case an exception was raised inside the context management block.
            exc_tb: The exception traceback in case an exception was raised inside the context management block.
        """
        self.close()

    def __del__(self) -> None:
        """
        Destructor method for VFlashProject class.
        """
        self.close()

    def close(self) -> None:
        """
        Method to unload the vflashpack project.
        """
        if self.__project_handle is not None:
            logger.debug(f'Closing project {self.name}...')
            vflash_unload_project = self._dll.vFlashUnloadProject
            vflash_unload_project.restype = FlashResult

            result = vflash_unload_project(self.__project_handle)
            if result != FlashResult.SUCCESS:
                logger.warning(f'Error unloading project: {result.name}\n{self.get_last_error()}')
            logger.debug('Project unloaded successfully.')

        self.__project_handle = None

    def get_last_error(self) -> str:
        """
        Method to get the error message in case an error occured executing
        a vflash operation.

        Returns:
            The last error message.

        Raises:
            Exception: There was an error trying to get the last error message.
        """
        return self.__api.get_last_error()


class VFlash:
    """
    Class to interact with the vFlash C API.
    """

    def __init__(self, executable_path: str | Path, dll_path: str | Path) -> None:
        """
        Init method for VFlash class.

        Args:
            executable_path: The path to vFlash.exe.
            dll_path: The path to the C API DLL provided in the vFlash installation.

        Raises:
            Exception: There was an error initializing the C API.
        """
        logger.debug(f'Loading vFlash with executable path {executable_path} and DLL path {dll_path}...')
        self.__executable = Path(executable_path)
        if not self.__executable.exists():
            logger.error(f'Specified vflash executable does not exist: {self.__executable}.')
            raise FileNotFoundError(f'Specified vflash executable does not exist: {self.__executable}.')

        dll_path = Path(dll_path).absolute()
        if not dll_path.exists():
            logger.error(f'DLL does not exist: {dll_path}.')
            raise FileNotFoundError(f'DLL does not exist: {dll_path}.')

        logger.debug('Loading DLL...')
        self.__dll = ctypes.windll.LoadLibrary(str(dll_path))

        vflash_initialize = self._dll.vFlashInitialize
        vflash_initialize.restype = FlashResult

        logger.debug('Initializing API...')
        result = vflash_initialize()
        if result != FlashResult.SUCCESS:
            error_msg = f'Error initializing vFlash API: {result.name}\n{self.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        self.__api_initialized = True
        logger.debug('API initialized successfully.')

    def close_api(self) -> None:
        """
        Method to deinitialize the vflash C API.
        """
        logger.debug('Deinitializing vFlash API...')
        vflash_deinitialize = self._dll.vFlashDeinitialize
        vflash_deinitialize.restype = FlashResult

        result = vflash_deinitialize()
        if result != FlashResult.SUCCESS:
            logger.warning(f'Error deinitializing vFlash API: {result.name}\n{self.get_last_error()}')

        self.__api_initialized = False
        logger.debug('vFlash API deinitialized successfully.')

    def __del__(self) -> None:
        """
        Destructor method for VFlash class. It deinitializes the C API.
        """
        if self.__api_initialized:
            self.close_api()

    @property
    def _dll(self) -> ctypes.WinDLL:
        """DLL used to interact with the vFlash C API"""
        if self.__dll is not None:
            return self.__dll

        logger.error('DLL must be loaded before calling this method')
        raise VFlashError('DLL must be loaded before calling this method')

    @property
    def executable(self) -> Path:
        """Path to the vflash executable."""
        return self.__executable

    def get_last_error(self) -> str:
        """
        Method to get the error message in case an error occured executing
        a vflash operation.

        Returns:
            The last error message.

        Raises:
            Exception: There was an error trying to get the last error message.
        """
        vflash_get_last_error = self._dll.vFlashGetLastError

        logger.debug('Trying to get last error.')

        buffer = ctypes.cast(ctypes.create_string_buffer(b'\x00'*1024), ctypes.POINTER(ctypes.c_char))
        vflash_get_last_error(ctypes.byref(buffer), 1024)

        return ctypes.cast(buffer, ctypes.c_char_p).value.decode()

    def open(self, project_path: str | Path) -> VFlashProject:
        """
        Method to open a vflashpack project.

        Args:
            project_path: The path to the project.

        Returns:
            A VFlashProject object to work with the project.

        Raises:
            FileNotFoundError: The project file does not exist.
            VFlashError: There was an error loading the project.
        """
        logger.debug(f'Trying to open vflash project {project_path}...')
        project_path = Path(project_path).absolute()
        if not project_path.exists():
            logger.error('The specified project file does not exist')
            raise FileNotFoundError('The specified project file does not exist')

        project_path = str(project_path)

        project_handle = VFlashProjectHandle()

        vflash_load_project = self._dll.vFlashLoadProject
        vflash_load_project.restype = FlashResult

        logger.debug('Loading project...')
        result = vflash_load_project(project_path.encode(encoding='ascii'), ctypes.byref(project_handle))
        if result != FlashResult.SUCCESS:
            error_msg = f'Error loading project: {result.name}\n{self.get_last_error()}'
            logger.error(error_msg)
            raise VFlashError(error_msg)

        logger.debug('Project loaded successfully.')

        return VFlashProject(api=self, project_handle=project_handle, project_name=project_path)

    def convert_to_vflashpack(
            self,
            source_vflash_file: str,
            destination_file: str | Path = THIS_DIR / 'temp/temp.vflashpack'
    ) -> None:
        """
        Method to create a .vflashpack file from a .vflash file.

        Args:
            source_vflash_file: The .vflash file from which the .vflashpack file will be created.
            destination_file: The path to store the created .vflashpack file in.
        """
        logger.debug(f'Creating vflashpack file {destination_file} from vflash file {source_vflash_file}...')
        destination_path = Path(destination_file)
        if destination_path.exists():
            logger.debug('File in destination path already exists, removing it...')
            destination_path.unlink()

        source_path = Path(source_vflash_file)
        if not source_path.exists():
            logger.error(f'Source vflash project file does not exist: {source_path}.')
            raise FileNotFoundError(f'Source vflash project file does not exist: {source_path}.')

        p = subprocess.run([self.__executable, '/ExportToPack', source_path, destination_path], timeout=60)

        if p.returncode != 0:
            logger.error(f'Error trying to convert vflash file to vflashpack, exit code {p.returncode}')
            raise ChildProcessError(f'Error trying to convert vflash file to vflashpack, exit code {p.returncode}')

        logger.debug('vflashpack file successfully created.')


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 05/09/2023  ABPA       FKU-888   Initial creation
