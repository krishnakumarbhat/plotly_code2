"""
This module contains classes to read from and write to specified addresses with Trace32.
"""
from lauterbach.trace32 import rcl
import array
from typing import Optional, Literal, get_args
import logging

from .address import Address
from .common import Trace32Logger
from .error import MemoryError

logger = logging.getLogger(__name__)

NumberType = Literal['int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64', 'float', 'double']


class MemoryService:
    """
    Class to read from and write to specified addresses with Trace32.
    Wrapper class for rcl.MemoryService.

    NOTE:
        Most of this class' methods are created during runtime due to the great amount
        of logic repetition. To get a full list of all the available methods use the
        help function for this class or a tool that generates documentation based on
        the objects' __doc__ attribute.
    """

    def __init__(self, memory_service: rcl.MemoryService, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for MemoryService class.

        Args:
            memory_service: The rcl.MemoryService instance to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__memory_service = memory_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def read(self, address: Address, *, length: int) -> bytes:
        return self.__memory_service.read(address=address, length=length)


def __add_read_method(value_type: NumberType):
    """
    Internal method used add methods to MemoryService class.
    """
    method_name = f'read_{value_type}'

    def _read_method(self, address: Address, *, byteorder: Optional[Literal['little', 'big']] = None) -> int | float:
        kwargs = {}
        if value_type not in ['int8', 'uint8']:
            kwargs['byteorder'] = byteorder

        try:
            self.logger.debug(f'Reading {value_type} value from address {address}...')
            read_method = getattr(self.__memory_service, method_name)
            value = read_method(address._internal_obj, **kwargs)
            self.logger.debug(f'Read {value}.')
            return value
        except Exception as exc:
            error_msg = f'Could not read {value_type} from address {address}.'
            self.logger.error(error_msg)
            raise MemoryError(error_msg) from exc
    _read_method.__name__ = method_name
    _read_method.__doc__ = f"""
        Method to read one {value_type} value from a specified address.

        Args:
            address: The address to read the value from.
            byteorder: The byteorder used to decode the memory bytes into a {value_type} value.

        Returns:
            The {value_type} value at the specified address.
        """

    setattr(MemoryService, method_name, _read_method)


def __add_read_array_method(value_type: Literal['int8', 'uint8']):
    """
    Internal method used add methods to MemoryService.
    """
    method_name = f'read_{value_type}_array'

    def _read_method(self, address: Address, *, length: int = 1) -> array.array:
        try:
            self.logger.debug(f'Reading {length} {value_type} values from address {address}...')
            read_method = getattr(self.__memory_service, method_name)
            value = read_method(address._internal_obj, lengh=length)
            self.logger.debug(f'Read {value}.')
            return value
        except Exception as exc:
            error_msg = f'Could not read {value_type} values from address {address}.'
            self.logger.error(error_msg)
            raise MemoryError(error_msg) from exc
    _read_method.__name__ = method_name
    _read_method.__doc__ = f"""
        Method to read multiple {value_type} values from the specified address.

        Args:
            address: The address to read the value from.
            length: The number of values to read.

        Returns:
            An array of {value_type} values.
        """

    setattr(MemoryService, method_name, _read_method)


def __add_write_method(value_type: NumberType):
    """
    Internal method used add methods to MemoryService.
    """
    method_name = f'write_{value_type}'

    def _write_method(self, address: Address, value: int | float, *, byteorder: Optional[Literal['little', 'big']] = None) -> None:
        kwargs = {}
        if value_type not in ['int8', 'uint8']:
            kwargs['byteorder'] = byteorder

        try:
            self.logger.debug(f'Writing {value_type} value {value} to address {address}...')
            write_method = getattr(self.__memory_service, method_name)
            write_method(address._internal_obj, value, **kwargs)
        except Exception as exc:
            error_msg = f'Could not write {value} to address {address}.'
            self.logger.error(error_msg)
            raise MemoryError(error_msg) from exc
    _write_method.__name__ = method_name
    _write_method.__doc__ = f"""
        Method to write one {value_type} value to the specified address.

        Args:
            address: The address to read the value from.
            value: The value to write.
            byteorder: The byteorder to use when writing the value in memory.
        """

    setattr(MemoryService, method_name, _write_method)


def __add_write_array_method(value_type: NumberType):
    """
    Internal method used add methods to MemoryService.
    """
    method_name = f'write_{value_type}_array'

    def _write_method(self, address: Address, data: list[int]) -> None:
        try:
            self.logger.debug(f'Writing array of {value_type} values {data} to address {address}...')
            write_method = getattr(self.__memory_service, method_name)
            write_method(address._internal_obj, data)
        except Exception as exc:
            error_msg = f'Could not write {data} to address {address}.'
            self.logger.error(error_msg)
            raise MemoryError(error_msg) from exc
    _write_method.__name__ = method_name
    _write_method.__doc__ = f"""
        Method to write multiple {value_type} values to the specified address.

        Args:
            address: The address to read the value from.
            data: The values to write.
        """

    setattr(MemoryService, method_name, _write_method)


for value_type in get_args(NumberType):
    __add_read_method(value_type)
    __add_write_method(value_type)

for value_type in ['int8', 'uint8']:
    __add_read_array_method(value_type)
    __add_write_array_method(value_type)

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
