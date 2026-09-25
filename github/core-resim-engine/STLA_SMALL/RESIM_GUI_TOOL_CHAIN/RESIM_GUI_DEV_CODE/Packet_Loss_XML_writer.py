from tkinter import *
from tkinter import messagebox
from tkinter import filedialog
import threading
import multiprocessing
import os
import io
from datetime import date
from os import system
import subprocess
from main_interface import *
from Packet_Loss_Tool_spec_widgets import *
from Packet_Loss_Json_writer import *

global output_text
global text_box_label
global exe_name


def Write_Packet_Loss_XML(input_value1, filelist):
    with io.open('Packet_Loss_config.xml', 'w+', encoding='cp1252')as file_pointer:
        file_pointer.write('<!-- <?xml version="1.0" encoding="UTF-8" ?>')
        file_pointer.write("\n<!-- Delphi SIL component configuration -->")
        file_pointer.write("\n<DATA_Extracter_Configuration>")
        file_pointer.write("\n  <!--DATA_Extracter_Config_XML_Version -->")
        file_pointer.write("\n  <!-- Version X.Y where:")
        file_pointer.write("\n		X - major change where some new parameters were added")
        file_pointer.write("\n		Y - minor change with no influence on parsing (comment or value change) -->")
        file_pointer.write("\n	<DATA_Extracter_Config_XML_Version>2.0</DATA_Extracter_Config_XML_Version>")
        file_pointer.write("\n	<!-- Sensor selection values: ")
        file_pointer.write("\n	1: Resim is executed for that Sensor Position")
        file_pointer.write("\n	0: Resim is not executed for that Sensor Position -->")
        file_pointer.write(
            "\n  <!-- Resim output path selection. Options are SAME_AS_INPUT or NEW_OUTPUT_PATH or SAME_AS_OUTPUT")
        file_pointer.write(
            "\n  SAME_AS_OUTPUT : This is Optional . Files will generated path provided in the third command line argument	-->")
        file_pointer.write("\n <Output_Path_Options>" + str(input_value1) + "</Output_Path_Options>")
        file_pointer.write(
            "\n<!-- If resim output path selection is NEW_OUTPUT_PATH use the below tag to enter valid folder path -->")
        if input_value1 == 'SAME_AS_INPUT' or input_value1 == 'SAME_AS_OUTPUT':
            file_pointer.write("\n  <Output_Path_Location>NONE</Output_Path_Location>")
        elif input_value1 == 'NEW_OUTPUT_PATH':
            file_pointer.write("\n  <Output_Path_Location>" + str(filelist[0]) + "</Output_Path_Location>")
        file_pointer.write("\n                                                      ")
        file_pointer.write("\n                                                      ")
        file_pointer.write("\n                                                      ")
        file_pointer.write("\n <Data_Extraction_Mode>")
        file_pointer.write("\n     <Log_Quality_Summary>1</Log_Quality_Summary>")
        file_pointer.write("\n </Data_Extraction_Mode>")
        file_pointer.write("\n                                                      ")
        file_pointer.write("\n                                                      ")
        file_pointer.write("\n</DATA_Extracter_Configuration>")
    return


def xml_path():
    global xlabel
    global xname
    xpath = os.path.abspath('Packet_Loss_config.xml')
    xlabel = Label(text='XML Path:', font=('arial', 10, 'bold'), bg='slate gray1').place(x=30, y=360)
    xname = Label(text=xpath, font=('arial', 10, 'bold'), bg='slate gray1').place(x=130, y=360)


def text_box(display):
    global output_text
    global text_box_label
    text_box_label = Label(text="Progress:", font=('arial', 10, 'bold'), bg='slate gray1').place(x=30, y=510)
    output_text = Text(root, height=3)
    output_text.place(x=130, y=500)
    output_text.insert(END, display)
    textbox_scroll = Scrollbar(root)
    textbox_scroll.config(command=output_text.yview)
    textbox_scroll.place(x=759, y=500)


def execute_packet_loss_command(bn_califr_log_list, faseth_log_list, srr_debug_log_list, srr_reference_log_list):
    global exe_name
    global exe_label
    global exe_command
    global p
    exe_path = os.path.abspath('MUDP_Log_DataExtracter.exe')
    exe_path_name = os.path.normpath(
        "../../RESIM_Toolset/MUDP_LogData_Extractertool/MUDP_Log_DataExtracter.exe")
    exe_name = Label(text='EXE File:', font=('arial', 10, 'bold'), bg='slate gray1').place(x=20, y=390)
    exe_label = Label(text=exe_path, font=('arial', 10, 'bold'), bg='slate gray1').place(x=150, y=390)
    if len(bn_califr_log_list) == len(faseth_log_list) == len(srr_debug_log_list) == len(srr_reference_log_list):
        exe_command = exe_path_name + " Packet_Loss_config.xml " + " packetloss_json.json "
    elif len(bn_califr_log_list and faseth_log_list and srr_reference_log_list) == 0 and len(srr_debug_log_list) >= 1:
        exe_command = exe_path_name + " Packet_Loss_config.xml " + " packetloss_flist.txt "
    else:
        messagebox.showerror(title='Execution Error', message='!!! PLEASE BROWSE THE LOG FILES, Create Json/ Flist and xml file !!!')
    print("exe_command = \n", exe_command)
    print("--------------")
    # progress_update(100)
    """t = threading.Thread(target=subprocess.call(exe_command))
    t.daemon = True
    t.start()"""
    p = subprocess.Popen(exe_command, stdout=subprocess.PIPE, shell=True)
    # multiprocessing.freeze_support()
    global display
    display = p.communicate()
    text_box(display)
    messagebox.showinfo(title='Packet Loss Tool', message='!Packet Loss Tool Run Completed Successfully')
    print("exe_command = ", exe_command)
    return
