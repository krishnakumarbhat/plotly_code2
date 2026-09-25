"""SOME/IP Service Discovery subscription/unsubscription utility.

This script allows subscribing or unsubscribing to SOME/IP services for various sensors.
It constructs and sends the appropriate SOME/IP Service Discovery packets over Ethernet.
The user can specify the operation (subscribe/unsubscribe) and the target sensor (SRR_RL, SRR_RR, SRR_FL, SRR_FR, FLR, or ALL).
This can be used for the bench testing for Gen7V2.
Make sure to update interface name ETHERNET_INTERFACE variable according to your setup.
Usage: python someip_sub_unsub.py <operation> <sensor>
"""
from scapy.all import sendp, Ether, IP, get_if_hwaddr, UDP
from scapy.packet import Packet
from scapy.fields import (
    PacketListField,
    ByteField,
    XByteField,
    XShortField,
    XIntField,
    IPField,
    BitField,
    ByteEnumField,
    FieldLenField,
    Field,
)
import struct
import sys

# Network Configuration
STARTUP_IP = "239.192.255.251"
ASDM_IP = "192.168.1.100"
NODE1_IP = "192.168.1.71"
NODE2_IP = "192.168.1.72"
NODE3_IP = "192.168.1.73"
NODE4_IP = "192.168.1.74"
NODE5_IP = "192.168.1.75"
INSTANCE_ID = 1
GRP_DES_IP = "224.0.0.22"
GRP_IP = "224.0.0.251"
MULTICAST_GRP = "239.192.255.251"
ETHERNET_INTERFACE = "Ethernet"  # TODO: Update this to your actual interface name

# Valid sensor arguments
allowed_sensors = {"SRR_RL", "SRR_RR", "SRR_FL", "SRR_FR", "FLR", "ALL"}
allowed_operations = {"subscribe", "unsubscribe", "sub", "unsub"}


def print_usage():
    """Print usage instructions for the script."""
    print("Usage: python someip_sub_unsub.py <operation> <sensor>")
    print("Operations: subscribe|sub, unsubscribe|unsub")
    print("Sensors: SRR_RL|SRR_RR|SRR_FL|SRR_FR|FLR|ALL")
    print("Example: python someip_sub_unsub.py subscribe SRR_RL")
    print("Example: python someip_sub_unsub.py unsub ALL")


if len(sys.argv) != 3:
    print_usage()
    sys.exit(1)

operation = sys.argv[1].lower()
sensor = sys.argv[2]

if operation not in allowed_operations:
    print(f"Invalid operation: {operation}. Allowed values are: {', '.join(allowed_operations)}")
    print_usage()
    sys.exit(1)

if sensor not in allowed_sensors:
    print(f"Invalid sensor: {sensor}. Allowed values are: {', '.join(allowed_sensors)}")
    print_usage()
    sys.exit(1)

# Normalize operation
is_subscribe = operation in ["subscribe", "sub"]


class IGMPv3GrpRecord(Packet):
    """IGMPv3 Group Record packet structure."""

    name = "IGMPv3 Group Record"
    fields_desc = [
        ByteField("record_type", 4),
        ByteField("aux_data_len", 0),
        XShortField("num_sources", 0),
        IPField("multicast_address", "0.0.0.0"),
    ]


class IGMPv3(Packet):
    """IGMPv3 packet structure."""

    name = "IGMPv3"
    fields_desc = [
        ByteField("type", 0x22),
        XByteField("maxRespCode", 0),
        XShortField("chksum", None),
        XShortField("resv", 0),
        XShortField("numGrpRecords", 1),
        PacketListField(
            "group_records", None, IGMPv3GrpRecord, count_from=lambda pkt: pkt.numGrpRecords
        ),
    ]


class ThreeBytesField(Field):
    """Custom field for handling 3-byte values in SOME/IP packets."""

    def __init__(self, name, default):
        """Initialize the ThreeBytesField.

        Args:
            name: Field name
            default: Default value
        """
        Field.__init__(self, name, default, "!I")

    def addfield(self, pkt, s, val):
        """Add field to packet."""
        return s + struct.pack(self.fmt, self.i2m(pkt, val) & 0xFFFFFF)[1:]

    def getfield(self, pkt, s):
        """Get field from packet."""
        return s[3:], self.m2i(pkt, struct.unpack(self.fmt, b"\x00" + s[:3])[0])


class SOMEIP(Packet):
    """SOME/IP protocol packet structure."""

    name = "SOME/IP Protocol"
    fields_desc = [
        XShortField("Service_ID", 0xFFFF),
        XShortField("Method_ID", 0x8100),
        XIntField("Length", None),
        XShortField("Client_ID", 0x0000),
        XShortField("Session_ID", 0x0001),
        XByteField("SOMEIP_Version", 0x01),
        XByteField("Interface_Version", 0x01),
        XByteField("Message_Type", 0x02),
        XByteField("Return_Code", 0x00),
    ]

    def post_build(self, p, pay):
        """Build the packet with correct length field."""
        if self.Length is None:
            length = len(pay)
            p = p[:2] + struct.pack("!H", length) + p[4:]
        return p + pay


class SDEntry_Sub(Packet):
    """Service Discovery Subscribe Eventgroup Entry."""

    name = "Subscribe Eventgroup Entry"
    fields_desc = [
        XByteField("Type", 0x06),
        XByteField("Index_1", 0x00),
        XByteField("Index_2", 0x00),
        XByteField("n_opt_1_2", 0x00),
        XShortField("Service_ID", 0x0011),
        XShortField("Instance_ID", 0x0001),
        XByteField("Major_Version", 0x01),
        ThreeBytesField("TTL", 0xFFFFFF),
        XByteField("Reserved1", 0x00),
        XByteField("Reserved2", 0x00),
        XShortField("Eventgroup_ID", 0x0001),
    ]


class SDOption(Packet):
    """Service Discovery Option packet structure."""

    name = "Service Discovery Option"
    fields_desc = [
        XShortField("len", 0x09),
        XByteField("type", 0x04),
        XByteField("res_hdr", 0x00),
        IPField("addr", ASDM_IP),
        XByteField("res_tail", 0x00),
        ByteEnumField("l4_proto", 0x11, {0x11: "UDP"}),
        XShortField("port", 30501),
    ]


class SOMEIP_SD(Packet):
    """SOME/IP Service Discovery protocol packet structure."""

    name = "SOME/IP Service Discovery Protocol"
    fields_desc = [
        BitField("Flags", 0x0, 8),
        ThreeBytesField("Reserved", 0x000000),
        FieldLenField("len_entry_array", None, length_of="entry_array", fmt="!I"),
        PacketListField(
            "entry_array", None, SDEntry_Sub, length_from=lambda pkt: pkt.len_entry_array
        ),
        FieldLenField("len_option_array", None, length_of="option_array", fmt="!I"),
        PacketListField(
            "option_array", None, SDOption, length_from=lambda pkt: pkt.len_option_array
        ),
    ]


def build_packet_instance_1(subscribe=True):
    """Build packet for SRR_RL (Instance 1)."""
    dst_mac = "28:63:bd:23:bb:71"
    eth = Ether(src=get_if_hwaddr(ETHERNET_INTERFACE), dst=dst_mac)
    ip = IP(src=ASDM_IP, dst=NODE1_IP)
    udp = UDP(sport=30490, dport=30490)

    # Use 0x10 for subscribe, 0x20 for unsubscribe
    opt_value = 0x10 if subscribe else 0x20

    service_entries = [
        SDEntry_Sub(
            Service_ID=0x0011,
            Instance_ID=INSTANCE_ID,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        SDEntry_Sub(
            Service_ID=0x0012,
            Instance_ID=INSTANCE_ID,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        # TODO: Temp workaround to remove subscription to the detection service
        # SDEntry_Sub(
        #     Service_ID=0x0010,
        #     Instance_ID=INSTANCE_ID,
        #     n_opt_1_2=opt_value,
        #     Eventgroup_ID=0x0001,
        #     Index_1=0x01,
        # ),
    ]
    options = [SDOption(), SDOption()]
    someip_sd = SOMEIP_SD(Flags=0xC0, entry_array=service_entries, option_array=options)
    someip = SOMEIP(Length=len(someip_sd) + 8)

    return eth / ip / udp / someip / someip_sd


def build_packet_instance_2(subscribe=True):
    """Build packet for SRR_RR (Instance 2)."""
    dst_mac = "28:63:bd:23:bb:72"
    eth = Ether(src=get_if_hwaddr(ETHERNET_INTERFACE), dst=dst_mac)
    ip = IP(src=ASDM_IP, dst=NODE2_IP)
    udp = UDP(sport=30490, dport=30490)

    opt_value = 0x10 if subscribe else 0x20

    service_entries = [
        SDEntry_Sub(
            Service_ID=0x0011,
            Instance_ID=2,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        SDEntry_Sub(
            Service_ID=0x0012,
            Instance_ID=2,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        # SDEntry_Sub(
        #     Service_ID=0x0010,
        #     Instance_ID=2,
        #     n_opt_1_2=opt_value,
        #     Eventgroup_ID=0x0001,
        #     Index_1=0x01,
        # ),
    ]
    options = [SDOption(), SDOption()]
    someip_sd = SOMEIP_SD(Flags=0xC0, entry_array=service_entries, option_array=options)
    someip = SOMEIP(Length=len(someip_sd) + 8)

    return eth / ip / udp / someip / someip_sd


def build_packet_instance_3(subscribe=True):
    """Build packet for SRR_FR (Instance 3)."""
    dst_mac = "28:63:bd:23:bb:73"
    eth = Ether(src=get_if_hwaddr(ETHERNET_INTERFACE), dst=dst_mac)
    ip = IP(src=ASDM_IP, dst=NODE3_IP)
    udp = UDP(sport=30490, dport=30490)

    opt_value = 0x10 if subscribe else 0x20

    service_entries = [
        SDEntry_Sub(
            Service_ID=0x0011,
            Instance_ID=3,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        SDEntry_Sub(
            Service_ID=0x0012,
            Instance_ID=3,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        # SDEntry_Sub(
        #     Service_ID=0x0010,
        #     Instance_ID=3,
        #     n_opt_1_2=opt_value,
        #     Eventgroup_ID=0x0001,
        #     Index_1=0x01,
        # ),
    ]
    options = [SDOption(), SDOption()]
    someip_sd = SOMEIP_SD(Flags=0xC0, entry_array=service_entries, option_array=options)
    someip = SOMEIP(Length=len(someip_sd) + 8)

    return eth / ip / udp / someip / someip_sd


def build_packet_instance_4(subscribe=True):
    """Build packet for SRR_FL (Instance 4)."""
    dst_mac = "28:63:bd:23:bb:74"
    eth = Ether(src=get_if_hwaddr(ETHERNET_INTERFACE), dst=dst_mac)
    ip = IP(src=ASDM_IP, dst=NODE4_IP)
    udp = UDP(sport=30490, dport=30490)

    opt_value = 0x10 if subscribe else 0x20

    service_entries = [
        SDEntry_Sub(
            Service_ID=0x0011,
            Instance_ID=4,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        SDEntry_Sub(
            Service_ID=0x0012,
            Instance_ID=4,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        # SDEntry_Sub(
        #     Service_ID=0x0010,
        #     Instance_ID=4,
        #     n_opt_1_2=opt_value,
        #     Eventgroup_ID=0x0001,
        #     Index_1=0x01,
        # ),
    ]
    options = [SDOption(), SDOption()]
    someip_sd = SOMEIP_SD(Flags=0xC0, entry_array=service_entries, option_array=options)
    someip = SOMEIP(Length=len(someip_sd) + 8)

    return eth / ip / udp / someip / someip_sd


def build_packet_instance_5(subscribe=True):
    """Build packet for FLR (Instance 5)."""
    dst_mac = "28:63:bd:23:bb:75"
    eth = Ether(src=get_if_hwaddr(ETHERNET_INTERFACE), dst=dst_mac)
    ip = IP(src=ASDM_IP, dst=NODE5_IP)
    udp = UDP(sport=30490, dport=30490)

    opt_value = 0x10 if subscribe else 0x20

    service_entries = [
        SDEntry_Sub(
            Service_ID=0x0011,
            Instance_ID=5,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        SDEntry_Sub(
            Service_ID=0x0012,
            Instance_ID=5,
            n_opt_1_2=opt_value,
            Eventgroup_ID=0x0001,
            Index_1=0x01,
        ),
        # SDEntry_Sub(
        #     Service_ID=0x0010,
        #     Instance_ID=5,
        #     n_opt_1_2=opt_value,
        #     Eventgroup_ID=0x0001,
        #     Index_1=0x01,
        # ),
    ]
    options = [SDOption(), SDOption()]
    someip_sd = SOMEIP_SD(Flags=0xC0, entry_array=service_entries, option_array=options)
    someip = SOMEIP(Length=len(someip_sd) + 8)

    return eth / ip / udp / someip / someip_sd


def send_packet(packet, sensor_name, operation):
    """Send packet and provide feedback."""
    try:
        sendp(packet, iface=ETHERNET_INTERFACE, verbose=False)
        action = "Subscribed to" if is_subscribe else "Unsubscribed from"
        print(f"{action} {sensor_name} successfully")
    except Exception as e:
        print(f"Error sending packet for {sensor_name}: {e}")


if __name__ == "__main__":

    # Build packets based on operation type
    packet_builders = {
        "SRR_RL": build_packet_instance_1,
        "SRR_RR": build_packet_instance_2,
        "SRR_FR": build_packet_instance_3,
        "SRR_FL": build_packet_instance_4,
        "FLR": build_packet_instance_5,
    }

    sensor_names = {
        "SRR_RL": "SRR Rear Left",
        "SRR_RR": "SRR Rear Right",
        "SRR_FR": "SRR Front Right",
        "SRR_FL": "SRR Front Left",
        "FLR": "Front Long Range",
    }

    action_word = "Subscribing to" if is_subscribe else "Unsubscribing from"

    if sensor == "ALL":
        print(f"{action_word} all sensors...")
        for sensor_key in packet_builders:
            packet = packet_builders[sensor_key](is_subscribe)
            send_packet(packet, sensor_names[sensor_key], operation)
        print("Operation completed for all sensors.")
    else:
        print(f"{action_word} {sensor_names[sensor]}...")
        packet = packet_builders[sensor](is_subscribe)
        send_packet(packet, sensor_names[sensor], operation)
        print("Operation completed.")
