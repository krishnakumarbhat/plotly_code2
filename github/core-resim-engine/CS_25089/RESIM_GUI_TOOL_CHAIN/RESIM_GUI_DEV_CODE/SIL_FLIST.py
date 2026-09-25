import os
from tkinter import *
from threading import *
from datetime import *
from os import *
from subprocess import *
from io import *
from SIL_TooL_spec_widgets import *


def SIL_flist_gen(srr_debug_log_SIL_list):
    with open('SIL_flist.txt', 'w+', encoding='cp1252') as file_pointer:
        for c in srr_debug_log_SIL_list:
            file_pointer.write(str(c) + "\n")
    return
