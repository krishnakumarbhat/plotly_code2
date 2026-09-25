from tkinter import *
from tkinter import messagebox
from tkinter import filedialog
import threading
import os
from datetime import date
from os import system
import subprocess
from Data_Crawler_Tool_spec_widgets import *
from main_interface import *
import io


def d_c_flist_gen(log_list):
    with io.open('flistdc.txt', 'w+', encoding='cp1252') as file_pointer:
        for c in log_list:
            file_pointer.write(str(c) + "\n")
    return


def text_box_Output(display_output):
    global output_text_data_crawler
    global text_box_Output_label
    text_box_Output_label = Label(text="Progress:", font=('arial', 10, 'bold'), bg='slate gray1')
    text_box_Output_label.place(x=20, y=300)
    output_text_data_crawler = Text(root, height=3)
    output_text_data_crawler.place(x=120, y=290)
    output_text_data_crawler.insert(END, display_output)
    textbox_scroll_opt = Scrollbar(root)
    textbox_scroll_opt.config(command=output_text_data_crawler.yview)
    textbox_scroll_opt.place(x=903, y=290)


def execute_data_crawler_command():
    global exe_name
    global exe_label
    global exe_command
    global flabel
    global fname
    global fpath
    fpath = os.path.abspath('flistdc.txt')
    flabel = Label(text='FLIST Path:', font=('arial', 10, 'bold'), bg='slate gray1').place(x=20, y=230)
    fname = Label(text=fpath, font=('arial', 10, 'bold'), bg='slate gray1').place(x=150, y=230)
    exe_path = os.path.abspath('APTIV_Data_Crawler.exe')
    exe_path_name = os.path.normpath(
        "../../RESIM_Toolset/APTIV_Data_Crawler_1.0.3/APTIV_Data_Crawler/APTIV_Data_Crawler.exe")
    exe_name = Label(text='EXE File:', font=('arial', 10, 'bold'), bg='slate gray1').place(x=20, y=250)
    exe_label = Label(text=exe_path, font=('arial', 10, 'bold'), bg='slate gray1').place(x=150, y=250)
    exe_command = exe_path_name + " flistdc.txt "
    print("exe_command = \n", exe_command)
    print("--------------")
    # progress_update(100)
    '''t = threading.Thread(target=subprocess.call(exe_command))
    t.daemon = True
    t.start()'''
    p = subprocess.Popen(exe_command, stdout=subprocess.PIPE, shell=True)
    # multiprocessing.freeze_support()
    global display_output
    display_output = p.communicate()
    text_box_Output(display_output)
    messagebox.showinfo(title='Data Crawler Tool', message='!Data Crawler Tool Run Completed Successfully')
    print("exe_command = ", exe_command)
    return
