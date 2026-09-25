"""
This test checks for Ethernet data every second and power cycles if it ever stops.

The purpose of this test is to check that UDP data is being sent and its checked every
interval and displays the bandwidth. If the bandwidth is ~0 then the radar is assumed
to have dropped out so the power cycle is cycles and the checking of bandwidth starts over.

This script needs a programmable power supply, media gateway and radar to test.


The following are the inputs:

[-t Test duration]: Test time duration in seconds
[-i Time inverval]: Test interval time to check the bandwidth in seconds
[-v Platform Variant]: srr7p, srr7hd, flr7, defaults to srr7p
[-p Sensor Position]: Sensor position
[-e Ethernet interface]: Ethernet port, defaults to "Ethernet 3"
[-k Power Supply COM port]: Power supply USB COM port, defaults to COM3
[-a elf File]: Elf file from the build use to connect XCP and read/write as necessary

"""
# !/usr/bin/python

import time
import argparse
import psutil
import sys
from os import path


BASE_PATH = path.dirname(path.abspath(__file__))
sys.path.insert(
    0,
    path.abspath(path.join(BASE_PATH, "..", "..", "..", "tools", "python", "power_supply")),
)
sys.path.insert(
    0,
    path.abspath(path.join(BASE_PATH, "..", "..", "..", "tools", "python", "xcpHandler")),
)

# Local libraries
from koradserial import KoradSerial  # noqa: E402
from XcpHandler import XcpHandler  # noqa: E402

#  OPTIONS
DEFAULT_KORAD_COM = "COM3"
DEFAULT_VARIANT = "SRR7p"
DEFAULT_ETH_COM = "Ethernet 3"
STARTUP_DELAY_TIME = 25

# Global space main loop
bw = 0
tot_cnt = 0


# ##############################################################################3
if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="APTIV Gen7 Radar UDP Check Test",
        epilog="APTIV Ltd, copyright 2023",
    )
    parser.add_argument(
        "-t",
        "--total_time",
        metavar="Test duration",
        type=int,
        required=False,
        help="Test time duration in seconds",
        default=60 * 60 * 80,
    )
    parser.add_argument(
        "-i",
        "--interval",
        metavar="Time inverval",
        type=int,
        required=False,
        help="Test interval time to check the bandwidth in seconds",
        default=10,
    )
    parser.add_argument(
        "-v",
        "--variant",
        metavar="Platform Variant",
        type=str,
        default=DEFAULT_VARIANT,
        required=False,
        help="Platform Variant (srr7p)",
    )
    parser.add_argument(
        "-p",
        "--position",
        metavar="Sensor Position",
        type=str,
        default=1,
        required=False,
        help="Sensor Position (1-14)",
    )
    parser.add_argument(
        "-e",
        "--ethnic",
        metavar="Ethernet interface",
        type=str,
        default=DEFAULT_ETH_COM,
        required=False,
        help="Ethernet interface name as listed",
    )
    parser.add_argument(
        "-k",
        "--comPort",
        metavar="Power Supply COM port",
        type=str,
        default=DEFAULT_KORAD_COM,
        required=False,
    )
    parser.add_argument(
        "-a", "--elfFile", metavar="elf File", type=str, default="", required=False
    )
    args = parser.parse_args()

    total_time = args.total_time
    variant = args.variant
    eth_nic = args.ethnic
    comPort = args.comPort
    elfFile = args.elfFile
    interval = args.interval

    print("Starting UDP Check Test")
    print("Arguments: {0}".format(sys.argv[1:]))

    powerSupply = KoradSerial(comPort, False)
    if powerSupply.connected is False:
        exit()

    powerSupply.output.off()
    time.sleep(2)
    chn = powerSupply.channels[0]
    # Set voltage and current settings
    chn.voltage = 12.00
    chn.current = 3.00
    powerSupply.output.on()
    print("Waiting for startup delay...")

    if path.isfile(elfFile):
        time.sleep(2)
        xcpHandler = XcpHandler(elfFile)

        if xcpHandler.xcpInit():
            addr_gXCP_Delay, size = xcpHandler.getAddress("gXCP_Delay")
            xcpHandler.writeCal(addr_gXCP_Delay, 1, 5)
            xcpHandler.disconnect()
    else:
        time.sleep(STARTUP_DELAY_TIME)

    powerSupplyChecks = 10

    current = chn.output_current
    voltage = chn.output_voltage
    if current is not None:
        while current < 0.310 and powerSupplyChecks > 0:
            print("Power Supply: " + str(current) + "A, " + str(voltage) + "V")
            time.sleep(2)
            current = chn.output_current
            voltage = chn.output_voltage
            powerSupplyChecks = powerSupplyChecks - 1

    if powerSupplyChecks == 0:
        print("ERROR: Current draw never reached expected level")
        exit()

    print("Power Supply: " + str(current) + "A, " + str(voltage) + "V")

    net_bw_prev = 0
    while True:

        if tot_cnt >= total_time:
            break

        net_data = psutil.net_io_counters(pernic=True)
        net_bw = net_data[eth_nic].bytes_recv
        bw = (net_bw - net_bw_prev) / 1024.0 / 1024.0 * 8  # variable in megabits

        net_bw_prev = net_bw

        time.sleep(1)

        if (tot_cnt % interval) == 0:
            print("(%d/%d) UDP detected. Bandwidth = %.2f Mbps" % (tot_cnt, total_time, bw))

        if bw == 0:
            print("No UDP detected. Cycling power")
            powerSupply.output.off()
            time.sleep(0.1)
            powerSupply.output.on()
            time.sleep(STARTUP_DELAY_TIME)
            totcnt = tot_cnt + STARTUP_DELAY_TIME

        tot_cnt = tot_cnt + 1

    # Finish test loop
    print("\nTest Complete")
    powerSupply.output.off()
