from tkinter import *
from tkinter import ttk
import threading
from datetime import *
import os
from Main import *
from main_interface import *
from subprocess import *
from io import *
from tkinter import messagebox
from tkinter import scrolledtext
from sys import *

from Bordnet_Tool_spec_widgets import *
from Bordnet_XML_Writer import *
import subprocess


def Bordnet_JSON(input_logfile_list, output_logfile_list):
    with io.open('BORDNET_fList.json', 'w+', encoding='cp1252') as file_pointer:
        file_pointer.write('\n{ ')
        file_pointer.write('\n	"reprocessingInputFileStreams": [')
        file_pointer.write('\n		{')
        file_pointer.write('\n			"key": "INPUT",')
        file_pointer.write('\n			"files": [')
        for inp_files in input_logfile_list:
            if inp_files != input_logfile_list[-1]:
                file_pointer.write('\n                "' + str(inp_files) + '",')
            else:
                file_pointer.write('\n                "' + str(inp_files) + '"')
        file_pointer.write('\n')
        file_pointer.write('\n			]')
        file_pointer.write('\n		},')
        file_pointer.write('\n		{')
        file_pointer.write('\n			"key": "OUTPUT",')
        file_pointer.write('\n			"files": [')
        for out_files in output_logfile_list:
            if out_files != output_logfile_list[-1]:
                file_pointer.write('\n                "' + str(out_files) + '",')
            else:
                file_pointer.write('\n                "' + str(out_files) + '"')
        file_pointer.write('\n')
        file_pointer.write('\n			]')
        file_pointer.write('\n		}')
        file_pointer.write('\n	]')
        file_pointer.write('\n}')
        file_pointer.write('\n')
        file_pointer.write('\n')


Bordnet_output_data = []


def Display_Bordnet_Process():
    Label(text='Bordnet Execution:', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=300, y=515)
    global Progress_Textbox
    # Progress_Textbox = Text(height=3, width=60, xscrollcommand=xscroll.set, yscrollcommand=yscroll.set, wrap=NONE)
    Progress_Textbox = scrolledtext.ScrolledText(root, height=2, width=85, wrap=NONE)
    # Progress_Textbox.tag_config("OUTPUTFile path is not provied in the BORDNET_fList.json.......", background="black", foreground="yellow")
    Progress_Textbox.place(x=450, y=500)
    for lines in Bordnet_output_data:
        Progress_Textbox.insert(END, lines)


def Bordnet_Tool_Execution():
    """exe_path_name = os.path.normpath(
        "../../../10028634_01_SRR_RESIM_Release/RESIM_Toolset/Bordnet_Tool/Linux/BordNetDecoder.exe")
    exe_command = exe_path_name + " bordnet_config.xml "
    print("exe_command = \n", exe_command)
    print("--------------")
    t = threading.Thread(target=subprocess.call(exe_command))
    t.daemon = True
    t.start()
    print("exe_command = ", exe_command)
    """
    exe_path_name = os.path.normpath("../../RESIM_Toolset/Bordnet_Tool/Windows/BordNetDecoder.exe")
    # exe_path_name = os.startfile("../../../10028634_01_SRR_RESIM_Release/RESIM_Toolset/Bordnet_Tool/Windows/BordNetDecoder.exe")
    exe_command = exe_path_name + " bordnet_config.xml "
    print("exe_command = ", exe_command)
    print("--------------")
    p = subprocess.Popen(exe_command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, bufsize=1,
                         universal_newlines=True)
    Bordnet_output_data.clear()
    for line in iter(p.stdout.readline, ''):
        Bordnet_output_data.append(line)
    print("exe_command = ", exe_command)
    messagebox.showinfo(title='Bordnet Tool', message='BORDNET Execution is Completed')
    Display_Bordnet_Process()
    return
