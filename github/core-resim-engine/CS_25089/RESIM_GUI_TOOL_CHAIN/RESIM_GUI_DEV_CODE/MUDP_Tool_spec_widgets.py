from tkinter import *
from tkinter.ttk import *
import tkinter as tk
from tkinter import filedialog
from tkinter import messagebox
# from file_read import *
# from file_write import *
from os import system
# from Main import *
from main_interface import *

from MUDP_Main import *
from MUDP_Json_Writer import *
from MUDP_FList import *
from MUDP_XML_writer import *

global sensor
sensor = [0, 0, 0, 0, 0, 0]

global STREAM_OPTION

global sen_ecu_stream
sen_ecu_stream = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
# global Ecu_stream
# Ecu_stream = [0,0,0]

global Verified_label
Verified_label = []

global sensor_active
global Radar_ecu_active
sensor_active = False
Radar_ecu_active = False

global STREAM_OPTION_SET
STREAM_OPTION_SET = 1

global TOOL_CONFIG
global Tool_library
global output_xml_trace
global Tool_library_label
global output_xml_trace_label
global Tool_OUTPUT_label
global Tool_OUTPUT
global csv
global Xml_Trace_Mode
global detail_error_info
global packet_loss_statistics
global log_quality_summary
'''global packet_loss_report_label
global report_text_format_RB
global report_xml_format_RB'''
global report_format
global file_mode_label
global file_mode_con_rb
global file_mode_seq_rb
global file_mode
global output_xml_trace_val
global report_format1
TOOL_CONFIG = None
Tool_library = None
output_xml_trace = None
# Tool_library_label = None
# output_xml_trace_label = None
# Tool_OUTPUT_label = None
Tool_OUTPUT = None
report_format1 = '.'

global browse_output_path_label
global browse_output_entry
global browse_output_path_button
global HOME
global START
global opath
browse_output_path_label = None
HOME = None
START = None
opath = None


'''global Set_Browse_OUTPUT_PATH
Set_Browse_OUTPUT_PATH = 0'''

global sum_stream
sum_stream = 0

global SET_LIB
SET_LIB = 0

global entries
entries = []


############################
class Global_var_class(object):
    def __init__(self):
        self.Library = None
        self.trace = None
        self.output_path = None
        # self.output_new_path = 'C:/Workspaces/SRR5_logs/RNA_MF41'
        self.CCA_Library = None
        self.Vector_Library = None
        self.HIL_port_xml = None
        self.Sensor_xml = None


class Sensor_and_ECU(object):
    def __init__(self):
        self.REAR_LEFT = None
        self.REAR_RIGHT = None
        self.FRONT_LEFT = None
        self.FRONT_RIGHT = None
        self.BP_LEFT = None
        self.BP_RIGHT = None
        self.RADAR_ECU = None


'''class Data_Extraction_option(object):
    def __init__(self):
        self.csv = None
        self.Xml_Trace_Mode = None
        self.detail_error_info = None
        self.packet_loss_statistics = None
        self.log_quality_summary = None
'''


class stream_options(object):
    def __init__(self):
        self.STREAM_HEADER = None
        self.CDC = None
        self.DSPACE = None
        self.CCA_HDR = None
        self.OSI_STREAM = None
        self.Z4_CORE = None
        self.Z7A_CORE = None
        self.Z7B_CORE = None
        self.Z4_CUST = None
        self.Z7A_CUST = None
        self.ECU_1 = None
        self.ECU_2 = None
        self.ECU_3 = None
        self.ECU_VRU_CLASSIFIER = None


class Sensor_and_ECU_position(object):
    def __init__(self):
        self.REAR_LEFT = 0
        self.REAR_RIGHT = 1
        self.FRONT_LEFT = 2
        self.FRONT_RIGHT = 3
        self.BP_LEFT = 4
        self.BP_RIGHT = 5
        self.RADAR_ECU = 6


class stream_options_position(object):
    def __init__(self):
        self.STREAM_HEADER = 0
        self.CDC = 1
        self.DSPACE = 2
        self.CCA_HDR = 3
        self.OSI_STREAM = 4
        self.Z4_CORE = 5
        self.Z7A_CORE = 6
        self.Z7B_CORE = 7
        self.Z4_CUST = 8
        self.Z7A_CUST = 9
        self.ECU_1 = 10
        self.ECU_2 = 11
        self.ECU_3 = 12
        self.ECU_VRU_CLASSIFIER = 13


'''class Data_Extraction_position(object):
    def __init__(self):
        self.csv = 0
        self.Xml_Trace_Mode = 1
        self.detail_error_info = 2
        self.packet_loss_statistics = 3
        self.log_quality_summary = 4
'''


class stream_options_chkbtn(object):
    def __init__(self):
        self.STREAM_HEADER_chkbtn = None
        self.CDC_chkbtn = None
        self.DSPACE_chkbtn = None
        self.CCA_HDR_chkbtn = None
        self.OSI_STREAM_chkbtn = None
        self.Z4_CORE_chkbtn = None
        self.Z7A_CORE_chkbtn = None
        self.Z7B_CORE_chkbtn = None
        self.Z4_CUST_chkbtn = None
        self.Z7A_CUST_chkbtn = None
        self.ECU_1_chkbtn = None
        self.ECU_2_chkbtn = None
        self.ECU_3_chkbtn = None
        self.ECU_VRU_CLASSIFIER_chkbtn = None


'''class Data_Extraction_chkbtn(object):
    def __init__(self):
        self.csv_chkbtn = None
        self.Xml_Trace_Mode_chkbtn = None
        self.detail_error_info_chkbtn = None
        self.packet_loss_statistics_chkbtn = None
        self.log_quality_summary_chkbtn = None'''

global Global_var_class_obj
Global_var_class_obj = Global_var_class()

global Sen_pos
Sen_pos = Sensor_and_ECU_position()

global Stream_pos
Stream_pos = stream_options_position()

global stream_options_obj
stream_options_obj = stream_options()

global Sensor_and_ECU_obj
Sensor_and_ECU_obj = Sensor_and_ECU()

global stream_options_chkbtn_obj
stream_options_chkbtn_obj = stream_options_chkbtn()

global bn_califr_MUDP_log_list
bn_califr_MUDP_log_list = []

global faseth_MUDP_log_list
faseth_MUDP_log_list = []

global srr_debug_MUDP_log_list
srr_debug_MUDP_log_list = []

global srr_reference_MUDP_log_list
srr_reference_MUDP_log_list = []

global json_file_log_list
json_file_log_list = []

global filelist
filelist = []


'''global Data_Extraction_option_obj
Data_Extraction_option_obj = Data_Extraction_option()

global Data_Extraction_option_position
Data_Extraction_option_position = Data_Extraction_position()

global Data_Extraction_chkbtn_obj
Data_Extraction_chkbtn_obj = Data_Extraction_chkbtn()'''


# method to make widget invisible
# or remove from toplevel 


def forget(widget):
    # This will remove the widget from toplevel
    # basically widget do not get deleted
    # it just becomes invisible and loses its position
    # and can be retrieve
    widget.forget()


'''def clear_browse_text():
    entries[0].delete(0, 'end')


def clear_all_entries():
    for ind in range(len(entries)):
        entries[ind].place_forget()
    return'''


def clear_mudp_log_list():
    bn_califr_MUDP_log_list.clear()
    faseth_MUDP_log_list.clear()
    srr_debug_MUDP_log_list.clear()
    srr_reference_MUDP_log_list.clear()
    filelist.clear()


def Update_SENSOR_POSITION(self, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    # Lable placing
    # grid method to arrange labels in respective
    # rows and columns as specified
    global Canvas_MUDP
    Canvas_MUDP = Canvas(height=620, width=1200, bg=Bg_colour)
    Canvas_MUDP.pack()
    # creating rectangle
    Canvas_MUDP.create_rectangle(50, 290, 280, 70, fill=None)
    SENSOR_POSITION = Label(root, text="SENSOR POSITION", font=('arial', 12, 'bold'), bg=Bg_colour)
    SENSOR_POSITION.place(x=SENSOR_POSITION_X_val, y=SENSOR_POSITION_Y_val)
    self.REAR_LEFT = IntVar()
    REAR_LEFT_chkbtn = Checkbutton(root, text="REAR LEFT", variable=self.REAR_LEFT, font=('arial', 10, 'bold'),
                                   bg=Bg_colour,
                                   onvalue=1, offvalue=0,
                                   command=lambda: set_sensor(self, Sen_pos.REAR_LEFT, self.REAR_LEFT))
    self.REAR_RIGHT = IntVar()
    REAR_RIGHT_chkbtn = Checkbutton(root, text="REAR_RIGHT", variable=self.REAR_RIGHT, font=('arial', 10, 'bold'),
                                    bg=Bg_colour, onvalue=1, offvalue=0,
                                    command=lambda: set_sensor(self, Sen_pos.REAR_RIGHT, self.REAR_RIGHT))
    self.FRONT_LEFT = IntVar()
    FRONT_LEFT_chkbtn = Checkbutton(root, text="FRONT LEFT", variable=self.FRONT_LEFT, font=('arial', 10, 'bold'),
                                    bg=Bg_colour, onvalue=1, offvalue=0,
                                    command=lambda: set_sensor(self, Sen_pos.FRONT_LEFT, self.FRONT_LEFT))
    self.FRONT_RIGHT = IntVar()
    FRONT_RIGHT_chkbtn = Checkbutton(root, text="FRONT RIGHT", variable=self.FRONT_RIGHT, font=('arial', 10, 'bold'),
                                     bg=Bg_colour, onvalue=1, offvalue=0,
                                     command=lambda: set_sensor(self, Sen_pos.FRONT_RIGHT, self.FRONT_RIGHT))
    self.BP_LEFT = IntVar()
    BP_LEFT_chkbtn = Checkbutton(root, text="BP LEFT", variable=self.BP_LEFT, font=('arial', 10, 'bold'), bg=Bg_colour,
                                 onvalue=1, offvalue=0, command=lambda: set_sensor(self, Sen_pos.BP_LEFT, self.BP_LEFT))
    self.BP_RIGHT = IntVar()
    BP_RIGHT_chkbtn = Checkbutton(root, text="BP RIGHT", variable=self.BP_RIGHT, font=('arial', 10, 'bold'),
                                  bg=Bg_colour,
                                  onvalue=1, offvalue=0,
                                  command=lambda: set_sensor(self, Sen_pos.BP_RIGHT, self.BP_RIGHT))
    self.RADAR_ECU = IntVar()
    RADAR_ECU_chkbtn = Checkbutton(root, text="RADAR ECU", variable=self.RADAR_ECU, font=('arial', 10, 'bold'),
                                   bg=Bg_colour,
                                   onvalue=1, offvalue=0, command=lambda: set_Radar_ecu(self, self.RADAR_ECU))

    REAR_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val + 10, y=SENSOR_POSITION_Y_val + (offset_y * 1.5))
    REAR_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val + 10, y=SENSOR_POSITION_Y_val + (offset_y * 2.5))
    FRONT_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val + 10, y=SENSOR_POSITION_Y_val + (offset_y * 3.5))
    FRONT_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val + 10, y=SENSOR_POSITION_Y_val + (offset_y * 4.5))
    BP_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val + 10, y=SENSOR_POSITION_Y_val + (offset_y * 5.5))
    BP_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val + 10, y=SENSOR_POSITION_Y_val + (offset_y * 6.5))
    RADAR_ECU_chkbtn.place(x=SENSOR_POSITION_X_val + 10, y=SENSOR_POSITION_Y_val + (offset_y * 7.5))
    return


def set_sensor(self, Index, Val):
    global sensor_active
    global sensor
    sum = 0
    if Val is not None:
        sensor[Index] = Val.get()
    else:
        print('fail', Val)
    for Ind in range(len(sensor)):
        sum += sensor[Ind]
    if sum != 0:
        sensor_active = True
    # Activ_sensor_ECU(stream_options_chkbtn_obj,310,60)
    else:
        sensor_active = False
        Clear_all_sensor(stream_options_chkbtn_obj)
    Activ_sensor_ECU(stream_options_chkbtn_obj, 310, 60)
    Update_Data_Extractor_Mode()
    print('senor selected', sensor_active)
    return


def Clear_all_sensor(self):
    self.STREAM_HEADER_chkbtn.place_forget()
    self.CDC_chkbtn.place_forget()
    self.DSPACE_chkbtn.place_forget()
    self.CCA_HDR_chkbtn.place_forget()
    self.OSI_STREAM_chkbtn.place_forget()
    self.Z4_CORE_chkbtn.place_forget()
    self.Z7A_CORE_chkbtn.place_forget()
    self.Z7B_CORE_chkbtn.place_forget()
    self.Z4_CUST_chkbtn.place_forget()
    self.Z7A_CUST_chkbtn.place_forget()
    return


def Clear_all_ECU(self):
    self.ECU_1_chkbtn.place_forget()
    self.ECU_2_chkbtn.place_forget()
    self.ECU_3_chkbtn.place_forget()
    self.ECU_VRU_CLASSIFIER_chkbtn.place_forget()
    return


def Clear_stream_label():
    global STREAM_OPTION
    global STREAM_OPTION_SET
    STREAM_OPTION.place_forget()
    STREAM_OPTION_SET = 1
    return


def set_Radar_ecu(self, Val):
    global Radar_ecu_active
    if Val is not None:
        print('Val.get()', Val.get())
        if Val.get():
            Radar_ecu_active = True
            # Activ_sensor_ECU(stream_options_chkbtn_obj,310,60)
            print('set')
        else:
            Radar_ecu_active = False
            # Activ_sensor_ECU(stream_options_chkbtn_obj,310,60)
            Clear_all_ECU(stream_options_chkbtn_obj)
            print('in clear')
    else:
        Radar_ecu_active = False
        Clear_all_ECU(stream_options_chkbtn_obj)
        print('clear')
    Activ_sensor_ECU(stream_options_chkbtn_obj, 310, 60)
    return


def Update_STREAM_OPTION(self, stream_chk, STREAM_OPTION_X_val, STREAM_OPTION_Y_val):
    # Canvas_MUDP.create_rectangle(300, 470, 480, 49, fill=None)
    self.STREAM_HEADER = IntVar()
    stream_chk.STREAM_HEADER_chkbtn = Checkbutton(root, text="STREAM HEADER", variable=self.STREAM_HEADER,
                                                  font=('arial', 10, 'bold'),
                                                  bg=Bg_colour, onvalue=1, offvalue=0,
                                                  command=lambda: Set_sensor_ecu_stream(self, Stream_pos.STREAM_HEADER,
                                                                                        self.STREAM_HEADER))
    self.CDC = IntVar()
    stream_chk.CDC_chkbtn = Checkbutton(root, text="CDC", variable=self.CDC, font=('arial', 10, 'bold'), bg=Bg_colour,
                                        onvalue=1,
                                        offvalue=0,
                                        command=lambda: Set_sensor_ecu_stream(self, Stream_pos.CDC, self.CDC))
    self.DSPACE = IntVar()
    stream_chk.DSPACE_chkbtn = Checkbutton(root, text="DSPACE", variable=self.DSPACE, font=('arial', 10, 'bold'),
                                           bg=Bg_colour,
                                           onvalue=1, offvalue=0,
                                           command=lambda: Set_sensor_ecu_stream(self, Stream_pos.DSPACE, self.DSPACE))
    self.CCA_HDR = IntVar()
    stream_chk.CCA_HDR_chkbtn = Checkbutton(root, text="CCA_HDR", variable=self.CCA_HDR, font=('arial', 10, 'bold'),
                                            bg=Bg_colour,
                                            onvalue=1, offvalue=0,
                                            command=lambda: Set_sensor_ecu_stream(self, Stream_pos.CCA_HDR,
                                                                                  self.CCA_HDR))
    self.OSI_STREAM = IntVar()
    stream_chk.OSI_STREAM_chkbtn = Checkbutton(root, text="OSI STREAM", variable=self.OSI_STREAM,
                                               font=('arial', 10, 'bold'), bg=Bg_colour,
                                               onvalue=1, offvalue=0,
                                               command=lambda: Set_sensor_ecu_stream(self, Stream_pos.OSI_STREAM,
                                                                                     self.OSI_STREAM))
    self.Z4_CORE = IntVar()
    stream_chk.Z4_CORE_chkbtn = Checkbutton(root, text="Z4_CORE", variable=self.Z4_CORE, font=('arial', 10, 'bold'),
                                            bg=Bg_colour,
                                            onvalue=1, offvalue=0,
                                            command=lambda: Set_sensor_ecu_stream(self, Stream_pos.Z4_CORE,
                                                                                  self.Z4_CORE))
    self.Z7A_CORE = IntVar()
    stream_chk.Z7A_CORE_chkbtn = Checkbutton(root, text="Z7A_CORE", variable=self.Z7A_CORE, font=('arial', 10, 'bold'),
                                             bg=Bg_colour,
                                             onvalue=1, offvalue=0,
                                             command=lambda: Set_sensor_ecu_stream(self, Stream_pos.Z7A_CORE,
                                                                                   self.Z7A_CORE))
    self.Z7B_CORE = IntVar()
    stream_chk.Z7B_CORE_chkbtn = Checkbutton(root, text="Z7B_CORE", variable=self.Z7B_CORE, font=('arial', 10, 'bold'),
                                             bg=Bg_colour,
                                             onvalue=1, offvalue=0,
                                             command=lambda: Set_sensor_ecu_stream(self, Stream_pos.Z7B_CORE,
                                                                                   self.Z7B_CORE))
    self.Z4_CUST = IntVar()
    stream_chk.Z4_CUST_chkbtn = Checkbutton(root, text="Z4_CUST", variable=self.Z4_CUST, font=('arial', 10, 'bold'),
                                            bg=Bg_colour,
                                            onvalue=1, offvalue=0,
                                            command=lambda: Set_sensor_ecu_stream(self, Stream_pos.Z4_CUST,
                                                                                  self.Z4_CUST))
    self.Z7A_CUST = IntVar()
    stream_chk.Z7A_CUST_chkbtn = Checkbutton(root, text="Z7A_CUST", variable=self.Z7A_CUST, font=('arial', 10, 'bold'),
                                             bg=Bg_colour,
                                             onvalue=1, offvalue=0,
                                             command=lambda: Set_sensor_ecu_stream(self, Stream_pos.Z7A_CUST,
                                                                                   self.Z7A_CUST))

    self.ECU_1 = IntVar()
    stream_chk.ECU_1_chkbtn = Checkbutton(root, text="ECU_1", variable=self.ECU_1,
                                          font=('arial', 10, 'bold'), bg=Bg_colour, onvalue=1,
                                          offvalue=0, command=lambda: Set_sensor_ecu_stream(self, 10, self.ECU_1))
    self.ECU_2 = IntVar()
    stream_chk.ECU_2_chkbtn = Checkbutton(root, text="ECU_2", variable=self.ECU_2,
                                          font=('arial', 10, 'bold'), bg=Bg_colour, onvalue=1,
                                          offvalue=0, command=lambda: Set_sensor_ecu_stream(self, 11, self.ECU_2))
    self.ECU_3 = IntVar()
    stream_chk.ECU_3_chkbtn = Checkbutton(root, text="ECU_3", variable=self.ECU_3,
                                          font=('arial', 10, 'bold'), bg=Bg_colour, onvalue=1,
                                          offvalue=0, command=lambda: Set_sensor_ecu_stream(self, 12, self.ECU_3))
    self.ECU_VRU_CLASSIFIER = IntVar()
    stream_chk.ECU_VRU_CLASSIFIER_chkbtn = Checkbutton(root, text="ECU VRU CLASSIFIER",
                                                       variable=self.ECU_VRU_CLASSIFIER, font=('arial', 10, 'bold'),
                                                       bg=Bg_colour, onvalue=1, offvalue=0,
                                                       command=lambda: Set_sensor_ecu_stream(self, 13,
                                                                                             self.ECU_VRU_CLASSIFIER))
    return


def Set_sensor_ecu_stream(self, Index, Val):
    global sen_ecu_stream
    global SET_LIB
    global sum_stream
    sum_stream = 0
    if Val is not None:
        sen_ecu_stream[Index] = Val.get()
    for ind in range(len(sen_ecu_stream)):
        sum_stream += sen_ecu_stream[ind]
    if sum_stream != 0 and SET_LIB == 0:
        SET_LIB = sum_stream
        Update_lib_tool_config(540, 60, SET_LIB)
    elif sum_stream == 0:
        Clear_lib_tool_config()
        SET_LIB = 0
    return


def Clear_lib_tool_config():
    global SET_LIB
    SET_LIB = 0
    Update_lib_tool_config(540, 60, SET_LIB)
    return


# def set_Radar_ecu_stream(self,Index,Val):
#	global Ecu_stream
#	global SET_LIB
#	global sum_stream
#	Ecu_stream[Index] = Val.get()
#	for ind in range(len(Ecu_stream)):
#		sum_stream += Ecu_stream[ind]
#	if sum_stream != 0 and SET_LIB==0:
#		SET_LIB=sum_stream
#		Update_lib_tool_config(540,60,SET_LIB)
#	else:
#		Clear_lib_tool_config()
#	return


def Update_Data_Extractor_Mode():
    Canvas_MUDP.create_rectangle(50, 310, 280, 460, fill=None)
    global data_extraction_mode_label
    global csv
    global Xml_Trace_Mode
    global detail_error_info
    global packet_loss_statistics
    global log_quality_summary
    global csv_chkbtn
    global Xml_Trace_Mode_chkbtn
    global detail_error_info_chkbtn
    global packet_loss_statistics_chkbtn
    global log_quality_summary_chkbtn
    data_extraction_mode_label = Label(root, text="DATA EXTRACTION", font=('arial', 12, 'bold'), bg=Bg_colour)
    data_extraction_mode_label.place(x=60, y=300)

    csv = IntVar()
    csv.set(0)
    csv_chkbtn = Checkbutton(root, text="CSV", variable=csv, font=('arial', 10, 'bold'),
                             bg=Bg_colour, onvalue=1, offvalue=0)
    csv_chkbtn.place(x=70, y=340)

    Xml_Trace_Mode = IntVar()
    Xml_Trace_Mode.set(0)
    Xml_Trace_Mode_chkbtn = Checkbutton(root, text="XML TRACE MODE", variable=Xml_Trace_Mode,
                                        font=('arial', 10, 'bold'), bg=Bg_colour, onvalue=1, offvalue=0)
    Xml_Trace_Mode_chkbtn.place(x=70, y=360)

    detail_error_info = IntVar()
    detail_error_info.set(0)
    detail_error_info_chkbtn = Checkbutton(root, text="Detail Error Info", variable=detail_error_info,
                                           font=('arial', 10, 'bold'), bg=Bg_colour, onvalue=1, offvalue=0)
    detail_error_info_chkbtn.place(x=70, y=380)

    packet_loss_statistics = IntVar()
    packet_loss_statistics.set(0)
    packet_loss_statistics_chkbtn = Checkbutton(root, text="PACKET LOSS STATISTICS", variable=packet_loss_statistics,
                                                font=('arial', 10, 'bold'), bg=Bg_colour, onvalue=1, offvalue=0,
                                                command=lambda: Update_format(packet_loss_statistics))
    packet_loss_statistics_chkbtn.place(x=70, y=400)

    log_quality_summary = IntVar()
    log_quality_summary.set(0)
    log_quality_summary_chkbtn = Checkbutton(root, text="Log Quality Summary", variable=log_quality_summary,
                                             font=('arial', 10, 'bold'), bg=Bg_colour, onvalue=1, offvalue=0)
    log_quality_summary_chkbtn.place(x=70, y=420)
    return


def Activ_sensor_ECU(self, STREAM_OPTION_X_val, STREAM_OPTION_Y_val):
    Canvas_MUDP.create_rectangle(300, 460, 480, 70, fill=None)
    global sensor_active
    global Radar_ecu_active
    global STREAM_OPTION_SET
    global STREAM_OPTION
    if STREAM_OPTION_SET:
        STREAM_OPTION = Label(root, text="STREAM OPTION", font=('arial', 12, 'bold'), bg=Bg_colour)
        STREAM_OPTION.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val)
        STREAM_OPTION_SET = 0

    if sensor_active:
        self.STREAM_HEADER_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 1.5))
        self.CDC_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 2.5))
        self.DSPACE_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 3.5))
        self.CCA_HDR_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 4.5))
        self.OSI_STREAM_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 5.5))
        self.Z4_CORE_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 6.5))
        self.Z7A_CORE_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 7.5))
        self.Z7B_CORE_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 8.5))
        self.Z4_CUST_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 9.5))
        self.Z7A_CUST_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 10.5))
        if Radar_ecu_active:
            self.ECU_1_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 11.5))
            self.ECU_2_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 12.5))
            self.ECU_3_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 13.5))
            self.ECU_VRU_CLASSIFIER_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 14.5))
    elif Radar_ecu_active:
        Clear_all_sensor(stream_options_chkbtn_obj)
        self.ECU_1_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 1))
        self.ECU_2_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 2))
        self.ECU_3_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 3))
        self.ECU_VRU_CLASSIFIER_chkbtn.place(x=STREAM_OPTION_X_val, y=STREAM_OPTION_Y_val + (offset_y * 4))
    else:
        Clear_stream_label()
    return


def Update_lib_tool_config(TOOL_CONFIG_X_val, TOOL_CONFIG_Y_val, SET_LIB):
    Canvas_MUDP.create_rectangle(510, 390, 1150, 250, fill=None)
    global TOOL_CONFIG
    global Tool_library
    global output_xml_trace
    global output_xml_trace_val
    global Tool_library_label
    global output_xml_trace_label
    global Tool_OUTPUT_label
    global Tool_OUTPUT
    global file_mode_label
    global file_mode_con_rb
    global file_mode_seq_rb
    global file_mode
    if SET_LIB == 1:
        TOOL_CONFIG = Label(root, text="TOOL CONFIGURATION", font=('arial', 12, 'bold'), bg=Bg_colour)
        TOOL_CONFIG.place(x=TOOL_CONFIG_X_val, y=TOOL_CONFIG_Y_val+180)
        Tool_library_val = ('Vector Library', 'CCA Library')
        Tool_library = Spinbox(root, font=('arial', 10, 'bold'), bg='white', values=Tool_library_val,
                               command=lambda: Update_library(Tool_library))
        Tool_library.place(x=TOOL_CONFIG_X_val + 250, y=TOOL_CONFIG_Y_val + (offset_y * 8.5))
        output_xml_trace_val = ('HIL_port_xml', 'Sensor_xml')
        output_xml_trace = Spinbox(root, font=('arial', 10, 'bold'), bg='white', values=output_xml_trace_val,
                                   command=lambda: Update_trace(output_xml_trace))
        output_xml_trace.place(x=TOOL_CONFIG_X_val + 250, y=TOOL_CONFIG_Y_val + (offset_y * 9.75))
        Tool_OUTPUT_PATH = ('SAME_AS_INPUT', 'NEW_OUTPUT_PATH')
        Tool_OUTPUT = Spinbox(root, font=('arial', 10, 'bold'), bg='white', values=Tool_OUTPUT_PATH,
                              command=lambda: Update_output_path(Tool_OUTPUT, 540, 60))
        Tool_OUTPUT.place(x=TOOL_CONFIG_X_val + 250, y=TOOL_CONFIG_Y_val + (offset_y * 11))
        Tool_library_label = Label(root, text="LIBRARY:", font=('arial', 10, 'bold'), bg=Bg_colour)
        Tool_library_label.place(x=TOOL_CONFIG_X_val, y=TOOL_CONFIG_Y_val + (offset_y * 8.5))
        output_xml_trace_label = Label(root, text="OUTPUT_XML_TRACE:", font=('arial', 10, 'bold'), bg=Bg_colour)
        output_xml_trace_label.place(x=TOOL_CONFIG_X_val, y=TOOL_CONFIG_Y_val + (offset_y * 9.75))
        Tool_OUTPUT_label = Label(root, text="SELECT OUTPUT PATH:", font=('arial', 10, 'bold'), bg=Bg_colour)
        Tool_OUTPUT_label.place(x=TOOL_CONFIG_X_val, y=TOOL_CONFIG_Y_val + (offset_y * 11))
        '''input_file_mode_label = Label(root, text="FILE MODE:", font=('arial', 10, 'bold'), bg=Bg_colour)
        input_file_mode_label.place(x=TOOL_CONFIG_X_val, y=TOOL_CONFIG_Y_val + (offset_y * 12))'''
        file_mode = StringVar()
        file_mode.set('.')
        file_mode_label = Label(root, text="FILE MODE:", font=('arial', 10, 'bold'), bg=Bg_colour)
        file_mode_label.place(x=TOOL_CONFIG_X_val, y=TOOL_CONFIG_Y_val + (offset_y * 12))
        file_mode_con_rb = Radiobutton(text='Continuous_FILE_Input', variable=file_mode, value='Continuous_FILE_Input',
                                       font=('arial', 10, 'bold'), bg=Bg_colour)
        file_mode_con_rb.place(x=TOOL_CONFIG_X_val + 100, y=TOOL_CONFIG_Y_val + (offset_y * 12))
        file_mode_seq_rb = Radiobutton(text='Sequential_FILE_Input', variable=file_mode, value='Sequential_FILE_Input',
                                       font=('arial', 10, 'bold'), bg=Bg_colour)
        file_mode_seq_rb.place(x=TOOL_CONFIG_X_val + 320, y=TOOL_CONFIG_Y_val + (offset_y * 12))
    elif SET_LIB == 0:
        TOOL_CONFIG.place_forget()
        Tool_library.place_forget()
        output_xml_trace.place_forget()
        # PACKET_LOSS_Report_Format.place_forget()
        # packet_loss_report_label.place_forget()
        Tool_library_label.place_forget()
        output_xml_trace_label.place_forget()
        Tool_OUTPUT_label.place_forget()
        Tool_OUTPUT.place_forget()
        file_mode.set('.')
        file_mode_label.place_forget()
        file_mode_con_rb.place_forget()
        file_mode_seq_rb.place_forget()

    # Entry_widgets(540, 60, SET_LIB)
    Button_widgets(540, 60, SET_LIB)
    return


def Update_library(Tool_library_val):
    Global_var_class_obj.Library = Tool_library_val.get()
    if Global_var_class_obj.Library == 'Vector Library':
        Global_var_class_obj.Vector_Library = 1
        Global_var_class_obj.CCA_Library = 0
    elif Global_var_class_obj.Library == 'CCA Library':
        Global_var_class_obj.CCA_Library = 1
        Global_var_class_obj.Vector_Library = 0
    else:
        Global_var_class_obj.Vector_Library = 0
        Global_var_class_obj.CCA_Library = 0
    return


def Update_trace(Trace):
    Global_var_class_obj.trace = Trace.get()
    if Global_var_class_obj.trace == 'HIL_port_xml':
        Global_var_class_obj.HIL_port_xml = 1
        Global_var_class_obj.Sensor_xml = 0
    elif Global_var_class_obj.trace == 'Sensor_xml':
        Global_var_class_obj.Sensor_xml = 1
        Global_var_class_obj.HIL_port_xml = 0
    else:
        Global_var_class_obj.HIL_port_xml = 0
        Global_var_class_obj.Sensor_xml = 0
    return


def Update_format(packet_loss_statistics):
    Canvas_MUDP.create_rectangle(50, 499, 480, 465, fill=None)
    global packet_loss_report_label
    global report_text_format_RB
    global report_xml_format_RB
    global report_format
    report_format = StringVar()
    report_format = '.'
    if packet_loss_statistics.get() == 1:
        packet_loss_report_label = Label(root, text="Report Format:", font=('arial', 10, 'bold'), bg=Bg_colour)
        packet_loss_report_label.place(x=60, y=470)
        report_text_format_RB = Radiobutton(text='Text Format', variable=report_format, value='text_format',
                                            font=('arial', 10, 'bold'), bg=Bg_colour)
        report_text_format_RB.place(x=200, y=470)
        report_xml_format_RB = Radiobutton(text='Xml Format', variable=report_format, value='xml_format',
                                           font=('arial', 10, 'bold'), bg=Bg_colour)
        report_xml_format_RB.place(x=320, y=470)


def clear_report_format_variables():
    try:
        packet_loss_report_label.place_forget()
        report_text_format_RB.place_forget()
        report_xml_format_RB.place_forget()
    except:
        pass
    return


def Update_output_path(path, Button_X_val, Button_Y_val):
    # global browse_output_path_label
    global Browse_Output_Entry
    global browse_output_path_button
    # filelist.clear()
    Global_var_class_obj.output_path = path.get()
    '''if Global_var_class_obj.output_path == 'SAME_AS_INPUT':
        global opath
        opath = None
        filelist.append(opath)'''

    if Global_var_class_obj.output_path == 'NEW_OUTPUT_PATH':
        Browse_Output_Entry = Entry(width=24)
        Browse_Output_Entry.place(x=Button_X_val + 250, y=Button_Y_val + (offset_y * 11))
        # filelist.clear()

        def new():
            global fpath
            # filelist.clear()
            fpath = filedialog.askdirectory()
            Browse_Output_Entry.insert(END, fpath)
            # print("fpath:", fpath)
            filelist.append(fpath)
            # print(filelist)

        browse_output_path_button = Button(text='BROWSE', bg=Bg_colour, command=new)
        browse_output_path_button.place(x=Button_X_val + 450, y=Button_Y_val + (offset_y * 10.8))


'''def Entry_widgets(ENTRIES_X_val, ENTRIES_Y_val, SET_LIB):
    # Entry Configuration
    global entries
    if SET_LIB == 1:
        entries.append(Entry(root, font=('arial', 10, 'bold'), width=21, bg='white'))
        entries.append(Entry(root, font=('arial', 10, 'bold'), width=21, bg='white'))
        # INPUT LOG
        entries[0].place(x=ENTRIES_X_val + 130, y=ENTRIES_Y_val + (offset_y * 6))

    elif SET_LIB == 0:
        clear_all_entries()
    return
'''


def Button_widgets(Button_X_val, Button_Y_val, SET_LIB):
    # Button configuration
    global Browse_OUTPUT_PATH_label
    # global Browse_Log_list
    # global Browse_OUTPUT_PATH
    global JSON_log_button
    # global xml_button
    # global JSON_log_label
    global HOME
    global START
    if SET_LIB == 1:
        Browse_OUTPUT_PATH_label = Label(root, bg=Bg_colour)
        Browse_OUTPUT_PATH_label.place(x=Button_X_val, y=Button_Y_val + (offset_y * 10))
        # HOME_label = Label(root, text="OUTPUT PATH:", font=('arial', 10, 'bold'), bg=Bg_colour)
        # HOME_label.place(x=Button_X_val, y=Button_Y_val+ (offset_y * 6))
        # START_label = Label(root, text="OUTPUT PATH:", font=('arial', 10, 'bold'), bg=Bg_colour)
        # START_label.place(x=Button_X_val, y=Button_Y_val+ (offset_y * 6))
        Browse_BN_Log_list_call()
        Browse_Faseth_Log_list_call()
        Browse_SRR_Log_list_call()
        Browse_REF_Log_list_call()
        HOME = Button(root, text=" HOME ", bg=Bg_colour,
                      command=lambda: HOME_Call())
        START = Button(root, text=" START ", bg=Bg_colour,
                       command=lambda: START_Call())
        JSON_log_button = Button(root, text=" JSON/FLIST ", bg=Bg_colour,
                                 command=lambda: JSON_OR_FLIST_Call())
        # xml_button = Button(root, text=" XML ", bg=Bg_colour,
        #                   command=lambda: xml_Call())

        # button placing

        HOME.place(x=Button_X_val + 150, y=Button_Y_val + (offset_y * 13.5))
        JSON_log_button.place(x=Button_X_val + 250, y=Button_Y_val + (offset_y * 5.75))
        # xml_button.place(x=Button_X_val + 290, y=Button_Y_val + (offset_y * 18))
        START.place(x=Button_X_val + 250, y=Button_Y_val + (offset_y * 13.5))
    elif SET_LIB == 0:
        Browse_OUTPUT_PATH_label.place_forget()
        HOME.place_forget()
        JSON_log_button.place_forget()
        # xml_button.place_forget()
        START.place_forget()
    return


clear_mudp_log_list()


def Browse_BN_Log_list_call():
    Canvas_MUDP.create_rectangle(510, 235, 1150, 70, fill=None)
    Label(root, text="CREATE JSON/FLIST", font=('arial', 12, 'bold'), bg=Bg_colour).place(x=530, y=60)
    global Browse_BN_Log_list_label
    global Browse_BN_Button
    global BN_CALIFR_Log_opt
    Browse_BN_Log_list_label = Label(root, text="BN CALIFR LOG PATH:", font=('arial', 10, 'bold'), bg=Bg_colour)
    Browse_BN_Log_list_label.place(x=540, y=90)
    BN_CALIFR_Log_opt = Entry(width=24)
    BN_CALIFR_Log_opt.place(x=790, y=90)

    def browse_bn_califr_log_list_call():
        bn_califr_logs = filedialog.askopenfilenames(title='Select BN Califr Logs',
                                                     filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for a in bn_califr_logs:
            bn_califr_MUDP_log_list.append(a)
        BN_CALIFR_Log_opt.insert(END, bn_califr_logs)

    Browse_BN_Button = Button(root, text="BROWSE", bg=Bg_colour, command=lambda: browse_bn_califr_log_list_call())
    Browse_BN_Button.place(x=990, y=85)


def Browse_Faseth_Log_list_call():
    global Browse_Faseth_Log_list_label
    global Browse_Faseth_button
    global BN_faseth_Log_opt
    Browse_Faseth_Log_list_label = Label(root, text="FASETH LOG PATH:", font=('arial', 10, 'bold'), bg=Bg_colour)
    Browse_Faseth_Log_list_label.place(x=540, y=120)
    BN_faseth_Log_opt = Entry(width=24)
    BN_faseth_Log_opt.place(x=790, y=120)

    def browse_bn_califr_log_list_call():
        faseth_logs = filedialog.askopenfilenames(title='Select BN Califr Logs',
                                                  filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for a in faseth_logs:
            faseth_MUDP_log_list.append(a)
        BN_faseth_Log_opt.insert(END, faseth_logs)

    Browse_Faseth_button = Button(root, text="BROWSE", bg=Bg_colour, command=lambda: browse_bn_califr_log_list_call())
    Browse_Faseth_button.place(x=990, y=115)


def Browse_SRR_Log_list_call():
    global Browse_SRR_Log_list_label
    global Browse_SRR_button
    global SRR_LOG_opt
    Browse_SRR_Log_list_label = Label(root, text="SRR DEBUG LOG PATH:", font=('arial', 10, 'bold'), bg=Bg_colour)
    Browse_SRR_Log_list_label.place(x=540, y=150)
    SRR_LOG_opt = Entry(width=24)
    SRR_LOG_opt.place(x=790, y=150)

    def browse_srr_log_list_call():
        srr_logs = filedialog.askopenfilenames(title='Select SRR Logs',
                                               filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for a in srr_logs:
            srr_debug_MUDP_log_list.append(a)
        SRR_LOG_opt.insert(END, srr_logs)

    Browse_SRR_button = Button(root, text="BROWSE", bg=Bg_colour, command=lambda: browse_srr_log_list_call())
    Browse_SRR_button.place(x=990, y=145)


def Browse_REF_Log_list_call():
    global Browse_REF_Log_list_label
    global Browse_REF_button
    global SRR_REF_LOG_opt
    Browse_REF_Log_list_label = Label(root, text="SRR REFERENCE LOG PATH:", font=('arial', 10, 'bold'), bg=Bg_colour)
    Browse_REF_Log_list_label.place(x=540, y=180)
    SRR_REF_LOG_opt = Entry(width=24)
    SRR_REF_LOG_opt.place(x=790, y=180)

    def browse_srr_ref_log_list_call():
        SRR_REF_logs = filedialog.askopenfilenames(title='Select BN Califr Logs',
                                                   filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for a in SRR_REF_logs:
            srr_reference_MUDP_log_list.append(a)
        SRR_REF_LOG_opt.insert(END, SRR_REF_logs)

    Browse_REF_button = Button(root, text="BROWSE", bg=Bg_colour, command=lambda: browse_srr_ref_log_list_call())
    Browse_REF_button.place(x=990, y=175)


def JSON_OR_FLIST_Call():
    if len(srr_debug_MUDP_log_list) == 0:
        messagebox.showerror(title='JSON/FLIST ERROR', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')
    elif len(bn_califr_MUDP_log_list) == len(faseth_MUDP_log_list) == len(srr_debug_MUDP_log_list) == len(
            srr_reference_MUDP_log_list):
        # oldfiledel()
        if os.path.exists("MUDP_json.json"):
            os.remove("MUDP_json.json")
        MUDP_json_writer(bn_califr_MUDP_log_list, faseth_MUDP_log_list, srr_debug_MUDP_log_list,
                         srr_reference_MUDP_log_list)
        messagebox.showinfo(title='JSON/FLIST', message='! SUCCESSFULLY GENERATED THE JSON FILE !')
        # json_path()
    elif len(bn_califr_MUDP_log_list and faseth_MUDP_log_list and srr_reference_MUDP_log_list) == 0 and len(
            srr_debug_MUDP_log_list) >= 1:
        if os.path.exists("MUDP_flist.txt"):
            os.remove("MUDP_flist.txt")
        MUDP_flist_gen(srr_debug_MUDP_log_list)
        messagebox.showinfo(title='JSON/FLIST', message='! SUCCESSFULLY GENERATED THE FLIST FILE !')
        # flist_path()
    else:
        messagebox.showerror(title='Json/Flist Error', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')


'''def Browse_OUTPUT_PATH_call():
    global entries
    Global_var_class_obj.output_new_path = entries[0].get()
    if (Global_var_class_obj.output_new_path == None) or (Global_var_class_obj.output_new_path == ''):
        # show an "Open" dialog box and return the path to the selected file
        Global_var_class_obj.output_new_path = askdirectory()
        entries[0].insert(0, Global_var_class_obj.output_new_path)
        print("output_new_path:", Global_var_class_obj.output_new_path)
    else:
        pass
    return'''


def Clear_all_Data_extraction_and_log_paths():
    data_extraction_mode_label.place_forget()
    csv_chkbtn.place_forget()
    Xml_Trace_Mode_chkbtn.place_forget()
    detail_error_info_chkbtn.place_forget()
    packet_loss_statistics_chkbtn.place_forget()
    log_quality_summary_chkbtn.place_forget()
    Browse_BN_Log_list_label.place_forget()
    Browse_Faseth_Log_list_label.place_forget()
    Browse_SRR_Log_list_label.place_forget()
    Browse_REF_Log_list_label.place_forget()
    BN_CALIFR_Log_opt.place_forget()
    BN_faseth_Log_opt.place_forget()
    SRR_LOG_opt.place_forget()
    SRR_REF_LOG_opt.place_forget()
    Browse_REF_button.place_forget()
    Browse_SRR_button.place_forget()
    Browse_Faseth_button.place_forget()
    Browse_BN_Button.place_forget()
    return


def HOME_Call():
    global sen_ecu_stream
    global sensor
    Canvas_MUDP.pack_forget()
    Clear_lib_tool_config()
    Clear_stream_label()
    Clear_all_ECU(stream_options_chkbtn_obj)
    Clear_all_sensor(stream_options_chkbtn_obj)
    Clear_Global_var_class(Global_var_class_obj)
    Clear_Sensor_and_ECU(Sensor_and_ECU_obj)
    Clear_stream_options(stream_options_obj)
    Clear_all_Data_extraction_and_log_paths()
    clear_report_format_variables()
    clear_mudp_log_list()
    for ind in range(len(sen_ecu_stream)):
        sen_ecu_stream[ind] = 0
    for ind in range(len(sensor)):
        sensor[ind] = 0
    clear_all_variables()
    Canvas_MUDP.pack()
    return


global Mode_list
Mode_list = []


def START_Call():
    global Global_var_class_obj
    global Tool_library
    global output_xml_trace
    global Tool_OUTPUT
    global report_format1
    global report_format
    global file_mode1
    Update_library(Tool_library)
    Update_trace(output_xml_trace)
    # Update_format(packet_loss_statistics)
    Update_output_path(Tool_OUTPUT, 540, 60)
    # Update_input_file_mode(Global_var_class_obj.file_mode)
    print("Library:", Global_var_class_obj.Library)
    print("Trace:", Global_var_class_obj.trace)
    # print("log_path:", Global_var_class_obj.log_path)
    print("output_path:", Global_var_class_obj.output_path)
    # print("output_path:", filelist[0])
    # print("Packet_Loss_Report_Format:", Global_var_class_obj.report_format)
    # print("Input File Mode:", Global_var_class_obj.file_mode)
    # Stream_pos
    # Sen_pos
    # Sensor_and_ECU_obj
    # stream_options_obj
    # filelist.clear()
    Mode_list.clear()
    input1 = csv.get()
    Mode_list.append(input1)
    input2 = Xml_Trace_Mode.get()
    Mode_list.append(input2)
    input3 = detail_error_info.get()
    Mode_list.append(input3)
    input4 = packet_loss_statistics.get()
    Mode_list.append(input4)
    input5 = log_quality_summary.get()
    Mode_list.append(input5)
    try:
        report_format1 = report_format.get()
    except:
        pass
    file_mode1 = file_mode.get()
    if os.path.exists("MUDP_DATA_Extracter_config_v2p0.xml"):
        os.remove("MUDP_DATA_Extracter_config_v2p0.xml")
    Write_XML(Sensor_and_ECU_obj, stream_options_obj, Global_var_class_obj, Mode_list, report_format1, file_mode1,
              filelist)
    if len(bn_califr_MUDP_log_list) == len(faseth_MUDP_log_list) == len(srr_debug_MUDP_log_list) == len(srr_reference_MUDP_log_list) == 0:
        messagebox.showerror(title='Execution Error', message='!!! PLEASE BROWSE THE LOG FILES, CREATE JSON/ FLIST !!!')
    else:
        Execute_Command(0, bn_califr_MUDP_log_list, faseth_MUDP_log_list, srr_debug_MUDP_log_list,
                        srr_reference_MUDP_log_list)
    return


def Update_progress(info=None):
    Update_progress_info = Label(root, text='Tips : ' + info, font=('arial', 8), bg=Bg_colour, width=70, anchor=NW)
    Update_progress_info.place(x=70, y=(400 + offset_y), anchor=NW)
    root.update()
    root.after(1000)
    return


def Clear_Global_var_class(self):
    self.Library = None
    self.trace = None
    # self.log_path = None
    self.output_path = None
    # self.output_new_path = 'C:/Workspaces/SRR5_logs/RNA_MF41'
    self.CCA_Library = None
    self.Vector_Library = None
    self.HIL_port_xml = None
    self.Sensor_xml = None
    return


def Clear_Sensor_and_ECU(self):
    self.REAR_LEFT.set(0)
    self.REAR_RIGHT.set(0)
    self.FRONT_LEFT.set(0)
    self.FRONT_RIGHT.set(0)
    self.BP_LEFT.set(0)
    self.BP_RIGHT.set(0)
    self.RADAR_ECU.set(0)
    return


def Clear_stream_options(self):
    self.STREAM_HEADER.set(0)
    self.CDC.set(0)
    self.DSPACE.set(0)
    self.CCA_HDR.set(0)
    self.OSI_STREAM.set(0)
    self.Z4_CORE.set(0)
    self.Z7A_CORE.set(0)
    self.Z7B_CORE.set(0)
    self.Z4_CUST.set(0)
    self.Z7A_CUST.set(0)
    self.ECU_1.set(0)
    self.ECU_2.set(0)
    self.ECU_3.set(0)
    self.ECU_VRU_CLASSIFIER.set(0)
    return


def clear_all_variables():
    global sensor_active
    global Radar_ecu_active
    global STREAM_OPTION_SET
    global TOOL_CONFIG
    global Tool_library
    global output_xml_trace
    global Tool_library_label
    # global output_xml_trace_label
    global data_extraction_mode_label
    global Tool_OUTPUT_label
    global Tool_OUTPUT
    # global Browse_Log_list_label
    # global Browse_OUTPUT_PATH_label
    # global Browse_Log_list
    # global Browse_OUTPUT_PATH
    global HOME
    global START
    # global Set_Browse_OUTPUT_PATH
    global sum_stream
    global SET_LIB
    # global entries
    global csv
    global Xml_Trace_Mode
    global detail_error_info
    global packet_loss_statistics
    global log_quality_summary
    global report_format
    global file_mode
    sensor_active = False
    Radar_ecu_active = False
    STREAM_OPTION_SET = 1
    TOOL_CONFIG = None
    Tool_library = None
    output_xml_trace = None
    # data_extraction_mode_label = None
    Tool_library_label = None
    # output_xml_trace_label = None
    # Tool_OUTPUT_label = None
    # Tool_OUTPUT = None
    # Browse_Log_list_label = None
    # Browse_OUTPUT_PATH_label = None
    # Browse_Log_list = None
    # Browse_OUTPUT_PATH = None
    HOME = None
    START = None
    # Set_Browse_OUTPUT_PATH = 0
    sum_stream = 0
    SET_LIB = 0
    # entries = []
    csv.set(0)
    Xml_Trace_Mode.set(0)
    detail_error_info.set(0)
    packet_loss_statistics.set(0)
    log_quality_summary.set(0)
    if packet_loss_statistics == 1:
        if report_format != '.':
            report_format = '.'
    file_mode.set('.')
    return
# class ScrollableFrame(Frame):
#	def __init__(self, parent, minimal_canvas_size, *args, **kw):
#		'''
#		Constructor
#		'''
#
#		Frame.__init__(self, parent, *args, **kw)
#
#		self.minimal_canvas_size = minimal_canvas_size
#
#		# create a vertical scrollbar
#		vscrollbar = Scrollbar(self, orient = VERTICAL)
#		vscrollbar.pack(fill = Y, side = RIGHT, expand = FALSE)
#
#		# create a horizontal scrollbar
#		hscrollbar = Scrollbar(self, orient = HORIZONTAL)
#		hscrollbar.pack(fill = X, side = BOTTOM, expand = FALSE)
#
#		#Create a canvas object and associate the scrollbars with it
#		self.canvas = Canvas(self, bd = 0, highlightthickness = 0, yscrollcommand = vscrollbar.set, xscrollcommand = hscrollbar.set)
#		self.canvas.pack(side = LEFT, fill = BOTH, expand = TRUE)
#
#		#Associate scrollbars with canvas view
#		vscrollbar.config(command = self.canvas.yview)
#		hscrollbar.config(command = self.canvas.xview)
#
#
#		# set the view to 0,0 at initialization
#
#		self.canvas.xview_moveto(0)
#		self.canvas.yview_moveto(0)
#
#		self.canvas.config(scrollregion='0 0 %s %s' % self.minimal_canvas_size)
#
#		# create an interior frame to be created inside the canvas
#
#		self.interior = interior = Frame(self.canvas)
#		interior_id = self.canvas.create_window(0, 0, window=interior,
#				anchor=NW)
