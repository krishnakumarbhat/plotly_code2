"""Import library"""
from tkinter import *
from tkinter.ttk import *
import tkinter as tk
from tkinter.filedialog import *
from tkinter import messagebox
from main_interface import *
import MUDP_Tool_spec_widgets as M_Tool
from HIL_Backup_data import *


""" ----------Class Declaration---------- """


class HIL_Global_var_class(object):
    def __init__(self):
        self.Customer_Name = None
        self.HIL_offset_X = M_Tool.offset_x
        self.HIL_offset_y = M_Tool.offset_y
        self.HIL_Tool_config_done = False
        self.Default_Var = None
        self.Backup_Radar_pos = False
        self.HIL_Tool_Radiobutton_done = False
        self.Ethernet_config_done = False
        self.HIL_Tool_data_save = False
        self.Ethernet_data_save = False
        self.application_buttons_init_done = False


class HIL_Sensor_and_ECU(object):
    def __init__(self):
        self.REAR_LEFT = None
        self.REAR_RIGHT = None
        self.FRONT_LEFT = None
        self.FRONT_RIGHT = None
        self.BP_LEFT = None
        self.BP_RIGHT = None
        self.RADAR_ECU = None


class HIL_Tool_Config_variables(object):
    def __init__(self):
        self.input_option = StringVar()
        self.Continues_run_mod = IntVar()
        self.Log_repeate_cnt = StringVar()
        self.Vehicle_data_source = StringVar()
        self.Fusion_det_source = StringVar()
        self.Auto_close_win = IntVar()
        self.Input_data_type = StringVar()
        self.Sensor_run_mode = StringVar()
        self.Radar_fusion_EN = IntVar()
        self.Execution_Mode = StringVar()
        self.Debug_Mode = IntVar()
        self.TOBJECT_injection_type = StringVar()
        self.INJECT_FAULT_SENSOR_RL = IntVar()
        self.INJECT_FAULT_SENSOR_RR = IntVar()


class HIL_widgets(object):
    def __init__(self):
        self.HIL_Tool_config_label = None
        self.SENSOR_POSITION_label = None
        self.Customer_spn = None
        self.Customer_label_spn = None
        self.Input_Type_spn = None
        self.Input_Type_label = None
        self.CONTINUOUS_RUN_MODE_chkbtn = None
        self.LOG_REPEAT_COUNT_spn = None
        self.VEHICLE_DATA_SOURCE_spn = None
        self.FUSION_DETECTION_SOURCE_spn = None
        self.AUTOCLOSE_COMMAND_WIN_chkbtn = None
        self.INPUT_DATA_TYPE_spn = None
        self.SENSOR_RUN_MODE_spn = None
        self.RADAR_FUSION_ENABLED_chkbtn = None
        self.RESIM_EXECUTION_MODE_spn = None
        self.DEBUGGING_HIL_chkbtn = None
        self.LOG_REPEAT_COUNT_label = None
        self.VEHICLE_DATA_SOURCE_label = None
        self.FUSION_DETECTION_SOURCE_label = None
        self.RESIM_EXECUTION_MODE_label = None
        self.TOBJECT_injection_type_label = None
        self.TOBJECT_injection_type_spn = None
        self.INJECT_FAULT_SENSOR_RL_chkbn = None
        self.INJECT_FAULT_SENSOR_RR_chkbn = None
        self.INPUT_DATA_TYPE_label = None
        self.SENSOR_RUN_MODE_label = None
        self.progress_bar = None
        self.progress_bar_label = None

    def Clear_all_Tool_config(self):
        self.HIL_Tool_config_label.place_forget()
        self.Input_Type_spn.place_forget()
        self.Input_Type_label.place_forget()
        self.CONTINUOUS_RUN_MODE_chkbtn.place_forget()
        self.LOG_REPEAT_COUNT_spn.place_forget()
        self.VEHICLE_DATA_SOURCE_spn.place_forget()
        self.FUSION_DETECTION_SOURCE_spn.place_forget()
        self.AUTOCLOSE_COMMAND_WIN_chkbtn.place_forget()
        self.INPUT_DATA_TYPE_spn.place_forget()
        self.SENSOR_RUN_MODE_spn.place_forget()
        self.RADAR_FUSION_ENABLED_chkbtn.place_forget()
        self.RESIM_EXECUTION_MODE_spn.place_forget()
        self.DEBUGGING_HIL_chkbtn.place_forget()
        self.LOG_REPEAT_COUNT_label.place_forget()
        self.VEHICLE_DATA_SOURCE_label.place_forget()
        self.FUSION_DETECTION_SOURCE_label.place_forget()
        self.RESIM_EXECUTION_MODE_label.place_forget()
        self.TOBJECT_injection_type_label.place_forget()
        self.TOBJECT_injection_type_spn.place_forget()
        self.INJECT_FAULT_SENSOR_RL_chkbn.place_forget()
        self.INJECT_FAULT_SENSOR_RR_chkbn.place_forget()
        self.INPUT_DATA_TYPE_label.place_forget()
        self.SENSOR_RUN_MODE_label.place_forget()

    def Clear_all(self):
        self.Customer_spn.place_forget()
        self.Customer_label_spn.place_forget()
        self.SENSOR_POSITION_label.place_forget()
        self.Clear_all_Tool_config()
        self.progress_bar.place_forget()
        self.progress_bar_label.place_forget()


class HIL_Sensor_pos_widgets(object):
    def __init__(self):
        self.REAR_LEFT_chkbtn = None
        self.REAR_RIGHT_chkbtn = None
        self.FRONT_LEFT_chkbtn = None
        self.FRONT_RIGHT_chkbtn = None
        self.BP_LEFT_chkbtn = None
        self.BP_RIGHT_chkbtn = None
        self.RADAR_ECU_chkbtn = None

    def Clear_all_HIL_Sensor_pos_widgets(self):
        self.REAR_LEFT_chkbtn.place_forget()
        self.REAR_RIGHT_chkbtn.place_forget()
        self.FRONT_LEFT_chkbtn.place_forget()
        self.FRONT_RIGHT_chkbtn.place_forget()
        self.BP_LEFT_chkbtn.place_forget()
        self.BP_RIGHT_chkbtn.place_forget()
        self.RADAR_ECU_chkbtn.place_forget()


class HIL_Sensor_and_ECU_position(object):
    def __init__(self):
        self.REAR_LEFT = 0
        self.REAR_RIGHT = 1
        self.FRONT_LEFT = 2
        self.FRONT_RIGHT = 3
        self.BP_LEFT = 4
        self.BP_RIGHT = 5
        self.RADAR_ECU = 6


class Tool_config_options(object):
    def __init__(self):
        self.HIL_Tool_cfg = None
        self.ENET_cfg = None

    def Clear_Tool_config_options(self):
        self.HIL_Tool_cfg.place_forget()
        self.ENET_cfg.place_forget()


class Ethernet_config_variables(object):
    def __init__(self):
        self.HIL_ENGINE_IP = "192.168.1.41"
        self.SCALEXIO_IP = "192.168.1.90"
        self.SCALEXIO_PORT = 5556
        self.UDP_PORT_DATA = 6001
        self.UDP_PORT_XCP = 4410
        self.UDP_PORT_LOGGING = 5555
        self.INTERFACE_PORT = 5544
        self.SET_FAULT_FROM = 0
        self.SET_FAULT_TILL = 0
        self.KPI_Security_Key = None
        self.Log_path = None


class Ethernet_config_widgets(object):
    def __init__(self):
        self.HIL_ENGINE_IP_Entry = None
        self.SCALEXIO_IP_Entry = None
        self.SCALEXIO_PORT_Entry = None
        self.UDP_PORT_DATA_Entry = None
        self.UDP_PORT_XCP_Entry = None
        self.UDP_PORT_LOGGING_Entry = None
        self.INTERFACE_PORT_Entry = None
        self.SET_FAULT_FROM_Entry = None
        self.SET_FAULT_TILL_Entry = None
        self.HIL_ENGINE_IP_label = None
        self.SCALEXIO_IP_label = None
        self.SCALEXIO_PORT_label = None
        self.UDP_PORT_DATA_label = None
        self.UDP_PORT_XCP_label = None
        self.UDP_PORT_LOGGING_label = None
        self.INTERFACE_PORT_label = None
        self.SET_FAULT_FROM_label = None
        self.SET_FAULT_TILL_label = None
        self.Ethernet_config_label = None
        self.KPI_KEY_Entry_label = None
        self.KPI_KEY_Entry = None
        self.Log_path_label = None
        self.Log_path_Entry = None
        self.Log_path_Button = None

    def Clear_all_ENET_config(self):
        self.HIL_ENGINE_IP_Entry.place_forget()
        self.SCALEXIO_IP_Entry.place_forget()
        self.SCALEXIO_PORT_Entry.place_forget()
        self.UDP_PORT_DATA_Entry.place_forget()
        self.UDP_PORT_XCP_Entry.place_forget()
        self.UDP_PORT_LOGGING_Entry.place_forget()
        self.INTERFACE_PORT_Entry.place_forget()
        self.SET_FAULT_FROM_Entry.place_forget()
        self.SET_FAULT_TILL_Entry.place_forget()
        self.HIL_ENGINE_IP_label.place_forget()
        self.SCALEXIO_IP_label.place_forget()
        self.SCALEXIO_PORT_label.place_forget()
        self.UDP_PORT_DATA_label.place_forget()
        self.UDP_PORT_XCP_label.place_forget()
        self.UDP_PORT_LOGGING_label.place_forget()
        self.INTERFACE_PORT_label.place_forget()
        self.SET_FAULT_FROM_label.place_forget()
        self.SET_FAULT_TILL_label.place_forget()
        self.Ethernet_config_label.place_forget()
        self.KPI_KEY_Entry.place_forget()
        self.KPI_KEY_Entry_label.place_forget()
        self.Log_path_label.place_forget()
        self.Log_path_Entry.place_forget()
        self.Log_path_Button.place_forget()
        return


class Application_widgets(object):
    def __init__(self):
        self.Start_btn = None
        self.Save_btn = None
        self.Clear_btn = None

    def Clear_Application_widgets(self):
        self.Start_btn.place_forget()
        self.Save_btn.place_forget()
        self.Clear_btn.place_forget()
        return


"""==========================================="""

""" ------------Global variables & Class objects------------- """
HIL_Global_var = HIL_Global_var_class()
HIL_widgets_obj = HIL_widgets()
HIL_Sensor_and_ECU_obj = HIL_Sensor_and_ECU()
HIL_Sensor_pos_widgets_obj = HIL_Sensor_pos_widgets()
HIL_sensor_ID = HIL_Sensor_and_ECU_position()
HIl_Tool_config = HIL_Tool_Config_variables()
Tool_cfg = Tool_config_options()
Enet_widgets = Ethernet_config_widgets()
Enet_Var = Ethernet_config_variables()
App_widgets = Application_widgets()

""" Cross dependency inclusion """
from HIL_XML_writer import *
"""============================"""
global HIL_sensor
HIL_sensor = [0, 0, 0, 0, 0, 0, 0]  # This global variable used to back up the sensor selection data
global HIL_sensor_active
HIL_sensor_active = False

"""==========================================="""


def Check_customer_name(self, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    global HIL_Global_var
    # Creating Customer_label object to Customer_label_spn
    self.Customer_label_spn = Label(root, text="SELECT CUSTOMER", font=('arial', 12, 'bold'), bg=Bg_colour)
    self.Customer_label_spn.place(x=SENSOR_POSITION_X_val, y=SENSOR_POSITION_Y_val)

    # Customer_list will hold a number of options which is available to choose
    Customer_list = (
        'SELECT CUSTOMER', 'BMW_LOW', 'BMW_SAT', 'BMW_BPIL', 'RNA_SUV', 'RNA_CDV', 'BMW_HIGH', 'GEELY_SRR5',
        'HKMC_SRR5')

    # Below statement will help to store/change the data when spinbox status change
    HIL_Global_var.Default_Var = StringVar()
    self.Customer_spn = Spinbox(root, textvariable=HIL_Global_var.Default_Var, font=('arial', 10, 'bold'), bg='white',
                                values=Customer_list, wrap=True,
                                command=lambda: Update_customer_name(self.Customer_spn))
    HIL_Global_var.Default_Var.set('SELECT CUSTOMER')
    self.Customer_spn.place(x=SENSOR_POSITION_X_val, y=(SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 1.5)))

    self.progress_bar = Progressbar(root, style="green.Horizontal.TProgressbar", orient=HORIZONTAL,
                                    length=275, mode='determinate')

    self.progress_bar.place(x=SENSOR_POSITION_X_val + 800,
                            y=(SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 19)))

    self.progress_bar_label = Label(root, text="PROGRESS ", font=('arial', 10, 'bold'), bg=Bg_colour)
    self.progress_bar_label.place(x=SENSOR_POSITION_X_val + 700,
                                  y=(SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 18.8)))
    return


def Update_HIL_Tool_offset():
    global HIL_Global_var
    # Global offsets setting based on the OS used
    HIL_Global_var.HIL_offset_X = M_Tool.offset_x
    HIL_Global_var.HIL_offset_y = M_Tool.offset_y
    return


def Update_customer_name(obj):
    global HIL_Global_var
    global HIL_Sensor_and_ECU_obj
    HIL_Global_var.Customer_Name = obj.get()
    print('HIL_Global_var.Customer_Name', HIL_Global_var.Customer_Name)
    Display_radar_pos_options(HIL_Sensor_and_ECU_obj, HIL_Sensor_pos_widgets_obj, HIL_sensor_ID, 50, 150)
    progress_update(10)
    return


def Display_radar_pos_options(self, Sensor_pos, HIL_sen_ID, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    global HIL_Global_var
    global HIL_widgets_obj
    HIL_widgets_obj.SENSOR_POSITION_label = Label(root, text="SENSOR POSITION", font=('arial', 12, 'bold'),
                                                  bg=Bg_colour)
    HIL_widgets_obj.SENSOR_POSITION_label.place(x=SENSOR_POSITION_X_val, y=SENSOR_POSITION_Y_val)

    self.REAR_LEFT = IntVar()
    Sensor_pos.REAR_LEFT_chkbtn = Checkbutton(root, text="REAR LEFT", variable=self.REAR_LEFT,
                                              font=('arial', 10, 'bold'),
                                              bg=Bg_colour,
                                              onvalue=1, offvalue=0,
                                              command=lambda: HIL_set_sensor(self, HIL_sen_ID.REAR_LEFT,
                                                                             self.REAR_LEFT))
    self.REAR_RIGHT = IntVar()
    Sensor_pos.REAR_RIGHT_chkbtn = Checkbutton(root, text="REAR_RIGHT", variable=self.REAR_RIGHT,
                                               font=('arial', 10, 'bold'),
                                               bg=Bg_colour, onvalue=1, offvalue=0,
                                               command=lambda: HIL_set_sensor(self, HIL_sen_ID.REAR_RIGHT,
                                                                              self.REAR_RIGHT))
    self.FRONT_LEFT = IntVar()
    Sensor_pos.FRONT_LEFT_chkbtn = Checkbutton(root, text="FRONT LEFT", variable=self.FRONT_LEFT,
                                               font=('arial', 10, 'bold'),
                                               bg=Bg_colour, onvalue=1, offvalue=0,
                                               command=lambda: HIL_set_sensor(self, HIL_sen_ID.FRONT_LEFT,
                                                                              self.FRONT_LEFT))
    self.FRONT_RIGHT = IntVar()
    Sensor_pos.FRONT_RIGHT_chkbtn = Checkbutton(root, text="FRONT RIGHT", variable=self.FRONT_RIGHT,
                                                font=('arial', 10, 'bold'),
                                                bg=Bg_colour, onvalue=1, offvalue=0,
                                                command=lambda: HIL_set_sensor(self, HIL_sen_ID.FRONT_RIGHT,
                                                                               self.FRONT_RIGHT))
    self.BP_LEFT = IntVar()
    Sensor_pos.BP_LEFT_chkbtn = Checkbutton(root, text="BP LEFT", variable=self.BP_LEFT, font=('arial', 10, 'bold'),
                                            bg=Bg_colour,
                                            onvalue=1, offvalue=0,
                                            command=lambda: HIL_set_sensor(self, HIL_sen_ID.BP_LEFT, self.BP_LEFT))
    self.BP_RIGHT = IntVar()
    Sensor_pos.BP_RIGHT_chkbtn = Checkbutton(root, text="BP RIGHT", variable=self.BP_RIGHT, font=('arial', 10, 'bold'),
                                             bg=Bg_colour,
                                             onvalue=1, offvalue=0,
                                             command=lambda: HIL_set_sensor(self, HIL_sen_ID.BP_RIGHT, self.BP_RIGHT))
    self.RADAR_ECU = IntVar()
    Sensor_pos.RADAR_ECU_chkbtn = Checkbutton(root, text="RADAR ECU", variable=self.RADAR_ECU,
                                              font=('arial', 10, 'bold'),
                                              bg=Bg_colour,
                                              onvalue=1, offvalue=0,
                                              command=lambda: HIL_set_sensor(self, HIL_sen_ID.RADAR_ECU,
                                                                             self.RADAR_ECU))

    Sensor_pos.REAR_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val,
                                      y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 2))
    Sensor_pos.REAR_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val,
                                       y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 3))
    Sensor_pos.FRONT_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val,
                                       y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 4))
    Sensor_pos.FRONT_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 5))
    Sensor_pos.BP_LEFT_chkbtn.place(x=SENSOR_POSITION_X_val,
                                    y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 6))
    Sensor_pos.BP_RIGHT_chkbtn.place(x=SENSOR_POSITION_X_val,
                                     y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 7))
    Sensor_pos.RADAR_ECU_chkbtn.place(x=SENSOR_POSITION_X_val,
                                      y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 8))

    if HIL_Global_var.Backup_Radar_pos == True:
        Data_backup_Display_radar_pos_options(HIL_sen_ID, Sensor_pos)
    HIL_Global_var.Backup_Radar_pos = True
    return


def Data_backup_Display_radar_pos_options(self, Sensor_pos):
    global HIL_sensor
    if HIL_sensor[self.REAR_LEFT]:
        Sensor_pos.REAR_LEFT_chkbtn.select()
    if HIL_sensor[self.REAR_RIGHT]:
        Sensor_pos.REAR_RIGHT_chkbtn.select()
    if HIL_sensor[self.FRONT_LEFT]:
        Sensor_pos.FRONT_LEFT_chkbtn.select()
    if HIL_sensor[self.FRONT_RIGHT]:
        Sensor_pos.FRONT_RIGHT_chkbtn.select()
    if HIL_sensor[self.BP_LEFT]:
        Sensor_pos.BP_LEFT_chkbtn.select()
    if HIL_sensor[self.BP_RIGHT]:
        Sensor_pos.BP_RIGHT_chkbtn.select()
    if HIL_sensor[self.RADAR_ECU]:
        Sensor_pos.RADAR_ECU_chkbtn.select()
    return


def HIL_set_sensor(self, Index, Val):
    global HIL_sensor
    global HIL_Global_var
    global HIL_widgets_obj
    global HIl_Tool_config
    HIL_sum = 0
    if Val != None:
        HIL_sensor[Index] = Val.get()
    else:
        print('fail', Val)
    for Ind in range(len(HIL_sensor)):
        HIL_sum += HIL_sensor[Ind]  # checking for at-least 1 sensor is selected or not
    if HIL_sum != 0:
        #   If any one of sensor selected calling the HIL config
        #   This function will display all the HIL Tool widgets
        HIL_Tool_Configuration_options(HIL_widgets_obj, HIL_Global_var, HIl_Tool_config, 370, 50)
    # Activ_sensor_ECU(stream_options_chkbtn_obj,310,60)
    else:
        pass

    return


def HIL_Tool_configuration(self, Global_var, Tool_config, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    global HIl_Tool_config
    global HIL_widgets_obj
    global Enet_widgets

    if Global_var.Ethernet_config_done == True:
        # If the Ethernet widgets are displayed already
        # We have to hide those widgets before calling HIL Tool config display
        Enet_widgets.Clear_all_ENET_config()
        Global_var.Ethernet_config_done = False
        pass
    if Global_var.HIL_Tool_config_done == False:
        # print("HIL Tool Config")
        """ --------------  check buttons  ---------------- """
        HIL_widgets_obj.HIL_Tool_config_label = Label(root, text="HIL TOOL CONFIGURATION", font=('arial', 14, 'bold'),
                                                      bg=Bg_colour)

        self.CONTINUOUS_RUN_MODE_chkbtn = Checkbutton(root, text="CONTINUOUS RUN MODE",
                                                      variable=Tool_config.Continues_run_mod,
                                                      font=('arial', 10, 'bold'),
                                                      bg=Bg_colour, onvalue=1, offvalue=0)

        self.AUTOCLOSE_COMMAND_WIN_chkbtn = Checkbutton(root, text="AUTOCLOSE COMMAND WIN",
                                                        variable=Tool_config.Auto_close_win, font=('arial', 10, 'bold'),
                                                        bg=Bg_colour, onvalue=1, offvalue=0)

        self.RADAR_FUSION_ENABLED_chkbtn = Checkbutton(root, text="RADAR FUSION ENABLE",
                                                       variable=Tool_config.Radar_fusion_EN, font=('arial', 10, 'bold'),
                                                       bg=Bg_colour, onvalue=1, offvalue=0)

        self.DEBUGGING_HIL_chkbtn = Checkbutton(root, text="DEBUGGING ENABLE",
                                                variable=Tool_config.Debug_Mode, font=('arial', 10, 'bold'),
                                                bg=Bg_colour, onvalue=1, offvalue=0)

        self.INJECT_FAULT_SENSOR_RL_chkbn = Checkbutton(root, text="INJECT FAULT SENSOR RL",
                                                        variable=Tool_config.INJECT_FAULT_SENSOR_RL,
                                                        font=('arial', 10, 'bold'),
                                                        bg=Bg_colour, onvalue=1, offvalue=0)

        self.INJECT_FAULT_SENSOR_RR_chkbn = Checkbutton(root, text="INJECT FAULT SENSOR RR",
                                                        variable=Tool_config.INJECT_FAULT_SENSOR_RR,
                                                        font=('arial', 10, 'bold'),
                                                        bg=Bg_colour, onvalue=1, offvalue=0)

        """--------------------------------------------------------------------------------------------------"""

        HIL_widgets_obj.HIL_Tool_config_label.place(x=SENSOR_POSITION_X_val + 170,
                                                    y=SENSOR_POSITION_Y_val)
        self.CONTINUOUS_RUN_MODE_chkbtn.place(x=SENSOR_POSITION_X_val,
                                              y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 3))
        self.AUTOCLOSE_COMMAND_WIN_chkbtn.place(x=SENSOR_POSITION_X_val,
                                                y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 4.5))

        self.RADAR_FUSION_ENABLED_chkbtn.place(x=SENSOR_POSITION_X_val,
                                               y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 6))
        self.DEBUGGING_HIL_chkbtn.place(x=SENSOR_POSITION_X_val,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 7.5))

        self.INJECT_FAULT_SENSOR_RL_chkbn.place(x=SENSOR_POSITION_X_val,
                                                y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 9))

        self.INJECT_FAULT_SENSOR_RR_chkbn.place(x=SENSOR_POSITION_X_val,
                                                y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 10.5))

        HIL_Tool_configuration_spinBox(self, Global_var, Tool_config, SENSOR_POSITION_X_val + 320,
                                       SENSOR_POSITION_Y_val)


    else:
        pass

    return


def HIL_Tool_configuration_spinBox(self, Global_var, Tool_config, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    global HIL_Global_var
    x_offset = 250

    Input_List = ('MDF4_INPUT', 'SINGLE_DVSU_INPUT', 'FOUR_DVSU_INPUT', 'SIX_DVSU_INPUT')
    self.Input_Type_label = Label(root, text="INPUT LOG TYPE", font=('arial', 10, 'bold'), bg=Bg_colour)
    print("Tool_config.input_option", Tool_config.input_option.get())
    self.Input_Type_spn = Spinbox(root, textvariable=Tool_config.input_option, font=('arial', 10, 'bold'), bg='white',
                                  values=Input_List, wrap=True)
    print("AF Tool_config.input_option", Tool_config.input_option.get())
    """--------------------------------------------------------------------------------------------------"""
    self.LOG_REPEAT_COUNT_label = Label(root, text="LOG REPEAT COUNT", font=('arial', 10, 'bold'),
                                        bg=Bg_colour)

    self.LOG_REPEAT_COUNT_spn = Spinbox(root, textvariable=Tool_config.Log_repeate_cnt, font=('arial', 10, 'bold'),
                                        wrap=True, bg='white', from_=0, to=50)

    """--------------------------------------------------------------------------------------------------"""
    data_source = ('UDP', 'CAN')
    self.VEHICLE_DATA_SOURCE_label = Label(root, text="VEHICLE DATA SOURCE", font=('arial', 10, 'bold'),
                                           bg=Bg_colour)

    self.VEHICLE_DATA_SOURCE_spn = Spinbox(root, textvariable=Tool_config.Vehicle_data_source,
                                           font=('arial', 10, 'bold'), values=data_source, wrap=True,
                                           bg='white')
    """--------------------------------------------------------------------------------------------------"""

    self.FUSION_DETECTION_SOURCE_label = Label(root, text="FUSION DETECTION SOURCE", font=('arial', 10, 'bold'),
                                               bg=Bg_colour)

    self.FUSION_DETECTION_SOURCE_spn = Spinbox(root, textvariable=Tool_config.Fusion_det_source,
                                               font=('arial', 10, 'bold'), values=data_source, wrap=True,
                                               bg='white')
    """--------------------------------------------------------------------------------------------------"""

    self.RESIM_EXECUTION_MODE_label = Label(root, text="RESIM EXECUTION MODE", font=('arial', 10, 'bold'),
                                            bg=Bg_colour)
    RESIM_MODE = ('NORMAL', 'IDLE')
    self.RESIM_EXECUTION_MODE_spn = Spinbox(root, textvariable=Tool_config.Execution_Mode, font=('arial', 10, 'bold'),
                                            values=RESIM_MODE, wrap=True, bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.TOBJECT_injection_type_label = Label(root, text="TOBJECT INJECTION TYPE", font=('arial', 10, 'bold'),
                                              bg=Bg_colour)
    Injection_Type = ('DSPACE', 'INTERNAL')
    self.TOBJECT_injection_type_spn = Spinbox(root, textvariable=Tool_config.TOBJECT_injection_type,
                                              font=('arial', 10, 'bold'), wrap=True, values=Injection_Type,
                                              bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.INPUT_DATA_TYPE_label = Label(root, text="INPUT DATA TYPE", font=('arial', 10, 'bold'),
                                       bg=Bg_colour)

    INPUT_DATA_TYPE = ('LOG_SIMULATION', 'LIVE_SIMULATION')
    self.INPUT_DATA_TYPE_spn = Spinbox(root, textvariable=Tool_config.Input_data_type,
                                       values=INPUT_DATA_TYPE, font=('arial', 10, 'bold'), wrap=True,
                                       bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.SENSOR_RUN_MODE_label = Label(root, text="SENSOR RUN MODE", font=('arial', 10, 'bold'),
                                       bg=Bg_colour)

    SENSOR_MODE = ('DETECTION', 'TRACKER')
    self.SENSOR_RUN_MODE_spn = Spinbox(root, textvariable=Tool_config.Sensor_run_mode,
                                       values=SENSOR_MODE, font=('arial', 10, 'bold'), wrap=True,
                                       bg='white')
    """--------------------------------------------------------------------------------------------------"""
    self.Input_Type_label.place(x=SENSOR_POSITION_X_val, y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 3))
    self.Input_Type_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                              y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 3))

    self.LOG_REPEAT_COUNT_label.place(x=SENSOR_POSITION_X_val,
                                      y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 4.5))
    self.LOG_REPEAT_COUNT_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                    y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 4.5))

    self.VEHICLE_DATA_SOURCE_label.place(x=SENSOR_POSITION_X_val,
                                         y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 6))
    self.VEHICLE_DATA_SOURCE_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                       y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 6))

    self.FUSION_DETECTION_SOURCE_label.place(x=SENSOR_POSITION_X_val,
                                             y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 7.5))
    self.FUSION_DETECTION_SOURCE_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                           y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 7.5))

    self.RESIM_EXECUTION_MODE_label.place(x=SENSOR_POSITION_X_val,
                                          y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 9))
    self.RESIM_EXECUTION_MODE_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 9))

    self.TOBJECT_injection_type_label.place(x=SENSOR_POSITION_X_val,
                                            y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 10.5))
    self.TOBJECT_injection_type_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                          y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 10.5))

    self.INPUT_DATA_TYPE_label.place(x=SENSOR_POSITION_X_val,
                                     y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 12))
    self.INPUT_DATA_TYPE_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                   y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 12))

    self.SENSOR_RUN_MODE_label.place(x=SENSOR_POSITION_X_val,
                                     y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 13.5))
    self.SENSOR_RUN_MODE_spn.place(x=SENSOR_POSITION_X_val + x_offset,
                                   y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 13.5))

    HIL_Data_save.Update_HIL_Tool_Config_backup_data(Tool_config)
    application_buttons(self, Global_var, Tool_config)
    Global_var.HIL_Tool_config_done = True
    return


def Get_Log_path(self, Tool_config, Log_Entry):
    Tool_config.log_path = Log_Entry.get()
    if (Tool_config.log_path is None) or (Tool_config.log_path is '') or ("Browse Log path" in Tool_config.log_path):
        # show an "Open" dialog box and return the path to the selected fileot
        Tool_config.log_path = askopenfilename()
        Log_Entry.delete(0, 'end')
        Log_Entry.insert(0, Tool_config.log_path)
        print("Log_path:", Tool_config.log_path)
    else:
        pass


def HIL_Tool_Configuration_options(HIL_widgets_obj, HIL_Global_var, HIl_Tool_config, X_AIX, Y_AIX):
    global Tool_cfg
    global Enet_widgets
    global Enet_Var
    print("HIL_Global_var.HIL_Tool_Radiobutton_done : ", HIL_Global_var.HIL_Tool_Radiobutton_done)
    if HIL_Global_var.HIL_Tool_Radiobutton_done == False:
        """Initialize data variables"""
        HIl_Tool_config.__init__()
        """-------------------------"""
        Radiobutton_var = IntVar()
        Tool_cfg.ENET_cfg = Radiobutton(root, text="ETHERNET CONFIGURATION", font=('arial', 10, 'bold'),
                                        bg=Bg_colour, variable=Radiobutton_var, value=1, state=NORMAL,
                                        command=lambda: Ethernet_Configuration(Enet_widgets, Enet_Var, HIL_Global_var,
                                                                               HIL_widgets_obj,
                                                                               X_AIX, Y_AIX))

        Tool_cfg.HIL_Tool_cfg = Radiobutton(root, text="HIL TOOL CONFIGURATION", font=('arial', 10, 'bold'),
                                            bg=Bg_colour, variable=Radiobutton_var, value=2, state=NORMAL,
                                            command=lambda: HIL_Tool_configuration(HIL_widgets_obj, HIL_Global_var,
                                                                                   HIl_Tool_config, X_AIX, Y_AIX))

        Tool_cfg.HIL_Tool_cfg.place(x=50, y=150 + (HIL_Global_var.HIL_offset_y * 10))
        Tool_cfg.ENET_cfg.place(x=50, y=150 + (HIL_Global_var.HIL_offset_y * 12))
        HIL_Global_var.HIL_Tool_Radiobutton_done = True
        progress_update(20)
    else:
        pass
    return


def Ethernet_Configuration(self, E_Var, Global_var, HIL_widgets_obj, SENSOR_POSITION_X_val, SENSOR_POSITION_Y_val):
    x_offset = 180
    chkbn_x_offset = 450
    if Global_var.HIL_Tool_config_done == True:
        # If the HIL widgets are displayed already
        # We have to hide those widgets before display ENET widgets
        HIL_widgets_obj.Clear_all_Tool_config()
        Global_var.HIL_Tool_config_done = False
    if Global_var.Ethernet_config_done == False:
        self.Ethernet_config_label = Label(root, text="ETHERNET CONFIGURATION", font=('arial', 14, 'bold'),
                                           bg=Bg_colour)

        self.Log_path_label = Label(root, text="ENTER LOG PATH", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.Log_path_Entry = Entry(root, font=('arial', 10, 'bold'), width=55, bg='white', fg='green')
        self.Log_path_Button = Button(root, text="Browse", font=('arial', 10, 'bold'), height=1, bg=Bg_colour, bd=1,
                                      command=lambda: Get_Log_path(self.Log_path_Button, E_Var,
                                                                   self.Log_path_Entry))

        """--------------------------------------------------------------------------------------------------"""
        self.KPI_KEY_Entry_label = Label(root, text="KPI SECURITY KEY", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.KPI_KEY_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        """--------------------------------------------------------------------------------------------------"""

        self.HIL_ENGINE_IP_label = Label(root, text="HIL ENGINE IP", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.HIL_ENGINE_IP_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.SCALEXIO_IP_label = Label(root, text="SCALEXIO IP", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.SCALEXIO_IP_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.SCALEXIO_PORT_label = Label(root, text="SCALEXIO PORT", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.SCALEXIO_PORT_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.UDP_PORT_DATA_label = Label(root, text="UDP PORT DATA", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.UDP_PORT_DATA_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.UDP_PORT_XCP_label = Label(root, text="UDP PORT XCP", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.UDP_PORT_XCP_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.UDP_PORT_LOGGING_label = Label(root, text="UDP LOGGING PORT ", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.UDP_PORT_LOGGING_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.INTERFACE_PORT_label = Label(root, text="INTERFACE PORT", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.INTERFACE_PORT_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.SET_FAULT_FROM_label = Label(root, text="SCAN INDEX START", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.SET_FAULT_FROM_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        self.SET_FAULT_TILL_label = Label(root, text="SCAN INDEX END", font=('arial', 10, 'bold'), bg=Bg_colour)
        self.SET_FAULT_TILL_Entry = Entry(root, font=('arial', 10, 'bold'), width=21, bg='white', fg='green')

        """----------------------------placing widgets-----------------------------------------"""
        self.Ethernet_config_label.place(x=SENSOR_POSITION_X_val + 170,
                                         y=SENSOR_POSITION_Y_val)

        self.Log_path_label.place(x=SENSOR_POSITION_X_val,
                                  y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 2))
        self.Log_path_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                  y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 2))
        self.Log_path_Button.place(x=SENSOR_POSITION_X_val + 685,
                                   y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 2))

        self.KPI_KEY_Entry_label.place(x=SENSOR_POSITION_X_val,
                                       y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 3.5))
        self.KPI_KEY_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                 y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 3.5))

        self.HIL_ENGINE_IP_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                       y=SENSOR_POSITION_Y_val + + (HIL_Global_var.HIL_offset_y * 5))
        self.HIL_ENGINE_IP_label.place(x=SENSOR_POSITION_X_val,
                                       y=SENSOR_POSITION_Y_val + + (HIL_Global_var.HIL_offset_y * 5))

        self.SCALEXIO_IP_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                     y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 6.5))
        self.SCALEXIO_IP_label.place(x=SENSOR_POSITION_X_val,
                                     y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 6.5))

        self.SCALEXIO_PORT_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                       y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 8))
        self.SCALEXIO_PORT_label.place(x=SENSOR_POSITION_X_val,
                                       y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 8))

        self.UDP_PORT_DATA_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                       y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 9.5))
        self.UDP_PORT_DATA_label.place(x=SENSOR_POSITION_X_val,
                                       y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 9.5))

        self.UDP_PORT_XCP_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                      y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 11))
        self.UDP_PORT_XCP_label.place(x=SENSOR_POSITION_X_val,
                                      y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 11))

        self.UDP_PORT_LOGGING_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                          y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 12.5))
        self.UDP_PORT_LOGGING_label.place(x=SENSOR_POSITION_X_val,
                                          y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 12.5))

        self.INTERFACE_PORT_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 14))
        self.INTERFACE_PORT_label.place(x=SENSOR_POSITION_X_val,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 14))

        self.SET_FAULT_FROM_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 15.5))
        self.SET_FAULT_FROM_label.place(x=SENSOR_POSITION_X_val,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 15.5))

        self.SET_FAULT_TILL_Entry.place(x=SENSOR_POSITION_X_val + x_offset,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 17))
        self.SET_FAULT_TILL_label.place(x=SENSOR_POSITION_X_val,
                                        y=SENSOR_POSITION_Y_val + (HIL_Global_var.HIL_offset_y * 17))

        ENET_data_save.Update_ETHERNET_Config_backup_data(self)
        application_buttons(self, Global_var, E_Var)
        Global_var.Ethernet_config_done = True
        pass
    return


def application_buttons(self, Global_var, Data):
    global App_widgets
    if Global_var.application_buttons_init_done == False:
        App_widgets.Start_btn = Button(root, text="START", font=('arial', 12, 'bold'), height=1, bg=Bg_colour, bd=1,
                                       command=lambda: HIL_START(self, Global_var, Data))

        App_widgets.Save_btn = Button(root, text="SAVE", font=('arial', 12, 'bold'), height=1, bg=Bg_colour, bd=1,
                                      command=lambda: HIL_SAVE(self, Global_var, Data))

        App_widgets.Clear_btn = Button(root, text="CLEAR", font=('arial', 12, 'bold'), height=1, bg=Bg_colour, bd=1,
                                       command=lambda: HIL_CLEAR(self, Global_var, Data))

        App_widgets.Start_btn.place(x=1040, y=50 + (HIL_Global_var.HIL_offset_y * 16))
        App_widgets.Save_btn.place(x=950, y=50 + (HIL_Global_var.HIL_offset_y * 16))
        App_widgets.Clear_btn.place(x=850, y=50 + (HIL_Global_var.HIL_offset_y * 16))

        Global_var.application_buttons_init_done = True

    Global_var.HIL_Tool_data_save = False
    Global_var.Ethernet_data_save = False
    return


def HIL_START(self, Global_var, Data):
    print("HIL_START write XML")
    Write_HIL_TOOL_XML(HIL_Global_var, HIL_Data_save, ENET_data_save, HIL_Sensor_and_ECU_obj)
    Execute_HIL_Command(ENET_data_save.Log_path)
    return


def HIL_SAVE(self, Global_var, Data):
    #print("HIL_SAVE write XML")
    progress_update(50)
    if Global_var.HIL_Tool_config_done == True:
        """save dtaa"""
        print("HIL SAVE DATA ==========================")
        HIL_Data_save.save_data(HIl_Tool_config)
        # HIL_data_print_test(HIL_Data_save)
        Global_var.HIL_Tool_data_save = True
    elif Global_var.Ethernet_config_done == True:
        ENET_data_save.save_data(Enet_widgets)
        # ENET_data_print_test(ENET_data_save)
        Global_var.Ethernet_data_save = True
        pass
    else:
        pass
    return


def HIL_CLEAR(self, Global_var, Data):
    print("HIL_CLEAR write XML")
    if Global_var.HIL_Tool_config_done == True:
        HIL_Data_save.__init__()
        HIL_Data_save.clear_check_buttons(HIL_widgets_obj)
        HIL_Data_save.Update_HIL_Tool_Config_backup_data(Data)
        # HIL_data_print_test(HIL_Data_save)
    elif Global_var.Ethernet_config_done == True:
        ENET_data_save.__init__()
        ENET_data_save.Update_ETHERNET_Config_backup_data(Enet_widgets)
    else:
        pass
    progress_update(20)
    return


def Update_process():
    print("Update process")
    root.update()
    root.after(1000)
    return


def Clear_all_widgets():
    # This function will help to clear all the widgets used in the project
    # init function calls will help restore the default value.
    HIL_Data_save.__init__()
    HIL_Data_save.clear_check_buttons(HIL_widgets_obj)
    ENET_data_save.__init__()
    HIL_widgets_obj.Clear_all()
    HIL_widgets_obj.Clear_all_Tool_config()
    Enet_widgets.Clear_all_ENET_config()
    HIL_widgets_obj.__init__()
    HIL_Sensor_pos_widgets_obj.Clear_all_HIL_Sensor_pos_widgets()
    Tool_cfg.Clear_Tool_config_options()
    App_widgets.Clear_Application_widgets()
    return


def HIL_data_print_test(HIl_Tool_config):
    print("Radar_fusion_EN ", HIl_Tool_config.Radar_fusion_EN)
    print("Debug_Mode ", HIl_Tool_config.Debug_Mode)
    print("Execution_Mode ", HIl_Tool_config.Execution_Mode)
    print("Auto_close_win ", HIl_Tool_config.Auto_close_win)
    print("Continues_run_mod ", HIl_Tool_config.Continues_run_mod)
    print("input_option ", HIl_Tool_config.input_option)
    print("Log_repeate_cnt ", HIl_Tool_config.Log_repeate_cnt)
    print("Vehicle_data_source ", HIl_Tool_config.Vehicle_data_source)
    print("Fusion_det_source ", HIl_Tool_config.Fusion_det_source)
    print("Input_data_type ", HIl_Tool_config.Input_data_type)
    print("Sensor_run_mode ", HIl_Tool_config.Sensor_run_mode)
    print("TOBJECT_injection_type ", HIl_Tool_config.TOBJECT_injection_type)
    return


def ENET_data_print_test(ENET_data_save):
    print("HIL_ENGINE_IP = ", ENET_data_save.HIL_ENGINE_IP)
    print("SCALEXIO_IP = ", ENET_data_save.SCALEXIO_IP)
    print("SCALEXIO_PORT = ", ENET_data_save.SCALEXIO_PORT)
    print("UDP_PORT_DATA = ", ENET_data_save.UDP_PORT_DATA)
    print("UDP_PORT_XCP = ", ENET_data_save.UDP_PORT_XCP)
    print("UDP_PORT_LOGGING = ", ENET_data_save.UDP_PORT_LOGGING)
    print("INTERFACE_PORT = ", ENET_data_save.INTERFACE_PORT)
    print("SET_FAULT_FROM = ", ENET_data_save.SET_FAULT_FROM)
    print("SET_FAULT_TILL = ", ENET_data_save.SET_FAULT_TILL)
    print("KPI_Security_Key = ", ENET_data_save.KPI_Security_Key)
    print("Log_path = ", ENET_data_save.Log_path)
    return



