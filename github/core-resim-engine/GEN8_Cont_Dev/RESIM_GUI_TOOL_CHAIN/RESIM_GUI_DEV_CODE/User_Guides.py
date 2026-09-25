from tkinter import *
from Main import *
from main_interface import *
from tkPDFViewer import tkPDFViewer


def MUDP_Guide():
    display = tkPDFViewer.ShowPdf()
    v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_MUDP_Extracter_User_Guide.pdf", load='before',
                          width=100, height=30)
    v1.place(x=200, y=30)


def Bordnet_Guide():
    display = tkPDFViewer.ShowPdf()
    v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_Bordnet_User_Guide.pdf", load='before',
                          width=100, height=30)
    v1.place(x=200, y=30)


def Converter_Guide():
    display = tkPDFViewer.ShowPdf()
    v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_Converter_Tool_User_Guide.pdf", load='before',
                          width=100, height=30)
    v1.place(x=200, y=30)


def Packetloss_Guide():
    display = tkPDFViewer.ShowPdf()
    v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_Packetloss_User_Guide.pdf", load='before',
                          width=100, height=30)
    v1.place(x=200, y=30)


def DataCrawler_Guide():
    display = tkPDFViewer.ShowPdf()
    v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_Data_Crawler_User_Guide.pdf", load='before',
                          width=100, height=30)
    v1.place(x=200, y=30)


'''
def display_user_guides():
    Ask_Tool()


def Show_Document():
    count = 0
    display = tkPDFViewer.ShowPdf()

    # Selected_value = tool_combobox.get()
    Selected_value = tool_val.get()

    if Selected_value == 'MUDP Extractor':

        v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_MUDP_Extracter_User_Guide.pdf", load='before', width=100,
                              height='30')
        v1.place(x=250, y=30)

    elif Selected_value == 'Bordnet Tool':
        Label(text='Scroll Down for Bordnet Tool User Guide           ', bg=Bg_colour, font=('arial', 10, 'bold', 'italic')).place(x=750, y=535)
        v2 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_Bordnet_User_Guide.pdf", width=100,
                              height='30')
        v2.place(x=250, y=30)

    elif Selected_value == 'Converter Tool':
        Label(text='Scroll Down for Converter Tool User Guide          ', bg=Bg_colour, font=('arial', 10, 'bold', 'italic')).place(x=750, y=535)
        v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_Converter_Tool_User_Guide.pdf", width=100,
                              height='30')
        v1.place(x=250, y=30)

    elif Selected_value == 'Packetloss Tool':
        Label(text='Scroll Down for Packetloss Tool User Guide             ', bg=Bg_colour, font=('arial', 10, 'bold', 'italic')).place(x=750, y=535)
        v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_Packetloss_User_Guide.pdf", width=100,
                              height=30)
        v1.place(x=250, y=30)

    elif Selected_value == 'Data Crawler Tool':
        Label(text='Scroll Down for Data Crawler Tool User Guide           ', bg=Bg_colour, font=('arial', 10, 'bold', 'italic')).place(x=750, y=535)
        v1 = display.pdf_view(root, pdf_location="User_Guides\Python_GUI_Data_Crawler_User_Guide.pdf", width=100,
                              height='30')
        v1.place(x=250, y=30)


def Ask_Tool():
    Label(text='Select Tool: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=30, y=30)
    global tool_val
    tool_val = StringVar()
    tool_val.set('MUDP Extractor')

    """
    global tool_combobox
    tool_combobox = Combobox(root, width=27, textvariable=tool_val)

    tool_combobox['values'] = ["Select Tool", "MUDP Extractor", "SIL Tool", "HIL Tool",
                               "Bordnet Tool", "Converter Tool", "Packetloss TOol", "Data Crawler Tool"]
    tool_combobox.current(0)
    tool_combobox.place(x=40, y=60)

    tool_combobox.bind("<<ComboboxSelected>>", Show_Document)
    """

    """global Mudp_option
    Mudp_option = Radiobutton(text='MUDP Extractor', font=('arial', 10, 'bold'), variable=tool_val,
                              value="MUDP Extractor", bg=Bg_colour, command=Show_Document())
    Mudp_option.place(x=40, y=60)

    global HIL_option
    HIL_option = Radiobutton(text='HIL Tool', font=('arial', 10, 'bold'), variable=tool_val,
                             value="HIL Tool", bg=Bg_colour)
    HIL_option.place(x=40, y=90)
    HIL_option.config(state=DISABLED)

    global SIL_option
    SIL_option = Radiobutton(text='SIL Tool', font=('arial', 10, 'bold'), variable=tool_val,
                             value="SIL Tool", bg=Bg_colour)
    SIL_option.place(x=40, y=120)
    SIL_option.config(state=DISABLED)

    global Bordnet_option
    Bordnet_option = Radiobutton(text='Bordnet Tool', font=('arial', 10, 'bold'), variable=tool_val,
                                 value="Bordnet Tool", bg=Bg_colour, command=Show_Document) # lambda: Bordnet_Doc()
    Bordnet_option.place(x=40, y=150)

    global Converter_option
    Converter_option = Radiobutton(text='Converter Tool', font=('arial', 10, 'bold'), variable=tool_val,
                                   value="Converter Tool", bg=Bg_colour, command=Show_Document)
    Converter_option.place(x=40, y=180)

    global Packetloss_option
    Packetloss_option = Radiobutton(text='Packetloss Tool', font=('arial', 10, 'bold'), variable=tool_val,
                                    value="Packetloss Tool", bg=Bg_colour, command=Show_Document)
    Packetloss_option.place(x=40, y=210)

    global Datacrawler_option
    Datacrawler_option = Radiobutton(text='Data Crawler Tool', font=('arial', 10, 'bold'), variable=tool_val,
                                     value="Data Crawler Tool", bg=Bg_colour, command=Show_Document)
    Datacrawler_option.place(x=40, y=240)"""
'''

