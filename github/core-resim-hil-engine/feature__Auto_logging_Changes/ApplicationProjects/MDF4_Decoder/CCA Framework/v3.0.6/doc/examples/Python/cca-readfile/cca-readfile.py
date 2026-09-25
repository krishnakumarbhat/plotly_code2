# Example file for the CCA-Framework-MDF: Demonstrates how to read a file and how to extract information from the read messages.
#
# @copyright            Copyright (c) 2021-2021 ViGEM GmbH. All rights reserved.
#
# @file                 cca-readfile.py
#
# @date                 2021-05-21
# @version              3.0.6
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
        res, busid = CcaMessage_busid(msg)
        res, msgClass = CcaMessage_class(msg)
        res = CcaMessage_payload(msg, pl)

        count = pl.Count
        element = pl.Elements

        print('\n-------------------------------')
        print('Index: ' + str(index))
        print('payload count: ' + str(count))
        payload = CcaMessagePayloadElement_frompointer(element)

        print('First 10 payload Elements: ')
        for i in range(10):
            print(payload[i], end=" ")


        timestamp_str = bytearray(25)
        if not CcaTime_toIsoUtcLong(timestamp, timestamp_str, len(timestamp_str)):
            print('CcaTime_toIsoUtc failed: '+ str(CcaLib_getLastError()))
            sys.exit(1)

        
        print('timestamp: ' +str(timestamp_str.decode('utf-8')))
        print('busid: ' + str(busid))
        print('msgclass: ' +str(msgClass))

        if msgClass == CCA_IF_ETHERNET:
            ethMsg = CcaMessage_createEthernetMessage(msg)
            if IsInvalidHandle(ethMsg):
                print('CcaMessage_createEthernetMessage failed: '+ str(CcaLib_getLastError()))
                sys.exit(1)
            
            res, sourceAddress= CcaEthernetMessage_sourceAddress(ethMsg)
            if not res:
                print('CcaEthernetMessage_sourceAddress failed: '+ str(CcaLib_getLastError()))
                sys.exit(1)
            print('Ethernet source Address: ' + sourceAddress)


            res, destinationAddress= CcaEthernetMessage_destinationAddress(ethMsg)
            if not res:
                print('CcaEthernetMessage_destinationAddress failed: '+ str(CcaLib_getLastError()))
                sys.exit(1)

            print('Ethernet destination Address: '+ destinationAddress)

            CcaEthernetMessage_destroy(ethMsg)
        index += 1

    CcaFile_destroy(ccaFileHandle)
    CcaSessionReader_destroy(sessReader)

else:
    print("Unexpected argument count")
    print("Usage:")
    print("cca-readfile.py <filename>")
