# Example file for the CCA-Framework-MDF: Demonstrates how to download all recorded data from a logger or CopyStation to a local folder.
#
# @copyright            Copyright (c) 2021-2024 ViGEM GmbH. All rights reserved.
#
# @file                 cca-convert.py
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


#PythonCcaProgressCallBack class is defined and derived from C++ class CcaProgressCallBack
class PythonCcaProgressCallBack(CcaProgressCallBack):
    def __init__(self):
        CcaProgressCallBack.__init__(self)

    # Override C++ method: void handle(CcaTask action, CcaObject ccaObject, uint32_t progress, const char* actionDescription) = 0
    def handle(self, action, ccaObject, progress, actionDescription):
        if action == ConvertSession:
            print("Description: " + actionDescription)
            print('Progress: ' + str(progress)+ '%')
        

if len(sys.argv) == 4:
    CcaLib_initInternal(CCA_API_VERSION)
    print('CcaLib Version: '+CcaLib_getVersion())
    CcaTrace_setDebugLevel(0)
    
    # Create and init filters
    sessionfilt = CcaSessionFilter() 
    CcaInitSessionFilter(sessionfilt)
    messagefilt = CcaMessageFilter() 
    CcaInitMessageFilter(messagefilt)

    # Set conversion options
    convOpt = CcaConversionOptions()
    CcaInitConversionOptions(convOpt)

    convOpt.outputFormats = CcaConversionFormatArray([CCA_VPCAP])              # Convert to VPCAP
    convOpt.merge = True;                               # Merge all sessions into one file
    convOpt.conversionType = CCA_CONVERT_ALL;           # Convert triggered and continious sessions
    convOpt.maxOutputFileSize = 5;                      # Set maximum size of the output file to 5MB. File will be split in 5MB file-chunks.
    convOpt.outputFileName = "%name%%label%_%st%";      # Set output file name with macros
    
    # Create CcaPath array
    #add the given input folders in CcaPathArray

    pathArray = CcaPathArray([sys.argv[1],sys.argv[2]])

    print('Converting from:')
    for index in range(len(pathArray)):
        print(pathArray[index])

    # destination folder which will contain the converted files
    outputDirectory = sys.argv[3]

    handler = PythonCcaProgressCallBack()
    if not Cca_convert(outputDirectory, pathArray, sessionfilt, messagefilt, convOpt, handler):
        print('Cca_convert failed: '+ str(CcaLib_getLastError()))
        sys.exit(1)

else:
    print("Unexpected argument count")
    print("Usage:")
    print("cca-convert.py [input_folder1] [input_folder2] [destination_folder]")
