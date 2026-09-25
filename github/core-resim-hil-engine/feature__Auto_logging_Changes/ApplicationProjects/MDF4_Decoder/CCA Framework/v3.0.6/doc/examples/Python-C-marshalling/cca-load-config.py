# Example file for the CCA-Framework-MDF: Demonstrates how to add a local settings file (.cca) to the presets of a
# CCA device and load it into the current configuration.
#
# @copyright            Copyright (c) 2019-2021 ViGEM GmbH. All rights reserved.
#
# @file                 cca-load-config.py
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


from CcaLib import *
import sys


if len(sys.argv) == 3:
    CcaLib_init()
    
    print('Connecting to: '+sys.argv[1])
    dev = CcaDevice_create(sys.argv[1])
    if dev == CCA_INVALID_HANDLE:
        sys.exit(1)
    
    if not CcaDevice_connect(dev, False):
        sys.exit(1)
    
    print('Loading settings from: '+sys.argv[2])
    settings = CcaSettings_fromFile(sys.argv[2])
    if settings == CCA_INVALID_HANDLE:
        sys.exit(1)

    arr = Array()
    if not CcaSettings_listInterfaces(settings, arr):
        sys.exit(1)

    print('Settings have following BusIds activated:')
    sarr = arr.cast(CcaCaptureInterfaceInfo)
    for i in range(arr.Count):
        if sarr[i].enabled and sarr[i].busId != 0:
            print(' - '+str(sarr[i].busId))
    arr.free()

    if not CcaSettings_setName(settings, 'test_settings'):
        sys.exit(1)

    print('Setting added as preset "test_settings" and loaded')
    if not CcaDevice_addPreset(dev, settings, True):
        sys.exit(1)

    CcaSettings_destroy(settings)
    CcaDevice_destroy(dev)
    
else:
    print("Unexpected argument count")
    print("Usage:")
    print("cca-config.py <logger_ip> <config_folder>")
