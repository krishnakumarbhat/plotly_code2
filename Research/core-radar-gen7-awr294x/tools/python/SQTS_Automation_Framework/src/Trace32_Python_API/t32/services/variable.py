"""
This module contains classes to read from and write to variables in Trace32.
"""
from lauterbach.trace32 import rcl
import logging

from .common import Trace32Logger
from .error import VariableError

logger = logging.getLogger(__name__)


class Variable:
    """
    Class that contains the data of a Trace32 variable.
    Wrapper class for rcl.Variable.

    Attributes:
        name: The name of the variable.
        value: The value of the variable.
    """

    def __init__(self, variable: rcl.Variable, _logger: logging.LoggerAdapter):
        """
        Init method for Variable class.

        Args:
            variable: The rcl.Variable object to wrap.
            _logger: A logger adapter to use for logging purposes.
        """
        self.__variable = variable
        self.logger = _logger

    def __str__(self) -> str:
        """
        Operator overloading for str.

        Returns:
            The string representation of a Variable.
        """
        return str(self.__variable)

    @property
    def name(self) -> str:
        """
        The name of the variable.
        """
        return self.__variable.name

    @property
    def value(self) -> int | float:
        """
        The value of the variable.
        """
        return self.__variable.value

    @value.setter
    def value(self, value: int | float) -> None:
        """
        Setter for value attribute.
        """
        self.__variable.value = value

    def read(self) -> None:
        """
        Method to update the read values.
        """
        try:
            self.logger.debug(f'Reading variable {self}...')
            self.__variable.read()
            self.logger.debug(f'Updated variable value: {self}.')
        except Exception as exc:
            error_msg = f'Could not read values from register {self.name}.'
            self.logger.error(error_msg)
            raise VariableError(error_msg) from exc

    def write(self) -> None:
        """
        Method to write the current value in Trace32.
        """
        try:
            self.logger.debug(f'Updating variable {self} with current values...')
            self.__variable.write()
        except Exception as exc:
            error_msg = f'Could not update values for variable {self.name}.'
            self.logger.error(error_msg)
            raise VariableError(error_msg) from exc


class VariableService:
    """
    Class to read from and write to variables in Trace32.
    Wrapper class for rcl.VariableService.
    """

    def __init__(self, variable_service: rcl.VariableService, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for VariableService class.

        Args:
            variable_service: The rcl.VariableService instance to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__variable_service = variable_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def read(self, name: str) -> Variable:
        """
        Method to read a Variable from the debugger.

        Args:
            name: Name of the desired Variable.

        Returns:
            The read Variable.
        """
        try:
            self.logger.debug(f'Reading variable {name}...')
            var = self.__variable_service.read(name)
            self.logger.debug(f'Got variable {var}.')
            return Variable(var, self.logger)
        except Exception as exc:
            error_msg = f'Could not read variable {name}.'
            self.logger.error(error_msg)
            raise VariableError(error_msg) from exc

    def write(self, name: str, value: int | float) -> None:
        """
        Method to write to a variable.

        Args:
            name: Name of the Variable that should be written.
            value: Value that should be written.
        """
        try:
            self.logger.debug(f'Writing {value} to variable {name}...')
            self.__variable_service.write(name, value)
        except Exception as exc:
            error_msg = f'Could not write to variable {name}.'
            self.logger.error(error_msg)
            raise VariableError(error_msg) from exc

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
