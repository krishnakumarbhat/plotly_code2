"""
This module contains classes to interact with practice macros in Trace32.
"""
from lauterbach.trace32 import rcl
import logging

from .common import Trace32Logger
from .error import PracticeError

logger = logging.getLogger(__name__)


class PracticeMacro:
    """
    Class that contains information about a practice macro.
    Wrapper class for rcl.PracticeMacro.

    Attributes:
        name: The name of the macro.
        value: The value of the macro.
    """

    def __init__(self, practice_macro: rcl.PracticeMacro) -> None:
        """
        Init method for PracticeMacro class.

        Args:
            practice_macro: The rcl.PracticeMacro instance to wrap.
        """
        self.__practice_macro = practice_macro

    def __str__(self) -> str:
        """
        Operator overloading for the str method.

        Returns:
            The string representation of the macro.
        """
        return str(self.__practice_macro)

    @property
    def name(self) -> str:
        """
        The name of the macro.
        """
        return self.__practice_macro.name

    @property
    def value(self):
        """
        The value of the macro.
        """
        return self.__practice_macro.value


class PracticeService:
    """
    Class to get and set practice macros in Trace32.
    Wrapper class for rcl.PracticeService.
    """

    def __init__(self, practice_service: rcl.PracticeService, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for PracticeService class.

        Args:
            practice_server: The rcl.PracticeServer instance to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__practice_service = practice_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def get_macro(self, name: str) -> PracticeMacro:
        """
        Method to get a (global) PRACTICE macro.

        Args:
            name: Name of the Macro.

        Returns:
            The practice macro.
        """
        try:
            self.logger.debug(f'Reading practice macro {name}...')
            macro = self.__practice_service.get_macro(name)
            self.logger.debug(f'Read macro {name}: {macro}.')
            return PracticeMacro(macro)
        except Exception as exc:
            error_msg = f'Could not read practice macro {name}.'
            self.logger.error(error_msg)
            raise PracticeError(error_msg) from exc

    def set_macro(self, name: str, value: str) -> PracticeMacro:
        """
        Method to set a (global) PRACTICE macro.

        Args:
            name: Name of the Macro.
            value: Value to set the Macro to.

        Returns:
            The resulting practice macro.
        """
        try:
            self.logger.debug(f'Setting value of practice macro {name} to {value}...')
            return PracticeMacro(self.__practice_service.set_macro(name, value))
        except Exception as exc:
            error_msg = f'Could not set value of practice macro {name} to {value}'
            self.logger.error(error_msg)
            raise PracticeError(error_msg) from exc

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
