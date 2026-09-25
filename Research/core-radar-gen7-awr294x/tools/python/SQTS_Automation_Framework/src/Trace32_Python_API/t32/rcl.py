"""
This module contains functions to connect with Trace32
and to interact with it.
"""
import logging
from lauterbach.trace32 import rcl
from typing import NamedTuple, Literal

from . import services

logger = logging.getLogger(__name__)


class Message(NamedTuple):
    test: str
    type: int


class Debugger:
    """
    Class to interact with Trace32.
    Wrapper class for rcl.Debugger.

    Attributes:
        address: Trace32 address service.
        breakpoint: Trace32 breakpoint service.
        cmd: Trace32 command service.
        fnc: Trace32 function service.
        memory: Trace32 memory service.
        practice: Trace32 practice service.
        register: Trace32 register service.
        symbol: Trace32 symbol service.
        variable: Trace32 variable service.
    """

    def __init__(self, debugger: rcl.Debugger, _logger: logging.LoggerAdapter) -> None:
        """
        Init method for Debugger class.

        Args:
            debugger: The rcl.Debugger object to wrap.
            _logger: A logger adapter from which to extract the extra info
                to log with this module's logger.
        """
        self.__debugger = debugger

        self.logger = services.Trace32Logger(logger, extra=_logger.extra)

        self.address = services.AddressService(self.__debugger.address, self.logger)
        self.breakpoint = services.BreakpointService(self.__debugger.breakpoint, self.logger)
        self.cmd = services.CommandService(self.__debugger.cmd, self.logger)
        self.fnc = services.FunctionService(self.__debugger.fnc, self.logger)
        self.memory = services.MemoryService(self.__debugger.memory, self.logger)
        self.practice = services.PracticeService(self.__debugger.practice, self.logger)
        self.register = services.RegisterService(self.__debugger.register, self.logger)
        self.symbol = services.SymbolService(self.__debugger.symbol, self.logger)
        self.variable = services.VariableService(self.__debugger.variable, self.logger)

    def connect(self) -> None:
        """
        Method to connect with Trace32.
        """
        self.logger.debug('Connecting...')
        self.__debugger.connect()

    def disconnect(self) -> None:
        """
        Method to disconnect from Trace32.
        """
        self.logger.debug('Disconnecting from Trace32...')
        self.__debugger.disconnect()

    def print(self, message: str) -> None:
        """
        Method to print something to the Trace32 AREA window.

        Args:
            message: The message to print.
        """
        self.logger.debug(f'Printing {message} to Trace32...')
        self.__debugger.print(message)

    def ping(self) -> None:
        """
        Method to ping Trace32.
        """
        self.logger.debug('Pinging Trace32 connection...')
        self.__debugger.ping()

    def get_message(self) -> Message:
        """
        Method to get the last message in the AREA window.

        Returns:
            The latest message.
        """
        self.logger.debug('Getting last message in AREA window...')
        return self.__debugger.get_message()

    def step(self) -> None:
        """
        Method to do a single program step with the debugger.
        """
        self.logger.debug('Stepping...')
        self.__debugger.step()

    def step_asm(self) -> None:
        """
        Method to do a single program step for asm with the debugger.
        """
        self.logger.debug('Stepping asm...')
        self.__debugger.step_asm()

    def step_hll(self) -> None:
        """
        Method to do a single program step for hll with the debugger.
        """
        self.logger.debug('Stepping hll...')
        self.__debugger.step_hll()

    def step_over(self) -> None:
        """
        Method to step over a program line with the debugger.
        """
        self.logger.debug('Stepping over...')
        self.__debugger.step_over()

    def go(self) -> None:
        """
        Method to start the program execution.
        """
        self.logger.debug('Go')
        self.__debugger.go()

    def go_up(self) -> None:
        """
        Method to start the program execution in order to return to the caller function.
        """
        self.logger.debug('Go Up')
        self.__debugger.go_up()

    def go_return(self) -> None:
        """
        The first call to this method stops at the function epilog.
        The second call stops at the return of the function. Stopping
        at the function epilog first has the advantage that the local
        variables are still valid at this point.
        """
        self.logger.debug('Go return')
        self.__debugger.go_return()

    def break_(self) -> None:
        """
        Method to stop the program execution.
        """
        self.logger.debug('Breaking...')
        self.__debugger.break_()

    def get_state(self) -> bytes:
        """
        Method to get the current program state.

        Returns:
            The state encoded in bytes.
        """
        self.logger.debug('Getting current state...')
        return self.__debugger.get_state()


def connect(
        *,
        node: str,
        port: int,
        protocol: Literal['TCP', 'UDP'],
        timeout: float,
        _logger: logging.LoggerAdapter
) -> Debugger:
    """
    Function to connect to Trace32.

    Args:
        node: The node for the connection.
        port: The port to connect to.
        protocol: The type of protocol to use for the connection.
        timeout: Timeout in seconds to try to connect for.
        _logger: A logger adapter to pass to the created Debugger instance.

    Returns:
        A Debugger instance to interact with Trace32.
    """
    dbg = rcl.connect(node=node, port=port, protocol=protocol, timeout=timeout)
    return Debugger(dbg, _logger)

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 07/27/2023  ABPA       FKU-897   Initial creation
