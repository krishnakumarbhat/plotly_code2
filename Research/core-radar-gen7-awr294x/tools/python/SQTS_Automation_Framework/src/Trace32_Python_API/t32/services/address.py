"""
This module contains classes to handle Trace32 addresses.
"""
from lauterbach.trace32 import rcl
import logging
from typing import Optional

from .common import Trace32Logger
from .error import AddressError

logger = logging.getLogger(__name__)


class Address:
    """
    Class to represent a memory address. This is a wrapper class
    for the rcl.Address class.

    Attributes:
        access: The type of access to the memory address.
        value: The numerical address.
    """

    def __init__(self, address: rcl.Address) -> None:
        """
        Init method for Address class.

        Args:
            address: The rcl.Address object to wrap.
        """
        self.__address = address

    def __str__(self) -> str:
        """
        Operator overloading for Address class to convert it
        to a string.

        Returns:
            The string representation of the Address object.
        """
        return str(self.__address)

    @property
    def access(self) -> str:
        """
        Type of access to the memory address.
        """
        return self.__address.access

    @property
    def value(self) -> int | None:
        """
        Numerical value of the address.
        """
        return self.__address.value

    def to_dualport(self) -> 'Address':
        """
        Method to convert Address to dualport.

        Returns:
            The converted address.
        """
        return Address(self.__address.to_dualport())

    @property
    def _internal_obj(self) -> rcl.Address:
        """
        Wrapped internal rcl.Address object.
        """
        return self.__address


class AddressService:
    """
    This class provides utility functions to get access to memory addresses.
    Wrapper class for rcl.AddressService class.
    """

    def __init__(self, address_service: rcl.AddressService, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for AddressService class.

        Args:
            address_service: The rcl.AddressService object to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__address_service = address_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def __call__(self, access: Optional[str] = None, value: Optional[int] = None) -> Address:
        """
        Operator overloading for calling AddressService instances.

        Args:
            access: The type of access to the memory address.
            value: The numerical address.

        Returns:
            An Address object with the specified parameters.
        """
        try:
            self.logger.debug(f'Getting address with access {access} and value {value}...')
            address = self.__address_service(access=access, value=value)
            return Address(address)
        except Exception as exc:
            error_msg = f'Could not get address with access type {access} and value {value}.'
            self.logger.error(error_msg)
            raise AddressError(error_msg) from exc

    def from_string(self, string: str) -> Address:
        """
        Get an Address object from string.

        Args:
            string: String representing the Address.

        Returns:
            The generated Address object.
        """
        try:
            self.logger.debug(f'Getting address from string {string}...')
            address = self.__address_service.from_string(string)
            return Address(address)
        except Exception as exc:
            error_msg = f'Could not get address from string {string}.'
            self.logger.error(error_msg)
            raise AddressError(error_msg) from exc

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
