from tkinter import *
from tkinter import messagebox
import threading
import os
from datetime import date
from os import system
import subprocess
import io
import multiprocessing
from main_interface import *

global MUDP_output_text
global MUDP_text_box_label


def Write_XML(Sensor_and_ECU_obj, stream_options_obj, Global_var_class_obj, Mode_list, report_format1, file_mode1, filelist):
    with open('MUDP_DATA_Extracter_config_v2p0.xml', 'w+', encoding='cp1252') as file_pointer:
        file_pointer.write('<!-- <?xml version="1.0" encoding="UTF-8" ?>')
        file_pointer.write("\n<!-- Delphi SIL component configuration -->")
        file_pointer.write("\n<DATA_Extracter_Configuration>")
        file_pointer.write("\n	<!--DATA_Extracter_Config_XML_Version -->")
        file_pointer.write("\n	<!-- Version X.Y where:")
        file_pointer.write("\n		X - major change where some new parameters were added")
        file_pointer.write("\n		Y - minor change with no influence on parsing (comment or value change) -->")
        file_pointer.write("\n	<DATA_Extracter_Config_XML_Version>2.0</DATA_Extracter_Config_XML_Version>")
        file_pointer.write("\n	<!-- Sensor selection values: ")
        file_pointer.write("\n	1: Resim is executed for that Sensor Position")
        file_pointer.write("\n	0: Resim is not executed for that Sensor Position -->")
        file_pointer.write("\n  <SENSOR_STATUS>")
        file_pointer.write("\n    <REAR_LEFT>" + str(Sensor_and_ECU_obj.REAR_LEFT.get()) + "</REAR_LEFT>")
        file_pointer.write("\n    <REAR_RIGHT>" + str(Sensor_and_ECU_obj.REAR_RIGHT.get()) + "</REAR_RIGHT>")
        file_pointer.write("\n    <FRONT_RIGHT>" + str(Sensor_and_ECU_obj.FRONT_RIGHT.get()) + "</FRONT_RIGHT>")
        file_pointer.write("\n    <FRONT_LEFT>" + str(Sensor_and_ECU_obj.FRONT_LEFT.get()) + "<FRONT_LEFT>")
        file_pointer.write("\n    <BP_RIGHT>" + str(Sensor_and_ECU_obj.BP_RIGHT.get()) + "</BP_RIGHT>")
        file_pointer.write("\n    <BP_LEFT>" + str(Sensor_and_ECU_obj.BP_LEFT.get()) + "</BP_LEFT>")
        file_pointer.write("\n	<RADAR_ECU>" + str(Sensor_and_ECU_obj.RADAR_ECU.get()) + "</RADAR_ECU>")
        file_pointer.write("\n  </SENSOR_STATUS>")
        file_pointer.write("\n	<!-- Output_Stream_Options available are :")
        file_pointer.write("\n		HDR : dump mudp stream header information")
        file_pointer.write("\n		CDC: dump cdc stream output data")
        file_pointer.write("\n		z4 Core: dump z4(Core0) stream output data ")
        file_pointer.write("\n		Z7A Core: dump z7A(Core1) stream output data ")
        file_pointer.write("\n		Z7B Core: dump z7B(Core2) stream output data")
        file_pointer.write("\n		Z4 Customer: dump z4(Customer) stream output data")
        file_pointer.write("\n		Z7B Customer: dump z7b(Customer) stream output data --> ")
        file_pointer.write("\n	<Output_Stream_Option>")
        file_pointer.write("\n	    <HDR>" + str(stream_options_obj.STREAM_HEADER.get()) + "</HDR>")
        file_pointer.write("\n		<CDC>" + str(stream_options_obj.CDC.get()) + "</CDC>")
        file_pointer.write("\n        <DSPACE>" + str(stream_options_obj.DSPACE.get()) + "</DSPACE>")
        file_pointer.write("\n		<CCA_HDR>" + str(stream_options_obj.CCA_HDR.get()) + "</CCA_HDR>")
        file_pointer.write("\n        <OSI_STREAM>" + str(stream_options_obj.OSI_STREAM.get()) + "</OSI_STREAM>")
        file_pointer.write("\n		<Z4_Core>" + str(stream_options_obj.Z4_CORE.get()) + "</Z4_Core>")
        file_pointer.write("\n		<Z7A_Core>" + str(stream_options_obj.Z7A_CORE.get()) + "</Z7A_Core>")
        file_pointer.write("\n		<Z7B_Core>" + str(stream_options_obj.Z7B_CORE.get()) + "</Z7B_Core>")
        file_pointer.write("\n	    <Z4_Customer>" + str(stream_options_obj.Z4_CUST.get()) + "</Z4_Customer>")
        file_pointer.write("\n	    <Z7B_Customer>" + str(stream_options_obj.Z7A_CUST.get()) + "</Z7B_Customer>")
        file_pointer.write("\n	</Output_Stream_Option>")
        file_pointer.write("\n	                                                      ")
        file_pointer.write("\n	                                                     ")
        file_pointer.write("\n	<!--RADAR_ECU_Stream_Options available are -")
        file_pointer.write("\n		ECU_Coreo:dump ECU0 output data")
        file_pointer.write("\n		ECU_Core1: dump ECU1 output data")
        file_pointer.write("\n		ECU_Core3: dump ECU3 output data -->")
        file_pointer.write("\n	<RADAR_ECU_Stream_Option>")
        file_pointer.write("\n		<ECU0>" + str(stream_options_obj.ECU_1.get()) + "</ECU0>")
        file_pointer.write("\n		<ECU1>" + str(stream_options_obj.ECU_2.get()) + "</ECU1>")
        file_pointer.write("\n		<ECU3>" + str(stream_options_obj.ECU_3.get()) + "</ECU3>")
        file_pointer.write("\n        <ECU_VRU_Classifier>" + str(stream_options_obj.ECU_VRU_CLASSIFIER.get()) +
                           "</ECU_VRU_Classifier>")
        file_pointer.write("\n	</RADAR_ECU_Stream_Option>")
        file_pointer.write("\n	                              ")
        file_pointer.write("\n	<!-- Resim output path selection. Options are SAME_AS_INPUT or NEW_OUTPUT_PATH -->")
        file_pointer.write(
            "\n         <Output_Path_Options>" + Global_var_class_obj.output_path + "</Output_Path_Options>")
        file_pointer.write(
            "\n    <!-- If resim output path selection is NEW_OUTPUT_PATH use the below tag to enter valid folder path -->")
        if Global_var_class_obj.output_path == 'SAME_AS_INPUT':
            file_pointer.write("\n		 <Output_Path_Location>NONE</Output_Path_Location>")
        elif Global_var_class_obj.output_path == 'NEW_OUTPUT_PATH':
            file_pointer.write("\n		 <Output_Path_Location>" + str(filelist[0]) + "</Output_Path_Location>")
        file_pointer.write("\n	                                               ")
        file_pointer.write("\n    <!-- Extractor User Options : ")
        file_pointer.write("\n         0 : Does not  Creates Embedded Lib UDP XML for HIL_PORT files ")
        file_pointer.write("\n         1 : Creates Embedded Lib UDP XML for HIL_PORT files  -->")
        file_pointer.write("\n                                     ")
        file_pointer.write("\n    <output_xml_trace_option>")
        file_pointer.write("\n		 <HIL_port_xml>" + str(Global_var_class_obj.HIL_port_xml) + "</HIL_port_xml>")
        file_pointer.write("\n		 <Sensor_xml>" + str(Global_var_class_obj.Sensor_xml) + "</Sensor_xml>")
        file_pointer.write("\n    </output_xml_trace_option>")
        file_pointer.write("\n    <!-- Data_Extraction_Mode User Options :		")
        file_pointer.write("\n        CSV_MODE:")
        file_pointer.write("\n				0 : Doesnot Extract the Input Log data into the CSV .")
        file_pointer.write("\n				1 :  Extract the Imput Log data into the CSV in the input log path according to the stream enabled.")
        file_pointer.write("\n	    XML_TRACE_MODE:")
        file_pointer.write("\n				0 : Doesn't generate the input xml_traces.")
        file_pointer.write("\n				1 : Generates the xml_traces in the MUDP_XML_TRACE folder in the log path.")
        file_pointer.write("\n	    Detail_Error_Info:")
        file_pointer.write("\n				0 : Doesn't generate the detailed error info of packetloss report.")
        file_pointer.write("\n				1: Generates only detailed error info of  packet_loss report.")
        file_pointer.write("\n	    PACKET_LOSS_STATISTICS :")
        file_pointer.write("\n	            0 : Does not prints  prints Packet loss statistics")
        file_pointer.write("\n              1 :  prints Packet loss statistics + Summery Table at the end")
        file_pointer.write("\n	    Log_Quality_Summary  :")
        file_pointer.write("\n              This tag is BMW Customer use only. do not use this tag  for extracting csv and packetloss report.")
        file_pointer.write("\n				            -->")
        file_pointer.write("\n    <Data_Extraction_Mode>")
        file_pointer.write("\n			<CSV_MODE>" + str(Mode_list[0]) + "</CSV_MODE>")
        file_pointer.write(
            "\n			<XML_TRACE_MODE>" + str(Mode_list[1]) + "</XML_TRACE_MODE>")
        file_pointer.write(
            "\n			<Detail_Error_Info>" + str(Mode_list[2]) + "</Detail_Error_Info>")
        file_pointer.write(
            "\n			<PACKET_LOSS_STATISTICS>" + str(Mode_list[3]) + "</PACKET_LOSS_STATISTICS>")
        file_pointer.write(
            "\n			<Log_Quality_Summary>" + str(Mode_list[4]) + "</Log_Quality_Summary>")
        file_pointer.write("\n    </Data_Extraction_Mode>")
        file_pointer.write("\n <!-- Extractor User Options :")
        file_pointer.write("\n        text_fomat-> Packet loss report will generate in .txt format")
        file_pointer.write("\n		 xml_format-> Packet loss report will generate in .xml format -->")
        file_pointer.write("\n		 <PACKET_LOSS_Report_Format>" + str(report_format1) + "</PACKET_LOSS_Report_Format>")
        file_pointer.write("\n <!-- LOG REPLAY mode selection: User shall provide the list of filenames the RESIM should execute in the Input_Log.txt file")
        file_pointer.write("\n		Sequential_FILE_Input -  RESIM shall execute in a sequential order of the Filenames provided")
        file_pointer.write("\n								 Ensuring that each input file is processed uniquely by resetting the memory after each file.")
        file_pointer.write("\n								 Hence history is not maintained between the input files.")
        file_pointer.write("\n   	Continuous_FILE_Input -  RESIM shall execute in a sequential order of the Filenames provided")
        file_pointer.write("\n								 Ensuring that each input file is processed in continuity between the files.")
        file_pointer.write("\n								 The memory is NOT reset after each file. Hence history is maintained between the input files.")
        file_pointer.write("\n								 Please ensure that the Logs are collected in a continuous manner from the Vehicle Expedition to use this mode")
        file_pointer.write("\n								 This Mode provides results with more correlation with actual Sensor behaviour")
        file_pointer.write("\n								 User is responsible to make sure that the file list is sequential the RESIM shall not check this.")
        file_pointer.write("\n								 USE THIS MODE WITH CAUTION -->")
        file_pointer.write("\n	<INPUT_FILE_MODE>" + str(file_mode1) + "</INPUT_FILE_MODE>")
        file_pointer.write("\n		<!-- Enable anyone of the library which needs to be integrated -->")
        file_pointer.write("\n	<LIBRARY_INTEGRATED>")
        file_pointer.write("\n		<CCA_Library>" + str(Global_var_class_obj.CCA_Library) + "</CCA_Library>")
        file_pointer.write(
            "\n	    <Vector_Library>" + str(Global_var_class_obj.Vector_Library) + "</Vector_Library>")
        file_pointer.write("\n	</LIBRARY_INTEGRATED>")
        file_pointer.write("\n		")
        file_pointer.write("\n</DATA_Extracter_Configuration>")
    return


def MUDP_text_box(display_MUDP_output):
    global MUDP_output_text
    global MUDP_text_box_label
    MUDP_text_box_label = Label(text="Progress:", font=('arial', 10, 'bold'), bg='slate gray1').place(x=490, y=460)
    MUDP_output_text = Text(root, height=3, width=70)
    MUDP_output_text.place(x=570, y=440)
    MUDP_output_text.insert(END, display_MUDP_output)
    MUDP_textbox_scroll = Scrollbar(root)
    MUDP_textbox_scroll.config(command=MUDP_output_text.yview)
    MUDP_textbox_scroll.place(x=1133, y=440)


def Execute_Command(Exe_Path, bn_califr_MUDP_log_list, faseth_MUDP_log_list, srr_debug_MUDP_log_list, srr_reference_MUDP_log_list):
    global exe_command
    exe_path_name = os.path.normpath("../../RESIM_Toolset\MUDP_LogData_Extractertool\MUDP_Log_DataExtracter.exe")
    if len(bn_califr_MUDP_log_list) == len(faseth_MUDP_log_list) == len(srr_debug_MUDP_log_list) == len(srr_reference_MUDP_log_list):
        exe_command = exe_path_name + " MUDP_DATA_Extracter_config_v2p0.xml " + "MUDP_json.json"
    elif len(bn_califr_MUDP_log_list and faseth_MUDP_log_list and srr_reference_MUDP_log_list) == 0 and len(srr_debug_MUDP_log_list) >= 1:
        exe_command = exe_path_name + " MUDP_DATA_Extracter_config_v2p0.xml " + "MUDP_flist.txt"
    else:
        messagebox.showerror(title='Execution Error',
                             message='!!! PLEASE BROWSE THE LOG FILES, Create Json/ Flist !!!')
    print("exe_command = \n", exe_command)
    print("--------------")
    # t = threading.Thread(target=subprocess.call(exe_command))
    # t.daemon = True
    # t.start()
    # t.join(10)
    p = subprocess.Popen(exe_command, stdout=subprocess.PIPE, shell=True)
    global display_MUDP_output
    display_MUDP_output = p.communicate()
    MUDP_text_box(display_MUDP_output)
    print("exe_command = ", exe_command)
    messagebox.showinfo(title='MUDP Extracter Tool', message='MUDP Extracter Tool Run Completed Successfully')
    # print("Global_var_class_obj.log_path :\n", Global_var_class_obj.log_path)
    return
