# Example file for the CCA-Framework-MDF: Demonstrates how to scan the network for cca devices and show some information about the found devices.
#
# @copyright            Copyright (c) 2021-2025 ViGEM GmbH. All rights reserved.
#
# @file                 cca-device.py
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

from PyCcalib_mdf import *
import sys

CcaLib_initInternal(CCA_API_VERSION)
print('CcaLib Version: ' + CcaLib_getVersion())

CcaTrace_setDebugLevel(CCALIB_TRACE_WARNING)

print('Scanning:')
devices = CcaDeviceCollection_create()
if not CcaFinder_scan(devices):
    print('Failed scanning network for CCA devices ({:x})'.format(CcaLib_getLastError()))
    CcaDeviceCollection_destroy(devices)
    sys.exit(1)

count = CcaDeviceCollection_count(devices)

if count == 0:
    print('no devices found')
    CcaDeviceCollection_destroy(devices)
    sys.exit(1)

print('number of devices found: ' + str(count))

for i in range(count):
    dev = CcaDeviceCollection_get(devices, i)
    ip_string = bytearray(30)
    CcaDevice_getIpAddress(dev, ip_string, len(ip_string))

    if not CcaDevice_connect(dev, False):
        print('Failed to connect to device with IP: ' + ip_string.decode("utf-8"))
        continue

    name_string = bytearray(30)
    if not CcaDevice_getName(dev, name_string, len(name_string)):
        print('Failed to get the name of the device with IP: ' + ip_string.decode("utf-8"))
        continue

    print('{}: {}'.format(name_string.decode("utf-8"), ip_string.decode("utf-8")))

CcaDeviceCollection_destroy(devices)
