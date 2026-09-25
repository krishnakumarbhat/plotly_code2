from tkinter import *
# import webbrowser
from tkinter import scrolledtext
from bs4 import BeautifulSoup
import io
from Main import *
from main_interface import *

text_list = []
def release_notes_call():
    text_file = io.open('Release_Notes.txt', 'r')  # or tf = open(tf, 'r')
    global data
    data = text_file.read()
    text_file.close()
    '''f = io.open('Release_Notes.html', 'w')
    html_content = """<html><head></head><body><p>Hello World!</p></body></html>"""
    f.write(html_content)
    # webbrowser.open_new_tab('Release_Notes.html')
    f.close()

    with io.open('Release_Notes.html') as fp:
        soup = BeautifulSoup(fp, 'html.parser')
    result = soup.find_all('p')[0].get_text()'''
    print_html()
    return


def print_html():
    global output_text_release
    global text_box_release_label
    text_box_release_label = Label(text="RELEASE NOTES", font=('arial', 20, 'bold'), bg='slate gray1')
    text_box_release_label.place(x=450, y=20)

    output_text_release = scrolledtext.ScrolledText(root, height=20, width=115)
    output_text_release.place(x=20, y=70)

    output_text_release.insert(END, data)
    output_text_release.configure(state=DISABLED)
    return
