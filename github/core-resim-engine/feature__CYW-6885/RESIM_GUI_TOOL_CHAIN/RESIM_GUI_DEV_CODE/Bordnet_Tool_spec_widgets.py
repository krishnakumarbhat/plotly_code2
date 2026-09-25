from tkinter import *
from tkinter.ttk import *
from tkinter import filedialog
import os
from Main import *
from main_interface import *
from Bordnet_JSON_Writer import *
from Bordnet_XML_Writer import *
from sys import platform


def call_bordnet_tool():
    Ask_Logfiles()
    other_options()
    declare_global()
    Executioninfo_Options()
    logtype_options()
    generate_file_options()
    create_csv_options()
    create_xml_options()
    bordnet_buttons()
    variants()
    call_labels()


input_logfile_list = []

output_logfile_list = []


def declare_global():
    global scan_low_label
    global bpil_left_label
    global bpil_right_label
    global ecu_ethernet_label
    global pcan_low_rear_label
    global pcan_low_front_label
    global pcan_mid_rl_label
    global pcan_mid_rr_label
    global pcan_mid_fr_label
    global pcan_mid_fl_label
    global vcan_low_label
    global vcan_ecu_label
    global scan_low_opt
    global bpil_left_opt
    global bpil_right_opt
    global ecu_ethernet_opt
    global pcan_low_rear_opt
    global pcan_low_front_opt
    global pcan_mid_rl_opt
    global pcan_mid_rr_opt
    global pcan_mid_fr_opt
    global pcan_mid_fl_opt
    global vcan_low_opt
    global vcan_ecu_opt
    global scan_low_label_val
    global bpil_left_label_val
    global bpil_right_label_val
    global ecu_ethernet_label_val
    global pcan_low_rear_label_val
    global pcan_low_front_label_val
    global pcan_mid_rl_label_val
    global pcan_mid_rr_label_val
    global pcan_mid_fr_label_val
    global pcan_mid_fl_label_val
    global vcan_low_label_val
    global vcan_ecu_label_val


def Ask_Logfiles():
    global Bordnet_Groupbox
    Bordnet_Groupbox = Canvas(height=620, width=1200, bg=Bg_colour)
    Bordnet_Groupbox.pack()

    Bordnet_Groupbox.create_rectangle(25, 25, 720, 95, fill=None)
    Label(text='CREATE JSON/FLIST', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=30, y=15)
    """Input Log Files"""
    global bordnet_input_log_opt
    Label(text='Select INPUT log Files: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=30, y=40)
    bordnet_input_log_opt = Entry(state=DISABLED)
    if platform == "linux" or platform == "linux2":
        bordnet_input_log_opt.config(width=35)
    else:
        bordnet_input_log_opt.config(width=50)
    bordnet_input_log_opt.place(x=200, y=40)

    def browse_input_log_files():
        bordnet_input_log_opt.config(state=NORMAL)
        input_logfile_list.clear()
        input_log_files = filedialog.askopenfilenames(title='Select INPUT log Files',
                                                      filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        bordnet_input_log_opt.insert(END, input_log_files)
        bordnet_input_log_opt.config(state=DISABLED)
        for inp_logs in input_log_files:
            input_logfile_list.append(inp_logs)

    global input_file_button
    input_file_button = Button(text='Browse', command=browse_input_log_files)
    if platform == "linux" or platform == "linux2":
        input_file_button.place(x=500, y=35)
    else:
        input_file_button.place(x=510, y=35)

    """Output Log Files"""
    global bordnet_output_log_opt
    output_file_label = Label(text='Select OUTPUT log Files: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=30,
                                                                                                                y=70)
    bordnet_output_log_opt = Entry(state=DISABLED)
    if platform == "linux" or platform == "linux2":
        bordnet_output_log_opt.config(width=35)
    else:
        bordnet_output_log_opt.config(width=50)
    bordnet_output_log_opt.place(x=200, y=70)

    def browse_output_log_files():
        bordnet_output_log_opt.config(state=NORMAL)
        output_logfile_list.clear()
        output_log_files = filedialog.askopenfilenames(title='Select OUTPUT log Files',
                                                       filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        bordnet_output_log_opt.insert(END, output_log_files)
        for opt_logs in output_log_files:
            output_logfile_list.append(opt_logs)
        bordnet_output_log_opt.config(state=DISABLED)

    global output_file_button
    output_file_button = Button(text='Browse', command=browse_output_log_files)
    if platform == "linux" or platform == "linux2":
        output_file_button.place(x=500, y=65)
    else:
        output_file_button.place(x=510, y=65)


output_path_list = []


def other_options():
    Label(root, text="Bordnet Config Version: 9.1", font=('arial', 10, 'bold', 'italic'),
          bg=Bg_colour).place(x=950, y=420)

    select_folder_label = Label(text='OUTPUT Path: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    select_folder_label.place(x=30, y=130)

    global output_path_opt
    if platform == "linux" or platform == "linux2":
        output_path_opt = Entry(width=35)
    else:
        output_path_opt = Entry(width=50)
    output_path_opt.place(x=200, y=130)

    def ask_newoutput_path():
        new_output_dir = filedialog.askdirectory()
        output_path_list.clear()
        output_path_list.append(new_output_dir)
        output_path_opt.insert(END, new_output_dir)

    global new_output_button
    new_output_button = Button(text='Browse', command=lambda: ask_newoutput_path())
    if platform == "linux" or platform == "linux2":
        new_output_button.place(x=500, y=125)
    else:
        new_output_button.place(x=510, y=125)

    """Execution Type Selection"""
    execution_type_label = Label(text='Select Execution Type: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    execution_type_label.place(x=30, y=170)

    global execution_val
    execution_val = StringVar()

    global execution_combobox
    execution_combobox = Combobox(root, width=22, textvariable=execution_val)

    execution_combobox['values'] = ["Select Execution Type", "SEQUENTIAL", "CONTINUOUS"]
    execution_combobox.current(0)
    execution_combobox.place(x=200, y=170)

    """Selecting Customer"""

    customer_label = Label(text='Select Customer Type: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    customer_label.place(x=30, y=210)
    global customer_val
    customer_val = StringVar()

    global customer_combobox
    customer_combobox = Combobox(root, width=22, textvariable=customer_val)

    customer_combobox['values'] = ["Select Customer", "BMW_SRR5"]
    customer_combobox.current(0)
    customer_combobox.place(x=200, y=210)

    """Select Version"""

    version_label = Label(text='Select Version: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    version_label.place(x=30, y=250)
    global version_val
    version_val = StringVar()

    global version_combobox
    version_combobox = Combobox(root, width=22, textvariable=version_val)

    version_combobox['values'] = ["Select Version", "A_310", "A_330", "A_350", "A_370",
                                  "A_390", "A_410", "A_430", "A_450", "A_470", "A_480"]
    version_combobox.current(0)
    version_combobox.place(x=200, y=250)


def candecoder_options(event):
    if variant_combobox.get() == 'BMW_LOW':
        """LOW VARIANT"""
        global low_variant_label
        low_variant_label = Label(text='BMW_LOW ', bg=Bg_colour, font=('arial', 10, 'bold'))
        low_variant_label.place(x=30, y=330)

        low_variant_val = StringVar

        global low_variant_combobox
        low_variant_combobox = Combobox(root, width=22, textvariable=low_variant_val)
        low_variant_combobox['values'] = ["CANDECODER Type", "RADAR_PCAN", "RADAR_SCAN", "RADAR_VCAN", "All"]
        low_variant_combobox.current(0)
        low_variant_combobox.place(x=200, y=330)

    elif variant_combobox.get() == "BMW_MID":
        """MID Variant"""
        global mid_variant_label
        mid_variant_label = Label(text='BMW_MID ', bg=Bg_colour, font=('arial', 10, 'bold'))
        mid_variant_label.place(x=30, y=330)

        mid_variant_val = StringVar

        global mid_variant_combobox
        mid_variant_combobox = Combobox(root, width=22, textvariable=mid_variant_val)
        mid_variant_combobox['values'] = ["CANDECODER Type", "RADAR_PCAN", "ECU_VCAN", "ECU_ETHERNET", "All"]
        mid_variant_combobox.current(0)
        mid_variant_combobox.place(x=200, y=330)

    elif variant_combobox.get() == "BMW_HIGH":
        """HIGH Variant"""
        global high_variant_label
        high_variant_label = Label(text='BMW_HIGH ', bg=Bg_colour, font=('arial', 10, 'bold'))
        high_variant_label.place(x=30, y=330)

        high_variant_val = StringVar

        global high_variant_combobox
        high_variant_combobox = Combobox(root, width=22, textvariable=high_variant_val)
        high_variant_combobox['values'] = ["CANDECODER Type", "RADAR_B_PIL"]
        high_variant_combobox.current(0)
        high_variant_combobox.place(x=200, y=330)
    elif variant_combobox.get() == "Select Variant":
        try:
            low_variant_label.place_forget()
            low_variant_combobox.place_forget()
            mid_variant_label.place_forget()
            mid_variant_combobox.place_forget()
            high_variant_label.place_forget()
            high_variant_combobox.place_forget()
        except:
            pass


def variants():
    variant_label = Label(text='Select Variant: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    variant_label.place(x=30, y=290)

    global variant_val
    variant_val = StringVar

    global variant_combobox
    variant_combobox = Combobox(root, width=22, textvariable=variant_val)

    variant_combobox['values'] = ["Select Variant", "BMW_LOW", "BMW_MID", "BMW_HIGH"]
    variant_combobox.current(0)
    variant_combobox.place(x=200, y=290)

    variant_combobox.bind("<<ComboboxSelected>>", candecoder_options)


def Executioninfo_Options():
    """OLD XML PRINT"""
    global old_xml_val
    old_xml_val = IntVar()
    old_xml_val.set(0)
    global old_xml_option
    old_xml_option = Checkbutton(root, text='OLD XML PRINT', variable=old_xml_val, onvalue=1, offvalue=0,
                                 font=('arial', 10, 'bold'), bg=Bg_colour)
    old_xml_option.place(x=30, y=360)


    """pcan_csv Option"""
    global pcan_csv_val
    pcan_csv_val = IntVar()
    pcan_csv_val.set(0)
    global pcan_csv_option
    pcan_csv_option = Checkbutton(root, text="PCAN CSV", variable=pcan_csv_val, onvalue=1, offvalue=0,
                                  font=('arial', 10, 'bold'), bg=Bg_colour)
    pcan_csv_option.place(x=230, y=360)

    Bordnet_Groupbox.create_rectangle(25, 510, 340, 400, fill=None)

    """Time Stamp Decoding"""

    Label(root, text='Timestamp Decoding', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=30, y=390)

    global scan_val
    scan_val = IntVar()
    scan_val.set(0)
    global scan_option
    scan_option = Checkbutton(root, text='SCAN', variable=scan_val, onvalue=1, offvalue=0,
                              font=('arial', 10, 'bold'), bg=Bg_colour)
    scan_option.place(x=80, y=420)

    global someip_val
    someip_val = IntVar()
    someip_val.set(0)
    global someip_option
    someip_option = Checkbutton(root, text='SOMEIP', variable=someip_val, onvalue=1, offvalue=0,
                                font=('arial', 10, 'bold'), bg=Bg_colour)
    someip_option.place(x=80, y=450)

    global vcan_bpil_val
    vcan_bpil_val = IntVar()
    vcan_bpil_val.set(0)
    global vcan_bpil_option
    vcan_bpil_option = Checkbutton(root, text='VCAN BPIL', variable=vcan_bpil_val, onvalue=1, offvalue=0,
                                   font=('arial', 10, 'bold'), bg=Bg_colour)
    vcan_bpil_option.place(x=80, y=480)

    global vcan_ecu_val
    vcan_ecu_val = IntVar()
    vcan_ecu_val.set(0)
    global vcan_ecu_option
    vcan_ecu_option = Checkbutton(root, text='VCAN ECU', variable=vcan_ecu_val, onvalue=1, offvalue=0,
                                  font=('arial', 10, 'bold'), bg=Bg_colour)
    vcan_ecu_option.place(x=200, y=420)

    global vcan_low_val
    vcan_low_val = IntVar()
    vcan_low_val.set(0)
    global vcan_low_option
    vcan_low_option = Checkbutton(root, text='VCAN LOW', variable=vcan_low_val, onvalue=1, offvalue=0,
                                  font=('arial', 10, 'bold'), bg=Bg_colour)
    vcan_low_option.place(x=200, y=450)

    global pcan_val
    pcan_val = IntVar()
    pcan_val.set(0)
    global pcan_option
    pcan_option = Checkbutton(root, text='PCAN', variable=pcan_val, onvalue=1, offvalue=0,
                              font=('arial', 10, 'bold'), bg=Bg_colour)
    pcan_option.place(x=200, y=480)

    """SORT"""

    Label(root, text='Sort: ', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=800, y=42)

    global sort_val
    sort_val = StringVar()
    sort_val.set(".")
    global sort_scan_option
    sort_scan_option = Checkbutton(root, text='SCAN', font=('arial', 10, 'bold'),
                                   variable=sort_val, onvalue="SCAN", bg=Bg_colour)
    sort_scan_option.place(x=850, y=40)

    global sort_bpil_option
    sort_bpil_option = Checkbutton(root, text='BPIL', font=('arial', 10, 'bold'),
                                   variable=sort_val, onvalue="BPIL", bg=Bg_colour)
    sort_bpil_option.place(x=930, y=40)


def clear_labels():
    scan_low_label.place_forget()
    bpil_left_label.place_forget()
    bpil_right_label.place_forget()
    ecu_ethernet_label.place_forget()
    pcan_low_rear_label.place_forget()
    pcan_low_front_label.place_forget()
    pcan_mid_rl_label.place_forget()
    pcan_mid_rr_label.place_forget()
    pcan_mid_fr_label.place_forget()
    pcan_mid_fl_label.place_forget()
    vcan_low_label.place_forget()
    vcan_ecu_label.place_forget()


def clear_textboxes():
    scan_low_opt.place_forget()
    bpil_left_opt.place_forget()
    bpil_right_opt.place_forget()
    ecu_ethernet_opt.place_forget()
    pcan_low_rear_opt.place_forget()
    pcan_low_front_opt.place_forget()
    pcan_mid_rl_opt.place_forget()
    pcan_mid_rr_opt.place_forget()
    pcan_mid_fr_opt.place_forget()
    pcan_mid_fl_opt.place_forget()
    vcan_low_opt.place_forget()
    vcan_ecu_opt.place_forget()


def clear_labels_val():
    scan_low_label_val.place_forget()
    bpil_left_label_val.place_forget()
    bpil_right_label_val.place_forget()
    ecu_ethernet_label_val.place_forget()
    pcan_low_rear_label_val.place_forget()
    pcan_low_front_label_val.place_forget()
    pcan_mid_rl_label_val.place_forget()
    pcan_mid_rr_label_val.place_forget()
    pcan_mid_fr_label_val.place_forget()
    pcan_mid_fl_label_val.place_forget()
    vcan_low_label_val.place_forget()
    vcan_ecu_label_val.place_forget()


def get_values():
    global scan_low_label
    global bpil_left_label
    global bpil_right_label
    global ecu_ethernet_label
    global pcan_low_rear_label
    global pcan_low_front_label
    global pcan_mid_rl_label
    global pcan_mid_rr_label
    global pcan_mid_fr_label
    global pcan_mid_fl_label
    global vcan_low_label
    global vcan_ecu_label
    global scan_low_opt
    global bpil_left_opt
    global bpil_right_opt
    global ecu_ethernet_opt
    global pcan_low_rear_opt
    global pcan_low_front_opt
    global pcan_mid_rl_opt
    global pcan_mid_rr_opt
    global pcan_mid_fr_opt
    global pcan_mid_fl_opt
    global vcan_low_opt
    global vcan_ecu_opt
    global scan_low_label_val
    global bpil_left_label_val
    global bpil_right_label_val
    global ecu_ethernet_label_val
    global pcan_low_rear_label_val
    global pcan_low_front_label_val
    global pcan_mid_rl_label_val
    global pcan_mid_rr_label_val
    global pcan_mid_fr_label_val
    global pcan_mid_fl_label_val
    global vcan_low_label_val
    global vcan_ecu_label_val
    if buschannel_val.get() == 'USER':
        scan_low_label = Label(text='SCAN LOW: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        scan_low_label.place(x=520, y=190)
        scan_low_opt = Entry(width=6)
        scan_low_opt.place(x=660, y=190)

        bpil_left_label = Label(text='BPIL LEFT: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        bpil_left_label.place(x=520, y=215)
        bpil_left_opt = Entry(width=6)
        bpil_left_opt.place(x=660, y=215)

        bpil_right_label = Label(text='BPIL RIGHT: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        bpil_right_label.place(x=520, y=240)
        bpil_right_opt = Entry(width=6)
        bpil_right_opt.place(x=660, y=240)

        ecu_ethernet_label = Label(text='ECU ETHERNET: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        ecu_ethernet_label.place(x=520, y=265)
        ecu_ethernet_opt = Entry(width=6)
        ecu_ethernet_opt.place(x=660, y=265)

        pcan_low_rear_label = Label(text='PCAN LOW REAR: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        pcan_low_rear_label.place(x=520, y=290)
        pcan_low_rear_opt = Entry(width=6)
        pcan_low_rear_opt.place(x=660, y=290)

        pcan_low_front_label = Label(text='PCAN LOW FRONT: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        pcan_low_front_label.place(x=520, y=315)
        pcan_low_front_opt = Entry(width=6)
        pcan_low_front_opt.place(x=660, y=315)

        pcan_mid_rl_label = Label(text='PCAN MID RL: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        pcan_mid_rl_label.place(x=520, y=340)
        pcan_mid_rl_opt = Entry(width=6)
        pcan_mid_rl_opt.place(x=660, y=340)

        pcan_mid_rr_label = Label(text='PCAN MID RR: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        pcan_mid_rr_label.place(x=520, y=365)
        pcan_mid_rr_opt = Entry(width=6)
        pcan_mid_rr_opt.place(x=660, y=365)

        pcan_mid_fr_label = Label(text='PCAN MID FR: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        pcan_mid_fr_label.place(x=520, y=390)
        pcan_mid_fr_opt = Entry(width=6)
        pcan_mid_fr_opt.place(x=660, y=390)

        pcan_mid_fl_label = Label(text='PCAN MID FL: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        pcan_mid_fl_label.place(x=520, y=415)
        pcan_mid_fl_opt = Entry(width=6)
        pcan_mid_fl_opt.place(x=660, y=415)

        vcan_low_label = Label(text='VCAN LOW: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        vcan_low_label.place(x=520, y=440)
        vcan_low_opt = Entry(width=6)
        vcan_low_opt.place(x=660, y=440)

        vcan_ecu_label = Label(text='VCAN ECU: ', bg=Bg_colour, font=('arial', 10, 'bold'))
        vcan_ecu_label.place(x=520, y=465)
        vcan_ecu_opt = Entry(width=6)
        vcan_ecu_opt.place(x=660, y=465)

    elif buschannel_val.get() == 'DEFAULT':
        global scan_low_label_val
        global bpil_left_label_val
        global bpil_right_label_val
        global ecu_ethernet_label_val
        global pcan_low_rear_label_val
        global pcan_low_front_label_val
        global pcan_mid_rl_label_val
        global pcan_mid_rr_label_val
        global pcan_mid_fr_label_val
        global pcan_mid_fl_label_val
        global vcan_low_label_val
        global vcan_ecu_label_val
        scan_low_label_val = Label(text='SCAN LOW:                 4       ', bg=Bg_colour,
                                   font=('arial', 10, 'bold'))
        scan_low_label_val.place(x=520, y=190)
        bpil_left_label_val = Label(text='BPIL LEFT:                  5       ', bg=Bg_colour,
                                    font=('arial', 10, 'bold'))
        bpil_left_label_val.place(x=520, y=215)
        bpil_right_label_val = Label(text='BPIL RIGHT:                6       ', bg=Bg_colour,
                                     font=('arial', 10, 'bold'))
        bpil_right_label_val.place(x=520, y=240)
        ecu_ethernet_label_val = Label(text='ECU ETHERNET:         12       ', bg=Bg_colour,
                                       font=('arial', 10, 'bold'))
        ecu_ethernet_label_val.place(x=520, y=265)
        pcan_low_rear_label_val = Label(text='PCAN LOW REAR:       2       ', bg=Bg_colour,
                                        font=('arial', 10, 'bold'))
        pcan_low_rear_label_val.place(x=520, y=290)
        pcan_low_front_label_val = Label(text='PCAN LOW FRONT:     2       ', bg=Bg_colour,
                                         font=('arial', 10, 'bold'))
        pcan_low_front_label_val.place(x=520, y=315)
        pcan_mid_rl_label_val = Label(text='PCAN MID RL:             2       ', bg=Bg_colour,
                                      font=('arial', 10, 'bold'))
        pcan_mid_rl_label_val.place(x=520, y=340)
        pcan_mid_rr_label_val = Label(text='PCAN MID RR:             2       ', bg=Bg_colour,
                                      font=('arial', 10, 'bold'))
        pcan_mid_rr_label_val.place(x=520, y=365)
        pcan_mid_fr_label_val = Label(text='PCAN MID FR:             2       ', bg=Bg_colour,
                                      font=('arial', 10, 'bold'))
        pcan_mid_fr_label_val.place(x=520, y=390)
        pcan_mid_fl_label_val = Label(text='PCAN MID FL:             2       ', bg=Bg_colour,
                                      font=('arial', 10, 'bold'))
        pcan_mid_fl_label_val.place(x=520, y=415)
        vcan_low_label_val = Label(text='VCAN LOW:                2       ', bg=Bg_colour,
                                   font=('arial', 10, 'bold'))
        vcan_low_label_val.place(x=520, y=440)
        vcan_ecu_label_val = Label(text='VCAN ECU:                 2        ', bg=Bg_colour,
                                   font=('arial', 10, 'bold'))
        vcan_ecu_label_val.place(x=520, y=465)


def logtype_entry(*event):
    global buschannel_val
    global buschannel_default_option
    global buschannel_user_option
    if logtype_combobox.get() == "BMW_CANOE":
        buschannel_val = StringVar()
        buschannel_val.set(".")

        buschannel_default_option = Radiobutton(root, text='DEFAULT', font=('arial', 10, 'bold'),
                                                variable=buschannel_val, value="DEFAULT", bg=Bg_colour,
                                                command=get_values)
        buschannel_default_option.place(x=420, y=190)

        buschannel_user_option = Radiobutton(root, text='USER', font=('arial', 10, 'bold'),
                                             variable=buschannel_val, value="USER", bg=Bg_colour, command=get_values)
        buschannel_user_option.place(x=420, y=220)
    elif logtype_combobox.get() == "MAGNA_X2E" or "BMW_VIGEM" or "Select Variant":
        try:
            buschannel_default_option.place_forget()
            buschannel_user_option.place_forget()
            clear_labels()
            clear_textboxes()
            clear_labels_val()
        except:
            pass


def logtype_options():
    logtype_label = Label(text='Log Type: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    logtype_label.place(x=400, y=160)

    global logtype_val
    logtype_val = StringVar
    global logtype_combobox
    logtype_combobox = Combobox(root, width=22, textvariable=logtype_val)

    logtype_combobox['values'] = ["Select Variant", "BMW_CANOE", "MAGNA_X2E", "BMW_VIGEM"]
    logtype_combobox.current(0)
    logtype_combobox.place(x=480, y=160)

    logtype_combobox.bind("<<ComboboxSelected>>", logtype_entry)


def generate_file_options():
    Bordnet_Groupbox.create_rectangle(705, 250, 935, 130, fill=None)
    """GENERATE FILE"""

    Label(root, text='Generate File Options', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=710, y=120)

    global timestampdiagnostics_val
    timestampdiagnostics_val = IntVar()
    timestampdiagnostics_val.set(0)
    global timestampdiagnostics_option
    timestampdiagnostics_option = Checkbutton(root, text='TIME STAMP DIAGNOSTICS', variable=timestampdiagnostics_val,
                                              onvalue=1, offvalue=0,
                                              font=('arial', 10, 'bold'), bg=Bg_colour)
    timestampdiagnostics_option.place(x=730, y=145)

    global canidtimestamp_val
    canidtimestamp_val = IntVar()
    canidtimestamp_val.set(0)
    global canidtimestamp_option
    canidtimestamp_option = Checkbutton(root, text='CANID TIMESTAMP', variable=canidtimestamp_val,
                                        onvalue=1, offvalue=0,
                                        font=('arial', 10, 'bold'), bg=Bg_colour)
    canidtimestamp_option.place(x=730, y=170)

    global diagnostics_val
    diagnostics_val = IntVar()
    diagnostics_val.set(0)
    global diagnostics_option
    diagnostics_option = Checkbutton(root, text='DIAGNOSTICS', variable=diagnostics_val,
                                     onvalue=1, offvalue=0,
                                     font=('arial', 10, 'bold'), bg=Bg_colour)
    diagnostics_option.place(x=730, y=195)

    global frameworktiming_val
    frameworktiming_val = IntVar()
    frameworktiming_val.set(0)
    global frameworktiming_option
    frameworktiming_option = Checkbutton(root, text='FRAMEWORK TIMING', variable=frameworktiming_val,
                                         onvalue=1, offvalue=0,
                                         font=('arial', 10, 'bold'), bg=Bg_colour)
    frameworktiming_option.place(x=730, y=220)


def create_xml_options():
    Bordnet_Groupbox.create_rectangle(705, 470, 935, 270, fill=None)
    """GENERATE XML OPTIONS"""

    Label(root, text='Generate XML Options', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=710, y=260)

    global xml_objectlist_val
    xml_objectlist_val = IntVar()
    xml_objectlist_val.set(0)
    global xml_objectlist_option
    xml_objectlist_option = Checkbutton(root, text='OBJECT LIST', variable=xml_objectlist_val,
                                        onvalue=1, offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour)
    xml_objectlist_option.place(x=730, y=285)

    global xml_detection_rr_val
    xml_detection_rr_val = IntVar()
    xml_detection_rr_val.set(0)
    global xml_detection_rr_option
    xml_detection_rr_option = Checkbutton(root, text='DETECTION RR', variable=xml_detection_rr_val,
                                          onvalue=1, offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour)
    xml_detection_rr_option.place(x=730, y=310)

    global xml_detection_rl_val
    xml_detection_rl_val = IntVar()
    xml_detection_rl_val.set(0)
    global xml_detection_rl_option
    xml_detection_rl_option = Checkbutton(root, text='DETECTION RL', variable=xml_detection_rl_val,
                                          onvalue=1, offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour)
    xml_detection_rl_option.place(x=730, y=335)

    global xml_detection_fr_val
    xml_detection_fr_val = IntVar()
    xml_detection_fr_val.set(0)
    global xml_detection_fr_option
    xml_detection_fr_option = Checkbutton(root, text='DETECTION FR', variable=xml_detection_fr_val,
                                          onvalue=1, offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour)
    xml_detection_fr_option.place(x=730, y=360)

    global xml_detection_fl_val
    xml_detection_fl_val = IntVar()
    xml_detection_fl_val.set(0)
    global xml_detection_fl_option
    xml_detection_fl_option = Checkbutton(root, text='DETECTION FL', variable=xml_detection_fl_val,
                                          onvalue=1, offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour)
    xml_detection_fl_option.place(x=730, y=385)

    global xml_freespace_val
    xml_freespace_val = IntVar()
    xml_freespace_val.set(0)
    global xml_freespace_option
    xml_freespace_option = Checkbutton(root, text='FREE SPACE', variable=xml_freespace_val,
                                       onvalue=1, offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour)
    xml_freespace_option.place(x=730, y=410)

    global xml_restmessage_val
    xml_restmessage_val = IntVar()
    xml_restmessage_val.set(0)
    global xml_restmessage_option
    xml_restmessage_option = Checkbutton(root, text='REST MESSAGE', variable=xml_restmessage_val,
                                         onvalue=1, offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour)
    xml_restmessage_option.place(x=730, y=435)


def create_csv_options():
    Bordnet_Groupbox.create_rectangle(955, 417, 1160, 130, fill=None)
    """GENERATE CSV OPTIONS"""

    Label(root, text='Generate CSV Options', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=960, y=120)

    global objectlist_val
    objectlist_val = IntVar()
    objectlist_val.set(0)
    global objectlist_option
    objectlist_option = Checkbutton(root, text='OBJECT LIST', variable=objectlist_val,
                                    onvalue=1, offvalue=0,
                                    font=('arial', 10, 'bold'), bg=Bg_colour)
    objectlist_option.place(x=980, y=150)

    global detection_rr_val
    detection_rr_val = IntVar()
    detection_rr_val.set(0)
    global detection_rr_option
    detection_rr_option = Checkbutton(root, text='DETECTION RR', variable=detection_rr_val,
                                      onvalue=1, offvalue=0,
                                      font=('arial', 10, 'bold'), bg=Bg_colour)
    detection_rr_option.place(x=980, y=180)

    global detection_rl_val
    detection_rl_val = IntVar()
    detection_rl_val.set(0)
    global detection_rl_option
    detection_rl_option = Checkbutton(root, text='DETECTION RL', variable=detection_rl_val,
                                      onvalue=1, offvalue=0,
                                      font=('arial', 10, 'bold'), bg=Bg_colour)
    detection_rl_option.place(x=980, y=210)

    global detection_fr_val
    detection_fr_val = IntVar()
    detection_fr_val.set(0)
    global detection_fr_option
    detection_fr_option = Checkbutton(root, text='DETECTION FR', variable=detection_fr_val,
                                      onvalue=1, offvalue=0,
                                      font=('arial', 10, 'bold'), bg=Bg_colour)
    detection_fr_option.place(x=980, y=240)

    global detection_fl_val
    detection_fl_val = IntVar()
    detection_fl_val.set(0)
    global detection_fl_option
    detection_fl_option = Checkbutton(root, text='DETECTION FL', variable=detection_fl_val,
                                      onvalue=1, offvalue=0,
                                      font=('arial', 10, 'bold'), bg=Bg_colour)
    detection_fl_option.place(x=980, y=270)

    global detection_bpil_left_val
    detection_bpil_left_val = IntVar()
    detection_bpil_left_val.set(0)
    global detection_bpil_left_option
    detection_bpil_left_option = Checkbutton(root, text='DETECTION BPIL LEFT', variable=detection_bpil_left_val,
                                             onvalue=1, offvalue=0,
                                             font=('arial', 10, 'bold'), bg=Bg_colour)
    detection_bpil_left_option.place(x=980, y=300)

    global detection_bpil_right_val
    detection_bpil_right_val = IntVar()
    detection_bpil_right_val.set(0)
    global detection_bpil_right_option
    detection_bpil_right_option = Checkbutton(root, text='DETECTION BPIL RIGHT', variable=detection_bpil_right_val,
                                              onvalue=1, offvalue=0,
                                              font=('arial', 10, 'bold'), bg=Bg_colour)
    detection_bpil_right_option.place(x=980, y=330)

    global vcan_alert_shr_val
    vcan_alert_shr_val = IntVar()
    vcan_alert_shr_val.set(0)
    global vcan_alert_shr_option
    vcan_alert_shr_option = Checkbutton(root, text='VCAN ALERT SHR', variable=vcan_alert_shr_val,
                                        onvalue=1, offvalue=0,
                                        font=('arial', 10, 'bold'), bg=Bg_colour)
    vcan_alert_shr_option.place(x=980, y=360)

    global vcsn_alert_svl_val
    vcsn_alert_svl_val = IntVar()
    vcsn_alert_svl_val.set(0)
    global vcsn_alert_svl_option
    vcsn_alert_svl_option = Checkbutton(root, text='VCAN ALERT SVL', variable=vcsn_alert_svl_val,
                                        onvalue=1, offvalue=0,
                                        font=('arial', 10, 'bold'), bg=Bg_colour)
    vcsn_alert_svl_option.place(x=980, y=390)


bordnet_json_list = []


def call_json():
    global bordnet_json_path
    bordnet_json_path = os.path.abspath("BORDNET_fList.json")
    Label(root, text='JSON File:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=30, y=100)
    global json_label
    json_label = Label(root, text=bordnet_json_path, font=('arial', 10, 'bold'), bg=Bg_colour).place(x=105, y=100)
    bordnet_json_list.clear()
    bordnet_json_list.append(bordnet_json_path)


def bordnet_json_creator():
    if len(input_logfile_list) == 0:
        messagebox.showerror(title='Browse Files', message='!!!PLEASE SELECT INPUT FILES!!!')
    elif len(input_logfile_list) >= 1:
        if os.path.exists("BORDNET_fList.json"):
            os.remove("BORDNET_fList.json")
        Bordnet_JSON(input_logfile_list, output_logfile_list)
        messagebox.showinfo(title='Browse Files', message='JSON File Created Successfully')
        call_json()
    else:
        messagebox.showerror(title='Browse Files', message='!!! Please Select all Files !!!')


Other_Options_List = []

XML_Options_List = []

Generate_File_Options_List = []

CSV_Options_List = []

Buschannel_Values_List = []

Execution_Info_values = []

Config_XML_list = []


def Create_Bordnet_XML():
    Other_Options_List.clear()
    Buschannel_Values_List.clear()
    Execution_Info_values.clear()
    Generate_File_Options_List.clear()
    CSV_Options_List.clear()
    XML_Options_List.clear()

    if len(bordnet_json_list) == 0:
        messagebox.showerror(title='Config XML', message='Please Create JSON')

    if os.path.exists("bordnet_config.xml"):
        os.remove("bordnet_config.xml")

    index_0 = execution_combobox.get()
    Other_Options_List.append(index_0)  # Execution Type - '0'

    index_1 = customer_combobox.get()
    Other_Options_List.append(index_1)  # Customer Name - '1'

    index_2 = version_combobox.get()
    Other_Options_List.append(index_2)  # Version - '2'

    index_3 = variant_combobox.get()
    Other_Options_List.append(index_3)  # Variant - '3'

    if index_3 == 'BMW_LOW':
        index_4 = low_variant_combobox.get()
        Other_Options_List.append(index_4)
    elif index_3 == 'BMW_MID':
        index_4 = mid_variant_combobox.get()
        Other_Options_List.append(index_4)
    elif index_3 == 'BMW_HIGH':
        index_4 = high_variant_combobox.get()
        Other_Options_List.append(index_4)

    bus_index_0 = logtype_combobox.get()
    Buschannel_Values_List.append(bus_index_0)  # Logtype - '0'

    if bus_index_0 == 'BMW_CANOE':
        bus_index_1 = buschannel_val.get()
        Buschannel_Values_List.append(bus_index_1)
        if bus_index_1 == '.':
            messagebox.showerror(title='', message='Select BUSCHANNEL TYPE')
        elif bus_index_1 == 'DEFAULT':
            messagebox.showinfo(title='Config', message='Config File Created with DEFAULT BUSCHANNEL VALUES')
        elif bus_index_1 == 'USER':
            uservalues_0 = scan_low_opt.get()
            Buschannel_Values_List.append(uservalues_0)
            uservalues_1 = bpil_left_opt.get()
            Buschannel_Values_List.append(uservalues_1)
            uservalues_2 = bpil_right_opt.get()
            Buschannel_Values_List.append(uservalues_2)
            uservalues_3 = ecu_ethernet_opt.get()
            Buschannel_Values_List.append(uservalues_3)
            uservalues_4 = pcan_low_rear_opt.get()
            Buschannel_Values_List.append(uservalues_4)
            uservalues_5 = pcan_low_front_opt.get()
            Buschannel_Values_List.append(uservalues_5)
            uservalues_6 = pcan_mid_rl_opt.get()
            Buschannel_Values_List.append(uservalues_6)
            uservalues_7 = pcan_mid_rr_opt.get()
            Buschannel_Values_List.append(uservalues_7)
            uservalues_8 = pcan_mid_fr_opt.get()
            Buschannel_Values_List.append(uservalues_8)
            uservalues_9 = pcan_mid_fl_opt.get()
            Buschannel_Values_List.append(uservalues_9)
            uservalues_10 = vcan_low_opt.get()
            Buschannel_Values_List.append(uservalues_10)
            uservalues_11 = vcan_ecu_opt.get()
            Buschannel_Values_List.append(uservalues_11)
    elif bus_index_0 == 'MAGNA_X2E' or 'BMW_VIGEM':
        Buschannel_Values_List.append(bus_index_0)

    """Execution Info Values"""
    ex_val_0 = old_xml_val.get()
    Execution_Info_values.append(ex_val_0)

    ex_val_1 = scan_val.get()
    Execution_Info_values.append(ex_val_1)

    ex_val_2 = someip_val.get()
    Execution_Info_values.append(ex_val_2)

    ex_val_3 = vcan_bpil_val.get()
    Execution_Info_values.append(ex_val_3)

    ex_val_4 = vcan_ecu_val.get()
    Execution_Info_values.append(ex_val_4)

    ex_val_5 = vcan_low_val.get()
    Execution_Info_values.append(ex_val_5)

    ex_val_6 = pcan_val.get()
    Execution_Info_values.append(ex_val_6)

    ex_val_7 = sort_val.get()
    Execution_Info_values.append(ex_val_7)

    ex_val_8 = pcan_csv_val.get()
    Execution_Info_values.append(ex_val_8)

    """Generate File Option Values"""
    file_val_0 = timestampdiagnostics_val.get()
    Generate_File_Options_List.append(file_val_0)

    file_val_1 = canidtimestamp_val.get()
    Generate_File_Options_List.append(file_val_1)

    file_val_2 = diagnostics_val.get()
    Generate_File_Options_List.append(file_val_2)

    file_val_3 = frameworktiming_val.get()
    Generate_File_Options_List.append(file_val_3)

    """Create CSV File Option Values"""
    csv_val_0 = objectlist_val.get()
    CSV_Options_List.append(csv_val_0)

    csv_val_1 = detection_rr_val.get()
    CSV_Options_List.append(csv_val_1)

    csv_val_2 = detection_rl_val.get()
    CSV_Options_List.append(csv_val_2)

    csv_val_3 = detection_fr_val.get()
    CSV_Options_List.append(csv_val_3)

    csv_val_4 = detection_fl_val.get()
    CSV_Options_List.append(csv_val_4)

    csv_val_5 = detection_bpil_left_val.get()
    CSV_Options_List.append(csv_val_5)

    csv_val_6 = detection_bpil_right_val.get()
    CSV_Options_List.append(csv_val_6)

    csv_val_7 = vcan_alert_shr_val.get()
    CSV_Options_List.append(csv_val_7)

    csv_val_8 = vcsn_alert_svl_val.get()
    CSV_Options_List.append(csv_val_8)

    """XML File Creation Option Values"""
    xml_val_0 = xml_objectlist_val.get()
    XML_Options_List.append(xml_val_0)

    xml_val_1 = xml_detection_rr_val.get()
    XML_Options_List.append(xml_val_1)

    xml_val_2 = xml_detection_rl_val.get()
    XML_Options_List.append(xml_val_2)

    xml_val_3 = xml_detection_fr_val.get()
    XML_Options_List.append(xml_val_3)

    xml_val_4 = xml_detection_fl_val.get()
    XML_Options_List.append(xml_val_4)

    xml_val_5 = xml_freespace_val.get()
    XML_Options_List.append(xml_val_5)

    xml_val_6 = xml_restmessage_val.get()
    XML_Options_List.append(xml_val_6)

    if len(output_path_list) == 0:
        messagebox.showerror(title='Output Path', message='Please Select the Output Path')
    elif Other_Options_List[0] == 'Select Execution Type':
        messagebox.showerror(title='', message='Please Select All Options')
    elif Other_Options_List[1] == 'Select Customer':
        messagebox.showerror(title='', message='Please Select All Options')
    elif Other_Options_List[2] == 'Select Version':
        messagebox.showerror(title='', message='Please Select All Options')
    elif Other_Options_List[3] == 'Select Variant':
        messagebox.showerror(title='', message='Please Select All Options')
    elif Other_Options_List[4] == 'CANDECODER Type':
        messagebox.showerror(title='CANDECODER', message='Please Select CANDECODER Type')
    elif bus_index_0 == 'Select Variant':
        messagebox.showerror(title='Log Type', message='Please Select the Log Type')
    elif bus_index_0 == "BMW_CANOE":
        for value in Buschannel_Values_List[2:]:
            if value > str('18'):
                messagebox.showerror(title='Buschannel Value', message="Enter BUSCHANNEL Value less-than 18")
                break
            elif value == '':
                messagebox.showerror(title='Buschannel Value', message='Please Enter BUSCHANNEL Value')
                break
    else:
        Bordnet_XML(output_path_list, bordnet_json_list, Other_Options_List, Buschannel_Values_List,
                    Execution_Info_values, Generate_File_Options_List, CSV_Options_List, XML_Options_List)
        messagebox.showinfo(title="Config XML", message="Config XML Created Succesfully")
        bordnet_xml_path = os.path.abspath("bordnet_config.xml")
        Config_XML_list.clear()
        Config_XML_list.append(bordnet_xml_path)


def Bordnet_Execution_Start():
    """if len(Config_XML_list) <= 0:
        messagebox.showerror(title='Config XML', message='Please Create Config XML')
    else:
        Bordnet_Tool_Execution()"""
    Bordnet_Tool_Execution()


def bordnet_buttons():
    global json_button
    json_button = Button(root, text='CREATE JSON', font=('arial', 10, 'bold'), height=2, width=14,
                         command=bordnet_json_creator)
    json_button.place(x=590, y=40)

    xml_button = Button(text='CREATE \n CONFIG XML', font=('arial', 10, 'bold'), height=2, width=12,
                        command=Create_Bordnet_XML)
    xml_button.place(x=940, y=450)

    start_button = Button(text='START \n BORDNET TOOL', font=('arial', 10, 'bold'), height=2, width=12,
                          command=Bordnet_Execution_Start)
    start_button.place(x=1050, y=450)

def call_labels():
    display_labels()
    Tool_info()
    help_label = Label(root, text="Help : sravan.kumar.devatha@aptiv.com",
                       font=('arial', 9, 'bold'), bg=Bg_colour)
    help_label.place(x=950, y=556)
