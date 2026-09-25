"""
This module contains classes to execute Trace32 commands.
"""
from lauterbach.trace32 import rcl
import logging

from .common import Trace32Logger
from .error import CommandError

logger = logging.getLogger(__name__)


class CommandService:
    """
    Class to execute Trace32 commands.
    Wrapper class for rcl.CommandService.
    """

    def __init__(self, command_service: rcl.CommandService, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for CommandService class.

        Args:
            command_service: The rcl.CommandService instance to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__command_service = command_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def __call__(self, cmd: str) -> None:
        """
        Operator overloading for calling CommandService instances.

        Args:
            cmd: The Trace32 command to execute.
        """
        try:
            self.logger.debug(f'Running command {cmd}')
            self.__command_service(cmd)
        except Exception as exc:
            error_msg = f'Could not run command {cmd}.'
            self.logger.error(error_msg)
            raise CommandError(error_msg) from exc

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
