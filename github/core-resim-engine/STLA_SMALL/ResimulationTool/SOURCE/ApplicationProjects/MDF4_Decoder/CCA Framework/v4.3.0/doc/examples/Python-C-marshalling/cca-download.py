# Example file for the CCA-Framework-MDF: Demonstrates how to download all files from a logger.
#
# @copyright            Copyright (c) 2019-2023 ViGEM GmbH. All rights reserved.
#
# @file                 cca-download.py
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


from CcaLib import *
import sys


@CCA_PROGRESS_CALLBACK
def cb(action, object, progress, desc):
    print("Action: " + str(action) + " Progress: " + str(progress) + " Description: " + desc.decode('utf-8'))


if len(sys.argv) == 3:
    CcaLib_init()
    CcaTrace_setDebugLevel(0)
    
    print('Connecting to: '+sys.argv[1])
    dev = CcaDevice_create(sys.argv[1])
    if dev == CCA_INVALID_HANDLE:
        sys.exit(1)
    
    if not CcaDevice_connect(dev, False):
        sys.exit(1)
    
    col = CcaSessionCollection_create()
    
    filt = CcaSessionFilter()
   
    if not CcaSession_scanDevice(dev, col, pointer(filt)):
        sys.exit(1)
    
    if not CcaSessionCollection_downloadData(sys.argv[2], col, pointer(filt), False, False, False, cb, 0):
       sys.exit(1)

    # if not CcaSessionCollection_executeDataSync(sys.argv[2], col, dev, pointer(filt), False, cb):
    #     sys.exit(1)
    
    CcaSessionCollection_destroy(col)
    CcaDevice_destroy(dev)
    
else:
    print("Unexpected argument count")
    print("Usage:")
    print("cca-download.py <logger_ip> <local_folder>")
