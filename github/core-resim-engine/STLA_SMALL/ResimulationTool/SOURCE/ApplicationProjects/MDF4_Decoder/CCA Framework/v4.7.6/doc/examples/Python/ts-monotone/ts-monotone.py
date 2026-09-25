# Example file for the CCA-Framework-MDF: Demonstrates how to scan for sessions and checking if the sessions have monotone timestamps
#
# @copyright            Copyright (c) 2023-2025 ViGEM GmbH. All rights reserved.
#
# @file                 ts-monotone.py
#
# @date                 2025-03-13
# @version              4.7.6
# @license              This example code from ViGEM GmbH has been provided
# for example and illustration purposes only.
#
# NOTE: THIS CODE IS PROVIDED FROM VIGEM GMBH "AS IS", WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTY. VIGEM GMBH SHALL ONLY BE LIABLE WITHOUT LIMITATION UNDER THE
# GERMAN PRODUCT ACT (PRODUKTHAFTUNGSGESETZ). IN NO EVENT SHALL VIGEM GMBH BE
# LIABLE FOR ANY CLAIMS, DAMAGES OR OTHER LIABILITIES ARISING OUT OF THE USE OF
# THIS CODE, UNLESS VIGEM GMBH IS RESPONSIBLE FOR SUCH CLAIMS, DAMAGES OR OTHER
# LIABILITIES DUE TO INTENT OR GROSS NEGLIGENCE ON ITS PART.
#
# The rights of use the code are based on the terms of:
#   - the contract between the user and ViGEM GmbH,
#   - the General Terms and Conditions for Software of ViGEM GmbH.
#     NOTE: SECTION 6.2 OF THE GENERAL TERMS AND CONDITIONS FOR SOFTWARE OF
#           VIGEM GMBH DOES NOT APPLY FOR CODES PROVIDED FROM VIGEM GMBH FOR
#           EXAMPLE AND ILLUSTRATION PURPOSES.
# In addition shall apply supplementary:
#   - the General Terms and Conditions of ViGEM GmbH
#
# These additional applicable provisions can be sent to you by postal mail on
# request and can be viewed as electronic documents on our homepage and our
# customer portal at www.ViGEM.de.


import PyCcalib_mdf as ccalib
import sys
import re


if len(sys.argv) == 2:
    ccalib.CcaLib_initInternal(ccalib.CCA_API_VERSION)
    
    path = sys.argv[1]    
    session_collection = ccalib.CcaSessionCollection_create()
    session_filter = ccalib.CcaSessionFilter()
    ccalib.CcaInitSessionFilter(session_filter)
    
    # Check if we got an IP or a path.
    # If we got an IP then we connect to the logger/CopyStation with that IP and use the remote API to scan for files on
    # the device. It will also automatically mount the SMB server of the device to read the data afterwards.
    # If we got a path, then we scan it ourselves.
    # This distinction is done because scanning over a network share has a network protocol overhead and is therefore a
    # lot slower. By using the remote APi the logger/CopyStation will scan locally and then return the found list.
    match = re.match("([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})", path)
    if match:
        print("Scanning by device")
        device = ccalib.CcaDevice_create(match.group(1))
        if not ccalib.CcaDevice_connect(device, True):
            print("ERROR: Couldn't connect to device ({:X})".format(ccalib.CcaLib_getLastError()))
            sys.exit(-1)
        if not ccalib.CcaSession_scanDevice(device, session_collection, session_filter):
            print("ERROR: Couldn't scan device ({:X})".format(ccalib.CcaLib_getLastError()))
            sys.exit(-1)
    else:
        print("Scanning by path")
        if not ccalib.CcaSession_scanPath(path, session_collection, session_filter):
            print("ERROR: Couldn't scan path ({:X})".format(ccalib.CcaLib_getLastError()))
            sys.exit(-1)

    # We use the found sessions to create a reader with it.
    reader_opts = ccalib.CcaSessionReaderOptions()
    ccalib.CcaInitSessionReaderOptions(reader_opts)
    reader = ccalib.CcaSessionReader_createBySessions(session_collection, reader_opts)
    
    if not ccalib.CcaSessionReader_open(reader):
        print("ERROR: Can't open reader ({:X})".format(ccalib.CcaLib_getLastError()))
        sys.exit(-1)

    # Read the data from the session reader and compare each message timestamp with the timestamp of the previous
    # message with the same bus ID
    last_ts = {}
    while True:
        res, message = ccalib.CcaSessionReader_read(reader)
        if not res:
            break

        res, time = ccalib.CcaMessage_timestamp(message)
        if not  res:
            print("WARNING: Couldn't get timestamp of message ({:X})".format(ccalib.CcaLib_getLastError()))
            continue
        res, timestamp, _async = ccalib.CcaTime_timestamp(time)
        if not res:
            print("WARNING: Couldn't get timestamp value ({:X})".format(ccalib.CcaLib_getLastError()))
            continue

        res, bus_id = ccalib.CcaMessage_busid(message)
        if not res:
            print("WARNING: Couldn't get bus ID of message ({:X})".format(ccalib.CcaLib_getLastError()))
            continue

        if not bus_id in last_ts:
            last_ts[bus_id] = 0

        if timestamp < last_ts[bus_id]:
            buffer_string = bytearray(25)
            if not ccalib.CcaTime_toIsoLocalLong(time, buffer_string, len(buffer_string)):
                print("WARNING: Couldn't get CcaTime_toIsoLocalLong of message ({:X})".format(ccalib.CcaLib_getLastError()))
                continue
            diff = timestamp-last_ts[bus_id]
            print("{} Error: timestamp smaller curr {} vs last {} = {}".format(buffer_string.decode("utf-8"), timestamp, last_ts[bus_id], diff))

        last_ts[bus_id] = timestamp

    # After reading is finished we can check the last error code if reading was successful. When EOF is returned then
    # no errors happened. In case of error you also received a short message on the console what happened.
    last_error = ccalib.CcaLib_getLastError()
    if last_error == ccalib.CCA_ERR_READ_END_OF_FILE:
        print("Reading finished successfully")
    else:
        print("Reading finished with errors: {:X}".format(last_error))
    
    ccalib.CcaSessionReader_destroy(reader)
    ccalib.CcaSessionCollection_destroy(session_collection)
    if match:
        ccalib.CcaDevice_destroy(device)
else:
    print("Unexpected argument count")
    print("Usage:")
    print(__file__+" <path to recorded files or IP to logger/CopyStation>")


    
    


