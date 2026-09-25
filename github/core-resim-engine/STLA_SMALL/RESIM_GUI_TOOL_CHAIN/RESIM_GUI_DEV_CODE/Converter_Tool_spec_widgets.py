from tkinter import *
from tkinter.ttk import *
from tkinter import messagebox
from Main import *
from tkinter import filedialog
from main_interface import *
from Converter_Tool_XML_writer import *
from Converter_Tool_JSON_writer import *
from os import *
from io import *
import shutil
import multiprocessing


def call_all():
    Display_Input_Options()
    Display_Output_Options()
    other_options()
    clear_files_list()
    browse_files()
    display_space()
    buttons()
    call_labels()
    Converter_Clear_all()
    return


def Display_Input_Options():
    global Converter_Groupbox
    Converter_Groupbox = Canvas(height=620, width=1200, bg=Bg_colour)
    Converter_Groupbox.pack()

    Converter_Groupbox.create_rectangle(25, 40, 210, 340, fill=None)

    Label(root, text='Select Input Format', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=30, y=30)

    global orcas_inp
    orcas_inp = IntVar()
    orcas_inp.set(0)
    global orcas_checkbox
    orcas_checkbox = Checkbutton(root, text='ORCAS MDF4', variable=orcas_inp, onvalue=1, offvalue=0,
                                 font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=60)

    global pcap_inp
    pcap_inp = IntVar()
    pcap_inp.set(0)
    global pcap_checkbox
    pcap_checkbox = Checkbutton(root, text='PCAP', variable=pcap_inp, onvalue=1, offvalue=0,
                                       font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=90)

    global autera_inp
    autera_inp = IntVar()
    autera_inp.set(0)
    global autera_checkbox
    autera_checkbox = Checkbutton(root, text='AUTERA', variable=autera_inp, onvalue=1, offvalue=0,
                                font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=120)

    global canape_inp
    canape_inp = IntVar()
    canape_inp.set(0)
    global canape_checkbox
    canape_checkbox = Checkbutton(root, text='CANAPE MDF4', variable=canape_inp, onvalue=1, offvalue=0,
                                  font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=150)

    global canape_can_inp
    canape_can_inp = IntVar()
    canape_can_inp.set(0)
    global canape_can_checkbox
    canape_can_checkbox = Checkbutton(root, text='CANAPE CAN MDF4', variable=canape_can_inp, onvalue=1, offvalue=0,
                                  font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=180)

    global vigem_inp
    vigem_inp = IntVar()
    vigem_inp.set(0)
    global vigem_checkbox
    vigem_checkbox = Checkbutton(root, text='VIGEM MDF4', variable=vigem_inp, onvalue=1, offvalue=0,
                                 font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=210)

    global vigem_vpcap_inp
    vigem_vpcap_inp = IntVar()
    vigem_vpcap_inp.set(0)
    global vigem_vpcap_checkbox
    vigem_vpcap_checkbox = Checkbutton(root, text='VIGEM VPCAP', variable=vigem_vpcap_inp, onvalue=1, offvalue=0,
                                       font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=240)

    global x2e_inp
    x2e_inp = IntVar()
    x2e_inp.set(0)
    global x2e_checkbox
    x2e_checkbox = Checkbutton(root, text='X2E', variable=x2e_inp, onvalue=1, offvalue=0,
                                       font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=270)

    global resim_vigem_inp
    resim_vigem_inp = IntVar()
    resim_vigem_inp.set(0)
    global resim_vigem_checkbox
    resim_vigem_checkbox = Checkbutton(root, text='RESIM VIGEM MDF4', variable=resim_vigem_inp, onvalue=1, offvalue=0,
                                       font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=300)


def Display_Output_Options():
    Converter_Groupbox.create_rectangle(235, 40, 400, 340, fill=None)
    Label(root, text='Select Output Format', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=240, y=30)

    global single_dvsu_opt
    single_dvsu_opt = IntVar()
    single_dvsu_opt.set(0)
    global single_dvsu_checkbox
    single_dvsu_checkbox = Checkbutton(text='SINGLE DVSU', variable=single_dvsu_opt, onvalue=1, offvalue=0,
                                       font=('arial', 10, 'bold'), bg=Bg_colour).place(x=250, y=60)

    global four_dvsu_opt
    four_dvsu_opt = IntVar()
    four_dvsu_opt.set(0)
    global four_dvsu_checkbox
    four_dvsu_checkbox = Checkbutton(text='FOUR DVSU', variable=four_dvsu_opt, onvalue=1, offvalue=0,
                                     font=('arial', 10, 'bold'), bg=Bg_colour).place(x=250, y=90)

    global orcas_opt
    orcas_opt = IntVar()
    orcas_opt.set(0)
    global orcas_opt_checkbox
    orcas_opt_checkbox = Checkbutton(text='ORCAS MDF4', variable=orcas_opt, onvalue=1, offvalue=0,
                                     font=('arial', 10, 'bold'), bg=Bg_colour).place(x=250, y=120)

    global canoe_opt
    canoe_opt = IntVar()
    canoe_opt.set(0)
    global canoe_opt_checkbox
    canoe_opt_checkbox = Checkbutton(text='CANoe MDF4', variable=canoe_opt, onvalue=1, offvalue=0,
                                     font=('arial', 10, 'bold'), bg=Bg_colour).place(x=250, y=150)

    global vigem_opt
    vigem_opt = IntVar()
    vigem_opt.set(0)
    global vigem_opt_checkbox
    vigem_opt_checkbox = Checkbutton(text='VIGEM MDF4', variable=vigem_opt, onvalue=1, offvalue=0,
                                     font=('arial', 10, 'bold'), bg=Bg_colour).place(x=250, y=180)

    global vigem_someip_opt
    vigem_someip_opt = IntVar()
    vigem_someip_opt.set(0)
    global vigem_someip_opt_checkbox
    vigem_someip_opt_checkbox = Checkbutton(text='VIGEM SOMEIP', variable=vigem_someip_opt, onvalue=1,
                                            offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour).place(x=250, y=210)

    global vigem_vpcap_opt
    vigem_vpcap_opt = IntVar()
    vigem_vpcap_opt.set(0)
    global vigem_vpcap_opt_checkbox
    vigem_vpcap_opt_checkbox = Checkbutton(text='VIGEM VPCAP', variable=vigem_vpcap_opt, onvalue=1,
                                           offvalue=0, font=('arial', 10, 'bold'), bg=Bg_colour).place(x=250, y=240)

    global video_opt
    video_opt = IntVar()
    video_opt.set(0)
    global video_opt_checkbox
    video_opt_checkbox = Checkbutton(text='VIDEO OUTPUT', variable=video_opt, onvalue=1, offvalue=0,
                                     font=('arial', 10, 'bold'), bg=Bg_colour).place(x=250, y=270)


global pathlist
pathlist = []

'''Creating Method to call merge and only srr files options'''


def other_options():
    Converter_Groupbox.create_rectangle(425, 373, 1100, 255, fill=None)
    Label(root, text="Converter Config", font=('arial', 10, 'bold'), bg=Bg_colour).place(x=430, y=245)
    Label(root, text="Converter Config Version: 1.7", font=('arial', 10, 'bold', 'italic'), bg=Bg_colour).place(x=40, y=460)

    """Creating Option to convert only SRR DEBUG FILES"""

    global only_srr
    only_srr = IntVar()
    only_srr.set(0)
    global only_SRR
    only_SRR = Checkbutton(root, text='Convert Only SRR DEBUG Files', font=('arial', 10, 'bold'), variable=only_srr,
                           onvalue=1, offvalue=0, bg=Bg_colour, command=browse_files)
    only_SRR.place(x=590, y=50)

    '''Creating option to Merge all converted files into a single file'''
    global mrg_all
    mrg_all = IntVar()
    mrg_all.set(0)
    global MRG_all
    MRG_all = Checkbutton(root, text='Merge All Traces To a Single File', font=('arial', 10, 'bold'), variable=mrg_all,
                          onvalue=1, offvalue=0, bg=Bg_colour)
    MRG_all.place(x=800, y=310)

    '''Creating option for selecting CSV needed or not'''
    global csv_val
    csv_val = IntVar()
    csv_val.set(0)
    global csv_need
    csv_need = Checkbutton(root, text='CSV DUMP', font=('arial', 10, 'bold'), variable=csv_val, onvalue=1, offvalue=0,
                           bg=Bg_colour)
    csv_need.place(x=800, y=280)

    global ignore_cdc_val
    ignore_cdc_val = StringVar()
    ignore_cdc_val.set('FALSE')
    global ignore_cdc_checkbox
    ignore_cdc_checkbox = Checkbutton(root, text='IGNORE CDC', font=('arial', 10, 'bold'), variable=ignore_cdc_val,
                           onvalue='TRUE', offvalue='FALSE', bg=Bg_colour)
    ignore_cdc_checkbox.place(x=920, y=280)

    Label(root, text='Save Converted Traces at:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=440, y=270)

    global path_val
    path_val = StringVar()
    path_val.set("SAME_AS_INPUT")
    global debug_path
    debug_path = Radiobutton(root, text='SAME AS INPUT (SRR DEBUG FOLDER)', font=('arial', 10, 'bold'),
                             variable=path_val, value="SAME_AS_INPUT", bg=Bg_colour, command=changes)
    debug_path.place(x=440, y=295)

    global inp_folder
    inp_folder = Radiobutton(root, text='CREATE OUTPUT IN CONVERTED FOLDER', font=('arial', 10, 'bold'),
                             variable=path_val, value="CONVERTED", bg=Bg_colour, command=changes)
    inp_folder.place(x=440, y=320)

    global new_path
    new_path = Radiobutton(root, text='NEW OUTPUT PATH', font=('arial', 10, 'bold'), variable=path_val,
                           value="NEW OUTPUT PATH", bg=Bg_colour, command=getpath)
    new_path.place(x=440, y=345)


def getpath():
    global new_output_path_opt
    if platform == "linux" or platform == 'linux2':
        new_output_path_opt = Entry(width=33)
        new_output_path_opt.place(x=605, y=345)
    else:
        new_output_path_opt = Entry(width=48)
        new_output_path_opt.place(x=600, y=347)

    def ask_newoutput_path():
        pathlist.clear()
        new_output_dir = filedialog.askdirectory()
        pathlist.append(new_output_dir)
        new_output_path_opt.insert(END, new_output_dir)

    global new_output_button
    new_output_button = Button(text='Browse', command=lambda: ask_newoutput_path())
    if platform == "linux" or platform == 'linux2':
        new_output_button.place(x=880, y=340)
    else:
        new_output_button.place(x=900, y=342)
    return new_output_path_opt, new_output_button


def changes():
    try:
        new_output_path_opt.config(state=DISABLED if path_val.get() == "SAME_AS_INPUT" or "CONVERTED" else NORMAL)
        new_output_button.config(state=DISABLED if path_val.get() == "SAME_AS_INPUT" or "CONVERTED" else NORMAL)
    except:
        pass


'''Creating Lists for each file option'''
global bn_califr_list
bn_califr_list = []

global faseth_list
faseth_list = []

global srr_debug_list
srr_debug_list = []

global srr_reference_list
srr_reference_list = []

global file_size_list
file_size_list = []
file_size_list.clear()

'''Creating Method for Calling browse files options'''


def browse_files():
    Converter_Groupbox.create_rectangle(425, 40, 960, 235, fill=None)
    Label(text="Create JSON/Flist", bg=Bg_colour, font=('arial', 10, 'bold')).place(x=430, y=30)
    """Creating Browsing File option for files"""

    """BN CALIFR"""

    global bn_califr_file_opt
    bn_califr_file_label = Label(text='BN CALIFR Files: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=440, y=80)
    bn_califr_file_opt = Entry()
    if platform == "linux" or platform == 'linux2':
        bn_califr_file_opt.config(width=35)
    else:
        bn_califr_file_opt.config(width=50)
    bn_califr_file_opt.place(x=590, y=80)
    bn_califr_file_opt.config(state=DISABLED if only_srr.get() == 1 else NORMAL)

    def browse_bn_califr_files():
        bn_califr_files = filedialog.askopenfilenames(title='Select BN Califr Files',
                                                      filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for b in bn_califr_files:
            bn_califr_list.append(b)
            file_size_list.append(b)

        bn_califr_file_opt.insert(END, bn_califr_files)

    global bn_califr_file_button
    bn_califr_file_button = Button(text='Browse', command=lambda: browse_bn_califr_files())
    if platform == "linux" or platform == 'linux2':
        bn_califr_file_button.place(x=880, y=75)
    else:
        bn_califr_file_button.place(x=900, y=75)
    bn_califr_file_button.config(state=DISABLED if only_srr.get() == 1 else NORMAL)

    """FASETH"""

    faseth_file_label = Label(text='BN FASETH Files: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=440, y=110)
    global faseth_file_opt
    faseth_file_opt = Entry()
    if platform == "linux" or platform == 'linux2':
        faseth_file_opt.config(width=35)
    else:
        faseth_file_opt.config(width=50)
    faseth_file_opt.place(x=590, y=110)
    faseth_file_opt.config(state=DISABLED if only_srr.get() == 1 else NORMAL)

    def browse_faseth_files():
        faseth_files = filedialog.askopenfilenames(title='Select FASETH Files',
                                                   filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for f in faseth_files:
            faseth_list.append(f)
            file_size_list.append(f)

        faseth_file_opt.insert(END, faseth_files)

    global faseth_file_button
    faseth_file_button = Button(text='Browse', command=lambda: browse_faseth_files())
    if platform == "linux" or platform == 'linux2':
        faseth_file_button.place(x=880, y=105)
    else:
        faseth_file_button.place(x=900, y=105)
    faseth_file_button.config(state=DISABLED if only_srr.get() == 1 else NORMAL)

    """SRR DEBUG"""

    srr_debug_file_label = Label(text='SRR DEBUG Files: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=440, y=140)
    global srr_debug_file_opt
    srr_debug_file_opt = Entry()
    if platform == "linux" or platform == 'linux2':
        srr_debug_file_opt.config(width=35)
    else:
        srr_debug_file_opt.config(width=50)
    srr_debug_file_opt.place(x=590, y=140)

    def browse_srr_debug_files():
        srr_debug_files = filedialog.askopenfilenames(title='Select SRR DEBUG Files',
                                                      filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for d in srr_debug_files:
            srr_debug_list.append(d)
            file_size_list.append(d)

        srr_debug_file_opt.insert(END, srr_debug_files)

    srr_debug_file_button = Button(text='Browse', command=lambda: browse_srr_debug_files())
    if platform == "linux" or platform == 'linux2':
        srr_debug_file_button.place(x=880, y=135)
    else:
        srr_debug_file_button.place(x=900, y=135)


    '''SRR REFERENCE'''

    Label(text='SRR REFERENCE Files: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=440, y=170)
    global srr_ref_file_opt
    srr_ref_file_opt = Entry()
    if platform == "linux" or platform == 'linux2':
        srr_ref_file_opt.config(width=35)
    else:
        srr_ref_file_opt.config(width=50)
    srr_ref_file_opt.place(x=590, y=170)
    srr_ref_file_opt.config(state=DISABLED if only_srr.get() == 1 else NORMAL)

    def browse_srr_ref_files():
        srr_ref_files = filedialog.askopenfilenames(title='Select SRR REFERENCE Files',
                                                    filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for r in srr_ref_files:
            srr_reference_list.append(r)
            file_size_list.append(r)
        srr_ref_file_opt.insert(END, srr_ref_files)

    global srr_ref_file_button
    srr_ref_file_button = Button(text='Browse', command=lambda: browse_srr_ref_files())
    if platform == 'linux' or platform == 'linux2':
        srr_ref_file_button.place(x=880, y=165)
    else:
        srr_ref_file_button.place(x=900, y=165)
    srr_ref_file_button.config(state=DISABLED if only_srr.get() == 1 else NORMAL)


def clear_files_list():
    bn_califr_list.clear()
    faseth_list.clear()
    srr_debug_list.clear()
    srr_reference_list.clear()


global json_file_list
json_file_list = []


def calljsonpath():
    global jsonpath
    jsonpath = os.path.abspath('BMW_fList.json')
    global jsonname
    jsonname = Label(root, text='JSON File:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=375)
    global json_label
    json_label = Label(root, text=jsonpath, font=('arial', 10, 'bold'), bg=Bg_colour).place(x=115, y=375)
    json_file_list.append(jsonpath)
    display_space()
    return jsonpath


xml_file_list = []  # creating a list to store the xml file path for validation


def callxmlpath():
    global xmlpath
    xmlpath = os.path.abspath('Mdf4_Converter_Config.xml')
    xml_file_list.append(xmlpath)
    global xmlname
    xmlname = Label(root, text='XML File:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=400)
    global xml_label
    xml_label = Label(root, text=xmlpath, font=('arial', 10, 'bold'), bg=Bg_colour).place(x=115, y=400)
    return xmlpath


inp_values_list = []  # Creating List for getting input checkbox values and used for XML Writer

opt_values_list = []  # Creating List for getting output checkbox values and used for XML Writer

other_values_list = []  # Creating List for getting other options checkbox values and used for XML Writer

path_inp_list = []

# Creating Method to Create XML File and appending checkbox values to above list
# values_list values/index are displayed after HASH symbol used in XML Writer
def Create_XML():
    inp_values_list.clear()
    opt_values_list.clear()
    other_values_list.clear()
    path_inp_list.clear()

    inp1 = orcas_inp.get()
    inp_values_list.append(inp1)  # 0
    inp2 = pcap_inp.get()
    inp_values_list.append(inp2)  # 1
    inp3 = autera_inp.get()
    inp_values_list.append(inp3)  # 2
    inp4 = canape_inp.get()
    inp_values_list.append(inp4)  # 3
    inp5 = canape_can_inp.get()
    inp_values_list.append(inp5)  # 4
    inp6 = vigem_inp.get()
    inp_values_list.append(inp6)  # 5
    inp7 = vigem_vpcap_inp.get()
    inp_values_list.append(inp7)  # 6
    inp8 = x2e_inp.get()
    inp_values_list.append(inp8)  # 7
    inp9 = resim_vigem_inp.get()
    inp_values_list.append(inp9)  # 8

    opt1 = single_dvsu_opt.get()
    opt_values_list.append(opt1)  # 0
    opt2 = four_dvsu_opt.get()
    opt_values_list.append(opt2)  # 1
    opt3 = orcas_opt.get()
    opt_values_list.append(opt3)  # 2
    opt4 = canoe_opt.get()
    opt_values_list.append(opt4)  # 3
    opt5 = vigem_opt.get()
    opt_values_list.append(opt5)  # 4
    opt6 = vigem_someip_opt.get()
    opt_values_list.append(opt6)  # 5
    opt7 = vigem_vpcap_opt.get()
    opt_values_list.append(opt7)  # 6
    opt8 = video_opt.get()
    opt_values_list.append(opt8)  # 7

    srr_only = only_srr.get()
    other_values_list.append(srr_only)  # 0
    mrgall = mrg_all.get()
    other_values_list.append(mrgall)  # 1
    needcsv = csv_val.get()
    other_values_list.append(needcsv)  # 2
    ignore_cdc = ignore_cdc_val.get()
    other_values_list.append(ignore_cdc)  # 3

    savefiles = path_val.get()
    path_inp_list.append(savefiles)

    global inpsum
    inpsum = inp1 + inp2 + inp3 + inp4 + inp5 + inp6 + inp7 + inp8 + inp9
    global optsum
    optsum = opt1 + opt2 + opt3 + opt4 + opt5 + opt6 + opt7 + opt8

    if inpsum > 1 or inpsum == 0:
        messagebox.showerror(title='Input Format', message='!!! SELECT ONLY ONE INPUT FORMAT !!!')
        inp_values_list.clear()
    elif optsum < 1 or optsum == 0:
        messagebox.showerror(title='Output Format', message='!!! SELECT ONE OUTPUT FORMAT !!!')
        opt_values_list.clear()
    elif len(json_file_list) == 0:
        messagebox.showerror(title='JSON File', message='!!! CREATE JSON FILE !!!')
        json_file_list.clear()
    elif path_inp_list[0] == "NEW OUTPUT PATH":
        if len(pathlist) <= 0:
            messagebox.showerror(title='Output Path', message='!!! PLEASE SELECT OUTPUT PATH !!!')
            pathlist.clear()
        else:
            if os.path.exists("Mdf4_Converter_Config.xml"):
                os.remove("Mdf4_Converter_Config.xml")
            xml_file_list.clear()
            Converter_XML_writer(inp_values_list, opt_values_list, other_values_list, json_file_list, path_inp_list,
                                 pathlist)
            messagebox.showinfo(title='Input Format', message='XML File Created Successfully')
            callxmlpath()
    else:
        if os.path.exists("Mdf4_Converter_Config.xml"):
            os.remove("Mdf4_Converter_Config.xml")
        xml_file_list.clear()
        Converter_XML_writer(inp_values_list, opt_values_list, other_values_list, json_file_list, path_inp_list, pathlist)
        messagebox.showinfo(title='Input Format', message='XML File Created Successfully')
        callxmlpath()


def Create_json():
    srr_only_selected = only_srr.get()
    if len(bn_califr_list or faseth_list or srr_debug_list or srr_reference_list) == 0:
        messagebox.showerror(title='Browse Files', message='!!!SELECT FILES!!!')
    elif srr_only_selected == 1:
        if os.path.exists("BMW_fList.json"):
            os.remove("BMW_fList.json")
        Converter_JSON_writer(bn_califr_list, faseth_list, srr_debug_list, srr_reference_list)
        messagebox.showinfo(title='JSON', message='JSON Created Successfully')
        calljsonpath()
    elif len(bn_califr_list) == len(faseth_list) == len(srr_debug_list) == len(srr_reference_list):
        if os.path.exists("BMW_fList.json"):
            os.remove("BMW_fList.json")
        Converter_JSON_writer(bn_califr_list, faseth_list, srr_debug_list, srr_reference_list)
        messagebox.showinfo(title='JSON', message='JSON Created Successfully')
        calljsonpath()
    else:
        messagebox.showerror(title='Browse Files', message='!!! PLEASE SELECT ALL THE FILES !!!')


def start_conversion():
    if len(json_file_list) == 0:
        messagebox.showwarning(title='EXECUTION', message='!!! Create JSON and CONFIG XML !!!')
    elif len(xml_file_list) == 0:
        messagebox.showwarning(title='EXECUTION', message='!!! Create Config XML !!!')
    else:
        displayexepath = os.path.abspath('mdf_udpData_Proc.exe')
        exename = Label(root, text='EXE File:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=40, y=425)
        exe_label = Label(root, text=displayexepath, font=('arial', 10, 'bold'), bg=Bg_colour).place(x=115, y=425)
        """Converting_Label = Label(text="Conversion in Progress... ", font=('arial', 11, 'italic', 'bold'), bg=Bg_colour)
        Converting_Label.place(x=260, y=460)"""
        Execute_Converter_Command()


def display_space():
    Converter_Groupbox.create_rectangle(975, 235, 1170, 40, fill=None)
    Label(root, text="System Space", font=('arial', 10, 'bold'), bg=Bg_colour).place(x=980, y=30)
    total, used, free = shutil.disk_usage("/")

    Label(root, text='Total Space:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=990, y=60)
    Label(root, text="%d GB" % (total // (2 ** 30)), font=('arial', 10, 'bold'), bg=Bg_colour).place(x=1110, y=60)

    Label(root, text='Used Space:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=990, y=90)
    Label(root, text="%d GB" % (used // (2 ** 30)), font=('arial', 10, 'bold'), bg=Bg_colour).place(x=1110, y=90)

    Label(root, text='Free Space:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=990, y=120)
    Label(root, text="%d GB" % (free // (2 ** 30)), font=('arial', 10, 'bold'), bg=Bg_colour).place(x=1110, y=120)

    global filesize
    filesize = 0
    for i in file_size_list:
        filename = i
        file_stats = os.stat(filename)
        bytesize = file_stats.st_size
        filesize = filesize + bytesize

    size = (((filesize / 1024) / 1024) / 1024)
    finalsize = "{:.2f}".format(size)

    Label(root, text='Total Files Size:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=990, y=150)
    Label(root, text=finalsize + ' GB', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=1110, y=150)

    available = size - free
    after = "{:.2f}".format(available)

    if size >= free:
        messagebox.showwarning(title='System Space', message="!!! YOU DON'T HAVE ENOUGH SPACE !!!")
    '''
    Label(root, text='Approx After Conversion:', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=960, y=170)
    Label(root, text=after + ' GB', font=('arial', 10, 'bold'), bg=Bg_colour).place(x=1100, y=190)
    '''
    # print("Total: %d GB" % (total // (2 ** 30)))
    # print("Used: %d GB" % (used // (2 ** 30)))
    # print("Free: %d GB" % (free // (2 ** 30)))


'''Creating Method for Buttons'''


def buttons():
    Button(text='CREATE JSON', font=('arial', 10, 'bold'), height=1, width=12, command=Create_json).place(x=650, y=200)

    clear_button = Button(text='HOME', font=('arial', 10, 'bold'), height=2, width=12,
                          command=Converter_Clear_all).place(x=40, y=490)

    create_config_button = Button(text='CREATE \n CONFIG XML', font=('arial', 10, 'bold'), command=Create_XML, height=2,
                                  width=12).place(x=160, y=490)

    Execute_button = Button(text='START \n CONVERSION', font=('arial', 10, 'bold'), height=2, width=12,
                            command=start_conversion).place(x=280, y=490)


'''Creating Method to clear all the fields - Clear all button'''


def Converter_Clear_all():
    orcas_inp.set(0)
    canape_inp.set(0)
    vigem_inp.set(0)
    vigem_vpcap_inp.set(0)
    resim_vigem_inp.set(0)
    single_dvsu_opt.set(0)
    four_dvsu_opt.set(0)
    orcas_opt.set(0)
    canoe_opt.set(0)
    vigem_opt.set(0)
    vigem_someip_opt.set(0)
    vigem_vpcap_opt.set(0)
    only_srr.set(0)
    mrg_all.set(0)
    csv_val.set(0)
    bn_califr_file_opt.delete(0, END)
    faseth_file_opt.delete(0, END)
    srr_debug_file_opt.delete(0, END)
    srr_ref_file_opt.delete(0, END)
    inp_values_list.clear()
    opt_values_list.clear()
    other_values_list.clear()
    json_file_list.clear()
    bn_califr_list.clear()
    faseth_list.clear()
    srr_debug_list.clear()
    srr_reference_list.clear()
    return


def call_labels():
    display_labels()
    Tool_info()
    Label(root, text="Help : sravan.kumar.devatha@aptiv.com", font=('arial', 9, 'bold'), bg=Bg_colour).place(x=950, y=556)

