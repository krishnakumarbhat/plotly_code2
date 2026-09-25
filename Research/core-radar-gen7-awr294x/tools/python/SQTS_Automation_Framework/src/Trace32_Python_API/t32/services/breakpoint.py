"""
This module contains classes to handle Trace32 breakpoints.
"""
from lauterbach.trace32 import rcl
import logging
from typing import Optional

from .common import Trace32Logger
from .error import BreakpointError
from .address import Address

logger = logging.getLogger(__name__)


class Breakpoint:
    """
    Class to represent a set breakpoint. Wrapper class for rcl.Breakpoint.

    Class Attributes:
        Action: Enum representing the possible actions to execute when a breakpoint is hit.
        Type: Enum representing the possible types of breakpoints to set.
        Impl: Enum representing the possible breakpoint implementations.

    Attributes:
        action: The action set for the breakpoint instance.
        address: The address where the breakpoint is set.
        core: The core for which the breakpoint is set.
        enabled: Whether the breakpoint is enabled or not.
        impl: The implementation for the breakpoint instance.
        size: The size of the breakpoint instance.
        type_: The type of breakpoint.
    """
    Action = rcl.Breakpoint.Action
    Type = rcl.Breakpoint.Type
    Impl = rcl.Breakpoint.Impl

    def __init__(self, breakpoint: rcl.Breakpoint, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for Breakpoint class.

        Args:
            breakpoint: The rcl.Breakpoint instance to wrap.
            _logger: A logger adapter to use for logging purposes.
        """
        self.__breakpoint = breakpoint
        self.logger = _logger

    def __str__(self) -> str:
        """
        Operator overloading for the str method.

        Returns:
            The string representation of the breakpoint.
        """
        return str(self.__breakpoint)

    @property
    def action(self) -> Action | None:
        """
        The action set for the breakpoint instance.
        """
        return self.__breakpoint.action

    @property
    def address(self) -> Address:
        """
        The address where the breakpoint is set.
        """
        return Address(self.__breakpoint.address)

    @property
    def core(self) -> str | None:
        """
        The core for which the breakpoint is set.
        """
        return self.__breakpoint.core

    @property
    def enabled(self) -> bool:
        """
        Whether the breakpoint is enabled or not.
        """
        return self.__breakpoint.enabled

    @property
    def impl(self) -> Impl | None:
        """
        The implementation for the breakpoint instance.
        """
        return self.__breakpoint.impl

    @property
    def size(self) -> int:
        """
        The size of the breakpoint instance.
        """
        return self.__breakpoint.size

    @property
    def type_(self) -> Type | None:
        """
        The type of breakpoint.
        """
        return self.__breakpoint.type_

    def delete(self) -> None:
        """
        Method to delete the breakpoint.
        """
        try:
            self.logger.debug(f'Deleting breakpoint {self}...')
            self.__breakpoint.delete()
        except Exception as exc:
            error_msg = f'Could not delete breakpoint {self}.'
            self.logger.error(error_msg)
            raise BreakpointError(error_msg) from exc

    def disable(self) -> 'Breakpoint':
        """
        Method to disable the breakpoint.

        Returns:
            The disabled breakpoint.
        """
        try:
            self.logger.debug(f'Disabling breakpoint {self}...')
            return Breakpoint(self.__breakpoint.disable(), self.logger)
        except Exception as exc:
            error_msg = f'Could not disable breakpoint {self}.'
            self.logger.error(error_msg)
            raise BreakpointError(error_msg) from exc

    def enable(self) -> 'Breakpoint':
        """
        Method to enable the breakpoint.

        Returns:
            The enabled breakpoint.
        """
        try:
            self.logger.debug(f'Enabling breakpoint {self}...')
            return Breakpoint(self.__breakpoint.enable(), self.logger)
        except Exception as exc:
            error_msg = f'Could not enable breakpoint {self}.'
            self.logger.error(error_msg)
            raise BreakpointError(error_msg) from exc


class BreakpointService:
    """
    Class to set breakpoints and get the currently set breakpoints.
    Wrapper for rcl.BreakpointService class.
    """

    def __init__(self, breakpoint_service: rcl.BreakpointService, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for Breakpoint Service.

        Args:
            brekpoint_service: The rcl.BreakpointService instance to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__breakpoint_service = breakpoint_service
        self.logger = Trace32Logger(logger, extra=_logger.extra)

    def set(
        self,
        address: Address,
        *,
        action: Optional[Breakpoint.Action] = None,
        core: Optional[str] = None,
        size: Optional[int] = None,
        type_: Optional[Breakpoint.Type] = None,
        impl: Optional[Breakpoint.Impl] = None,
        enabled: bool = True
    ) -> Breakpoint:
        """
        Operator overloading for calling BreakpointService instances.
        It creates a Breakpoint and sets it.

        Args:
            address: The address where the breakpoint is set.
            action: The action set for the breakpoint instance.
            core: The core for which the breakpoint is set.
            enabled: Whether the breakpoint is enabled or not.
            impl: The implementation for the breakpoint instance.
            size: The size of the breakpoint instance.
            type_: The type of breakpoint.

        Returns:
            A Breakpoint object with the specified parameters.
        """
        try:
            self.logger.debug(
                f'Creating and setting breakpoint at address {address} for core {core} of type '
                f'{type_}, size {size}, action {action}, impl {impl} and enabled {enabled}'
            )
            bp = self.__breakpoint_service.set(
                action=action,
                address=address._internal_obj,
                core=core,
                size=size,
                type_=type_,
                impl=impl,
                enabled=enabled
            )
            return Breakpoint(bp, self.logger)
        except Exception as exc:
            error_msg = f'Failed to create and set breakpoint at address {address}.'
            self.logger.error(error_msg)
            raise BreakpointError(error_msg) from exc

    def list(self) -> list[Breakpoint]:
        """
        Method to get a list of the currently set Breakpoints.

        Returns:
            The currently set breakpoints.
        """
        try:
            self.logger.debug('Getting list of currently set breakpoints...')
            bps = [Breakpoint(bp, self.logger) for bp in self.__breakpoint_service.list()]
            self.logger.debug(f'Got list of breakpoints {bps}.')
            return bps
        except Exception as exc:
            error_msg = 'Could not get list of currently set breakpoints.'
            self.logger.error(error_msg)
            raise BreakpointError(error_msg) from exc

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
