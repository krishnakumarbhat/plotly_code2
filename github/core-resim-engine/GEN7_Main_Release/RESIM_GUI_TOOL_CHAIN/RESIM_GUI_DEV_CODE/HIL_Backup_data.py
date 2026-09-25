class Data_save_HIL_Tool_Config_variables(object):
    def __init__(self):
        self.input_option = None
        self.Continues_run_mod = None
        self.Log_repeate_cnt = None
        self.Vehicle_data_source = None
        self.Fusion_det_source = None
        self.Auto_close_win = None
        self.Input_data_type = None
        self.Sensor_run_mode = None
        self.Radar_fusion_EN = None
        self.Execution_Mode = None
        self.Debug_Mode = None
        self.TOBJECT_injection_type = None
        self.INJECT_FAULT_SENSOR_RL = None
        self.INJECT_FAULT_SENSOR_RR = None

    def save_data(self, HIL_data):
        self.input_option = HIL_data.input_option.get()
        Continues_run_mod_dist = ("NO", "YES")
        self.Continues_run_mod = Continues_run_mod_dist[HIL_data.Continues_run_mod.get()]
        self.Log_repeate_cnt = HIL_data.Log_repeate_cnt.get()
        self.Vehicle_data_source = HIL_data.Vehicle_data_source.get()
        self.Fusion_det_source = HIL_data.Fusion_det_source.get()
        Auto_close_win_dist = ("NO", "YES")
        self.Auto_close_win = Auto_close_win_dist[HIL_data.Auto_close_win.get()]
        Input_data_type_dist = {'LOG_SIMULATION': 1, 'LIVE_SIMULATION': 2}
        self.Input_data_type = Input_data_type_dist.get(HIL_data.Input_data_type.get())
        Sensor_run_mode_dist = {'DETECTION': 1, 'TRACKER': 2}
        self.Sensor_run_mode = Sensor_run_mode_dist.get(HIL_data.Sensor_run_mode.get())
        Radar_fusion_EN_dist = ("NO", "YES")
        self.Radar_fusion_EN = Radar_fusion_EN_dist[HIL_data.Radar_fusion_EN.get()]
        self.Execution_Mode = HIL_data.Execution_Mode.get()
        Debug_Mode_dist = ("ENABLED", "DISABLED")
        self.Debug_Mode = Debug_Mode_dist[HIL_data.Debug_Mode.get()]
        self.TOBJECT_injection_type = HIL_data.TOBJECT_injection_type.get()
        self.INJECT_FAULT_SENSOR_RL = HIL_data.INJECT_FAULT_SENSOR_RL.get()
        self.INJECT_FAULT_SENSOR_RR = HIL_data.INJECT_FAULT_SENSOR_RR.get()
        return

    def Update_HIL_Tool_Config_backup_data(self, HIL_data):
        HIL_data.input_option.set(self.input_option)
        HIL_data.Log_repeate_cnt.set(self.Log_repeate_cnt)
        HIL_data.Vehicle_data_source.set(self.Vehicle_data_source)
        HIL_data.Fusion_det_source.set(self.Fusion_det_source)
        HIL_data.Input_data_type.set(self.Input_data_type)
        HIL_data.Sensor_run_mode.set(self.Sensor_run_mode)
        HIL_data.Execution_Mode.set(self.Execution_Mode)
        HIL_data.TOBJECT_injection_type.set(self.TOBJECT_injection_type)

    def clear_check_buttons(self, HIL_data):
        HIL_data.CONTINUOUS_RUN_MODE_chkbtn.deselect()
        HIL_data.AUTOCLOSE_COMMAND_WIN_chkbtn.deselect()
        HIL_data.RADAR_FUSION_ENABLED_chkbtn.deselect()
        HIL_data.DEBUGGING_HIL_chkbtn.deselect()
        HIL_data.INJECT_FAULT_SENSOR_RL_chkbn.deselect()
        HIL_data.INJECT_FAULT_SENSOR_RR_chkbn.deselect()


class Data_save_Ethernet_config_variables(object):
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
        self.KPI_Security_Key = "Enter Security Key"
        self.Log_path = "===========> Browse Log path <==========="

    def save_data(self, Enet_data):
        self.HIL_ENGINE_IP = Enet_data.HIL_ENGINE_IP_Entry.get()
        self.SCALEXIO_IP = Enet_data.SCALEXIO_IP_Entry.get()
        self.SCALEXIO_PORT = Enet_data.SCALEXIO_PORT_Entry.get()
        self.UDP_PORT_DATA = Enet_data.UDP_PORT_DATA_Entry.get()
        self.UDP_PORT_XCP = Enet_data.UDP_PORT_XCP_Entry.get()
        self.UDP_PORT_LOGGING = Enet_data.UDP_PORT_LOGGING_Entry.get()
        self.INTERFACE_PORT = Enet_data.INTERFACE_PORT_Entry.get()
        self.SET_FAULT_FROM = Enet_data.SET_FAULT_FROM_Entry.get()
        self.SET_FAULT_TILL = Enet_data.SET_FAULT_TILL_Entry.get()
        self.KPI_Security_Key = Enet_data.KPI_KEY_Entry.get()
        self.Log_path = Enet_data.Log_path_Entry.get()
        return

    def Update_ETHERNET_Config_backup_data(self, Enet_data):
        self.Clear_entry_field(Enet_data)
        Enet_data.HIL_ENGINE_IP_Entry.insert(0, self.HIL_ENGINE_IP)
        Enet_data.SCALEXIO_IP_Entry.insert(0, self.SCALEXIO_IP)
        Enet_data.SCALEXIO_PORT_Entry.insert(0, self.SCALEXIO_PORT)
        Enet_data.UDP_PORT_DATA_Entry.insert(0, self.UDP_PORT_DATA)
        Enet_data.UDP_PORT_XCP_Entry.insert(0, self.UDP_PORT_XCP)
        Enet_data.UDP_PORT_LOGGING_Entry.insert(0, self.UDP_PORT_LOGGING)
        Enet_data.INTERFACE_PORT_Entry.insert(0, self.INTERFACE_PORT)
        Enet_data.SET_FAULT_FROM_Entry.insert(0, self.SET_FAULT_FROM)
        Enet_data.SET_FAULT_TILL_Entry.insert(0, self.SET_FAULT_TILL)
        Enet_data.KPI_KEY_Entry.insert(0, self.KPI_Security_Key)
        Enet_data.Log_path_Entry.delete(0, 'end')
        Enet_data.Log_path_Entry.insert(0, self.Log_path)
        return

    def Clear_entry_field(self, Enet_data):
        Enet_data.HIL_ENGINE_IP_Entry.delete(0, 'end')
        Enet_data.SCALEXIO_IP_Entry.delete(0, 'end')
        Enet_data.SCALEXIO_PORT_Entry.delete(0, 'end')
        Enet_data.UDP_PORT_DATA_Entry.delete(0, 'end')
        Enet_data.UDP_PORT_XCP_Entry.delete(0, 'end')
        Enet_data.UDP_PORT_LOGGING_Entry.delete(0, 'end')
        Enet_data.INTERFACE_PORT_Entry.delete(0, 'end')
        Enet_data.SET_FAULT_FROM_Entry.delete(0, 'end')
        Enet_data.SET_FAULT_TILL_Entry.delete(0, 'end')
        Enet_data.KPI_KEY_Entry.delete(0, 'end')
        Enet_data.Log_path_Entry.delete(0, 'end')


"""Global class objects"""
HIL_Data_save = Data_save_HIL_Tool_Config_variables()
ENET_data_save = Data_save_Ethernet_config_variables()


