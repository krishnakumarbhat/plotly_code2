# Example file for the CCA-Framework-MDF: Demonstrates how to scan the network for CCA devices and print some
# information to the found devices.
#
# @copyright            Copyright (c) 2016-2025 ViGEM GmbH. All rights reserved.
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


from CcaLib import *
import sys


@CCA_TRACE_WRITE_CALLBACK
def trace(lvl, file, function, line, message):
    print(message)


CcaLib_init()
CcaTrace_setWriteCallback(trace)
print('Scanning:')
devices = CcaDeviceCollection_create()
if not CcaFinder_scan(devices):
    print('Failed scanning network for CCA devices ({:x})'.format(CcaLib_getLastError()))
    sys.exit(1)
count = CcaDeviceCollection_count(devices)
print('Found {} devices:'.format(count))
for i in range(count):
    dev = CcaDeviceCollection_get(devices, i)
    ip_string = create_string_buffer(20)
    CcaDevice_getIpAddress(dev, ip_string, len(ip_string))
    if not CcaDevice_connect(dev, False):
        print('Failed to connect to device with IP {} to get more information ({:x})'.format(ip_string.value.decode(),
                                                                                           CcaLib_getLastError()))
        continue
    name_string = create_string_buffer(CCA_DEVICE_NAME_MAX)
    if not CcaDevice_getName(dev, name_string, len(name_string)):
        print('Failed to getting name of device with IP {} ({:x})'.format(ip_string.value.decode(),
                                                                        CcaLib_getLastError()))
        continue
    print('{}: {}'.format(name_string.value.decode(), ip_string.value.decode()))
CcaDeviceCollection_destroy(devices)
