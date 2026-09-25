from MUDP_Tool_spec_widgets import *
import threading
from datetime import date
from os import system
import subprocess
from HIL_Tool_spec_widgets import *

def Write_HIL_TOOL_XML(Global_var, HIL_Data, ENET_data, Sensor_and_ECU):
    with open('Hil_Configuration.xml', 'w+', encoding='cp1252') as file_pointer:
        file_pointer.write('\n<?xml version="1.0" encoding="UTF - 8" ?> ')
        file_pointer.write("\n<!-- SRR5 APTIV HIL component configuration -->")
        file_pointer.write("\n<HIL_SRR5_Configuration>")
        file_pointer.write("\n	<!-- HIL SRR5 Configuration XML version -->")
        file_pointer.write("\n	<!-- Version X.Y where:")
        file_pointer.write("\n		X - major change where some new parameters were added")
        file_pointer.write("\n		Y - minor change with no influence on parsing (comment or value change) -->	")
        file_pointer.write("\n    <HIL_SRR_Configuration_XML_Version>8.0</HIL_SRR_Configuration_XML_Version>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!-- To define the CUSTOMER for which the HIL is Executed. ")
        file_pointer.write(
            "\n		 Options available are: SRR5: GEELY_SRR5,HKMC_SRR5,RNA_SUV,RNA_CDV,BMW_LOW,BMW_SAT,BMW_BPIL,BMW_HIGH -->")
        file_pointer.write("\n								<!-- Defined in enum Customer_T in Radar_Config.h -->")
        file_pointer.write("\n	<CUSTOMER_NAME>" + Global_var.Customer_Name + "</CUSTOMER_NAME>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!--UDP_REC_VER_INFO>A318</UDP_REC_VER_INFO> <!--A318 or A218 or A118-->")
        file_pointer.write("\n	")
        file_pointer.write(
            "\n	<!-- Sensor selection values: 1 => Sensor Connected , Resim is executed for that Sensor ")
        file_pointer.write(
            "\n	                              0 => Sensor is not connected to bench,Resim is not executed for that Sensor Position -->")
        file_pointer.write("\n	<SENSOR_CONNECTION_STATUS>")
        file_pointer.write("\n		<REAR_LEFT>" + str(Sensor_and_ECU.REAR_LEFT.get()) + "</REAR_LEFT>")
        file_pointer.write("\n		<REAR_RIGHT>" + str(Sensor_and_ECU.REAR_RIGHT.get()) + "</REAR_RIGHT>")
        file_pointer.write("\n		<FRONT_RIGHT>" + str(Sensor_and_ECU.FRONT_RIGHT.get()) + "</FRONT_RIGHT>")
        file_pointer.write("\n		<FRONT_LEFT>" + str(Sensor_and_ECU.FRONT_LEFT.get()) + "</FRONT_LEFT>")
        file_pointer.write("\n		")
        file_pointer.write(
            "\n		<BPR_RIGHT>" + str(
                Sensor_and_ECU.BP_RIGHT.get()) + "</BPR_RIGHT> <!-- BPILLAR RIGHT CENTER IP 77-->")
        file_pointer.write(
            "\n		<BPR_LEFT>" + str(
                Sensor_and_ECU.BP_LEFT.get()) + "</BPR_LEFT> <!-- BPILLAR LEFT CENTER IP 78-->")
        file_pointer.write("\n	")
        file_pointer.write("\n		<RADAR_ECU>" + str(Sensor_and_ECU.RADAR_ECU.get()) + "</RADAR_ECU>")
        file_pointer.write("\n	</SENSOR_CONNECTION_STATUS>")
        file_pointer.write("\n	")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!-- Adding the dvsu path to take input log from that file path -->")
        file_pointer.write("\n	<!-- If same folder has series 001/002/003/etc , then Hil will run for all sets -->")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!--INPUT_FILES>")
        file_pointer.write(
            "\n		<FILE_PATH_1>D:\Ritesh\A310_Logs\Mid_Logs\ECU_3_9_2_SENS_3_9_3_SRR_DEBUG_WBATR91070LC63638_20191128_162334_deb_0005.MF4</FILE_PATH_1>	")
        file_pointer.write(
            "\n		<!--FILE_PATH_2>D:\Ritesh\A310_Logs\Mid_Logs\ECU_3_9_2_SENS_3_9_3_SRR_DEBUG_WBATR91070LC63638_20191128_162334_deb_0005.MF4</FILE_PATH_2-->	")
        file_pointer.write("\n	<!--/INPUT_FILES -->	")
        file_pointer.write("\n	")
        file_pointer.write('\n	 <!-- Input Mode Options available are - ')
        file_pointer.write(
            "\n	SIX_DVSU_INPUT : SIX dvsu enabled to handle six  input DVSU file ,proceed hil with 6 dvsu's file.(default option)")
        file_pointer.write(
            "\n	FOUR_DVSU_INPUT : FOUR dvsu enabled to handle four  input DVSU file ,proceed hil with 4 dvsu's file.(default option)")
        file_pointer.write(
            "\n	SINGLE_DVSU_INPUT: Single dvsu enabled to handle Single input DVSU file , proceed hil with Single dvsu file")
        file_pointer.write("\n	MDF4_INPUT :  MDF4_DVSU_ENABLED to handle MDF4 file proceed hil with mdf4 file -->")
        file_pointer.write("\n	<INPUT_OPTIONS>MDF4_INPUT</INPUT_OPTIONS>")
        file_pointer.write("\n	")
        file_pointer.write(
            "\n	<!--To define whether the HiL execution should happen in a continuous mode(i.e Executing series of log) or execution of single set of log")
        file_pointer.write(
            "\n		The option's available are : YES => Runs all the series of log in the folder based on the path provided")
        file_pointer.write(
            "\n									  NO => Runs only one log even though there is series of logs-->")
        file_pointer.write("\n	<CONTINUOUS_RUN_MODE>" + str(HIL_Data.Continues_run_mod) + "</CONTINUOUS_RUN_MODE>")
        file_pointer.write("\n	")
        file_pointer.write("\n	")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!-- To define the number of times the LOG must be Executed.-->")
        file_pointer.write("\n	<LOG_REPEAT_COUNT>" + str(HIL_Data.Log_repeate_cnt) + "</LOG_REPEAT_COUNT>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!--UDP or CAN-->")
        file_pointer.write("\n	<VEHICLE_DATA_SOURCE>" + HIL_Data.Vehicle_data_source + "</VEHICLE_DATA_SOURCE>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!--UDP or CAN-->")
        file_pointer.write(
            "\n	<FUSION_DETECTION_SOURCE>" + HIL_Data.Fusion_det_source + "</FUSION_DETECTION_SOURCE>		")
        file_pointer.write("\n	")
        file_pointer.write(
            "\n	<!--To define whether to close Command window automatically after HiL Execution : YES => Close Command window automatically after execution")
        file_pointer.write(
            "\n																				    : NO  => Wait for user to Press Enter-->")
        file_pointer.write("\n	<AUTOCLOSE_COMMAND_WIN>" + str(HIL_Data.Auto_close_win) + "</AUTOCLOSE_COMMAND_WIN>")
        file_pointer.write("\n	")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!-- To define the CUSTOMER for which the HIL MODE is Executed. ")
        file_pointer.write("\n	Options available are: LOG_SIMULATION = 1, LIVE_SIMULATION = 2  -->")
        file_pointer.write("\n    <INPUT_DATA_TYPE>" + str(HIL_Data.Input_data_type) + "</INPUT_DATA_TYPE>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!-- To define the CUSTOMER for which mode the radars are working. ")
        file_pointer.write("\n		 Options available are: DETECTION = 1, TRACKER = 2 -->")
        file_pointer.write("\n		 <!-- TRACKER/FEATURE_FUNCTION-->")
        file_pointer.write("\n	<SENSOR_RUN_MODE>" + str(HIL_Data.Sensor_run_mode) + "</SENSOR_RUN_MODE>")
        file_pointer.write("\n	")
        file_pointer.write(
            "\n	<!--Whether the HiL execution should Run Radar Algorithms without Fusion or With Fusion")
        file_pointer.write("\n		The option's available are : YES => Runs Radar Algorithms with Fusion")
        file_pointer.write("\n									  NO => Runs Radar Algorithms without Fusion -->")
        file_pointer.write("\n	<RADAR_FUSION_ENABLED>" + HIL_Data.Radar_fusion_EN + "</RADAR_FUSION_ENABLED>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!-- KPI_OUTPUT -- To enable statistics for HiL execution, available options are")
        file_pointer.write("\n		DISABLED: KPI will not be printed after successful HiL execution")
        file_pointer.write("\n		ENABLED: KPI will be printed after successful HiL execution -->")
        file_pointer.write("\n	<KPI_OUTPUT>" + ENET_data.KPI_Security_Key + "</KPI_OUTPUT> ")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!-- RESIM_EXECUTION_MODE")
        file_pointer.write(
            "\n		NORMAL: Resim.exe will exit if initial connection with sensors is not established.")
        file_pointer.write("\n		IDLE: Resim.exe will go into idle mode and will stay operational -->")
        file_pointer.write("\n	<RESIM_EXECUTION_MODE>" + HIL_Data.Execution_Mode + "</RESIM_EXECUTION_MODE>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<!-- DEBUGGING_HIL")
        file_pointer.write(
            '\n		ENABLED : Resim.exe creates text file named as "HiL_Debugging.txt" and starts writing, debugging info to the file')
        file_pointer.write('\n		DISABLED: Resim.exe will not be create a "HiL_Debugging.txt" text file   -->')
        file_pointer.write("\n	<DEBUGGING_HIL>" + str(HIL_Data.Debug_Mode) + "</DEBUGGING_HIL>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<UDP_PORT_DATA>" + ENET_data.UDP_PORT_DATA + "</UDP_PORT_DATA>")
        file_pointer.write("\n	<UDP_PORT_XCP>" + ENET_data.UDP_PORT_XCP + "</UDP_PORT_XCP>")
        file_pointer.write("\n	<UDP_PORT_LOGGING>" + ENET_data.UDP_PORT_LOGGING + "</UDP_PORT_LOGGING>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<INTERFACE_PORT>" + ENET_data.INTERFACE_PORT + "</INTERFACE_PORT>")
        file_pointer.write("\n	")
        file_pointer.write(
            "\n	<!-- TOBJECT_INJECTION_TYPE parameter can be used to select the track injection based on the user input")
        file_pointer.write("\n		DSPACE: DSPACE ASM objects Injection")
        file_pointer.write("\n		INTERNAL: Objects injection from csv -->")
        file_pointer.write(
            "\n	<TOBJECT_INJECTION_TYPE>" + HIL_Data.TOBJECT_injection_type + "</TOBJECT_INJECTION_TYPE>	")
        file_pointer.write("\n	")
        file_pointer.write(
            "\n	<!-- To define the CUSTOMER for which IP Address and port number the SCALEXIO/Scene Generator,HIL_Engine is running. -->")
        file_pointer.write("\n     <HIL_ENGINE_IP>" + ENET_data.HIL_ENGINE_IP + "</HIL_ENGINE_IP>")
        file_pointer.write("\n	 ")
        file_pointer.write("\n    <SCALEXIO_IP>" + ENET_data.SCALEXIO_IP + "</SCALEXIO_IP>")
        file_pointer.write("\n	")
        file_pointer.write("\n	<SCALEXIO_PORT>" + ENET_data.SCALEXIO_PORT + "</SCALEXIO_PORT>")
        file_pointer.write("\n	")
        file_pointer.write(
            "\n	<!--To define which fault has to be injected :BLOCKAGE =>  Set the sensors to blockage")
        file_pointer.write(
            "\n												 :ALIGNMENT => Injects Alignment fault to the sensors ")
        file_pointer.write(
            "\n												 :NOTE      => Make the field empty if no faults need to be injected -->")
        file_pointer.write("\n	<FAULT_INJECTION></FAULT_INJECTION>")
        file_pointer.write("\n	")
        file_pointer.write(
            "\n		<SET_FAULT_FROM>" + ENET_data.SET_FAULT_FROM + "</SET_FAULT_FROM> <!-- Provide the scan-index from which fault has to be injected -->")
        file_pointer.write(
            "\n		<SET_FAULT_TILL>" + ENET_data.SET_FAULT_TILL + "</SET_FAULT_TILL> <!-- Provide the scan-index till which fault has to be injected -->")
        file_pointer.write("\n			")
        file_pointer.write(
            "\n		<INJECT_FAULT_SENSOR_RL>" + str(
                HIL_Data.INJECT_FAULT_SENSOR_RL) + "</INJECT_FAULT_SENSOR_RL> <!-- Provide 1 to enable fault injection for Rear Left sensor or make it empty-->")
        file_pointer.write(
            "\n		<INJECT_FAULT_SENSOR_RR>" + str(
                HIL_Data.INJECT_FAULT_SENSOR_RR) + "</INJECT_FAULT_SENSOR_RR> <!-- Provide 1 to enable fault injection for Rear Left sensor or make it empty-->	")
        file_pointer.write("\n	")
        file_pointer.write("\n</HIL_SRR5_Configuration>")
    progress_update(80)
    return


def Execute_HIL_Command(Logpath):
    exe_path_name = os.path.normpath('..\..\HIL\SRR5_HiL_Release\SRR_HiL_Resim.exe')
    exe_command = exe_path_name + " Hil_Configuration.xml " + Logpath
    print("exe_command = \n", exe_command)
    print("--------------")
    progress_update(100)
    t = threading.Thread(target=subprocess.call(exe_command))
    t.daemon = True
    t.start()
    t.join(10)
    print("exe_command = ", exe_command)
    print("Global_var_class_obj.log_path :\n", Global_var_class_obj.log_path)
    return

def progress_update(val):
    HIL_widgets_obj.progress_bar['value'] = val
    root.update_idletasks()
    return