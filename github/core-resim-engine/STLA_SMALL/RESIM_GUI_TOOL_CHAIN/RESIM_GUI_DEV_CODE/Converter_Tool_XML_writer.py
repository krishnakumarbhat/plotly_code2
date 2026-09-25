from tkinter import *
import threading
from datetime import *
import os
import subprocess
import io
from multiprocessing import Process, Manager
from tkinter import messagebox
from typing import io
from Main import *
from main_interface import *
from Converter_Tool_spec_widgets import *
from Converter_Tool_JSON_writer import *


def Converter_XML_writer(inp_values_list, opt_values_list, other_values_list, json_file_list, path_inp_list, pathlist):
    with io.open('Mdf4_Converter_Config.xml', 'w+', encoding='cp1252') as file_pointer:
        file_pointer.write('\n<?xml version="1.0" encoding="UTF-8" ?>')
        file_pointer.write('\n<!--  APTIV SIL component configuration -->')
        file_pointer.write('\n<CONVERTOR_CONFIGURATION>')
        file_pointer.write('\n  <!-- SIL SRR5 Configuration XML version -->')
        file_pointer.write('\n  <!-- Version X.Y where:')
        file_pointer.write('\n		X - major change where some new parameters were added')
        file_pointer.write('\n		Y - minor change with no influence on parsing (comment or value change) -->')
        file_pointer.write('\n  <CONVERTOR_CONFIGURATION_XML_VERSION>1.7</CONVERTOR_CONFIGURATION_XML_VERSION>')
        file_pointer.write('\n ')
        file_pointer.write('\n<!-- Input Mode Options available are -')
        file_pointer.write('\n	ORCAS_MDF4_INPUT : Input to the convertor will be ORCAS_MDF4')
        file_pointer.write('\n	CANAPE_MDF4_INPUT: Input to the convertor will be CANAPE_MDF4')
        file_pointer.write('\n	VIGEM_MDF4_INPUT : Input to the convertor will be VIGEM_MDF4')
        file_pointer.write('\n	VIGEM_VPCAP_INPUT: Input to the convertor will be VIGEM_VPCAP_MDF4')
        file_pointer.write('\n	RESIM_VIGEM_MDF4_INPUT: Input to the convertor will be resimulated SOMEIP VIGEM_MDF4	-->')
        file_pointer.write('\n ')
        file_pointer.write('\n	<CONVERTOR_INPUT_FILE_OPTION>')
        file_pointer.write('\n		<ORCAS_MDF4_INPUT_ENABLE>' + str(inp_values_list[0]) + '</ORCAS_MDF4_INPUT_ENABLE>')
        file_pointer.write('\n		<PCAP_INPUT_ENABLE>' + str(inp_values_list[1]) + '</PCAP_INPUT_ENABLE>')
        file_pointer.write('\n 		<AUTERA_INPUT_ENABLE>' + str(inp_values_list[2]) + '</AUTERA_INPUT_ENABLE>')
        file_pointer.write('\n		<CANAPE_MDF4_INPUT_ENABLE>' + str(inp_values_list[3]) + '</CANAPE_MDF4_INPUT_ENABLE>')
        file_pointer.write('\n		<CANAPE_CAN_MDF4_INPUT_ENABLE>' + str(inp_values_list[4]) + '</CANAPE_CAN_MDF4_INPUT_ENABLE>')
        file_pointer.write('\n		<VIGEM_MDF4_INPUT_ENABLE>' + str(inp_values_list[5]) + '</VIGEM_MDF4_INPUT_ENABLE>')
        file_pointer.write('\n		<VIGEM_VPCAP_INPUT_ENABLE>' + str(inp_values_list[6]) + '</VIGEM_VPCAP_INPUT_ENABLE>')
        file_pointer.write('\n		<X2E_INPUT_ENABLE>' + str(inp_values_list[7]) + '</X2E_INPUT_ENABLE>')
        file_pointer.write(
            '\n		<RESIM_VIGEM_MDF4_INPUT_ENABLE>' + str(inp_values_list[8]) + '</RESIM_VIGEM_MDF4_INPUT_ENABLE>')
        file_pointer.write('\n	</CONVERTOR_INPUT_FILE_OPTION>')
        file_pointer.write('\n ')
        file_pointer.write('\n	<!-- Output Mode Options available are -')
        file_pointer.write('\n 	SINGLE_DVSU_OUTPUT : Output will be single_dvsu file')
        file_pointer.write('\n 	FOUR_DVSU_OUTPUT   : Output will be Four dvsu files')
        file_pointer.write('\n 	ORCAS_MDF4_OUTPUT  : Output will be ORCAS_MDF4_OUTPUT file')
        file_pointer.write('\n 	CANoe_MDF4_OUTPUT  : Output will be CANoe or CANAPE MDF4 file')
        file_pointer.write('\n 	VIGEM_MDF4_OUTPUT  : Output will be ViGEM MDF4')
        file_pointer.write('\n 	VIGEM_SOMEIP_OUTPUT: Output will be CANOE SOMEIP MDF4')
        file_pointer.write('\n 	VIGEM_VPCAP_OUTPUT : Output will be ViGEM VPCAP MDF4')
        file_pointer.write('\n	VIDEO_OUTPUT       : Output will be video file')
        file_pointer.write('\n-->	')
        file_pointer.write('\n	<CONVERTOR_OUTPUT_FILE_OPTION>')
        file_pointer.write(
            '\n		<SINGLE_DVSU_OUTPUT_ENABLE>' + str(opt_values_list[0]) + '</SINGLE_DVSU_OUTPUT_ENABLE>')
        file_pointer.write('\n		<FOUR_DVSU_OUTPUT_ENABLE>' + str(opt_values_list[1]) + '</FOUR_DVSU_OUTPUT_ENABLE>')
        file_pointer.write('\n		<ORCAS_MDF4_OUTPUT_ENABLE>' + str(opt_values_list[2]) + '</ORCAS_MDF4_OUTPUT_ENABLE>')
        file_pointer.write('\n		<CANoe_MDF4_OUTPUT_ENABLE>' + str(opt_values_list[3]) + '</CANoe_MDF4_OUTPUT_ENABLE>')
        file_pointer.write('\n		<VIGEM_MDF4_OUTPUT_ENABLE>' + str(opt_values_list[4]) + '</VIGEM_MDF4_OUTPUT_ENABLE>')
        file_pointer.write(
            '\n		<VIGEM_SOMEIP_OUTPUT_ENABLE>' + str(opt_values_list[5]) + '</VIGEM_SOMEIP_OUTPUT_ENABLE>')
        file_pointer.write(
            '\n		<VIGEM_VPCAP_OUTPUT_ENABLE>' + str(opt_values_list[6]) + '</VIGEM_VPCAP_OUTPUT_ENABLE>')
        file_pointer.write('\n		<VIDEO_OUTPUT_ENABLE>' + str(opt_values_list[7]) + '</VIDEO_OUTPUT_ENABLE>')
        file_pointer.write('\n	</CONVERTOR_OUTPUT_FILE_OPTION>')
        file_pointer.write('\n ')
        file_pointer.write('\n ')
        file_pointer.write(
            '\n	<!-- CONVERT_ONLY_SRR_DEBUG_FILES = 1 : Input is taken from only the SRR_DEBUG folder and converts accordingly')
        file_pointer.write('\n	     CONVERT_ONLY_SRR_DEBUG_FILES = 0 : Input is taken from CALIFR,FASETH,SRR_REFERENCE and the SRR_DEBUG folder and converts accordingly-->')
        file_pointer.write(
            '\n	<CONVERT_ONLY_SRR_DEBUG_FILES>' + str(other_values_list[0]) + '</CONVERT_ONLY_SRR_DEBUG_FILES>')
        file_pointer.write('\n ')
        file_pointer.write('\n	<!-- MERGE_CONVERTED_DATA = MERGE_ALL_TRACES_TO_SINGLE_FILE             : Merge All records to single file.')
        file_pointer.write(
            '\n	     MERGE_CONVERTED_DATA = MERGE_RESPECTIVE_STREAMS_TO_INDIVIDUAL_FILE : Merge All Califr to single Califr, All FASETH to single FASETH, ALL SRR_DEBUG to single SRR_DEBUG, All SRR_REF to Single SRR_REF-->')
        if other_values_list[1] == 0:
            file_pointer.write('\n	<MERGE_CONVERTED_DATA>MERGE_RESPECTIVE_STREAMS_TO_INDIVIDUAL_FILE</MERGE_CONVERTED_DATA>')
        else:
            file_pointer.write('\n	<MERGE_CONVERTED_DATA>MERGE_ALL_TRACES_TO_SINGLE_FILE</MERGE_CONVERTED_DATA>')
        file_pointer.write('\n ')
        file_pointer.write('\n	<!-- CSV_ENABLE = 1 : CSV files will be generated along with the converted file output. Csv contains header data info of log')
        file_pointer.write('\n	     CSV_ENABLE = 0 : No CSV files will get generated along with the converted file output-->')
        file_pointer.write('\n	<CSV_ENABLE>' + str(other_values_list[2]) + '</CSV_ENABLE>')
        file_pointer.write('\n ')
        file_pointer.write('\n	<!--Output Path Options')
        file_pointer.write('\n 	SAME_AS_INPUT                  - Output will be created in the path of SRR_DEBUG file')
        file_pointer.write('\n	OUTPUT_INSIDE_CONVERTED_FOLDER - CONVERTED folder will be created inside SRR DEBUG folder.')
        file_pointer.write('\n	C:/test/                       - User Defined Output path')
        file_pointer.write('\n	-->')
        if path_inp_list[0] == "SAME_AS_INPUT":
            file_pointer.write('\n	<CONVERTOR_OUTPUT_PATH>SAME_AS_INPUT</CONVERTOR_OUTPUT_PATH>')
        elif path_inp_list[0] == "CONVERTED":
            file_pointer.write('\n	<CONVERTOR_OUTPUT_PATH>OUTPUT_INSIDE_CONVERTED_FOLDER</CONVERTOR_OUTPUT_PATH>')
        else:
            file_pointer.write('\n	<CONVERTOR_OUTPUT_PATH>' + str(pathlist[0]) + '</CONVERTOR_OUTPUT_PATH>')
        file_pointer.write('\n ')
        file_pointer.write(
            '\n	<!--IGNORECDC = TRUE    --   The CDC content from VIGEM input wont be written in the Output')
        file_pointer.write(
            '\n	    IGNORECDC = FALSE    --  The CDC content from VIGEM input will be written in the Output-->')
        file_pointer.write('\n	<IGNORECDC>' + str(other_values_list[3]) + '</IGNORECDC>')
        file_pointer.write('\n	</CONVERTOR_CONFIGURATION>')
    return

Output_Data = []


def Display_Process():
    Output_Textbox_Label = Label(text="Conversion Log:", font=('arial', 10, 'bold'), bg=Bg_colour)
    Output_Textbox_Label.place(x=400, y=480)
    global Progress_Textbox
    Progress_Textbox = scrolledtext.ScrolledText(root, height=5, width=75, wrap=tk.WORD)
    Progress_Textbox.place(x=520, y=460)
    for lines in Output_Data:
        Progress_Textbox.insert(END, lines)


def Execute_Converter_Command():
    exe_path_name = os.path.normpath("../../RESIM_Toolset/MDF4_Converter_Tool/mdf_udpData_Proc.exe")
    exe_command = exe_path_name + " Mdf4_Converter_Config.xml " + "BMW_fList.json"
    print("exe_command = \n", exe_command)
    print("--------------")
    p = subprocess.Popen(exe_command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, bufsize=1, universal_newlines=True)
    Output_Data.clear()
    for line in iter(p.stdout.readline, ''):
        Output_Data.append(line)
        sys.stdout.flush()
    # Converting_Label.place_forget()
    """Completed_Label = Label(root, text="Conversion Completed ", font=('arial', 11, 'italic', 'bold'), bg=Bg_colour)
    Completed_Label.place(x=260, y=460)"""
    p.wait()
    Display_Process()
    messagebox.showinfo(title='Converter Tool', message='Conversion Completed')
    return
