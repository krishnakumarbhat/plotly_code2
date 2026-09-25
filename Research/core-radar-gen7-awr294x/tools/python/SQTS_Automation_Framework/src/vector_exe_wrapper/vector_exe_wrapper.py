"""
This module contains all classes and methods required to automate CANalyzer or CANoe.
"""
import time
from win32com import client
from win32com.client.dynamic import Dispatch
from sys import exit
from pathlib import Path
from typing import Literal, TypeVar, Optional, Callable
import can
import logging

from .com_handlers import Nodes, Networks, DiagnosticsDevice
from .can_logging import LoggingBlock, LoggingCollection, LogManager
from .periodic_messages import PeriodicMessage, PeriodicMessagesManager
from .fdx import FDXInterface
from .utils import data_length_to_dlc, dlc_to_data_length
from .canoe_ethernet import EthernetHandler, ProtocolType, DiagnosticType
from ..utils import com_sleep

logger = logging.getLogger(__name__)

# Dynamic types used with the COM interface
IApplication = TypeVar('IApplication')
IMeasurement = TypeVar('IMeasurement')
ISystem = TypeVar('ISystem')
INamespaces = TypeVar('INamespaces')
ICAPLFunction = TypeVar('ICAPLFunction')

# Constants
EXTENDED_ID_FLAG = 0x80000000
FDX_SEND_MESSAGE_ID = 0x8000
FDX_RECEIVE_MESSAGE_ID = 0x8001
FDX_SEND_PACKET_ID = 0x8002
FDX_RECEIVE_PACKET_ID = 0x8003
FDX_SEND_ETHERNET_DIAGNOSTIC_ID = 0x8004

THIS_DIR = Path(__file__).parent
SQTS_DIR = THIS_DIR.parent.parent

# Create python script header
__author__ = 'Rachel McDaniel & Devin Jaenicke'
__version__ = '1.0.0'
__email__ = 'devin.k.jaenicke@aptiv.com'
__copyright__ = 'Copyright 2018 Aptiv, All Rights Reserved.'


class _Mutex:
    """
    Class to lock shared resources.

    Attributes:
        _locked: Whether the resource is locked.
    """
    def __init__(self) -> None:
        """
        Init method for _Mutex class.
        """
        self._locked: bool = False

    def is_locked(self) -> bool:
        """
        Method to check if the resource is locked.

        Returns:
            Whether the resource is locked.
        """
        return self._locked

    def lock(self) -> None:
        """
        Method to lock the resource.
        """
        if not self._locked:
            self._locked = True
        else:
            logger.error("Invalid Mutex usage")
            raise Exception("Invalid Mutex usage")

    def unlock(self) -> None:
        """
        Method to unlock the resource.
        """
        if self._locked:
            self._locked = False
        else:
            logger.error("Invalid Mutex usage")
            raise Exception("Invalid Mutex usage")


class SignalState:
    """
    Base class used to indicate the state of a signal in CANoe.
    """
    DEFAULT_VALUE = 0
    MEAS_NOT_RUNNING_APP_VALUE = 1
    MEAS_NOT_RUNNING_LAST_VALUE = 2
    SIG_RXED_IN_CURRENT_MEASUREMENT = 3
    UNKNOWN = 4

    def __init__(self) -> None:
        """
        Init method for signal state class.
        """
        self.state = self.UNKNOWN


class Signal(SignalState):
    """
    Class used for returning the results of a CAN signal read.

    Attributes:
        read_error: True when an error occurred while reading the signal.
        raw_value: Raw value of the signal.
        phys_value: Physical value of the signal.
        state: State of the signal.
        is_online: True when measurement is running and the parent message is received.
    """
    def __init__(self):
        self.read_error: bool = False
        self.raw_value: int = 0
        self.phys_value: float = 0.0
        SignalState.__init__(self)
        self.is_online: bool = False  # True when measurement is running and the message is received


class VectorExe:
    """
    Class to be used by all integration tests for controlling CANalyzer/CANoe

    Attributes:
        networks: The networks in the simulation setup.
        config_file_name: The name of the currently loaded CANoe configuration.
        version: The CANoe version.
        measurement_running: Whether the measurement in CANoe is currently running.
        _can_app: Whether to use CANoe or CANalyzer.
        _verbose_mode: Whether to print trace messages.
        _app_com_obj: The COM interface object to control CANoe or CANalyzer.
        _measurement: The COM interface object to control the measurement.
        _system: The COM interface object to control the CANoe or CANalyzer system.
        _namespaces: The COM interface object to control the namespaces.
        _capl_func_dict: A dictionary that stores CAPL function objects.
        _capl_func_mutex: A mutex to control the execution of CAPL functions.
        _config_file: The path to the currently loaded configuration.
        _logging_collection: A collection of logging blocks.
        _version: The CANoe or CANalyzer version in a tuple with the format (major number, minor number).
        _pytest_data: Data provided during pytest execution.
        nodes: Nodes object used to manage the simulation nodes added by the framework.
        com_interface: The COM interface used to interact with CANoe.

    Class Attributes:
        MAX_CAPL_FUNC_PARAMS: The maximum number of parameters that a CAPL function can receive with COM.
    """
    class exposed_types:
        """
        Class to expose internal types.
        """
        PeriodicMessage = PeriodicMessage
        ProtocolType = ProtocolType
        DiagnosticType = DiagnosticType

    MAX_CAPL_FUNC_PARAMS = 10

    def __init__(
            self,
            application: Literal['CANoe', 'CANalyzer'],
            enable_verbose: bool = True
            ) -> None:
        """
        Init method for VectorExe class.

        Args:
            application: Which Vector application should be used.
            enable_verbose: Whether to print trace messages.
        """
        self._can_app: Literal['CANoe', 'CANalyzer'] = application
        self._verbose_mode: bool = enable_verbose
        self._app_com_obj: IApplication = None
        self._measurement: IMeasurement = None
        self._system: ISystem = None
        self._namespaces: INamespaces = None
        self._capl_func_dict: dict[str, ICAPLFunction] = {}
        self._capl_func_mutex: _Mutex = _Mutex()
        self._config_file: Path = None
        self._logging_collection: LoggingCollection = None
        self._version: tuple[int, int] = None
        self._pytest_data: dict = {}
        self.nodes: Nodes = None
        self.networks: Networks = None
        self.ethernet: EthernetHandler = None

    @property
    def config_file_name(self) -> str:
        """
        Name of the loaded configuration.
        """
        if self._config_file is None:
            return ''
        return str(self._config_file.absolute())

    @property
    def version(self) -> tuple[int, int]:
        """
        Version of the program used.
        """
        return self._version

    @property
    def com_interface(self) -> IApplication:
        """
        COM interface used to communicate with CANoe.
        """
        return self._app_com_obj

    @property
    def measurement_running(self) -> bool:
        """
        Whether the measurement is currently running.
        """
        return self._measurement.Running

    def start_exe(self, seconds_delay: float = 10) -> None:
        """
        Method for starting the Vector application. This method will raise an error if the
        Vector application cannot be opened correctly. The user should handle the error case
        in their script.

        Args:
            seconds_delay: Amount of time to delay to allow the application to open.
        """
        logger.info(f'Opening {self._can_app}...')

        if "CANalyzer" == self._can_app:
            progID = "CANalyzer.Application"
        elif "CANoe" == self._can_app:
            progID = "CANoe.Application"
        else:
            logger.error(f"Tried to open an unsopported application: {self._can_app}")
            raise Exception(f"Unsupported Vector application - {self._can_app}")

        try:
            logger.debug(f"Opening {self._can_app} executable...")
            self._app_com_obj = Dispatch(progID)
            self._version = (self._app_com_obj.Version.major, self._app_com_obj.Version.minor)

        except Exception:
            logger.error(f"Error opening {self._can_app}. Please verify that the application is installed.")
            raise

        logger.debug(f'Starting delay after opening {self._can_app}...')
        time.sleep(seconds_delay)

    def load_config(
            self,
            cfg_file: str,
            capl_func_list: list[str] = [],
            seconds_timeout: float = 5,
            fdx_ip_address: str = '127.0.0.1',
            fdx_port: int = 2809,
            fdx_transport_layer: Literal['UDP', 'TCP'] = 'UDP'
            ) -> None:
        """
        Method for loading a CANalyzer/CANoe config file.

        NOTE:
            start_exe method must be called first.

        Args:
            cfg_file: Path to config file (path + file name).
            capl_func_list: A list of CAPL function names to load for use with invoke_capl_func method.
            seconds_timeout: Amount of time to wait for the config to load before exiting.
            fdx_ip_address: The ethernet address to use for communication with CANoe via FDX.
            fdx_port: The ethernet port to use for communication with CANoe via FDX.
            fdx_transport_layer: The transport layer used for communication with CANoe via FDX.
        """
        cfg_file = Path(cfg_file)

        if not cfg_file.exists():
            logger.error(f"Config file not found {cfg_file.absolute()}")
            self.__error_clean_up()

        # Need to stop any running measurements before loading new config
        logger.debug('Stopping measurement to load the configuration...')
        self.__safe_measurement_stop()

        try:
            cfg_file_abspath = str(cfg_file.absolute())
            logger.info(f'Opening configuration file - {cfg_file_abspath}')
            self._app_com_obj.Open(cfg_file_abspath)
        except Exception as e:
            logger.error(f"Configuration file failed to load - {e}")

            logger.debug('Trying to get more information about why the configuration failed to load...')

            try:
                error_msg = self._app_com_obj.Configuration.OpenConfigurationResult.ErrorMessage.strip()
                if error_msg:
                    logger.error(f'Error message from CANoe: {error_msg}')
                else:
                    logger.error('Empty error/warning explanation from CANoe.')
            except Exception as e:
                logger.error(f'Failed to get error explanation from CANoe - {e}')

            self.__error_clean_up()

        # Wait for configuration to load
        logger.debug('Waiting for configuration to be loaded...')
        start = time.time()
        # Note: zero indicates that the config file opened successfully
        while self._app_com_obj.Configuration.OpenConfigurationResult.result != 0:
            elapsed = time.time() - start
            if elapsed > seconds_timeout:
                logger.error('Timeout expired trying to load the configuration file')
                logger.debug('Trying to get more information about why the configuration failed to load...')
                try:
                    error_msg = self._app_com_obj.Configuration.OpenConfigurationResult.ErrorMessage.strip()
                    if error_msg:
                        logger.error(f'Error message from CANoe: {error_msg}')
                    else:
                        logger.error('Empty error/warning explanation from CANoe.')
                except Exception as e:
                    logger.error(f'Failed to get error explanation from CANoe - {e}')
                self.__safe_exe_quit()

        logger.debug(f'Configuration loaded successfully after {time.time()-start}s')

        # Save path of config file
        self._config_file = cfg_file

        # General COM objects
        logger.debug('Getting interfaces for common CANoe objects...')
        self._measurement = self._app_com_obj.Measurement
        self._system = self._app_com_obj.System
        self._namespaces = self._app_com_obj.System.Namespaces

        # SQTS system variables
        logger.debug('Adding SQTS system variables to the configuration...')
        self._system.VariablesFiles.Add(str(THIS_DIR / r'sys_vars\sqts.vsysvar'))

        # Data required for expect_message and retrieve_message methods
        self._expected_message = None

        # Add FDX support
        self.__load_fdx(fdx_ip_address, fdx_port, fdx_transport_layer)

        # Logging
        logger.debug('Getting logging collection from configuration...')
        self._logging_collection = LoggingCollection(self._app_com_obj.Configuration.OnlineSetup.LoggingCollection)
        self._test_log_block = None

        # Create an empty dictionary between function name and CAPLFunction objects
        logger.debug('Setting up CAPL functions to be loaded...')
        self._capl_func_dict = {func_name: None for func_name in capl_func_list}
        self.__load_event_handler()

        # Simulation nodes
        self.__load_capl_nodes()

        # Networks
        self.networks = Networks(self)
        self.ethernet = EthernetHandler(self.fdx)

    def change_config(self,  config_file: str, capl_func_list: list[str], *args, **kwargs) -> str:
        """
        Method to change the CANoe configuration.

        WARNING:
            If the measurement is running, this method will stop it first, load the new configuration
            and then start it again. If the measurement was stopped, it will continue stopped after
            the new configuration is loaded.

        Args:
            config_file: Name of the config file to load.
            capl_func_list: The list of CAPL functions to load.
            args: Positional arguments to be passed to load_config method.
            kwargs: Keyword arguments to be passed to load_config method.

        Returns:
            The name of the current configuration before changing it.
        """
        logger.info('Changing the CANoe configuration...')
        current_config = self.config_file_name

        if Path(config_file) == self._config_file:  # If configuration already loaded
            logger.debug('Configuration is already loaded')
            logger.debug('Setting up CAPL functions to be loaded...')
            self.load_capl_functions(capl_func_list)
            return current_config

        if self._app_com_obj is not None:
            logger.debug('Stopping measurement to load the new configuration...')
            measurement_was_running = self.__safe_measurement_stop()
            try:
                logger.debug('Undoing changes in the configuration before changing it...')
                self._app_com_obj.Configuration.Modified = False
            except Exception as exc:
                logger.error('Error trying to undo changes to the configuration...')
                raise Exception('Error trying to change the configuration') from exc

            logger.debug('Loading new configuration...')
            self.load_config(config_file, capl_func_list, *args, **kwargs)

            if measurement_was_running:
                logger.debug('Restarting measurement after changing configuration...')
                self.start_measurement()

        return current_config

    def __load_event_handler(self) -> None:
        """
        Method to load COM events.
        """
        logger.debug('Loading event handler for measurement events...')

        class MeasurementEvents:
            """
            Class that contains the functions to be called when certain CANoe events occur.
            """
            # Note: these methods are executed asynchonously. This makes handling errors difficult.
            def OnInit(_):
                """
                Function that is executed when starting a measurement, before the measurement is
                actually running.
                """
                for func_name in list(self._capl_func_dict.keys()):
                    self._capl_func_dict[func_name] = self._app_com_obj.CAPL.GetFunction(func_name)

                # Make CANoe send these data groups whenever they are triggered by a CAPL script
                self.fdx.request_free_running_mode(group_id=FDX_SEND_MESSAGE_ID)
                self.fdx.request_free_running_mode(group_id=FDX_RECEIVE_MESSAGE_ID)
                self.fdx.request_free_running_mode(group_id=FDX_SEND_PACKET_ID)
                self.fdx.request_free_running_mode(group_id=FDX_RECEIVE_PACKET_ID)
                self.fdx.request_free_running_mode(group_id=FDX_SEND_ETHERNET_DIAGNOSTIC_ID)

        client.WithEvents(self._measurement, MeasurementEvents)

    def __load_fdx(
            self,
            fdx_ip_address: str,
            fdx_port: int,
            fdx_transport_layer: Literal['UDP', 'TCP']
            ) -> None:
        """
        Method to enable FDX support in CANoe and prepare things that are
        necessary for FDX communication.

        Args:
            fdx_ip_address: The ethernet address to use for communication with CANoe via FDX.
            fdx_port: The ethernet port to use for communication with CANoe via FDX.
            fdx_transport_layer: The transport layer used for communication with CANoe via FDX.
        """
        logger.debug('Adding FDX support...')

        # Enable FDX
        logger.debug('Enabling FDX...')
        i_configuration = self._app_com_obj.Configuration
        i_configuration.FDXEnabled = True

        sqts_fdx_description_file = THIS_DIR / r'fdx\fdx_description_files\sqts.xml'
        sqts_fdx_description_file_path = str(sqts_fdx_description_file)

        i_fdx_files = i_configuration.FDXFiles

        # Remove sqts fdx files if already loaded
        # Save indexes of fdx_files to remove
        logger.debug('Removing previous SQTS FDX description files...')
        indexes_to_remove = []
        for i, fdx_file in enumerate(i_fdx_files, start=1):
            if 'sqts' in Path(fdx_file.FullName).name:
                indexes_to_remove.append(i)
            del fdx_file

        # Remove them from greatest index to lowest
        for i in sorted(indexes_to_remove, reverse=True):
            i_fdx_files.Remove(i)

        # Load sqts fdx file
        logger.debug('Adding SQTS FDX description file...')
        try:
            sqts_fdx_description_file = i_fdx_files.Add(sqts_fdx_description_file_path)
            sqts_fdx_description_file.Enabled = True
        except Exception as exc:
            logger.error(f'Error trying to add SQTS FDX description file: {exc}')
            raise Exception('Error trying to add SQTS FDX description file') from exc

        # Load FDX interface
        logger.debug('Creating FDX interface...')
        self.fdx = FDXInterface(
            host=fdx_ip_address,
            port=fdx_port,
            transport_layer=fdx_transport_layer,
            recv_timeout_s=0.1,
            description_files=[sqts_fdx_description_file_path]
        )

        # Load counters used by send_message
        self._fdx_send_message_counter = 1

        logger.debug('FDX successfully enabled.')

    def __load_capl_nodes(self) -> None:
        """
        Method to load CAPL nodes necessary for some methods to work.
        If the CAPL nodes to load are already loaded, they will first be removed.
        """
        self.nodes = Nodes(self)

        sqts_node_path = THIS_DIR / r'simulation_nodes\sqts_simulation_node.can'

        self.nodes.add(sqts_node_path)

        logger.debug('Compiling CAPL nodes...')
        self.com_interface.CAPL.Compile()

        logger.debug('SQTS simulation nodes added successfully.')

    def load_capl_functions(self, function_names: list[str]) -> None:
        """
        Method to load CAPL functions. To invoke a CAPL function, its corresponding COM object is needed.
        These COM objects cannot be requested from CANoe while the simulation is running, so they must be
        requested and stored beforehand. The load_config method requests COM objects for the functions passed
        in the capl_func_list argument. This method can also be used to request them without having to reload
        the configuration.

        WARNING:
        If the simulation is running when this method is called, it will stop the simulation before requesting
        the COM objects for the CAPL functions, and start it again after it has obtained it.

        Args:
            function_names: A list with CAPL function names to load.
        """
        logger.debug('Loading newly specified CAPL functions...')

        logger.debug('Stopping measurement to load CAPL functions...')
        measurement_was_running = self.__safe_measurement_stop()
        self._capl_func_dict = {func: None for func in function_names} | self._capl_func_dict

        if measurement_was_running:
            logger.debug('Restarting measurement after loading CAPL functions...')
            self.start_measurement()

    def start_measurement(self, seconds_timeout: float = 20) -> None:
        """
        Method for starting the CANalyzer/CANoe measurement

        NOTE:
            start_exe and load_config methods must be called first.

        Args:
            seconds_timeout: Amount of time to wait for the measurement to begin before exiting.
        """
        logger.debug('Trying to start measurement...')

        if self._measurement is None:
            logger.error("Configuration must be loaded before a measurement can be started...")
            return

        if self._measurement.Running:
            logger.debug("Measurement already running.")
            return

        # Clean temporary logs created
        logger.debug('Cleaning temporary log files...')
        for f in (SQTS_DIR / 'logs/canoe_logs').glob('temp_log*.blf'):
            try:
                f.unlink()
            except PermissionError:
                logger.warning(f"Couldn't delete file {f.absolute()}.")

        try:
            logger.debug('Starting measurement...')
            self._measurement.Start()
        except Exception:
            logger.error("Unable to start measurement... verify HW and SW configurations")
            self.__safe_exe_quit()

        # Wait for measurement to start
        start = time.time()
        while not self._measurement.Running:
            elapsed = time.time() - start
            if elapsed > seconds_timeout:
                logger.error("Starting measurement timed out... Check GUI for details")
                self.__safe_exe_quit()
            com_sleep(0.1)

        logger.debug(f'Measurement successfully started after {time.time()-start}s.')

    def stop_measurement(self) -> None:
        """
        Method for stoping the CANalyzer/CANoe measurement.

        NOTE:
            If the measurement isn't started, this method is inert.
        """
        logger.info('Stopping measurement...')
        self.__safe_measurement_stop()

    def send_can_signal_raw(self, channel: int, message: str, signal: str, value: int, timeout_s: float = 15) -> Signal:
        """
        Method for writing a CAN signal with its raw value.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            channel: Physical CAN channel where the signal is sent or received.
            message: Name of CAN message containing the signal.
            signal: Name of signal to be read.
            value: Raw value to write to the signal.
            timeout_s: Timeout in seconds that the signal should try verifying the sent value
                before returning an error.

        Returns:
            Signal object containing the read results.
        """
        logger.debug(f'Sending CAN signal {message}.{signal} through channel {channel} with raw value {value}...')
        self.__check_method_preconditions("read_can_signal", test_running=False)
        ret_sig = Signal()

        try:
            logger.debug('Getting signal from bus...')
            signal_obj = self._app_com_obj.Bus.GetSignal(channel, message, signal)

            logger.debug('Setting new signal value...')
            signal_obj.RawValue = value

            # read signal again
            current_signal = self._app_com_obj.Bus.GetSignal(channel, message, signal)
            # wait till signal is updated
            start = time.time()
            while (round(current_signal.RawValue) != round(value)):
                if time.time() - start > timeout_s:
                    logger.error(f'Timeout: Read value from signal {message}.{signal} is not equal to the value written.')
                    break
                current_signal = self._app_com_obj.Bus.GetSignal(channel, message, signal)
            else:
                logger.debug('Successfully set new value of the signal.')

            ret_sig.raw_value = current_signal.RawValue
            ret_sig.phys_value = current_signal.Value
            ret_sig.state = current_signal.State
            ret_sig.is_online = current_signal.IsOnline
        except Exception as e:
            logger.error(f'Exception occured while trying to set signal value: {e}.')
            ret_sig.read_error = True

        if not ret_sig.read_error:
            if ret_sig.is_online:
                logger.debug(f"{message}.{signal} = {ret_sig.phys_value}")
            else:
                logger.debug(f"Signal read, but never received: {message}.{signal}")
        else:
            logger.error(f"Unable to read {message}.{signal}")

        return ret_sig

    def send_can_signal_phy(self, channel: int, message: str, signal: str, value: float, timeout_s: float = 15) -> Signal:
        """
        Method for writing a CAN signal with its physical value.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            channel: Physical CAN channel where the signal is sent or received.
            message: Name of CAN message containing the signal.
            signal: Name of signal to be read.
            value: Physical value to write to the signal.
            timeout_s: Timeout in seconds that the signal should try verifying the sent value
                before returning an error.

        Returns:
            Signal object containing the read results.
        """
        logger.debug(f'Sending CAN signal {message}.{signal} through channel {channel} with physical value {value}...')

        self.__check_method_preconditions("read_can_signal", test_running=False)
        ret_sig = Signal()

        try:
            logger.debug('Getting signal from bus...')
            signal_obj = self._app_com_obj.Bus.GetSignal(channel, message, signal)

            logger.debug('Setting new signal value...')
            signal_obj.Value = value

            # read signal again
            current_signal = self._app_com_obj.Bus.GetSignal(channel, message, signal)
            # wait till signal is updated
            start = time.time()
            while (round(current_signal.Value) != round(value)):
                if time.time() - start > timeout_s:
                    logger.error(f'Timeout: Read value from signal {message}.{signal} is not equal to the value written.')
                    break
                current_signal = self._app_com_obj.Bus.GetSignal(channel, message, signal)
            else:
                logger.debug('Successfully set new value of the signal.')

            ret_sig.raw_value = current_signal.RawValue
            ret_sig.phys_value = current_signal.Value
            ret_sig.state = current_signal.State
            ret_sig.is_online = current_signal.IsOnline
        except Exception as e:
            logger.error(f'Exception occured while trying to set signal value: {e}.')
            ret_sig.read_error = True

        if not ret_sig.read_error:
            if ret_sig.is_online:
                logger.debug(f"{message}.{signal} = {ret_sig.phys_value}")
            else:
                logger.debug(f"Signal read, but never received: {message}.{signal}")
        else:
            logger.error(f"Unable to read {message}.{signal}")

        return ret_sig

    def read_can_signal(self, channel: int, message: str, signal: str) -> Signal:
        """
        Method for reading a CAN signal.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            channel: Physical CAN channel where the signal is sent or received.
            message: Name of CAN message containing the signal.
            signal: Name of signal to be read.

        Returns:
            Signal object containing the read results.
        """
        logger.debug(f'Reading CAN signal {message}.{signal} from channel {channel}...')
        self.__check_method_preconditions("read_can_signal", test_running=False)
        ret_sig = Signal()

        try:
            logger.debug('Getting signal object from bus...')
            signal_obj = self._app_com_obj.Bus.GetSignal(channel, message, signal)
            ret_sig.raw_value = signal_obj.RawValue
            ret_sig.phys_value = signal_obj.Value
            ret_sig.state = signal_obj.State
            ret_sig.is_online = signal_obj.IsOnline
        except Exception as e:
            logger.error(f'Exception occured trying to read signal: {e}')
            ret_sig.read_error = True

        if not ret_sig.read_error:
            if ret_sig.is_online:
                logger.debug(f"{message}.{signal} = {ret_sig.phys_value}")
            else:
                logger.debug(f"Signal read, but never received: {message}.{signal}")
        else:
            logger.error(f"Unable to read {message}.{signal}")

        return ret_sig

    def read_sys_variable(self, namespace: str, var_name: str) -> int | float | str:
        """
        Method for reading a system variable.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            namespace: The namespace where the variable is located.
            var_name: The name of the variable to read.

        Returns:
            The value of the variable.
        """
        logger.debug(f'Reading system variable {namespace}::{var_name}...')
        if self._namespaces is not None:
            value = self._namespaces.Item(namespace).Variables.Item(var_name).Value
            logger.debug(f'System variable {namespace}::{var_name} has value {value}')
            return value
        else:
            logger.error('No namespaces found')
            raise Exception("No namespaces found")

    def write_sys_variable(self, namespace: str, var_name: str, value: int | float | str, seconds_timeout: float = 5) -> int | float | str:
        """
        Method for writing a value to a system variable.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            namespace: The namespace where the variable is located.
            var_name: The name of the variable to read.

        Returns:
            The current value of the variable.
        """
        logger.debug(f'Setting value of system variable {namespace}::{var_name} to {value}...')
        if self._namespaces is not None:
            current_val = self.read_sys_variable(namespace, var_name)
            logger.debug(f'Current value of {namespace}::{var_name} is {current_val}.')

            logger.debug('Setting new value for system variable...')
            self._namespaces.Item(namespace).Variables.Item(var_name).Value = value
            if (namespace == "Security_Access"):
                logger.warning("Can't set value of system variable because it's part of the Security_Access namespace.")
                current_val = value
            else:
                start = time.time()
                while current_val != value:
                    current_val = self.read_sys_variable(namespace, var_name)
                    elapsed = time.time() - start
                    if elapsed > seconds_timeout:
                        logger.error("Writing variable timed out")
                        raise Exception("Writing variable timed out")

                logger.debug(f'Value of system variable set successfully after {time.time() - start}s.')
        else:
            logger.error('No namespaces found')
            raise Exception("Namespace not found")

        return current_val

    def get_diagnostics_device(self, network: str, ecu_qualifier: str) -> DiagnosticsDevice:
        """
        Method to get a diagnostics device through which one can make diagnostic requests.

        Args:
            network: Exact name of network (e.g. FDCAN3_1)
            ecu_qualifier: Exact name of ecu_qualifier (e.g. CADM_0002_00000000_032)

        Returns:
            A DiagnosticsDevice object.

        Raises:
            Exception: The network or the ecu_qualifier don't exist in the configuration.
        """
        try:
            com_diag_device = self._app_com_obj.Networks(network).Devices(ecu_qualifier).Diagnostic
        except Exception as exc:
            raise Exception(f'Error occurred while trying to get the {ecu_qualifier} diagnostics device from the network {network}.') from exc

        return DiagnosticsDevice(com_diag_device, network=network, ecu_qualifier=ecu_qualifier)

    def send_diag_req(
            self,
            network: str,
            ecu_qualifier: str,
            req_bytes: bytes,
            *,
            timeout_s: float = 10,
            callback: Optional[Callable[[], None]] = None
            ) -> tuple[bool, bytes]:
        """
        Method for Sending Diagnostic request

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            network: Exact name of network (e.g. FDCAN3_1)
            ecu_qualifier: Exact name of ecu_qualifier (e.g. CADM_0002_00000000_032)
            req_bytes: stream of bytes to be send

        Returns:
            A tuple with the format (whether the request received a positive response, the response bytes).
        """
        diag_device = self.get_diagnostics_device(network=network, ecu_qualifier=ecu_qualifier)

        responses = diag_device.send_request(req_bytes, timeout_s=timeout_s, callback=callback)

        if not responses:
            return False, b''

        response = responses[0]  # Getting just the first response because the return type only allows for one
        positive = response.positive
        resp_bytes = response.data

        logger.debug(f'{"Positive" if positive else "Negative"} response received: {resp_bytes.hex(" ")}')

        return positive, resp_bytes

    def press_key(self, key: str) -> None:
        """
        Method to simulate pressing a key in CANoe.

        Args:
            key: The key to press.
        """
        self.fdx.press_key(key)

    def send_message(
            self,
            id: int,
            data: list[int] | bytes | bytearray,
            channel: int,
            extended_id: bool = False,
            fd: bool = False,
            timeout_s: float = 1.0
            ) -> can.Message:
        """
        Function to send a message by updating the values of system variables.
        The sqts_simulation_node capl node that is attached to the simulation
        will assemble and send the message when the SQTS::send_message_counter
        is updated.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            id: The ID of the message.
            data: The data to send in the message, a list of integers greater or equal to 0 and less than 256.
            channel: Which channel should be used to send the message.
            extended_id: Flag to indicate if the message ID is extended or not.
            fd: Whether the message should be sent as CAN FD or as Standard CAN.
            timeout_s: Timeout to try confirming that the message was sent.

        Raises:
            TimeoutError: The timeout expired trying to confirm that the message was sent.
            Exception: There was an error in CAPL sending the message.
        """
        logger.debug(f'Sending message with ID {id} and data {data} through channel {channel}...')

        data_length = len(data)
        if data_length > 64:
            logger.error(f'Message size too large, expected a maximum of 64 bytes, received {data_length}')
            raise Exception(f'Message size too large, expected a maximum of 64 bytes, received {data_length}')

        if not (0 <= channel < 256):
            logger.error(f'Channel must be between 0 and 255, {channel} is invalid.')
            raise Exception(f'Channel must be between 0 and 255, {channel} is invalid.')

        dlc = data_length_to_dlc(data_length)
        new_data_length = dlc_to_data_length(dlc)

        try:
            # Verify that it can be converted to bytes and fill the rest with 00
            logger.debug('Converting data to bytes and filling with 00...')
            data = bytes(data) + (new_data_length-data_length)*b'\x00'
        except Exception:
            logger.error('Error converting data to bytes.')
            raise Exception(f'Message data could not be converted to bytes: {data}') from None

        # Translate ID if it's extended
        arbitration_id = id
        is_extended_id = (id & 0xFFFFF800 != 0) or extended_id
        if is_extended_id:  # If the ID uses more than 11 bits or was marked explicitly as extended
            logger.debug('Converting ID to extended ID...')
            id |= EXTENDED_ID_FLAG

        if dlc > 8:
            fd = True

        if fd:
            logger.debug('Message will be sent as a CAN FD message.')
        else:
            logger.debug('Message will be sent as a standard CAN message.')

        current_counter = self._fdx_send_message_counter

        fdx_data = {
            'SQTS::send_message::message_buffer': data,
            'SQTS::send_message::id': id,
            'SQTS::send_message::dlc': dlc,
            'SQTS::send_message::channel': channel,
            'SQTS::send_message::fd': fd,
            'SQTS::send_message::counter': current_counter
        }

        self._fdx_send_message_counter += 2  # The sent counter is always an odd number, and the received counter even.

        # Send updated system variables data to CANoe through FDX, this triggers a CAPL on sysvar event
        # in sqts_simulation_node that assembles and sends the message.
        logger.debug(f'Sending message with counter {current_counter}...')
        self.fdx.send_data(fdx_data, group_id=FDX_SEND_MESSAGE_ID)

        # Once the CAPL script has sent the message, it will trigger the transmission of that
        # same data group from CANoe to this script. wait for that response to verify
        # that the message has been sent.
        logger.debug('Verifying that the message was sent correctly...')
        start = time.time()
        received_data = self.fdx.receive_data(FDX_SEND_MESSAGE_ID, timeout_s=timeout_s)
        elapsed = time.time() - start
        while elapsed < timeout_s:
            # Verify that the check_counter is correct, it should be the sent counter plus 1
            if received_data['SQTS::send_message::check_counter'] == current_counter + 1:
                error_code = received_data['SQTS::send_message::error_code']
                if error_code != 0:
                    logger.error(f'There was an error sending the message, error code: {error_code}.')
                    raise Exception(f'There was an error sending the message, error code: {error_code}.')
                logger.debug('Message sent correctly.')
                return can.Message(
                    timestamp=received_data['SQTS::send_message::sent_timestamp_ns'] / 1_000_000_000,
                    arbitration_id=arbitration_id,
                    is_extended_id=is_extended_id,
                    channel=channel,
                    dlc=dlc,
                    data=data,
                    is_fd=fd,
                    is_rx=False
                )
            else:
                received_data = self.fdx.receive_data(FDX_RECEIVE_MESSAGE_ID, timeout_s=timeout_s-elapsed)
                elapsed = time.time() - start
        else:
            logger.error('Timeout expired while verifying that the message was sent correctly')
            raise TimeoutError('Timeout expired while verifying that the message was sent correctly')

    def expect_message(
            self,
            id: int,
            is_extended_id: bool = False,
            channel: int = 0,
            *,
            num_messages: int = 1
    ) -> None:
        """
        Method to tell the sqts_simulation_node to expect a message and send it
        to the framework if it is received in CANoe.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            id: The ID of the expected message.
            is_extended_id: Whether the ID is an extended ID.
            channel: The channel through which the message should come from,
                if set as 0, the message can come through any channel.

        Raises:
            Exception: A message is already expected.
        """
        logger.debug(f'Expecting {num_messages} messages with ID 0x{id:X} through channel {channel}...')

        # Check status of expected message
        if self._expected_message is not None:
            expected_id = self._expected_message['id']

            logger.warning(f'Called expected_message when another message is already expected (ID 0x{expected_id:X}).')

        arbitration_id = id & 0x7FFFFFFF

        # Convert ID if it's extended
        if id & 0xFFFFF800 or is_extended_id:
            logger.debug('Converting ID to extended ID...')
            is_extended_id = True
            id |= EXTENDED_ID_FLAG

        fdx_data = {
            'SQTS::receive_message::expected_number_of_messages': num_messages,
            'SQTS::receive_message::expected_id': id,
            'SQTS::receive_message::expected_channel': channel
        }

        # Send updated system variables data to CANoe through FDX, this tells a CAPL on message * event
        # in sqts_simulation_node to wait for a specific message to arrive and store its data in
        # the SQTS::receive_message system variables
        logger.debug(f'Requesting CANoe to send message 0x{arbitration_id:X} after it arrives in the bus...')
        self.fdx.send_data(fdx_data, group_id=FDX_RECEIVE_MESSAGE_ID)

        # Update status of expected message
        self._expected_message = {
            'id': arbitration_id,
            'extended_id': is_extended_id,
            'num_expected_messages': num_messages
        }

    def retrieve_message(self, timeout_s: float = 1.0) -> can.Message:
        """
        Method to receive a message an expected message.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            timeout_s: Timeout to wait for the message to come through.

        Returns:
            The received CAN message.

        Raises:
            Exception: No message is expected.
            Exception: There was an error retrieving the message data from CAPL.
            TimeoutError: The timeout expired while waiting for the message.
        """
        # Check if a message is expected
        if self._expected_message is None:
            logger.error('Tried to retrieve a message when no message was expected.')
            raise Exception('Cannot retrieve message, no message was expected.')

        # Once the CAPL event handler has stored the data in system variables, it will trigger
        # the transmission of the receive_message data group through FDX, wait for that data
        # and build the can.Message from it.
        expected_id = self._expected_message['id']
        logger.debug(f'Retrieving message with ID 0x{expected_id:X} from CANoe...')
        received_data = self.fdx.receive_data(FDX_RECEIVE_MESSAGE_ID, timeout_s=timeout_s)
        logger.debug('Received response from CANoe.')
        error_code = received_data['SQTS::receive_message::error_code']
        if error_code != 0:
            logger.error(f'There was an error retrieving the message data, error code: {error_code}')
            raise Exception(f'There was an error retrieving the message data, error code: {error_code}')

        message = can.Message(
            arbitration_id=expected_id,
            timestamp=received_data['SQTS::receive_message::timestamp_ns'] / 1_000_000_000,
            is_extended_id=received_data['SQTS::receive_message::is_extended_id'],
            channel=received_data['SQTS::receive_message::channel'],
            dlc=received_data['SQTS::receive_message::dlc'],
            data=received_data['SQTS::receive_message::message_buffer'],
            is_fd=received_data['SQTS::receive_message::is_fd']
        )

        logger.debug(f'Received message {message}.')

        # Update status of expected message
        if self._expected_message['num_expected_messages'] <= 1:
            logger.debug('No more messages expected.')
            self._expected_message = None
        else:
            self._expected_message['num_expected_messages'] -= 1
            num_expected_messages = self._expected_message['num_expected_messages']
            logger.debug(f'{num_expected_messages} message(s) still expected.')

        return message

    def receive_message(
            self,
            id: int,
            is_extended_id: bool = False,
            channel: int = 0,
            timeout_s: float = 1.0
            ) -> can.Message:
        """
        Method to receive a message that has yet to be sent through the bus.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            id: The ID of the expected message.
            is_extended_id: Whether the ID is an extended ID.
            channel: The channel through which the message should come from,
                if set as 0, the message can come through any channel.
            timeout_s: Timeout to wait for the message to come through.

        Returns:
            The received CAN message.

        Raises:
            TimeoutError: The timeout expired while waiting for the message.
            Exception: There was an error retrieving the message data from CAPL.
        """
        self.expect_message(id=id, is_extended_id=is_extended_id, channel=channel)
        return self.retrieve_message(timeout_s=timeout_s)

    def invoke_capl_func(self, func_name: str, func_params: list[int | float] = []) -> None:
        """
        Method for invoking a CAPL function within the open configuration.

        NOTE:
            start_exe and load_config must be called first and the measurement must be running.

        Args:
            func_name: Exact name of function to invoke.
            func_params: List of numbers to pass as parameters to the function.
        """
        logger.debug(f'Invoking CAPL function {func_name} with parameters {func_params}...')
        # Check if another CAPL function is already running. If it is, wait.
        while self._capl_func_mutex.is_locked():
            pass

        self._capl_func_mutex.lock()

        ret_val = None
        self.__check_method_preconditions("invoke_capl_func")

        # Try to run the CAPL function
        try:
            if len(func_params) <= self.MAX_CAPL_FUNC_PARAMS:
                capl_function = self._capl_func_dict[func_name]
                if capl_function is None:
                    logger.error('CAPL function not yet loaded.')
                    raise Exception('CAPL function not yet loaded, stop and start the simulation once to load it')

                logger.debug('Calling CAPL function...')
                ret_val = capl_function.Call(*func_params)
            else:
                logger.error("Max number of CAPL function params is limited to 10")
                raise Exception("Max number of CAPL function params is limited to 10")

            if ret_val is None:
                logger.error(f"Unable to invoke CAPL function - {func_name}")
                raise Exception(f"Unable to invoke CAPL function - {func_name}")

            logger.debug('Successfully invoked CAPL function.')

        except KeyError:
            logger.error('CAPL function not loaded')
            raise Exception('CAPL function not loaded, please load it before trying to invoke it and stop and start again the simulation') from None

        except Exception as exc:
            logger.error(f'Exception thrown while trying to invoke CAPL function: {exc}')
            raise Exception(f"Exception thrown while trying to invoke CAPL function - {func_name}") from exc

        finally:
            self._capl_func_mutex.unlock()

    def _add_log_block(self, name: str) -> LoggingBlock:
        """
        Method to add a logging block to the measurement setup.

        WARNINGS:
            This method will stop the measurement if it is running before creating the logging block,
            and start it again after creating the logging block if it was running.

        Args:
            name: The path/name of the log file that will be created by the new logging block.

        Returns:
            The created logging block.
        """
        logger.debug(f'Trying to add logging block with name {name}...')

        # Check if the logging block has already been created
        if name in self._logging_collection:
            logger.debug('Logging block with that name was already loaded.')
            return self._logging_collection[name]

        logger.debug('Stopping measurement to add logging block...')
        measurement_running = self.__safe_measurement_stop()

        logger.debug('Adding logging block...')
        log_block = self._logging_collection.add_logging_block(name)

        if measurement_running:
            logger.debug('Restarting measurement...')
            self.start_measurement()

        return log_block

    def _remove_log_block(self, name: str) -> None:
        """
        Method to remove a logging block from the measurement setup.

        WARNINGS:
            This method will stop the measurement if it is running before removing the logging block,
            and start it again after removing the logging block if it was running.

        Args:
            name: The path/name of the log file that is created by the logging block that will be removed.
        """
        logger.debug(f'Trying to remove logging block with name {name}')

        logger.debug('Stopping measurement to remove logging block...')
        measurement_running = self.__safe_measurement_stop()

        logger.debug('Removing logging block...')
        self._logging_collection.remove_logging_block(name)

        if measurement_running:
            logger.debug('Restarting measurement...')
            self.start_measurement()

    def log(
            self,
            log_name: str = None,
            log_write_timeout_s: float = 5,
            log_format: Literal['blf', 'asc', 'csv', 'db', 'log', 'trc'] = 'blf'
            ) -> LogManager:
        """
        Method to get a logging context manager to be used in 'with' blocks.

        NOTE:
            The specified alternatives in the log_format argument are the ones that are supported for reading, CANoe may not
            support them all for writing. The ones that have been tested are: blf and asc.

        Args:
            log_name: The path and name of the log that the LogManager will create in its context, can only be specified explicitly
                in CANoe version 13 or above.
            log_write_timeout_s: How much time the LogManager will wait after the context ends to let CANoe
                write the log into the disk.
            log_format: The format to use when saving the log file.

        Returns:
            The corresponding LogManager object.

        Raises:
            Exception: The path to store the logs could not be created.
        """
        logger.debug('Log manager requested.')
        if log_name is None:  # If no log name provided, reuse the logging block in self
            # Create folder for log
            test_logs_path = self._pytest_data['test_logs_path']
            current_test_module = self._pytest_data['current_sqts_test_module']

            # Find the first relative path available for the current test module in relation to the test paths
            for test_path in self._pytest_data['test_paths']:
                try:
                    log_dir = test_logs_path / current_test_module.relative_to(test_path)
                    break
                except ValueError:
                    if test_path.suffix == '.py':
                        log_dir = test_logs_path
                        break
                    pass
            else:
                logger.error(f'Could not create logs folder for {current_test_module}.')
                raise Exception(f'Could not create logs folder for {current_test_module}.')

            log_dir.mkdir(parents=True, exist_ok=True)

            # Define log name
            log_name = log_dir / (self._pytest_data['current_sqts_test_case'] + '.' + log_format)

            # Return log manager
            return LogManager(self, log_name, log_write_timeout_s, test_log=True)

        if self.version[0] >= 13:  # If the CANoe version is greater or equal to 13, create a new logging block instead of reusing the one in self
            return LogManager(self, log_name, log_write_timeout_s)

        logger.error('Tried to specify explicit name with version 12 or lower.')
        raise Exception('Logging with explicit name is only available starting from version 13')

    def periodic_messages(self, periodic_messages: list[PeriodicMessage]) -> PeriodicMessagesManager:
        """
        Method to get a periodic messages context manager to be used in 'with' blocks.
        When used in a 'with' block it will create a CAPL script to send periodic messages according to the
        passed specifications.

        Args:
            periodic_messages: A list containing the specification of the periodic messages to send.

        Returns:
            A context manager that will create a CAPL script to send the specified periodic messages
            and load it as a node in the simulation.
        """
        logger.debug('Periodic messages manager requested.')
        return PeriodicMessagesManager(self, periodic_messages)

    def clear_write_window(self) -> None:
        """
        Method for clearing the contents of the write window in CANalyzer/CANoe.

        NOTE:
            start_exe must be called before.
        """
        logger.debug('Trying to clear write window...')
        if self._app_com_obj is not None:
            logger.debug("Clearing write window in UI...")
            self._app_com_obj.UI.Write.Clear()
        else:
            logger.debug('No COM interface to CANoe detected.')

    def get_write_window_contents(self) -> str:
        """
        Method for reading the current contents of the write window in CANalyzer/CANoe.

        NOTE:
            start_exe must be called before.

        Returns:
            String containing the write windown contents.
        """
        logger.debug('Getting write window contents...')
        if self._app_com_obj is not None:
            logger.debug("Reading write window contents from UI...")
            return self._app_com_obj.UI.Write.Text
        else:
            logger.debug('No COM interface to CANoe detected.')

    def get_app_version(self) -> str:
        """
        Method for getting the version number of the application (CANalyzer/CANoe).

        NOTE:
            start_exe must be called before.

        Returns:
            String containing the version information.
        """
        logger.debug('Getting app version...')
        if self._app_com_obj is not None:
            logger.debug("Reading CANalyzer/CANoe version...")
            return str(self._app_com_obj.Version)
        else:
            logger.debug('No COM interface to CANoe detected.')

    def __check_method_preconditions(self, method_name: str, test_running: bool = True) -> None:
        """
        Method to check if the preconditions for calling a method are correct.

        Args:
            method_name: The name of the method which conditions are being checked.
            test_running: Whether the measurement is running.
        """
        if self._app_com_obj is None:
            logger.error(f"start exe must be called before calling {method_name}")
            self.__error_clean_up()

        if self._measurement is None:
            logger.error(f"load_config must be called before calling {method_name}")
            self.__error_clean_up()

        if test_running:
            if not self._measurement.Running:
                logger.error(f"Measurement must be started before calling {method_name}")
                self.__error_clean_up()

    def __safe_measurement_stop(self) -> bool:
        """
        Method to safely stop the measurment.

        Returns:
            Whether the measurement was running when this method was called.
        """
        logger.debug('Trying to stop measurement...')
        if self._measurement is not None and self._measurement.Running:
            self._measurement.StopEx()
            while self._measurement.Running:
                # Wait till measurement is stopped
                com_sleep(0.1)
            logger.debug('Measurement stopped successfully')
            return True
        logger.debug('Measurement was already stopped.')
        return False

    def __safe_exe_quit(self) -> None:
        """
        Method to safely exit the application.
        """
        logger.debug(f'Trying to quit {self._can_app}...')
        if self._app_com_obj is not None:
            try:
                logger.debug('Undoing changes to the configuration...')
                self._app_com_obj.Configuration.Modified = False

                logger.debug('Exiting the application...')
                self._app_com_obj.Quit()

                logger.debug(f'Quitted {self._can_app} successfully.')
            except Exception:
                logger.error("Unable to exit Vector executable. Manual closure is required.")
                exit(1)
        else:
            logger.debug(f'{self._can_app} was not open.')

    def __error_clean_up(self) -> None:
        """
        Method to clean up and exit the application after an irrecoverable error.
        """
        logger.debug('Cleaning up after an error...')
        self.__safe_measurement_stop()
        self.__safe_exe_quit()
        exit(1)

    def exit_exe(self) -> None:
        """
        Method for exiting the Vector application.

        NOTE:
            If the exe isn't started, this method is inert.
        """
        logger.debug('Exiting application...')
        self.__safe_measurement_stop()
        if self._app_com_obj is not None:
            logger.info(f"Exiting {self._can_app}...")
            self.__safe_exe_quit()

            # Clean temporary capl scripts created
            logger.debug('Cleaning temporary CAPL scripts created...')
            for f in (THIS_DIR / 'simulation_nodes').glob('sqts_periodic_messages_*'):
                try:
                    f.unlink()
                except PermissionError:
                    logger.warning(f"Couldn't delete file {f}")

            # Clean temporary logs created
            logger.debug('Cleaning temporary log files...')
            for f in (SQTS_DIR / 'logs/canoe_logs').glob('temp_log*.blf'):
                try:
                    f.unlink()
                except PermissionError:
                    logger.warning(f"Couldn't delete file {f}.")
        else:
            logger.debug("Executable not running. No need to exit.")

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/       JIRA AAA-####
#             Initials    Explanation of changes done here.
#    Date        By             Description
# ----------  ---------   -----------------------
# 07/25/2018  Rachel M.   APS-4422  Initial creation
# 07/26/2018  Devin J.    APS-4422  Functional updates
# 09/18/2018  Luke W.     BVH-0008  Updated to throw error if application cannot
#                                   be opened instead of letting the system exit
# 04/08/2019  Luke W.     BVH-0038  Updated to close successfully even if cfg is changed.
# 09/05/2019  Silpa G     APS-58769 Add the check for CANoe in load_config function
#                                   in vector_exe_wrapper
# 08/31/2021  Nestor S.   DZI-1743  Removed check for CANoe in load_config
#                                   function to fix errors when invoking capl
#                                   functions with CANoe
# 11/21/2022  NPresley    FKU-442   Fixed deprecated function usage.
#
# 12/02/2022  Updates from CS 61 to CS 246:
#    to       - Moved unit tests to test_vector.py in Testcases folder.
# 03/17/2023  - Removed unnecessary imports.
#             - Removed use of global variables.
#             - Removed convert_to_byte function and chec_diag_resp method.
#             - Moved _EventHandler class inside VectorExe and changed its name to MeasurementEvents.
#             - Changed use of os.path to pathlib Path.
#             - Added trace_print method to avoid all the 'if self.enable_verbose:' lines.
#             - Added config_file_name property and _config_file attribute to get the name of the
#               currently loaded configuration.
#             - Changed write_can_signal_raw to send_can_signal_raw.
#             - Added send_can_signal_phy method.
#             - Added load_capl_functions method to load the capl functions without having
#               to reload the configuration.
#             - Added send_message method to send arbitrary messages.
#             - Modified invoke_capl_func method to raise exceptions instead of closing the application
#               whenever an error occurred.
#
# 03/24/2023  ABPA        FKU-796   Added change_config method to change the currently loaded configuration
#                                   and added _add_log_block, _remove_log_block and log methods to support
#                                   adding temporary logging blocks to the measurement.
#
# 04/12/2023  ABPA        FKU-848   - Added periodic_messages method.
#                                   - Added exposed_types attribute to give users access to some types
#                                     without having to import the corresponding modules.
#                                   - Changed use of deprecated Stop method in measurement to StopEx.
#                                   - Added a small sleep in loops that check if the measurment is still Running.
#                                   - Added code to remove temporary logs in start_measurement and to remove
#                                     temporary CAPL scripts in exit_exe.
#
# 05/01/2023  ABPA        FKU-838   Added FDX support and changed method for sending a message from invoking
#                                   a CAPL function to setting the value of system variables through FDX
#                                   and having a CAPL event handler build and send the message when those
#                                   system variables change.
#
# 05/04/2023  ABPA        FKU-881   Added receive_message method to get the data from a message that has yet to
#                                   be sent in the CAN bus.
#
# 05/05/2023  ABPA        FKU-883   Changed how the DLC is interpreted and sent to the CAPL script.
#
# 05/11/2023  ABPA        FKU-736   Added trace logging.
#
# 05/22/2023  ABPA        FKU-893   Added __load_fdx and __load_capl methods.
#
# 05/23/2023  ABPA        FKU-918   Added expect_message and retrieve_message methods.
#
# 05/31/2023  ABPA        FKU-926   Added use of Nodes and Networks objects from com_handlers module.
#
# 06/12/2023  ABPA        FKU-888   Added press_key method.
#
# 06/28/2023  ABPA        FKU-975   Added get_diagnostics_device method and changed how send_diag_req works internally.
#
# 06/30/2023  Kushagra G. FKU-854   Added connection to EthernetHandler class for FDX and VectorExe, added
#                                   request_free_running mode for FDX_SEND_PACKET_ID and FDX_RECEIVE_PACKET_ID.
# 07/26/2023  Kushagra G. FKU-1021  Added request_free_running mode for FDX_SEND_ETHERNET_DIAGNOSTIC_ID and exposed types
#                                   for DiagnosticType and ProtocolType
###############################################################################
