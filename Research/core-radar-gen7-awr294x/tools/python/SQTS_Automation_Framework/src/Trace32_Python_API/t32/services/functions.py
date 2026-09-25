"""
This module contains classes to call Trace32 functions and get
back the results.
"""
from lauterbach.trace32 import rcl
import logging

from .common import Trace32Logger
from .error import FunctionError

logger = logging.getLogger(__name__)


class FunctionService:
    """
    Class to call Trace32 functions and get the result back.
    Wrapper class for rcl.FunctionService.
    """

    def __init__(self, function_service: rcl.FunctionService, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for FunctionService class.

        Args:
            function_service: The rcl.FunctionService instance to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__function_service = function_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def __call__(self, command: str) -> int | float | str:
        """
        Operator overloading for calling FunctionService instances.

        Args:
            command: The function command to execute in Trace32.

        Returns:
            The result of calling the specified function.
        """
        try:
            self.logger.debug(f'Calling function {command}')
            function_value = self.__function_service(command)
            self.logger.debug(f'Got {function_value} from calling {command}.')
            return function_value
        except Exception as exc:
            error_msg = f'Could not call function {command}.'
            self.logger.error(error_msg)
            raise FunctionError(error_msg) from exc

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
