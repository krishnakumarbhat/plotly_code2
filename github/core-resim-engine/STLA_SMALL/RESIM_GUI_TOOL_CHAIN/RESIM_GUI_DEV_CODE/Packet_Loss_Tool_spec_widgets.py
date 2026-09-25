from tkinter import *
from tkinter import messagebox
from tkinter import filedialog
from Main import *
from main_interface import *
from Packetloss_Flist import *
from Packet_Loss_XML_writer import *
from Packet_Loss_Json_writer import *
from os import *


'''def clear_screen():
    for child in root.winfo_children():
        child.destroy()
    return'''


# global browse_log_list_label
# global browse_Log_list
# global browse_log_list_entry
global browse_output_entry
global browse_output_path_button
global same_as_input
global same_as_output
global new_output_path
global bn_califr_label
global bn_califr_opt
global faseth_log_label
global faseth_log_opt
global srr_debug_log_label
global srr_debug_log_opt
global srr_debug_log_list
global srr_reference_log_label
global srr_ref_log_opt
global clear
global json
global config
global run
global text_box_name

bn_califr_opt = None
faseth_log_opt = None
srr_debug_log_opt = None
srr_ref_log_opt = None
browse_output_entry = None
same_as_input = None
same_as_output = None
text_box_name = ''


# clear = None
# json = None
# config = None
# run = None


def call_methods():
    root.title("PACKETLOSS TOOL")
    browse_files()
    output_path()
    clear_list()
    # text_box()
    display_labels()
    Tool_info()
    buttons()
    return


# display_labels()
# Tool_info()

global bn_califr_log_list
bn_califr_log_list = []

global faseth_log_list
faseth_log_list = []

global srr_debug_log_list
srr_debug_log_list = []

global srr_reference_log_list
srr_reference_log_list = []

global json_file_log_list
json_file_log_list = []

global filelist
filelist = []

global val

global fpath


def text_box():
    global text_box_name
    global text_box_label
    text_box_label = Label(text="Progress:", font=('arial', 10, 'bold'), bg=Bg_colour).place(x=970, y=250)
    text_box_name = Text(root, height=8, width=25).place(x=900, y=290)


def clear_list():
    bn_califr_log_list.clear()
    faseth_log_list.clear()
    srr_debug_log_list.clear()
    srr_reference_log_list.clear()
    filelist.clear()


def browse_files():
    global Packetloss_Groupbox
    Packetloss_Groupbox = Canvas(height=620, width=1200, bg=Bg_colour)
    Packetloss_Groupbox.pack()

    Packetloss_Groupbox.create_rectangle(25, 40, 1080, 220, fill=None)
    """To Create Browsing Log File option for files"""
    log_files = Label(text='CREATE JSON/FLIST', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=30, y=30)

    ''' BN Califr '''
    global bn_califr_opt
    global bn_califr_label
    global bn_califr_button
    bn_califr_label = Label(text='BN CALIFR LOGS: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=40, y=60)
    bn_califr_opt = Entry()
    if platform == 'linux' or platform == 'linux2':
        bn_califr_opt.config(width=95)
    else:
        bn_califr_opt.config(width=125)
    bn_califr_opt.place(x=220, y=60)

    def browse_bn_califr_logs():
        bn_califr_logs = filedialog.askopenfilenames(title='Select BN Califr Logs',
                                                     filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for a in bn_califr_logs:
            bn_califr_log_list.append(a)
        bn_califr_opt.insert(END, bn_califr_logs)

    bn_califr_button = Button(text='Browse', command=lambda: browse_bn_califr_logs()).place(x=1000, y=55)

    '''BN FASETH'''
    global faseth_log_opt
    global faseth_log_label
    global faseth_log_button
    faseth_log_label = Label(text='BN FASETH LOGS: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=40, y=100)
    faseth_log_opt = Entry()
    if platform == 'linux' or platform == 'linux2':
        faseth_log_opt.config(width=95)
    else:
        faseth_log_opt.config(width=125)
    faseth_log_opt.place(x=220, y=100)

    def browse_faseth_logs():
        faseth_logs = filedialog.askopenfilenames(title='Select FASETH Logs',
                                                  filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))

        for b in faseth_logs:
            faseth_log_list.append(b)

        faseth_log_opt.insert(END, faseth_logs)

    faseth_log_button = Button(text='Browse', command=lambda: browse_faseth_logs()).place(x=1000, y=95)

    """SRR DEBUG"""
    global srr_debug_log_opt
    global srr_debug_log_label
    global srr_deb_button
    srr_debug_log_label = Label(text='SRR DEBUG LOGS: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=40, y=140)
    srr_debug_log_opt = Entry()
    if platform == 'linux' or platform == 'linux2':
        srr_debug_log_opt.config(width=95)
    else:
        srr_debug_log_opt.config(width=125)
    srr_debug_log_opt.place(x=220, y=140)

    def browse_srr_debug_logs():
        srr_debug_logs = filedialog.askopenfilenames(title='Select SRR DEBUG Logs',
                                                     filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))

        for c in srr_debug_logs:
            srr_debug_log_list.append(c)

        srr_debug_log_opt.insert(END, srr_debug_logs)

    srr_deb_button = Button(text='Browse', command=lambda: browse_srr_debug_logs()).place(x=1000, y=135)

    """SRR REFERENCE"""
    global srr_reference_log_label
    global srr_ref_log_opt
    global srr_ref_log_button
    srr_reference_log_label = Label(text='SRR REFERENCE LOGS: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=40,
                                                                                                                 y=180)
    srr_ref_log_opt = Entry()
    if platform == 'linux' or platform == 'linux2':
        srr_ref_log_opt.config(width=95)
    else:
        srr_ref_log_opt.config(width=125)
    srr_ref_log_opt.place(x=220, y=180)

    def browse_srr_ref_logs():
        srr_ref_logs = filedialog.askopenfilenames(title='Select SRR REFERENCE Files',
                                                   filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for r in srr_ref_logs:
            srr_reference_log_list.append(r)
        srr_ref_log_opt.insert(END, srr_ref_logs)

    srr_ref_log_button = Button(text='Browse', command=lambda: browse_srr_ref_logs()).place(x=1000, y=175)


def output_path():
    global same_as_input
    global same_as_output
    global input_value
    global loc
    global output_path_label
    Packetloss_Groupbox.create_rectangle(25, 240, 1080, 320, fill=None)

    output_path_label = Label(text='PACKETLOSS CONFIG', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=30, y=230)

    input_value = StringVar()
    input_value.set('SAME_AS_INPUT')
    same_as_input = Radiobutton(text='Same As Input', bg=Bg_colour, variable=input_value, font=('arial', 10, 'bold')
                                , value='SAME_AS_INPUT', command=new_output).place(x=40, y=250)

    same_as_output = Radiobutton(text='Same As Output', bg=Bg_colour, variable=input_value, font=('arial', 10, 'bold')
                                 , value='SAME_AS_OUTPUT', command=new_output).place(x=180, y=250)

    new_output_path = Radiobutton(text='New Output Path', bg=Bg_colour, variable=input_value, font=('arial', 10, 'bold')
                                  , value='NEW_OUTPUT_PATH', command=new_output).place(x=340, y=250)


def new_output():
    global val
    val = input_value.get()
    global browse_output_entry
    global browse_output_path_button
    # global output_Path

    if val == 'NEW_OUTPUT_PATH':
        browse_output_path_label = Label(text="OUTPUT PATH:", font=('arial', 10, 'bold'), bg=Bg_colour)
        browse_output_path_label.place(x=40, y=290)
        browse_output_entry = Entry()
        if platform == 'linux' or platform == 'linux2':
            browse_output_entry.config(width=80)
        else:
            browse_output_entry.config(width=106)
        browse_output_entry.place(x=150, y=290)

        def new():
            global fpath
            fpath = filedialog.askdirectory()
            browse_output_entry.insert(END, fpath)
            filelist.append(fpath)

        browse_output_path_button = Button(text='Browse', command=new).place(x=800, y=285)


def clear_elements():
    input_value.set('.')
    bn_califr_opt.delete(0, END)
    faseth_log_opt.delete(0, END)
    srr_debug_log_opt.delete(0, END)
    srr_ref_log_opt.delete(0, END)
    browse_output_entry.delete(0, END)


def json_file():
    if len(srr_debug_log_list) == 0:
        messagebox.showerror(title='Logs', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')
    elif len(bn_califr_log_list) == len(faseth_log_list) == len(srr_debug_log_list) == len(srr_reference_log_list):
        # oldfiledel()
        json_writer(bn_califr_log_list, faseth_log_list, srr_debug_log_list, srr_reference_log_list)
        messagebox.showinfo(title='Logs', message='! SUCCESSFULLY GENERATED THE JSON FILE !')
        json_path()
    elif len(bn_califr_log_list and faseth_log_list and srr_reference_log_list) == 0 and len(srr_debug_log_list) >= 1:
        flist_gen(srr_debug_log_list)
        messagebox.showinfo(title='Logs', message='! SUCCESSFULLY GENERATED THE FLIST FILE !')
        flist_path()
    else:
        messagebox.showerror(title='Logs', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')


def config_xml_file():
    global input_value1
    input_value1 = input_value.get()
    if input_value1 == ".":
        messagebox.showerror(title='XML File Creation', message='!!! PLEASE SELECT OUTPUT PATH !!!')
    else:
        Write_Packet_Loss_XML(input_value1, filelist)
        messagebox.showinfo(title='XML File Creation', message='!SUCCESSFULLY GENERATED XML FILE!')
        xml_path()


def run_command():
    if len(srr_debug_log_list) == 0:
        messagebox.showerror(title='Logs', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')
    elif len(bn_califr_log_list or faseth_log_list or srr_debug_log_list or srr_reference_log_list) == 0:
        messagebox.showerror(title='Logs', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')
    else:
        execute_packet_loss_command(bn_califr_log_list, faseth_log_list, srr_debug_log_list, srr_reference_log_list)


def buttons():
    global clear
    global json
    global config
    global run
    clear = Button(text="CLEAR ALL", font=('arial', 10, 'bold'), height=2, width=18, command=clear_elements)\
        .place(x=30, y=445)

    json = Button(text="CREATE \n JSON/FLIST", font=('arial', 10, 'bold'), height=2, width=18, command=json_file)\
        .place(x=220, y=445)

    config = Button(text="CREATE \n CONFIG XML", font=('arial', 10, 'bold'), height=2, width=18,
                    command=config_xml_file).place(x=410, y=445)

    run = Button(text="START \n PACKETLOSS TOOL", font=('arial', 10, 'bold'), height=2, width=18,
                 command=run_command).place(x=610, y=445)

    help = Label(root, text="Help : chalapathi.kalyan.donepudi@aptiv.com",
                 font=('arial', 8, 'bold'), bg=Bg_colour).place(x=940, y=556)

