"""Import Modules"""
from tkinter import *
from tkinter.ttk import *
from MUDP_XML_writer import *
from MUDP_Main import *
from MUDP_Tool_spec_widgets import *
from HIL_Main import *
from SIL_Main import *


global canvas
global image_Num
global canvas_init
image_Num = 0
canvas_init=0

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
    Tools.add_separator()
    Tools.add_command(label='Exit', command=root.quit)
    helpmenu = Menu(menu)
    menu.add_cascade(label='Help', menu=helpmenu)
    helpmenu.add_command(label='About')
    display_labels()  # Display aptiv labels
    Tool_info()  # display tool informations
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
    image_Num += 1
    img = 'Aptiv' + str(image_Num) + '.png'
    # print('img=', img)
    Image = PhotoImage(file=img)
    root_ptr.Image = Image  # to prevent the image garbage collected.
    canvas.create_image(0, 0, anchor=NW, image=Image)
    canvas_init = 1
    if image_Num == 4:
        image_Num = 0
    # print('Home_image canvas_init', canvas_init)
    return


def Call_MUDP_Extractor():
    destroy_canvas()
    MUDP_Extractor()
    return

def Call_HIL_Tool():
    destroy_canvas()
    HIL_TOOL()
    return

def Call_SIL_Tool():
    destroy_canvas()
    SIL_TOOL()
    return

"""Program starting point"""
if __name__ == "__main__":
    root.geometry('1200x600')  # setting the frame size
    root.title("RESIM TOOL CHAIN")  # Tool Name
    root.configure(background=Bg_colour)  # Setting background colour
    root.resizable(0, 0)  # Don't allow resizing in the x or y direction
    run_OS_check()  # checking for OS to set the proper x-y value
    Home_image(root)
    main()
    root.mainloop()
