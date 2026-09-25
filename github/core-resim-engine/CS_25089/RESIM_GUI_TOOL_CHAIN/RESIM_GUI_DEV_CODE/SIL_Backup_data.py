class Data_save_SIL_Tool_Config_variables(object):
    def __init__(self):
        self.output_Path_options = None
        self.output_Path_Location = None
        # self.JSON_Flist_Path = None
        self.Sensor_Config = None
        self.RECU_Config = None
        self.CAN_Output = None
        self.APTIV_Internal_Output = None
        self.CDC_Write_output = None
        self.ORCAS_MDF4 = None
        self.CANAPE_MDF4 = None
        self.VIGEM_VECTOR_MDF4 = None
        self.VIGEM_VPCAP = None
        self.VIGEM_CCA_MDF4 = None
        self.X2E_VECTOR_MDF4 = None
        self.CANOE_VECTOR_MF4 = None
        self.TS_TRACE = None
        self.DEBUG_TRACE = None
        self.TIMING_PROFILE = None
        self.SIL_Injection_Mode = None
        self.Output_File_Format = None
        self.LOG_Replay_Mode = None
        self.SIL_Entrypoint = None
        self.Calibration_Source = None
        self.GDSR_Init_Status = None
        self.GDSR_Periodic_Status = None
        self.TIMING_PROFILE_Path = None
        self.RESIM_ERROR_TRACE_Path = None
        self.PLP_TIMESTAMP = None
        self.SIGNAL_CREATION = None
        self.STATISTIC_REPORT_PATH = None
        self.Log_Include_Date_Time = None
        self.Log_Tracing_Path = None
        self.Log_Level = None
        # self.TIMING_PROFILE_Path_entry = None

    def save_data(self, SIL_data, SIL_widgets):
        self.output_Path_options = SIL_data.output_Path_options.get()
        self.output_Path_Location = SIL_widgets.output_Path_Location_spn.get()
        # self.JSON_Flist_Path = SIL_widgets.JSON_Flist_Path_spn.get()
        self.Sensor_Config = SIL_widgets.Sensor_Config_spn.get()
        self.RECU_Config = SIL_widgets.RECU_Config_spn.get()
        self.CAN_Output = SIL_data.CAN_Output.get()
        self.APTIV_Internal_Output = SIL_data.APTIV_Internal_Output.get()
        self.CDC_Write_output = SIL_data.CDC_Write_output.get()
        self.ORCAS_MDF4 = SIL_data.ORCAS_MDF4.get()
        self.CANAPE_MDF4 = SIL_data.CANAPE_MDF4.get()
        self.VIGEM_VECTOR_MDF4 = SIL_data.VIGEM_VECTOR_MDF4.get()
        self.VIGEM_VPCAP = SIL_data.VIGEM_VPCAP.get()
        self.VIGEM_CCA_MDF4 = SIL_data.VIGEM_CCA_MDF4.get()
        self.X2E_VECTOR_MDF4 = SIL_data.X2E_VECTOR_MDF4.get()
        self.CANOE_VECTOR_MF4 = SIL_data.CANOE_VECTOR_MF4.get()
        self.TS_TRACE = SIL_data.TS_TRACE.get()
        self.DEBUG_TRACE = SIL_data.DEBUG_TRACE.get()
        self.TIMING_PROFILE = SIL_data.TIMING_PROFILE.get()
        self.GDSR_Init_Status = SIL_data.GDSR_Init_Status.get()
        self.GDSR_Periodic_Status = SIL_data.GDSR_Periodic_Status.get()
        self.Output_File_Format = SIL_data.Output_File_Format.get()
        self.SIL_Injection_Mode = SIL_data.SIL_Injection_Mode.get()
        self.LOG_Replay_Mode = SIL_data.LOG_Replay_Mode.get()
        self.SIL_Entrypoint = SIL_data.SIL_Entrypoint.get()
        self.Calibration_Source = SIL_data.Calibration_Source.get()
        self.TIMING_PROFILE_Path = SIL_data.TIMING_PROFILE_Path.get()
        self.RESIM_ERROR_TRACE_Path = SIL_data.RESIM_ERROR_TRACE_Path.get()
        self.PLP_TIMESTAMP = SIL_data.PLP_TIMESTAMP.get()
        self.SIGNAL_CREATION = SIL_data.SIGNAL_CREATION.get()
        self.STATISTIC_REPORT_PATH = SIL_data.STATISTIC_REPORT_PATH.get()
        self.Log_Include_Date_Time = SIL_data.Log_Include_Date_Time.get()
        self.Log_Tracing_Path = SIL_data.Log_Tracing_Path.get()
        self.Log_Level = SIL_data.Log_Level.get()
        return

    def Update_SIL_Tool_Config_backup_data(self, SIL_data):
        SIL_data.output_Path_options.set(self.output_Path_options)
        SIL_data.output_Path_Location.set(self.output_Path_Location)
        # SIL_data.TIMING_PROFILE_Path.set(self.TIMING_PROFILE_Path)
        # SIL_data.JSON_Flist_Path.set(self.JSON_Flist_Path)
        SIL_data.Sensor_Config.set(self.Sensor_Config)
        SIL_data.RECU_Config.set(self.RECU_Config)
        SIL_data.SIL_Injection_Mode.set(self.SIL_Injection_Mode)
        SIL_data.Output_File_Format.set(self.Output_File_Format)
        SIL_data.LOG_Replay_Mode.set(self.LOG_Replay_Mode)
        SIL_data.Calibration_Source.set(self.Calibration_Source)
        SIL_data.SIL_Entrypoint.set(self.SIL_Entrypoint)
        # SIL_data.STATISTIC_REPORT_PATH.set(self.STATISTIC_REPORT_PATH)
        # SIL_data.TIMING_PROFILE_Path_entry.insert(0, self.TIMING_PROFILE_Path)

    def clear_check_buttons(self, SIL_data):
        SIL_data.CAN_Output_chkbtn.deselect()
        SIL_data.APTIV_Internal_Output_chkbtn.deselect()
        SIL_data.CDC_Write_output_chkbtn.deselect()
        # SIL_data.TS_TRACE_chkbtn.deselect()
        # SIL_data.DEBUG_TRACE_chkbtn.deselect()
        SIL_data.TIMING_PROFILE_chkbtn.deselect()
        SIL_data.GDSR_Init_Status_chkbtn.deselect()
        SIL_data.GDSR_Periodic_Status_chkbtn.deselect()
        SIL_data.Log_Include_Date_Time_chkbtn.deselect()
        # SIL_data.TIMING_PROFILE_chkbtn.deselect()

    '''def clear_Entries(self, SIL_data):
        SIL_data.TIMING_PROFILE_Path_entry.delete(0, 'end')'''


class Data_save_SIL_Ethernet_config_variables(object):
    def __init__(self):
        self.DESTINATION_IP = "127.0.0.1"
        self.DEST_OUTPUT_PORT = 5555
        self.DEST_INPUT_PORT = 5556
        self.OUTPUT_UDP_TRANSMISSION = "DISABLE"
        self.INPUT_UDP_TRANSMISSION = "DISABLE"
        self.TRANSMISSION_STATUS = '"TRUE"'
        # self.LogLevel = "0"
        # self.LogTracingPath = "NONE"
        # self.LogIncludeDateTime = "0"
        # self.STATISTIC_REPORT_PATH = "NONE"

    def save_data(self, SIL_Enet_data):
        self.DESTINATION_IP = SIL_Enet_data.DESTINATION_IP_Entry.get()
        self.DEST_OUTPUT_PORT = SIL_Enet_data.DEST_OUTPUT_PORT_Entry.get()
        self.DEST_INPUT_PORT = SIL_Enet_data.DEST_INPUT_PORT_Entry.get()
        self.OUTPUT_UDP_TRANSMISSION = SIL_Enet_data.OUTPUT_UDP_TRANSMISSION_Entry.get()
        self.INPUT_UDP_TRANSMISSION = SIL_Enet_data.INPUT_UDP_TRANSMISSION_Entry.get()
        self.TRANSMISSION_STATUS = SIL_Enet_data.TRANSMISSION_STATUS_Entry.get()
        # self.LogLevel = SIL_Enet_data.LogLevel_Entry.get()
        # self.LogTracingPath = SIL_Enet_data.LogTracingPath_Entry.get()
        # self.LogIncludeDateTime = SIL_Enet_data.LogIncludeDateTime_Entry.get()
        # self.STATISTIC_REPORT_PATH = SIL_Enet_data.STATISTIC_REPORT_PATH_Entry.get()
        return

    def Update_ETHERNET_Config_backup_data(self, SIL_Enet_data):
        self.Clear_entry_field(SIL_Enet_data)
        SIL_Enet_data.DESTINATION_IP_Entry.insert(0, self.DESTINATION_IP)
        SIL_Enet_data.DEST_OUTPUT_PORT_Entry.insert(0, self.DEST_OUTPUT_PORT)
        SIL_Enet_data.DEST_INPUT_PORT_Entry.insert(0, self.DEST_INPUT_PORT)
        SIL_Enet_data.OUTPUT_UDP_TRANSMISSION_Entry.insert(0, self.OUTPUT_UDP_TRANSMISSION)
        SIL_Enet_data.INPUT_UDP_TRANSMISSION_Entry.insert(0, self.INPUT_UDP_TRANSMISSION)
        SIL_Enet_data.TRANSMISSION_STATUS_Entry.insert(0, self.TRANSMISSION_STATUS)
        # SIL_Enet_data.LogLevel_Entry.insert(0, self.LogLevel)
        # SIL_Enet_data.LogTracingPath_Entry.insert(0, self.LogTracingPath)
        # SIL_Enet_data.LogIncludeDateTime_Entry.insert(0, self.LogIncludeDateTime)
        # SIL_Enet_data.STATISTIC_REPORT_PATH_Entry.insert(0, self.STATISTIC_REPORT_PATH)
        return

    def Clear_entry_field(self, SIL_Enet_data):
        SIL_Enet_data.DESTINATION_IP_Entry.delete(0, 'end')
        SIL_Enet_data.DEST_OUTPUT_PORT_Entry.delete(0, 'end')
        SIL_Enet_data.DEST_INPUT_PORT_Entry.delete(0, 'end')
        SIL_Enet_data.OUTPUT_UDP_TRANSMISSION_Entry.delete(0, 'end')
        SIL_Enet_data.INPUT_UDP_TRANSMISSION_Entry.delete(0, 'end')
        SIL_Enet_data.TRANSMISSION_STATUS_Entry.delete(0, 'end')
        # SIL_Enet_data.LogLevel_Entry.delete(0, 'end')
        # SIL_Enet_data.LogTracingPath_Entry.delete(0, 'end')
        # SIL_Enet_data.LogIncludeDateTime_Entry.delete(0, 'end')
        # SIL_Enet_data.STATISTIC_REPORT_PATH_Entry.delete(0, 'end')


"""Global class objects"""
SIL_Data_save = Data_save_SIL_Tool_Config_variables()
SIL_ENET_data_save = Data_save_SIL_Ethernet_config_variables()
