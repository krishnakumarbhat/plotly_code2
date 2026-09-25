"""
This module contains classes to deal with adding, removing and using CANoe logging blocks.
"""
from pathlib import Path
import time
from uuid import uuid4
from typing import TypeVar
import logging

from .can_log import CANLog
from ...utils import com_sleep

logger = logging.getLogger(__name__)

VectorExe = TypeVar('VectorExe')
ILogging = TypeVar('ILogging')
ILoggingCollection = TypeVar('ILoggingCollection')

THIS_DIR = Path(__file__).parent
SQTS_DIR = THIS_DIR.parent.parent.parent


class LoggingBlock:
    """
    Class that represents a CANoe Logging block.

    Attributes:
        _com_obj: The COM Dispatch interface to interact with the logging block in CANoe.
        fullname: The full name or path where the log file created by the logging block is.
    """

    def __init__(self, logging_com_obj: ILogging) -> None:
        """
        Init method for LoggingBlock.

        Args:
            logging_com_obj: The COM Dispatch interface to interact with the logging block in CANoe.
        """
        self._com_obj: ILogging = logging_com_obj
        self.fullname: str = self._com_obj.FullName

    @property
    def name(self) -> str:
        """
        File where the log file will be saved.
        """
        return self.fullname

    @name.setter
    def name(self, log_name):
        """
        File where the log file will be saved.
        """
        log_name = str(Path(log_name).absolute())

        logger.debug(f'Changing logging block name from {self.name} to {log_name}')

        self._com_obj.FullName = log_name
        self.fullname = self._com_obj.FullName


class LoggingCollection:
    """
    Class that represents a collection of CANoe Logging blocks.

    Attributes:
        _com_obj: The COM Dispatch interface to interact with the collection of logging blocks in CANoe.
        logging_blocks: A dictionary with the logging blocks of open configuration, the keys are the full names
            of the logging blocks, and the values the respective LoggingBlock objects.
    """

    def __init__(self, logging_collection_com_obj: ILoggingCollection) -> None:
        """
        Init method for LoggingBlock.

        Args:
            logging_collection_com_obj: The COM Dispatch interface to interact with the logging block collecition in CANoe.
        """
        self._com_obj: ILoggingCollection = logging_collection_com_obj
        self.logging_blocks: dict[str, LoggingBlock] = {}
        self._update_logging_blocks()

    def _update_logging_blocks(self) -> None:
        """
        Internal method to update the dictionary of LoggingBlocks each time they are updated in the loaded configuration.
        """
        logger.debug('Updating logging blocks collection')
        self.logging_blocks = {str(Path(logging_block.FullName).absolute()): LoggingBlock(logging_block)
                               for logging_block in self._com_obj}

    def add_logging_block(self, fullname: str) -> LoggingBlock:
        """
        Method to add a logging block to the loaded simulation.

        Args:
            fullname: The full name or path of the log file that will be created by the created logging block.

        Returns:
            The LoggingBlock for the created logging block.
        """
        log_file = Path(fullname)
        if log_file.exists():  # Delete file if it exists
            log_file.unlink()

        abs_path = str(log_file.absolute())
        # Check if it's not present already in the configuration
        if abs_path in self.logging_blocks:
            logger.warning(f'Logging block to be added is already in collection: {abs_path}')
            return self.logging_blocks[abs_path]

        logger.debug(f'Adding logging block with path {abs_path}')
        self._com_obj.Add(abs_path)
        self._update_logging_blocks()
        return self.logging_blocks[abs_path]

    def remove_logging_block(self, name: str) -> None:
        """
        Method to remove a logging block from the loaded simulation.

        Args:
            name: A part of the name or path of the log file that is created by the logging block that will be removed.
        """
        name = str(Path(name).absolute())

        logger.debug(f'Removing logging block {name}')
        for i, log_block in enumerate(self._com_obj, start=1):
            if name == str(Path(log_block.FullName).absolute()):
                self._com_obj.Remove(i)
                break
        self._update_logging_blocks()

    def __contains__(self, logging_block_name: str) -> bool:
        """
        Overloading of contains method to facilitate checking if a logging block is present in the loaded configuration.

        Args:
            logging_block_name: The path to the log file that is created by the logging block if it is in the loaded configuration

        Returns:
            Whether the logging block is already in the loaded configuration or not.
        """
        abs_path = str(Path(logging_block_name).absolute())
        return abs_path in self.logging_blocks

    def __getitem__(self, logging_block_name: str) -> LoggingBlock:
        """
        Overloading of getitem method to facilitate gettint a logging block present in the loaded configuration.

        Args:
            logging_block_name: The path to the log file that is created by the logging block.

        Returns:
            The corresponding LoggingBlock object.
        """
        abs_path = str(Path(logging_block_name).absolute())
        return self.logging_blocks[abs_path]


class LogManager:
    """
    Class to use in context managers to manage CANoe log blocks.

    Attributes:
        log_name: The name of the log that will be created for the context where the LogManager is used.
        log_write_timeout_s: How much time the LogManager will wait after the context ends to let CANoe
            write the log into the disk.
        _vector_app: The VectorExe object to use for adding and removing the logging block.
        _log_block: The logging block created by LogManager.
        _measurement_was_running: A flag that indicates whether the measurement was running when a context is entered.
        _log_path: The path to the log file.
        _test_log: Flag that indicates whether to create a new logging block and remove it afterwards (False) or use the
            test_log in VectorExe (True).
    """

    def __init__(self, vector_app: VectorExe, log_name: str, log_write_timeout_s: float = 5, *, test_log: bool = False) -> None:
        """
        Init method for LogManager.

        Args:
            vector_app: The VectorExe object to use for adding and removing the logging block.
            log_name: The name of the log that will be created for the context where the LogManager is used.
            log_write_timeout_s: How much time the LogManager will wait after the context ends to let CANoe
                write the log into the disk.
            test_log: Whether to create a new logging block and remove it afterwards (False) or use the
                test_log in VectorExe (True).
        """
        self.log_write_timeout_s: float = log_write_timeout_s
        self._test_log: bool = test_log
        self._log_path: Path = Path(log_name).absolute()
        self._vector_app: VectorExe = vector_app
        self._log_block: LoggingBlock = None
        self._measurement_was_running: bool = False

    @property
    def log_name(self) -> str:
        """
        Property representing the path of the log block.
        """
        return str(self._log_path)

    def __enter__(self) -> 'LogManager':
        """
        Enter method for LogManager, this method will be called before the statements inside the 'with' block.

        Returns:
            self
        """
        logger.debug(f'Entering context management block for log {self.log_name}...')
        self._in_context_management_block = True
        self._measurement_was_running = self._vector_app.measurement_running

        # Stop measurement to be able to add or modify logging blocks
        logger.debug('Stopping measurement to configure log...')
        self._vector_app.stop_measurement()

        # Remove the log file if it already exists to avoid pop-ups asking if you want to overwrite the log file.
        logger.debug('Checking if log file already exists...')
        self.remove_log_file()

        # Add logging block or reuse the one in self._vector_app by changing its name
        if self._test_log:
            logger.debug('Using test log block for logging')
            if self._vector_app._test_log_block is None:  # If there is no logging block in _vector_app
                logger.debug('Test log block not found, creating a new one...')
                self._vector_app._test_log_block = self._vector_app._logging_collection.add_logging_block(self._log_path)
            self._log_block = self._vector_app._test_log_block
            self._log_block.name = self._log_path
        else:
            logger.debug('Using new log block for logging')
            self._log_block = self._vector_app._add_log_block(self.log_name)

        # Start measurement again
        com_sleep(3)
        logger.debug('Restarting measurement to log...')
        self._vector_app.start_measurement()
        return self

    def __exit__(self, exc_type, exc_value, exc_tb) -> None:
        """
        Exit method for LogManager, this method will be called after the 'with' block is finished.

        Args:
            exc_type: If an exception happens in the 'with' block, this argument contains the type of the exception type.
            exc_value: If an exception happens in the 'with' block, this argument contains the exception object.
            exc_tb: If an exception happens in the 'with' block, this argument contains the exception traceback.
        """
        logger.debug('Exiting context management block for logging...')
        # Stop measurement to allow CANoe to write the log file and to modify the logging blocks
        self._vector_app.stop_measurement()
        # Give CANoe time to write the log
        com_sleep(self.log_write_timeout_s)

        if self._test_log:  # If the vector_app logging block was used
            # Change its name to avoid conflicts with the log file just created
            new_log_name = f'temp_log_{uuid4()}.blf'
            new_log_path = str(SQTS_DIR / fr'logs\canoe_logs\{new_log_name}')

            logger.debug(f'Changing test log block name to {new_log_name}...')

            self._log_block.name = new_log_path
        else:
            logger.debug(f'Removing created log block {self.log_name}...')
            self._vector_app._remove_log_block(self.log_name)

        self._in_context_management_block = False

        # Resume measurement if it was running before entering the 'with' block
        if self._measurement_was_running:
            logger.debug('Resuming measurement after logging...')
            self._vector_app.start_measurement()

    def get_contents(self, timeout_s: float = 10) -> CANLog:
        """
        Method to get the parsed contents of the log created by the logging block.

        Args:
            timeout_s: Timeout in seconds to wait for CANoe to write the log file, in case the measurement was just
                stopped when this method was called.

        Returns:
            The CANLog object that contains the contents of the log created by the logging block.
        """
        if self._in_context_management_block:
            logger.error('Tried to get the log contents while in the context management block')
            raise Exception('Cannot get contents from log within the context management block')

        start = time.time()

        log_file_path = self._log_path
        # Check that the logging file exists
        if not log_file_path.exists():
            logger.error(f'Tried to get contents from log file {self.log_name}, file does not exist')
            raise Exception(f'Cannot get contents from logging block {self.log_name}, file does not exist')

        # Check that the logging file has data in it
        while time.time() - start < timeout_s:
            if log_file_path.stat().st_size > 0:
                # Change file permissions to read it
                log_file_path.chmod(666)

                logger.debug('Getting log contents...')
                return CANLog.from_file(self.log_name)

        logger.error('Timed out waiting for log file to have a size greater than 0')
        raise Exception(f'Cannot get contents from logging block {self.log_name}, file has size 0, it may still be in use by CANoe')

    def remove_log_file(self) -> None:
        """
        Method to remove the log created in the context where the LogManager object was used.
        """
        if self._log_path.exists():
            logger.debug(f'Removig log file {str(self._log_path)}...')
            # Change file permissions
            self._log_path.chmod(666)
            # Delete file
            self._log_path.unlink(missing_ok=True)
            logger.debug('Removed log file.')

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 03/21/2023  ABPA       FKU-796   Initial creation
# 05/11/2023  ABPA       FKU-736   Added trace logging.
