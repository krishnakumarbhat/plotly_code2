"""
This module provides classes and functions to serialize and deserialize data according
to the corresponding FDX description files.
"""
from typing import Protocol, Literal
from dataclasses import dataclass
from enum import Enum
import struct
import xml.etree.ElementTree as ET
import logging

logger = logging.getLogger(__name__)


class DataType(Enum):
    """
    Enum representing the possible data types that can be sent
    in a data exchange through FDX.
    """
    INT8 = 'int8'
    UINT8 = 'uint8'
    INT16 = 'int16'
    UINT16 = 'uint16'
    INT32 = 'int32'
    UINT32 = 'uint32'
    INT64 = 'int64'
    UINT64 = 'uint64'
    FLOAT = 'float'
    DOUBLE = 'double'
    STRING = 'string'
    BYTE_ARRAY = 'bytearray'
    FLOAT_ARRAY = 'floatarray'
    DOUBLE_ARRAY = 'doublearray'
    INT32_ARRAY = 'int32array'


def serialize_data(
    value: int | float | str | bytes | list[int] | list[float],
    data_type: DataType
) -> bytes:
    """
    Function to serialize data to send through FDX into the corresponding bytes.

    Args:
        value: The data to serialize.

    Returns:
        The bytes of the serialized data.
    """
    match data_type:
        case DataType.INT8:
            return value.to_bytes(length=1, byteorder='little', signed=True)

        case DataType.UINT8:
            return value.to_bytes(length=1, byteorder='little', signed=False)

        case DataType.INT16:
            return value.to_bytes(length=2, byteorder='little', signed=True)

        case DataType.UINT16:
            return value.to_bytes(length=2, byteorder='little', signed=False)

        case DataType.INT32:
            return value.to_bytes(length=4, byteorder='little', signed=True)

        case DataType.UINT32:
            return value.to_bytes(length=4, byteorder='little', signed=False)

        case DataType.INT64:
            return value.to_bytes(length=8, byteorder='little', signed=True)

        case DataType.UINT64:
            return value.to_bytes(length=8, byteorder='little', signed=False)

        case DataType.FLOAT:
            return struct.pack('<f', value)

        case DataType.DOUBLE:
            return struct.pack('<d', value)

        case DataType.STRING:
            return bytes(value, encoding='ascii') + b'\x00'

        case DataType.BYTE_ARRAY:
            size = len(value).to_bytes(length=4, byteorder='little', signed=False)
            return size + value

        case DataType.FLOAT_ARRAY:
            value = b''.join([struct.pack('<f', x) for x in value])
            size = len(value).to_bytes(length=4, byteorder='little', signed=False)
            return size + value

        case DataType.DOUBLE_ARRAY:
            value = b''.join([struct.pack('<d', x) for x in value])
            size = len(value).to_bytes(length=4, byteorder='little', signed=False)
            return size + value

        case DataType.INT32_ARRAY:
            value = b''.join([i.to_bytes(length=4, byteorder='little', signed=True) for i in value])
            size = len(value).to_bytes(length=4, byteorder='little', signed=False)
            return size + value


def deserialize_data(
    value: bytes,
    data_type: DataType
) -> int | float | str | bytes | list[int] | list[float]:
    """
    Function to deserialize data received through FDX.

    Args:
        The bytes received from FDX that correspond to a particular data group item.

    Returns:
        The object that the data represents.
    """
    match data_type:
        case DataType.INT8 | DataType.INT16 | DataType.INT32 | DataType.INT64:
            return int.from_bytes(value, byteorder='little', signed=True)

        case DataType.UINT8 | DataType.UINT16 | DataType.UINT32 | DataType.UINT64:
            return int.from_bytes(value, byteorder='little', signed=False)

        case DataType.FLOAT:
            return struct.unpack('<f', value)[0]

        case DataType.DOUBLE:
            return struct.unpack('<d', value)[0]

        case DataType.STRING:
            return value.decode()[:-1]

        case DataType.BYTE_ARRAY:
            size = int.from_bytes(value[:4], byteorder='little', signed=False)
            return value[4:4+size]

        case DataType.FLOAT_ARRAY:
            size = int.from_bytes(value[:4], byteorder='little', signed=False)
            value = value[4:4+size]
            windows = [value[4 * i:4 * (i + 1)] for i in range(len(value) // 4)]
            return [struct.unpack('<f', bs)[0] for bs in windows]

        case DataType.DOUBLE_ARRAY:
            size = int.from_bytes(value[:4], byteorder='little', signed=False)
            value = value[4:4+size]
            windows = [value[8 * i:8 * (i + 1)] for i in range(len(value) // 8)]
            return [struct.unpack('<d', bs)[0] for bs in windows]

        case DataType.INT32_ARRAY:
            size = int.from_bytes(value[:4], byteorder='little', signed=False)
            value = value[4:4+size]
            windows = [value[4 * i:4 * (i + 1)] for i in range(len(value) // 4)]
            return [int.from_bytes(bs, byteorder='little', signed=True) for bs in windows]


class CANoeSymbol(Protocol):
    """
    Protocol that describes the interrface provided by a CANoe symbol.
    CANoe symbols are the objects that can be sent through FDX, these include:
    signals, messages, system variables, environment variables, etc.
    """
    symbol_name: str
    value: Literal['raw', 'phys']

    @property
    def symbolic_name(self) -> str:
        ...


@dataclass
class CANoeSignal:
    """
    Class that represents a signal sent through FDX.
    """


@dataclass
class CANoeMessage:
    """
    Class that represents a message sent through FDX.
    """


@dataclass
class CANoePDU:
    """
    Class that represents a PDU sent through FDX.
    """


@dataclass
class CANoeSystemVariable:
    """
    Class that represents a system variable sent through FDX.

    Attributes:
        name: The name of the system variable.
        namespace: The name of the namespace that the system variable belongs to.
        value: How the value of the system variable data should be interpreted.
        unit: The units of the system variable if its data is represented as a physical value.
        symbol_name: The qualified name of the system variable, includes the namespace it belongs to.
    """
    name: str
    namespace: str
    value: Literal['raw', 'phys']
    unit: str

    @property
    def symbol_name(self) -> str:
        return f'{self.namespace}::{self.name}'


@dataclass
class CANoeEnvironmentVariable:
    """
    Class that represents an environment variable sent through FDX.
    """


@dataclass
class CANoeValueEntity:
    """
    Class that represents an value entity sent through FDX.
    """


@dataclass(kw_only=True)
class FDXDataGroupItem:
    """
    Class that represents an FDX data group item.

    Attributes:
        offset: The byte offset that the item's data starts in.
        size: The size in bytes of the item's data.
        data_type: What type of data the bytes of the item's data represent.
        identifier: A string to represent the data group item.
        symbol: The CANoe object that contains the item's data.
    """
    offset: int
    size: int
    data_type: DataType
    identifier: str
    symbol: CANoeSymbol

    def serialize(self, value: int | float | str | bytes | list[int] | list[float]) -> bytes:
        """
        Method to serialize data into bytes that represent the CANoe object of the data group item.

        Args:
            value: The data to serialize.

        Returns:
            The serialized data bytes.
        """
        return serialize_data(value, self.data_type)

    def deserialize(self, value: bytes) -> int | float | str | bytes | list[int] | list[float]:
        """
        Method to deserialize data bytes that represent the CANoe object of the data group item.

        Args:
            value: The bytes to deserialize.

        Returns:
            The deserialized data.
        """
        return deserialize_data(value, self.data_type)


@dataclass(kw_only=True)
class FDXDataGroupDescription:
    """
    Class that represents an FDX data group.

    Attributes:
        id: The id of the data group.
        size: The size in bytes of the data group.
        identifier: A string to identify the data group.
        items: The data group item descriptions contained in the data group.
    """
    id: int
    size: int
    identifier: str = ''
    items: dict[str, FDXDataGroupItem]

    def serialize(self, data: dict) -> bytes:
        """
        Method to serialize data from a data group into bytes for sending in an
        FDX data exchange.

        Args:
            data: A dictionary containing the entries:
                (symbolic name of data group item, data to be sent)

        Returns:
            The serializes data bytes.
        """
        serialized_data = bytearray(self.size)
        for symbol_name, data_value in data.items():
            item = self.items[symbol_name]
            data_bytes = item.serialize(data_value)
            if len(data_bytes) > item.size:
                raise Exception(
                    f'Error serializing FDX data group {self.id}:\n'
                    f'Serialized bytes of {symbol_name} ({data_bytes}) have a size ({len(data_bytes)} bytes) greater than '
                    f'what is specified in the FDX description file ({item.size} bytes)'
                )
            serialized_data[item.offset:item.offset + len(data_bytes)] = data_bytes

        return bytes(serialized_data)

    def deserialize(self, data: bytes) -> dict:
        """
        Method to deserialize data received in an FDX data exchange.

        Args:
            The serialized data bytes.

        Returns:
            A dictionary with the entries (symbolic name of data group item, deserialized data)
        """
        return {
            symbol_name: item.deserialize(data[item.offset:item.offset + item.size])
            for symbol_name, item in self.items.items()
        }


@dataclass(kw_only=True)
class FDXFunction:
    """
    Class that represents an FDX network function.

    Attributes:
        path: The path to the function.
        identifier: A string to identify the function.
    """
    path: str
    identifier: str
    call_id: int
    call_size: int
    return_size: int


@dataclass
class FDXFunctionDescription:
    """
    Class that represents the FDX description of a network function.
    """


def get_data_group_item(node: ET.Element) -> FDXDataGroupItem:
    """
    Function to parse the contents from an FDX description file's item node.

    Args:
        node: The xml node containing the item's data.

    Returns:
        The corresponding FDXDataGroupItem object.
    """
    identifier_node = node[0]
    symbol_node = node[1]

    symbol_type = symbol_node.tag

    match symbol_type:
        case 'sysvar':
            symbol = CANoeSystemVariable(
                name=symbol_node.get('name', ''),
                namespace=symbol_node.get('namespace', ''),
                value=symbol_node.get('value', ''),
                unit=symbol_node.get('unit', '')
            )

        case _:
            logger.error(f'{symbol_type} symbol from FDX description file is not yet implemented')
            raise Exception('Symbol not implemented yet')

    identifier = identifier_node.text if identifier_node.text else ''

    return FDXDataGroupItem(
        offset=int(node.get('offset')),
        size=int(node.get('size')),
        data_type=DataType(node.get('type')),
        identifier=identifier,
        symbol=symbol
    )


def get_data_group_description(node: ET.Element) -> FDXDataGroupDescription:
    """
    Function to parse the contents from an FDX description file's datagroup node.

    Args:
        node: The xml node that contains the data group's data.

    Returns:
        The corresponding FDXDataGroupDescription object.
    """
    identifier_node = node[0]
    item_nodes = node[1:]

    id = int(node.get('groupID'))
    size = int(node.get('size'))
    identifier = identifier_node.text if identifier_node.text else ''

    logger.debug(f'Getting data group items from data group {id} ({identifier})...')

    items = [get_data_group_item(node) for node in item_nodes]

    return FDXDataGroupDescription(
        id=id,
        size=size,
        identifier=identifier,
        items={item.symbol.symbol_name: item for item in items}
    )


def data_group_descriptions_from_file(filename: str) -> dict[int, FDXDataGroupDescription]:
    """
    Function to extract the data group descriptions from an FDX description file.

    Args:
        filename: The filename of the xml FDX description file that contains the data group descriptions.

    Returns:
        A dictionary with entries (id of the data group, data group object).
    """
    logger.debug(f'Getting data group descriptions from {filename}...')
    xml_tree = ET.parse(filename)
    root = xml_tree.getroot()
    data_group_descriptions = [get_data_group_description(dg_node) for dg_node in root]
    return {dg.id: dg for dg in data_group_descriptions}


class FDXDataTranslator:
    """
    Class to translate data from and for data exchanges and function calls.

    Attributes:
        datagroups: A dictionary with entries (id of the data group, data group object).
    """
    def __init__(self, description_files: list[str] = []) -> None:
        """
        Init method for FDXDescriptions.

        Args:
            description_files: Paths to FDX description files to extract descriptions from.
        """
        logger.debug('Creating FDXDataTranslator...')
        self.datagroups: dict[int, FDXDataGroupDescription] = {}
        self.add_description_files(description_files)

    def add_description_files(self, description_files: list[str]) -> None:
        """
        Method to extract data from FDX description files.

        Args:
            description_files: The paths to the FDX description files.
        """
        for description_file in description_files:
            logger.debug(f'Adding FDX description file {description_file} to FDXDataTranslator...')
            self.datagroups.update(data_group_descriptions_from_file(description_file))

    def serialize_exchange_data(self, group_id: int, data: dict) -> bytes:
        """
        Method to serialize data to send in an FDX data exchange according to the
        description files.

        Args:
            group_id: The data group id of the FDX data group description to follow for serialization.
            data: A dictionary containing the data to send. Its entries must have the format
                (symbolic name of the CANoe object, the data for the CANoe object)

        Returns:
            The serialized data bytes to send in an FDX data exchange.
        """
        return self.datagroups[group_id].serialize(data)

    def deserialize_exchange_data(self, group_id: int, data: bytes) -> dict:
        """
        Method to deserialize data received from an FDX data exchange.

        Args:
            group_id: The data group id of the FDX data group description to follow for deserialization.
            data: The bytes of the serialized data.

        Returns:
            A dictionary containing the received data. Its entries have the format
            (symbolic name of the CANoe object, the data of the CANoe object)
        """
        return self.datagroups[group_id].deserialize(data)


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 04/28/2023  ABPA       FKU-838   Initial creation
# 05/11/2023  ABPA       FKU-736   Added trace logging.
