"""
This module contains classes to facilitate interaction with CANoe through the FDX interface.
More information about the FDX protocol can be found in:

    CANoe > File > Help > CANoe Help > Setup and Extensions > FDX protocol
"""

import socket
import types
from typing import Literal, Optional
from collections import defaultdict
import time
import logging
import pythoncom

from . import datagrams as dg
from .description_files import FDXDataTranslator

logger = logging.getLogger(__name__)


class DataExchangeError(Exception):
    """An error occured with a data exhange"""

    def __init__(self, group_id: int, error_code: dg.DataErrorCode) -> None:
        """
        Init method for DataExchangeError.

        Args:
            group_id: The id of the requested data group that raised an error.
            error_code: The error code of the issue that arised with the data exchange request.
        """
        error_message = f'Error getting requested exchange data from FDX for data group {group_id}:\n'
        match error_code:
            case dg.DataErrorCode.MEASUREMENT_NOT_RUNNING:
                error_message += 'The measurement is not currently running. Data exchange is only possible when a measurement is running.'

            case dg.DataErrorCode.GROUP_INVALID:
                error_message += 'The specified data group id does not exist in any FDX description file.'

            case dg.DataErrorCode.DATA_SIZE_TOO_LARGE:
                error_message += 'The datagram for the specified group id exceeds the maximum FDX datagram length.'

        super().__init__(error_message)


class CommandResponseError(Exception):
    """The FDX response did not contain the expected command"""


class FDXInterface:
    """
    Class to send commands and receive responses from CANoe through FDX.

    Attributes:
        _connection: The socket connection used to communicate with CANoe.
        _descriptions: A translator object used to serialize and deserialize exchange data.
    """

    class commands:
        """
        Class to expose FDX commands for external use.
        """
        StartCommand = dg.StartCommand
        StopCommand = dg.StopCommand
        DataExchangeCommand = dg.DataExchangeCommand
        StatusRequestCommand = dg.StatusRequestCommand

    def __init__(
            self, *,
            host: str = '127.0.0.1',
            port: int = 2809,
            transport_layer: Literal['UDP', 'TCP'] = 'UDP',
            recv_timeout_s: float = 1.0,
            description_files: list[str] = []
    ) -> None:
        """
        Init method for FDXInterface.

        Args:
            host: The ethernet host address to use for communication with CANoe, defaults to localhost.
            port: The port to use for communication with CANoe, defaults to 2809, the CANoe default.
            transport_layer: The layer to use for communication with CANoe. TCP is recommended to ensure
                that no packets are dropped and that they are read in the correct order, defaults to UDP
                because that is the CANoe default.
            recv_timeout_s: Timeout to wait for a response from CANoe to arrive.
            description_files: A list of paths to description files to be used for data exchange translation.
        """
        logger.debug('Creating FDXInterface...')

        self._datagram_max_size = 65507 if transport_layer == 'UDP' else 65535
        self._transport_layer = transport_layer

        logger.debug(f'Opening {transport_layer} connection...')

        connection = socket.socket(
            socket.AF_INET,
            socket.SOCK_DGRAM if transport_layer == 'UDP' else socket.SOCK_STREAM
            )

        logger.debug(f'Setting connection receive timeout to {recv_timeout_s}s')
        connection.settimeout(recv_timeout_s)

        logger.debug(f'Connecting to IP {host}, port {port}...')
        connection.connect((host, port))

        self._connection: socket.socket = connection

        self._descriptions: FDXDataTranslator = FDXDataTranslator(description_files)

        self._data_exchange_buffer = defaultdict(list)

    def __del__(self) -> None:
        """
        Destructor method for FDXInterface.
        """
        self._connection.close()

    def add_description_files(self, description_files: list[str]) -> None:
        """
        Method to add description files for data exchange translation.

        Args:
            description_files: A list with the paths to the fdx description files.
        """
        logger.debug('Adding description files to FDXInterface...')
        self._descriptions.add_description_files(description_files)

    def send_commands(self, commands: list[dg.FDXCommand]) -> None:
        """
        Method to send FDX commands to CANoe.

        Args:
            commands: A list of commands to send.
        """
        self._connection.sendall(dg.Datagram(commands, transport_layer=self._transport_layer).to_bytes())

    def send_command(self, command: dg.FDXCommand) -> None:
        """
        Method to send a single FDX command to CANoe.

        Args:
            command: The FDX command to send.
        """
        logger.debug(f'Sending FDX {command.command_code.name} command...')
        self.send_commands([command])

    def get_response(self, timeout_s: float = 1.0) -> list[dg.FDXCommand]:
        """
        Method to get a response from CANoe to a previously sent FDX command.

        Args:
            timeout_s: Timeout to wait for a response from CANoe.

        Returns:
            A list with FDX commands received from CANoe.
        """
        logger.debug('Wating for FDX response from CANoe...')
        start = time.time()
        while time.time() - start < timeout_s:
            try:
                response = self._connection.recv(self._datagram_max_size)

                logger.debug(f'FDX response received after {time.time()-start}s.')

                received_commands = dg.parse_datagrams(response)
                return received_commands
            except TimeoutError:
                pythoncom.PumpWaitingMessages()  # Pump messages to keep the COM connection from blocking

        logger.error('Timeout expired waiting for an FDX packet from CANoe.')
        raise TimeoutError('Timeout expired waiting for an FDX packet from CANoe.')

    def start_measurement(self) -> None:
        """
        Method to start the CANoe measurement.
        """
        logger.debug('Starting measurement through FDX...')
        self.send_command(dg.StartCommand())

    def stop_measurement(self) -> None:
        """
        Method to stop the CANoe measurement.
        """
        logger.debug('Stopping measurement through FDX...')
        self.send_command(dg.StopCommand())

    def get_status(self) -> dg.StateOfMeasurement:
        """
        Method to get the current status of the CANoe measurement.

        Returns:
            The current state of the measurement.
        """
        logger.debug('Getting measurement status...')
        self.send_command(dg.StatusRequestCommand())
        response = self.get_response()
        for command in response:
            if isinstance(command, dg.StatusCommand):
                logger.debug(f'Current measurement status is {command.measurement_state.name}')
                return command.measurement_state

        logger.error(f'Got a different response from CANoe than the one expected. Expected Status, received {response}')
        raise CommandResponseError('Status')

    def send_data(self, data: dict, group_id: int) -> None:
        """
        Method to send data to CANoe.

        Args:
            data: A dictionary with the data to send.
            group_id: The id of the FDX data group to which the data belongs.
        """
        serialized_data = self._descriptions.serialize_exchange_data(
            group_id=group_id,
            data=data
        )

        logger.debug(f'Sending FDX data to CANoe, data group ID: {group_id}, data: {data}')
        self.send_command(dg.DataExchangeCommand(group_id=group_id, data_bytes=serialized_data))

    def get_exchange_data_from_buffer(self, group_id: int) -> Optional[dict]:
        """
        Method to get a data exchange command or a data error command for a data group
        from the data exchange buffer if present there.

        Args:
            group_id: The ID of the data group received in a data exchange.

        Returns:
            The data exchange commaand for the requested data group if present in the buffer.
        """
        # Namespace used for match
        ns = types.SimpleNamespace()
        ns.group_id = group_id

        logger.debug(f'Checking Data Exchange buffer for data group {group_id}')
        if self._data_exchange_buffer[group_id]:  # At least one element with matching group_id in the buffer
            logger.debug(f'Entry for {group_id} data group found in buffer.')
            command = self._data_exchange_buffer[group_id].pop(0)
            match command:
                case dg.DataExchangeCommand(group_id=ns.group_id):
                    data = self._descriptions.deserialize_exchange_data(group_id=group_id, data=command.data_bytes)
                    logger.debug(f'Got data: {data}')
                    return data

                case dg.DataErrorCommand(group_id=ns.group_id):
                    logger.error(f'Receive Data Exchange Error from CANoe: {command.data_error_code}')
                    raise DataExchangeError(group_id=group_id, error_code=command.data_error_code)

                case other:
                    logger.error(f'Unrecognized command entry in buffer for data group {group_id}: {other}.')
                    raise CommandResponseError(f'Unrecognized command entry in buffer for data group {group_id}: {other}.')

        return None

    def receive_data(self, group_id: int, timeout_s: float = 1.0) -> dict:
        """
        Method to receive data from a specified data group from CANoe.

        Args:
            group_id: The group id of the expected data group.
            timeout_s: How many seconds should the method try for before raising a TimeoutError exception.

        Returns:
            The parsed data from the received data group.

        Raises:
            DataExchangeError: There was an error with the requested data.
            TimeoutError: The timeout expired while waiting for the data.
        """
        # Check if data is already in buffer
        command_in_buffer = self.get_exchange_data_from_buffer(group_id)
        if command_in_buffer is not None:
            return command_in_buffer

        # Namespace used for match
        ns = types.SimpleNamespace()
        ns.group_id = group_id

        received_command = None

        # Wait for a response from CANoe
        logger.debug(f'Expecting FDX data from data group {group_id}...')
        start = time.time()
        elapsed = 0
        while elapsed < timeout_s:
            response_commands = self.get_response(timeout_s-elapsed)
            elapsed = time.time() - start
            for command in response_commands:
                match command:
                    # Expected command was contained in the response
                    case dg.DataExchangeCommand(group_id=ns.group_id) | dg.DataErrorCommand(group_id=ns.group_id):
                        logger.debug(f'Got data exchange response for expected data group {group_id}.')
                        if received_command is not None:
                            logger.warning(f'A data exchange response for data group {group_id} had already been received previously, overwriting it...')
                        received_command = command

                    # A data exchange response for a different data group was received
                    case dg.DataExchangeCommand(group_id=gid) | dg.DataErrorCommand(group_id=gid):
                        logger.debug(f'Got response for data group {gid}, saving it to buffer...')
                        self._data_exchange_buffer[gid].append(command)

                    case other_command:
                        logger.debug(f'Found {other_command.command_code.name} command in response.')

            # Check if expected command was contained in the response
            match received_command:
                case dg.DataExchangeCommand():
                    data = self._descriptions.deserialize_exchange_data(group_id=group_id, data=received_command.data_bytes)
                    logger.debug(f'Got data after {elapsed}s: {data}')
                    return data

                case dg.DataErrorCommand():
                    logger.error(f'Received Data Exchange Error from CANoe: {received_command.data_error_code}')
                    raise DataExchangeError(group_id=group_id, error_code=received_command.data_error_code)

        logger.error('Timeout expired waiting for FDX data')
        raise TimeoutError('Timeout expired waiting for FDX data')

    def request_data(self, group_id: int, timeout_s: float = 1.0) -> dict:
        """
        Method to get data from CANoe.

        Args:
            group_id: The group id of the requested data.

        Returns:
            A dictionary with the received data.
        """
        logger.debug(f'Requesting FDX data group {group_id} from CANoe...')
        self.send_command(dg.DataRequestCommand(group_id=group_id))
        return self.receive_data(group_id, timeout_s)

    def request_free_running_mode(self, group_id: int, flags: dg.FreeRunningFlags = dg.FreeRunningFlags.TRANSMIT_AT_TRIGGER) -> None:
        """
        Method to request free running mode for a specified data group.

        Args:
            group_id: The ID of the data group that the free running mode is requested for.
            flags: The flags to use then requesting free running mode.
        """
        logger.debug(f'Requesting free running mode for data group {group_id} with flags 0b{flags:b}...')
        self.send_command(dg.FreeRunningRequestCommand(group_id=group_id, flags=flags, cycle_time=0, first_duration=0))

    def press_key(self, key: str) -> None:
        """
        Method to send a KeyCommand through FDX.

        Args:
            key: The key to send in the command.
        """
        if len(key) != 1:
            logger.error(f'Tried to send key with incorrect length: {key}.')
            raise ValueError('Length of key string should be 1')

        if not key.isalnum():
            logger.error(f'Tried to send invalid key: {key}.')
            raise ValueError('Key should be an alphanumeric character')

        self.send_command(dg.KeyCommand(key_code=ord(key)))


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 05/01/2023  ABPA       FKU-838   Initial creation
# 05/11/2023  ABPA       FKU-736   Added trace logging.
# 05/23/2023  ABPA       FKU-918   Added get_exchange_data_from_buffer method and modified receive_data
#                                  to store unused received data exchange commands into a buffer.
# 06/12/2023  ABPA       FKU-888   Added press_key method.
