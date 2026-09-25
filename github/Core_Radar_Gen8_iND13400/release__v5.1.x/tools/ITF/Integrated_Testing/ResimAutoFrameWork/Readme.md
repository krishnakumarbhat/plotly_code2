# Gen8 iND13400 ITF
### Overview
This folder contains Integration tetsing framework which is used to validate the signal processing building blocks.

### What it does?
This framework simulates the ADC data from the corresponding testcases using Radar simulator and feeds into Executable spec, Matlab Executable spec and Embedded software and collects the data across all the Spec and compares.

There will be HTML report generated for the corresponding testcase.

### Requirements
Ensure that the Executable Specs and Mex Spec are cloned before running ITF
- For the time being, Ensure the PBL that is being flashed on the board is relevant to the board.
- Power cycle after flashing the board is required at the moment to start the ECU, There is a timeout window to perform a powercycle. Please comply.
- Ensure the build outputs are present for the time being.

### Triggering the Framework
Lite_ITF_data_collection.m is the main file that needs to be run through Matlab to perform Integration testing.
- Ensure the testcases are present in the file folder corresponding to the selected sensor type.

### Flags and it's uses
There are some flags that are used/needs to set for different scenarios as given below.

debug_mode - Set this to 1 for running ITF lite just for data collection, 0 for Integration testing

offline_mode_inject - offline_mode_inject used to enable injection mode selection
    0 - enable ITF data collection of normal ADC input from MARS
    1 - enable radar datacube injection
    2 - enable ADC injection

save_ADC_data - Set this to 1 for using the earlier simulated data from the radar simulator for a corresponding testcase.

YET TO ENABLE: mex_embed_comparison_mode -  Enable this for Mex vs Embed only comparision

TODO: af_Testing - set to 1 to just run AF and comparision

### Reports
Reports will be generated for the corresponding testcase under test in the root folder.
