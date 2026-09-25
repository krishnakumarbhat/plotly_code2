
# ALL ABOUT PSP BB Resim Testing stratgey
-------------------------
# Table of contents
- [Table of contents]
  - [1. Introduction]
  - [2. Input Required]
  - [3. External Repo Required]
  - [4. Testing step]
      - [4.1. intergrate all new changes ]
      - [4.2. Build the new changes ]
      - [4.3. Update SIL_Engine_Config.xml]
      - [4.4. Update `Emb_Lib_Config.xml]
	  - [4.5. Update `SIL_Input_Logs.txt]
      - [4.6. copy the dll files]
      - [4.7. run the resim framework]
	  - [4.8. verify the embedded and SIL results]

-------------------------
## 1. Introduction
Gpo Resim framework to be used to test PSP building block in SIL.

Resim framework Path: ADVRADAR_AWR294X\software\app\emb_lib

-------------------------
## 2. Input Required
The below Inputs are required  to Test the PSP building block in Resim framework.

• Get Respective released embedded software vehicle log for each building block.
• Get SIL Engine from the `10028634_01_SRR_RESIM_Release` Plastic repository. get need changeset number from resim team for the released sw.

-------------------------
## 3. External Repo Required

10028634_01_SRR_RESIM_Release ( download from plastic to C:\Plastic\10028634_01_SRR_RESIM_Release path)

-------------------------
## 4. Testing step
CDC (Compressed Data Cube) : Selected Beam Vector Arrays from 'range doppler array' based on certain threshold and  Selection logic.
All bins within rdop_avg that are above the CDC_thold, along with any that are neighbors in range or Doppler to bins above the CDC_thold, shall be identified for logging.The data that is logged for such bins is the complex beamvector, range bin, and Doppler bin. 

-------------------------
#### 4.1. intergrate all new changes
Integrate all new changes to resim framework including building block jfrog link.

-------------------------
#### 4.2.  Build the new changes
Build the newly inegrated chnages in resim framework.

Build path: C:\WrkSp\GEN7\ADV_radar\ADVRADAR_AWR294X\software\app\emb_lib\build
build command: python build.py

#### 4.3 Update SIL_Engine_Config.xml
Update `SIL_Engine_Config.xml` in the SIL engine directory (`10028634_01_SRR_RESIM_Release/RESIM_SIL/GEN7_SIL_ENGINE`)
Change `RESIM_OUTPUT_PATH.Output_Path_Options` to `SAME_AS_INPUT`
Change `SENSOR_CONFIG` to the `<ADVRADAR_AWR294X>/software/app/emb_lib/Emb_Lib_Config.xml`
Change `RESIM_OUTPUT_FILE_FORMAT' ( ORCAS_MDF4 to 1)
Enable the required sensors in `SENSOR_STATUS` ( SENSOR RL to 1)
update the running mode in SIL_Entrypoint. ( example: for RDD mode : <SIL_Entrypoint>RDD_MODE</SIL_Entrypoint>)

#### 4.4  Update `Emb_Lib_Config.xml:
do the below update in ADVRADAR_AWR294X/software/app/emb_lib/Emb_Lib_Config.xml

1. XML_TRACE_DISABLED to XML_TRACE_ENABLED  ( to get embedded and sil log in xml format)
2. make 1 in XML_Prints which you want to test or verify.( example: for IDBB <Interference_Dets>1</Interference_Dets>)

#### 4.5  Update `SIL_Input_Logs.txt:
create a floder and keep the vehicle logs here.
Update the log path`SIL_Input_Logs.txt` in the SIL engine directory. (for example: C:\Resim_v_log\DEV_X8310_235_SDV_4SRad_20240916_124820_010_ESW-SRad9.0_Aptiv.MF4)


#### 4.6  copy the dll files:

1.copy radar_stream_decoder.dll files from C:\Plastic\10028634_01_SRR_RESIM_Release\RESIM_SIL\DECODER_DLL to C:\Plastic\10028634_01_SRR_RESIM_Release\RESIM_SIL\GEN7_SIL_ENGINE
2.build the resim frame work once again : python build.py
3.use copy_position_dll.bat C:\Plastic\10028634_01_SRR_RESIM_Release\RESIM_SIL\GEN7_SIL_ENGINE command to copy additional dll files.

#### 4.7  run the resim framework:

in Vs code run the resim framework in debug mode( click on Run -> start debugging).
wait for sometime untill resimulation for the log is in progress.

#### 4.7 verify the embedded and SIL results :

embedded and SIL simulated log will be generated in the path(XML\REAR_LEFT) where vehicle log is kept.
use beyond compare to verify SIL_xml_input(embedded) and SIL_xml_output(SIL).
