"""
This is the Integration test that is run as part of the jenkins smoke test.

This script needs a programmable power supply, media gateway and radar to test.
The tests run in this file are checking for things like scan index skips, reading temps, xput, faults, etc.

[-t Test duration]: Test time duration in seconds
[-v Platform Variant]: srr7p, srr7hd, flr7, defaults to srr7p
[-p Sensor Position]: Sensor position (1-14), (not used)
[-e Ethernet interface]: Ethernet port, defaults to "Ethernet 3"
[-k Power Supply COM port]: Power supply USB COM port, defaults to COM3
[-g Create Graphs]: Enable/disable for adding graphs to report
[-c Test Comments]: Comments to add to the html report
[-m Max Throughput Test]: Flag to enable/disable max throupugh test
[-s Test Source Test]: Test source test (not complete)
[-a elf File]: Elf file from the build use to connect XCP and read/write as necessary
[-b Branch]: Git branch, used for tagging data sent to datadog
[-w wiresharklua]: Wireshare lua script path to use durning testing
[-o outputHtml]: Output html path name, default is a name with variant and timestamp
[-r swVersion]: sw version to add to comments.

"""
# !/usr/bin/python
import logging
import logging.handlers
import time

import tqdm
import argparse
import psutil
import platform
import shelve
from datetime import datetime
from statistics import mean
from os import path, getcwd, remove, walk, system
from multiprocessing import Process, Queue, Pipe, SimpleQueue
from threading import Thread, Event
import pyshark
import numpy
import sys
import os

# adding libs to the system path
BASE_PATH = path.dirname(path.abspath(__file__))
sys.path.insert(
    0, path.abspath(path.join(BASE_PATH, "..", "..", "..", "tools", "python", "power_supply"))
)
sys.path.insert(
    0, path.abspath(path.join(BASE_PATH, "..", "..", "..", "tools", "python", "tester"))
)
sys.path.insert(
    0, path.abspath(path.join(BASE_PATH, "..", "..", "..", "tools", "python", "xcpHandler"))
)
sys.path.insert(
    0, path.abspath(path.join(BASE_PATH, "..", "..", "..", "tools", "python", "dataDogMetrics"))
)

# Local libraries
from koradserial import KoradSerial  # noqa: E402
from tester import Tester  # noqa: E402
from XcpHandler import XcpHandler  # noqa: E402
from dataDogMetrics import dataDogStats  # noqa: E402

#  OPTIONS
ADD_PLOTS = True
DEFAULT_KORAD_COM = "COM3"
DEFAULT_VARIANT = "SRR7p"
DEFAULT_ETH_COM = "Ethernet 3"
STARTUP_DELAY_TIME = 25

DISPLAY_XCP_VARS = False
CODED_FALSE = 0x55
CODED_TRUE = 0xAA
ONE_BYTE = 1

TX_RATE = 50
TX_RATE_TOLERANCE = TX_RATE * 0.10

# local directories to save reports and test data
TEST_DATA_PATH = getcwd() + r"\\testData\\"
REPORT_PATH = getcwd() + r"\\reports\\"
# User app data path for wireshare plugins to store the lua script
WIRESHARK_PLUGIN_DIR = path.join(str(os.getenv("APPDATA")) + "\\Wireshark\\plugins\\")

# Shelf files to store recorded Ethernet data
BRR_SHELF = TEST_DATA_PATH + "brr_shelf"
XCP_SHELF = TEST_DATA_PATH + "xcp_shelf"

# Global space main loop
test_time = 0
net_bw_exit = Event()
avg_eth_bandwidth = []
avg_eth_bandwidth2 = []
brr1_exit_event = Event()
brr2_exit_event = Event()
xcp_exit_event = Event()

brr_signals_data = {}

prev_hwtmstmp = None
prev_scan_idx = None
scan_idx_cntr = 0
scan_repeating_cntr = 0
af_overrun_cntr = 0
rdd_overrun_cntr = 0
can_log = logging.getLogger()


CAN_LOOK_ID = {"LOOK_A": 0, "LOOK_B": 1, "LOOK_C": 2, "LOOK_D": 3}


class tester_Error(Exception):
    """
    This class handles error and prints out descriptions for them.
    """

    error_codes = {
        "CAN_rx_to": "CAN timeout waiting for messages",
        "CAN_bus_error": "CAN bus driver error",
        "Ethernet_NIC_error": "Unknown ethernet network interface",
        "CAN_tools_error": "Problem loading CAN database",
    }

    def __init__(self, msg, code):
        """
        Initialize the error string with the fault description.
        """
        self.error_str = msg
        code_str = self.error_codes.get(code)
        if code_str is None:
            code_str = "No Additional Info"
        self.error_str += ": " + code_str

    def __str__(self):
        """
        Return the error string.
        """
        return repr(self.error_str)


def average(lst):
    """
    Calculate the average of a list and return the value.
    """
    return sum(lst) / len(lst)


def countMoreThan(lst, limit, disp=0):
    """
    Return a string formatted to display a total number of occurances in a list above a limit.

    Parameters:
        lst: list of values
        limit: specific limit to check easy value in the list against.
        disp: 0 for the default percentage string format, anything else will just show a count over total.
    """
    strVal = ""
    cnt = sum(i > limit for i in lst)

    if disp == 0:
        strVal = "{0} %".format(cnt / len(lst) * 100)
    else:
        strVal = "{0}/{1}".format(cnt, len(lst))

    return strVal


def countLessThan(lst, limit, disp=0):
    """
    Return a string formatted to display a total number of occurances in a list below a limit.

    Parameters:
        lst: list of values
        limit: specific limit to check easy value in the list against.
        disp: 0 for the default percentage string format, anything else will just show a count over total.
    """
    strVal = ""
    cnt = sum(i < limit for i in lst)

    if disp == 0:
        strVal = "{0} %".format(cnt / len(lst) * 100)
    else:
        strVal = "{0}/{1}".format(cnt, len(lst))
    return strVal


def countBetween(lst, high, low, disp=0):
    """
    Return a string formatted to display a total number of occurances between a high and low limit.

    Parameters:
        lst: list of values
        high: high value to compare against.
        low: low value to compare against.
        disp: 0 for the default percentage string format, anything else will just show a count over total.
    """
    strVal = ""
    cnt = sum(i > low and i < high for i in lst)
    if disp == 0:
        strVal = "{0} %".format(cnt / len(lst) * 100)
    else:
        strVal = "{0}/{1}".format(cnt, len(lst))
    return strVal


def checkSlowSkips(values):
    """
    Return the number of skips in a list of values.

    Parameters:
        values: list of values
    """
    skipCntr = 0
    lastIndex = 0
    for index in values:
        if lastIndex != 0:
            if (int(index) == int(lastIndex)) or (int(index) == int(lastIndex) + 1):
                skipCntr = skipCntr
            else:
                skipCntr += 1
        lastIndex = int(index)
    return skipCntr


def skipCounter(values):
    """
    Return the number of skips in a list of values.

    Parameters:
        values: list of values
    """
    skipCntr = 0
    lastIndex = None
    for index in values:
        if lastIndex is not None:
            if int(index) != (int(lastIndex) + 1):
                if (int(index) == 0) and (int(lastIndex) == 65535):
                    skipCntr = skipCntr
                else:
                    skipCntr += 1
        lastIndex = int(index)
    return skipCntr


def repeatCounter(values):
    """
    Return the number of repeats in a list of values.

    Parameters:
        values: list of values
    """
    repeatCntr = 0

    last = None
    for i in values:
        if last is not None:
            if i == last:
                repeatCntr += 1
        last = i
    return repeatCntr


def get_ns_diff_data(values):
    """
    Return the difference in nanoseconds between the values.

    Parameters:
        values: list of values
    """
    MAX_NS = 1000000000
    l2 = []
    lastns = values[0]

    for ns in values[1:]:
        if ns > lastns:
            new = ns - lastns
        else:
            new = (MAX_NS - lastns) + ns

        l2.append(new)
        lastns = ns

    _min = float(min(l2) / 1000000)
    _max = float(max(l2) / 1000000)
    _mean = float(mean(l2) / 1000000)

    return _min, _max, _mean


def checkStuck(values):
    """
    Check if all items in an array are equal.

    Parameters:
        values: list of values
    """
    # Check if all items in an array are equal
    result = numpy.max(values) == numpy.min(values)

    stuckStatus = int(result)

    if (stuckStatus == 1) and (stuckStatus != values[0]):  # they match
        stuckStatus = 0
    return stuckStatus


def getToggleStuckStatus(values):
    """
    Check if values in the array are toggling.

    Parameters:
        values: list of values
    """
    togglingStatus = 0
    nonZeroCnt = 0
    zeroCnt = 0

    nonZeroCnt = numpy.count_nonzero(values)
    zeroCnt = len(values) - nonZeroCnt

    if nonZeroCnt > 0 and zeroCnt > 0:
        togglingStatus = 1

    if togglingStatus == 0 and zeroCnt == 0 and nonZeroCnt > 0:
        # Stuck, return the stuck value
        togglingStatus = max(values)
    # print("nonZero : {0}, zero : {1}, toggling: {2}".format(nonZeroCnt,zeroCnt,toggling))
    return togglingStatus


def getToggleStatus(values):
    """
    Check if values in the array are toggling.

    Parameters:
        values: list of values
    """
    toggling = 0
    nonZeroCnt = 0
    zeroCnt = 0

    nonZeroCnt = numpy.count_nonzero(values)
    zeroCnt = len(values) - nonZeroCnt

    if nonZeroCnt > 0 and zeroCnt > 0:
        toggling = 1
    # print("nonZero : {0}, zero : {1}, toggling: {2}".format(nonZeroCnt,zeroCnt,toggling))
    return toggling


def getErrorStatus(values):
    """
    Return 1 if there are any non zero values in a list.

    Parameters:
        values: list of values
    """
    errSts = 0
    errCnt = numpy.count_nonzero(values)
    if errCnt > 0:
        errSts = 1

    return errSts


def getErrorStatusCnt(values):
    """
    Return total of non zero values in a list.

    Parameters:
        values: list of values
    """
    errCnt = 0
    errCnt = numpy.count_nonzero(values)
    return errCnt


def checkSkips(values):
    """
    Return total number of skips in a list.

    Parameters:
        values: list of values
    """
    skipCntr = 0
    lastIndex = 0
    for index in values:
        if lastIndex != 0:
            if int(index) != (int(lastIndex) + 1):
                if (int(index) == 0) and (int(lastIndex) == 65535):
                    skipCntr = skipCntr
                else:
                    skipCntr += 1
        lastIndex = int(index)
    return skipCntr


def checkRepeats(values):
    """
    Return total number of repeats in a list.

    Parameters:
        values: list of values
    """
    repeatCntr = 0
    my_dict = {i: values.count(i) for i in values}

    for i in my_dict:
        if my_dict[i] > 1:
            repeatCntr += 1

    return repeatCntr


def saveLuaScript(luaScript):
    """Save the lua script to the wireshark director."""
    if luaScript is not None:

        luaScript = os.getcwd() + "\\" + luaScript.replace(r"/", "\\")

        try:
            if path.exists(luaScript):
                # Copy the new wireshark lua script since it can change between variants and different builds
                print('copy /y "{}" "{}"'.format(luaScript, WIRESHARK_PLUGIN_DIR))
                system('copy /y "{}" "{}"'.format(luaScript, WIRESHARK_PLUGIN_DIR))
            else:
                print("Lua script doesnt exist")
        except Exception as e:
            print(str(e))
            print("Couldnt copy wireshark file")
            pass
    else:
        print("Lua script is empty!")


def readLuaScript():
    """
    Print out the wireshark dissector file name.
    """
    for _root, _dirs, files in walk(WIRESHARK_PLUGIN_DIR):
        for file in files:
            if file.endswith(".lua"):
                # print("Wireshark Dissector: " + file)
                return file
                break


def checkCycling(values, cycleMin, cycleMax):
    """
    Return true or fals if the values in the array are cycling and in what direction.

    Parameters:
        values: list of values
        min: min value of the cycle
        max: max value of the cycle
    """
    cylingPassed = 0
    lastValue = None
    isIncrementing = True

    if numpy.min(values) == cycleMin:
        if numpy.max(values) == cycleMax:
            for i in values:
                currentValue = i
                if lastValue is not None:

                    if (currentValue == (lastValue + 1)) or (
                        currentValue == cycleMin and lastValue == cycleMax
                    ):
                        isIncrementing = True
                    elif (currentValue == (lastValue - 1)) or (
                        currentValue == cycleMax and lastValue == cycleMin
                    ):
                        isIncrementing = False

                    if isIncrementing is True:
                        if lastValue != cycleMax:
                            if lastValue == (currentValue - 1):
                                cylingPassed = 1
                                pass
                            else:
                                cylingPassed = 0
                                # print(str(l) + " - " + str(currentValue))
                                break
                        else:
                            if lastValue == cycleMax and currentValue == cycleMin:
                                cylingPassed = 1
                                pass
                            else:
                                cylingPassed = 0
                                # print(str(l) + " - " + str(currentValue))
                                break
                    else:
                        if lastValue != cycleMin:
                            if lastValue == (currentValue + 1):
                                cylingPassed = 1
                                pass
                            else:
                                cylingPassed = 0
                                # print(str(l) + " - " + str(currentValue))
                                break
                        else:
                            if lastValue == cycleMin and currentValue == cycleMax:
                                cylingPassed = 1
                                pass
                            else:
                                cylingPassed = 0
                                # print(str(l) + " - " + str(currentValue))
                                break

                lastValue = i

    return cylingPassed, isIncrementing


def brr1_listener_master_msg(conn):
    """
    Stop the brr thread.
    """
    if conn.recv() == "stop":
        # print('msg received brr1 stop')
        brr1_exit_event.clear()


def brr2_listener_master_msg(conn):
    """
    Stop the brr thread.
    """
    if conn.recv() == "stop":
        # print('msg received brr2 stop')
        brr2_exit_event.clear()
        time.sleep(2)


def xcp_listener_master_msg(conn):
    """
    Stop the xcp thread.
    """
    if conn.recv() == "stop":
        # print('msg received xcp stop')
        xcp_exit_event.clear()
        time.sleep(2)


def splitUint32(val):
    """
    Split the uint32 into two uint16 values.
    """
    h = "%08x" % val
    a = int(h[0:4], 16)
    b = int(h[4:8], 16)
    return b, a


def brr_listener_indexcheck(sensorPosition, q, eq, conn, eth, variant):
    """
    Process that reads brr data but only the summaries.
    """
    h = logging.handlers.QueueHandler(q)
    brr = logging.getLogger()
    brr.addHandler(h)
    brr.setLevel(logging.INFO)

    msg_cntr = 0
    packets = {1: 0, 2: 0, 3: 0, 4: 0, 6: 0, 7: 0, 8: 0, 9: 0, 14: 0}
    brr_signals_data["txRates"] = {1: [], 2: [], 3: [], 4: [], 6: [], 7: [], 8: [], 9: [], 14: []}
    brr_signals_data["dataLengths"] = {1: 0, 2: 0, 3: 0, 4: 0, 6: 0, 7: 0, 8: 0, 9: 0, 14: 0}
    streamIndexes = {1: [], 2: [], 3: [], 4: [], 6: [], 7: [], 8: [], 9: [], 14: []}
    streamIdxSkips = {1: 0, 2: 0, 3: 0, 4: 0, 6: 0, 7: 0, 8: 0, 9: 0, 14: 0}
    streamIdxRepeats = {1: 0, 2: 0, 3: 0, 4: 0, 6: 0, 7: 0, 8: 0, 9: 0, 14: 0}
    prevStreamIdx = {
        1: None,
        2: None,
        3: None,
        4: None,
        6: None,
        7: None,
        8: None,
        9: None,
        14: None,
    }

    lookID = {1: [], 2: [], 3: [], 4: [], 8: []}
    # rbin_res = []
    dbin_res = []
    timeStamps = {"timeStampSec": [], "timeStampNs": []}
    targetCnts = []
    numFpDet = []
    numSpDet = []
    RadarPostDaqCnt = []
    ModeTrigCnt = []
    RadarPostRDDCnt = []
    firstTP = 0
    lastTP = 0
    # create thred within process for checking master msgs
    lua = WIRESHARK_PLUGIN_DIR + readLuaScript()

    param = ["-X", "lua_script:{0}".format(lua)]
    cap = pyshark.LiveCapture(
        interface=eth, bpf_filter="udp", only_summaries=True, custom_parameters=param
    )
    master_msg = Thread(target=brr2_listener_master_msg, args=(conn,))
    master_msg.start()
    brr2_exit_event.set()

    brr.info("Starting BRR2 listener...")
    for packet in cap.sniff_continuously():
        if brr2_exit_event.is_set() is False:
            brr.info("Exiting brr_listener_indexcheck")
            break
        ts = float(packet.summary_line.split()[1])

        if firstTP == 0:
            firstTP = ts
        lastTP = ts
        if "Aptiv UDP Stream 1 " in packet.summary_line:
            brr_signals_data["dataLengths"][1] += int(packet.summary_line.split()[8])
            if "ScanIndex:" in packet.summary_line:
                packets[1] += 1

                brr_signals_data["txRates"][1].append(ts * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(",")[0].strip()
                index = int(index)

                streamIndexes[1].append(index)
                if prevStreamIdx[1] is not None:
                    if index != (prevStreamIdx[1] + 1):
                        if not ((index == 0) and (prevStreamIdx[1] == 65535)):  # signal overflow
                            streamIdxSkips[1] += 1
                    if index == prevStreamIdx[1]:
                        streamIdxRepeats[1] += 1
                prevStreamIdx[1] = index

            if "TargetCnt:" in packet.summary_line:
                targetCnt = int(
                    packet.summary_line.split("TargetCnt:")[1].strip().split(",")[0].strip()
                )
                targetCnts.append(targetCnt)
            if "num_fp_detections:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("num_fp_detections:")[1]
                    .strip()
                    .split(",")[0]
                    .strip()
                )
                numFpDet.append(det)
            if "num_sp_detections:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("num_sp_detections:")[1]
                    .strip()
                    .split(",")[0]
                    .strip()
                )
                numSpDet.append(det)
        elif "Aptiv UDP Stream 2 " in packet.summary_line:
            brr_signals_data["dataLengths"][2] += int(packet.summary_line.split()[8])
            if "ScanIndex:" in packet.summary_line:
                packets[2] += 1

                brr_signals_data["txRates"][2].append(ts * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(",")[0].strip()
                index = int(index)
                streamIndexes[2].append(index)

                if prevStreamIdx[2] is not None:
                    if index != (prevStreamIdx[2] + 1):
                        if not ((index == 0) and (prevStreamIdx[2] == 65535)):
                            streamIdxSkips[2] += 1
                    if index == prevStreamIdx[2]:
                        streamIdxRepeats[2] += 1
                prevStreamIdx[2] = index
            if "TSsec:" in packet.summary_line:
                tsSec = packet.summary_line.split("TSsec:")[1].strip().split(",")[0].strip()
                timeStamps["timeStampSec"].append(int(tsSec))
            if "TSnsec:" in packet.summary_line:
                stNCec = packet.summary_line.split("TSnsec:")[1].strip().split(",")[0].strip()
                timeStamps["timeStampNs"].append(int(stNCec))
            if "LookId:" in packet.summary_line:
                look = int(packet.summary_line.split("LookId:")[1].strip().split(",")[0].strip())
                lookID[2].append(look)
        elif "Aptiv UDP Stream 3 " in packet.summary_line:
            brr_signals_data["dataLengths"][3] += int(packet.summary_line.split()[8])
            if "ScanIndex:" in packet.summary_line:
                packets[3] += 1

                brr_signals_data["txRates"][3].append(ts * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(",")[0].strip()
                index = int(index)
                streamIndexes[3].append(index)
                if prevStreamIdx[3] is not None:
                    if index != (prevStreamIdx[3] + 1):
                        if not ((index == 0) and (prevStreamIdx[3] == 65535)):
                            streamIdxSkips[3] += 1
                    if index == prevStreamIdx[3]:
                        streamIdxRepeats[3] += 1
                prevStreamIdx[3] = index
            if "ModeTrigCnt:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("ModeTrigCnt:")[1].strip().split(",")[0].strip()
                )
                ModeTrigCnt.append(det)
            if "RadarPostDaqCnt:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("RadarPostDaqCnt:")[1].strip().split(",")[0].strip()
                )
                RadarPostDaqCnt.append(det)
            if "RadarPostRDDCnt:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("RadarPostRDDCnt:")[1].strip().split(",")[0].strip()
                )
                RadarPostRDDCnt.append(det)
        elif "Aptiv UDP Stream 4 " in packet.summary_line:
            brr_signals_data["dataLengths"][4] += int(packet.summary_line.split()[8])
            if "ScanIndex:" in packet.summary_line:
                packets[4] += 1

                brr_signals_data["txRates"][4].append(ts * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(",")[0].strip()
                index = int(index)
                streamIndexes[4].append(index)
                if prevStreamIdx[4] is not None:
                    if index != (prevStreamIdx[4] + 1):
                        if not ((index == 0) and (prevStreamIdx[4] == 65535)):
                            streamIdxSkips[4] += 1
                    if index == prevStreamIdx[4]:
                        streamIdxRepeats[4] += 1
                prevStreamIdx[4] = index
            if "num_fp_detections:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("num_fp_detections:")[1]
                    .strip()
                    .split(",")[0]
                    .strip()
                )
                numFpDet.append(det)
            if "num_sp_detections:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("num_sp_detections:")[1]
                    .strip()
                    .split(",")[0]
                    .strip()
                )
                numSpDet.append(det)

            if "LookId:" in packet.summary_line:
                look = int(packet.summary_line.split("LookId:")[1].strip().split(",")[0].strip())
                lookID[4].append(look)
            if "dbinRes:" in packet.summary_line:
                v = int(packet.summary_line.split("dbinRes:")[1].strip().split(",")[0].strip())
                dbin_res.append(v)
        elif "Aptiv UDP Stream 6 " in packet.summary_line:
            brr_signals_data["dataLengths"][6] += int(packet.summary_line.split()[8])
        elif "Aptiv UDP Stream 7 " in packet.summary_line:
            brr_signals_data["dataLengths"][7] += int(packet.summary_line.split()[8])
            if "ScanIndex:" in packet.summary_line:
                packets[7] += 1
                brr_signals_data["txRates"][7].append(ts * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(",")[0].strip()
                index = int(index)
                streamIndexes[7].append(index)
                if prevStreamIdx[7] is not None:
                    if index != (prevStreamIdx[7] + 1):
                        if not ((index == 0) and (prevStreamIdx[7] == 65535)):
                            streamIdxSkips[7] += 1
                    if index == prevStreamIdx[7]:
                        streamIdxRepeats[7] += 1
                prevStreamIdx[7] = index
        elif "Aptiv UDP Stream 8 " in packet.summary_line:
            brr_signals_data["dataLengths"][8] += int(packet.summary_line.split()[8])
            if "ScanIndex:" in packet.summary_line:
                packets[8] += 1
                brr_signals_data["txRates"][8].append(ts * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(",")[0].strip()
                index = int(index)
                streamIndexes[8].append(index)
                if prevStreamIdx[8] is not None:
                    if index != (prevStreamIdx[8] + 1):
                        if not ((index == 0) and (prevStreamIdx[4] == 65535)):
                            streamIdxSkips[8] += 1
                    if index == prevStreamIdx[8]:
                        streamIdxRepeats[8] += 1
                prevStreamIdx[8] = index
        elif "Aptiv UDP Stream 9 " in packet.summary_line:
            brr_signals_data["dataLengths"][9] += int(packet.summary_line.split()[8])
        elif "Aptiv UDP Stream 14 " in packet.summary_line:
            brr_signals_data["dataLengths"][14] += int(packet.summary_line.split()[8])
        msg_cntr += 1
        # brr.info('brr msg received: %d', msg_cntr)

    cap.close()
    brr.info("streamIdxSkips:")
    brr.info(streamIdxSkips)
    brr.info("dataLengths")
    brr.info(brr_signals_data["dataLengths"])
    brr.info("Total UDP2 frames received: %d", msg_cntr)
    brr_signals_data["total_udp_frames"] = msg_cntr
    brr_signals_data["packets"] = packets
    brr_signals_data["lookID"] = lookID
    brr_signals_data["streamIndexes"] = streamIndexes
    brr_signals_data["streamIdxRepeats"] = streamIdxRepeats
    brr_signals_data["streamIdxSkips"] = streamIdxSkips
    brr_signals_data["timeStamps"] = timeStamps
    brr_signals_data["targetCnts"] = targetCnts
    brr_signals_data["numFpDetections"] = numFpDet
    brr_signals_data["numSpDetections"] = numSpDet
    brr_signals_data["dbinRes"] = dbin_res
    brr_signals_data["firstTP"] = firstTP
    brr_signals_data["lastTP"] = lastTP
    brr_signals_data["RadarPostDaqCnt"] = RadarPostDaqCnt
    brr_signals_data["RadarPostRDDCnt"] = RadarPostRDDCnt
    brr_signals_data["ModeTrigCnt"] = ModeTrigCnt

    brr.info("Opening brr_shelf in BRR2")

    with shelve.open(BRR_SHELF) as brr_shelf:
        for key in brr_signals_data:
            brr_shelf[key] = brr_signals_data[key]

    brr_shelf.close()
    master_msg.join()


def XCP_listener(elfFile, maxXput, q, eq, conn):
    """
    Process that reads xcp data.
    """
    h = logging.handlers.QueueHandler(q)
    xcp = logging.getLogger()
    xcp.addHandler(h)
    xcp.setLevel(logging.INFO)
    totalRunTimes = []
    master_msg = Thread(target=xcp_listener_master_msg, args=(conn,))
    master_msg.start()
    xcp_exit_event.set()
    xcp.info("In XCP_Listener")

    if path.exists(elfFile):
        xcp.info("elf file exists")
        time.sleep(10)
        xcpHandler = XcpHandler(elfFile)
        xcp.info("XcpHandler ok")
        if xcpHandler.xcpInit():
            xcp.info("Past xcpInit")
            addr_xcp_rdd_vary_xput_flag, size = xcpHandler.getAddress("xcp_rdd_vary_xput_flag")
            addr_max_static_target_case_enable, size = xcpHandler.getAddress(
                "xcp_max_static_target_case_enable"
            )

            if maxXput:
                xcp.info("Set max throuput conditions")
                # Set Max Throughput Conditions
                xcpHandler.readCal(addr_max_static_target_case_enable, ONE_BYTE)
                xcpHandler.readCal(addr_xcp_rdd_vary_xput_flag, ONE_BYTE)

                xcpHandler.writeCal(addr_max_static_target_case_enable, ONE_BYTE, CODED_TRUE)
                xcpHandler.writeCal(addr_xcp_rdd_vary_xput_flag, ONE_BYTE, CODED_TRUE)

                xcpHandler.readCal(addr_max_static_target_case_enable, ONE_BYTE)
                xcpHandler.readCal(addr_xcp_rdd_vary_xput_flag, ONE_BYTE)
            else:
                xcp.info("Set varying throuput conditions")
                # Set Varying Throughput Conditions
                xcpHandler.readCal(addr_max_static_target_case_enable, ONE_BYTE)
                xcpHandler.readCal(addr_xcp_rdd_vary_xput_flag, ONE_BYTE)

                xcpHandler.writeCal(addr_max_static_target_case_enable, ONE_BYTE, CODED_FALSE)
                xcpHandler.writeCal(addr_xcp_rdd_vary_xput_flag, ONE_BYTE, CODED_TRUE)

                xcpHandler.readCal(addr_max_static_target_case_enable, ONE_BYTE)
                xcpHandler.readCal(addr_xcp_rdd_vary_xput_flag, ONE_BYTE)

            # print("Done setting up Max Throughtput test")

            addr_ProfileInfo, size_ProfileInfo = xcpHandler.getAddress("DFFT_CONFIG_ProfileInfo")
            xcpHandler.setupDAQ(addr_ProfileInfo + 16)

        xcpHandler.disconnect()

        # while(True):
        # if (xcp_exit_event.is_set() == False):
        # xcp.info('Exiting XCP_listener')
        # break
        # data = xcpHandler.readDaq(4, 1000)
        # if (data):
        # totalRunTimes.append(data)
        # time.sleep(.1)

    else:
        xcp.info("elf file does not exist")
    # print ("elf file does not exist")

    xcp_shelf = shelve.open(XCP_SHELF)

    xcp_shelf["totalRunTime"] = totalRunTimes
    xcp_shelf.close()
    master_msg.join()


def brr_listener(sensorPosition, q, eq, conn, eth):
    """
    Process that reads all brr data.
    """
    h = logging.handlers.QueueHandler(q)
    brr = logging.getLogger()
    brr.addHandler(h)
    brr.setLevel(logging.INFO)

    msg_cntr = 0
    firstTime = True
    streamList = {}
    streamDataLengths = {}
    # packets = {"1":0, "2":0, "3":0, "4":0, "5":0, "6":0, "8":0, "14":0}
    streamIndexes2 = {1: [], 2: [], 3: [], 4: [], 5: [], 6: [], 8: [], 14: []}
    brr_signals_data["versioning"] = {}
    brr_signals_data["temps"] = {
        "mmic_temp_tx0": [],
        "mmic_temp_tx1": [],
        "mmic_temp_tx2": [],
        "mmic_temp_tx3": [],
        "mmic_temp_rx0": [],
        "mmic_temp_rx1": [],
        "mmic_temp_rx2": [],
        "mmic_temp_rx3": [],
        "c66_temp": [],
        "radar_temp": [],
        "tmu_temp": [],
        "dsp_temp": [],
        "hwa_temp": [],
        "hsm_temp": [],
    }
    brr_signals_data["xput"] = {"RFFT_xput_inst": [], "RDD_xput_inst": [], "AF_xput_inst": []}
    brr_signals_data["sensorTimestampSec"] = []
    brr_signals_data["timeSyncValidityFlag"] = []
    brr_signals_data["timeSyncSourceMaster"] = []
    brr_signals_data["sensorTimestampNs"] = []
    brr_signals_data["target_count"] = []
    brr_signals_data["af_detections"] = []
    brr_signals_data["voltage"] = {
        "supplyVolts11V": [],
        "supplyVolts082V": [],
        "supplyVolts33V": [],
        "supplyVolts18V": [],
        "supplyVolts5V": [],
        "supplyVolts23V": [],
    }
    brr_signals_data["radarPostDaqCount"] = []
    brr_signals_data["modeTrigCount"] = []
    brr_signals_data["radarPostRddCount"] = []
    brr_signals_data["heartbeatCount"] = []
    brr_signals_data["ipcDspToSramCnt"] = []
    brr_signals_data["rangeOverrunCnt"] = []
    brr_signals_data["dopplerOverrunCnt"] = []
    brr_signals_data["chirpOutOfSyncCnt"] = []
    brr_signals_data["interruptErrCounts"] = []
    brr_signals_data["ipcr50ToDspErrCnt"] = []
    brr_signals_data["evtDispOverrunCnt"] = []
    brr_signals_data["dspSptLookIdErrCnt"] = []
    brr_signals_data["rbinErrorCnt"] = []
    brr_signals_data["minChirpScaling"] = []
    brr_signals_data["loadStatus"] = {
        "smc_load_status": [],
        "usc_load_status": [],
        "radar_cals_load": [],
    }
    # brr_signals_data['numFpDetections'] = []
    # brr_signals_data['lookID'] = []
    # brr_signals_data['numSpDetections'] = []
    brr_signals_data["dataCoherency"] = {
        "0": {"rddRangeCoverage": 0.0, "rddDoppCoverage": 0.0},
        "1": {"rddRangeCoverage": 0.0, "rddDoppCoverage": 0.0},
        "2": {"rddRangeCoverage": 0.0, "rddDoppCoverage": 0.0},
        "3": {"rddRangeCoverage": 0.0, "rddDoppCoverage": 0.0},
    }

    brr_signals_data["CCAppConfig"] = {
        "CCAppConfigCC": [],
        "CCAppConfigLVDS": [],
        "CCAppConfigMIPI": [],
        "CCAppConfigTE": [],
        "CCAppConfigSC": [],
        "CCAppConfigCAFC": [],
        "CCAppConfigADC12": [],
        "CCAppConfigADC34": [],
        "CCAppConfigGBIAS": [],
        "CCAppConfigLOI": [],
        "CCAppConfigMCLK": [],
        "CCAppConfigRX1": [],
        "CCAppConfigRX2": [],
        "CCAppConfigRX3": [],
        "CCAppConfigRX4": [],
        "CCAppConfigTX1": [],
        "CCAppConfigTX2": [],
        "CCAppConfigTX3": [],
        "CCAppConfigGLDO": [],
        "CCAppConfigATB": [],
        "CCAppConfigOTP": [],
        "CCAppConfigISM": [],
        "CCAppConfigCHIRP5GMODE": [],
        "CCAppConfigPR": [],
        "CCAppConfigLOIN": [],
        "CCAppConfigLOOUT": [],
    }
    brr_signals_data["barracuda"] = {
        "major": [],
        "minor": [],
        "patch": [],
        "variantType": [],
        "wtDay": [],
        "wtMonth": [],
        "wtYear": [],
        "wafer": [],
        "lotidMsb": [],
        "lotidLsb": [],
        "maskversion": [],
        "wtSite": [],
        "wtTpVersionMajor": [],
        "wtTpVersionMinor": [],
        "ftTpVersionMajor": [],
        "ftTpVersionMinor": [],
    }

    brr_signals_data["bc_errors"] = {
        "consecutiveFrameErrorCount": [],
        "ismWarningCounter": [],
        "persistentErrorCount": [],
        "consecutiveInitRetryCount": [],
        "txErrorFlagMaskStatus": [],
        "bcUdpFailures": [],
        "bcCcConfigpadErrorn": [],
        "bcCcConfigpadErrorReset": [],
        "bcCcConfigpadHReset": [],
        "bcIsmGetismerrstatusPreAcq": [],
        "bcIsmGetismwarningstatusPreAcq": [],
        "bcIsmGetmastererrstatusPreAcq": [],
        "bcIsmResetismerr": [],
        "bcIsmMaskrtmsetclrtxerr": [],
    }

    brr_signals_data["cdc"] = {
        "RngDopIdx": [],
        "max_doppler_log_bins_per_range_bin": [],
        "max_log_bins": [],
        "range_bins_limited_count": [],
        "detection_range_rate_bins_limited_count": [],
        "look_num_dbins_per_rbin_max": [],
        "look_total_bins_max": [],
        "look_total_bins": [],
        "look_num_frames_max": [],
        "look_num_frames": [],
        "num_dbins_per_rbin_max": [],
        "ENET_TxBuffers_Pending_Count_MAX": [],
        "ENET_CDC_Tx_Buf_UNAVBL_Count": [],
        "ENET_ARP_Tx_Buf_UNAVBL_Count": [],
        "ENET_Partial_Buffer_Mismatch_Count": [],
    }

    brr_signals_data["exception_cntrs"] = {
        "Float_Exception_Count_c0": [],
        "Float_Exception_Count_c1": [],
        "Float_Exception_Count_c2": [],
        "c1_c2_ipc_err_cntr": [],
        "c0_c2_ipc_err_cntr": [],
    }
    # brr_signals_data_rdop = {"rngIdx": [], "dopIdx": []}
    brr_signals_data["mmic"] = {"mmic_err_counter": [], "mmic_err_code": []}

    brr_signals_data["cdc_stats"] = {}
    # FRED brr_signals_data['txRates']={}

    # create thread within process for checking master msgs
    lua = WIRESHARK_PLUGIN_DIR + readLuaScript()

    param = ["-X", "lua_script:{0}".format(lua)]
    cap = pyshark.LiveCapture(interface=eth, custom_parameters=param)
    master_msg = Thread(target=brr1_listener_master_msg, args=(conn,))
    master_msg.start()
    brr1_exit_event.set()

    brr.info("Starting BRR listener...")
    s1_streamChunkIdx = -1
    for packet in cap.sniff_continuously():
        if brr1_exit_event.is_set() is False:
            brr.info("Exiting brr_listener")
            break
        if packet.highest_layer == "APTIVUDP":

            streamNumber = 0
            streamChunkIdx = 0
            streamVersion = 0
            # sourceInfo = 0
            streamDataLength = 0

            if hasattr(packet["APTIVUDP"], "streamNumber"):
                streamNumber = int(packet["APTIVUDP"].streamNumber)
            if hasattr(packet["APTIVUDP"], "streamChunkIdx"):
                streamChunkIdx = int(packet["APTIVUDP"].streamChunkIdx)
            if hasattr(packet["APTIVUDP"], "streamDataLength"):
                streamDataLength = packet.streamDataLength
            if hasattr(packet["APTIVUDP"], "streamVersion"):
                streamVersion = int(packet["APTIVUDP"].streamVersion)
            # if hasattr(packet["APTIVUDP"], "sourceInfo"):
            #    sourceInfo = packet["APTIVUDP"].sourceInfo

            if int(streamNumber) not in streamList:
                streamList[streamNumber] = streamVersion
            # FRED brr_signals_data['txRates'][streamNumber] = []
            if int(streamNumber) not in streamDataLengths:
                streamDataLengths[streamNumber] = []

            if streamVersion > 0:
                streamDataLengths[streamNumber].append(streamDataLength)

            if streamNumber == 1 and s1_streamChunkIdx == -1:
                # find the chunk where these values are found
                if hasattr(packet["APTIVUDP"], "s1_target_count"):
                    s1_streamChunkIdx = streamChunkIdx

            # if (streamChunkIdx == 0):
            if streamNumber == 1 and streamChunkIdx == s1_streamChunkIdx:
                if hasattr(packet["APTIVUDP"], "s1_target_count"):
                    brr_signals_data["target_count"].append(
                        int(packet["APTIVUDP"].s1_target_count)
                    )
                if hasattr(packet["APTIVUDP"], "s1_scanindex"):
                    streamIndexes2[streamNumber].append(int(packet["APTIVUDP"].s1_scanindex))
                if hasattr(packet["APTIVUDP"], "s1_af_detections_num_af_det"):
                    brr_signals_data["af_detections"].append(
                        int(packet["APTIVUDP"].s1_af_detections_num_af_det)
                    )

            # FRED brr_signals_data['txRates'][streamNumber].append(float(packet.sniff_timestamp)*1000)
            elif streamNumber == 2:  # and streamChunkIdx == "0"):
                # FRED brr_signals_data['txRates'][streamNumber].append(float(packet.sniff_timestamp)*1000)
                if hasattr(packet["APTIVUDP"], "s2_scanindex"):
                    streamIndexes2[streamNumber].append(int(packet["APTIVUDP"].s2_scanindex))

                if hasattr(packet["APTIVUDP"], "s2_RFFT_xput_inst"):
                    brr_signals_data["xput"]["RFFT_xput_inst"].append(
                        float(packet["APTIVUDP"].s2_RFFT_xput_inst)
                    )
                if hasattr(packet["APTIVUDP"], "s2_RDD_xput_inst"):
                    brr_signals_data["xput"]["RDD_xput_inst"].append(
                        float(packet["APTIVUDP"].s2_RDD_xput_inst)
                    )
                if hasattr(packet["APTIVUDP"], "s2_af_xput_inst"):
                    brr_signals_data["xput"]["AF_xput_inst"].append(
                        float(packet["APTIVUDP"].s2_af_xput_inst)
                    )

                if hasattr(packet["APTIVUDP"], "s2_RFFT_xput_peak"):
                    brr_signals_data["xput"]["RFFT_xput_peak"] = float(
                        packet["APTIVUDP"].s2_RFFT_xput_peak
                    )
                if hasattr(packet["APTIVUDP"], "s2_RDD_xput_peak"):
                    brr_signals_data["xput"]["RDD_xput_peak"] = float(
                        packet["APTIVUDP"].s2_RDD_xput_peak
                    )
                if hasattr(packet["APTIVUDP"], "s2_af_xput_peak"):
                    brr_signals_data["xput"]["AF_xput_peak"] = float(
                        packet["APTIVUDP"].s2_af_xput_peak
                    )

                if hasattr(packet["APTIVUDP"], "s2_sensor_timestamp_sec"):
                    brr_signals_data["sensorTimestampSec"].append(
                        int(packet["APTIVUDP"].s2_sensor_timestamp_sec)
                    )
                if hasattr(packet["APTIVUDP"], "s2_sensor_timestamp_ns"):
                    brr_signals_data["sensorTimestampNs"].append(
                        int(packet["APTIVUDP"].s2_sensor_timestamp_ns)
                    )

                if hasattr(packet["APTIVUDP"], "s2_time_sync_validity_flag"):
                    brr_signals_data["timeSyncValidityFlag"].append(
                        int(packet["APTIVUDP"].s2_time_sync_validity_flag)
                    )
                if hasattr(packet["APTIVUDP"], "s2_time_sync_source_master"):
                    brr_signals_data["timeSyncSourceMaster"].append(
                        int(packet["APTIVUDP"].s2_time_sync_source_master)
                    )

                if hasattr(packet["APTIVUDP"], "s2_rdd_range_coverage"):
                    brr_signals_data["dataCoherency"][packet["APTIVUDP"].s2_look_type][
                        "rddRangeCoverage"
                    ] = (int(packet["APTIVUDP"].s2_rdd_range_coverage) / 128.0)
                if hasattr(packet["APTIVUDP"], "s2_rdd_dopp_coverage"):
                    brr_signals_data["dataCoherency"][packet["APTIVUDP"].s2_look_type][
                        "rddDoppCoverage"
                    ] = (int(packet["APTIVUDP"].s2_rdd_dopp_coverage) / 128.0)

            elif streamNumber == 3:  # and streamChunkIdx == "0"):
                # brr.info('Stream 3')
                # FRED brr_signals_data['txRates'][streamNumber].append(float(packet.sniff_timestamp)*1000)
                if hasattr(packet["APTIVUDP"], "s3_scanindex"):
                    streamIndexes2[streamNumber].append(int(packet["APTIVUDP"].s3_scanindex))
                # packets[streamNumber] += 1
                # if ( hasattr(packet["APTIVUDP"], 'scanIndex3')): streamIndexes[streamNumber].append(packet["APTIVUDP"].scanindex3)
                # if ( hasattr(packet["APTIVUDP"], 's3_pbl_version')):
                if firstTime:
                    brr_signals_data["versioning"]["pbl_version_maj"] = packet[
                        "APTIVUDP"
                    ].s3_pbl_version_maj
                    brr_signals_data["versioning"]["pbl_version_min"] = packet[
                        "APTIVUDP"
                    ].s3_pbl_version_min
                    brr_signals_data["versioning"]["pbl_version_patch"] = packet[
                        "APTIVUDP"
                    ].s3_pbl_version_patch

                    brr_signals_data["versioning"]["software_version_gen"] = packet[
                        "APTIVUDP"
                    ].s3_software_version_gen
                    brr_signals_data["versioning"]["software_version_variant"] = packet[
                        "APTIVUDP"
                    ].s3_software_version_variant
                    brr_signals_data["versioning"]["software_version_maj"] = packet[
                        "APTIVUDP"
                    ].s3_software_version_maj
                    brr_signals_data["versioning"]["software_version_min"] = packet[
                        "APTIVUDP"
                    ].s3_software_version_min
                    brr_signals_data["versioning"]["software_version_patch"] = packet[
                        "APTIVUDP"
                    ].s3_software_version_patch

                    brr_signals_data["versioning"]["smcVersion1"] = packet[
                        "APTIVUDP"
                    ].s3_smc_version_major
                    brr_signals_data["versioning"]["smcVersion2"] = packet[
                        "APTIVUDP"
                    ].s3_smc_version_platform
                    brr_signals_data["versioning"]["smcVersion3"] = packet[
                        "APTIVUDP"
                    ].s3_smc_version_minor
                    brr_signals_data["versioning"]["smcVersion4"] = packet[
                        "APTIVUDP"
                    ].s3_smc_version_patch

                    brr_signals_data["versioning"]["uscVersion1"] = packet[
                        "APTIVUDP"
                    ].s3_usc_version_major
                    brr_signals_data["versioning"]["uscVersion2"] = packet[
                        "APTIVUDP"
                    ].s3_usc_version_platform
                    brr_signals_data["versioning"]["uscVersion3"] = packet[
                        "APTIVUDP"
                    ].s3_usc_version_minor
                    brr_signals_data["versioning"]["uscVersion4"] = packet[
                        "APTIVUDP"
                    ].s3_usc_version_patch
                    brr_signals_data["versioning"]["platformVariant"] = packet[
                        "APTIVUDP"
                    ].s3_platform_variant

                    brr_signals_data["versioning"]["stream1Version"] = packet[
                        "APTIVUDP"
                    ].s3_stream1_version
                    brr_signals_data["versioning"]["stream2Version"] = packet[
                        "APTIVUDP"
                    ].s3_stream2_version
                    brr_signals_data["versioning"]["stream3Version"] = packet[
                        "APTIVUDP"
                    ].s3_stream3_version
                    brr_signals_data["versioning"]["stream4Version"] = packet[
                        "APTIVUDP"
                    ].s3_stream4_version
                    brr_signals_data["versioning"]["stream5Version"] = packet[
                        "APTIVUDP"
                    ].s3_stream5_version
                    brr_signals_data["versioning"]["stream6Version"] = packet[
                        "APTIVUDP"
                    ].s3_stream6_version

                brr_signals_data["temps"]["mmic_temp_tx0"].append(
                    int(packet["APTIVUDP"].s3_mmic_temp_tx0)
                )
                brr_signals_data["temps"]["mmic_temp_tx1"].append(
                    int(packet["APTIVUDP"].s3_mmic_temp_tx1)
                )
                brr_signals_data["temps"]["mmic_temp_tx2"].append(
                    int(packet["APTIVUDP"].s3_mmic_temp_tx2)
                )
                brr_signals_data["temps"]["mmic_temp_tx3"].append(
                    int(packet["APTIVUDP"].s3_mmic_temp_tx3)
                )

                brr_signals_data["temps"]["mmic_temp_rx0"].append(
                    int(packet["APTIVUDP"].s3_mmic_temp_rx0)
                )
                brr_signals_data["temps"]["mmic_temp_rx1"].append(
                    int(packet["APTIVUDP"].s3_mmic_temp_rx1)
                )
                brr_signals_data["temps"]["mmic_temp_rx2"].append(
                    int(packet["APTIVUDP"].s3_mmic_temp_rx2)
                )
                brr_signals_data["temps"]["mmic_temp_rx3"].append(
                    int(packet["APTIVUDP"].s3_mmic_temp_rx3)
                )

                if hasattr(packet["APTIVUDP"], "s3_c66_temp"):
                    brr_signals_data["temps"]["c66_temp"].append(
                        int(packet["APTIVUDP"].s3_c66_temp)
                    )
                    brr_signals_data["temps"]["radar_temp"].append(
                        int(packet["APTIVUDP"].s3_radar_temp)
                    )
                    brr_signals_data["temps"]["tmu_temp"].append(
                        int(packet["APTIVUDP"].s3_tmu_temp)
                    )
                if hasattr(packet["APTIVUDP"], "s3_dsp_temp"):
                    brr_signals_data["temps"]["dsp_temp"].append(
                        int(packet["APTIVUDP"].s3_dsp_temp)
                    )
                    brr_signals_data["temps"]["hwa_temp"].append(
                        int(packet["APTIVUDP"].s3_hwa_temp)
                    )
                    brr_signals_data["temps"]["hsm_temp"].append(
                        int(packet["APTIVUDP"].s3_hsm_temp)
                    )

                # brr_signals_data['afOverrunCnt'].append(packet["APTIVUDP"].s3_AF_overrun_cnt)
                # brr_signals_data['rddOverrunCnt'].append(packet["APTIVUDP"].s3_RDD_overrun_cnt)
                brr_signals_data["rangeOverrunCnt"].append(
                    int(packet["APTIVUDP"].s3_range_overrun)
                )
                brr_signals_data["dopplerOverrunCnt"].append(
                    int(packet["APTIVUDP"].s3_doppler_overrun)
                )

                brr_signals_data["voltage"]["supplyVolts11V"].append(
                    float(packet["APTIVUDP"].s3_supply_volts_1_1_V)
                )
                brr_signals_data["voltage"]["supplyVolts082V"].append(
                    float(packet["APTIVUDP"].s3_supply_volts_0_82_V)
                )
                brr_signals_data["voltage"]["supplyVolts33V"].append(
                    float(packet["APTIVUDP"].s3_supply_volts_3_3_V)
                )
                brr_signals_data["voltage"]["supplyVolts18V"].append(
                    float(packet["APTIVUDP"].s3_supply_volts_1_8_V)
                )
                brr_signals_data["voltage"]["supplyVolts5V"].append(
                    float(packet["APTIVUDP"].s3_supply_volts_5_V)
                )
                brr_signals_data["voltage"]["supplyVolts23V"].append(
                    float(packet["APTIVUDP"].s3_supply_volts_2_3_V)
                )

                brr_signals_data["mmic"]["mmic_err_counter"].append(
                    int(packet["APTIVUDP"].s3_mmic_err_counter)
                )
                brr_signals_data["mmic"]["mmic_err_code"].append(
                    int(packet["APTIVUDP"].s3_mmic_err_code)
                )

                brr_signals_data["radarPostDaqCount"].append(
                    int(packet["APTIVUDP"].s3_radar_post_daq_count)
                )
                brr_signals_data["modeTrigCount"].append(
                    int(packet["APTIVUDP"].s3_mode_trig_count)
                )
                brr_signals_data["radarPostRddCount"].append(
                    int(packet["APTIVUDP"].s3_radar_post_rdd_count)
                )
                brr_signals_data["heartbeatCount"].append(
                    int(packet["APTIVUDP"].s3_heartbeat_count)
                )
                brr_signals_data["ipcDspToSramCnt"].append(
                    int(packet["APTIVUDP"].s3_ipc_dsp_to_sram_cnt)
                )
                brr_signals_data["chirpOutOfSyncCnt"].append(
                    int(packet["APTIVUDP"].s3_chirp_out_of_sync_cnt)
                )
                brr_signals_data["interruptErrCounts"].append(
                    int(packet["APTIVUDP"].s3_interrupt_err_counts)
                )
                brr_signals_data["ipcr50ToDspErrCnt"].append(
                    int(packet["APTIVUDP"].s3_ipc_r50_to_dsp_err_cnt)
                )
                brr_signals_data["evtDispOverrunCnt"].append(
                    int(packet["APTIVUDP"].s3_evt_disp_overrun_cnt)
                )
                brr_signals_data["dspSptLookIdErrCnt"].append(
                    int(packet["APTIVUDP"].s3_dsp_spt_look_id_err_cnt)
                )
                brr_signals_data["rbinErrorCnt"].append(int(packet["APTIVUDP"].s3_rbin_error_cnt))

                brr_signals_data["loadStatus"]["smc_load_status"].append(
                    int(packet["APTIVUDP"].s3_smc_load_status)
                )
                brr_signals_data["loadStatus"]["usc_load_status"].append(
                    int(packet["APTIVUDP"].s3_usc_load_status)
                )
                # brr_signals_data['loadStatus']['radar_cals_load'].append(int(packet["APTIVUDP"].s3_radar_cals_load))

            elif streamNumber == 4 and streamChunkIdx == 0:
                if hasattr(packet["APTIVUDP"], "s4_min_chirp_scaling"):
                    brr_signals_data["minChirpScaling"].append(
                        int(packet["APTIVUDP"].s4_min_chirp_scaling)
                    )
            elif streamNumber == 4 and streamChunkIdx == 62:
                if hasattr(packet["APTIVUDP"], "s4_scanindex"):
                    # brr.info('UDP Stream 4 Scan Index: %d', int(packet["APTIVUDP"].s4_scanindex))
                    streamIndexes2[streamNumber].append(int(packet["APTIVUDP"].s4_scanindex))

            # elif (streamNumber == "4" and hasattr(packet["APTIVUDP"], 's4_num_fp_detections') == True):
            # if ( hasattr(packet["APTIVUDP"], 's4_num_fp_detections')): brr_signals_data['numFpDetections'].append(int(packet["APTIVUDP"].s4_num_fp_detections))
            # if ( hasattr(packet["APTIVUDP"], 's4_num_sp_detections')): brr_signals_data['numSpDetections'].append(int(packet["APTIVUDP"].s4_num_sp_detections))

            # elif (streamNumber == "5" and streamChunkIdx == "0"):
            # packets[streamNumber] += 1
            # if ( hasattr(packet["APTIVUDP"], 'lookindex5')): streamIndexes[streamNumber].append(packet["APTIVUDP"].lookindex5)
            # elif (streamNumber == "6"):

            msg_cntr += 1
            # brr.info('brr1 msg received: %d', msg_cntr)

    cap.close()
    brr.info("Total UDP frames received: %d", msg_cntr)
    brr.info("UDP Stream List: %s", str(sorted(streamList)))
    # brr.info(packets)
    # brr.info(skips)
    # brr.info(repeats)
    # brr.info(streamIndexes)
    # brr_signals_data['packets'] = packets
    # brr_signals_data['skips'] = skips
    # brr_signals_data['repeats'] = repeats
    brr_signals_data["streamIndexes2"] = streamIndexes2
    brr_signals_data["StreamList"] = streamList  # str(sorted(streamList))
    brr_signals_data["StreamDataLength"] = streamDataLengths
    brr_signals_data["UDPMsgCntr"] = msg_cntr
    brr.info("Opening brr_shelf in BRR1")
    time.sleep(3.0)
    with shelve.open(BRR_SHELF) as brr_shelf:
        for key in brr_signals_data:
            brr_shelf[key] = brr_signals_data[key]

    brr.info("Done in BRR Listener")
    master_msg.join()


def master_logger(q):
    """
    Process that updates a log file.
    """
    # init master logger
    master = logging.getLogger("tester")
    master.setLevel(logging.INFO)

    # checking if the directory TEST_DATA_PATH exist or not.
    if not os.path.isdir(TEST_DATA_PATH):
        # if the TEST_DATA_PATH directory is
        # not present then create it.
        os.makedirs(TEST_DATA_PATH)

    # set handler
    logfile = logging.FileHandler(TEST_DATA_PATH + "test.log", mode="w")
    logfile.setLevel(logging.INFO)
    formatter = logging.Formatter(
        "[%(asctime)s] - %(processName)-13s - %(levelname)-8s : %(message)s"
    )
    logfile.setFormatter(formatter)
    # add handler
    master.addHandler(logfile)

    while True:
        record = q.get()
        if record is None:
            break
        else:
            master.handle(record)


def eth_bw_meas(ethnic, eq):
    """
    Process that reads net_data to get the bandwidth.
    """
    net_bw_prev = 0
    net_bw_exit.set()
    # bytes_sent = 0
    bytes_recv = 0
    try:
        while net_bw_exit.is_set():
            time.sleep(1.0)
            prog.update(1)

            # get the stats again
            io_2 = psutil.net_io_counters()
            # new - old stats gets us the speed
            # _us, ds = io_2.bytes_sent - bytes_sent, io_2.bytes_recv - bytes_recv
            ds = io_2.bytes_recv - bytes_recv

            # update the bytes_sent and bytes_recv for next iteration
            # bytes_sent, bytes_recv = io_2.bytes_sent, io_2.bytes_recv
            bytes_recv = io_2.bytes_recv

            avg_eth_bandwidth2.append(ds / 1024.0 / 1024.0)
            net_data = psutil.net_io_counters(pernic=True)
            if platform.system() == "Windows":
                net_bw = net_data[ethnic].bytes_sent + net_data[ethnic].bytes_recv
            else:
                net_bw = net_data["eth0"].bytes_sent + net_data["eth0"].bytes_recv
            if net_bw_prev:
                bw = (net_bw - net_bw_prev) / 1024.0 / 1024.0 * 8  # variable in megabits
                avg_eth_bandwidth.append(bw)
                # logging.info('Eth Bandwidth: %f', bw)
            net_bw_prev = net_bw
        prog.update(1)
    except KeyError as err:
        root.error("Unknown network interface: %s\n%s", ethnic, err)
        eq.put("Ethernet_NIC_error")


def timed_test():
    """
    Count up to the test time then break out of this function.
    """
    root.info("Timed test: %d seconds", test_time)

    while True:
        current_time = time.time()
        elapsed_time = current_time - start_tmstamp

        if elapsed_time > test_time:
            break
        if errq.empty() is False:
            mperror = errq.get()
            raise tester_Error("Abort test", mperror)


##############################################################################
# Reports
##############################################################################
def report_Versions(trr, brr_res):
    """
    Report the sw versions and stream versions to the html report.
    """
    if "pbl_version_maj" in brr_res["versioning"]:

        udp_pbl_version = "{0}.{1}.{2}".format(
            brr_res["versioning"]["pbl_version_maj"],
            brr_res["versioning"]["pbl_version_min"],
            brr_res["versioning"]["pbl_version_patch"],
        )
        # udp_app_version = '{0}.{1}.{2}.{3}'.format(brr_res["versioning"]['swVersion1'], brr_res["versioning"]['swVersion2'], brr_res["versioning"]['swVersion3'], brr_res["versioning"]['swVersion4'])
        udp_app_version = "{0}.{1}.{2}.{3}.{4}".format(
            brr_res["versioning"]["software_version_gen"],
            brr_res["versioning"]["software_version_variant"],
            brr_res["versioning"]["software_version_maj"],
            brr_res["versioning"]["software_version_min"],
            brr_res["versioning"]["software_version_patch"],
        )
        udp_smc_version = "{0}.{1}.{2}.{3}".format(
            brr_res["versioning"]["smcVersion1"],
            brr_res["versioning"]["smcVersion2"],
            brr_res["versioning"]["smcVersion3"],
            brr_res["versioning"]["smcVersion4"],
        )
        udp_usc_version = "{0}.{1}.{2}.{3}".format(
            brr_res["versioning"]["uscVersion1"],
            brr_res["versioning"]["uscVersion2"],
            brr_res["versioning"]["uscVersion3"],
            brr_res["versioning"]["uscVersion4"],
        )
        udp_plat_var = "{0}".format(brr_res["versioning"]["platformVariant"])

        trr.add_part_number(
            "APP: {0}, PBL: {1}, SMC: {2}, USC: {3}, Platform: {4}".format(
                udp_app_version, udp_pbl_version, udp_smc_version, udp_usc_version, udp_plat_var
            )
        )

        # trr.is_greater_than_or_equal('Hardware Version (CAN)',  0 , can_hw_version)

        trr.add_info_row("-- SOFTWARE CONFIG SUMMARY --")

        trr.add_result_row("Pbl Version", udp_pbl_version)
        trr.add_result_row("App Version", udp_app_version)
        trr.add_result_row("Smc Version ", udp_smc_version)
        trr.add_result_row("Usc Version", udp_usc_version)
        trr.add_result_row("Platform Variant", udp_plat_var)

    if "StreamList" in brr_res:
        for stream in sorted(brr_res["StreamList"]):
            ver = brr_res["StreamList"][stream]
            trr.add_result_row("UDP Stream {0} Version".format(stream), ver)


def report_ScanIndex(trr, brr_res):
    """
    Report the scan index skips and repeats for each stream.
    """
    trr.add_info_row("-- SCAN INDEX DROPS --")

    trr.add_result_row("UDP Total Received Messages", brr_res["total_udp_frames"])
    for stream in sorted(brr_res["streamIndexes"]):
        if brr_res["streamIndexes"][stream] and brr_res["packets"][stream]:

            trr.is_less_than(
                "Stream {0} Scan Index Skips ({1}/{2})".format(
                    stream, (brr_res["streamIdxSkips"][stream]), brr_res["packets"][stream]
                ),
                0.1,
                round(
                    (((brr_res["streamIdxSkips"][stream]) / brr_res["packets"][stream])) * 100, 2
                ),
                "%",
            )
            if ADD_PLOTS:
                trr.add_image(
                    brr_res["streamIndexes"][stream],
                    TEST_DATA_PATH + "udpStream" + str(stream) + "ScanIndex.png",
                    "Stream " + str(stream) + " Scan Index",
                    "Samples",
                    "Scan Index",
                )

            diff = []
            prev = None
            for scanIndex in brr_res["streamIndexes"][stream]:
                if prev is not None:
                    diff.append(scanIndex - prev)
                prev = scanIndex

            if ADD_PLOTS:
                trr.add_image(
                    diff,
                    TEST_DATA_PATH + "udpStream" + str(stream) + "ScanIndexDiff.png",
                    "Stream " + str(stream) + " Scan Index Difference",
                    "Samples",
                    "Scan Index Diff",
                )

            # trr.is_equal("CAN - Scan Index Repeats",      0, can_res['scan_repeating_cntr'],"-")
            # if brr_res['streamIndexes'] and brr_res['packets'] :
            trr.is_less_than(
                "Stream {0} Scan Index Repeats ({1}/{2})".format(
                    stream, (brr_res["streamIdxRepeats"][stream]), brr_res["packets"][stream]
                ),
                0.1,
                round(
                    (((brr_res["streamIdxRepeats"][stream]) / brr_res["packets"][stream])) * 100, 2
                ),
                "%",
            )

        if brr_res["txRates"]:
            # for stream in sorted (brr_res['txRates']):

            timestamps = brr_res["txRates"][stream]
            # print(timestamps)
            diff = []
            prev = None
            for i in timestamps:
                if prev is not None:
                    diff.append(i - prev)
                prev = i

            if len(diff) > 0:
                _maxTxRate = TX_RATE + TX_RATE_TOLERANCE
                _minTxRate = TX_RATE - TX_RATE_TOLERANCE
                # trr.is_less_than(   'Stream %d Rate - Max (%s)' % (stream, countMoreThan(diff,_maxTxRate,1)),        _maxTxRate, round(max(diff),2),    "ms")
                trr.is_almost_equal(
                    "Stream %d Rate - Avg (%s)"
                    % (stream, countBetween(diff, _maxTxRate, _minTxRate, 1)),
                    TX_RATE,
                    TX_RATE_TOLERANCE,
                    round(average(diff), 2),
                    "ms",
                )
                # trr.is_greater_than('Stream %d Rate - Min (%s)' % (stream, countLessThan(diff, _minTxRate,1)),     _minTxRate, round(min(diff),2),    "ms")
                trr.add_image(
                    diff,
                    TEST_DATA_PATH + "\\StreamRate_" + str(stream) + ".png",
                    "Stream " + str(stream) + " Transmission Rate",
                    "Packets",
                    "Time (ms)",
                )


def report_LookID(trr, brr_res):
    """
    Report the look id and check if it is cycling as expected.
    """
    if len(brr_res["lookID"]) > 0:
        trr.add_info_row("-- LOOK ID --")

        lookIDs = brr_res["lookID"][4]

        cyclingPassed, isIncrementing = checkCycling(lookIDs, 0, 3)

        trr.is_equal("UDP Look ID Min", 0, min(lookIDs))
        trr.is_equal("UDP Look ID Max", 3, max(lookIDs))
        if isIncrementing:
            trr.is_equal("UDP Look ID Valid Cycling (Incrementing)", 1, cyclingPassed)
        else:
            trr.is_equal("UDP Look ID Valid Cycling (Decrementing)", 1, cyclingPassed)

        if ADD_PLOTS:
            trr.add_image(
                lookIDs, TEST_DATA_PATH + "UDPlookID.png", "Stream 2 Look ID", "Samples", "Look ID"
            )

        if cyclingPassed == 0:
            root.info(lookIDs)


def report_Xput(trr, brr_res, maxXput, branch=None, variant="", swVersion=""):
    """
    Report the xput as reported in the brr stream data.
    """
    xputDict = {}
    xcp_res = shelve.open(XCP_SHELF)

    trr.add_info_row("-- XPUT SUMMARY --")
    if branch == "dev":
        dataDogMetrics = dataDogStats(branch=branch)
        datadog = True
    else:
        datadog = False
        dataDogMetrics = None

    if brr_res["xput"]:
        trr.is_less_than(
            "UDP - RDD Xput Avg",
            23,
            round(mean(brr_res["xput"]["RDD_xput_inst"]), 2),
            "ms",
            "WI-316483",
        )
        trr.is_less_than(
            "UDP - RDD Xput Max",
            23,
            round((brr_res["xput"]["RDD_xput_peak"]), 2),
            "ms",
            "WI-316483",
        )
        xputDict["RDD Xput Avg"] = round(mean(brr_res["xput"]["RDD_xput_inst"]), 2)
        xputDict["RDD Xput Max"] = round((brr_res["xput"]["RDD_xput_peak"]), 2)
        if ADD_PLOTS:
            trr.add_image(
                brr_res["xput"]["RDD_xput_inst"],
                TEST_DATA_PATH + "udprddXput.png",
                "UDP - RDD Xput",
                "Samples",
                "xput (%)",
            )

        trr.is_less_than(
            "UDP - AF Xput Avg",
            25,
            round(mean(brr_res["xput"]["AF_xput_inst"]), 2),
            "ms",
            "WI-316483",
        )
        trr.is_less_than(
            "UDP - AF Xput Max", 25, round((brr_res["xput"]["AF_xput_peak"]), 2), "ms", "WI-316483"
        )
        xputDict["AF Xput Avg"] = round(mean(brr_res["xput"]["AF_xput_inst"]), 2)
        xputDict["AF Xput Max"] = round((brr_res["xput"]["AF_xput_peak"]), 2)
        if ADD_PLOTS:
            trr.add_image(
                brr_res["xput"]["AF_xput_inst"],
                TEST_DATA_PATH + "udpafXput.png",
                "UDP - AF Xput",
                "Samples",
                "xput (%)",
            )

        trr.is_less_than(
            "UDP - RFFT Xput Avg",
            28,
            round(mean(brr_res["xput"]["RFFT_xput_inst"]), 2),
            "ms",
            "WI-316483",
        )
        trr.is_less_than(
            "UDP - RFFT Xput Max",
            28,
            round((brr_res["xput"]["RFFT_xput_peak"]), 2),
            "ms",
            "WI-316483",
        )
        xputDict["RFFT Xput Avg"] = round(mean(brr_res["xput"]["RFFT_xput_inst"]), 2)
        xputDict["RFFT Xput Max"] = round((brr_res["xput"]["RFFT_xput_peak"]), 2)
        if ADD_PLOTS:
            trr.add_image(
                brr_res["xput"]["RFFT_xput_inst"],
                TEST_DATA_PATH + "udprfftXput.png",
                "UDP - RFFT Xput",
                "Samples",
                "xput (%)",
            )

        if datadog:
            if maxXput:
                metricName = "advradar.gen7.smokeTest.MaxXput"
            else:
                metricName = "advradar.gen7.smokeTest.xput"
            f = open("dataDogMetrics.txt", "w")

            for key in xputDict:
                tags = ["gen7_env:test", "Type:" + key, "Variant:" + variant]
                if swVersion:
                    tags.append("SW_Version:" + swVersion)
                f.write("{0};{1};{2};{3}\n".format(branch, metricName, xputDict[key], tags))

                dataDogMetrics.sendGaugeMetric(
                    metricName,
                    xputDict[key],
                    ["gen7_env:test", "Type:" + key, "Variant:" + variant],
                )

            f.close()

    if DISPLAY_XCP_VARS:
        if "totalRunTime" in xcp_res:
            trr.is_less_than(
                "XCP - DFFT+Rdd time Avg", 25, round(mean(xcp_res["totalRunTime"]), 2), "ms"
            )
            trr.is_less_than(
                "XCP - DFFT+Rdd_time Max", 25, round(max(xcp_res["totalRunTime"]), 2), "ms"
            )
            trr.is_less_than(
                "XCP - DFFT+Rdd_time Min", 25, round(min(xcp_res["totalRunTime"]), 2), "ms"
            )

    xcp_res.close()


def report_TimeSync(trr, brr_res):
    """
    Report the Time Sync status.
    """
    trr.add_info_row("-- TIME SYNC STATUS --")

    trr.is_equal(
        "UDP - Sensor Timestamp Sec Skips",
        0,
        round(((checkSlowSkips(brr_res["sensorTimestampSec"]))), 3),
        "%",
    )

    if brr_res["sensorTimestampNs"]:
        [_min, _max, _mean] = get_ns_diff_data(brr_res["timeStamps"]["timeStampNs"])

        trr.is_almost_equal("UDP - Sensor Timestamp ns Avg", 50, 3, round(_mean, 3), "ms")
        trr.is_less_than("UDP - Sensor Timestamp ns Max", 53, round(_max, 3), "ms")
        trr.is_greater_than("UDP - Sensor Timestamp ns Min", 47, round(_min, 3), "ms")

    trr.is_equal(
        "UDP - Time Sync Validiy Flag  (No Toggling)",
        0,
        getToggleStatus(brr_res["timeSyncValidityFlag"]),
    )
    trr.is_equal(
        "UDP - Time Sync Source Master (No Toggling)",
        0,
        getToggleStatus(brr_res["timeSyncSourceMaster"]),
    )


def report_Overrun(trr, brr_res):
    """
    Report the overruns detected for AF, RDD and Range.
    """
    trr.add_info_row("-- OVERRUN RESULTS --")

    if brr_res["afOverrunCnt"]:
        trr.is_equal("UDP - Total AF Overruns", 0, brr_res["afOverrunCnt"])
    if brr_res["rddOverrunCnt"]:
        trr.is_equal("UDP - Total RDD Overruns", 0, brr_res["rddOverrunCnt"])
    if brr_res["rangeOverrunCnt"]:
        trr.is_equal("UDP - Total Range Overruns", 0, brr_res["rangeOverrunCnt"])
    if brr_res["dopplerOverrunCnt"]:
        trr.is_equal("UDP - Total Doppler Overruns", 0, brr_res["dopplerOverrunCnt"])


def report_TempInfo(trr, brr_res):
    """
    Report the temperature values.
    """
    trr.add_info_row("-- TEMPERATURE INFO --")

    brr_temps = []
    if brr_res["temps"]:
        brr_temps = brr_res["temps"]

        if ADD_PLOTS:
            trr.add_image_compare4(
                TEST_DATA_PATH + "udpmmic_temp_tx_all.png",
                brr_temps["mmic_temp_tx0"],
                brr_temps["mmic_temp_tx1"],
                "mmic_temp_tx0",
                "mmic_temp_tx1",
                data3=brr_temps["mmic_temp_tx2"],
                label3="mmic_temp_tx2",
                title="UDP - MMIC Temp TX",
                xlabel="Samples",
                ylabel="Temp (C)",
            )

        trr.is_almost_equal(
            "UDP - MMIC Temp TX0",
            90,
            40,
            round(mean(brr_temps["mmic_temp_tx0"]), 2),
            "C",
            "WI-316478",
        )
        trr.is_almost_equal(
            "UDP - MMIC Temp TX1",
            90,
            40,
            round(mean(brr_temps["mmic_temp_tx1"]), 2),
            "C",
            "WI-316478",
        )
        trr.is_almost_equal(
            "UDP - MMIC Temp TX2",
            90,
            40,
            round(mean(brr_temps["mmic_temp_tx2"]), 2),
            "C",
            "WI-316478",
        )

        if ADD_PLOTS:
            trr.add_image_compare4(
                TEST_DATA_PATH + "udpmmic_temp_rx_all.png",
                brr_temps["mmic_temp_rx0"],
                brr_temps["mmic_temp_rx1"],
                "mmic_temp_rx0",
                "mmic_temp_rx1",
                data3=brr_temps["mmic_temp_rx2"],
                data4=brr_temps["mmic_temp_rx3"],
                label3="mmic_temp_rx2",
                label4="mmic_temp_rx3",
                title="UDP - MMIC Temp RX",
                xlabel="Samples",
                ylabel="Temp (C)",
            )

        trr.is_almost_equal(
            "UDP - MMIC Temp RX0",
            90,
            40,
            round(mean(brr_temps["mmic_temp_rx0"]), 2),
            "C",
            "WI-316478",
        )
        trr.is_almost_equal(
            "UDP - MMIC Temp RX1",
            90,
            40,
            round(mean(brr_temps["mmic_temp_rx1"]), 2),
            "C",
            "WI-316478",
        )
        trr.is_almost_equal(
            "UDP - MMIC Temp RX2",
            90,
            40,
            round(mean(brr_temps["mmic_temp_rx2"]), 2),
            "C",
            "WI-316478",
        )
        trr.is_almost_equal(
            "UDP - MMIC Temp RX3",
            90,
            40,
            round(mean(brr_temps["mmic_temp_rx3"]), 2),
            "C",
            "WI-316478",
        )

        if len(brr_temps["c66_temp"]) > 0:
            trr.is_almost_equal(
                "UDP - C66 Temp", 90, 40, round(mean(brr_temps["c66_temp"]), 2), "C"
            )
            trr.is_almost_equal(
                "UDP - Radar Temp", 90, 40, round(mean(brr_temps["radar_temp"]), 2), "C"
            )
            trr.is_almost_equal(
                "UDP - Tmu Temp", 90, 40, round(mean(brr_temps["tmu_temp"]), 2), "C"
            )
        if len(brr_temps["dsp_temp"]) > 0:
            trr.is_almost_equal(
                "UDP - Dsp Temp", 90, 40, round(mean(brr_temps["dsp_temp"]), 2), "C"
            )
            trr.is_almost_equal(
                "UDP - Hwa Temp", 90, 40, round(mean(brr_temps["hwa_temp"]), 2), "C"
            )
            trr.is_almost_equal(
                "UDP - Hsm Temp", 90, 40, round(mean(brr_temps["hsm_temp"]), 2), "C"
            )


def report_VoltageConditions(trr, brr_res):
    """
    Report the reported voltage values.
    """
    trr.add_info_row("-- VOLTAGE CONDITIONS --")

    brr_volts = None
    if brr_res["voltage"]:
        brr_volts = brr_res["voltage"]
        trr.is_almost_equal(
            "UDP - Supply Volts 0.82V",
            0.82,
            0.2,
            round(mean(brr_volts["supplyVolts082V"]), 3),
            "V",
        )
        trr.is_almost_equal(
            "UDP - Supply Volts 1.1V", 1.1, 0.2, round(mean(brr_volts["supplyVolts11V"]), 3), "V"
        )
        trr.is_almost_equal(
            "UDP - Supply Volts 1.8V", 1.8, 0.2, round(mean(brr_volts["supplyVolts18V"]), 3), "V"
        )
        trr.is_almost_equal(
            "UDP - Supply Volts 2.3V", 2.3, 0.2, round(mean(brr_volts["supplyVolts23V"]), 3), "V"
        )
        trr.is_almost_equal(
            "UDP - Supply Volts 3.3V", 3.3, 0.2, round(mean(brr_volts["supplyVolts33V"]), 3), "V"
        )
        trr.is_almost_equal(
            "UDP - Supply Volts 5.0V", 5.0, 0.2, round(mean(brr_volts["supplyVolts5V"]), 3), "V"
        )


def report_RddDistribution(trr, brr_res):
    """
    Report the RDD distribution results.
    """
    diff = []
    if (4 in brr_res["packets"] and brr_res["packets"][4] > 0) or (
        1 in brr_res["packets"] and brr_res["packets"][1] > 0
    ):
        trr.add_info_row("-- RDD DISTRIBUTION --")

        for i in range(0, len(brr_res["numFpDetections"])):
            diff.append(brr_res["numFpDetections"][i] - brr_res["numSpDetections"][i])

        if len(brr_res["numFpDetections"]) > 0:
            trr.is_greater_than(
                "UDP Number of First Pass Detections Max",
                0,
                round(max(brr_res["numFpDetections"]), 1),
            )
            # trr.is_almost_equal("UDP Number of First Pass Detections Max", 256, 256, round(max(brr_res['numFpDetections']),1))
            trr.is_greater_than_or_equal(
                "UDP Number of First Pass Detections Min",
                0,
                round(min(brr_res["numFpDetections"]), 1),
            )
            # trr.is_almost_equal("UDP Number of First Pass Detections Min", 256, 256, round(min(brr_res['numFpDetections']),1))
            trr.is_greater_than(
                "UDP Number of First Pass Detections Avg",
                0,
                round(mean(brr_res["numFpDetections"]), 1),
            )
            # trr.is_almost_equal("UDP Number of First Pass Detections Avg", 256, 256, round(mean(brr_res['numFpDetections']),1))

        if len(brr_res["numSpDetections"]) > 0:
            trr.is_greater_than(
                "UDP Number of Second Pass Detections Max",
                0,
                round(max(brr_res["numSpDetections"]), 1),
            )
            trr.is_greater_than_or_equal(
                "UDP Number of Second Pass Detections Min",
                0,
                round(min(brr_res["numSpDetections"]), 1),
            )
            trr.is_greater_than(
                "UDP Number of Second Pass Detections Avg",
                0,
                round(mean(brr_res["numSpDetections"]), 1),
            )

        #        if(ADD_PLOTS): trr.add_image_compare(brr_res['numFpDetections'], brr_res['numSpDetections'], "FP", "SP",
        #       TEST_DATA_PATH+"udpNumFPSPDetec.png","UDP - Number of FP vs SP Detections","Samples","Detections")

        if ADD_PLOTS:
            trr.add_image_compare5(
                brr_res["numFpDetections"],
                brr_res["numSpDetections"],
                "FP",
                "SP",
                TEST_DATA_PATH + "udpNumFPSPDetec5.png",
                "UDP - Number of FP vs SP Detections",
                "Samples",
                "Detections",
            )

    if len(brr_res["minChirpScaling"]) > 0:
        trr.is_greater_than("UDP Min Chirp Scaling Max", 0, max(brr_res["minChirpScaling"]))


def report_DataCoherency(trr, brr_res):
    """
    Report the Data coherency results.
    """
    trr.add_info_row("-- DATA COHERENCY --")

    trr.add_result_row(
        "Range Coverage (Look Type 0)", round(brr_res["dataCoherency"]["0"]["rddRangeCoverage"], 3)
    )
    trr.add_result_row(
        "Doppler Coverage (Look Type 0)",
        round(brr_res["dataCoherency"]["0"]["rddDoppCoverage"], 3),
    )
    trr.add_result_row(
        "Range Coverage (Look Type 1)", round(brr_res["dataCoherency"]["1"]["rddRangeCoverage"], 3)
    )
    trr.add_result_row(
        "Doppler Coverage (Look Type 1)",
        round(brr_res["dataCoherency"]["1"]["rddDoppCoverage"], 3),
    )
    trr.add_result_row(
        "Range Coverage (Look Type 2)", round(brr_res["dataCoherency"]["2"]["rddRangeCoverage"], 3)
    )
    trr.add_result_row(
        "Doppler Coverage (Look Type 2)",
        round(brr_res["dataCoherency"]["2"]["rddDoppCoverage"], 3),
    )
    trr.add_result_row(
        "Range Coverage (Look Type 3)", round(brr_res["dataCoherency"]["3"]["rddRangeCoverage"], 3)
    )
    trr.add_result_row(
        "Doppler Coverage (Look Type 3)",
        round(brr_res["dataCoherency"]["3"]["rddDoppCoverage"], 3),
    )


def report_AfDistribution(trr, brr_res):
    """
    Report the AF distribution.
    """
    trr.add_info_row("-- AF DISTRIBUTION --")

    if brr_res["targetCnts"]:
        trr.is_greater_than(
            "UDP - AF Number of Targets Max", 0, round(max(brr_res["targetCnts"]), 1)
        )
        trr.is_greater_than_or_equal(
            "UDP - AF Number of Targets Min", 0, round(min(brr_res["targetCnts"]), 1)
        )
        trr.is_greater_than(
            "UDP - AF Number of Targets Avg", 0, round(mean(brr_res["targetCnts"]), 1)
        )

        if ADD_PLOTS:
            trr.add_image(
                brr_res["targetCnts"],
                TEST_DATA_PATH + "udptarget_count.png",
                "UDP - AF Number of Targets Max",
                "Samples",
                "Detections",
            )

    if brr_res["af_detections"]:
        trr.is_greater_than(
            "UDP - AF Number of Detections Max", 0, round(max(brr_res["af_detections"]), 1)
        )
        trr.is_greater_than_or_equal(
            "UDP - AF Number of Detections Min", 0, round(min(brr_res["af_detections"]), 1)
        )
        trr.is_greater_than(
            "UDP - AF Number of Detections Avg", 0, round(mean(brr_res["af_detections"]), 1)
        )

        if ADD_PLOTS:
            trr.add_image(
                brr_res["af_detections"],
                TEST_DATA_PATH + "udpDetection_count.png",
                "UDP - AF Number of Detections Max",
                "Samples",
                "Detections",
            )


def report_AfPathHistogram(trr, brr_res):
    """
    Report the AF path histogram.
    """
    trr.add_info_row("-- AF PATH HISTOGRAM --")


def report_SystemStatusInfo(trr, brr_res):
    """
    Report the system status information.
    """
    trr.add_info_row("-- SYSTEM STATUS INFO --")

    trr.is_equal("UDP MMIC Error Counter", 0, max(brr_res["mmic"]["mmic_err_counter"]))
    if max(brr_res["mmic"]["mmic_err_counter"]) > 0:
        trr.add_image(
            brr_res["mmic"]["mmic_err_counter"],
            TEST_DATA_PATH + "mmicErrorCounter.png",
            "MMIC Error Counter",
            "Samples",
            "Error Counter",
        )
    trr.is_equal("UDP MMIC Error Code", 0, max(brr_res["mmic"]["mmic_err_code"]))
    if max(brr_res["mmic"]["mmic_err_code"]) > 0:
        trr.add_image(
            brr_res["mmic"]["mmic_err_code"],
            TEST_DATA_PATH + "mmicErrorCode.png",
            "MMIC Error Code",
            "Samples",
            "Error Code",
        )

    # radarPostDaqCountSkips = skipCounter(brr_res['radarPostDaqCount'])
    # trr.is_equal('UDP Radar Post Daq Count Skips (%0.2f)'%(radarPostDaqCountSkips/len(brr_res['radarPostDaqCount'])),    0, radarPostDaqCountSkips)
    # NEW

    rRadarPostDaqCntSkips = skipCounter(brr_res["RadarPostDaqCnt"])
    trr.is_less_than(
        "UDP Radar Post Daq Count Skips ({0}/{1})".format(
            rRadarPostDaqCntSkips, len(brr_res["RadarPostDaqCnt"])
        ),
        0.1,
        round((rRadarPostDaqCntSkips / len(brr_res["RadarPostDaqCnt"])) * 100, 2),
        "%",
    )

    if rRadarPostDaqCntSkips > 0:
        trr.add_image(
            brr_res["RadarPostDaqCnt"],
            TEST_DATA_PATH + "RadarPostDaqCnt.png",
            "Radar Post Daq Count",
            "Samples",
            "Count",
        )

    # modeTrigCountSkips = skipCounter(brr_res['modeTrigCount'])
    # trr.is_equal('UDP Mode Trigger Count Skips',    0, modeTrigCountSkips)
    # NEW
    # ModeTrigCntSkips = skipCounter(brr_res["ModeTrigCnt"])
    # --- Disabled until the 50ms rate is integrated, this always will fail until then
    # trr.is_less_than("UDP Mode Trigger Count Skips ({0}/{1})".format(ModeTrigCntSkips,len(brr_res['ModeTrigCnt'])),
    # 0.1, 	round((ModeTrigCntSkips/len(brr_res['ModeTrigCnt']))*100,2), "%")

    # if (ModeTrigCntSkips > 0): trr.add_image(brr_res['ModeTrigCnt'], TEST_DATA_PATH+"ModeTrigCnt.png","Mode Trigger Count","Samples", "Count")

    # radarPostRddCountSkips = skipCounter(brr_res['radarPostRddCount'])
    # trr.is_equal('UDP Radar post rdd Count Skips',    0, radarPostRddCountSkips)
    # NEW
    RadarPostRDDCntSkips = skipCounter(brr_res["RadarPostRDDCnt"])
    trr.is_less_than(
        "UDP Radar post rdd Count Skips ({0}/{1})".format(
            RadarPostRDDCntSkips, len(brr_res["RadarPostRDDCnt"])
        ),
        0.1,
        round((RadarPostRDDCntSkips / len(brr_res["RadarPostRDDCnt"])) * 100, 2),
        "%",
    )
    if RadarPostRDDCntSkips > 0:
        trr.add_image(
            brr_res["RadarPostRDDCnt"],
            TEST_DATA_PATH + "RadarPostRDDCnt.png",
            "Radar Post RDD Count",
            "Samples",
            "Count",
        )

    trr.is_greater_than(
        "UDP Radar Post Daq Count", 0, getToggleStuckStatus(brr_res["RadarPostDaqCnt"])
    )
    # trr.is_greater_than('UDP Mode Trigger Count',      0, getToggleStuckStatus(brr_res['ModeTrigCnt']))
    trr.is_greater_than(
        "UDP Radar post rdd Count", 0, getToggleStuckStatus(brr_res["RadarPostRDDCnt"])
    )
    # trr.is_equal('UDP Heartbeat Count',         0, getToggleStuckStatus(brr_res['heartbeatCount']))
    trr.is_equal("UDP Ipc dsp to sram Count", 0, getToggleStuckStatus(brr_res["ipcDspToSramCnt"]))
    trr.is_equal("UDP Range Overrun Count", 0, getToggleStuckStatus(brr_res["rangeOverrunCnt"]))
    trr.is_equal(
        "UDP Doppler Overrun Count", 0, getToggleStuckStatus(brr_res["dopplerOverrunCnt"])
    )
    trr.is_equal(
        "UDP Chirp out of sync Count", 0, getToggleStuckStatus(brr_res["chirpOutOfSyncCnt"])
    )
    trr.is_equal(
        "UDP Interrupt Error Count", 0, getToggleStuckStatus(brr_res["interruptErrCounts"])
    )
    trr.is_equal(
        "UDP Ipc M7 to DSP Error Count", 0, getToggleStuckStatus(brr_res["ipcr50ToDspErrCnt"])
    )
    trr.is_equal("UDP Evt disp Error Count", 0, getToggleStuckStatus(brr_res["evtDispOverrunCnt"]))
    trr.is_equal(
        "UDP Dsp spt look id Error Count", 0, getToggleStuckStatus(brr_res["dspSptLookIdErrCnt"])
    )
    trr.is_equal("UDP Rbin Error Count", 0, getToggleStuckStatus(brr_res["rbinErrorCnt"]))

    trr.is_equal(
        "UDP SMC Load Status", 0, getToggleStuckStatus(brr_res["loadStatus"]["smc_load_status"])
    )
    trr.is_equal(
        "UDP USC Load Status", 0, getToggleStuckStatus(brr_res["loadStatus"]["usc_load_status"])
    )
    # trr.is_equal('UDP Radar Cals Load Status',  0, getToggleStuckStatus(brr_res['loadStatus']['radar_cals_load']))

    # if (max(brr_res['heartbeatCount']) > 0): trr.add_image(brr_res['heartbeatCount'], TEST_DATA_PATH+"heartbeatCount.png","heartbeatCount","Samples", "Count")
    if max(brr_res["ipcDspToSramCnt"]) > 0:
        trr.add_image(
            brr_res["ipcDspToSramCnt"],
            TEST_DATA_PATH + "ipcDspToSramCnt.png",
            "ipcDspToSramCnt",
            "Samples",
            "Count",
        )
    if max(brr_res["rangeOverrunCnt"]) > 0:
        trr.add_image(
            brr_res["rangeOverrunCnt"],
            TEST_DATA_PATH + "rangeOverrunCnt.png",
            "rangeOverrunCnt",
            "Samples",
            "Count",
        )
    if max(brr_res["dopplerOverrunCnt"]) > 0:
        trr.add_image(
            brr_res["dopplerOverrunCnt"],
            TEST_DATA_PATH + "dopplerOverrunCnt.png",
            "dopplerOverrunCnt",
            "Samples",
            "Count",
        )
    if max(brr_res["chirpOutOfSyncCnt"]) > 0:
        trr.add_image(
            brr_res["chirpOutOfSyncCnt"],
            TEST_DATA_PATH + "chirpOutOfSyncCnt.png",
            "chirpOutOfSyncCnt",
            "Samples",
            "Count",
        )
    if max(brr_res["interruptErrCounts"]) > 0:
        trr.add_image(
            brr_res["interruptErrCounts"],
            TEST_DATA_PATH + "interruptErrCounts.png",
            "interruptErrCounts",
            "Samples",
            "Count",
        )
    if max(brr_res["ipcr50ToDspErrCnt"]) > 0:
        trr.add_image(
            brr_res["ipcr50ToDspErrCnt"],
            TEST_DATA_PATH + "ipcr50ToDspErrCnt.png",
            "ipcr50ToDspErrCnt",
            "Samples",
            "Count",
        )
    if max(brr_res["evtDispOverrunCnt"]) > 0:
        trr.add_image(
            brr_res["evtDispOverrunCnt"],
            TEST_DATA_PATH + "evtDispOverrunCnt.png",
            "evtDispOverrunCnt",
            "Samples",
            "Count",
        )
    if max(brr_res["dspSptLookIdErrCnt"]) > 0:
        trr.add_image(
            brr_res["dspSptLookIdErrCnt"],
            TEST_DATA_PATH + "dspSptLookIdErrCnt.png",
            "dspSptLookIdErrCnt",
            "Samples",
            "Count",
        )
    if max(brr_res["rbinErrorCnt"]) > 0:
        trr.add_image(
            brr_res["rbinErrorCnt"],
            TEST_DATA_PATH + "rbinErrorCnt.png",
            "rbinErrorCnt",
            "Samples",
            "Count",
        )


def report_CDC(trr, brr_res):
    """
    Report the CDC bin breakdown.
    """
    import datetime

    # ct stores current time
    ct = datetime.datetime.now()
    print("Time:", ct)

    trr.add_info_row("-- CDC BINS --")

    maximum_range_idx = 420
    maximum_doppler_idx = 256

    each_range_bin_count = []
    each_data_frame_count = []
    skipped = 0
    print("RngDopInx Len: " + str(len(brr_res["cdc"]["RngDopIdx"])))
    for k in range(0, len(brr_res["cdc"]["RngDopIdx"])):

        rdm = [[0 for i in range(maximum_range_idx)] for j in range(maximum_doppler_idx)]
        cdc_struct = brr_res["cdc"]["RngDopIdx"][k]

        for num_cdc_ele in range(0, len(cdc_struct["rngIdx"])):
            range_index = cdc_struct["rngIdx"][num_cdc_ele] + 1
            doppler_index = cdc_struct["dopIdx"][num_cdc_ele] + 1

            # print(doppler_index, '-', range_index)
            if (doppler_index >= maximum_doppler_idx) or (range_index >= maximum_range_idx):
                skipped += 1
                continue
            else:
                rdm[doppler_index][range_index] = 1

        each_range_bin_count += numpy.sum(rdm, axis=0).tolist()
        each_data_frame_count.append(sum(sum(rdm, [])))

    ct = datetime.datetime.now()
    print("Time:", ct)
    print("Skipped Total: " + str(skipped))
    trr.is_greater_than(
        "Max Number of filled bins in a single range bin", 0, max(each_range_bin_count)
    )
    trr.is_greater_than(
        "Max Number of filled bins in a single data frame", 0, max(each_data_frame_count)
    )

    if ADD_PLOTS:
        trr.add_image_bar(
            each_range_bin_count,
            TEST_DATA_PATH + "cdcrngIdx.png",
            "Filled CDC Bins in each Range Bin",
            "Number of CDC Bins per Range Bin",
            "Number of Occurrences",
        )
    if ADD_PLOTS:
        trr.add_image_bar(
            each_data_frame_count,
            TEST_DATA_PATH + "cdcdopIdx.png",
            "Filled CDC Bins in each Data Frame",
            "Number of CDC Bins per Data Frame",
            "Number of Occurrences",
        )

    root.info("each_range_bin_count")
    root.info(each_range_bin_count)
    root.info("each_data_frame_count")
    root.info(each_data_frame_count)

    ct = datetime.datetime.now()
    print("Time:", ct)


def report_CDC_Statistics(trr, brr_res):
    """
    Report the CDC statistics.
    """
    trr.add_info_row("-- CDC STATISTICS --")

    # CDC Limit Parameters
    trr.add_result_row("Max Log Bins", max(brr_res["cdc"]["max_log_bins"]))
    trr.add_result_row(
        "Max Doppler Log Bins per Range Bin",
        max(brr_res["cdc"]["max_doppler_log_bins_per_range_bin"]),
    )

    # Limiting Effect Counters to Plot
    if ADD_PLOTS:
        trr.add_image(
            brr_res["cdc"]["range_bins_limited_count"],
            TEST_DATA_PATH + "range_bins_limited_count.png",
            "Range Bins Limited Count",
            "Look Index",
            "Count",
        )
    if ADD_PLOTS:
        trr.add_image(
            brr_res["cdc"]["detection_range_rate_bins_limited_count"],
            TEST_DATA_PATH + "detection_range_rate_bins_limited_count.png",
            "Doppler Bins Limited Count",
            "Look Index",
            "Count",
        )

    # CDC Statistics Overall Maximum Values
    trr.add_result_row(
        "Max Doppler Bins Logged for One Range Index",
        max(brr_res["cdc"]["num_dbins_per_rbin_max"]),
    )
    trr.add_result_row("Max Bins Logged for All Looks", max(brr_res["cdc"]["look_total_bins_max"]))
    trr.add_result_row(
        "Max CDC Ethernet Frames for All Looks", max(brr_res["cdc"]["look_num_frames_max"])
    )

    # CDC Statistics to Plot
    if ADD_PLOTS:
        trr.add_image(
            brr_res["cdc"]["look_num_dbins_per_rbin_max"],
            TEST_DATA_PATH + "look_num_dbins_per_rbin_max.png",
            "Max Dopper Bins Logged for One RangeIndex for Each Look",
            "Look Index",
            "Number of Doppler Bins",
        )
    if ADD_PLOTS:
        trr.add_image(
            brr_res["cdc"]["look_total_bins"],
            TEST_DATA_PATH + "look_total_bins.png",
            "Max Bins Logged for Each Look",
            "Look Index",
            "Number of Bins",
        )
    if ADD_PLOTS:
        trr.add_image(
            brr_res["cdc"]["look_num_frames"],
            TEST_DATA_PATH + "look_num_frames.png",
            "Max CDC Ethernet Frames Transmitted for Each Look",
            "Look Index",
            "Number of CDC Ethernet Frames",
        )

    # trr.add_info_row('-- CDC Statistics Maximum Values --')

    trr.add_result_row(
        "Max Ethernet Frames Buffered and Pending Transmission",
        max(brr_res["cdc"]["ENET_TxBuffers_Pending_Count_MAX"]),
    )

    trr.is_equal(
        "Max Times an Ethernet Buffer Was Requested But Unavailable for CDC",
        0,
        max(brr_res["cdc"]["ENET_CDC_Tx_Buf_UNAVBL_Count"]),
    )
    trr.is_equal(
        "Max Times an Ethernet Buffer Was Requested But Unavailable for non-CDC UDP Streams",
        0,
        max(brr_res["cdc"]["ENET_ARP_Tx_Buf_UNAVBL_Count"]),
    )
    trr.is_equal(
        "CDC Error Condition", 0, max(brr_res["cdc"]["ENET_Partial_Buffer_Mismatch_Count"])
    )


def reoprt_ExceptionCounters(trr, brr_res):
    """
    Report the float exception counters.
    """
    trr.add_info_row("-- EXCEPTION COUNTERS --")

    # CDC Statistics Overall Maximum Values
    trr.is_equal(
        "Max Float_Exception_Count_c0",
        0,
        max(brr_res["exception_cntrs"]["Float_Exception_Count_c0"]),
    )
    trr.is_equal(
        "Max Float_Exception_Count_c1",
        0,
        max(brr_res["exception_cntrs"]["Float_Exception_Count_c1"]),
    )
    trr.is_equal(
        "Max Float_Exception_Count_c2",
        0,
        max(brr_res["exception_cntrs"]["Float_Exception_Count_c2"]),
    )

    trr.is_equal(
        "Max c1_c2_ipc_err_cntr", 0, max(brr_res["exception_cntrs"]["c1_c2_ipc_err_cntr"])
    )
    trr.is_equal(
        "Max c0_c2_ipc_err_cntr", 0, max(brr_res["exception_cntrs"]["c0_c2_ipc_err_cntr"])
    )

    if max(brr_res["exception_cntrs"]["Float_Exception_Count_c0"]) > 0:
        if ADD_PLOTS:
            trr.add_image(
                brr_res["exception_cntrs"]["Float_Exception_Count_c0"],
                TEST_DATA_PATH + "Float_Exception_Count_c0.png",
                "Float_Exception_Count_c0",
                "Samples",
                "Count",
            )
    if max(brr_res["exception_cntrs"]["Float_Exception_Count_c1"]) > 0:
        if ADD_PLOTS:
            trr.add_image(
                brr_res["exception_cntrs"]["Float_Exception_Count_c1"],
                TEST_DATA_PATH + "Float_Exception_Count_c1.png",
                "Float_Exception_Count_c1",
                "Samples",
                "Count",
            )
    if max(brr_res["exception_cntrs"]["Float_Exception_Count_c2"]) > 0:
        if ADD_PLOTS:
            trr.add_image(
                brr_res["exception_cntrs"]["Float_Exception_Count_c2"],
                TEST_DATA_PATH + "Float_Exception_Count_c2.png",
                "Float_Exception_Count_c2",
                "Samples",
                "Count",
            )

    if max(brr_res["exception_cntrs"]["c1_c2_ipc_err_cntr"]) > 0:
        if ADD_PLOTS:
            trr.add_image(
                brr_res["exception_cntrs"]["c1_c2_ipc_err_cntr"],
                TEST_DATA_PATH + "c1_c2_ipc_err_cntr.png",
                "c1_c2_ipc_err_cntr",
                "Samples",
                "Count",
            )
    if max(brr_res["exception_cntrs"]["c0_c2_ipc_err_cntr"]) > 0:
        if ADD_PLOTS:
            trr.add_image(
                brr_res["exception_cntrs"]["c0_c2_ipc_err_cntr"],
                TEST_DATA_PATH + "c0_c2_ipc_err_cntr.png",
                "c0_c2_ipc_err_cntr",
                "Samples",
                "Count",
            )


def report_Bandwidth(trr):
    """
    Report the ethernet bandwidth breakdown.
    """
    trr.add_info_row("-- ETHERNET BANDWIDTH --")

    brr_res = shelve.open(BRR_SHELF)
    totalTime = brr_res["lastTP"] - brr_res["firstTP"]

    for stream in brr_res["dataLengths"]:
        bits = brr_res["dataLengths"][stream] * 8 / 1000000 / totalTime
        trr.is_less_than(
            "UDP Stream {0} Bandwidth Avg".format(stream), 20, round(bits, 3), "Mbps", "WI-316477"
        )

    brr_res.close()

    if len(avg_eth_bandwidth):
        root.info(
            "Ethernet Bandwidth (Avg): %f  (Max): %f  (Min): %f",
            mean(avg_eth_bandwidth),
            max(avg_eth_bandwidth),
            min(avg_eth_bandwidth),
        )

        trr.is_less_than(
            "UDP Total Ethernet Bandwidth Avg",
            50,
            round(mean(avg_eth_bandwidth), 2),
            "Mbps",
            "WI-316477",
        )
        trr.is_less_than(
            "UDP Total Ethernet Bandwidth Max",
            50,
            round(max(avg_eth_bandwidth), 2),
            "Mbps",
            "WI-316477",
        )
        trr.is_greater_than(
            "UDP Total Ethernet Bandwidth Min",
            0,
            round(min(avg_eth_bandwidth), 2),
            "Mbps",
            "WI-316477",
        )
        if ADD_PLOTS:
            trr.add_image(
                avg_eth_bandwidth,
                TEST_DATA_PATH + "avg_eth_bandwidth.png",
                "Ethernet Bandwidth",
                "Samples",
                "Bandwidth (Mbps)",
            )


def SetTestSourceConditions(elfFile):
    """
    Set the condition for test source.
    """
    if path.exists(elfFile):
        time.sleep(10)
        xcpHandler = XcpHandler(elfFile)

        if xcpHandler.xcpInit():
            addr_xcp_rdd_vary_xput_flag, size = xcpHandler.getAddress("xcp_rdd_vary_xput_flag")
    else:
        print("elf file does not exist")


def CleanUp():
    """
    Delete the brr and xcp shelf files.
    """
    try:
        if path.exists(BRR_SHELF + ".dat"):
            remove(BRR_SHELF + ".dat")
        if path.exists(BRR_SHELF + ".bak"):
            remove(BRR_SHELF + ".bak")
        if path.exists(BRR_SHELF + ".dir"):
            remove(BRR_SHELF + ".dir")
        if path.exists(XCP_SHELF + ".dat"):
            remove(XCP_SHELF + ".dat")
        if path.exists(XCP_SHELF + ".bak"):
            remove(XCP_SHELF + ".bak")
        if path.exists(XCP_SHELF + ".dir"):
            remove(XCP_SHELF + ".dir")
        if path.exists("dataDogMetrics.txt"):
            remove("dataDogMetrics.txt")
    except Exception as e:
        print("Could not delete shelf files: %s", e)


def CreateHTMLReport(trr, outputHtml, variant):
    """
    Create the html file and jenkins files.
    """
    trr.timer.stop()
    now = datetime.now()

    # if an outhtml path is given use it, otherwise create a file name with variant and timestamp info
    if outputHtml:
        reportFileName = outputHtml
    else:
        reportFileName = "{}_Integration_Test_{}_{}_{}_{}_{}_{}".format(
            variant, now.year, now.month, now.day, now.hour, now.minute, now.second
        )

    # checking if the directory REPORT_PATH exist or not.
    if not os.path.isdir(REPORT_PATH):
        # if the REPORT_PATH directory is not present then create it.
        os.makedirs(REPORT_PATH)
    trr.generate_html_report(REPORT_PATH, reportFileName, open_report=False)

    print("Copying xml and html to base directory for Jenkins to be able to read them.")
    system(
        "copy {} {}".format(
            REPORT_PATH + reportFileName + "_jenkins.xml", getcwd() + "\\..\\..\\.."
        )
    )
    system("copy {} {}".format(REPORT_PATH + reportFileName + ".html", getcwd() + "\\..\\..\\.."))


# ##############################################################################3
if __name__ == "__main__":

    print("Starting Integration Test...")
    parser = argparse.ArgumentParser(
        description="APTIV Gen7 Radar Integration Test", epilog="APTIV Ltd, copyright 2022"
    )
    parser.add_argument(
        "-t",
        "--time",
        metavar="Test duration",
        type=int,
        required=False,
        help="Test time duration in seconds",
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
        required=True,
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
        "-g", "--graphs", metavar="Create Graphs", type=bool, default=True, required=False
    )
    parser.add_argument(
        "-c", "--comment", metavar="Test Comments", type=str, default="", required=False
    )
    parser.add_argument(
        "-m", "--maxXput", metavar="Max Throughput Test", type=str, default="false", required=False
    )
    parser.add_argument(
        "-s", "--testSource", metavar="Test Source Test", type=str, default="false", required=False
    )
    parser.add_argument(
        "-a", "--elfFile", metavar="elf File", type=str, default="", required=False
    )
    parser.add_argument("-b", "--branch", metavar="Branch", type=str, default=None, required=False)
    parser.add_argument(
        "-w", "--wiresharklua", metavar="wiresharklua", type=str, default=None, required=False
    )
    parser.add_argument(
        "-o", "--outputHtml", metavar="outputHtml", type=str, default=None, required=False
    )
    parser.add_argument(
        "-r", "--swVersion", metavar="swVersion", type=str, default="", required=False
    )

    args = parser.parse_args()
    branch = args.branch
    test_time = args.time
    variant = args.variant
    eth_nic = args.ethnic
    cmt = args.comment
    comPort = args.comPort
    luaScript = args.wiresharklua
    elfFile = args.elfFile
    outputHtml = args.outputHtml
    swVersion = args.swVersion
    ADD_PLOTS = args.graphs

    saveLuaScript(luaScript)

    # values from build parameters arent the same as python boolean so
    # I turned maxXput and testSource into strings
    if args.maxXput == "false":
        maxXput = False
    else:
        maxXput = True

    if args.testSource == "false":
        testSource = False
    else:
        testSource = True
    sensorPosition = args.position

    print("Arguments: {0}".format(sys.argv[1:]))

    powerSupply = KoradSerial(comPort, False)
    if powerSupply.connected is False:
        exit()

    if maxXput:
        cmt = cmt + ", Max Throughput Test"
    elif testSource:
        cmt = cmt + ", Test Source Test"
    else:
        cmt = cmt

    test_results_report = Tester(
        "Integration Test",
        "Integration Test Results for " + variant + " build",
        program=variant.upper(),
        comments=cmt,
    )
    test_results_report.timer.start()

    # get main logger queue
    logq = Queue(-1)
    # logger configuration
    h = logging.handlers.QueueHandler(logq)
    errq = SimpleQueue()
    root = logging.getLogger()
    root.addHandler(h)
    root.setLevel(logging.INFO)
    # processes: logger, can, brr
    log_master = Process(name="Tester01", target=master_logger, args=(logq,))
    log_master.start()

    readLuaScript()

    xcp_m_conn, xcp_s_conn = Pipe()
    xcp_proc = Process(
        name="XCP_listener",
        target=XCP_listener,
        args=(elfFile, maxXput, logq, errq, xcp_s_conn),
    )
    xcp_proc.start()

    powerSupply.output.off()
    time.sleep(2)
    chn = powerSupply.channels[0]
    # Set voltage and current settings
    chn.voltage = 12.00
    chn.current = 3.00
    powerSupply.output.on()
    print("Waiting for startup delay...")

    time.sleep(STARTUP_DELAY_TIME)

    powerSupplyChecks = 10

    current = chn.output_current
    voltage = chn.output_voltage
    if current is not None:
        while current < 0.300 and powerSupplyChecks > 0:
            print("Power Supply: " + str(current) + "A, " + str(voltage) + "V")
            time.sleep(2)
            current = chn.output_current
            voltage = chn.output_voltage
            powerSupplyChecks = powerSupplyChecks - 1

    if powerSupplyChecks == 0:
        print("ERROR: Current draw never reached expected level")
        exit()

    print("Power Supply: " + str(current) + "A, " + str(voltage) + "V")

    eth_bw_chkr = Thread(target=eth_bw_meas, args=(eth_nic, errq))

    # connectors for the spawned processes
    brr_m_conn, brr_s_conn = Pipe()
    brr_m_conn2, brr_s_conn2 = Pipe()

    brr_proc = Process(
        name="BRR_listener",
        target=brr_listener,
        args=(sensorPosition, logq, errq, brr_s_conn, eth_nic),
    )
    brr_proc.start()
    brr_proc2 = Process(
        name="BRR_listener_IndexCheck",
        target=brr_listener_indexcheck,
        args=(sensorPosition, logq, errq, brr_s_conn2, eth_nic, variant),
    )
    brr_proc2.start()

    # Main testing loop STARTS here
    root.info("Starting test")
    prog = tqdm.trange(test_time, desc="Timed Test Progress")
    start_tmstamp = time.time()
    try:
        eth_bw_chkr.start()

        if test_time > 0:
            timed_test()

    except tester_Error as err:
        root.error("{0}".format(err))
        print("{0}".format(err))
    # Finish test loop

    net_bw_exit.clear()
    brr_m_conn.send("stop")
    xcp_m_conn.send("stop")
    time.sleep(10)
    brr_m_conn2.send("stop")
    xcp_proc.join()
    brr_proc.join()
    time.sleep(2)
    brr_proc2.join()
    root.info("Sleeping for 5...")
    time.sleep(5)

    root.info("Finishing test...")
    root.info("*****TEST RESULTS*****")

    time.sleep(2)

    with shelve.open(BRR_SHELF) as brr_res:
        if "UDPMsgCntr" not in brr_res:
            logq.put_nowait(None)
            log_master.join()
            print("ERROR: No BRR data received in Process 1. (exit code 5)")
            exit(5)
        elif "total_udp_frames" not in brr_res:
            logq.put_nowait(None)
            log_master.join()
            print("ERROR: No BRR data received in Process 2. (exit code 4)")
            exit(4)
        else:

            try:
                print("----------------------------")
                print("01. report_Versions")
                report_Versions(test_results_report, brr_res)
                print("02. report_Bandwidth")
                report_Bandwidth(
                    test_results_report,
                )
                print("03. report_ScanIndex")
                report_ScanIndex(test_results_report, brr_res)
                print("04. report_LookID")
                report_LookID(test_results_report, brr_res)
                print("05. report_Xput")
                report_Xput(test_results_report, brr_res, maxXput, branch, variant, swVersion)

                # print("06. report_TimeSync")
                # report_TimeSync()
                # print("07. report_Overrun")
                # report_Overrun()
                print("08. report_TempInfo")
                report_TempInfo(test_results_report, brr_res)
                # print("09. report_VoltageConditions")
                # report_VoltageConditions()
                print("10. report_RddDistribution")
                report_RddDistribution(test_results_report, brr_res)

                print("11. report_DataCoherency")
                report_DataCoherency(test_results_report, brr_res)
                print("12. report_AfDistribution")
                report_AfDistribution(test_results_report, brr_res)
                # #print("report_AfPathHistogram")
                # #report_AfPathHistogram()

                print("15. report_SystemStatusInfo")
                report_SystemStatusInfo(test_results_report, brr_res)
                # #print("report_CDC")
                # #report_CDC_v2()
                # print("16. report_CDC_Statistics")
                # report_CDC_Statistics()
                # print("17. reoprt_ExceptionCounters")
                # reoprt_ExceptionCounters()
                print("----------------------------")
                root.info("Complete")
                # Finally, close/kill the logging process
                logq.put_nowait(None)
                log_master.join()

            except Exception as e:
                print("ERROR: Issues with reporting. (exit code 3)")
                print(e)
                logq.put_nowait(None)
                log_master.join()
                exit(3)

    # Test Results
    CleanUp()

    CreateHTMLReport(test_results_report, outputHtml, variant)

    print("\nTest Complete")
    powerSupply.output.off()
