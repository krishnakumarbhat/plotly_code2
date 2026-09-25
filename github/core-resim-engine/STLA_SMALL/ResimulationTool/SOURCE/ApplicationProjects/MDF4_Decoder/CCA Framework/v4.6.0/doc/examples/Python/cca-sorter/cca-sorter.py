# Example file for the CCA-Framework-MDF: Demonstrates how to create a writer which sorts all messages given to it.
#
# @copyright            Copyright (c) 2021-2024 ViGEM GmbH. All rights reserved.
#
# @file                 cca-sorter.py
#
# @date                 2024-09-05
# @version              4.6.0
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

if len(sys.argv) == 3:

    CcaLib_initInternal(CCA_API_VERSION)
    print('CcaLib Version: '+CcaLib_getVersion())

    CcaTrace_setDebugLevel(0)

    filter = CcaSessionFilter() 
    CcaInitSessionFilter(filter)

    sessions = CcaSessionCollection_create()
    CcaSession_scanPath(sys.argv[1], sessions, filter)

    readerOptions = CcaSessionReaderOptions()
    CcaInitSessionReaderOptions(readerOptions)

    reader = CcaSessionReader_createBySessions(sessions, readerOptions)
    if IsInvalidHandle(reader):
        print('CcaSessionReader_createBySessions failed: '+ CcaLib_getLastError())
        CcaSessionCollection_destroy(sessions)
        sys.exit(1)

    if not CcaSessionReader_open(reader):
        print('CcaSessionReader_open failed: '+ CcaLib_getLastError())
        CcaSessionReader_destroy(reader)
        CcaSessionCollection_destroy(sessions)
        sys.exit(1)

    writer = CcaWriter_createVpcap(0)
    if IsInvalidHandle(writer):
        print('CcaWriter_createVpcap failed: '+ CcaLib_getLastError())
        CcaSessionReader_close(reader)
        CcaSessionReader_destroy(reader)
        CcaSessionCollection_destroy(sessions)
        sys.exit(1)

    sorter = CcaWriter_createSorter(writer,1000 ,1000 ,10 )
    if IsInvalidHandle(sorter):
        print('CcaWriter_createSorter failed: '+ CcaLib_getLastError())
        CcaSessionReader_close(reader)
        CcaSessionReader_destroy(reader)
        CcaSessionCollection_destroy(sessions)
        sys.exit(1)

    if not CcaWriter_open(sorter, sys.argv[2]):
        print('CcaWriter_open failed: '+ CcaLib_getLastError())
        CcaWriter_destroy(sorter)
        CcaSessionReader_close(reader)
        CcaSessionReader_destroy(reader)
        CcaSessionCollection_destroy(sessions)
        sys.exit(1)

    index = 0
    while True:
        res, msg = CcaSessionReader_read(reader)
        if not res:
            break
        res, writeresult = CcaWriter_write(sorter, msg)
        if not res:
            print('CcaWriter_write failed: '+ CcaLib_getLastError())
            sys.exit(1)
        index += 1
        print(index)

    CcaWriter_destroy(sorter)
    CcaSessionReader_close(reader)
    CcaSessionReader_destroy(reader)
    CcaSessionCollection_destroy(sessions)

else:
    print("Unexpected argument count")
    print("Usage:")
    print("cca-sorter.py [inputDirectory] [outputFile]")
