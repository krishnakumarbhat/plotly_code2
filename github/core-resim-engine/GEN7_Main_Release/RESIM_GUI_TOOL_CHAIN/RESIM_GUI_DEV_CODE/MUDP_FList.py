import os
from tkinter import *
from threading import *
from datetime import *
from os import *
from subprocess import *
from io import *
from Packet_Loss_Tool_spec_widgets import *


def MUDP_flist_gen(srr_debug_log_list):
    with io.open('MUDP_flist.txt', 'w+', encoding='cp1252') as file_pointer:
        for c in srr_debug_log_list:
            file_pointer.write(str(c) + "\n")
    return
