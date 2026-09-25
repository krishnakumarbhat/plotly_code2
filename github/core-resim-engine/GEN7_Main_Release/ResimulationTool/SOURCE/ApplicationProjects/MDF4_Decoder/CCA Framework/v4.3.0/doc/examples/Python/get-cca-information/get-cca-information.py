# Example file for the CCA-Framework-MDF: Demonstrates how to connect to a device and print different information about it.
#
# @copyright            Copyright (c) 2022-2023 ViGEM GmbH. All rights reserved.
#
# @file                 get-cca-information.py
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


import sys
import PyCcalib_mdf as ccalib


def get_copystation_statistics(dev):
    """
    This function will print information specific to a cca device of type copy station.
    This function requires an already connected CcaDevice of type copy station.
    """
    # Technically the data-sync is also available on a logger, but it is usually not used there
    state = ccalib.CcaDataSyncState()
    res = ccalib.Cca_getDataSyncState(dev, state)
    if not res:
        print("Failed getting data-sync state from device: 0x{:X}".format(ccalib.CcaLib_getLastError()))
        return
    if state.state == ccalib.DATASYNC_UNKNOWN:
        print("unknown data-sync state in device")
        return
    elif state.state == ccalib.DATASYNC_INIT:
        print("No data-sync started yet")
        return
    elif state.state == ccalib.DATASYNC_FAILED:
        print("No data-sync active and last one failed with error code 0x{:X}".format(state.last_error_number))
        print_speed = False
    elif state.state == ccalib.DATASYNC_SUCCESS:
        print("No data-sync active and last one succeeded")
        print_speed = False
    elif state.state == ccalib.DATASYNC_SUSPEND:
        print("A data-sync is being canceled")
        print_speed = False
    else:
        print("A data-sync is transferring data")
        print_speed = True
    print("Transferred {} of {} files and skipped {}".format(state.curr_files, state.tot_files, state.curr_files_skip))
    print("Transferred {} of {} bytes and skipped {}".format(state.curr_bytes, state.tot_bytes, state.curr_bytes_skip))
    print("Progress: {}%".format(state.progress))
    print("Transferred data for {} seconds".format(state.time_elapsed))
    if print_speed:
        print("Estimated remaining time: {} seconds".format(state.time_estimated))
        print("Currently transferring with {} bytes/second".format(state.bytes_p_s))


def print_string_property(client_instance, property_name):
    """
    This function will read out a string property from a property server of a device and print it to the command line.
    """
    buffer = bytearray(64)
    if ccalib.CcaProperty_getStringValue(client_instance, property_name, buffer, len(buffer)):
        print("{} is {}".format(property_name, buffer.decode("utf-8")))
    else:
        error_code = ccalib.CcaLib_getLastError()
        # CCA_ERR_PROPERTY_HAS_NO_REQUESTED_VALUE can also mean that the requested property is of a different type
        # or does not exist in the device, but in our case it will always mean, that the property is not configured.
        if error_code == ccalib.CCA_ERR_PROPERTY_HAS_NO_REQUESTED_VALUE:
            print("No {} property configured on device".format(property_name))
        else:
            print("Failed getting {} from device: 0x{:X}".format(property_name, error_code))


def get_logger_statistics(dev):
    """
    This function will print information specific to a cca device of type logger.
    This function requires an already connected CcaDevice of type logger.
    """
    # First we start by getting some recording properties.
    # For this we need to connect to the server containing the properties.
    recorder = ccalib.CcaClientInstance_createForServerClass(dev, ccalib.CCA_SERVER_RECORDER)
    if ccalib.IsInvalidHandle(recorder):
        print("Failed getting recorder server from device to read out some properties: 0x{:X}".format(
            ccalib.CcaLib_getLastError()))
    else:
        print_string_property(recorder, "vin")
        print_string_property(recorder, "registration_number")
        print_string_property(recorder, "driver")
        print_string_property(recorder, "technician")
        print_string_property(recorder, "project")
        print_string_property(recorder, "ecu_serial")
        ccalib.CcaClientInstance_destroy(recorder)

    # Now we will read out the information about the current/last recording
    state_summary = ccalib.CcaDeviceStateSummary()
    res = ccalib.CcaDevice_getStateSummary(dev, state_summary)
    if not res:
        print("Failed getting state summary from device: 0x{:X}".format(ccalib.CcaLib_getLastError()))
        return
    print("Recording is active" if state_summary.captureActive else "Recording is inactive")
    print("A trigger is active" if state_summary.triggerActive else "No trigger is active")
    print(
        "An overflow happened during recording" if state_summary.overflow else "No overflow happened during recording")
    print(
        "An cca error happened during recording" if state_summary.ccaError else "No cca error happened during recording")
    print(
        "An disk error happened during recording" if state_summary.diskError else "No disk error happened during recording")
    print(
        "Component error(s) happened during recording" if state_summary.componentSummary == ccalib.CCA_CS_ERROR else "All components are OK")
    print(
        "The removable data storage is full" if state_summary.diskFull else "The removable data storage has storage space left")
    print("The data storage is filled to {} GB / {} GB ({}%)".format(state_summary.diskUsageBytes / 1e9,
                                                                     state_summary.diskSizeInByte / 1e9,
                                                                     state_summary.diskUsagePercent))
    print("Last trigger ID: {}".format(state_summary.lastTriggerId))
    print("Last marker ID: {}".format(state_summary.lastMarkerId))
    print("Number of triggers events: {}".format(state_summary.triggerEventCounter))
    print("Received messages: {}".format(state_summary.receivedMessageCounter))
    print("Received bytes: {}".format(state_summary.receivedBytesCounter))
    print("Message/s: {}".format(state_summary.messagesPerSecond))
    print("Bytes/s: {}".format(state_summary.bytesPerSecond))
    print("Disk bytes/s: {}".format(state_summary.diskBytesPerSecond))
    print("CCA errors: {}".format(state_summary.ccaErrCounter))
    print("Overflow errors: {}".format(state_summary.overflowErrCounter))
    print("Bus errors: {}".format(state_summary.busErrCounter))

    # Detailed statistic about all active writers
    writers = ccalib.CcaWriterStateArray()
    res = ccalib.CcaDevice_getWriterState(dev, writers)
    if not res:
        print("Failed getting writer statistic from device: 0x{:X}".format(ccalib.CcaLib_getLastError()))
        return
    print("Logger has {} enabled writers".format(len(writers)))
    print("{:<15} {:>14} {:>14} {:>14} {:>14} {:>14}".format("Label", "Messages", "Messages/s", "Bytes", "Bytes/s",
                                                             "Incoming B/s"))
    for i in range(len(writers)):
        maximum_label_length = 15
        writer = writers[i]
        print("{:<15} {:>14} {:>14} {:>14} {:>14} {:>14}".format(
            (writer.label[:maximum_label_length - 2] + '..') if len(
                writer.label) > maximum_label_length else writer.label, writer.messages, writer.messages_per_second,
            writer.writtenBytes, writer.bytesPerSecond, writer.incomingBytesPerSecond))
        pass

    # Detailed statistic about all receiving interfaces
    # To be listed here an interface has to receive at least one message
    interface_statistics = ccalib.CcaBusinterfaceStatisticArray()
    if not ccalib.CcaDevice_getBusInterfaceStatisticComplete(dev, interface_statistics):
        print("Failed getting interface statistic from device: 0x{:X}".format(ccalib.CcaLib_getLastError()))
        return
    print("Logger has {} receiving interfaces".format(len(interface_statistics)))
    print("{:<8} {:>14} {:>14} {:>14} {:>14}".format("Bus ID", "Messages", "Messages/s", "Bytes", "Bytes/s"))
    for i in range(len(interface_statistics)):
        interface = interface_statistics[i]
        print("0x{:0>6X} {:>14} {:>14} {:>14} {:>14}".format(interface.busId, interface.receivedMessageCounter,
                                                             interface.messagesPerSecond,
                                                             interface.receivedBytesCounter, interface.bytesPerSecond))


def get_cca_information(ip):
    """
    This function will print a lot of information about the requested cca device.
    """
    # This will initialize the library.
    # This will primarily check if the header file has the same version as the library file.
    ccalib.CcaLib_initInternal(ccalib.CCA_API_VERSION)

    # Connect to the device.
    # If a hostname is given as IP, then the device already needs to be connect when we call create,
    # because we have to resolve the DNS name into an IP.
    # Otherwise the device could stay unconnected until we call CcaDevice_connect
    dev = ccalib.CcaDevice_create(ip)
    if ccalib.IsInvalidHandle(dev):
        print("Failed creating device for IP/hostname {}: 0x{:X}".format(ip, ccalib.CcaLib_getLastError()))
        return
    if not ccalib.CcaDevice_connect(dev, False):
        print("Failed connecting to device with IP/Hostname {}: 0x{:X}".format(ip, ccalib.CcaLib_getLastError()))
        ccalib.CcaDevice_destroy(dev)
        return

    # Get some general information about the firmware and device
    system_info = ccalib.CcaSystemInfo()
    if not ccalib.CcaDevice_getSystemInfo(dev, system_info):
        print("Failed getting system info from device: 0x{:X}".format(ccalib.CcaLib_getLastError()))
        ccalib.CcaDevice_destroy(dev)
        return
    print("Device serial number: {}".format(system_info.serial))
    print("Device firmware version: {}.{}.{}".format(system_info.image.version.major, system_info.image.version.minor,
                                                     system_info.image.version.build))
    print("Product family: {}".format(system_info.product_family))
    print("Product generation: {}".format(system_info.product_generation))
    print("Product variant: {}".format(system_info.product_variant))

    # Find out if we have a logger or copy station. Some functions only work for a specific device type.
    res, is_logger = ccalib.CcaDevice_isLogger(dev)
    if not res:
        print("Failed checking if device is a logger or copy station: 0x{:X}".format(ccalib.CcaLib_getLastError()))
        ccalib.CcaDevice_destroy(dev)
        return

    # Check if a removable data storage is connected and print the serial number.
    # These functions only work for copy stations or 9010 logger running firmware 3.x and higher.
    # We only need to check the firmware version for the second case because older loggers can't run this firmware.
    if not is_logger or ccalib.CCA_VERSION_GEQ(system_info.image.version, 3, 0, 0):
        # Check if a removable data storage is inserted
        res, connected = ccalib.CcaDevice_isDatabayConnected(dev)
        if not res:
            print("Failed checking if a removable data storage is connected to device: 0x{:X}".format(
                ccalib.CcaLib_getLastError()))
            ccalib.CcaDevice_destroy(dev)
            return
        if connected:
            res, count = ccalib.CcaDevice_getDatabayCount(dev)
            if not res:
                print("Failed getting the removable data storage count of device: 0x{:X}".format(
                    ccalib.CcaLib_getLastError()))
                ccalib.CcaDevice_destroy(dev)
                return
            if count > 0:
                serial_number = bytearray(20)
                # The actual serial number of the data storage can only be get in firmware 3.7.5 or newer.
                # Older Versions have the fallback of a generated hardware ID.
                if ccalib.CCA_VERSION_GEQ(system_info.image.version, 3, 7, 5):
                    res = ccalib.CcaDevice_getDatabaySN(dev, serial_number, len(serial_number))
                    if not res:
                        print("Failed getting the removable data storage serial number of device: 0x{:X}".format(
                            ccalib.CcaLib_getLastError()))
                        ccalib.CcaDevice_destroy(dev)
                        return
                    print("Removable data storage with the following serial number is mounted in device: {}".format(
                        serial_number.decode("utf-8")))
                else:
                    res = ccalib.CcaDevice_getDatabayID(dev, serial_number)
                    if not res:
                        print("Failed getting the removable data storage hardware ID of device: 0x{:X}".format(
                            ccalib.CcaLib_getLastError()))
                        ccalib.CcaDevice_destroy(dev)
                        return
                    print("Removable data storage with the following hardware ID is mounted in device: {}".format(
                        serial_number.decode("utf-8")))
            else:
                print("Removable data storage is inserted in device, but not mounted")
        else:
            print("No removable data storage is connected to device")
    if is_logger:
        get_logger_statistics(dev)
    else:
        get_copystation_statistics(dev)
    ccalib.CcaDevice_destroy(dev)


if __name__ == "__main__":
    if len(sys.argv) >= 2:
        get_cca_information(sys.argv[1])
    else:
        print("Unexpected argument count")
        print("Usage:")
        print("get-cca-information.py [device IP or hostname]")
