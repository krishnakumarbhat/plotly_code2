__author__      = 'Devin K. Jaenicke'
__version__     = '1.1.0'
__email__       = 'devin.k.jaenicke@aptiv.com'
__copyright__   = 'Copyright 2017 Aptiv, All Rights Reserved.'

import inspect
import os

os.system('color FF')

def colored(r, g, b, text):
    return f"\033[38;2;{r};{g};{b}m{text} \033[38;2;255;255;255m"


def print_green(text):
        text = colored_text = colored(15, 218, 15, text)
        print (text)

def print_violet(text):
        text = colored_text = colored(164, 46, 172, text)
        print(text)

def print_red(text):
        text = colored_text = colored(250, 15, 15, text)
        print(text)

def print_yellow(text):
        text = colored_text = colored(243, 228, 14, text)
        print(text)


class Trace:

    @staticmethod
    def print_error(e_message, exit_on_complete=False):
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])

        # Only add the module name if not called from base script
        if mod and mod.__name__ != '__main__':
            print_red("Error: %s.py -> %s" % (mod.__name__, e_message))
        else:
        	print_red("Error: %s" % e_message)
        if exit_on_complete:
            exit(1)

    @staticmethod
    def print_error_header(e_message, exit_on_complete=False):
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])

        # Only add the module name if not called from base script
        if mod and mod.__name__ != '__main__':
            temp_string = ' ERROR: ' + mod.__name__ + '.py ' + e_message + ' '
        else:
            temp_string = ' ERROR: ' + e_message + ' '

        print("\n")
        print('*')
        print('*')
        print('*')
        print('*')
        print('********************************************************************************************')
        print(temp_string.center(92, '*'))
        print('********************************************************************************************')
        print("\n")
        if exit_on_complete:
            exit(1)

    @staticmethod
    def print_header(message):
        temp_string = ' ' + message + ' '
        print_violet("\n")
        print_violet('*')
        print_violet('*')
        print_violet('*')
        print_violet('*')
        print_violet('********************************************************************************************')
        print_violet(temp_string.center(92, '*'))
        print_violet('********************************************************************************************')
        print_violet("\n")

    @staticmethod
    def print_info(i_message):
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])

        # Only add the module name if not called from base script
        if mod and mod.__name__ != '__main__':
            print_green("Info: %s.py -> %s" % (mod.__name__, i_message))
        else:
            print_green("Info: %s" % i_message)

    @staticmethod
    def print_warning(w_message):
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])

        # Only add the module name if not called from base script
        if mod and mod.__name__ != '__main__':
            text = "Warning: %s.py -> %s" % (mod.__name__, w_message)
            print_yellow(text)
        else:
            text = "Warning: %s" % w_message
            print_yellow(text)

###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/       JIRA AAA-#### for release x.x.x.
#             Initials    Explanation of changes done here.
#    Date        By             Description
# ----------  ---------   -----------------------
# 12/18/2017  Devin J.    APS-0603  Initial creation
# 12/18/2017  Devin J.    APS-0694  Added calling module to print statements
# 02/06/2018  Devin J.    APS-2366  Replaced references to delphi with aptiv
# 11/20/2018  Tim B.      APS-14367 Ensure 'mod' exists before calling it.
# 08/22/2019  Jan S.      APS-56064 Use Python3 style prints
#
###############################################################################
