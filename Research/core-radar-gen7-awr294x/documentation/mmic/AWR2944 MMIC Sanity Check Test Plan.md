# MMIC Software Sanity Check Test Plan - AWR2944

This document provides a list of items to check the basic functionality of MMIC using the debugger (Lauterbach).

1. Run the script, **mmic_debug.cmm** to launch Latuerbach windows, which includes MMIC related variables added to the watch windows, for easy monitoring.

2. First thing to check is to ensure that all the MMIC APIs are exectued successfully without any error response. There is a global variable '**MMIC_API_Error_Code**' which captures the API responses in error scenario at different stages of the MMIC configuration.
*Non-zero* value indicates an *error*, while zero indicates successful configuration.

3. Also can check the return value '**ret_val**' of a particluar API of interest to ensure that it was successful, or at the end of major configuration stages '**Mmwavelink_Init(), Mmwavelink_Open(), Mmwavelink_Config(), Mmwavelink_Start(), MMIC_Look_Trigger()**' of MMIC, by setting a software breakpoint at those code lines.

4. Once the MMIC is successfully configured and triggered, it transmits the waveform configured and captures the data received in ADC Buffer, which is available at memory address **0xA5000000**. Check that the data is being received continuously for every frame and are non-zero values.
![ADC data screenshot](/ADC%20data.PNG)

5. There are few counters for monitoring the number of chirps received in a Look type. In this project, with the current implementation, a *Look*, *Frame* and *Sub-Frame* are synonymous, indicating that they are a set of chirps transmitted for a Single trigger of the MMIC.
* **Chirp_Count** - gets incremented for every chirp data received, this counter increments upto the maximum number of chirps configured for that particular Look type. For instance, if Look A is set to 512 chirps, the counter increments to 512. It is reset to '0' at the beginning of every frame.
* **Frame_Start_Count** - this counter gets incremented at the beginning of every Frame.
* **Frame_End_Count** - this counter gets incremented at the end of every Frame. *Frame_Start_Count* and *Frame_End_Count* should match at the end of a Frame and prior to next Look trigger.
* **Chirp_Per_Frame** - this is a test buffer of size 8, which stores the *Chirp_Count* received and is overwritten for every 8 frames. This is useful in observing the number of chirps received for every frame. Please note that the number of chirps has been one off by the actual number configured, as the Frame end interrupt seems to be occurring prior to the last chirp received interrupt. For example, if Look A has 512 chirps configured, *Chirp_Per_Frame[0]* shows value *511* instead of 512, whereas *Chirp_Count* shows *512* as expected.
![Chirp and Frame counters](/Chirp%20Counters.PNG)

6. *Radar_Ctl_Data* provides the status of the radar module, the look information and scan index value.
Make sure that the looks (*this_look* and *next_look*) are cycling through all 4 looks as configured in the normal working mode, except when a Single Look type is forced, during which it would only show the Look type that is being forced.
*init_status* should be 'RADAR_CTL_INIT_SUCCESS'.
And *scan_index* should be incrementing.
![Radar Control Data](/Radar%20Control%20Status.PNG)

7. MMIC functional safety checks, there are different periodic checks performed by the MMIC on variaous analog and digital modules available in it. And these checks provide the status along with relevant information as a report, for the users to take necessary action accordingly. All these monitoring reports are grouped and available under *RF_Mon_Report*. Critical errors are captured as faults, can check *Safety_Critical_Cause_Fault_Detected* and *System_Operation_State*, to ensure that there are no functional safety related errors on a high level.

8. *MMIC_Error* - Radar control module captures all the relevant MMIC errors at different stages in configuration, along with other related module errors. These provide a high level guidance to figure out the stage at which the Radar functionality had errored out and failed. Can monitor all the error counters and ensure that there are no errors. This structure is yet to be populated with relevant error counters, so some of them are not applicable.
![MMIC Error counters](/MMIC_Error.PNG)

9. Can also check that the MPRB (Max_Per_Range_Bin) Plot looks normal with a peak, usually around the 0th range bin, when the sensor is residing on bench facing up.
*IPC_D2M_Buffer[0].payload.rdd_stream_data.rdd_data.rdd1_max_per_range_bin* and
*IPC_D2M_Buffer[1].payload.rdd_stream_data.rdd_data.rdd1_max_per_range_bin* are available in graphical view when mmic_debug.cmm script is launched.
![Max Per Range Bin](/MPRB.PNG)

*Optional checks, since they don't exactly help verifying the Radar Front End, but more like the supporting modules.*

10. *Test Pattern generation* - TI MMIC has an internal test pattern generator that can create an output ramp pattern, to test the path from ADC buffer until the final output through LVDS. This test may just serve as a check for the ADC buffer integrity but not the data from the Radar subsystem. To enable this mode, need to define the macro *ADC_TEST_PATTERN_GEN_MODE_ENABLED* in the Bazel build files as mentioned below.
"-DADC_TEST_PATTERN_GEN_MODE_ENABLED", in tools/bazel/transition_ti_arm_platform.bzl and
"--define=ADC_TEST_PATTERN_GEN_MODE_ENABLED", in tools/bazel/transition_ti_c6000_platform.bzl
And as defined in the test pattern configuration API, the data pattern can be observed in the ADC Buffer at *0xA5000000*. Below screenshot shows the data incrementing by '2'.
![Test pattern data](/Test%20Pattern.PNG)

This document can be updated as the diagnostic features mature and more checks are available to monitor.

**File Revision History**

|Rev|Date|NetId|Name|SCR|
|-|-|-|-|-|
|0.1|10-Aug-2022| tj16xt| Sruthi Kilari| DDR-1693|
