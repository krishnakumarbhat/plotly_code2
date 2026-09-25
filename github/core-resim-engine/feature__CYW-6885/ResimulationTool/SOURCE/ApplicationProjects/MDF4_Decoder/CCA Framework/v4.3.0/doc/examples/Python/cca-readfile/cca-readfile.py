# Example file for the CCA-Framework-MDF: Demonstrates how to read a file and how to extract information from the read messages.
#
# @copyright            Copyright (c) 2021-2023 ViGEM GmbH. All rights reserved.
#
# @file                 cca-readfile.py
#
# @date                 2023-07-27
# @version              4.3.0
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

from PyCcalib_mdf import *
import sys


if len(sys.argv) == 2:
    CcaLib_initInternal(CCA_API_VERSION)
    print('CcaLib Version: '+ str(CcaLib_getVersion()))

    ccaFileHandle = CcaFile_create(sys.argv[1]);
    if IsInvalidHandle(ccaFileHandle):
        print('CcaFile_create failed: '+ str(CcaLib_getLastError()))
        sys.exit(1)
    
    sessReaderOpt = CcaSessionReaderOptions()
    CcaInitSessionReaderOptions(sessReaderOpt)

    sessReader = CcaSessionReader_createByFile(ccaFileHandle, sessReaderOpt)
    if IsInvalidHandle(sessReader):
        print('CcaSessionReader_createByFile failed: '+ str(CcaLib_getLastError()))
        sys.exit(1)

    if not CcaSessionReader_open(sessReader):
        print('Error: cannot open session reader: '+ str(CcaLib_getLastError()))
        sys.exit(1)

    index = 0
    while True:
        res, msg = CcaSessionReader_read(sessReader)
        if not res:
            break

        pl = CcaMessagePayload()
        
        res, timestamp = CcaMessage_timestamp(msg)
        if not res:
            print('Could not get timestamp from message')
        res, busid = CcaMessage_busid(msg)
        if not res:
            print('Could not get bus ID from message')
        res, msgClass = CcaMessage_class(msg)
        if not res:
            print('Could not get message class from message')

        print('\n-------------------------------')
        print('Index: ' + str(index))

        print("\n")
        timestamp_str = bytearray(25)
        if not CcaTime_toIsoUtcLong(timestamp, timestamp_str, len(timestamp_str)):
            print('CcaTime_toIsoUtc failed: '+ str(CcaLib_getLastError()))
            sys.exit(1)

        print('timestamp: ' +str(timestamp_str.decode('utf-8')))
        print('busid: ' + str(busid))
        print('msgclass: ' +str(msgClass))

        if msgClass == CCA_IF_ETHERNET:
            res, sourceAddress= CcaEthernetMessage_sourceAddress(msg)
            if not res:
                print('CcaEthernetMessage_sourceAddress failed: '+ str(CcaLib_getLastError()))
                sys.exit(1)
            print('Ethernet source Address: ' + sourceAddress)

            res, destinationAddress= CcaEthernetMessage_destinationAddress(msg)
            if not res:
                print('CcaEthernetMessage_destinationAddress failed: '+ str(CcaLib_getLastError()))
                sys.exit(1)

            print('Ethernet destination Address: '+ destinationAddress)

        index += 1

    CcaFile_destroy(ccaFileHandle)
    CcaSessionReader_destroy(sessReader)

else:
    print("Unexpected argument count")
    print("Usage:")
    print("cca-readfile.py <filename>")
