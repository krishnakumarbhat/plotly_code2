"""Import Modules"""
from tkinter import *
from tkinter.ttk import *
import time
import multiprocessing
from MUDP_Main import *
from MUDP_Tool_spec_widgets import *
from HIL_Main import *
from SIL_Main import *
from Converter_Main import *
from Packet_Loss_Main import *
from Data_Crawler_Main import *
from Bordnet_Main import *
from User_Guides import *
from Release_Notes import *

global canvas
global image_Num
global canvas_init
image_Num = 0
canvas_init = 0


def main():
    # Setting menu buttons for different applications
    print('in main')
    menu = Menu(root)  # Creating object of menu , root is the tkinter object
    root.config(menu=menu)
    Tools = Menu(menu)  # creating object for each menu button
    Homemenu = Menu(menu)
    menu.add_cascade(label='Home', menu=Homemenu)
    Homemenu.add_command(label='Home', command=lambda: Home_image(root))
    menu.add_cascade(label='Tools', menu=Tools)  # adding multiple menu button
    Tools.add_command(label='MUDP Extractor',
                      command=lambda: Call_MUDP_Extractor())  # adding sub menu button and command
    Tools.add_command(label='HIL Tool', command=lambda: Call_HIL_Tool())
    Tools.add_command(label='SIL Tool', command=lambda: Call_SIL_Tool())
    Tools.add_command(label='Bordnet Tool', command=lambda: Call_Bordnet_Tool())
    Tools.add_command(label='Converter Tool', command=lambda: Call_Converter_Tool())
    Tools.add_command(label='Data Crawler', command=lambda: Call_Data_Crawler_Tool())
    Tools.add_command(label='Packetloss Tool', command=lambda: Call_Packet_Loss_Tool())
    Tools.add_separator()
    Tools.add_command(label='Exit', command=root.quit)
    helpmenu = Menu(menu)
    menu.add_cascade(label='Help', menu=helpmenu)
    helpmenu.add_command(label='About', command=lambda: About_call())
    toolsmenu = Menu(helpmenu)
    toolsmenu.add_command(label='MUDP Extractor', command=lambda: MUDP_User_Guide())
    toolsmenu.add_command(label='SIL Tool')
    toolsmenu.add_command(label='HIL Tool')
    toolsmenu.add_command(label='Bordnet Tool', command=Bordnet_User_Guide)
    toolsmenu.add_command(label='Converter Tool', command=Converter_User_Guide)
    toolsmenu.add_command(label='Packet loss Tool', command=Packetloss_User_Guide)
    toolsmenu.add_command(label='Data Crawler Tool', command=DataCrawler_User_Guide)
    helpmenu.add_cascade(label="User Guides", underline=0,menu=toolsmenu)  # , menu=toolsmenu
    # helpmenu.add_command(label="User Guides", command=lambda: Call_User_Guides())

    display_labels()  # Display aptiv labels
    Tool_info()  # display tool informations
    return


def clear_screen():
    for child in root.winfo_children():
        child.destroy()
    return


def destroy_canvas():
    global canvas
    global canvas_init
    print('dest canvas_init', canvas_init)
    if canvas_init == 1:
        canvas.destroy()
        canvas_init = 0
    else:
        canvas_init = 0
    return


def Home_image(root_ptr):
    root.title("RESIM TOOL CHAIN")
    clear_screen()
    main()
    # Setting image in this function
    global image_Num
    global canvas_init
    global Image
    global canvas
    # print('canvas_init bfr', canvas_init)
    destroy_canvas()
    # print('canvas_init aft', canvas_init)
    canvas = Canvas(root_ptr, bg=Bg_colour, width=1200, height=550, bd=0, highlightthickness=0, relief='ridge')
    canvas.place(x=0, y=0, anchor=NW)
    images_path_name = os.path.normpath('Images/Aptiv')
    image_Num += 1
    img = images_path_name + str(image_Num) + '.png'
    print('img=', img)
    Image = PhotoImage(file=img)
    root_ptr.Image = Image  # to prevent the image garbage collected.
    canvas.create_image(0, 0, anchor=NW, image=Image)
    canvas_init = 1
    if image_Num == 4:
        image_Num = 0
    # print('Home_image canvas_init', canvas_init)
    return


def Call_MUDP_Extractor():
    clear_screen()
    main()
    MUDP_Extractor()
    return


def Call_HIL_Tool():
    clear_screen()
    main()
    HIL_TOOL()
    return


def Call_SIL_Tool():
    clear_screen()
    main()
    SIL_TOOL()
    return


def Call_Bordnet_Tool():
    clear_screen()
    main()
    Bordnet_Tool()


def Call_Converter_Tool():
    clear_screen()
    main()
    Converter_Tool()
    return


def Call_Packet_Loss_Tool():
    clear_screen()
    main()
    call_methods()
    return


def Call_Data_Crawler_Tool():
    clear_screen()
    main()
    Data_Crawler_Tool()
    return


def About_call():
    clear_screen()
    main()
    release_notes_call()
    # print_html()
    return


"""Calling User Guides"""
def MUDP_User_Guide():
    clear_screen()
    main()
    MUDP_Guide()


def Bordnet_User_Guide():
    clear_screen()
    main()
    Bordnet_Guide()


def Converter_User_Guide():
    clear_screen()
    main()
    Converter_Guide()


def Packetloss_User_Guide():
    clear_screen()
    main()
    Packetloss_Guide()


def DataCrawler_User_Guide():
    clear_screen()
    main()
    DataCrawler_Guide()


"""Program starting point"""
if __name__ == "__main__":
    multiprocessing.freeze_support()
    root.geometry('1200x620')  # setting the frame size
    root.resizable(0, 0)
    root.title("RESIM TOOL CHAIN")  # Tool Name
    root.configure(background=Bg_colour)  # Setting background colour
    root.resizable(0, 0)  # Don't allow resizing in the x or y direction
    run_OS_check()  # checking for OS to set the proper x-y value
    Home_image(root)
    main()
    root.mainloop()
