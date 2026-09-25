"""
This module contains classes to format the contents of the generated reports.
"""
import logging


class ReportMaker:
    """
    Class to format steps and info statements.
    """
    def __init__(self, logger: logging.Logger) -> None:
        """
        Init method for ReportMaker class.

        Args:
            logger: A Logger object through which the report contents
                will be transmitted.
        """
        self._logger = logger
        self._current_step = 1

    def info(self, msg: str, *args, **kwargs) -> None:
        """
        Method to log info into the report.

        Args:
            msg: The message to log.
            args: Positional arguments to pass to the logger.info method.
            kwargs: Keyword arguments to pass to the logger.info method.
        """
        self._logger.info(msg, *args, **kwargs)

    def warning(self, msg: str, *args, **kwargs) -> None:
        """
        Method to log warnings into the report.

        Args:
            msg: The message to log.
            args: Positional arguments to pass to the logger.info method.
            kwargs: Keyword arguments to pass to the logger.info method.
        """
        self._logger.warning(msg, *args, **kwargs)

    def _step_line(self, msg: str, *args, **kwargs) -> None:
        """
        Method to log a line of a step header in the report.

        Args:
            msg: The message to log.
            args: Positional arguments to pass to the logger.info method.
            kwargs: Keyword arguments to pass to the logger.info method.
        """
        self.info(msg, *args, extra={'status': ''}, **kwargs)

    def step(self, msg: str, *args, **kwargs) -> None:
        """
        Method to log a step header into the log.

        Args:
            msg: The description of the step.
            args: Positional arguments to pass to the logger.info method.
            kwargs: Keyword arguments to pass to the logger.info method.
        """
        self._step_line(100 * '=')
        self._step_line(f'STEP {self._current_step:2}: {msg}', *args, **kwargs)
        self._step_line(100 * '=')
        self._current_step += 1


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/14/2023  ABPA       FKU-1009  Initial creation
