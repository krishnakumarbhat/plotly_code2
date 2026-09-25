from tkinter import *
from tkinter import messagebox
from tkinter import filedialog
from Main import *
from main_interface import *
from Data_Crawler_Execution import *

global log_opt
global run_data_crawler
global clear_log_entry
global help_mail

global log_list
log_list = []


def call_all_data_crawler_methods():
    browse_log_files()
    display_labels()
    Tool_info()
    buttons_data_crawler()
    return


"""display_labels()
Tool_info()"""


def browse_log_files():
    global Datacrawler_Groupbox
    Datacrawler_Groupbox = Canvas(height=620, width=1200, bg=Bg_colour)
    Datacrawler_Groupbox.pack()

    Datacrawler_Groupbox.create_rectangle(25, 40, 1180, 95, fill=None)
    Label(text='CREATE JSON/FLIST', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=30, y=30)
    """To Create Browsing Log File option for files"""
    global log_label
    global log_button
    global log_opt
    log_label = Label(text='Logs: ', bg=Bg_colour, font=('arial', 10, 'bold')).place(x=30, y=60)
    log_opt = Entry()
    if platform == 'linux' or platform == 'linux2':
        log_opt.config(width=125)
    else:
        log_opt.config(width=166)
    log_opt.place(x=80, y=60)

    def browse_logs():
        logs = filedialog.askopenfilenames(title='Select Logs',
                                           filetypes=(("MF4 Files", "*.mf4"), ("All files", "*.*")))
        for a in logs:
            log_list.append(a)
        log_opt.insert(END, logs)

    log_button = Button(text='Browse', command=lambda: browse_logs()).place(x=1100, y=55)


def clear_entries():
    log_opt.delete(0, END)


def execute_data_crawler():
    if len(log_list) >= 1:
        d_c_flist_gen(log_list)
        execute_data_crawler_command()
    else:
        messagebox.showerror(title='Logs', message="!!!Please select the logs")


def buttons_data_crawler():
    global clear_log_entry
    global run_data_crawler
    global help_mail

    clear_log_entry = Button(text="HOME", font=('arial', 10, 'bold'), height=2, width=17,
                             command=clear_entries).place(x=500, y=180)
    run_data_crawler = Button(text="RUN DATA CRAWLER", font=('arial', 10, 'bold'), height=2, width=17,
                              command=execute_data_crawler).place(x=500, y=120)
    help_mail = Label(root, text="Help : chalapathi.kalyan.donepudi@aptiv.com",
                      font=('arial', 8, 'bold'), bg=Bg_colour).place(x=940, y=556)

