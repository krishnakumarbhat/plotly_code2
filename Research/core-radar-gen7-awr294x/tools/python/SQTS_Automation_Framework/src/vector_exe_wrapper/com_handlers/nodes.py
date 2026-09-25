"""
This module contains classes to handle CAPL nodes in CANoe.
"""

from typing import Optional, TypeVar
from pathlib import Path
from win32com.client import CastTo
import logging
import time

logger = logging.getLogger(__name__)

VectorExe = TypeVar('VectorExe')
INodes = TypeVar('INodes')
INode = TypeVar('INode')
IBus = TypeVar('IBus')


class Node:
    """
    Class to handle an individual CAPL node.

    Attributes:
        _handler: The Nodes object that created this instance.
        _com_obj: The COM object used to interact with the CAPL node.
        _path: The path of the capl script for the node.
        _name: The name of the CAPL node.
    """
    def __init__(self, handler: 'Nodes', node_com_obj: INode, vector_app: VectorExe) -> None:
        """
        Init method for Node class.

        Args:
            handler: The Nodes object that will manage this instance.
            node_com_obj: The COM object used to interact with the CAPL node.
            vector_app: The VectorExe object that owns the nodes to handle.
        """
        self._vector_app = vector_app
        self._handler = handler
        self._com_obj = CastTo(node_com_obj, 'INode8')
        self._path = Path(node_com_obj.FullName)
        self._name = node_com_obj.Name
        self.__valid = True

        logger.debug(f'Node handler for CAPL node {self._name} has been created.')

    def remove_inactive_buses(self) -> None:
        """
        Method to detach inactive buses from the CAPL node.
        """
        logger.debug(f'Removing inactive buses from {self._name} node...')
        attached_buses = [CastTo(bus, 'IBusVB2') for bus in self._com_obj.AttachedBuses]
        for bus in attached_buses:
            if not bus.Active:
                logger.debug(f'Removing {bus.Name}...')
                self._com_obj.DetachBus(bus)
        logger.debug('Inactive buses removed from sqts_simulation_node.')

    def remove(self) -> None:
        """
        Method to remove a CAPL node.
        """
        logger.debug(f'Removing node {self._path}...')
        self._handler.remove(str(self._path))
        self._mark_invalid()

    def enable(self) -> None:
        """
        Method to activate a CAPL node.
        """
        if not self.__valid:
            logger.error(f'Tried to enable invalid node: {self._path}.')
            raise Exception(f'Node {self._path} is no longer valid, it must have been removed from the simulation')

        logger.debug(f'Enabling node {self._path}...')

        measurement_was_running = self._vector_app.measurement_running
        self._vector_app.stop_measurement()

        self._com_obj.Active = True

        if measurement_was_running:
            time.sleep(2)
            self._vector_app.start_measurement()

    def disable(self) -> None:
        """
        Method to deactivate a CAPL node.
        """
        if not self.__valid:
            logger.error(f'Tried to enable invalid node: {self._path}.')
            raise Exception(f'Node {self._path} is no longer valid, it must have been removed from the simulation')

        logger.debug(f'Disabling node {self._path}...')

        measurement_was_running = self._vector_app.measurement_running
        self._vector_app.stop_measurement()

        self._com_obj.Active = False

        if measurement_was_running:
            time.sleep(2)
            self._vector_app.start_measurement()

    def add_network(self, network: IBus) -> None:
        """
        Method to attach a bus to the CAPL node.
        """
        if not self._com_obj.IsBusAttached(network):
            logger.debug(f'Attached network {network.Name} to node {self._name}')
            self._com_obj.AttachBus(network)
        else:
            logger.debug(f'Tried attaching network {network.Name} to node {self._name}, it was already attached.')

    def remove_network(self, network: IBus) -> None:
        """
        Method to detach a bus from the CAPL node.
        """
        if self._com_obj.IsBusAttached(network):
            logger.debug(f'Dettached network {network.Name} from node {self._name}')
            self._com_obj.DetachBus(network)
        else:
            logger.debug(f'Tried dettaching network {network.Name} from node {self._name}, it was already dettached.')

    def _mark_invalid(self) -> None:
        """
        Method to mark a node as invalid.
        """
        self.__valid = False


class Nodes:
    """
    Class to handle CAPL nodes in the simulation.

    Attributes:
        _vector_app: The VectorExe object that owns the nodes to handle.
        _nodes_com_obj: The COM interface interact with the nodes in CANoe.
        _nodes: The nodes that have been attached with this Nodes handler.
    """
    def __init__(self, vector_app: VectorExe) -> None:
        """
        Init method for Nodes class.

        Args:
            vector_app: The VectorExe object that owns the nodes to handle.
        """
        self._vector_app = vector_app
        self._nodes_com_obj: INodes = vector_app.com_interface.Configuration.SimulationSetup.Nodes
        self._nodes: dict[str, Node] = {}

    def _remove_if_exists(self, node_fullname: str) -> None:
        """
        Method to remove a node if present in the simulation.

        Args:
            node_fullname: The full path of the node to remove.
        """
        indexes_to_remove = []
        logger.debug(f'Checking if node with path {node_fullname} exists in simulation...')
        for i, node in enumerate(self._nodes_com_obj, start=1):
            if node_fullname == node.FullName:
                logger.debug(f'Node found with index: {i}')
                indexes_to_remove.append(i)

        if not indexes_to_remove:
            logger.debug(f'No nodes found with path {node_fullname}.')
            return

        for i in sorted(indexes_to_remove, reverse=True):
            node = CastTo(self._nodes_com_obj.Item(i), 'INode8')

            # Remove all buses except 1
            attached_buses = [CastTo(bus, 'IBusVB2') for bus in node.AttachedBuses]
            logger.debug(f'Removing buses of node with index {i}...')
            for bus in attached_buses[1:]:
                node.DetachBus(bus)

            # Remove node
            logger.debug(f'Removing node with index {i}...')
            self._nodes_com_obj.Remove(i)

        logger.debug(f'Node {node_fullname} has been removed.')

    def add(self, node_path: str, node_name: Optional[str] = None) -> Node:
        """
        Method to add a CAPL node to the simulation.

        Args:
            node_path: The full path to the CAPL script that the node will use.
            node_name: The name that will be shown for the node in the simulation.
        """
        full_node_path = Path(node_path).absolute()
        node_fullname = str(full_node_path)

        if node_name is None:
            node_name = full_node_path.stem

        # Remove duplicates if present
        self._remove_if_exists(node_path)

        # Add to simulation
        logger.debug(f'Adding {node_name} node to simulation...')
        node = self._nodes_com_obj.Add(node_name)
        node.FullName = node_fullname
        node.Active = True

        logger.debug(f'{node_name} node added to the configuration.')

        created_node = Node(self, node, self._vector_app)
        created_node.remove_inactive_buses()

        self._nodes[node_fullname] = created_node

        return created_node

    def remove(self, node_fullname: str) -> None:
        """
        Method to remove a node from the simulation.

        Args:
            node_fullname: The full path to the
        """
        if node_fullname in self._nodes:
            node = self._nodes.pop(node_fullname)
            node._mark_invalid()

        self._remove_if_exists(node_fullname)

    def remove_network_from_nodes(self, network: IBus) -> None:
        """
        Method to dettach a network from all the nodes that have been
        added to the simulation with this node handler.

        Args:
            network: The COM interface object of the network to remove.
        """
        measurement_was_running = self._vector_app.measurement_running
        self._vector_app.stop_measurement()

        for node in self._nodes.values():
            node.remove_network(network)

        if measurement_was_running:
            time.sleep(2)
            self._vector_app.start_measurement()

    def add_network_to_nodes(self, network: IBus) -> None:
        """
        Method to attach a network to all the nodes that have been
        added to the simulation with this node handler.

        Args:
            network: The COM interface object of the network to remove.
        """
        measurement_was_running = self._vector_app.measurement_running
        self._vector_app.stop_measurement()

        for node in self._nodes.values():
            node.add_network(network)

        if measurement_was_running:
            time.sleep(2)
            self._vector_app.start_measurement()

    def __getitem__(self, node_name: str) -> Node:
        """
        Operator overloading for indexing Nodes objects.

        Args:
            node_name: The name of the node to fetch.

        Returns:
            The handle for the requested node.
        """
        for node in self._nodes_com_obj:
            if node.Name == node_name:
                return Node(self, node, vector_app=self._vector_app)

        raise IndexError(f'No node with name {node_name} was found.')

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 05/31/2023  ABPA       FKU-926   Initial creation
# 07/04/2023  LACE       FKU-937   Added get item method for individual Nodes.
