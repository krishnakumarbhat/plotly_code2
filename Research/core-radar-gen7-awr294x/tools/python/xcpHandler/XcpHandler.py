"""XCP Handler.

This module contains the necessary commands to connect and read out data via XCP.
"""
import socket
import json
import time
import sys
from os import path
from pathlib import Path

BASE_PATH = path.dirname(path.abspath(__file__))
sys.path.insert(0, path.abspath(path.join(BASE_PATH, "..", "A2lReader")))

from A2lReader import A2lReader  # noqa: E402

CODED_TRUE = 0xAA
ONE_BYTE = 1
XCP_INIT = 0  # 0 - not init  1- init
CONNECT = [0x08, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]  # Connect
CONNECT_POSITIVE_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x0C, 0x00, 0xFB, 0xFB, 0x00, 0x01, 0x01]

DISCONNECT = [0x08, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]  # Disconnect
DISCONNECT_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x0C, 0x00, 0xFB, 0xFB, 0x00, 0x01, 0x01]


GET_COMM_MODE_INFO = [
    0x08,
    0x00,
    0x00,
    0x00,
    0xFB,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
]  # Get Comm Mode Info
GET_COMM_MODE_INFO_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x01]

GET_STATUS = [0x08, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]  # Get Status
GET_STATUS_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]


# ---- DAQ
CLEAR_DAQ_LIST = [
    0x08,
    0x00,
    0x00,
    0x00,
    0xE3,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
]  # CLEAR_DAQ_LIST
CLEAR_DAQ_LIST_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

SET_DAQ_PTR = [
    0x08,
    0x00,
    0x00,
    0x00,
    0xE2,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
]  # SET_DAQ_PTR
SET_DAQ_PTR_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

WRITE_DAQ_PTR = [0x08, 0x00, 0x00, 0x00, 0xE1, 0xFF, 0x04, 0x00]  # WRITE_DAQ
WRITE_DAQ_PTR_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

SET_DAQ_LIST_MODE = [
    0x08,
    0x00,
    0x00,
    0x00,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
]  # SET_DAQ_LIST_MODE
SET_DAQ_LIST_MODE_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

START_STOP_DAQ_LIST = [
    0x08,
    0x00,
    0x00,
    0x00,
    0xDE,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
]  # START_STOP_DAQ_LIST
START_STOP_DAQ_LIST_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

GET_DAQ_CLOCK = [
    0x08,
    0x00,
    0x00,
    0x00,
    0xDC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
]  # GET_DAQ_CLOCK
GET_DAQ_CLOCK_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

START_STOP_SYNCH = [
    0x08,
    0x00,
    0x00,
    0x00,
    0xDD,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
]  # START_STOP_SYNCH
START_STOP_SYNCH_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]


SET_MTA = [0x08, 0x00, 0x00, 0x00, 0xF6, 0x00, 0x00, 0x00]
DOWNLOAD = [0x08, 0x00, 0x00, 0x00, 0xF0]
UPLOAD = [0x08, 0x00, 0x00, 0x00, 0xF5]
STREAM_VERSION_RES = [0x40, 0x00, 0x00, 0x00, 0xFF, 0x0C]


class XcpConfigDataManager:
    """Structure to read out the config.json file."""

    def __init__(self) -> None:
        """Verify that config.json exits."""
        if not (Path(str(BASE_PATH) + "\\config.json").is_file()):
            print("Config File is missing.")

    def PullConfigData(self):
        """Read values out of the config.json file."""
        with open(str(BASE_PATH) + "\\config.json") as f:
            data = json.load(f)
            ipConfig = data["ipConfig"]
            bufferSize = data["bufferSize"]
            timeout_sec = data["timeout_sec"]
            xcp_buffersize = data["xcp_buffersize"]
        return ipConfig, bufferSize, timeout_sec, xcp_buffersize

    def DumpConfigData(self, ipConfig, bufferSize, timeout_sec, xcp_buffersize):
        """Write values back to the config.json file."""
        configDict = {
            "ipConfig": ipConfig,
            "bufferSize": bufferSize,
            "timeout_sec": timeout_sec,
            "xcp_buffersize": xcp_buffersize,
        }
        with open(str(BASE_PATH) + "\\config.json", "w") as fp:
            json.dump(configDict, fp, indent=4)


class XcpHandler:
    """Class that handles all the XCP commands."""

    def __init__(self, a2lFile) -> None:
        """Initialize the necessary connection and flags to XCP."""
        xcpConfigData = XcpConfigDataManager()

        m_ipConfig, m_bufferSize, m_timeout_sec, m_xcp_bufferSize = xcpConfigData.PullConfigData()

        self.mSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.mSocket.bind((m_ipConfig["Ip_Hil_Replay"], int(m_ipConfig["Port_Hil_Replay"])))
        self.mSocket.settimeout(m_timeout_sec)
        self.mSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.bufferSize = m_bufferSize
        self.ipConfig = m_ipConfig
        self.verbose = True
        self.connected = False
        self.a2lReader = A2lReader(a2lFile)

    def getAddress(self, variable):
        """Get the address from the given variable."""
        addr, size = self.a2lReader.getAddress(variable)
        return addr, size

    def callCommand(self, commandBytes, description=""):
        """Call the given command."""
        print("XCP: Calling command %s" % description)
        status = True
        recv_data = ""
        try:
            self.mSocket.sendto(
                bytes(commandBytes),
                (self.ipConfig["Ip_Sensor"], int(self.ipConfig["Port_Sensor_XCP"])),
            )
        except Exception as e:
            print("XCP: Request send to sensor failed - " + str(e))
            status = False
        if status:
            try:
                recv_data = self.mSocket.recvfrom(self.bufferSize)
            except Exception as e:
                print("XCP: Response from sensor Timeout Error - " + str(e))
                status = False
        # print (recv_data)
        return status, recv_data

    def xcpInit(self) -> bool:
        """Initialize the XCP connection."""
        recv_data = ""
        if self.connected is False:
            try:
                self.mSocket.sendto(
                    bytes(CONNECT),
                    (self.ipConfig["Ip_Sensor"], int(self.ipConfig["Port_Sensor_XCP"])),
                )
            except Exception as e:
                print("XCP: Request send to sensor failed - " + str(e))
                return False
            try:
                recv_data = self.mSocket.recvfrom(self.bufferSize)
            except Exception as e:
                print("XCP: Response from sensor Timeout Error - " + str(e))
                return False
            recv_data = recv_data[0]

            if (recv_data[4] == CONNECT_POSITIVE_RES[4]) and (
                recv_data[5] == CONNECT_POSITIVE_RES[5]
            ):
                print("XCP: Init successfully\n")
                self.connected = True
                return True
            else:
                return False
        else:
            print("XCP: Init already done\n")
            return True

    def appendAddress(self, cmdBytes, address) -> str:
        """Populate the command bytes with the given address."""
        address = str("%X" % address).zfill(8)

        byteStr = cmdBytes.copy()
        byteStr.append(int(address[6:8], 16))
        byteStr.append(int(address[4:6], 16))
        byteStr.append(int(address[2:4], 16))
        byteStr.append(int(address[0:2], 16))
        return byteStr

    def setupDAQ(self, address) -> bool:
        """Call the given commands to setup the Daq."""
        # print ("setup daq with address 0x%X"%address)
        self.callCommand(CLEAR_DAQ_LIST, "CLEAR_DAQ_LIST")
        self.callCommand(SET_DAQ_PTR, "SET_DAQ_PTR")

        writeDaqBytes = self.appendAddress(WRITE_DAQ_PTR, address)

        self.callCommand(writeDaqBytes, "WRITE_DAQ_PTR")
        self.callCommand(SET_DAQ_LIST_MODE, "SET_DAQ_LIST_MODE")
        self.callCommand(START_STOP_DAQ_LIST, "START_STOP_DAQ_LIST")
        self.callCommand(GET_DAQ_CLOCK, "GET_DAQ_CLOCK")
        self.callCommand(START_STOP_SYNCH, "START_STOP_SYNCH")
        return True

    def stopDaq(self) -> bool:
        """Stop the Daq."""
        self.callCommand(START_STOP_SYNCH, "START_STOP_SYNCH")
        return True

    def disconnect(self) -> bool:
        """Disconnect."""
        self.stopDaq()
        self.callCommand(DISCONNECT, "DISCONNECT")
        self.connected = False
        return True

    def getResponseBytes(self, recvBytes, recvlen) -> str:
        """Get the received bytes."""
        val = ""
        if recvlen == 1:
            val = "%02X" % recvBytes[0][5]
        elif recvlen == 2:
            val = "%02X%02X" % (recvBytes[0][6], recvBytes[0][5])
        elif recvlen == 4:
            val = "%02X%02X%02X%02X" % (
                recvBytes[0][8],
                recvBytes[0][7],
                recvBytes[0][6],
                recvBytes[0][5],
            )

        else:
            print("Unknown size")
            return ""
        print(val)
        return val

    def writeCal(self, address, size, val) -> bool:
        """Write calibration value."""
        downloadBytes = DOWNLOAD
        downloadBytes.append(size)
        downloadBytes.append(val)

        # Download
        try:
            self.mSocket.sendto(
                bytes(downloadBytes),
                (self.ipConfig["Ip_Sensor"], int(self.ipConfig["Port_Sensor_XCP"])),
            )
        except Exception as e:
            print(e)
            return False
        try:
            self.mSocket.recvfrom(self.bufferSize)
            print("XCP: Writing Calibration: 0x%s = 0x%X" % (address, val))
            time.sleep(0.05)
            return True
        except Exception as e:
            print("XCP: Response Timout Error (writeCal) - " + str(e))
            return False

    def readDaq(self, size, scaler=1) -> float:
        """Read Daq."""
        try:
            recv_data = self.mSocket.recvfrom(self.bufferSize)

            val = 0
            if size == 1:
                val = "%02X" % recv_data[0][5]
            elif size == 2:
                val = "%02X%02X" % (recv_data[0][6], recv_data[0][5])
            elif size == 4:
                val = "%02X%02X%02X%02X" % (
                    recv_data[0][9],
                    recv_data[0][8],
                    recv_data[0][7],
                    recv_data[0][6],
                )
            else:
                print("Unknown size")
                return False

            # print ("Reading Calibration: 0x%X = 0x%s"%(addrWOffset,val))
            # print ("Reading Calibration: 0x%s = 0x%X"%(address,recv_data[0][5]))

            return float("%3.3f" % (int(val, 16) / scaler))

        except Exception as e:
            print("XCP: Response Timout Error (readDaq) - " + str(e))
            return False

    def readCal(self, address, size, offset=0) -> bool:
        """Read clibration variable."""
        recv_data = ""
        addrWOffset = address + offset
        # print ("ADDRESS 0x%X"%addrWOffset)
        # self._setMta(addrWOffset)

        uploadBytes = UPLOAD
        uploadBytes.append(size)

        # Download
        try:
            self.mSocket.sendto(
                bytes(uploadBytes),
                (self.ipConfig["Ip_Sensor"], int(self.ipConfig["Port_Sensor_XCP"])),
            )
        except Exception as e:
            print(e)
            return False
        try:
            recv_data = self.mSocket.recvfrom(self.bufferSize)
            val = 0
            if size == 1:
                val = "%02X" % recv_data[0][5]
            elif size == 2:
                val = "%02X%02X" % (recv_data[0][6], recv_data[0][5])
            elif size == 4:
                val = "%02X%02X%02X%02X" % (
                    recv_data[0][8],
                    recv_data[0][7],
                    recv_data[0][6],
                    recv_data[0][5],
                )

            else:
                print("Unknown size")
                return False

            print("XCP: Reading Calibration: 0x%X = 0x%s" % (addrWOffset, val))

            time.sleep(0.05)
            return True

        except Exception as e:
            print("XCP: Response Timout Error (readCal) - " + str(e))
            return False
