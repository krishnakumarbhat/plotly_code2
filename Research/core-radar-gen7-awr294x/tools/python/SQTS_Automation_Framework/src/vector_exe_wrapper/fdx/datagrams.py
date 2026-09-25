"""
This module contains classes and functions to create and parse FDX datagrams.
More information about the FDX protocol can be found in:

    CANoe > File > Help > CANoe Help > Setup and Extensions > FDX protocol

A sample implementation in C# can be found here:
    https://cdn.vector.com/cms/content/know-how/_application-notes/AN-AND-1-119_Fast_Data_Exchange_with_CANoe.pdf
"""

from enum import IntEnum
from dataclasses import dataclass
from typing import ClassVar, Protocol, NewType, Literal
import logging

logger = logging.getLogger(__name__)

uint8 = NewType('uint8', int)
uint16 = NewType('uint16', int)
uint32 = NewType('uint32', int)
uint64 = NewType('uint64', int)
int64 = NewType('int64', int)


# Constant bytes that are part of the FDX datagram header
SIGNATURE = bytes([0x43, 0x41, 0x4E, 0x6F, 0x65, 0x46, 0x44, 0x58])
PROTOCOL_VERSION_MAJOR = 2
PROTOCOL_VERSION_MINOR = 1
FDX_VERSION = bytes([PROTOCOL_VERSION_MAJOR, PROTOCOL_VERSION_MINOR])
PROTOCOL_FLAGS = b'\x00'


def int_from_bytes(bs: bytes, signed: bool = False) -> int:
    """
    Utility function to convert bytes to an int.

    Args:
        bs: The bytes to convert.
        signed: whether the bytes represent a signed int.

    Returns:
        The resulting int value.
    """
    return int.from_bytes(bs, byteorder='little', signed=signed)


def bytes_from_int(n: int, length: int, signed: bool = False) -> bytes:
    """
    Utility function to convert an int into bytes.

    Args:
        n: The int to convert.
        length: Into how many bytes to convert the int.
        signed: Whether the int is signed.

    Returns:
        The resulting bytes.
    """
    return n.to_bytes(length=length, byteorder='little', signed=signed)


class FDXCommandCode(IntEnum):
    """
    Enum to represent FDX command codes. The codes are represented as unsigned 16 bit integers.
    """
    START = 0x0001
    STOP = 0x0002
    KEY = 0x0003
    STATUS = 0x0004
    DATA_EXCHANGE = 0x0005
    DATA_REQUEST = 0x0006
    DATA_ERROR = 0x0007
    FREE_RUNNING_REQUEST = 0x0008
    FREE_RUNNING_CANCEL = 0x0009
    STATUS_REQUEST = 0x000A
    SEQUENCE_NUMBER_ERROR = 0x000B
    FUNCTION_CALL = 0x000C
    FUNCTION_CALL_ERROR = 0x000D
    INCREMENT_TIME = 0x0011


class FDXCommand(Protocol):
    """
    Protocol that defines the interface required by an FDX command.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: FDXCommandCode

    @property
    def command_size(self) -> uint16:
        ...

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """


@dataclass(kw_only=True)
class StartCommand:
    """
    Command to start the measurement. If the measurement is already running the Start command is ignored.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.START
    command_size: ClassVar[uint16] = 4

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)

        return size + code


@dataclass(kw_only=True)
class StopCommand:
    """
    Command to stop the measurement. If the measurement is not running the Stop command is ignored.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.STOP
    command_size: ClassVar[uint16] = 4

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)

        return size + code


@dataclass(kw_only=True)
class KeyCommand:
    """
    The Key command is sent to CANoe to achieve the effect of pressing a key.
    In CANoe, the On Key handlers in the CAPL programs are then called.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.KEY
    command_size: ClassVar[uint16] = 8
    key_code: uint32

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)
        key_code = bytes_from_int(self.key_code, length=4)

        return size + code + key_code


class StateOfMeasurement(IntEnum):
    """
    NOT_RUNNING: Measurement is not running.
    PRE_START: Start of measurement is prepared.
    RUNNING: Measurement is running.
    STOP: Measurement is stopping.
    """
    NOT_RUNNING = 0x01
    PRE_START = 0x02
    RUNNING = 0x03
    STOP = 0x04


@dataclass(kw_only=True)
class StatusCommand:
    """
    The Status command is sent by CANoe. It contains the current measurement time and status.
    The current time has the value of 0 if there is no measurement running at the moment.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.STATUS
    command_size: ClassVar[uint16] = 16
    measurement_state: StateOfMeasurement
    timestamps: int64

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """


@dataclass(kw_only=True)
class DataExchangeCommand:
    """
    The DataExchange command is used to exchange a group of signals or variables with CANoe. The exchanged
    groups must be defined in an FDX description file.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.DATA_EXCHANGE
    group_id: uint16
    data_bytes: bytes = b''

    @property
    def data_size(self) -> uint16:
        return len(self.data_bytes)

    @property
    def command_size(self) -> uint16:
        return 8 + self.data_size

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)
        group_id = bytes_from_int(self.group_id, length=2)
        data_size = bytes_from_int(self.data_size, length=2)

        return b''.join([
            size,
            code,
            group_id,
            data_size,
            self.data_bytes
        ])


@dataclass(kw_only=True)
class DataRequestCommand:
    """
    The DataRequest command is used to query a group of signal/variable values a single time.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.DATA_REQUEST
    command_size: ClassVar[uint16] = 6
    group_id: uint16

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)
        group_id = bytes_from_int(self.group_id, length=2)

        return size + code + group_id


class DataErrorCode(IntEnum):
    """
    Enum that describes the cause of an exchange error with CANoe. The variants are
    represented by 16 bit unsigned integers.

    Variants:
        MEASUREMENT_NOT_RUNNING: The measurement is not currently running. Data exchange is only possible
            when a measurement is running.
        GROUP_INVALID: The specified GroupID does not exist in any FDX description file.
        DATA_SIZE_TOO_LARGE: The datagram for the specified GroupID exceeds the maximum FDX datagram length.
    """
    MEASUREMENT_NOT_RUNNING = 0x0001
    GROUP_INVALID = 0x0002
    DATA_SIZE_TOO_LARGE = 0x0003


@dataclass(kw_only=True)
class DataErrorCommand:
    """
    The DataExchange command is sent by CANoe in response to a DataRequest that cannot be processed by CANoe.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.DATA_ERROR
    command_size: ClassVar[uint16] = 8
    group_id: uint16
    data_error_code: DataErrorCode

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """


class FreeRunningFlags(IntEnum):
    """
    Flags to specify the behaviour of FreeRunning mode for a data group.
    The variants are represented by 16 bit unsigned integers.

    Variants:
        TRANSMIT_AT_PRE_START: Transmits data group at pre start phase of CANoe measurement.
        TRANSMIT_AT_STOP: Transmit data group at stop of measurement.
        TRANSMIT_CYCLIC: Transmit data cyclically while the measurement is running. The transmit cycle is
            defined by the fields cycle_time and first_duration.
        TRANSMIT_AT_TRIGGER: Transmits the data group if the CAPL function FDXTriggerDataGroup is called.
    """
    TRANSMIT_AT_PRE_START = 0x0001
    TRANSMIT_AT_STOP = 0x0002
    TRANSMIT_CYCLIC = 0x0004
    TRANSMIT_AT_TRIGGER = 0x0008


@dataclass(kw_only=True)
class FreeRunningRequestCommand:
    """
    The FreeRunningRequest command switches FreeRunning mode on for a data group.
    In FreeRunning mode CANoe sends data from a group independently in a cyclic manner
    or whenever a specified trigger is encountered, without having to manually request it.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.FREE_RUNNING_REQUEST
    command_size: ClassVar[uint16] = 16
    group_id: uint16
    flags: FreeRunningFlags
    cycle_time: uint32
    first_duration: uint32

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)
        group_id = bytes_from_int(self.group_id, length=2)
        flags = bytes_from_int(self.flags, length=2)
        cycle_time = bytes_from_int(self.cycle_time, length=4)
        first_duration = bytes_from_int(self.first_duration, length=4)

        return b''.join([
            size,
            code,
            group_id,
            flags,
            cycle_time,
            first_duration
        ])


@dataclass(kw_only=True)
class FreeRunningCancelCommand:
    """
    The FreeRunningCancel command is used to stop FreeRunning mode for a data group.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.FREE_RUNNING_CANCEL
    command_size: ClassVar[uint16] = 6
    group_id: uint16

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)
        group_id = bytes_from_int(self.group_id, length=2)

        return size + code + group_id


@dataclass(kw_only=True)
class StatusRequestCommand:
    """
    The Status Request command is used to request CANoe to transmit a Status command.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.STATUS_REQUEST
    command_size: ClassVar[uint16] = 4

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)

        return size + code


@dataclass(kw_only=True)
class SequenceNumberErrorCommand:
    """
    The Sequence Number Error is sent by CANoe whenever the sequence number in the header of
    the received datagram is incorrect. This is typically an indication for the loss of one or
    more previous datagrams. Only sent when using UDP, TCP has mechanisms to guarantee transmission
    of datagrams.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.SEQUENCE_NUMBER_ERROR
    command_size: ClassVar[uint16] = 8
    received_seq_nr: uint16
    expected_seq_nr: uint16

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """


@dataclass(kw_only=True)
class FunctionCallCommand:
    """
    Network Function Call commands are sent to CANoe in order to invoke a function call on
    a consumer side function or service method port. See FDX manual for more details.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.FUNCTION_CALL
    function_id: uint16
    request_id: uint16
    data_bytes: bytes

    @property
    def data_size(self) -> uint16:
        return len(self.data_bytes)

    @property
    def command_size(self) -> uint16:
        return 10 + self.data_size

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)
        function_id = bytes_from_int(self.function_id, length=2)
        request_id = bytes_from_int(self.request_id, length=2)
        data_size = bytes_from_int(self.data_size, length=2)

        return b''.join([
            size,
            code,
            function_id,
            request_id,
            data_size,
            self.data_bytes
        ])


class FunctionCallErrorCode(IntEnum):
    """
    Enum that represents the possible errors when executing the Function Call command.
    The variants are represented by 16 bit unsigned integers.

    Variants:
        MEASUREMENT_NOT_RUNNING: The measurement is not currently running. Function calls can only
            be invoked when a measurement is running.
        FUNCTION_ID_INVALID: The specified FunctionID does not exist in any active FDX description file.
        DATA_SIZE_TOO_LARGE: The FunctionResponse command for the specified function combined with the
            required datagram header will not fit into a single UDP/TCP datagram.
        PARAMETER_FORMAT: The deserialization of input parameters to the function call has failed because
            the data contains an invalid array length or union discriminator.
        TIMEOUT: The function call was not answered by the server before it timed out. This error is not
            sent as an immediate response to a function call invocation, but if the context of a successfully
            invoked function call is finalized before it got a response.
        MEASUREMENT_STOPPED: The measurement was stopped before a respnse for the function call was sent.
    """
    MEASUREMENT_NOT_RUNNING = 0x0001
    FUNCTION_ID_INVALID = 0x0002
    DATA_SIZE_TOO_LARGE = 0x0003
    PARAMETER_FORMAT = 0x0004
    TIMEOUT = 0x0005
    MEASUREMENT_STOPPED = 0x0006


@dataclass(kw_only=True)
class FunctionCallErrorCommand:
    """
    If any Function Call command was not executed successfully, a Function Call Error
    response is sent by CANoe.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.FUNCTION_CALL_ERROR
    command_size: ClassVar[uint16] = 10
    function_id: uint16
    request_id: uint16
    error_code: FunctionCallErrorCode

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """


@dataclass(kw_only=True)
class IncrementTimeCommand:
    """
    The Increment Time command is sent to CANoe in order to increase the system time during
    the measurement in simulated mode. The command is only applicable when the CANoe measurement
    is running in slave mode.

    Attributes:
        command_code: The FDX command code.
        command_size: The size in bytes of the command content, including code and size fields.
    """
    command_code: ClassVar[FDXCommandCode] = FDXCommandCode.INCREMENT_TIME
    command_size: ClassVar[uint16] = 16
    timestep: uint64

    def as_datagram_bytes(self) -> bytes:
        """
        Method to produce the command bytes to send in a UDP/TCP datagram.

        Returns:
            The byte representation of the command for the datagram.
        """
        size = bytes_from_int(self.command_size, length=2)
        code = bytes_from_int(self.command_code, length=2)
        filler = b'\x00\x00\x00\x00'
        timestep = bytes_from_int(self.timestep, length=8)

        return size + code + filler + timestep


@dataclass
class Datagram:
    """
    Class to represent an FDX datagram.

    Attributes:
        commands: A list of FDX commands to send in the datagram.
        transport_layer: The transport layer used to send the datagram.
    """
    commands: list[FDXCommand]
    transport_layer: Literal['UDP', 'TCP'] = 'UDP'

    def to_bytes(self) -> bytes:
        body = b''.join([command.as_datagram_bytes() for command in self.commands])

        number_of_commands = bytes_from_int(len(self.commands), length=2)
        seq_nr_or_dgram_len = bytes_from_int(16 + len(body), length=2) if self.transport_layer == 'TCP' else b'\x00\x80'

        header = b''.join([
            SIGNATURE,
            FDX_VERSION,
            number_of_commands,
            seq_nr_or_dgram_len,
            PROTOCOL_FLAGS,
            b'\x00'  # Reserved byte
        ])

        return header + body


def get_command_bytes(commands_bytes: bytes, num_of_commands: int) -> list[bytes]:
    """
    Function to separate the command data packed into the FDX datagram into the
    respective bytes for each of the sent commands.

    Args:
        commands_bytes: The FDX datagram bytes, without the datagram header.
        num_of_commands: The number of commands contained in the datagram bytes.

    Returns:
        A list where each element is the bytes corresponding to an individual FDX command.
    """
    split_data = []
    while num_of_commands > 0 and commands_bytes:
        size = int_from_bytes(commands_bytes[:2])
        split_data.append(commands_bytes[2:size])
        commands_bytes = commands_bytes[size:]
        num_of_commands -= 1

    return split_data


def parse_command(command_data: bytes) -> FDXCommand:
    """
    Function to parse the bytes of an FDX command.

    Args:
        command_data: The bytes for an FDX command, excluding the two bytes for the command_size.

    Returns:
        The parsed FDX command.
    """
    logger.debug('Parsing command bytes from FDX datagram...')
    command_code = FDXCommandCode(int_from_bytes(command_data[:2]))
    command_data = command_data[2:]

    logger.debug(f'{command_code.name} FDX command identified')
    match command_code:
        case FDXCommandCode.START:
            return StartCommand()

        case FDXCommandCode.STOP:
            return StopCommand()

        case FDXCommandCode.KEY:
            key_code = int_from_bytes(command_data[:4])
            return KeyCommand(key_code=key_code)

        case FDXCommandCode.STATUS:
            measurement_state = StateOfMeasurement(command_data[0])
            timestamps = int_from_bytes(command_data[4:12], signed=True)
            return StatusCommand(measurement_state=measurement_state, timestamps=timestamps)

        case FDXCommandCode.DATA_EXCHANGE:
            group_id = int_from_bytes(command_data[:2])
            data_bytes = command_data[4:]
            return DataExchangeCommand(group_id=group_id, data_bytes=data_bytes)

        case FDXCommandCode.DATA_REQUEST:
            group_id = int_from_bytes(command_data[:2])
            return DataRequestCommand(group_id=group_id)

        case FDXCommandCode.DATA_ERROR:
            group_id = int_from_bytes(command_data[:2])
            data_error_code = DataErrorCode(int_from_bytes(command_data[2:4]))
            return DataErrorCommand(group_id=group_id, data_error_code=data_error_code)

        case FDXCommandCode.FREE_RUNNING_REQUEST:
            group_id = int_from_bytes(command_data[:2])
            flags = int_from_bytes(command_data[2:4])
            cycle_time = int_from_bytes(command_data[4:8])
            first_duration = int_from_bytes(command_data[8:12])

            return FreeRunningRequestCommand(group_id=group_id, flags=flags, cycle_time=cycle_time, first_duration=first_duration)

        case FDXCommandCode.FREE_RUNNING_CANCEL:
            group_id = int_from_bytes(command_data[:2])
            return FreeRunningCancelCommand(group_id=group_id)

        case FDXCommandCode.STATUS_REQUEST:
            return StatusRequestCommand()

        case FDXCommandCode.SEQUENCE_NUMBER_ERROR:
            received_seq_nr = int_from_bytes(command_data[:2])
            expected_seq_nr = int_from_bytes(command_data[2:4])
            return SequenceNumberErrorCommand(received_seq_nr=received_seq_nr, expected_seq_nr=expected_seq_nr)

        case FDXCommandCode.FUNCTION_CALL:
            function_id = int_from_bytes(command_data[:2])
            request_id = int_from_bytes(command_data[2:4])
            data_bytes = command_data[6:]
            return FunctionCallCommand(function_id=function_id, request_id=request_id, data_bytes=data_bytes)

        case FDXCommandCode.FUNCTION_CALL_ERROR:
            function_id = int_from_bytes(command_data[:2])
            request_id = int_from_bytes(command_data[2:4])
            error_code = FunctionCallErrorCode(int_from_bytes(command_data[4:6]))
            return FunctionCallErrorCommand(function_id=function_id, request_id=request_id, error_code=error_code)

        case FDXCommandCode.INCREMENT_TIME:
            timestep = int_from_bytes(command_data[4:12])
            return IncrementTimeCommand(timestep=timestep)


def parse_datagram_header(datagram_data: bytes) -> dict:
    """
    Function to extract the header data from an FDX datagram.

    Args:
        datagram_data: The bytes of the FDX datagram.

    Returns:
        A dictionary with the extracted header data
    """
    return {
        'signature': datagram_data[:8],
        'major_version': datagram_data[8],
        'minor_version': datagram_data[9],
        'number_of_commands': int_from_bytes(datagram_data[10:12]),
        'seq_nr_or_dgram_len': int_from_bytes(datagram_data[12:14]),
        'protocol_flags': datagram_data[14]
    }


def parse_datagram(datagram_data: bytes) -> dict:
    """
    Function to parse the bytes of an FDX datagram.

    Args:
        datagram_data: The bytes of the FDX datagram.

    Returns:
        A dictionary with the extracted data.
    """
    logger.debug('Parsing FDX datagram...')

    parsed_data = parse_datagram_header(datagram_data)

    number_of_commands = parsed_data['number_of_commands']
    logger.debug(f'{number_of_commands} commands found in datagram.')

    commands_bytes = get_command_bytes(datagram_data[16:], number_of_commands)
    parsed_data['datagram_size'] = sum([len(command_bytes) + 2 for command_bytes in commands_bytes]) + 16

    parsed_commands = [parse_command(command_bytes) for command_bytes in commands_bytes]
    parsed_data['commands'] = parsed_commands

    return parsed_data


def parse_datagrams(datagrams_data: bytes) -> list[FDXCommand]:
    """
    Function to parse the bytes received by a UDP/TCP socket buffer,
    which may contain 1 or more datagrams.

    Args:
        datagrams_data: The bytes from the socket's buffer.

    Returns:
        A list of the parsed FDX commands.
    """
    parsed_commands = []
    while datagrams_data:
        parsed_datagram = parse_datagram(datagrams_data)
        parsed_commands += parsed_datagram['commands']
        datagrams_data = datagrams_data[parsed_datagram['datagram_size']:]

    return parsed_commands


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 04/27/2023  ABPA       FKU-838   Initial creation
# 05/11/2023  ABPA       FKU-736   Added trace logging.
# 05/23/2023  ABPA       FKU-918   Added parse_datagram_header and parse_datagrams methods.
