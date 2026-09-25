
# Smoke Tests

The python script in this folder are used for either smoke tests kicked off by jenkins or manually at a test bench.

The scripts included in this folder are the following:

# 1. IntegrationTest.py

    This script runs as part of the Gerrit smoke test that reads only UDP data processes the streams for various things.
    Examples of some of the checks done in this script are scan index skip check, temperature range checkes, first and second pass detections, faults, etc.

    This script requires a bench with a programmable power supply, a technica media box and a radar device.

# 2. PowerCycleTest.py

    This script simply cycles the power every 30 seconds. It captures the UDP data collected during each power cycle and then at the end it will combine all the data so you can see if there were any spike of xput or temperature. It will show the scan index incrementing then dropping back to zero to show when the power cycle occured. It will show if any power cycle had any faults. etc.
    This scrip is run only on the Dev branch in jenkins and not for every commit as this test can take time depending on how many power cycles are needed.

# 3. UDPChecker.py
    This script is used at a temperature chamber bench. It simly checkes UDP bandwidth and if it ever drops out it will power cycle the radar and then keep checking bandwidth.

# 4. streamChecker.py
    This script can be used locally to check scan index values for a given pcap file. It will provide a summary of each stream it detects.
