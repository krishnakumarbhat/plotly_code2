from guizero import App, Combo, Text, CheckBox, ButtonGroup, PushButton, info, TextBox, Picture
import os
import time
import sys

from io import StringIO


def counter_loop():
    import Converter_Tool_XML_writer

    old_stdout = sys.stdout

    # This variable will store everything that is sent to the standard output

    result = StringIO()

    sys.stdout = result
    Converter_Tool_XML_writer.Execute_Converter_Command()
    # sys.stdout = old_stdout
    result_string = result.getvalue()
    counter.value = result_string  # read output

    button.disable()


app = App(title="get data", width=1000, height=1000)
counter = Text(app, font=('arial', 8), color="black")
button = PushButton(app, command=counter_loop, text="Display")

app.display()