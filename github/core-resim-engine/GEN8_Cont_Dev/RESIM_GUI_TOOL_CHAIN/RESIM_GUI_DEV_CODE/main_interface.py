from tkinter import *
from tkinter.ttk import *
import tkinter as tk
from tkinter.filedialog import *
from tkinter import messagebox
from sys import platform


# Create an object for Tkinter
global root
root = tk.Tk()
if platform == "linux" or platform == "linux2":
    Bg_colour = 'light blue'
else:
    Bg_colour = 'slate gray1'

s = Style()
s.theme_use('alt')
s.configure("green.Horizontal.TProgressbar", foreground='green', background='green', bg='green')

# global variables
offset_y = 0
offset_x = 0


# ------------------------------------#

"""---- Import Module ------- """

import MUDP_Tool_spec_widgets as M_Tool

""" ---------------------------- """


# root.configure(background='blue')
def Tool_info():
    # *APTIV* #
    red_dot_left = tk.Button(root, bg='red')
    red_dot_left.place(x=13, y=569, height=10, width=10, anchor=NW)
    red_dot_right = tk.Button(root, bg='red')
    red_dot_right.place(x=127 - offset_x, y=569, height=10, width=10, anchor=NW)

    # name = Label(root, text = "Creator : PRIYAN PRASAD", font = ('arial', 8, 'bold'), bg = Bg_colour)
    # name.place(x=800, y= 504,anchor = NW)
    help = Label(root, text="Help : khushnuma.ghazal@aptiv.com", font=('arial', 8, 'bold'), bg=Bg_colour)
    help.place(x=980, y=554, anchor=NW)
    # Helpg = Label(root, text="Help : khushnuma.ghazal@aptiv.com", font=('arial', 8, 'bold'), bg=Bg_colour)
    # Helpg.place(x=980, y=574, anchor=NW)
    Copyright = Label(root, text="Copyright 2018 APTIV Technologies ", font=('arial', 8, 'bold'), bg=Bg_colour)
    Copyright.place(x=460, y=554, anchor=NW)
    Release = Label(root, text="Release Date : 15-MARCH-2021", font=('arial', 8, 'bold'), bg=Bg_colour)
    Release.place(x=475, y=574, anchor=NW)
    return

def display_labels():
    # labels
    APTIV = Label(root, text=" APTIV ", font=('arial', 20, 'bold'), bg=Bg_colour)

    # Lable placing
    # grid method to arrange labels in respective
    # rows and columns as specified
    APTIV.place(x=22, y=554, anchor=NW)
    return

def run_OS_check():
    M_Tool.offset_y = 25  # 20
    M_Tool.offset_x = 20  # 20
    return


"""global offset_x
global offset_y
result = messagebox.askquestion("OS Confirmation", "Are you using Windows 7 OS?")
print(result)
if "yes" == result:
    M_Tool.offset_y = 25  # 20
    M_Tool.offset_x = 0  # 20
else:
    M_Tool.offset_y = 25  # 20
    M_Tool.offset_x = 20  # 20
return"""