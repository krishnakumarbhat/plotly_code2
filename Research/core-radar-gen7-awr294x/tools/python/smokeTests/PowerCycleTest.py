r"""
This is the power cycle that is run as part of the jenkins smoke test.

This script needs a programmable power supply, media gateway and radar to test.

The following are the required inputs:
[-w wiresharklua]: Path to the wireshark file generated,
        for example: bazel-bin\\outputs\\srr7p\\streamdefs\\wiresharkDissector.lua

The following are optional inputs but will likely be needed:
[-v Platform Variant]: srr7p, srr7hd, flr7, defaults to srr7p
[-p Power Cyle Count]: Number of power cycles to run, defaults to 1
[-c COM Port for Power Supply]: Power supply USB COM port, defaults to COM3
[-e Ethernet interface]: Ethernet port, defaults to "Ethernet 3"
[-x Test Comments]: Comments to add the the html report
[-o outputHtml]: Output html path name, default is a name with variant and timestamp
[-r swVersion]: sw version to add to comments.
"""
# !/usr/bin/python3
import logging
import logging.handlers
import time
import argparse
import datetime
from multiprocessing import Process, Queue, Pipe, SimpleQueue
from os import path, walk, system, getcwd, remove, getenv
import shelve
import serial
import pyshark
from threading import Thread, Event
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


# OPTIONS

ADD_PLOTS = True

# local directories to save reports and test data
TEST_DATA_PATH = getcwd() + r"\\testData\\"
REPORT_PATH = getcwd() + r"\\reports\\"
# User app data path for wireshare plugins to store the lua script
WIRESHARK_PLUGIN_DIR = path.join(str(getenv("APPDATA")) + "\\Wireshark\\plugins\\")

# checking if the directory TEST_DATA_PATH exist or not.
if not os.path.isdir(TEST_DATA_PATH):
    # if the TEST_DATA_PATH directory is not present then create it.
    os.makedirs(TEST_DATA_PATH)

DEFAULT_KORAD_COM = "COM3"
DEFAULT_VARIANT = "SRR7p"
DEFAULT_ETH_COM = "Ethernet 3"
XCP_POWER_UP_DELAY = 25
DEFAULT_COMMENTS = "Average Current after 25s & Normal power up current"
# Current Threshold in Amps
CURRENT_THRESHOLD = 0.300

# Shelf file to store recorded Ethernet data
BRR_SHELF = TEST_DATA_PATH + "brr_shelf"

# Global space
test_results_report = None
root = None
brr_exit_event = Event()


def Average(lst):
    """Return the average value of the given list."""
    return sum(lst) / len(lst)


def MaxCheck(arr):
    """Return the max value of a list if it has values."""
    if len(arr) > 0:
        return max(arr)
    else:
        return -1


def master_logger(q):
    """Logger to record data."""
    # init master logger
    master = logging.getLogger("tester")
    master.setLevel(logging.INFO)
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


def saveLuaScript(luaScript):
    """Save the lua script to the correct wireshark directory."""
    if luaScript is not None:
        luaScript = getcwd() + "\\" + luaScript.replace(r"/", "\\")
        print(luaScript)
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
        print("Lua scrit is empty!")


def readLuaScript():
    """Read the lua script from the wireshark directory."""
    for _root, _dirs, files in walk(WIRESHARK_PLUGIN_DIR):
        for file in files:
            if file.endswith(".lua"):
                # print("Wireshark Dissector: " + file)
                return file
                break


def power_cycle_test(pwrCycles, comPort):
    """Run the power cycle test for the given number of power cycles."""
    print("Start of Power Cycle Tests (%d)..." % pwrCycles)
    root.info("Start of Power Cycle Tests (%d)", pwrCycles)
    powerSupply = KoradSerial(comPort, False)

    currentAvg = []
    # Turn it off and ensure correct voltage
    chn = powerSupply.channels[0]
    powerSupply.output.off()
    time.sleep(3)

    # Set voltage and current settings
    chn.voltage = 12.50
    chn.current = 3.00

    # Power On then wait this time
    delay_time = XCP_POWER_UP_DELAY

    # Number of sample to read for each power cycle
    sample_cnt = 50
    # sample rate
    sample_time = 0.1

    cyclesCompleted = 0
    for i in range(pwrCycles):
        root.info("Start of Power Cycle Test  (%d/%d)", i + 1, pwrCycles)
        powerSupply.output.on()
        currents = []

        time.sleep(delay_time)
        print("Sampling current from power supply...")
        for _j in range(0, sample_cnt):
            current = chn.output_current
            if current is not None:
                currents.append(current)
            time.sleep(sample_time)

        avg = Average(currents)
        currentAvg.append(avg)
        root.info("Test %d: Average Current = %0.2f" % (i + 1, avg))
        test_results_report.is_greater_than(
            "Test %d: Average Current" % (i + 1), CURRENT_THRESHOLD, round(avg, 3), "A"
        )
        powerSupply.output.off()
        time.sleep(2.0)
        print(
            "Test %d of %d, Time: %0.2f secs, Avg Current: %0.3f A"
            % (i + 1, pwrCycles, delay_time + (sample_cnt * sample_time), avg)
        )

        cyclesCompleted += 1

    powerSupply.close_port()

    if ADD_PLOTS:
        test_results_report.add_image(
            currentAvg,
            TEST_DATA_PATH + "avgCurrentReadings.png",
            "Average Current after %d s" % (XCP_POWER_UP_DELAY + 5),
            "Power Cycles",
            "Current (A)",
        )

    print(
        "Power Cycle Test Finished: Completed "
        + str(cyclesCompleted)
        + "/"
        + str(pwrCycles)
        + " cycles"
    )


def power_up_test(comPort):
    """Run the power up test."""
    root.info("Start of Power Up Test")
    print("Running Power Up Test...")
    powerSupply = KoradSerial(comPort, False)
    currentReadings = []

    # Turn it off and ensure correct voltage
    chn = powerSupply.channels[0]
    powerSupply.output.off()
    time.sleep(2)

    # Set voltage and current settings
    chn.voltage = 12.50
    chn.current = 3.00

    # time after power up to sample
    duration = 20
    # sample rate
    sample_time = 0.05

    powerSupply.output.on()

    totalSamples = int(duration / sample_time)

    for j in range(0, totalSamples):
        current = chn.output_current
        print("Power Up Test Progress %d/%d... " % (j + 1, totalSamples))
        if current is not None:
            currentReadings.append(current)
        time.sleep(sample_time)

    powerSupply.output.off()
    powerSupply.close_port()
    test_results_report.add_image(
        currentReadings,
        TEST_DATA_PATH + "powerUpCurrentReadings.png",
        "Power Up Current (0s to %d s)" % (duration),
        "Samples (%0.0f ms sample rate)" % (sample_time * 1000),
        "Current (A)",
    )
    print("End of Power Up Test")


def brr_listener_master_msg(conn):
    """Stop the brr process."""
    if conn.recv() == "stop":
        # print('msg received brr stop')
        brr_exit_event.clear()


def brr_listener(q, eq, conn, eth):
    """Process to read BRR data."""
    h = logging.handlers.QueueHandler(q)
    brr = logging.getLogger()
    brr.addHandler(h)
    brr.setLevel(logging.INFO)
    # firstTime = True
    msg_cntr = 0
    streamScanIndex = {}
    streamPackets = {}
    brr_signals_data = {}
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
    }
    brr_signals_data["mmic"] = {"mmic_err_counter": [], "mmic_err_code": []}
    brr_signals_data["xput"] = {"RFFT_xput_inst": [], "RDD_xput_inst": [], "AF_xput_inst": []}
    brr_signals_data["usc_load_status"] = []
    brr_signals_data["smc_load_status"] = []
    brr_signals_data["radar_cals_load"] = []

    lua = WIRESHARK_PLUGIN_DIR + readLuaScript()
    param = ["-X", "lua_script:{0}".format(lua)]
    cap = pyshark.LiveCapture(interface=eth, custom_parameters=param)
    master_msg = Thread(target=brr_listener_master_msg, args=(conn,))
    master_msg.start()
    brr_exit_event.set()

    brr.info("Starting BRR listener...")
    for packet in cap.sniff_continuously():
        if packet.highest_layer == "APTIVUDP":
            msg_cntr += 1
            streamNumber = 0
            # streamChunkIdx = 0
            if hasattr(packet["APTIVUDP"], "streamNumber"):
                streamNumber = int(packet["APTIVUDP"].streamNumber)
            # if (hasattr(packet["APTIVUDP"], 'streamChunkIdx')): streamChunkIdx = int(packet["APTIVUDP"].streamChunkIdx)
            if streamNumber not in streamScanIndex:
                streamScanIndex[streamNumber] = []
                streamPackets[streamNumber] = 0

            if streamNumber == 1:  # and streamChunkIdx == 36):
                if hasattr(packet["APTIVUDP"], "s1_scanindex"):
                    streamPackets[streamNumber] += 1
                    streamScanIndex[streamNumber].append(int(packet["APTIVUDP"].s1_scanindex))
            elif streamNumber == 2:
                streamPackets[streamNumber] += 1
                if hasattr(packet["APTIVUDP"], "s2_scanindex"):
                    streamScanIndex[streamNumber].append(int(packet["APTIVUDP"].s2_scanindex))

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

            elif streamNumber == 3:
                streamPackets[streamNumber] += 1
                if hasattr(packet["APTIVUDP"], "s3_scanindex"):
                    streamScanIndex[streamNumber].append(int(packet["APTIVUDP"].s3_scanindex))

                if hasattr(packet["APTIVUDP"], "s3_usc_load_status"):
                    brr_signals_data["usc_load_status"].append(
                        int(packet["APTIVUDP"].s3_usc_load_status)
                    )
                if hasattr(packet["APTIVUDP"], "s3_smc_load_status"):
                    brr_signals_data["smc_load_status"].append(
                        int(packet["APTIVUDP"].s3_smc_load_status)
                    )
                if hasattr(packet["APTIVUDP"], "s3_radar_cals_load"):
                    brr_signals_data["radar_cals_load"].append(
                        int(packet["APTIVUDP"].s3_radar_cals_load)
                    )

                if hasattr(packet["APTIVUDP"], "s3_pbl_version_maj"):
                    brr_signals_data["versioning"]["pbl_version_maj"] = int(
                        packet["APTIVUDP"].s3_pbl_version_maj
                    )
                    brr_signals_data["versioning"]["pbl_version_min"] = int(
                        packet["APTIVUDP"].s3_pbl_version_min
                    )
                    brr_signals_data["versioning"]["pbl_version_patch"] = int(
                        packet["APTIVUDP"].s3_pbl_version_patch
                    )
                    brr_signals_data["versioning"]["software_version_maj"] = int(
                        packet["APTIVUDP"].s3_software_version_maj
                    )
                    brr_signals_data["versioning"]["software_version_min"] = int(
                        packet["APTIVUDP"].s3_software_version_min
                    )
                    brr_signals_data["versioning"]["software_version_patch"] = int(
                        packet["APTIVUDP"].s3_software_version_patch
                    )

                    brr_signals_data["versioning"]["smcVersion1"] = packet[
                        "APTIVUDP"
                    ].s3_smc_version_platform
                    brr_signals_data["versioning"]["smcVersion2"] = packet[
                        "APTIVUDP"
                    ].s3_smc_version_major
                    brr_signals_data["versioning"]["smcVersion3"] = packet[
                        "APTIVUDP"
                    ].s3_smc_version_minor
                    brr_signals_data["versioning"]["smcVersion4"] = packet[
                        "APTIVUDP"
                    ].s3_smc_version_patch

                    brr_signals_data["versioning"]["uscVersion1"] = packet[
                        "APTIVUDP"
                    ].s3_usc_version_platform
                    brr_signals_data["versioning"]["uscVersion2"] = packet[
                        "APTIVUDP"
                    ].s3_usc_version_major
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
                        brr_signals_data["temps"]["c66_temp"].append(
                            int(packet["APTIVUDP"].s3_dsp_temp)
                        )
                        brr_signals_data["temps"]["radar_temp"].append(
                            int(packet["APTIVUDP"].s3_hwa_temp)
                        )
                        brr_signals_data["temps"]["tmu_temp"].append(
                            int(packet["APTIVUDP"].s3_hsm_temp)
                        )
                    brr_signals_data["mmic"]["mmic_err_counter"].append(
                        int(packet["APTIVUDP"].s3_mmic_err_counter)
                    )
                    brr_signals_data["mmic"]["mmic_err_code"].append(
                        int(packet["APTIVUDP"].s3_mmic_err_code)
                    )

            elif streamNumber == 4:
                streamPackets[streamNumber] += 1
                if hasattr(packet["APTIVUDP"], "s4_scanindex"):
                    streamScanIndex[streamNumber].append(int(packet["APTIVUDP"].s4_scanindex))
        if brr_exit_event.is_set() is False:
            break
    cap.close()

    brr_signals_data["scanIndexes"] = streamScanIndex
    brr_signals_data["packets"] = streamPackets
    brr_signals_data["UDPMsgCntr"] = msg_cntr

    with shelve.open(BRR_SHELF) as brr_res:
        for key in brr_signals_data:
            brr_res[key] = brr_signals_data[key]

    master_msg.join()


def brr_listener_indexcheck(q, eq, conn, eth, variant):
    """Process BRR data."""
    h = logging.handlers.QueueHandler(q)
    brr = logging.getLogger()
    brr.addHandler(h)
    brr.setLevel(logging.INFO)
    brr_signals_data = {}
    msg_cntr = 0
    packets = {"1": 0, "2": 0, "3": 0, "4": 0, "5": 0, "6": 0, "8": 0, "14": 0}
    streamIndexes = {1: [], 2: [], 3: [], 4: [], 8: []}
    streamIdxSkips = {"1": 0, "2": 0, "3": 0, "4": 0, "5": 0, "6": 0, "8": 0, "14": 0}
    streamIdxRepeats = {"1": 0, "2": 0, "3": 0, "4": 0, "5": 0, "6": 0, "8": 0, "14": 0}
    prevStreamIdx = {
        "1": None,
        "2": None,
        "3": None,
        "4": None,
        "5": None,
        "6": None,
        "8": None,
        "14": None,
    }
    lookID = []
    timeStamps = {"timeStampSec": [], "timeStampNs": []}
    targetCnts = []
    numFpDet = []
    numSpDet = []
    # firstTime = True
    udp_timeStamps = {}
    # streamData = {}
    # create thred within process for checking master msgs
    lua = WIRESHARK_PLUGIN_DIR + readLuaScript()
    param = ["-X", "lua_script:{0}".format(lua)]
    cap = pyshark.LiveCapture(
        interface=eth, bpf_filter="udp", only_summaries=True, custom_parameters=param
    )
    master_msg = Thread(target=brr_listener_master_msg, args=(conn,))
    master_msg.start()
    brr_exit_event.set()

    brr.info("Starting BRR2 listener...")
    for packet in cap.sniff_continuously():

        if "Aptiv UDP Stream 1" in packet.summary_line:
            if 1 not in udp_timeStamps:
                udp_timeStamps[1] = []
            if "ScanIndex:" in packet.summary_line:
                packets["1"] += 1
                udp_timeStamps[1].append(float(packet.summary_line.split()[1]) * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(", ")[0].strip()
                index = int(index)
                streamIndexes[1].append(index)
                if prevStreamIdx["1"] is not None:
                    if index != (prevStreamIdx["1"] + 1):
                        if not (
                            (index == 0) and (prevStreamIdx["1"] == 65535)
                        ):  # CAN signal overflow
                            streamIdxSkips["1"] += 1
                    if index == prevStreamIdx["1"]:
                        streamIdxRepeats["1"] += 1
                prevStreamIdx["1"] = index

            if "TargetCnt:" in packet.summary_line:
                targetCnt = int(
                    packet.summary_line.split("TargetCnt:")[1].strip().split(", ")[0].strip()
                )
                targetCnts.append(targetCnt)

        elif "Aptiv UDP Stream 2" in packet.summary_line:
            if 2 not in udp_timeStamps:
                udp_timeStamps[2] = []
            if "ScanIndex:" in packet.summary_line:
                packets["2"] += 1
                udp_timeStamps[2].append(float(packet.summary_line.split()[1]) * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(", ")[0].strip()
                index = int(index)
                streamIndexes[2].append(index)

                if prevStreamIdx["2"] is not None:
                    if index != (prevStreamIdx["2"] + 1):
                        if not ((index == 0) and (prevStreamIdx["2"] == 65535)):
                            streamIdxSkips["2"] += 1
                    if index == prevStreamIdx["2"]:
                        streamIdxRepeats["2"] += 1
                prevStreamIdx["2"] = index

            if "TSsec:" in packet.summary_line:
                tsSec = packet.summary_line.split("TSsec:")[1].strip().split(", ")[0].strip()
                timeStamps["timeStampSec"].append(int(tsSec))
            if "TSnsec:" in packet.summary_line:
                stNCec = packet.summary_line.split("TSnsec:")[1].strip().split(", ")[0].strip()
                timeStamps["timeStampNs"].append(int(stNCec))
            if "LookId:" in packet.summary_line:
                look = int(packet.summary_line.split("LookId:")[1].strip().split(", ")[0].strip())
                lookID.append(look)
        elif "Aptiv UDP Stream 3" in packet.summary_line:
            if 3 not in udp_timeStamps:
                udp_timeStamps[3] = []
            if "ScanIndex:" in packet.summary_line:
                packets["3"] += 1
                udp_timeStamps[3].append(float(packet.summary_line.split()[1]) * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(", ")[0].strip()
                index = int(index)
                streamIndexes[3].append(index)
                if prevStreamIdx["3"] is not None:
                    if index != (prevStreamIdx["3"] + 1):
                        if not ((index == 0) and (prevStreamIdx["3"] == 65535)):
                            streamIdxSkips["3"] += 1
                    if index == prevStreamIdx["3"]:
                        streamIdxRepeats["3"] += 1
                prevStreamIdx["3"] = index

        elif "Aptiv UDP Stream 4" in packet.summary_line:
            if 4 not in udp_timeStamps:
                udp_timeStamps[4] = []
            if "ScanIndex:" in packet.summary_line:
                packets["4"] += 1
                udp_timeStamps[4].append(float(packet.summary_line.split()[1]) * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(", ")[0].strip()
                index = int(index)
                streamIndexes[4].append(index)
                if prevStreamIdx["4"] is not None:
                    if index != (prevStreamIdx["4"] + 1):
                        if not ((index == 0) and (prevStreamIdx["4"] == 65535)):
                            streamIdxSkips["4"] += 1
                    if index == prevStreamIdx["4"]:
                        streamIdxRepeats["4"] += 1
                prevStreamIdx["4"] = index
            if "num_fp_detections:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("num_fp_detections:")[1]
                    .strip()
                    .split(", ")[0]
                    .strip()
                )
                numFpDet.append(det)
            if "num_sp_detections:" in packet.summary_line:
                det = int(
                    packet.summary_line.split("num_sp_detections:")[1]
                    .strip()
                    .split(", ")[0]
                    .strip()
                )
                numSpDet.append(det)
        elif "Aptiv UDP Stream 8" in packet.summary_line:
            if 8 not in udp_timeStamps:
                udp_timeStamps[8] = []
            if "ScanIndex:" in packet.summary_line:
                packets["8"] += 1
                udp_timeStamps[8].append(float(packet.summary_line.split()[1]) * 1000)
                index = packet.summary_line.split("ScanIndex:")[1].strip().split(", ")[0].strip()
                index = int(index)
                streamIndexes[8].append(index)
                if prevStreamIdx["8"] is not None:
                    if index != (prevStreamIdx["8"] + 1):
                        if not ((index == 0) and (prevStreamIdx["8"] == 65535)):
                            streamIdxSkips["8"] += 1
                    if index == prevStreamIdx["8"]:
                        streamIdxRepeats["8"] += 1
                prevStreamIdx["8"] = index

        msg_cntr += 1
        # brr.info('brr msg received: %d', msg_cntr)
        if brr_exit_event.is_set() is False:
            break

    cap.close()
    brr.info("streamIdxSkips:")
    brr.info(streamIdxSkips)

    brr.info("Total UDP2 frames received: %d", msg_cntr)
    brr_signals_data["msg_cntr"] = msg_cntr
    brr_signals_data["packets"] = packets
    brr_signals_data["lookID"] = lookID
    # brr_signals_data['streamIndexes'] = streamData
    brr_signals_data["streamIndexes"] = streamIndexes
    brr_signals_data["streamIdxRepeats"] = streamIdxRepeats
    brr_signals_data["streamIdxSkips"] = streamIdxSkips
    # brr_signals_data['timeStamps'] = timeStamps
    brr_signals_data["timeStamps"] = udp_timeStamps

    brr_signals_data["targetCnts"] = targetCnts
    brr_signals_data["numFpDetections"] = numFpDet
    brr_signals_data["numSpDetections"] = numSpDet

    brr_shelf = shelve.open(BRR_SHELF)
    for key in brr_signals_data:
        brr_shelf[key] = brr_signals_data[key]
    brr_shelf.close()
    master_msg.join()


def reportUDP():
    """Report the Versions."""
    brr_res = shelve.open(BRR_SHELF)

    if brr_res["versioning"]:

        udp_pbl_version = "{0}.{1}.{2}".format(
            brr_res["versioning"]["pbl_version_maj"],
            brr_res["versioning"]["pbl_version_min"],
            brr_res["versioning"]["pbl_version_patch"],
        )
        udp_app_version = "{0}.{1}.{2}".format(
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

        test_results_report.add_part_number(
            "APP: {0}, PBL: {1}, SMC: {2}, USC: {3}, Platform: {4}".format(
                udp_app_version, udp_pbl_version, udp_smc_version, udp_usc_version, udp_plat_var
            )
        )

    # if brr_res['scanIndexes']:
    # for stream in sorted(brr_res['scanIndexes']):
    #    if (ADD_PLOTS):  test_results_report.add_image(brr_res['scanIndexes'][stream], TEST_DATA_PATH + "udpStream"+str(stream)+"ScanIndex.png", "Stream " +str(stream)+ " Scan Index", "Samples", "Scan Index")

    for stream in sorted(brr_res["streamIndexes"]):
        if ADD_PLOTS:
            test_results_report.add_image(
                brr_res["streamIndexes"][stream],
                TEST_DATA_PATH + "udpStream" + str(stream) + "StreamIndex.png",
                "Stream " + str(stream) + " Scan Index",
                "Samples",
                "Scan Index",
            )

        # print("Scan Index: %d"%(stream))
        # print(brr_res['scanIndexes'][stream])"

    # test_results_report.is_less_than("UDP - RDD Xput Avg", 90, round(mean(brr_res['xput']['RDD_xput_inst']),2),'%')
    # test_results_report.is_less_than("UDP - RDD Xput Max", 90, round((brr_res['xput']['RDD_xput_peak']),2),'%')
    if ADD_PLOTS:
        test_results_report.add_image(
            brr_res["xput"]["RDD_xput_inst"],
            TEST_DATA_PATH + "udprddXput.png",
            "UDP - RDD xput",
            "Samples",
            "xput (%)",
        )

        # test_results_report.is_less_than("UDP - AF xput Avg", 90, round(mean(brr_res['xput']['af_xput_inst']),2),'%')
        # test_results_report.is_less_than("UDP - AF xput Max", 90, round((brr_res['xput']['af_xput_peak']),2),'%')

        test_results_report.add_image(
            brr_res["xput"]["AF_xput_inst"],
            TEST_DATA_PATH + "udpafXput.png",
            "UDP - AF xput",
            "Samples",
            "xput (%)",
        )
        test_results_report.add_image(
            brr_res["xput"]["RFFT_xput_inst"],
            TEST_DATA_PATH + "udprfftXput.png",
            "UDP - RFFT xput",
            "Samples",
            "xput (%)",
        )

    brr_temps = []
    if brr_res["temps"]:
        brr_temps = brr_res["temps"]
        # test_results_report.is_almost_equal("UDP - C66 Temp",   47.5,87.5, round(mean(brr_temps['c66_temp']),3),'C')
        # if (ADD_PLOTS): test_results_report.add_image(brr_temps['c66_temp'], TEST_DATA_PATH + "udpc66_temp.png", "UDP - C66 Temp", "Samples", "Temp (C)")
        # test_results_report.is_almost_equal("UDP - Radar Temp",   47.5,87.5, round(mean(brr_temps['radar_temp']),3),'C')
        # if (ADD_PLOTS): test_results_report.add_image(brr_temps['radar_temp'], TEST_DATA_PATH + "udpRadartemp.png", "UDP - Radar Temp", "Samples", "Temp (C)")
        # test_results_report.is_almost_equal("UDP - Tmu Temp",   47.5,87.5, round(mean(brr_temps['tmu_temp']),3),'C')
        # if (ADD_PLOTS): test_results_report.add_image(brr_temps['tmu_temp'], TEST_DATA_PATH + "udptmutemp.png", "UDP - Tmu Temp", "Samples", "Temp (C)")

        if ADD_PLOTS:
            test_results_report.add_image_compare4(
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

            test_results_report.add_image_compare4(
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

    if brr_res["mmic"]:
        test_results_report.is_equal(
            "UDP MMIC Error Counter Max", 0, MaxCheck(brr_res["mmic"]["mmic_err_counter"])
        )
        test_results_report.is_equal(
            "UDP MMIC Error Code Max", 0, MaxCheck(brr_res["mmic"]["mmic_err_code"])
        )

        if ADD_PLOTS:
            test_results_report.add_image(
                brr_res["mmic"]["mmic_err_counter"],
                TEST_DATA_PATH + "mmicErrorCounter.png",
                "MMIC Error Counter",
                "Samples",
                "Error Counter",
            )
            test_results_report.add_image(
                brr_res["mmic"]["mmic_err_code"],
                TEST_DATA_PATH + "mmicErrorCode.png",
                "MMIC Error Code",
                "Samples",
                "Error Code",
            )

    test_results_report.is_equal(
        "UDP USC Load Status Max", 0, MaxCheck(brr_res["usc_load_status"])
    )
    test_results_report.is_equal(
        "UDP SMC Load Status Max", 0, MaxCheck(brr_res["smc_load_status"])
    )
    # test_results_report.is_equal('UDP Radar Cals Load Status Max',    0, MaxCheck(brr_res['radar_cals_load']))

    if ADD_PLOTS:
        test_results_report.add_image(
            brr_res["usc_load_status"],
            TEST_DATA_PATH + "uscLoadStatus.png",
            "USC Load Status",
            "Samples",
            "Status",
        )
        test_results_report.add_image(
            brr_res["smc_load_status"],
            TEST_DATA_PATH + "smcLoadStatus.png",
            "SMC Load Status",
            "Samples",
            "Status",
        )
    # if (ADD_PLOTS): test_results_report.add_image(brr_res['radar_cals_load'], TEST_DATA_PATH + "radarCalsLoadStatus.png", "Radar Cals Load Status", "Samples", "Status")

    brr_res.close()


def CleanUp():
    """Delete the brr and xcp shelf files."""
    try:
        if path.exists(BRR_SHELF + ".dat"):
            remove(BRR_SHELF + ".dat")
        if path.exists(BRR_SHELF + ".bak"):
            remove(BRR_SHELF + ".bak")
        if path.exists(BRR_SHELF + ".dir"):
            remove(BRR_SHELF + ".dir")
    except Exception as e:
        print("Could not delete shelf files: %s", e)


def CreateHTMLReport(trr, outputHtml, variant):
    """
    Create the html file and jenkins files.
    """
    now = datetime.datetime.now()

    test_results_report.timer.stop()

    if outputHtml:
        reportFileName = outputHtml
    else:
        reportFileName = "{}_Power_Cycle_Test_{}_cycles_{}_{}_{}_{}_{}_{}".format(
            variant, pwrCycles, now.year, now.month, now.day, now.hour, now.minute, now.second
        )

    # checking if the directory REPORT_PATH exist or not.
    if not os.path.isdir(REPORT_PATH):
        # if the REPORT_PATH directory is not present then create it.
        os.makedirs(REPORT_PATH)

    test_results_report.generate_html_report(REPORT_PATH, reportFileName, open_report=False)

    print("Copying xml and html to base directory for Jenkins to be able to read them.")
    system(
        "copy {} {}".format(
            REPORT_PATH + reportFileName + "_jenkins.xml", getcwd() + "\\..\\..\\.."
        )
    )
    system("copy {} {}".format(REPORT_PATH + reportFileName + ".html", getcwd() + "\\..\\..\\.."))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="APTIV Gen7 Radar Power Cycle Test", epilog="APTIV Ltd, copyright 2022"
    )

    parser.add_argument(
        "-v",
        "--variant",
        metavar="Platform Variant",
        type=str,
        required=False,
        default=DEFAULT_VARIANT,
    )
    parser.add_argument(
        "-p", "--powercycles", metavar="Power Cyle Count", type=int, required=False, default=2
    )
    parser.add_argument(
        "-c",
        "--comport",
        metavar="COM Port for Power Supply",
        type=str,
        required=False,
        default=DEFAULT_KORAD_COM,
    )
    parser.add_argument(
        "-e",
        "--ethnic",
        metavar="Ethernet interface",
        type=str,
        required=False,
        default=DEFAULT_ETH_COM,
        help="Ethernet interface name as listed",
    )
    parser.add_argument(
        "-x", "--comment", metavar="Test Comments", type=str, default="", required=False
    )
    parser.add_argument(
        "-w", "--wiresharklua", metavar="wiresharklua", type=str, default=None, required=True
    )
    parser.add_argument(
        "-o", "--outputHtml", metavar="outputHtml", type=str, default=None, required=False
    )
    parser.add_argument(
        "-r", "--swVersion", metavar="swVersion", type=str, default="", required=False
    )
    args = parser.parse_args()

    variant = args.variant
    comPort = args.comport
    pwrCycles = args.powercycles
    eth_nic = args.ethnic
    comment = args.comment
    luaScript = args.wiresharklua
    outputHtml = args.outputHtml

    saveLuaScript(luaScript)

    # get main logger queue
    logq = Queue(-1)

    # logger configuration
    h = logging.handlers.QueueHandler(logq)
    root = logging.getLogger()
    root.addHandler(h)
    root.setLevel(logging.INFO)
    log_master = Process(name="Tester01", target=master_logger, args=(logq,))
    log_master.start()

    # Check that the port is available
    try:
        ser = serial.Serial(comPort)
        ser.close()
    except Exception as e:
        print(str(e))
        print(
            "\nERROR: Port {0} is not available. Check connection or port name.\n".format(comPort)
        )
        root.error('Port "{0}" is not available. Check connection or port name.'.format(comPort))
        logq.put_nowait(None)
        log_master.join()
        exit()

    # --- Main testing loop STARTS here
    test_results_report = Tester(
        "Power Cycle Test",
        "Power Cycle Test Results for {0} build".format(variant.upper()),
        program=variant.upper(),
        powerCycles=pwrCycles,
        comments=DEFAULT_COMMENTS + "; " + comment,
    )
    test_results_report.timer.start()
    root.info("Starting test")

    errq = SimpleQueue()
    # connectors for the spawned processes
    brr_m_conn, brr_s_conn = Pipe()
    brr_proc = Process(
        name="BRR_listener", target=brr_listener, args=(logq, errq, brr_s_conn, eth_nic)
    )
    brr_proc.start()
    brr_m_conn2, brr_s_conn2 = Pipe()
    brr_proc2 = Process(
        name="BRR_listener_IndexCheck",
        target=brr_listener_indexcheck,
        args=(logq, errq, brr_s_conn2, eth_nic, variant),
    )
    brr_proc2.start()

    if pwrCycles > 0:
        power_cycle_test(pwrCycles, comPort)

    brr_m_conn.send("stop")
    brr_m_conn2.send("stop")
    brr_proc.join(timeout=10)
    brr_proc2.join(timeout=1)

    power_up_test(comPort)

    root.info("Finishing test...")

    # Finally, close/kill the logging process

    logq.put_nowait(None)
    log_master.join()

    with shelve.open(BRR_SHELF) as brr_res:
        if "msg_cntr" not in brr_res:
            print("ERROR: No BRR data received.")
            exit(5)

    reportUDP()

    CreateHTMLReport(test_results_report, outputHtml, variant)

    CleanUp()

    print("Test Complete")
