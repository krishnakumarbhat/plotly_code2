from tkinter import *
from tkinter.ttk import *
from Main import *
from main_interface import *
from SIL_TooL_spec_widgets import delete_list_logs, SIL_widgets_obj
from SIL_TooL_spec_widgets import *


def SIL_TOOL():
    root.title("SIL TOOL")
    delete_list_logs()
    Update_SIL_Tool_offset()
    Check_SIL_customer_name(SIL_widgets_obj, 50, 50)
    display_labels()
    Tool_info()
    # browse_sil_logs()
    return

# ================ Basic working of HIL tool ===========================================================
# Once the SIL tool  is selected from the main menu control will come to SIL main
# SIL offset will set based on the Global offset (Depend on OS)
# Checking for customer name : Once selected customer name
# Sensor options will display, at least one sensor selected SIL and ENET config options will display
# Select any one of them to input the configurations to generate th SIL XML
# Once you selected any one option, set or enter values to the fields, once enter the values
# press the save button to store the config data to backup variables
# if not press the save button, when you change the config options to another one
# Unsaved data will lose and all the fields will reset to default value.
# Once input all the required data press start button to start writing XML file.
# Once XML writing is done, automatically call the execution command
# this will be a combination of 3 inputs (exe, xml ,log path)
# ========================================================================================================
# root - Mast tkinter object
# textvariable - setting a variable to store the data when status change happen
# font=('arial', 10, 'bold'),
# bg - back ground colour setting
# values - if a list of options available assign to values (list,tuple)
# wrap - This is used to enable or disable rotation of spinbox
# command - is used to call the required actions
# =========================================================================================================