# Example file for the CCA-Framework-MDF: Demonstrates how to download all recorded data from a logger or CopyStation to a local folder.
#
# @copyright            Copyright (c) 2021-2021 ViGEM GmbH. All rights reserved.
#
# @file                 cca-download.py
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


#PythonCcaProgressCallBack class is defined and derived from C++ class CcaProgressCallBack
class PythonCcaProgressCallBack(CcaProgressCallBack):
    def __init__(self):
        CcaProgressCallBack.__init__(self)

    # Override C++ method: void handle(CcaTask action, CcaObject ccaObject, uint32_t progress, const char* actionDescription) = 0
    def handle(self, action, ccaObject, progress, actionDescription):
        if action == DownloadData:
            print("Description: " + actionDescription)
            
            res, downloadStats = CcaCallbackStatisticFromCcaObject(ccaObject)
            if not res:
                print('CcaCallbackStatisticFromCcaObject failed: '+ str(CcaLib_getLastError()))
                sys.exit(1)

            if downloadStats.ds_state == DS_DOWNLOADSESSION:
                print('transfered bytes/total bytes to transfer:' + str(downloadStats.curr_bytes ) + '/' +  str(downloadStats.tot_bytes_act ))
                print('skipped bytes: ' + str(downloadStats.curr_bytes_skip))
                print('current synched bytes/total bytes to sync: ' + str(downloadStats.curr_bytes + downloadStats.curr_bytes_skip)  + '/' +  str(downloadStats.tot_bytes))
                print('transfered files/total files: ' + str(downloadStats.curr_files) + '/' +  str(downloadStats.tot_files))
                print('skipped files: ' + str(downloadStats.curr_files_skip) )
                print('current file: ' + downloadStats.curr_name)

            if downloadStats.ds_state == DS_DELETESESSION:
                print('Deleting folder: ' + downloadStats.curr_name)
            
            print('Progress: ' + str(progress)+ '%')
          
        if action == DeleteData:
            print("Description: " + actionDescription)
            print('Progress: ' + str(progress)+ '%')

        if action == ConvertSession:
            print("Description: " + actionDescription)
            print('Progress: ' + str(progress)+ '%')
        

if len(sys.argv) == 3:
    CcaLib_initInternal(CCA_API_VERSION)
    print('CcaLib Version: '+CcaLib_getVersion())

    CcaTrace_setDebugLevel(0)
    
    print('Connecting to: '+sys.argv[1])
    dev = CcaDevice_create(sys.argv[1])
    if IsInvalidHandle(dev):
        print('CcaDevice_create failed: '+ str(CcaLib_getLastError()))
        sys.exit(1)
    
    if not CcaDevice_connect(dev, False):
        print('CcaDevice_connect failed: '+ str(CcaLib_getLastError()))
        sys.exit(1)
    
    col = CcaSessionCollection_create()
    
    filt = CcaSessionFilter() 
    CcaInitSessionFilter(filt)
       
    if not CcaSession_scanDevice(dev, col, filt):
        print('CcaSession_scanDevice failed: '+ str(CcaLib_getLastError()))
        sys.exit(1)
    
    handler = PythonCcaProgressCallBack()
    if not CcaSessionCollection_downloadData(sys.argv[2], col, filt, False, False, False, handler, 0):
       sys.exit(1)

    CcaSessionCollection_destroy(col)
    CcaDevice_destroy(dev)
    
else:
    print("Unexpected argument count")
    print("Usage:")
    print("cca-download.py [logger_ip] [local_folder]")
