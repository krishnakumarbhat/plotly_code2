"""
This module contains classes to query symbol information from Trace32.
"""
from lauterbach.trace32 import rcl
import logging

from .common import Trace32Logger
from .error import SymbolError
from .address import Address

logger = logging.getLogger(__name__)


class Symbol:
    """
    Class that contains the information of a symbol.
    Wrapper class for rcl.Symbol.

    Attributes:
        name: The name of the symbol.
        address: The address where the symbol is located.
        path: The path to the symbol, if there is any.
        size: The size of the symbol in memory.
        start_addr: The starting address of the symbol.
        length: The size of the symbol in memory.
    """

    def __init__(self, symbol: rcl.Symbol) -> None:
        """
        Init method for Symbol class.

        Args:
            symbol: The rcl.Symbol object to wrap.
        """
        self.__symbol = symbol

    def __str__(self) -> str:
        """
        Operator overloading of str method.

        Returns:
            The string representation of a symbol.
        """
        return str(self.__symbol)

    @property
    def name(self) -> str:
        """
        The name of the symbol.
        """
        return self.__symbol.name

    @property
    def address(self) -> Address:
        """
        The address where the symbol is located.
        """
        return Address(self.__symbol.address)

    @property
    def path(self) -> str:
        """
        The path to the symbol, if there is any.
        """
        return self.__symbol.path

    @property
    def size(self) -> int:
        """
        The size of the symbol in memory.
        """
        return self.__symbol.size

    @property
    def start_addr(self) -> int:
        """
        The starting address of the symbol.
        """
        return self.address.value

    @property
    def length(self) -> int:
        """
        The size of the symbol in memory.
        """
        return self.size


class SymbolService:
    """
    Class to query symbol information from Trace32.
    Wrapper class for rcl.SymbolService.
    """

    def __init__(self, symbol_service: rcl.SymbolService, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for SymbolService class.

        Args:
            symbol_service: The rcl.SymbolService instance to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__symbol_service = symbol_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def query_by_address(self, address: Address) -> Symbol:
        """
        Method to search a symbol by address.

        Args:
            address: Address where the symbol is located.

        Returns:
            The queried Symbol.
        """
        try:
            self.logger.debug(f'Getting symbol with address {address}...')
            symbol = self.__symbol_service.query_by_address(address._internal_obj)
            self.logger.debug(f'Got symbol {symbol}.')
            return Symbol(symbol)
        except Exception as exc:
            error_msg = f'Could not get symbol with address {address}.'
            self.logger.error(error_msg)
            raise SymbolError(error_msg) from exc

    def query_by_name(self, name: str) -> Symbol:
        """
        Method to search a symbol by name.

        Args:
            name: Name to use for searching the symbol.

        Returns:
            The queried Symbol.
        """
        try:
            self.logger.debug(f'Getting symbol with name {name}...')
            symbol = self.__symbol_service.query_by_name(name)
            self.logger.debug(f'Got symbol {symbol}.')
            return Symbol(symbol)
        except Exception as exc:
            error_msg = f'Could not get symbol with name {name}.'
            self.logger.error(error_msg)
            raise SymbolError(error_msg) from exc

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
