from tkinter import *
from tkinter.ttk import *
from HIL_Tool_spec_widgets import *


def HIL_TOOL():
    root.title("HIL TOOL")
    Update_HIL_Tool_offset()
    Check_customer_name(HIL_widgets_obj, 50, 50)
    return

# ================ Basic working of HIL tool ===========================================================
# Once the HIL tool  is selected from the main menu control will come to HIL main
# HIL offset will set based on the Global offset (Depend on OS)
# Checking for customer name : Once selected customer name
# Sensor options will display, at least one sensor selected HIL and ENET config options will display
# Select any one of them to input the configurations to generate th HIL XML
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