"""Import library"""
from tkinter import *
from tkinter.ttk import *
import tkinter as tk
from tkinter.filedialog import *
from tkinter import messagebox
# from multiprocessing import Process, Manager
# import multiprocessing
from main_interface import *
import MUDP_Tool_spec_widgets as M_Tool
from SIL_Backup_data import *
from SIL_JSON import *
from SIL_FLIST import *

global bn_califr_log_SIL_label
global bn_califr_log_SIL_opt
global faseth_log_SIL_label
global faseth_log_SIL_opt
global srr_debug_log_SIL_label
global srr_debug_log_SIL_opt
global srr_reference_log_SIL_label
global srr_reference_log_SIL_opt
# global bn_califr_SIL_button
# global faseth_log_SIL_button
# global srr_deb_SIL_button
# global srr_ref_log_SIL_button

bn_califr_log_SIL_label = None
bn_califr_log_SIL_opt = None
# bn_califr_SIL_button = None
faseth_log_SIL_label = None
faseth_log_SIL_opt = None
# faseth_log_SIL_button = None
srr_debug_log_SIL_label = None
srr_debug_log_SIL_opt = None
# srr_deb_SIL_button = None
srr_reference_log_SIL_label = None
srr_reference_log_SIL_opt = None
# srr_ref_log_SIL_button = None

global bn_califr_log_SIL_list
bn_califr_log_SIL_list = []

global faseth_log_SIL_list
faseth_log_SIL_list = []

global srr_debug_log_SIL_list
srr_debug_log_SIL_list = []

global srr_reference_log_SIL_list
srr_reference_log_SIL_list = []

""" ----------Class Declaration---------- """


class SIL_Global_var_class(object):
    def __init__(self):
        self.Customer_Name = None
        self.SIL_offset_X = M_Tool.offset_x
        self.SIL_offset_y = M_Tool.offset_y
        self.SIL_Tool_config_done = False
        self.SIL_Ethernet_config_done = False
        self.Default_Var = None
        self.Backup_Radar_pos = False
        self.SIL_Tool_Radiobutton_done = False
        self.SIL_Tool_data_save = False
        self.application_buttons_init_done = False


class SIL_Sensor_and_ECU(object):
    def __init__(self):
        self.REAR_LEFT = None
        self.REAR_RIGHT = None
        self.FRONT_LEFT = None
        self.FRONT_RIGHT = None
        self.FRONT_CENTRE = None
        self.BP_LEFT = None
        self.BP_RIGHT = None
        self.RADAR_ECU = None


'''class SIL_Output_Format(object):
    def __init__(self):
        self.ORCAS_MDF4 = None
        self.CANAPE_MDF4 = None
        self.VIGEM_VECTOR_MDF4 = None
        self.VIGEM_VPCAP = None
        self.VIGEM_CCA_MDF4 = None
        self.X2E_VECTOR_MDF4 = None
        self.CANOE_VECTOR_MF4 = None
'''


class SIL_Tool_Config_variables(object):
    def __init__(self):
        self.JSON_Flist_Path = StringVar()
        self.output_Path_options = StringVar()
        self.output_Path_Location = StringVar()
        self.Sensor_Config = StringVar()
        self.RECU_Config = StringVar()
        self.CAN_Output = IntVar()
        self.APTIV_Internal_Output = IntVar()
        self.CDC_Write_output = IntVar()
        self.Output_File_Format = StringVar()
        self.TS_TRACE = StringVar()
        self.DEBUG_TRACE = StringVar()
        self.ORCAS_MDF4 = IntVar()
        self.CANAPE_MDF4 = IntVar()
        self.VIGEM_VECTOR_MDF4 = IntVar()
        self.VIGEM_VPCAP = IntVar()
        self.VIGEM_CCA_MDF4 = IntVar()
        self.X2E_VECTOR_MDF4 = IntVar()
        self.CANOE_VECTOR_MF4 = IntVar()
        self.TIMING_PROFILE = IntVar()
        self.TIMING_PROFILE_Path = StringVar()
        self.RESIM_ERROR_TRACE_Path = StringVar()
        self.SIGNAL_CREATION = StringVar()
        self.PLP_TIMESTAMP = StringVar()
        # self.TIMING_PROFILE_Path_entry = StringVar()
        # self.TIMING_PROFILE_Path_Options = StringVar()
        self.GDSR_Init_Status = IntVar()
        self.GDSR_Periodic_Status = IntVar()
        self.SIL_Injection_Mode = StringVar()
        self.LOG_Replay_Mode = StringVar()
        self.SIL_Entrypoint = StringVar()
        self.Calibration_Source = StringVar()
        self.STATISTIC_REPORT_PATH = StringVar()
        self.Log_Include_Date_Time = IntVar()
        self.Log_Tracing_Path = StringVar()
        self.Log_Level = StringVar()


class SIL_widgets(object):
    def __init__(self):
        self.SIL_Tool_config_label = None
        self.SENSOR_POSITION_label = None
        self.Customer_spn = None
        self.Customer_label_spn = None
        self.output_Path_options_spn = None
        self.output_Path_options_label = None
        self.output_Path_Location_spn = None
        self.output_Path_Location_label = None
        self.output_Path_Location_Button = None
        # self.JSON_Flist_Path_spn = None
        # self.JSON_Flist_Path_label = None
        # self.JSON_Flist_Path_Button = None
        self.Sensor_Config_spn = None
        self.RECU_Config_spn = None
        self.CAN_Output_chkbtn = None
        self.APTIV_Internal_Output_chkbtn = None
        self.CDC_Write_output_chkbtn = None
        self.TIMING_PROFILE_chkbtn = None
        # self.TIMING_PROFILE_Path = None
        # self.TIMING_PROFILE_Path_Options = None
        # self.TIMING_PROFILE_Path_Button = None
        self.GDSR_Init_Status_chkbtn = None
        self.GDSR_Periodic_Status_chkbtn = None
        self.ORCAS_MDF4_chkbtn = None
        self.CANAPE_MDF4_chkbtn = None
        self.VIGEM_VECTOR_MDF4_chkbtn = None
        self.VIGEM_VPCAP_chkbtn = None
        self.VIGEM_CCA_MDF4_chkbtn = None
        self.X2E_VECTOR_MDF4_chkbtn = None
        self.CANOE_VECTOR_MF4_chkbtn = None
        self.Log_Include_Date_Time_chkbtn = None
        self.TS_TRACE_spn = None
        self.DEBUG_TRACE_spn = None
        self.SIL_Injection_Mode_spn = None
        # self.Output_File_Format_spn = None
        self.LOG_Replay_Mode_spn = None
        self.SIL_Entrypoint_spn = None
        self.Calibration_Source_spn = None
        self.TS_TRACE_label = None
        self.DEBUG_TRACE_label = None
        self.Sensor_Config_label = None
        self.RECU_Config_label = None
        self.TIMING_PROFILE_Path_Label = None
        self.TIMING_PROFILE_Path_spn = None
        # self.TIMING_PROFILE_Path_Button = None
        self.RESIM_ERROR_TRACE_Label = None
        self.RESIM_ERROR_TRACE_spn = None
        self.PLP_TIMESTAMP_Label = None
        self.PLP_TIMESTAMP_spn = None
        self.SIGNAL_CREATION_Label = None
        self.SIGNAL_CREATION_spn = None
        self.Sensor_Config_Button = None
        self.RECU_Config_Button = None
        self.Output_File_Format_label = None
        self.SIL_Injection_Mode_label = None
        self.LOG_Replay_Mode_label = None
        self.SIL_Entrypoint_label = None
        self.Calibration_Source_label = None
        self.STATISTIC_REPORT_PATH_label = None
        self.STATISTIC_REPORT_PATH_spn = None
        self.Log_Tracing_Path_label = None
        self.Log_Tracing_Path_spn = None
        # self.progress_bar = None
        # self.progress_bar_label = None
        self.Log_Level_label = None
        self.Log_Level_spn = None

    def SIL_Clear_all_Tool_config(self):
        self.SIL_Tool_config_label.place_forget()
        # self.SENSOR_POSITION_label.place_forget()
        # self.Customer_spn.place_forget()
        # self.Customer_label_spn.place_forget()
        self.output_Path_options_label.place_forget()
        self.output_Path_options_spn.place_forget()
        self.output_Path_Location_label.place_forget()
        self.output_Path_Location_spn.place_forget()
        self.output_Path_Location_Button.place_forget()
        # self.JSON_Flist_Path_label.place_forget()
        # self.JSON_Flist_Path_spn.place_forget()
        # self.JSON_Flist_Path_Button.place_forget()
        self.Sensor_Config_spn.place_forget()
        self.RECU_Config_spn.place_forget()
        self.Sensor_Config_Button.place_forget()
        self.RECU_Config_Button.place_forget()
        self.Log_Include_Date_Time_chkbtn.place_forget()
        self.CAN_Output_chkbtn.place_forget()
        self.APTIV_Internal_Output_chkbtn.place_forget()
        self.CDC_Write_output_chkbtn.place_forget()
        self.ORCAS_MDF4_chkbtn.place_forget()
        self.CANAPE_MDF4_chkbtn.place_forget()
        self.VIGEM_VECTOR_MDF4_chkbtn.place_forget()
        self.VIGEM_VPCAP_chkbtn.place_forget()
        self.VIGEM_CCA_MDF4_chkbtn.place_forget()
        self.X2E_VECTOR_MDF4_chkbtn.place_forget()
        self.CANOE_VECTOR_MF4_chkbtn.place_forget()
        self.TS_TRACE_label.place_forget()
        self.TS_TRACE_spn.place_forget()
        self.DEBUG_TRACE_label.place_forget()
        self.DEBUG_TRACE_spn.place_forget()
        self.TIMING_PROFILE_chkbtn.place_forget()
        self.GDSR_Init_Status_chkbtn.place_forget()
        self.GDSR_Periodic_Status_chkbtn.place_forget()
        self.SIL_Injection_Mode_spn.place_forget()
        self.LOG_Replay_Mode_spn.place_forget()
        self.SIL_Entrypoint_spn.place_forget()
        self.Calibration_Source_spn.place_forget()
        self.Sensor_Config_label.place_forget()
        self.RECU_Config_label.place_forget()
        self.Sensor_Config_Button.place_forget()
        self.RECU_Config_Button.place_forget()
        self.SIL_Injection_Mode_label.place_forget()
        self.Output_File_Format_label.place_forget()
        self.LOG_Replay_Mode_label.place_forget()
        self.SIL_Entrypoint_label.place_forget()
        self.Calibration_Source_label.place_forget()
        self.TIMING_PROFILE_Path_Label.place_forget()
        self.TIMING_PROFILE_Path_spn.place_forget()
        self.RESIM_ERROR_TRACE_Label.place_forget()
        self.RESIM_ERROR_TRACE_spn.place_forget()
        self.PLP_TIMESTAMP_Label.place_forget()
        self.PLP_TIMESTAMP_spn.place_forget()
        self.SIGNAL_CREATION_Label.place_forget()
        self.SIGNAL_CREATION_spn.place_forget()
        self.STATISTIC_REPORT_PATH_label.place_forget()
        self.STATISTIC_REPORT_PATH_spn.place_forget()
        self.Log_Tracing_Path_label.place_forget()
        self.Log_Tracing_Path_spn.place_forget()
        self.Log_Level_label.place_forget()
        self.Log_Level_spn.place_forget()

    def Clear_all(self):
        self.Customer_spn.place_forget()
        self.Customer_label_spn.place_forget()
        self.SENSOR_POSITION_label.place_forget()
        self.SIL_Clear_all_Tool_config()
        # self.progress_bar.place_forget()
        # self.progress_bar_label.place_forget()


class SIL_Sensor_pos_widgets(object):
    def __init__(self):
        self.REAR_LEFT_chkbtn = None
        self.REAR_RIGHT_chkbtn = None
        self.FRONT_LEFT_chkbtn = None
        self.FRONT_RIGHT_chkbtn = None
        self.FRONT_CENTRE_chkbtn = None
        self.BP_LEFT_chkbtn = None
        self.BP_RIGHT_chkbtn = None
        self.RADAR_ECU_chkbtn = None

    def Clear_all_SIL_Sensor_pos_widgets(self):
        self.REAR_LEFT_chkbtn.place_forget()
        self.REAR_RIGHT_chkbtn.place_forget()
        self.FRONT_LEFT_chkbtn.place_forget()
        self.FRONT_RIGHT_chkbtn.place_forget()
        self.FRONT_CENTRE_chkbtn.place_forget()
        self.BP_LEFT_chkbtn.place_forget()
        self.BP_RIGHT_chkbtn.place_forget()
        self.RADAR_ECU_chkbtn.place_forget()


class SIL_Sensor_and_ECU_position(object):
    def __init__(self):
        self.REAR_LEFT = 0
        self.REAR_RIGHT = 1
        self.FRONT_LEFT = 2
        self.FRONT_RIGHT = 3
        self.FRONT_CENTRE = 4
        self.BP_LEFT = 5
        self.BP_RIGHT = 6
        self.RADAR_ECU = 7


class Tool_config_options(object):
    def __init__(self):
        self.SIL_Tool_cfg = None
        self.SIL_ENET_cfg = None

    def Clear_Tool_config_options(self):
        self.SIL_Tool_cfg.place_forget()
        self.SIL_ENET_cfg.place_forget()


class SIL_Ethernet_config_variables(object):
    def __init__(self):
        self.DESTINATION_IP = "127.0.0.1"
        self.DEST_OUTPUT_PORT = 5555
        self.DEST_INPUT_PORT = 5556
        self.OUTPUT_UDP_TRANSMISSION = None
        self.INPUT_UDP_TRANSMISSION = None
        self.TRANSMISSION_STATUS = None
        # self.LogLevel = None
        # self.LogTracingPath = None
        # self.LogIncludeDateTime = None
        # self.STATISTIC_REPORT_PATH = None


class SIL_Ethernet_config_widgets(object):
    def __init__(self):
        self.SIL_Ethernet_config_label = None
        self.DESTINATION_IP_Entry = None
        self.DEST_OUTPUT_PORT_Entry = None
        self.DEST_INPUT_PORT_Entry = None
        self.OUTPUT_UDP_TRANSMISSION_Entry = None
        self.INPUT_UDP_TRANSMISSION_Entry = None
        self.TRANSMISSION_STATUS_Entry = None
        # self.LogLevel_Entry = None
        # self.LogTracingPath_Entry = None
        # self.LogIncludeDateTime_Entry = None
        self.STATISTIC_REPORT_PATH_Entry = None
        self.DESTINATION_IP_label = None
        self.DEST_OUTPUT_PORT_label = None
        self.DEST_INPUT_PORT_label = None
        self.OUTPUT_UDP_TRANSMISSION_label = None
        self.INPUT_UDP_TRANSMISSION_label = None
        self.TRANSMISSION_STATUS_label = None
        # self.LogLevel_label = None
        # self.LogTracingPath_label = None
        # self.LogTracingPath_Button = None
        # self.LogIncludeDateTime_label = None
        # self.STATISTIC_REPORT_PATH_label = None
        # self.STATISTIC_REPORT_PATH_Button = None

    def Clear_all_ENET_config(self):
        self.SIL_Ethernet_config_label.place_forget()
        self.DESTINATION_IP_Entry.place_forget()
        self.DEST_OUTPUT_PORT_Entry.place_forget()
        self.DEST_INPUT_PORT_Entry.place_forget()
        self.OUTPUT_UDP_TRANSMISSION_Entry.place_forget()
        self.INPUT_UDP_TRANSMISSION_Entry.place_forget()
        self.TRANSMISSION_STATUS_Entry.place_forget()
        # self.LogLevel_Entry.place_forget()
        # self.LogTracingPath_Entry.place_forget()
        # self.LogIncludeDateTime_Entry.place_forget()
        # self.STATISTIC_REPORT_PATH_Entry.place_forget()
        self.DESTINATION_IP_label.place_forget()
        self.DEST_OUTPUT_PORT_label.place_forget()
        self.DEST_INPUT_PORT_label.place_forget()
        self.OUTPUT_UDP_TRANSMISSION_label.place_forget()
        self.INPUT_UDP_TRANSMISSION_label.place_forget()
        self.TRANSMISSION_STATUS_label.place_forget()
        # self.LogLevel_label.place_forget()
        # self.LogTracingPath_label.place_forget()
        # self.LogTracingPath_Button.place_forget()
        # self.LogIncludeDateTime_label.place_forget()
        # self.STATISTIC_REPORT_PATH_label.place_forget()
        # self.STATISTIC_REPORT_PATH_Button.place_forget()
        return


class Application_widgets(object):
    def __init__(self):
        self.Start_btn = None
        self.Save_btn = None
        self.Clear_btn = None
        self.Json_btn = None

    def Clear_Application_widgets(self):
        self.Start_btn.place_forget()
        self.Save_btn.place_forget()
        self.Clear_btn.place_forget()
        self.Json_btn.place_forget()
        return


"""==========================================="""

""" ------------Global variables & Class objects------------- """
SIL_Global_var = SIL_Global_var_class()
SIL_widgets_obj = SIL_widgets()
SIL_Sensor_and_ECU_obj = SIL_Sensor_and_ECU()
SIL_Sensor_pos_widgets_obj = SIL_Sensor_pos_widgets()
# = SIL_Output_Format()
# SIL_Output_format_widgets_obj = SIL_Output_format_widgets()
SIL_sensor_ID = SIL_Sensor_and_ECU_position()
# = SIL_Output_format_position()
SIL_Tool_config = SIL_Tool_Config_variables()
Tool_cfg = Tool_config_options()
SIL_Enet_widgets = SIL_Ethernet_config_widgets()
SIL_Enet_Var = SIL_Ethernet_config_variables()
App_widgets = Application_widgets()

""" Cross dependency inclusion """

from SIL_XML_writer import *

"""============================"""
global SIL_sensor
SIL_sensor = [0, 0, 0, 0, 0, 0, 0, 0]  # This global variable used to back up the sensor selection data
global SIL_sensor_active
SIL_sensor_active = False

"""==========================================="""


def clear_log_entries():
    bn_califr_log_SIL_opt.delete(0, END)
    faseth_log_SIL_opt.delete(0, END)
    srr_debug_log_SIL_opt.delete(0, END)
    srr_reference_log_SIL_opt.delete(0, END)


def forget_log():
    bn_califr_log_SIL_label.place_forget()
    bn_califr_log_SIL_opt.place_forget()
    bn_califr_SIL_button.place_forget()
    faseth_log_SIL_label.place_forget()
    faseth_log_SIL_opt.place_forget()
    faseth_log_SIL_button.place_forget()
    srr_debug_log_SIL_label.place_forget()
    srr_debug_log_SIL_opt.place_forget()
    srr_deb_SIL_button.place_forget()
    srr_reference_log_SIL_label.place_forget()
    srr_reference_log_SIL_opt.place_forget()
    srr_ref_log_SIL_button.place_forget()


def delete_list_logs():
    bn_califr_log_SIL_list.clear()
    faseth_log_SIL_list.clear()
    srr_debug_log_SIL_list.clear()
    srr_reference_log_SIL_list.clear()


def Check_SIL_customer_name(self, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    global our_canvas
    our_canvas = Canvas(height=620, width=1200, bg=Bg_colour)
    our_canvas.pack()
    # creating rectangle
    our_canvas.create_rectangle(40, 115, 330, 49, fill=None)
    global SIL_Global_var
    # Creating Customer_label object to Customer_label_spn

    self.Customer_label_spn = Label(root, text="CUSTOMER NAME", font=('arial', 12, 'bold'), bg=Bg_colour)
    self.Customer_label_spn.place(x=SENSOR_POSITION_X_val, y=SENSOR_POSITION_Y_val)

    # Customer_list will hold a number of options which is available to choose
    Customer_list = (
        'SELECT CUSTOMER', 'RNA_SUV', 'RNA_CDV', 'GEELY_SRR5', 'GEELY_SRR5_INTERNAL', 'CHANGAN_SRR5', 'SGM_358_SRR5',
        'GWM_SRR5', 'BMW_LOW', 'BMW_MID', 'BMW_HIGH', 'BMW_MID_LOW', 'MAN_SRR3')  # 'MAN_SRR3'

    # Below statement will help to store/change the data when spinbox status change
    SIL_Global_var.Default_Var = StringVar()
    self.Customer_spn = Spinbox(root, textvariable=SIL_Global_var.Default_Var, font=('arial', 10, 'bold'), bg='white',
                                values=Customer_list, wrap=True,
                                command=lambda: Update_SIL_customer_name(self.Customer_spn))
    SIL_Global_var.Default_Var.set('SELECT CUSTOMER')
    self.Customer_spn.place(x=SENSOR_POSITION_X_val, y=(SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1.5)))

    '''self.progress_bar = Progressbar(root, style="green.Horizontal.TProgressbar", orient=HORIZONTAL,
                                    length=275, mode='determinate')

    self.progress_bar.place(x=SENSOR_POSITION_X_val + 800,
                            y=(SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 19)))

    self.progress_bar_label = Label(root, text="PROGRESS ", font=('arial', 10, 'bold'), bg=Bg_colour)
    self.progress_bar_label.place(x=SENSOR_POSITION_X_val + 700,
                                  y=(SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 18.8)))'''
    return


def Update_SIL_Tool_offset():
    global SIL_Global_var
    # Global offsets setting based on the OS used
    SIL_Global_var.SIL_offset_X = M_Tool.offset_x
    SIL_Global_var.SIL_offset_y = M_Tool.offset_y
    return


def Update_SIL_customer_name(obj):
    """our_canvas = Canvas(height=620, width=1200, bg=Bg_colour)
    our_canvas.pack()
    # creating rectangle
    our_canvas.create_rectangle(40, 115, 250, 49, fill=None)
    # our_canvas.create_rectangle(500, 350, 300, 100, fill=None)"""
    global SIL_Global_var
    global SIL_Sensor_and_ECU_obj
    SIL_Global_var.Customer_Name = obj.get()
    print('Customer Name: ', SIL_Global_var.Customer_Name)
    Display_SIL_radar_pos_options(SIL_Sensor_and_ECU_obj, SIL_Sensor_pos_widgets_obj, SIL_sensor_ID, 50, 120)
    # progress_update(10)
    return


def Display_SIL_radar_pos_options(self, Sensor_pos, SIL_sen_ID, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    our_canvas.create_rectangle(40, 320, 330, 119, fill=None)
    global SIL_Global_var
    global SIL_widgets_obj
    SIL_widgets_obj.SENSOR_POSITION_label = Label(root, text="SENSOR POSITION", font=('arial', 12, 'bold'),
                                                  bg=Bg_colour)
    SIL_widgets_obj.SENSOR_POSITION_label.place(x=SENSOR_POSITION_X_val, y=SENSOR_POSITION_Y_val)

    self.REAR_LEFT = IntVar()
    Sensor_pos.REAR_LEFT_chkbtn = Checkbutton(root, text="RL", variable=self.REAR_LEFT,
                                              font=('arial', 10, 'bold'),
                                              bg=Bg_colour,
                                              onvalue=1, offvalue=0,
                                              command=lambda: SIL_set_sensor(self, SIL_sen_ID.REAR_LEFT,
                                                                             self.REAR_LEFT))
    self.REAR_RIGHT = IntVar()
    Sensor_pos.REAR_RIGHT_chkbtn = Checkbutton(root, text="RR", variable=self.REAR_RIGHT,
                                               font=('arial', 10, 'bold'),
                                               bg=Bg_colour, onvalue=1, offvalue=0,
                                               command=lambda: SIL_set_sensor(self, SIL_sen_ID.REAR_RIGHT,
                                                                              self.REAR_RIGHT))
    self.FRONT_LEFT = IntVar()
    Sensor_pos.FRONT_LEFT_chkbtn = Checkbutton(root, text="FL", variable=self.FRONT_LEFT,
                                               font=('arial', 10, 'bold'),
                                               bg=Bg_colour, onvalue=1, offvalue=0,
                                               command=lambda: SIL_set_sensor(self, SIL_sen_ID.FRONT_LEFT,
                                                                              self.FRONT_LEFT))
    self.FRONT_RIGHT = IntVar()
    Sensor_pos.FRONT_RIGHT_chkbtn = Checkbutton(root, text="FR", variable=self.FRONT_RIGHT,
                                                font=('arial', 10, 'bold'),
                                                bg=Bg_colour, onvalue=1, offvalue=0,
                                                command=lambda: SIL_set_sensor(self, SIL_sen_ID.FRONT_RIGHT,
                                                                               self.FRONT_RIGHT))
    self.FRONT_CENTRE = IntVar()
    Sensor_pos.FRONT_CENTRE_chkbtn = Checkbutton(root, text="FC", variable=self.FRONT_CENTRE,
                                                 font=('arial', 10, 'bold'),
                                                 bg=Bg_colour, onvalue=1, offvalue=0,
                                                 command=lambda: SIL_set_sensor(self, SIL_sen_ID.FRONT_CENTRE,
                                                                                self.FRONT_CENTRE))
    self.BP_LEFT = IntVar()
    self.BP_RIGHT = IntVar()

    Sensor_pos.BP_LEFT_chkbtn = Checkbutton(root, text="BPIL", variable=self.BP_LEFT, font=('arial', 10, 'bold'),
                                            bg=Bg_colour,
                                            onvalue=1, offvalue=0,
                                            command=lambda: SIL_set_sensor(self, SIL_sen_ID.BP_LEFT, self.BP_LEFT))

    Sensor_pos.BP_RIGHT_chkbtn = Checkbutton(root, text="BPIR", variable=self.BP_RIGHT, font=('arial', 10, 'bold'),
                                             bg=Bg_colour,
                                             onvalue=1, offvalue=0,
                                             command=lambda: SIL_set_sensor(self, SIL_sen_ID.BP_RIGHT, self.BP_RIGHT))

    self.RADAR_ECU = IntVar()
    Sensor_pos.RADAR_ECU_chkbtn = Checkbutton(root, text="RECU", variable=self.RADAR_ECU,
                                              font=('arial', 10, 'bold'),
                                              bg=Bg_colour,
                                              onvalue=1, offvalue=0,
                                              command=lambda: SIL_set_sensor(self, SIL_sen_ID.RADAR_ECU,
                                                                             self.RADAR_ECU))
    """--------------------------------------------------------------------------------------------------"""
    if SIL_Global_var.Customer_Name == 'MAN_SRR3':
        Sensor_pos.FRONT_CENTRE_chkbtn.configure(state='normal')
    elif SIL_Global_var.Customer_Name == 'BMW_LOW' or 'BMW_MID' or 'BMW_HIGH' or 'RNA_SUV' or 'RNA_CDV' or ' BMW_MID_LOW' or 'GEELY_SRR5' or 'GEELY_SRR5_INTERNAL' or 'HKMC_SRR5' or 'CHANGAN_SRR5':
        Sensor_pos.FRONT_CENTRE_chkbtn.configure(state='disabled')

    '''SIL_Global_var.Customer_Name == 'MAN_SRR3':
    Sensor_pos.FRONT_CENTRE_chkbtn.configure(state='enabled')
    SIL_Global_var.Customer_Name == 'BMW_LOW' or 'BMW_MID' or 'BMW_HIGH' or 'RNA_SUV' or 'RNA_CDV' or 'BMW_HIGH' or 'GEELY_SRR5' or 'HKMC_SRR5' or 'CHANGAN_SRR5':
    Sensor_pos.FRONT_CENTRE_chkbtn.configure(state='disabled')'''
    """--------------------------------------------------------------------------------------------------"""

    """--------------------------------------------------------------------------------------------------"""
    if SIL_Global_var.Customer_Name == 'GEELY_SRR5_INTERNAL':
        Sensor_pos.FRONT_CENTRE_chkbtn.configure(state='disable')
        Sensor_pos.BP_LEFT_chkbtn.configure(state='disable')
        Sensor_pos.BP_RIGHT_chkbtn.configure(state='disable')
        Sensor_pos.RADAR_ECU_chkbtn.configure(state='disable')

    """--------------------------------------------------------------------------------------------------"""

    """--------------------------------------------------------------------------------------------------"""

    """--------------------------------------------------------------------------------------------------"""

    '''if SIL_Global_var.Customer_Name == 'BMW_MID_LOW':
        Sensor_pos.FRONT_CENTRE_chkbtn.configure(state='disabled')
        Sensor_pos.BP_LEFT_chkbtn.configure(state='disabled')
        Sensor_pos.BP_RIGHT_chkbtn.configure(state='disabled')
        # Sensor_pos.RADAR_ECU_chkbtn.configure(state='normal')
    else:
        pass'''

    """--------------------------------------------------------------------------------------------------"""

    """--------------------------------------------------------------------------------------------------"""

    if SIL_Global_var.Customer_Name == 'BMW_MID':
        Sensor_pos.BP_RIGHT_chkbtn.configure(state='disabled')
        Sensor_pos.BP_LEFT_chkbtn.configure(state='disabled')
    elif SIL_Global_var.Customer_Name != 'BMW_HIGH':
        Sensor_pos.BP_RIGHT_chkbtn.configure(state='disabled')
        Sensor_pos.BP_LEFT_chkbtn.configure(state='disabled')
        Sensor_pos.RADAR_ECU_chkbtn.configure(state='disabled')
    else:
        pass

    """--------------------------------------------------------------------------------------------------"""
    Sensor_pos.REAR_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val,
                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1))
    Sensor_pos.REAR_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val + 60,
                                       y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1))
    Sensor_pos.FRONT_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val + 120,
                                       y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1))
    Sensor_pos.FRONT_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val + 180,
                                        y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1))
    Sensor_pos.FRONT_CENTRE_chkbtn.place(x=SENSOR_POSITION_X_val,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1.9))
    Sensor_pos.BP_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val + 60,
                                    y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1.9))
    Sensor_pos.BP_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val + 120,
                                     y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1.9))
    Sensor_pos.RADAR_ECU_chkbtn.place(x=SENSOR_POSITION_X_val + 180,
                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1.9))
    if SIL_Global_var.Backup_Radar_pos:
        Data_backup_Display_radar_pos_options(SIL_sen_ID, Sensor_pos)
    SIL_Global_var.Backup_Radar_pos = True
    return


def Data_backup_Display_radar_pos_options(self, Sensor_pos):
    global SIL_sensor
    if SIL_sensor[self.REAR_LEFT]:
        Sensor_pos.REAR_LEFT_chkbtn.select()
    if SIL_sensor[self.REAR_RIGHT]:
        Sensor_pos.REAR_RIGHT_chkbtn.select()
    if SIL_sensor[self.FRONT_LEFT]:
        Sensor_pos.FRONT_LEFT_chkbtn.select()
    if SIL_sensor[self.FRONT_RIGHT]:
        Sensor_pos.FRONT_RIGHT_chkbtn.select()
    if SIL_sensor[self.FRONT_CENTRE]:
        Sensor_pos.FRONT_CENTRE_chkbtn.select()
    if SIL_sensor[self.BP_LEFT]:
        Sensor_pos.BP_LEFT_chkbtn.select()
    if SIL_sensor[self.BP_RIGHT]:
        Sensor_pos.BP_RIGHT_chkbtn.select()
    if SIL_sensor[self.RADAR_ECU]:
        Sensor_pos.RADAR_ECU_chkbtn.select()
    return


def SIL_set_sensor(self, Index, Val):
    global SIL_sensor
    global SIL_Global_var
    global SIL_widgets_obj
    global SIL_Tool_config
    SIL_sum = 0
    if Val is not None:
        SIL_sensor[Index] = Val.get()
    else:
        print('fail', Val)
    for Ind in range(len(SIL_sensor)):
        SIL_sum += SIL_sensor[Ind]  # checking for at-least 1 sensor is selected or not
    if SIL_sum != 0:
        print('pass', Val)
        #   If any one of sensor selected calling the SIL config
        #   This function will display all the SIL Tool widgets
        SIL_Tool_Configuration_options(SIL_widgets_obj, SIL_Global_var, SIL_Tool_config, 370, 40)
    # Activ_sensor_ECU(stream_options_chkbtn_obj,310,60)
    else:
        pass

    return


def SIL_Tool_configuration(self, E_Var, Global_var, Tool_config, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    our_canvas.create_rectangle(725, 360, 355, 70, fill=None)
    global SIL_Tool_config
    global SIL_widgets_obj
    global SIL_Enet_widgets
    # print("SIL_Tool_configuration inside")
    if Global_var.SIL_Ethernet_config_done:
        # If the Ethernet widgets are displayed already
        # We have to hide those widgets before calling HIL Tool config display
        # print("EnetTool Config_1", Global_var.SIL_Ethernet_config_done)
        SIL_Enet_widgets.Clear_all_ENET_config()
        Global_var.Ethernet_config_done = False
        pass
    if not Global_var.SIL_Tool_config_done:
        # print("SIL Tool Config_1", Global_var.SIL_Tool_config_done)
        """ --------------  check buttons  ---------------- """

        SIL_widgets_obj.SIL_Tool_config_label = Label(root, text="SIL TOOL CONFIGURATION", font=('arial', 14, 'bold'),
                                                      bg=Bg_colour)

        self.CAN_Output_chkbtn = Checkbutton(root, text="CUSTOMER CAN OUTPUT",
                                             variable=Tool_config.CAN_Output,
                                             font=('arial', 10, 'bold'),
                                             bg=Bg_colour, onvalue=1, offvalue=0)

        self.APTIV_Internal_Output_chkbtn = Checkbutton(root, text="APTIV INTERNAL OUTPUT",
                                                        variable=Tool_config.APTIV_Internal_Output,
                                                        font=('arial', 10, 'bold'),
                                                        bg=Bg_colour, onvalue=1, offvalue=0)
        self.CDC_Write_output_chkbtn = Checkbutton(root, text="CDC WRITE OUTPUT ENABLE",
                                                   variable=Tool_config.CDC_Write_output,
                                                   font=('arial', 10, 'bold'),
                                                   bg=Bg_colour, onvalue=1, offvalue=0)

        '''self.TS_TRACE_chkbtn = Checkbutton(root, text="TS TRACE",
                                           variable=Tool_config.TS_TRACE, font=('arial', 10, 'bold'),
                                           bg=Bg_colour, onvalue=1, offvalue=0)

        self.DEBUG_TRACE_chkbtn = Checkbutton(root, text="DEBUG TRACE",
                                              variable=Tool_config.DEBUG_TRACE, font=('arial', 10, 'bold'),
                                              bg=Bg_colour, onvalue=1, offvalue=0)'''
        self.TS_TRACE_label = Label(root, text="Timestamp Trace Path", font=('arial', 10, 'bold'),
                                    bg=Bg_colour)

        Timestamp_Trace = ('NONE', 'SAME_AS_INPUT', 'SAME_AS_OUTPUT')
        self.TS_TRACE_spn = Spinbox(root, textvariable=Tool_config.TS_TRACE, font=('arial', 10, 'bold'),
                                    values=Timestamp_Trace, wrap=True, bg='white')
        self.DEBUG_TRACE_label = Label(root, text="Log Scan Drop Trace Path", font=('arial', 10, 'bold'),
                                       bg=Bg_colour)

        Debug_Trace = ('NONE', 'SAME_AS_INPUT', 'SAME_AS_OUTPUT')
        self.DEBUG_TRACE_spn = Spinbox(root, textvariable=Tool_config.DEBUG_TRACE, font=('arial', 10, 'bold'),
                                       values=Debug_Trace, wrap=True, bg='white')

        self.ORCAS_MDF4_chkbtn = Checkbutton(root, text="ORCAS_MDF4", variable=Tool_config.ORCAS_MDF4,
                                             font=('arial', 10, 'bold'),
                                             bg=Bg_colour, onvalue=1, offvalue=0)
        self.CANAPE_MDF4_chkbtn = Checkbutton(root, text="CANAPE_MDF4", variable=Tool_config.CANAPE_MDF4,
                                              font=('arial', 10, 'bold'),
                                              bg=Bg_colour, onvalue=1, offvalue=0)
        self.VIGEM_VECTOR_MDF4_chkbtn = Checkbutton(root, text="VIGEM_VECTOR",
                                                    variable=Tool_config.VIGEM_VECTOR_MDF4,
                                                    font=('arial', 10, 'bold'),
                                                    bg=Bg_colour, onvalue=1, offvalue=0)
        self.VIGEM_VPCAP_chkbtn = Checkbutton(root, text="VIGEM_VPCAP",
                                              variable=Tool_config.VIGEM_VPCAP,
                                              font=('arial', 10, 'bold'),
                                              bg=Bg_colour, onvalue=1, offvalue=0)
        self.VIGEM_CCA_MDF4_chkbtn = Checkbutton(root, text="VIGEM_CCA_MDF4",
                                                 variable=Tool_config.VIGEM_CCA_MDF4,
                                                 font=('arial', 10, 'bold'),
                                                 bg=Bg_colour, onvalue=1, offvalue=0)
        self.X2E_VECTOR_MDF4_chkbtn = Checkbutton(root, text="X2E_VECTOR",
                                                  variable=Tool_config.X2E_VECTOR_MDF4,
                                                  font=('arial', 10, 'bold'),
                                                  bg=Bg_colour, onvalue=1, offvalue=0)
        self.CANOE_VECTOR_MF4_chkbtn = Checkbutton(root, text="CANOE_VECTOR_MF4",
                                                   variable=Tool_config.CANOE_VECTOR_MF4,
                                                   font=('arial', 10, 'bold'),
                                                   bg=Bg_colour, onvalue=1, offvalue=0)

        self.TIMING_PROFILE_chkbtn = Checkbutton(root, text="TIMING PROFILE",
                                                 variable=Tool_config.TIMING_PROFILE,
                                                 font=('arial', 10, 'bold'),
                                                 bg=Bg_colour, onvalue=1, offvalue=0)

        self.GDSR_Init_Status_chkbtn = Checkbutton(root, text="GDSR INIT INJECTION",
                                                   variable=Tool_config.GDSR_Init_Status,
                                                   font=('arial', 10, 'bold'),
                                                   bg=Bg_colour, onvalue=1, offvalue=0)

        self.GDSR_Periodic_Status_chkbtn = Checkbutton(root, text="GDSR PERIODIC INJECTION",
                                                       variable=Tool_config.GDSR_Periodic_Status,
                                                       font=('arial', 10, 'bold'),
                                                       bg=Bg_colour, onvalue=1, offvalue=0)

        self.Output_File_Format_label = Label(root, text="RESIM OUTPUT FORMAT", font=('arial', 10, 'bold'),
                                              bg=Bg_colour)

        '''File_Format = (
            'ORCAS_MDF4', 'CANAPE_MDF4', 'VIGEM_VECTOR_MDF4', 'VIGEM_VPCAP', 'VIGEM_CCA_MDF4', 'X2E_VECTOR_MDF4',
            'CANOE_VECTOR_MF4')
        self.Output_File_Format_spn = Spinbox(root, textvariable=Tool_config.Output_File_Format,
                                              font=('arial', 10, 'bold'),
                                              values=File_Format, wrap=True, bg='white')'''

        self.Calibration_Source_label = Label(root, text="CALIB SOURCE", font=('arial', 10, 'bold'),
                                              bg=Bg_colour)

        SENSOR_MODE = ('LOAD_DEFAULT_CAL', 'LOAD_UDP_CAL')
        self.Calibration_Source_spn = Spinbox(root, textvariable=Tool_config.Calibration_Source,
                                              values=SENSOR_MODE, font=('arial', 10, 'bold'), wrap=True,
                                              bg='white')
        self.TIMING_PROFILE_Path_Label = Label(root, text="Timing Profile Path", font=('arial', 10, 'bold'),
                                               bg=Bg_colour)
        Timing_profile_path = ('NONE', 'SAME_AS_INPUT', 'SAME_AS_OUTPUT')
        self.TIMING_PROFILE_Path_spn = Spinbox(root, textvariable=Tool_config.TIMING_PROFILE_Path,
                                               values=Timing_profile_path, font=('arial', 10, 'bold'), wrap=True,
                                               bg='white')
        self.RESIM_ERROR_TRACE_Label = Label(root, text="Resim Error Trace Path", font=('arial', 10, 'bold'),
                                             bg=Bg_colour)
        Resim_Error_Trace_Path = ('NONE', 'SAME_AS_INPUT', 'SAME_AS_OUTPUT')
        self.RESIM_ERROR_TRACE_spn = Spinbox(root, textvariable=Tool_config.RESIM_ERROR_TRACE_Path,
                                             values=Resim_Error_Trace_Path, font=('arial', 10, 'bold'), wrap=True,
                                             bg='white')
        self.PLP_TIMESTAMP_Label = Label(root, text="PLP Timestamp", font=('arial', 10, 'bold'),
                                         bg=Bg_colour)
        PLP_TIMESTAMP_OPT = ('NONE', 'SAME_AS_INPUT', 'SAME_AS_OUTPUT')
        self.PLP_TIMESTAMP_spn = Spinbox(root, textvariable=Tool_config.PLP_TIMESTAMP,
                                         values=PLP_TIMESTAMP_OPT, font=('arial', 10, 'bold'), wrap=True,
                                         bg='white')
        self.Log_Include_Date_Time_chkbtn = Checkbutton(root, text="LogIncludeDateTime",
                                                        variable=Tool_config.Log_Include_Date_Time,
                                                        font=('arial', 10, 'bold'),
                                                        bg=Bg_colour, onvalue=1, offvalue=0)
        """--------------------------------------------------------------------------------------------------"""

        SIL_widgets_obj.SIL_Tool_config_label.place(x=SENSOR_POSITION_X_val + 170,
                                                    y=SENSOR_POSITION_Y_val)

        self.CAN_Output_chkbtn.place(x=SENSOR_POSITION_X_val,
                                     y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 1.6))

        self.APTIV_Internal_Output_chkbtn.place(x=SENSOR_POSITION_X_val,
                                                y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 2.4))

        self.CDC_Write_output_chkbtn.place(x=SENSOR_POSITION_X_val,
                                           y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 3.2))

        self.GDSR_Init_Status_chkbtn.place(x=SENSOR_POSITION_X_val,
                                           y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 4.1))

        self.GDSR_Periodic_Status_chkbtn.place(x=SENSOR_POSITION_X_val,
                                               y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.0))
        self.TS_TRACE_label.place(x=SENSOR_POSITION_X_val,
                                  y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 10.55))
        self.TS_TRACE_spn.place(x=SENSOR_POSITION_X_val + 185,
                                y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 10.66))
        self.DEBUG_TRACE_label.place(x=SENSOR_POSITION_X_val,
                                     y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 11.4))
        self.DEBUG_TRACE_spn.place(x=SENSOR_POSITION_X_val + 185,
                                   y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 11.5))

        '''self.TS_TRACE_chkbtn.place(x=SENSOR_POSITION_X_val,
                                   y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.9))
        self.DEBUG_TRACE_chkbtn.place(x=SENSOR_POSITION_X_val,
                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.8))'''

        self.TIMING_PROFILE_chkbtn.place(x=SENSOR_POSITION_X_val,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.7))

        self.Output_File_Format_label.place(x=SENSOR_POSITION_X_val - 320,
                                            y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6))
        self.ORCAS_MDF4_chkbtn.place(x=SENSOR_POSITION_X_val - 320,
                                     y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.8))
        self.CANAPE_MDF4_chkbtn.place(x=SENSOR_POSITION_X_val - 340 + 169,
                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.8))
        self.VIGEM_VECTOR_MDF4_chkbtn.place(x=SENSOR_POSITION_X_val - 320,
                                            y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 7.8))
        self.VIGEM_VPCAP_chkbtn.place(x=SENSOR_POSITION_X_val - 340 + 169,
                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 7.8))
        self.VIGEM_CCA_MDF4_chkbtn.place(x=SENSOR_POSITION_X_val - 320,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.8))
        self.X2E_VECTOR_MDF4_chkbtn.place(x=SENSOR_POSITION_X_val - 340 + 169,
                                          y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.8))
        self.CANOE_VECTOR_MF4_chkbtn.place(x=SENSOR_POSITION_X_val - 350 + 80,
                                           y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 9.9))
        '''self.Output_File_Format_spn.place(x=SENSOR_POSITION_X_val + 190,
                                          y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 14))'''

        self.Calibration_Source_label.place(x=SENSOR_POSITION_X_val,
                                            y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.9))
        self.Calibration_Source_spn.place(x=SENSOR_POSITION_X_val + 185,
                                          y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.9))
        self.TIMING_PROFILE_Path_Label.place(x=SENSOR_POSITION_X_val,
                                             y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 9.8))
        self.TIMING_PROFILE_Path_spn.place(x=SENSOR_POSITION_X_val + 185,
                                           y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 9.8))
        self.RESIM_ERROR_TRACE_Label.place(x=SENSOR_POSITION_X_val,
                                           y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.8))
        self.RESIM_ERROR_TRACE_spn.place(x=SENSOR_POSITION_X_val + 185,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.8))
        self.PLP_TIMESTAMP_Label.place(x=SENSOR_POSITION_X_val,
                                       y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 7.7))
        self.PLP_TIMESTAMP_spn.place(x=SENSOR_POSITION_X_val + 185,
                                     y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 7.7))
        self.Log_Include_Date_Time_chkbtn.place(x=SENSOR_POSITION_X_val + 370,
                                                y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 14.9))

        # self.INJECT_FAULT_SENSOR_RR_chkbn.place(x=SENSOR_POSITION_X_val,
        #                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 10.5))
        #
        SIL_Tool_configuration_spinBox(self, E_Var, Global_var, Tool_config, SENSOR_POSITION_X_val + 320,
                                       SENSOR_POSITION_Y_val)

    else:
        pass

    return


def SIL_Tool_configuration_spinBox(self, E_Var, Global_var, Tool_config, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    our_canvas.create_rectangle(1190, 490, 730, 69, fill=None)
    global SIL_Global_var
    x_offset = 250
    Input_List = ('SAME_AS_INPUT', 'SAME_AS_OUTPUT', 'NEW_OUTPUT_PATH')
    self.output_Path_options_label = Label(root, text="OUTPUT_OPTION", font=('arial', 10, 'bold'), bg=Bg_colour)

    self.output_Path_options_spn = Spinbox(root, textvariable=Tool_config.output_Path_options,
                                           font=('arial', 10, 'bold'), bg='white',
                                           values=Input_List, wrap=True)

    """--------------------------------------------------------------------------------------------------"""
    self.output_Path_Location_label = Label(root, text="OUTPUT_PATH_LOCATION", font=('arial', 10, 'bold'),
                                            bg=Bg_colour)
    self.output_Path_Location_spn = Entry(root, font=('arial', 10, 'bold'), width=22, bg='white', fg='green')
    self.output_Path_Location_Button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                              bg=Bg_colour, bd=1,
                                              command=lambda: Get_Folder_path(self.output_Path_Location_Button,
                                                                              Tool_config,
                                                                              self.output_Path_Location_spn))
    """--------------------------------------------------------------------------------------------------"""

    ''''self.TIMING_PROFILE_Path_Label = Label(root, text="Timing Profile Path", font=('arial', 10, 'bold'),
                                           bg=Bg_colour)
    # Timing_profile_path = ('NONE', 'SAME_AS_INPUT', 'SAME_AS_OUTPUT', 'NEW_OUTPUT_PATH')
    # self.TIMING_PROFILE_Path_Options = Spinbox(root, textvariable=Tool_config.TIMING_PROFILE_Path,
      #                                          values=Timing_profile_path, font=('arial', 10, 'bold'), wrap=True,
       #                                         bg='white')
    self.TIMING_PROFILE_Path_entry = Entry(root, font=('arial', 10, 'bold'), width=19, bg='white', fg='green')
    self.TIMING_PROFILE_Path_Button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                             bg=Bg_colour, bd=1,
                                             command=lambda: Get_Log_path(self.TIMING_PROFILE_Path_Button,
                                                                          Tool_config,
                                                                          self.TIMING_PROFILE_Path_entry))'''

    """--------------------------------------------------------------------------------------------------"""
    self.Sensor_Config_label = Label(root, text="SENSOR EMB CONFIG", font=('arial', 10, 'bold'),
                                     bg=Bg_colour)

    self.Sensor_Config_spn = Entry(root, font=('arial', 10, 'bold'), width=22, bg='white', fg='green')

    self.Sensor_Config_Button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                       bg=Bg_colour, bd=1,
                                       command=lambda: Get_json_path(self.Sensor_Config_Button,
                                                                     Tool_config,
                                                                     self.Sensor_Config_spn))
    """--------------------------------------------------------------------------------------------------"""

    self.RECU_Config_label = Label(root, text="RECU EMB CONFIG", font=('arial', 10, 'bold'),
                                   bg=Bg_colour)

    self.RECU_Config_spn = Entry(root, font=('arial', 10, 'bold'), width=22, bg='white', fg='green')

    self.RECU_Config_Button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                     bg=Bg_colour, bd=1,
                                     command=lambda: Get_json_path(self.RECU_Config_Button,
                                                                   Tool_config,
                                                                   self.RECU_Config_spn))

    # if(SIL_Global_var.Customer_Name == 'BMW_HIGH' or SIL_Global_var.Customer_Name == 'BMW_MID'):
    #     print("if:",SIL_Global_var.Customer_Name)
    # else:
    #     print("else:",SIL_Global_var.Customer_Name)
    #     self.RECU_Config_Button.configure(state='disabled')
    #     self.RECU_Config_spn.configure(state='disabled')
    #     self.RECU_Config_label.configure(state='disabled')
    #     pass

    '''self.JSON_Flist_Path_label = Label(root, text="JSON/FLIST_PATH", font=('arial', 10, 'bold'),
                                       bg=Bg_colour)
    self.JSON_Flist_Path_spn = Entry(root, font=('arial', 10, 'bold'), width=22, bg='white', fg='green')
    self.JSON_Flist_Path_Button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                         bg=Bg_colour, bd=1,
                                         command=lambda: Get_json_path(self.JSON_Flist_Path_Button,
                                                                       Tool_config,
                                                                       self.JSON_Flist_Path_spn))'''

    """--------------------------------------------------------------------------------------------------"""
    self.SIL_Injection_Mode_label = Label(root, text="SIL_INJECTION_MODE", font=('arial', 10, 'bold'), bg=Bg_colour)
    RESIM_MODE = ('VEHICLE_EXPEDITION_FILE', 'VIRTUAL_SIMULATION_FILE', 'VIRTUAL_SIMULATION_LIVE')
    self.SIL_Injection_Mode_spn = Spinbox(root, textvariable=Tool_config.SIL_Injection_Mode, font=('arial', 10, 'bold'),
                                          values=RESIM_MODE, wrap=True, bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.LOG_Replay_Mode_label = Label(root, text="LOG_REPLAY_MODE", font=('arial', 10, 'bold'),
                                       bg=Bg_colour)
    Injection_Type = ('Sequential_FILE_Input', 'Continuous_FILE_Input')
    self.LOG_Replay_Mode_spn = Spinbox(root, textvariable=Tool_config.LOG_Replay_Mode,
                                       font=('arial', 10, 'bold'), wrap=True, values=Injection_Type,
                                       bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.SIL_Entrypoint_label = Label(root, text="SIL_ENTRYPOINT", font=('arial', 10, 'bold'),
                                      bg=Bg_colour)

    INPUT_DATA_TYPE = ('CDC', 'DETECTIONS_UDP', 'DSPACE_MODE', 'IGNORE_RESIM_KPI_BAD_LOGS_DETECTION_MODE')
    self.SIL_Entrypoint_spn = Spinbox(root, textvariable=Tool_config.SIL_Entrypoint,
                                      values=INPUT_DATA_TYPE, font=('arial', 10, 'bold'), wrap=True,
                                      bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.SIGNAL_CREATION_Label = Label(root, text="SIGNAL_CREATION", font=('arial', 10, 'bold'), bg=Bg_colour)
    SIGNAL_CREATION_OPT = ('ENABLE', 'DISABLE')
    self.SIGNAL_CREATION_spn = Spinbox(root, textvariable=Tool_config.SIGNAL_CREATION,
                                       values=SIGNAL_CREATION_OPT, font=('arial', 10, 'bold'), wrap=True,
                                       bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.STATISTIC_REPORT_PATH_label = Label(root, text="STATISTIC_REPORT_PATH", font=('arial', 10, 'bold'),
                                             bg=Bg_colour)
    STATISTIC_REPORT_PATH_OPT = ('NONE', 'SAME_AS_INPUT', 'SAME_AS_OUTPUT')
    self.STATISTIC_REPORT_PATH_spn = Spinbox(root, textvariable=Tool_config.STATISTIC_REPORT_PATH,
                                             values=STATISTIC_REPORT_PATH_OPT, font=('arial', 10, 'bold'), wrap=True,
                                             bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.Log_Tracing_Path_label = Label(root, text="Log_Tracing_Path", font=('arial', 10, 'bold'),
                                        bg=Bg_colour)
    Log_Tracing_Path_OPT = ('NONE', 'SAME_AS_INPUT', 'SAME_AS_OUTPUT')
    self.Log_Tracing_Path_spn = Spinbox(root, textvariable=Tool_config.Log_Tracing_Path,
                                        values=Log_Tracing_Path_OPT, font=('arial', 10, 'bold'), wrap=True,
                                        bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.Log_Level_label = Label(root, text="Log_Level", font=('arial', 10, 'bold'),
                                 bg=Bg_colour)
    Log_Level_opt = ('0', '1', '2', '3', '4')
    self.Log_Level_spn = Spinbox(root, textvariable=Tool_config.Log_Level,
                                 values=Log_Level_opt, font=('arial', 10, 'bold'), wrap=True,
                                 bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.output_Path_options_label.place(x=SENSOR_POSITION_X_val + 50,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.6))
    self.output_Path_options_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                       y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.6))

    '''self.TIMING_PROFILE_Path_Label.place(x=SENSOR_POSITION_X_val - 320,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.3))
    # self.TIMING_PROFILE_Path_Options.place(x=SENSOR_POSITION_X_val - 130,
      #                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.3))

    self.TIMING_PROFILE_Path_entry.place(x=SENSOR_POSITION_X_val - 175,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.3))
    self.TIMING_PROFILE_Path_Button.place(x=SENSOR_POSITION_X_val - 20,
                                          y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.2))'''

    self.output_Path_Location_label.place(x=SENSOR_POSITION_X_val + 50,
                                          y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.50))
    self.output_Path_Location_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                        y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.50))
    self.output_Path_Location_Button.place(x=SENSOR_POSITION_X_val + 420,
                                           y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.35))

    self.Sensor_Config_label.place(x=SENSOR_POSITION_X_val + 50,
                                   y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 7.77))
    self.Sensor_Config_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                 y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 7.77))
    self.Sensor_Config_Button.place(x=SENSOR_POSITION_X_val + 420,
                                    y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 7.62))

    self.RECU_Config_label.place(x=SENSOR_POSITION_X_val + 50,
                                 y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.95))
    self.RECU_Config_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                               y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.95))

    self.RECU_Config_Button.place(x=SENSOR_POSITION_X_val + 420,
                                  y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8.85))

    '''self.JSON_Flist_Path_label.place(x=SENSOR_POSITION_X_val + 50,
                                     y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.35))
    self.JSON_Flist_Path_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                   y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.35))
    self.JSON_Flist_Path_Button.place(x=SENSOR_POSITION_X_val + 420,
                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5.3))'''

    self.SIL_Injection_Mode_label.place(x=SENSOR_POSITION_X_val + 50,
                                        y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 10.30))
    self.SIL_Injection_Mode_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 10.30))

    self.LOG_Replay_Mode_label.place(x=SENSOR_POSITION_X_val + 50,
                                     y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 11.30))
    self.LOG_Replay_Mode_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                   y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 11.30))

    self.SIL_Entrypoint_label.place(x=SENSOR_POSITION_X_val + 50,
                                    y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 12.35))
    self.SIL_Entrypoint_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                  y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 12.35))

    self.SIGNAL_CREATION_Label.place(x=SENSOR_POSITION_X_val + 50,
                                     y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 13.35))
    self.SIGNAL_CREATION_spn.place(x=SENSOR_POSITION_X_val + 250,
                                   y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 13.35))
    self.STATISTIC_REPORT_PATH_label.place(x=SENSOR_POSITION_X_val + 50,
                                           y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 14.35))
    self.STATISTIC_REPORT_PATH_spn.place(x=SENSOR_POSITION_X_val + 250,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 14.35))
    self.Log_Tracing_Path_label.place(x=SENSOR_POSITION_X_val + 50,
                                      y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 15.75))
    self.Log_Tracing_Path_spn.place(x=SENSOR_POSITION_X_val + 250,
                                    y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 15.75))
    self.Log_Level_label.place(x=SENSOR_POSITION_X_val + 50,
                               y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 16.7))
    self.Log_Level_spn.place(x=SENSOR_POSITION_X_val + 250,
                             y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 16.7))

    browse_sil_logs()
    SIL_Data_save.Update_SIL_Tool_Config_backup_data(Tool_config)
    application_buttons(self, Global_var, Tool_config)
    Global_var.SIL_Tool_config_done = True
    return


def browse_sil_logs():
    """ BN Califr """
    global bn_califr_log_SIL_opt
    global bn_califr_log_SIL_label
    global bn_califr_SIL_button
    bn_califr_log_SIL_label = Label(text='BN CALIFR Logs: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    bn_califr_log_SIL_label.place(x=740, y=75)
    bn_califr_log_SIL_opt = Entry(root, font=('arial', 10, 'bold'), width=22, bg='white', fg='green')
    bn_califr_log_SIL_opt.place(x=940, y=75)

    def browse_bn_califr_SIL_logs():
        bn_califr_SIL_logs = filedialog.askopenfilenames(title='Select BN Califr Logs',
                                                         filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for a in bn_califr_SIL_logs:
            bn_califr_log_SIL_list.append(a)
        bn_califr_log_SIL_opt.insert(END, bn_califr_SIL_logs)

    bn_califr_SIL_button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                  bg=Bg_colour, bd=1, command=lambda: browse_bn_califr_SIL_logs())
    bn_califr_SIL_button.place(x=1110, y=70)

    """BN FASETH"""
    global faseth_log_SIL_opt
    global faseth_log_SIL_label
    global faseth_log_SIL_button
    faseth_log_SIL_label = Label(text='BN FASETH Logs: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    faseth_log_SIL_label.place(x=740, y=99)
    faseth_log_SIL_opt = Entry(root, font=('arial', 10, 'bold'), width=22, bg='white', fg='green')
    faseth_log_SIL_opt.place(x=940, y=99)

    def browse_faseth_SIL_logs():
        faseth_SIL_logs = filedialog.askopenfilenames(title='Select FASETH Logs',
                                                      filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))

        for b in faseth_SIL_logs:
            faseth_log_SIL_list.append(b)

        faseth_log_SIL_opt.insert(END, faseth_SIL_logs)

    faseth_log_SIL_button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                   bg=Bg_colour, bd=1, command=lambda: browse_faseth_SIL_logs()).place(x=1110, y=96)

    """SRR DEBUG"""
    global srr_debug_log_SIL_opt
    global srr_debug_log_SIL_label
    global srr_deb_SIL_button
    srr_debug_log_SIL_label = Label(text='SRR DEBUG Logs: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=740,
                                                                                                             y=128)
    srr_debug_log_SIL_opt = Entry(root, font=('arial', 10, 'bold'), width=22, bg='white', fg='green')
    srr_debug_log_SIL_opt.place(x=940, y=128)

    def browse_srr_debug_SIL_logs():
        srr_debug_SIL_logs = filedialog.askopenfilenames(title='Select SRR DEBUG Logs',
                                                         filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))

        for c in srr_debug_SIL_logs:
            srr_debug_log_SIL_list.append(c)

        srr_debug_log_SIL_opt.insert(END, srr_debug_SIL_logs)

    srr_deb_SIL_button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                bg=Bg_colour, bd=1, command=lambda: browse_srr_debug_SIL_logs()).place(x=1110, y=125)

    """SRR REFERENCE"""
    global srr_reference_log_SIL_label
    global srr_reference_log_SIL_opt
    global srr_ref_log_SIL_button
    srr_reference_log_SIL_label = Label(text='SRR REFERENCE Logs: ', bg=Bg_colour, font=('arial', 10, 'bold'))
    srr_reference_log_SIL_label.place(x=740, y=157)
    srr_reference_log_SIL_opt = Entry(root, font=('arial', 10, 'bold'), width=22, bg='white', fg='green')
    srr_reference_log_SIL_opt.place(x=940, y=157)

    def browse_srr_ref_SIL_logs():
        srr_ref_SIL_logs = filedialog.askopenfilenames(title='Select SRR REFERENCE Files',
                                                       filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        # srr_reference_log_list.clear()
        for r in srr_ref_SIL_logs:
            srr_reference_log_SIL_list.append(r)
        srr_reference_log_SIL_opt.insert(END, srr_ref_SIL_logs)

    srr_ref_log_SIL_button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                    bg=Bg_colour, bd=1, command=lambda: browse_srr_ref_SIL_logs()).place(x=1110, y=154)


def Get_Log_path(self, Tool_config, Log_Entry):
    Tool_config.log_path = Log_Entry.get()
    if (Tool_config.log_path is None) or (Tool_config.log_path == '') or ("Browse Log path" in Tool_config.log_path):
        # show an "Open" dialog box and return the path to the selected fileot
        Tool_config.log_path = askopenfilename()
        Log_Entry.delete(0, 'end')
        Log_Entry.insert(0, Tool_config.log_path)
        print("Log_path:", Tool_config.log_path)
    else:
        pass


def Get_json_path(self, Tool_config, Log_Entry):
    Tool_config.JSON_Flist_Path = Log_Entry.get()
    if (Tool_config.JSON_Flist_Path is None) or (Tool_config.JSON_Flist_Path == '') or (
            "Browse Log path" in Tool_config.JSON_Flist_Path):
        # show an "Open" dialog box and return the path to the selected fileot
        Tool_config.JSON_Flist_Path = askopenfilename()
        Log_Entry.delete(0, 'end')
        Log_Entry.insert(0, Tool_config.JSON_Flist_Path)
        print("File_Path", Tool_config.JSON_Flist_Path)
    else:
        pass


def Get_Folder_path(self, Tool_config, Log_Entry):
    Tool_config.output_Path_Location = Log_Entry.get()
    if (Tool_config.output_Path_Location is None) or (Tool_config.output_Path_Location == '') or (
            "Browse Folder path" in Tool_config.output_Path_Location):
        # show an "Open" dialog box and return the path to the selected fileot
        Tool_config.output_Path_Location = askdirectory()
        Log_Entry.delete(0, 'end')
        Log_Entry.insert(0, Tool_config.output_Path_Location)
        print("folder_path:", Tool_config.output_Path_Location)
    else:
        pass


def SIL_Tool_Configuration_options(SIL_widgets_obj, SIL_Global_var, SIL_Tool_config, X_AIX, Y_AIX):
    our_canvas.create_rectangle(40, 390, 330, 325, fill=None)
    global Tool_cfg
    global SIL_Enet_widgets
    global SIL_Enet_Var
    # global SIL_E_Var
    # print("SIL_Global_var.SIL_Tool_Radiobutton_done : ", SIL_Global_var.SIL_Tool_Radiobutton_done)
    # if not SIL_Global_var.SIL_Tool_Radiobutton_done:
    """Initialize data variables"""
    SIL_Tool_config.__init__()
    """-------------------------"""
    Radiobutton_var = IntVar()
    Tool_cfg.SIL_ENET_cfg = Radiobutton(root, text="UDP_FRAME_TRANSMISSION_CONFIG", font=('arial', 10, 'bold'),
                                        bg=Bg_colour, variable=Radiobutton_var, value=1, state=NORMAL,
                                        command=lambda: SIL_Ethernet_Configuration(SIL_Enet_widgets, SIL_Enet_Var,
                                                                                   SIL_Global_var,
                                                                                   SIL_widgets_obj,
                                                                                   X_AIX, Y_AIX))

    Tool_cfg.SIL_Tool_cfg = Radiobutton(root, text="SIL TOOL CONFIGURATION", font=('arial', 10, 'bold'),
                                        bg=Bg_colour, variable=Radiobutton_var, value=2, state=NORMAL,
                                        command=lambda: SIL_Tool_configuration(SIL_widgets_obj, SIL_Tool_config,
                                                                               SIL_Global_var,
                                                                               SIL_Tool_config, X_AIX, Y_AIX))

    Tool_cfg.SIL_Tool_cfg.place(x=50, y=150 + (SIL_Global_var.SIL_offset_y * 7.2))
    Tool_cfg.SIL_ENET_cfg.place(x=50, y=150 + (SIL_Global_var.SIL_offset_y * 8.2))
    SIL_Global_var.SIL_Tool_Radiobutton_done = True
    # progress_update(20)
    '''else:
        pass'''
    return


def SIL_Ethernet_Configuration(self, E_Var, Global_var, SIL_widgets_obj, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    our_canvas.create_rectangle(725, 320, 350, 79, fill=None)
    global our_canvas_SIL_UDP
    x_offset = 210
    chkbn_x_offset = 450
    # print("SIL_Ethernet_Configuration_evar:", Global_var.SIL_Ethernet_config_done)
    # print("SIL_Ethernet_Configuration:", Global_var.SIL_Tool_config_done)
    if Global_var.SIL_Tool_config_done:
        # If the SIL widgets are displayed already
        # We have to hide those widgets before display ENET widgets
        # print("SIL_tool", Global_var.SIL_Ethernet_config_done)
        SIL_widgets_obj.SIL_Clear_all_Tool_config()
        # forget_log()
        Global_var.SIL_Tool_config_done = False
        # our_canvas.pack_forget()
        # our_canvas.create_rectangle(725, 320, 350, 79, fill=None)
        # our_canvas.create_rectangle(1190, 490, 730, 69, fill=None)
    if not Global_var.SIL_Ethernet_config_done:
        our_canvas.pack_forget()
        our_canvas_SIL_UDP = Canvas(height=620, width=1200, bg=Bg_colour)
        our_canvas_SIL_UDP.pack()
        # creating rectangle
        # main()
        display_labels()
        Tool_info()
        our_canvas_SIL_UDP.create_rectangle(40, 115, 330, 49, fill=None)
        our_canvas_SIL_UDP.create_rectangle(40, 320, 330, 119, fill=None)
        our_canvas_SIL_UDP.create_rectangle(40, 390, 330, 325, fill=None)
        our_canvas_SIL_UDP.create_rectangle(725, 360, 355, 79, fill=None)
        SIL_Tool_Configuration_options(SIL_widgets_obj, SIL_Global_var, SIL_Tool_config, 370, 40)
        Check_SIL_customer_name(SIL_widgets_obj, 50, 50)
        Display_SIL_radar_pos_options(SIL_Sensor_and_ECU_obj, SIL_Sensor_pos_widgets_obj, SIL_sensor_ID, 50, 120)
        # application_buttons(self, Global_var, Data)
        try:
            forget_log()
        except:
            print()
        print("SIL_tool")
        self.SIL_Ethernet_config_label = Label(root, text="UDP_FRAME_TRANSMISSION_CONFIG", font=('arial', 14, 'bold'),
                                               bg=Bg_colour)
        """--------------------------------------------------------------------------------------------------"""
        self.DESTINATION_IP_label = Label(root, text="DESTINATION IP", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.DESTINATION_IP_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.DEST_INPUT_PORT_label = Label(root, text="INPUT PORT", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.DEST_INPUT_PORT_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.DEST_OUTPUT_PORT_label = Label(root, text="OUTPUT PORT", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.DEST_OUTPUT_PORT_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.OUTPUT_UDP_TRANSMISSION_label = Label(root, text="OUTPUT UDP TRANSMISSION", font=('arial', 10, 'bold'),
                                                   bg=Bg_colour)
        self.OUTPUT_UDP_TRANSMISSION_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.INPUT_UDP_TRANSMISSION_label = Label(root, text="INPUT UDP TRANSMISSION", font=('arial', 10, 'bold'),
                                                  bg=Bg_colour)
        self.INPUT_UDP_TRANSMISSION_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.TRANSMISSION_STATUS_label = Label(root, text="TRANSMISSION STATUS ", font=('arial', 10, 'bold'),
                                               bg=Bg_colour)
        self.TRANSMISSION_STATUS_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        '''self.LogLevel_label = Label(root, text="LOG_LEVEL ", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.LogLevel_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')'''

        '''self.LogTracingPath_label = Label(root, text="ENTER LOG TRACING PATH", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.LogTracingPath_Entry = Entry(root, font=('arial', 10, 'bold'), width=30, bg='white', fg='green')
        self.LogTracingPath_Button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1, bg=Bg_colour,
                                            bd=1,
                                            command=lambda: Get_Log_path(self.LogTracingPath_Button, E_Var,
                                                                         self.LogTracingPath_Entry))'''

        '''self.LogIncludeDateTime_label = Label(root, text="LOG INCLUDE DATE TIME", font=('arial', 10, 'bold'),
                                              bg=Bg_colour)
        self.LogIncludeDateTime_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')'''

        '''self.STATISTIC_REPORT_PATH_label = Label(root, text="ENTER STATISTICS PATH", font=('arial', 10, 'bold'),
                                                 bg=Bg_colour)
        self.STATISTIC_REPORT_PATH_Entry = Entry(root, font=('arial', 10, 'bold'), width=30, bg='white', fg='green')
        self.STATISTIC_REPORT_PATH_Button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1,
                                                   bg=Bg_colour,
                                                   bd=1,
                                                   command=lambda: Get_Log_path(self.STATISTIC_REPORT_PATH_Button,
                                                                                E_Var,
                                                                                self.STATISTIC_REPORT_PATH_Entry))'''
        """----------------------------placing widgets-----------------------------------------"""
        self.SIL_Ethernet_config_label.place(x=SENSOR_POSITION_X_val + 170,
                                             y=SENSOR_POSITION_Y_val)

        self.TRANSMISSION_STATUS_label.place(x=SENSOR_POSITION_X_val - 10,
                                             y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 2))
        self.TRANSMISSION_STATUS_Entry.place(x=SENSOR_POSITION_X_val + (x_offset - 10),
                                             y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 2))

        self.DESTINATION_IP_label.place(x=SENSOR_POSITION_X_val - 10,
                                        y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 3.5))
        self.DESTINATION_IP_Entry.place(x=SENSOR_POSITION_X_val + (x_offset - 10),
                                        y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 3.5))

        self.DEST_INPUT_PORT_label.place(x=SENSOR_POSITION_X_val - 10,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5))
        self.DEST_INPUT_PORT_Entry.place(x=SENSOR_POSITION_X_val + (x_offset - 10),
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 5))

        self.DEST_OUTPUT_PORT_label.place(x=SENSOR_POSITION_X_val - 10,
                                          y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.5))
        self.DEST_OUTPUT_PORT_Entry.place(x=SENSOR_POSITION_X_val + (x_offset - 10),
                                          y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 6.5))

        self.INPUT_UDP_TRANSMISSION_label.place(x=SENSOR_POSITION_X_val - 10,
                                                y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8))
        self.INPUT_UDP_TRANSMISSION_Entry.place(x=SENSOR_POSITION_X_val + (x_offset - 10),
                                                y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 8))

        self.OUTPUT_UDP_TRANSMISSION_label.place(x=SENSOR_POSITION_X_val - 10,
                                                 y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 9.5))
        self.OUTPUT_UDP_TRANSMISSION_Entry.place(x=SENSOR_POSITION_X_val + (x_offset - 10),
                                                 y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 9.5))

        '''self.STATISTIC_REPORT_PATH_label.place(x=SENSOR_POSITION_X_val,
                                               y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 11))
        self.STATISTIC_REPORT_PATH_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                               y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 11))
        self.STATISTIC_REPORT_PATH_Button.place(x=SENSOR_POSITION_X_val + 500,
                                                y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 11))'''

        '''self.LogLevel_label.place(x=SENSOR_POSITION_X_val,
                                  y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 12.5))
        self.LogLevel_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                  y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 12.5))'''

        '''self.LogTracingPath_label.place(x=SENSOR_POSITION_X_val,
                                        y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 14))
        self.LogTracingPath_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                        y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 14))
        self.LogTracingPath_Button.place(x=SENSOR_POSITION_X_val + 500,
                                         y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 14))'''

        '''self.LogIncludeDateTime_label.place(x=SENSOR_POSITION_X_val,
                                            y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 15.5))
        self.LogIncludeDateTime_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                            y=SENSOR_POSITION_Y_val + (SIL_Global_var.SIL_offset_y * 15.5))'''

        SIL_ENET_data_save.Update_ETHERNET_Config_backup_data(self)
        application_buttons(self, Global_var, E_Var)
        Global_var.SIL_Ethernet_config_done = True
        pass
    return


def application_buttons(self, Global_var, Data):
    global App_widgets
    # if not Global_var.application_buttons_init_done:
    App_widgets.Start_btn = Button(root, text="START_RESIM", font=('arial', 12, 'bold'), height=1, bg=Bg_colour,
                                   command=lambda: SIL_START(self, Global_var, Data))

    App_widgets.Save_btn = Button(root, text="XML_CONFIG", font=('arial', 12, 'bold'), height=1, bg=Bg_colour,
                                  command=lambda: SIL_SAVE(self, Global_var, Data))

    App_widgets.Clear_btn = Button(root, text="CLEAR", font=('arial', 12, 'bold'), height=1, bg=Bg_colour,
                                   command=lambda: SIL_CLEAR(self, Global_var, Data))

    App_widgets.Json_btn = Button(root, text="JSON/FLIST", font=('arial', 12, 'bold'), height=1, bg=Bg_colour,
                                  command=lambda: SIL_JSON_FILE())
    App_widgets.Start_btn.place(x=1065, y=50 + (SIL_Global_var.SIL_offset_y * 18))
    App_widgets.Json_btn.place(x=950, y=50 + (SIL_Global_var.SIL_offset_y * 18))
    App_widgets.Save_btn.place(x=825, y=50 + (SIL_Global_var.SIL_offset_y * 18))
    App_widgets.Clear_btn.place(x=750, y=50 + (SIL_Global_var.SIL_offset_y * 18))

    Global_var.application_buttons_init_done = True

    Global_var.SIL_Tool_data_save = False
    Global_var.SIL_Ethernet_data_save = False
    return


def SIL_START(self, Global_var, Data):
    # Execute_SIL_Command(SIL_Data_save.JSON_Flist_Path, SIL_Data_save.output_Path_Location)
    # progress_update(100)
    if len(srr_debug_log_SIL_list) == 0:
        messagebox.showerror(title='Logs', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')
    elif len(bn_califr_log_SIL_list or faseth_log_SIL_list or srr_debug_log_SIL_list or srr_ref_log_SIL_button) == 0:
        messagebox.showerror(title='Logs', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')
    else:
        '''Get_data = multiprocessing.Process(target=Execute_SIL_Command,
                                           args=(SIL_Data_save.output_Path_Location,
                                                 SIL_Data_save.output_Path_options,
                                                 bn_califr_log_SIL_list,
                                                 faseth_log_SIL_list,
                                                 srr_debug_log_SIL_list, srr_reference_log_SIL_list))
        Get_data.start()'''
        Execute_SIL_Command(SIL_Data_save.output_Path_Location,
                            SIL_Data_save.output_Path_options,
                            bn_califr_log_SIL_list,
                            faseth_log_SIL_list,
                            srr_debug_log_SIL_list, srr_reference_log_SIL_list)
    return


def SIL_JSON_FILE():
    if len(srr_debug_log_SIL_list) == 0:
        messagebox.showerror(title='Logs', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')
    elif len(bn_califr_log_SIL_list) == len(faseth_log_SIL_list) == len(srr_debug_log_SIL_list) == len(
             srr_reference_log_SIL_list):
        # oldfiledel()
        if os.path.exists("SIL_json.json"):
            os.remove("SIL_json.json")
        SIL_json_writer(bn_califr_log_SIL_list, faseth_log_SIL_list, srr_debug_log_SIL_list, srr_reference_log_SIL_list)
        messagebox.showinfo(title='Logs', message='! SUCCESSFULLY GENERATED THE JSON FILE !')
        # json_path()
    elif len(bn_califr_log_SIL_list and faseth_log_SIL_list and srr_reference_log_SIL_list) == 0 and len(
            srr_debug_log_SIL_list) >= 1:
        if os.path.exists("SIL_flist.txt"):
            os.remove("SIL_flist.txt")
        SIL_flist_gen(srr_debug_log_SIL_list)
        messagebox.showinfo(title='Logs', message='! SUCCESSFULLY GENERATED THE FLIST FILE !')
        # flist_path()
    else:
        messagebox.showerror(title='Logs', message='!!! PLEASE BROWSE ALL THE LOG FILES !!!')


def SIL_SAVE(self, Global_var, Data):
    # print("SIL_SAVE write XML")
    # progress_update(50)
    if Global_var.SIL_Tool_config_done:
        """save data"""
        # print("SIL SAVE DATA ==========================")
        # print("SIL_Tool_config.path=", SIL_Tool_config.JSON_Flist_Path)
        SIL_Data_save.save_data(SIL_Tool_config, SIL_widgets_obj)
        # SIL_data_print_test(SIL_Data_save)
        Global_var.SIL_Tool_data_save = True
    elif Global_var.SIL_Ethernet_config_done:
        SIL_ENET_data_save.save_data(SIL_Enet_widgets)
        # SIL_ENET_data_print_test(SIL_ENET_data_save)
        Global_var.SIL_Ethernet_data_save = True
        pass
    else:
        pass
    if SIL_Data_save.output_Path_options == 'None':
        messagebox.showerror(title='XML File Creation', message='!Please Do Select Output Path!')
    else:
        Write_SIL_TOOL_XML(SIL_Global_var, SIL_Data_save, SIL_ENET_data_save, SIL_Sensor_and_ECU_obj)
        messagebox.showinfo(title='XML File Creation', message='!SUCCESSFULLY GENERATED XML FILE!')
    # print("JSON_/_Flist_Path", SIL_Data_save.JSON_Flist_Path)
    return


def SIL_CLEAR(self, Global_var, Data):
    print("SIL_CLEAR write XML")
    print("SIL_CLEAR", Global_var.SIL_Tool_config_done)
    print("SIL_CLEAR_enet", Global_var.SIL_Tool_config_done)
    if Global_var.SIL_Tool_config_done:
        # print("SIL_CLEAR_sil", Global_var.SIL_Tool_config_done)
        SIL_Data_save.__init__()
        SIL_Data_save.clear_check_buttons(SIL_widgets_obj)
        SIL_Data_save.Update_SIL_Tool_Config_backup_data(Data)
        SIL_data_print_test(SIL_Data_save)
        # clear_log_entries()
        # forget_log()
    elif Global_var.SIL_Ethernet_config_done:
        # print("SIL_CLEAR_enet", Global_var.SIL_Tool_config_done)
        SIL_ENET_data_save.__init__()
        SIL_ENET_data_save.Update_ETHERNET_Config_backup_data(SIL_Enet_widgets)
        # clear_log_entries()
        # forget_log()
    else:
        pass
    # progress_update(20)
    return


'''def Update_process():
    print("Update process")
    root.update()
    root.after(1000)
    return'''


def Clear_all_widgets():
    # This function will help to clear all the widgets used in the project
    # init function calls will help restore the default value.
    SIL_Data_save.__init__()
    SIL_Data_save.clear_check_buttons(SIL_widgets_obj)
    SIL_ENET_data_save.__init__()
    SIL_widgets_obj.Clear_all()
    SIL_widgets_obj.SIL_Clear_all_Tool_config()
    SIL_Enet_widgets.Clear_all_ENET_config()
    SIL_widgets_obj.__init__()
    SIL_Sensor_pos_widgets_obj.Clear_all_SIL_Sensor_pos_widgets()
    Tool_cfg.Clear_Tool_config_options()
    App_widgets.Clear_Application_widgets()
    forget_log()
    delete_list_logs()
    clear_log_entries()
    return


def SIL_data_print_test(SIL_Tool_config):
    # print("Radar_fusion_EN ", SIL_Tool_config.out)
    # print("Debug_Mode ", SIL_Tool_config.Debug_Mode)
    # print("Execution_Mode ", SIL_Tool_config.Execution_Mode)
    # print("Auto_close_win ", SIL_Tool_config.Auto_close_win)
    # print("Continues_run_mod ", SIL_Tool_config.Continues_run_mod)
    # print("input_option ", SIL_Tool_config.input_option)
    # print("Log_repeate_cnt ", SIL_Tool_config.Log_repeate_cnt)
    # print("Vehicle_data_source ", SIL_Tool_config.Vehicle_data_source)
    # print("Fusion_det_source ", SIL_Tool_config.Fusion_det_source)
    # print("Input_data_type ", SIL_Tool_config.Input_data_type)
    # print("Sensor_run_mode ", SIL_Tool_config.Sensor_run_mode)
    # print("TOBJECT_injection_type ", SIL_Tool_config.TOBJECT_injection_type)
    return
