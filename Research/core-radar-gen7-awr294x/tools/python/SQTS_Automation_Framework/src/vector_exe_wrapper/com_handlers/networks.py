"""
This module contains classes to handle the networks in a CANoe simulation.
"""

from typing import TypeVar

VectorExe = TypeVar('VectorExe')
IBus = TypeVar('IBus')


class Network:
    """
    Class to handle an individual network/bus in the CANoe simulation.

    Attributes:
        _handler: The Networks object that handles this network.
        _com_obj: The COM interface to this particular network to communicate with CANoe.
        _name: The name of the network.
    """
    def __init__(self, handler: 'Networks', com_obj: IBus) -> None:
        """
        Init method for Network class.

        Args:
            handler: The Networks object that handles this network.
            com_obj: The COM interface to this particular network to communicate with CANoe.
        """
        self._handler = handler
        self._com_obj = com_obj
        self._name = com_obj.Name

    @property
    def name(self) -> str:
        """
        Name of the network.
        """
        return self._name

    def enable(self) -> None:
        """
        Method to enable the network.
        """
        self._com_obj.Active = True
        self._handler.add_network_to_nodes(self._com_obj)

    def disable(self) -> None:
        """
        Method to disable the network.
        """
        self._com_obj.Active = False
        self._handler.remove_network_from_nodes(self._com_obj)


class Networks:
    """
    Class to handle the networks in the CANoe simulation.

    Attributes:
        vector_app: The VectorExe object that owns the networks to handle.
        _com_obj: The COM interface to the simulation networks to communicate with CANoe.
    """
    def __init__(self, vector_app: VectorExe) -> None:
        """
        Init method for Networks class.

        Args:
            vector_app: The VectorExe object that owns the networks to handle.
        """
        self._vector_app = vector_app
        self._com_obj = vector_app.com_interface.Configuration.SimulationSetup.Buses

    def remove_network_from_nodes(self, network: IBus) -> None:
        """
        Method to dettach a network from the simulation nodes.

        Args:
            network: The COM network object of the network to dettach.
        """
        self._vector_app._nodes.remove_network_from_nodes(network)

    def add_network_to_nodes(self, network: IBus) -> None:
        """
        Method to attach a network to the simulation nodes.

        Args:
            network: The COM network object of the network to attach.
        """
        self._vector_app._nodes.add_network_to_nodes(network)

    def __getitem__(self, network_name: str) -> Network:
        """
        Operator overloading for indexing Networks objects.

        Args:
            network_name: The name of the network to fetch.

        Returns:
            The handle for the requested network.
        """
        for bus in self._com_obj:
            if bus.Name == network_name:
                return Network(self, bus)

        raise IndexError(f'No network with name {network_name} was found.')

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials    JIRA     Explanation of changes done here.
#    Date        By      AAA-####      Description
# ----------  ---------  --------  ----------------------------------
# 05/31/2023  ABPA       FKU-926   Initial creation
