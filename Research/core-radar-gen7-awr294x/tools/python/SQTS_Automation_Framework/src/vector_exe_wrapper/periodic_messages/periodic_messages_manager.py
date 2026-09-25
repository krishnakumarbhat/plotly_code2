"""
This module contains classes to handle periodic messages in CANoe.
"""

from pathlib import Path
from uuid import uuid4
from functools import reduce
import operator
import time
from pythoncom import com_error
from typing import TypeVar
import logging

from ..com_handlers import Nodes, Node
from .periodic_message import PeriodicMessage
from .capl_generation import generate_capl_script

logger = logging.getLogger(__name__)

VectorExe = TypeVar('VectorExe')

THIS_DIR = Path(__file__).parent


def capl_functions_names(messages: list[PeriodicMessage]) -> set[str]:
    """
    Function to get the names of the generated CAPL functions from a list of periodic messages.

    Args:
       messages: The periodic messages used to generate the CAPL functions.

    Returns:
        A set with the names of the generated functions.
    """
    return reduce(operator.or_, [
        {
            f'sqts_start_{message.id}',
            f'sqts_stop_{message.id}',
            f'sqts_set_period_{message.id}'
        } for message in messages
    ])


class PeriodicMessageHandle:
    """
    Class to handle a single periodic message.

    Attributes:
       _vector_app: The VectorExe object used to send the message. Needed to invoke CAPL functions.
       _message: The specification of the periodic message.
    """

    def __init__(self, vector_app: VectorExe, message: PeriodicMessage) -> None:
        """
        Init method.

        Args:
           vector_app: The VectorExe object used to send the message.
           message: The specification of the periodic message.
        """
        self._vector_app = vector_app
        self._message = message

    def start(self) -> None:
        """
        Method to start the transmission of the periodic message.

        Args:
           self
        """
        logger.debug(f'Starting transmission of periodic message {self._message.id}...')
        self._vector_app.invoke_capl_func(f'sqts_start_{self._message.id}')

    def stop(self) -> None:
        """
        Method to stop the transmission of the periodic message.
        """
        logger.debug(f'Stopping transmission of periodic message {self._message.id}...')
        self._vector_app.invoke_capl_func(f'sqts_stop_{self._message.id}')

    def set_period(self, period_ms: int) -> None:
        """
        Method to start the transmission of the periodic message.

        Args:
           period_ms: The new period of the periodic message in ms.
        """
        logger.debug(f'Setting period of message {self._message.id} to {period_ms}ms')
        self._vector_app.invoke_capl_func(f'sqts_set_period_{self._message.id}', [period_ms])
        self._message.period_ms = period_ms


class PeriodicMessagesManager:
    """
    Class to handle the creation and removal of CAPL nodes to send periodic messages. To be used in a context manager.

    Attributes:
       _vector_app: The VectorExe object used to send the messages.
       _nodes: The CAPL nodes in the simulation setup.
       _node: The managed CAPL node used for sending periodic messages.
       _node_name: The name of the created CAPL node.
       _node_path: The path to the created CAPL node.
       _message_specifications: A dictionary with the periodic messages specifications.
    """

    def __init__(self, vector_app: VectorExe, messages_specifications: list[PeriodicMessage]) -> None:
        """
        Init method.

        Args:
           vector_app: The VectorExe object used to send the messages.
           messages_specifications: A list with the periodic messages specifications.
        """
        self._vector_app: VectorExe = vector_app
        self._nodes: Nodes = self._vector_app.nodes
        self._node: Node = None
        self._node_name: str = f'sqts_periodic_messages_{uuid4()}.can'
        self._node_path: Path = THIS_DIR.parent / fr'simulation_nodes\{self._node_name}'
        self._message_specifications: dict[str, PeriodicMessage] = {message.id: message for message in messages_specifications}

    def __enter__(self) -> 'PeriodicMessagesManager':
        """
        Operator overloading of enter method. Executed at the start of a 'with' block.

        Returns:
            self
        """
        logger.debug('Entering context management block for Periodic Messages...')
        self._in_context_management_block = True
        self._measurement_was_running = self._vector_app.measurement_running

        # Create capl script
        generate_capl_script(self._node_path, self._message_specifications.values())

        # Stop measurement to be able to add CAPL script
        logger.debug('Stopping measurement to attach CAPL node to the simulation...')
        self._vector_app.stop_measurement()

        # Attach capl node
        logger.debug(f'Attaching {self._node_name} node to the simulation...')
        self._node = self._nodes.add(self._node_path)

        # Compile
        logger.debug('Compiling CAPL nodes...')
        self._vector_app._app_com_obj.CAPL.Compile()

        # Load CAPL functions
        capl_functions = {f: None for f in capl_functions_names(self._message_specifications.values())}
        logger.debug(f'Specifying CAPL functions to load that are needed to work with the periodic messages: {capl_functions}')
        self._vector_app._capl_func_dict.update(capl_functions)

        # Start measurement again
        time.sleep(3)
        logger.debug('Starting measurement again to initiate transmission of periodic messages...')
        self._vector_app.start_measurement()

        return self

    def __exit__(self, exc_type, exc_value, exc_tb) -> None:
        """
        Operator overloading of exit method. Executed when exiting a 'with' block.

        Args:
           exc_type: If an exception happens in the 'with' block, this argument contains the type of the exception type.
           exc_value: If an exception happens in the 'with' block, this argument contains the exception object.
           exc_tb: If an exception happens in the 'with' block, this argument contains the exception traceback.
        """
        logger.debug('Exiting context management block for periodic messages...')

        # Stop measurement
        logger.debug('Stopping measurement to remove periodic messages CAPL node...')
        self._vector_app.stop_measurement()

        # Wait a bit to remove capl node
        time.sleep(3)

        try:
            # Remove capl node
            logger.debug('Trying to remove periodic messages CAPL node...')
            self._node.remove()

            logger.debug('CAPL node removed.')

            # Delete capl file
            logger.debug('Removing temporary CAPL script created to send periodic messages')
            self._node_path.unlink(missing_ok=True)
        except com_error:
            logger.warning('An exception occured trying to remove the CAPL node, disabling it instead...')
            self._node.disable()

        # Remove CAPL functions
        logger.debug('Removing CAPL functions used to handle the periodic messages from internal dictionary...')
        capl_functions = capl_functions_names(
            self._message_specifications.values())
        self._vector_app._capl_func_dict = {fun_name: capl_fun
                                            for fun_name, capl_fun in self._vector_app._capl_func_dict.items()
                                            if fun_name not in capl_functions}

        # Resume measurement
        if self._measurement_was_running:
            logger.debug('Resuming measurement...')
            self._vector_app.start_measurement()

    def __getitem__(self, msg_id: int) -> PeriodicMessageHandle:
        """
        Operator overloading of getitem for indexing with IDs.

        Args:
           msg_id: The ID of the message

        Returns:
            A handle for the specific periodic message requested.
        """
        if msg_id not in self._message_specifications:
            logger.error(f'Tried to get a handle to a periodic message not among ones available, requested ID: {msg_id}')
            raise Exception(f'ID not among the specified periodic messages: {msg_id}')
        return PeriodicMessageHandle(self._vector_app, self._message_specifications[msg_id])

    def __getattr__(self, msg_name: str) -> PeriodicMessageHandle:
        """
        Operator overloading of getitem for indexing with IDs.

        Args:
           msg_name: The name of the message

        Returns:
            A handle for the specific periodic message requested.
        """
        if msg_name not in self._message_specifications:
            logger.error(f'Tried to get a handle to a periodic message not among ones available, requested message name: {msg_name}')
            raise Exception(f'Message name not among the specified periodic messages: {msg_name}')
        return PeriodicMessageHandle(self._vector_app, self._message_specifications[msg_name])

    def start_all(self) -> None:
        """
        Method to start the transmission of all the periodic messages in the CAPL node.
        """
        logger.debug('Starting transmission of all periodic messages...')
        for id in self._message_specifications:
            self._vector_app.invoke_capl_func(f'sqts_start_{id}')

    def stop_all(self) -> None:
        """
        Method to stop the transmission of all the periodic messages in the CAPL node.
        """
        logger.debug('Stopping transmission of all periodic messages...')
        for id in self._message_specifications:
            self._vector_app.invoke_capl_func(f'sqts_stop_{id}')

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 04/10/2023  ABPA       FKU-848   Initial creation
# 05/11/2023  ABPA       FKU-736   Added trace logging.
# 05/31/2023  ABPA       FKU-926   Added use of Node and Nodes objects.
