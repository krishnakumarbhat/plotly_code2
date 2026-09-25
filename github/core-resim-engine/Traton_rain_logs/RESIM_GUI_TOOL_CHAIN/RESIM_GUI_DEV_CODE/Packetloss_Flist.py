import os
from tkinter import *
from threading import *
from datetime import *
from os import *
from subprocess import *
from io import *
from Packet_Loss_Tool_spec_widgets import *


def flist_gen(srr_debug_log_list):
    with open('packetloss_flist.txt', 'w+', encoding='cp1252') as file_pointer:
        for c in srr_debug_log_list:
            file_pointer.write(str(c) + "\n")
    return


def flist_path():
    global flabel
    global fname
    global fpath
    fpath = os.path.abspath('packetloss_flist.txt')
    flabel = Label(text='FLIST Path:', font=('arial', 10, 'bold'), bg='slate gray1').place(x=30, y=330)
    fname = Label(text=fpath, font=('arial', 10, 'bold'), bg='slate gray1').place(x=130, y=330)
