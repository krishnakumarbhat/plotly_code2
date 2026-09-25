from tkinter import *
from tkinter.ttk import *
from Main import *
from main_interface import *
from MUDP_Tool_spec_widgets import Sensor_and_ECU_obj
from MUDP_Tool_spec_widgets import *

def MUDP_Extractor():
    root.title("MUDP EXTRACTOR")
    # Update_SENSOR_POSITION(x,y)
    Update_SENSOR_POSITION(Sensor_and_ECU_obj, 60, 60)
    Update_STREAM_OPTION(stream_options_obj, stream_options_chkbtn_obj, 310, 60)
    display_labels()
    Tool_info()
    # clear_mudp_log_list()

    # Update_lib_tool_config(540,60)
    # Entry_widgets(540,60)
    # Button_widgets(540,60)
    return




