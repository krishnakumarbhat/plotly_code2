'''
    This module contains the classes and functions for support of automated ethernet
    packet transmission and retrieval to/from CANoe
'''
from enum import IntEnum
import logging
import time
from typing import TypeVar

logger = logging.getLogger(__name__)

# Dynamic Types
VectorExe = TypeVar('VectorExe')

# Constants
FDX_SEND_PACKET_ID = 0x8002
FDX_RECEIVE_PACKET_ID = 0x8003
FDX_SEND_ETHERNET_DIAGNOSTIC_ID = 0x8004


class ProtocolType:
    '''
    Enum to represent ethernet packet transmission protocols. The codes are represented as unsigned 16 bit integers.
    '''
    IPV4 = 0x0800
    PTP = 0x88F7


class DiagnosticType(IntEnum):
    '''
        Enum class to represent different types of diagnostic flags that can be passed to send_diagnostic() for ports in a network
    '''
    LINK_STATUS = 1
    SPEED = 2
    SQI = 3
    ERROR_PACKETS = 4


class EthernetPacket:
    '''
    class used to store extracted data from system variables of an ethernet packet

    Attributes:
        channel: Ethernet channel over which packet is transmitted.
        type: Internet protocol type (ipv4 or ipv6).
        source_address: MAC address of source node.
        destination_address: MAC address of destination node.
        packet_buffer: The data to send in the packet, a list less than 1500 bytes and composed of integers.
    '''

    def __init__(self, channel: int, protocolType: int, source_address: str, destination_address: str, data:  bytearray | list[int] | bytes):
        '''
            Init method for initializing class member variables
        '''
        self.channel: int = channel
        self.protocolType: int = protocolType
        self.source_address: str = source_address
        self.destination_address: str = destination_address
        self.data: bytearray | list[int] | bytes = data


class EthPTPpacket:
    '''
    Inherited class used to store extracted data from system variables of an ethernet PTP packet

    Attributes:
        hwport: the hardware port of the node.
        time: the timestamp when the packet was read.
        msgtype: the type of PTP message received.
        source_port_number: the port number of the source node of the PTP message.
        domain_number: the domain number of the node.
        is_ptp: is the message a PTP message.
        vlan_id: the VLAN ID tag of the PTP message.
        destination: the MAC address of the destination node

    '''
    def __init__(self, hwport: int, time: int, msgtype: int, source_port_number: int, domain_number: int, is_ptp: bool, vlan_id: int, destination: str):
        '''
            Init method for initializing class member varibles
        '''
        self.hwport: int = hwport
        self.time: int = time
        self.msgtype: int = msgtype
        self.source_port_number: int = source_port_number
        self.domain_number: int = domain_number
        self.is_ptp: int = is_ptp
        self.vlan_id: int = vlan_id
        self.destination: str = destination


class EthernetHandler:
    '''
    class handle containing the ethernet packet transmission and retrieval functionality

    Attributes:
        fdx: Giving EthernetHandler access to FDX module
        _fdx_send_ethernet_packet_counter: Counter that gets updated to indicate updating of system variables to CAPL event handler.
        _fdx_receive_ethernet_packet_counter: Counter to check if its odd (correct ethernet packet receival).
        _expected_packet: Dictionary to store expected values that are sent to CAPL script for receiving packet.
    '''

    def __init__(self, fdx):
        '''
            Init method for initializing class member variables
        '''

        self.fdx = fdx
        self._fdx_send_ethernet_packet_counter: int = 1
        self._fdx_send_ethernet_diagnostic_counter: int = 1
        self._fdx_receive_ethernet_packet_counter: int = 1
        self._expected_packet = None

    def send_diagnostic(
                        self,
                        port_name: str,
                        type_flag: int,
                        timeout_s: float = 5,
                        ) -> int:
        '''
        Function to send a diagnostic request by updating the values of system variables.
        The sqts_simulation_node capl node that is attached to the simulation
        will assemble and send the packet when the SQTS::send_ethernet_diagnostic_counter
        is updated.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            port_name: The name of the ethernet port.
            type_flag: Which diagnostic request is being sent (link_status, speed, SQI, error packets).
            timeout_s: Timeout to try confirming that the request was sent.

        Returns:
            The diagnostic value (Speed, LinkStatus, SQI, ErrorPackets)

        Raises:
            TimeoutError: The timeout expired trying to confirm that the ethernet diagnostic request was sent.
            Exception: There was an error in CAPL sending the request.
        '''

        if not (1 <= type_flag <= 4):
            logger.error(f'type_flag must be between 1 and 4, {type_flag} is invalid')
            raise Exception(f'type_flag must be between 1 and 4, {type_flag} is invalid')

        current_counter = self._fdx_send_ethernet_diagnostic_counter

        fdx_data = {  # update system variables with input values
            'SQTS::send_ethernet_diagnostic::portName': port_name,
            'SQTS::send_ethernet_diagnostic::type_flag': type_flag,
            'SQTS::send_ethernet_diagnostic::counter': current_counter,
        }

        self._fdx_send_ethernet_diagnostic_counter += 2  # The sent counter is always an odd number, and the received counter even.

        # Send updated system variables data to CANoe through FDX, this triggers a CAPL on sysvar event
        # in sqts_simulation_node that assembles and sends the request.
        logger.debug(f'Sending diagnostic request with counter {current_counter}...')
        self.fdx.send_data(fdx_data, group_id=FDX_SEND_ETHERNET_DIAGNOSTIC_ID)

        # Once the CAPL script has sent the request, it will trigger the transmission of that
        # same data group from CANoe to this script. wait for that response to verify
        # that the packet has been sent.
        logger.debug('Verifying that the request was sent correctly...')
        start = time.time()
        received_data = self.fdx.receive_data(FDX_SEND_ETHERNET_DIAGNOSTIC_ID, timeout_s=timeout_s)
        elapsed = time.time() - start
        while elapsed < timeout_s:
            # Verify that the check_counter is correct, it should be the sent counter plus 1
            if received_data['SQTS::send_ethernet_diagnostic::check_counter'] == current_counter + 1:
                error_code = received_data['SQTS::send_ethernet_diagnostic::error_code']
                if error_code != 0:
                    logger.error(f'There was an error sending the request, error code: {error_code}.')
                    raise Exception(f'There was an error sending the request, error code: {error_code}.')
                logger.debug('request sent correctly.')

                received_type_flag = received_data['SQTS::send_ethernet_diagnostic::type_flag']

                if received_type_flag != type_flag:
                    logger.error(f'Expected different diagnostic type, expected {type_flag}, response had type {received_type_flag}')
                    raise Exception(f'Expected different diagnostic type, expected {type_flag}, response had type {received_type_flag}')

                match received_type_flag:
                    case DiagnosticType.LINK_STATUS:
                        status = received_data['SQTS::send_ethernet_diagnostic::linkStatus']
                        logger.debug(f'Received status {status}.')
                        return status

                    case DiagnosticType.SPEED:
                        speed = received_data['SQTS::send_ethernet_diagnostic::speed']
                        logger.debug(f'Received speed {speed}.')
                        return speed

                    case DiagnosticType.SQI:
                        sqi = received_data['SQTS::send_ethernet_diagnostic::SQI']
                        logger.debug(f'Received SQI {sqi}.')
                        return sqi

                    case DiagnosticType.ERROR_PACKETS:
                        error_packets = received_data['SQTS::send_ethernet_diagnostic::error_packets']
                        logger.debug(f'Received error packets {error_packets}.')
                        return error_packets

            else:
                received_data = self.fdx.receive_data(FDX_SEND_ETHERNET_DIAGNOSTIC_ID, timeout_s=timeout_s-elapsed)
                elapsed = time.time() - start
        else:
            logger.error('Timeout expired while verifying that the request was sent correctly')
            raise TimeoutError('Timeout expired while verifying that the request was sent correctly')

    def send_packet(
                        self,
                        destination_address: str,
                        source_address: str,
                        vlan_id: int,
                        data: list[int] | bytes | bytearray,
                        channel: int,
                        timeout_s: float = 0.5,
                        priority: int = 1,
                        Type: int = ProtocolType.IPV4
                        ) -> None:

        '''
        Function to send an ethernet packet by updating the values of system variables.
        The sqts_simulation_node capl node that is attached to the simulation
        will assemble and send the packet when the SQTS::send_ethernet_packet_counter
        is updated.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            destination_address: MAC address of destination node.
            source_address: MAC address of source node.
            vlan_id: VLAN ID over which packet is sent.
            data: The data to send in the packet, a list less than 1500 bytes and composed of integers.
            channel: Ethernet channel for packet transmission.
            priority: Priority of VLAN.
            timeout_s: Timeout to try confirming that the packet was sent.
            type: Internet protocol type (ipv4 or ipv6).

        Raises:
            TimeoutError: The timeout expired trying to confirm that the ethernet packet was sent.
            Exception: There was an error in CAPL sending the ethernet packet.
        '''

        if (len(data) > 1500):  # list size must not exceed 1500
            logger.error(f'Packet size too large, expected a maximum of 1500 bytes, received {len(data)}')
            raise Exception(f'Packet size too large, expected a maximum of 64 bytes, received {len(data)}')

        if not (1 <= vlan_id <= 4094):
            logger.error(f'Vlan ID must be between 1 and 4094, {vlan_id} is invalid')
            raise Exception(f'Vlan ID must be between 1 and 4094, {vlan_id} is invalid')

        if not (0 <= channel < 256):
            logger.error(f'Channel must be between 0 and 255, {channel} is invalid.')
            raise Exception(f'Channel must be between 0 and 255, {channel} is invalid.')

        if not (0 <= priority < 8):
            logger.error(f'Priority must be between 0 and 7, {priority} is invalid.')
            raise Exception(f'Priority must be between 0 and 7, {priority} is invalid.')

        current_counter = self._fdx_send_ethernet_packet_counter

        fdx_data = {  # update system variables with input values
            'SQTS::send_ethernet_packet::packet_buffer': bytes(data),
            'SQTS::send_ethernet_packet::counter': current_counter,
            'SQTS::send_ethernet_packet::dest_addr': destination_address,
            'SQTS::send_ethernet_packet::source_addr': source_address,
            'SQTS::send_ethernet_packet::vlanId': vlan_id,
            'SQTS::send_ethernet_packet::channel': channel,
            'SQTS::send_ethernet_packet::priority': priority,
            'SQTS::send_ethernet_packet::type': Type
        }

        self._fdx_send_ethernet_packet_counter += 2  # The sent counter is always an odd number, and the received counter even.

        # Send updated system variables data to CANoe through FDX, this triggers a CAPL on sysvar event
        # in sqts_simulation_node that assembles and sends the packet.
        logger.debug(f'Sending packet with counter {current_counter}...')
        self.fdx.send_data(fdx_data, group_id=FDX_SEND_PACKET_ID)

        # Once the CAPL script has sent the packet, it will trigger the transmission of that
        # same data group from CANoe to this script. wait for that response to verify
        # that the packet has been sent.
        logger.debug('Verifying that the packet was sent correctly...')
        start = time.time()
        received_data = self.fdx.receive_data(FDX_SEND_PACKET_ID, timeout_s=timeout_s)
        elapsed = time.time() - start
        while elapsed < timeout_s:
            # Verify that the check_counter is correct, it should be the sent counter plus 1
            if received_data['SQTS::send_ethernet_packet::check_counter'] == current_counter + 1:
                error_code = received_data['SQTS::send_ethernet_packet::error_code']
                if error_code != 0:
                    logger.error(f'There was an error sending the packet, error code: {error_code}.')
                    raise Exception(f'There was an error sending the packet, error code: {error_code}.')
                logger.debug('Packet sent correctly.')
                return
            else:
                received_data = self.fdx.receive_data(FDX_SEND_PACKET_ID, timeout_s=timeout_s-elapsed)
                elapsed = time.time() - start
        else:
            logger.error('Timeout expired while verifying that the packet was sent correctly')
            raise TimeoutError('Timeout expired while verifying that the packet was sent correctly')

    def expect_packet(
                    self,
                    vlan_id: int = 0,
                    source_address: str = "00:00:00:00:00:00",
                    destination_address: str = "00:00:00:00:00:00",
                    channel: int = 0,
                    type: int = ProtocolType.IPV4,
                    num_packets: int = 1
                    ) -> None:
        '''
        Method to tell the sqts_simulation_node to expect a packet and send it
        to the framework if it is received in CANoe.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            vlan_id: The VLAN ID of the expected packet.
            source_address: MAC address of source node.
            destination_address: MAC address of destination node.
            channel: The channel through which the packet should come from,
                    if set as 0, the packet can come through any channel.
            type: protocol type of expected packet.
            num_packets: the number of ethernet packets expected.

        Raises:
            Exception: A packet is already expected.
        '''
        logger.debug(f'Expecting {num_packets} packet(s) from MAC {source_address} to MAC {destination_address} with VLAN ID 0x{vlan_id:X} through channel {channel}...')

        # Check status of expected packet
        if self._expected_packet is not None:
            expected_id = self._expected_packet['vlan_id']

            logger.warning(f'Called expect_packet when another packet is already expected (ID 0x{expected_id:X}).')

        fdx_data = {    # sending expected values to system variables
            'SQTS::receive_ethernet_packet::expected_number_of_packets': num_packets,
            'SQTS::receive_ethernet_packet::expected_vlanId': vlan_id,
            'SQTS::receive_ethernet_packet::expected_channel': channel,
            'SQTS::receive_ethernet_packet::expected_source_addr': source_address,
            'SQTS::receive_ethernet_packet::expected_dest_addr': destination_address,
            'SQTS::receive_ethernet_packet::expected_type': type
        }

        # Send updated system variables data to CANoe through FDX, this tells a CAPL on ethernetPacket * event
        # in sqts_simulation_node to wait for a specific packet to arrive and store its data in
        # the SQTS::receive_ethernet_packet system variables
        logger.debug(f'Requesting CANoe to send packet 0x{vlan_id:X} from MAC {source_address} to MAC {destination_address} after it arrives in the bus...')
        self.fdx.send_data(fdx_data, group_id=FDX_RECEIVE_PACKET_ID)

        # Update status of expected packet
        self._expected_packet = {
            'vlan_id': vlan_id,
            'num_expected_packets': num_packets,
            'type': type
        }

    def retrieve_packet(
                    self,
                    timeout_s: float = 1.0
                    ) -> EthernetPacket | EthPTPpacket:
        '''
        Method to receive a packet from an expected packet.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            timeout_s: Timeout to wait for the packet to come through.

        Returns:
            The received ethernet packet.

        Raises:
            Exception: No packet is expected.
            Exception: There was an error retrieving the packet data from CAPL.
            TimeoutError: The timeout expired while waiting for the packet.
        '''

        if self._expected_packet is None:
            logger.error('Tried to retrieve a packet when no packet was expected.')
            raise Exception('Cannot retrieve packet, no packet was expected.')

        # Once the CAPL event handler has stored the data in system variables, it will trigger
        # the transmission of the receive_packet data group through FDX, wait for that data
        # and build the EthernetPacket from it.
        vlan_id = self._expected_packet['vlan_id']
        logger.debug(f'Retrieving packet with ID 0x{vlan_id:X} from CANoe...')

        received_data = self.fdx.receive_data(FDX_RECEIVE_PACKET_ID, timeout_s=timeout_s)

        logger.debug('Received response from CANoe.')
        error_code = received_data['SQTS::receive_ethernet_packet::error_code']
        if error_code != 0:
            logger.error(f'There was an error retrieving the packet data, error code: {error_code}')
            raise Exception(f'There was an error retrieving the packet data, error code: {error_code}')

        # if the expected protocol type is PTP
        if (self._expected_packet['type'] == ProtocolType.PTP):
            packet = EthPTPpacket(  # build PTP ethernet packet from received system values
                                hwport=received_data['SQTS::receive_ethernet_packet::hwport'],
                                msgtype=received_data['SQTS::receive_ethernet_packet::msgtype'],
                                time=received_data['SQTS::receive_ethernet_packet::time'],
                                source_port_number=received_data['SQTS::receive_ethernet_packet::sourcePortNumber'],
                                domain_number=received_data['SQTS::receive_ethernet_packet::domainNumber'],
                                vlan_id=received_data['SQTS::receive_ethernet_packet::vlanId'],
                                is_ptp=received_data['SQTS::receive_ethernet_packet::isPTP'],
                                destination=received_data['SQTS::receive_ethernet_packet::dest_addr']
                            )

        else:
            packet = EthernetPacket(    # build ethernet packet from received system values
                                    channel=received_data['SQTS::receive_ethernet_packet::channel'],
                                    protocolType=received_data['SQTS::receive_ethernet_packet::type'],
                                    source_address=received_data['SQTS::receive_ethernet_packet::source_addr'],
                                    destination_address=received_data['SQTS::receive_ethernet_packet::dest_addr'],
                                    data=received_data['SQTS::receive_ethernet_packet::packet_buffer']
                                )

        logger.debug(f'Received packet {packet}.')

        # Update status of expected packet
        if self._expected_packet['num_expected_packets'] <= 1:
            logger.debug('No more packets expected.')
            self._expected_packet = None
        else:
            self._expected_packet['num_expected_packets'] -= 1
            num_expected_packets = self._expected_packet['num_expected_packets']
            logger.debug(f'{num_expected_packets} packet(s) still expected.')

        return packet

    def receive_packet(
            self,
            vlan_id: int = 0,
            source_address: str = "00:00:00:00:00:00",
            destination_address: str = "00:00:00:00:00:00",
            channel: int = 0,
            timeout_s: float = 1.0,
            type: int = ProtocolType.IPV4,
            num_packets: int = 1

            ) -> EthernetPacket:

        '''
        Method to receive a packet that has yet to be sent through the bus.

        NOTE:
            start_exe and load_config must be called first, and the measurement must be running.

        Args:
            vlan_id: The VLAN ID of the expected packet.
            source_address: MAC address of source node.
            destination_address: MAC address of destination node.
            channel: The channel through which the packet should come from,
                    if set as 0, the packet can come through any channel.
            type: protocol type of expected packet.
            num_packets: the number of ethernet packets expected.
            timeout_s: Timeout to wait for the packet to come through.

        Returns:
            The received ethernet packet.

        Raises:
            TimeoutError: The timeout expired while waiting for the packet.
            Exception: There was an error retrieving the packet data from CAPL.
        '''
        self.expect_packet(vlan_id=vlan_id, channel=channel, source_address=source_address, destination_address=destination_address, type=type, num_packets=num_packets)
        return self.retrieve_packet(timeout_s=timeout_s)


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/
#             Initials     JIRA     Explanation of changes done here.
#    Date        By       AAA-####      Description
# ----------  ---------   --------  ----------------------------------
# 06/30/2023  Kushagra G. FKU-854   - Defined EthernetPacket and EthernetHander classes with __init__ functions.
#                                   - Added send_packet, expect_packet and receive_packet functions.
# 07/05/2023  Kushagra G. FKU-1002  Added expected source and destination MAC addresses to expect_packet() function.
#
# 07/20/2023  Kushagra G. FKU-1016  Added EthPTPpacket class and functionality to read PTP variables from sysvars in receive_packet()
#
# 07/25/2023  Kushagra G. FKU-1021  Added send_diagnostic function to support transmission and retreival of diagnostic values
#                                   from CAPL scripts. Added DiagnosticType class for types of diagnostic requests.
