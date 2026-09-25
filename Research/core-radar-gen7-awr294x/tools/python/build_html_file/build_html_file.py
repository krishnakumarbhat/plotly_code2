"""Build Html File.

This module contains all the classes needed to build a basic HTML file for reporting
information from the build scripts or integration testing. It contains a custom
implementation of an HTML builder, which can be customized as needed to add HTML functionality.
"""

import sys
import os
from os.path import abspath, join, dirname
import webbrowser
import base64


#####################################
# Insert local directories into path
#####################################
BASE_PATH = dirname(abspath(__file__))
sys.path.insert(0, abspath(join(BASE_PATH, "..", "print_trace")))


#####################################
# CUSTOM LIBRARIES
#####################################
from print_trace import Trace  # noqa: E402

#####################################

# Create python script header
__author__ = "Luke Welch"
__version__ = "1.0.0"
__email__ = "luke.welch@aptiv.com"
__copyright__ = "Copyright 2018 Aptiv, All Rights Reserved."


#####################################
# CLASSES
#####################################
class Row(object):
    """
    This class is used to specify the contents of a row in a table.

    It is then added to the table via the add_row_to_table or add_header_row_to_table methods
    in the BuildHTMLFile class.

    :param list cell_text_list: A list containing all the text that will be placed in each cell
        in the row.

    :param list color_list: A list of the color the text per cell in the row
        should be (ie. Black, Red, Green)

    :param list align_list: A list of how to align the text in the cell (ie. left, right, center)
    """

    def __init__(self, cell_text_list, color_list, align_list=None):
        """
        Init method.

        """
        if len(cell_text_list) != len(color_list):
            Trace.print_error_header(
                "Number of colors in list must be equal to number of cells!", True
            )
        self.cell_text_list = cell_text_list
        self.color_list = color_list

        if align_list is None:
            self.align_list = ["left"] * len(cell_text_list)
        else:
            self.align_list = align_list


class Link(object):
    """
    This class is used to specify the URL or path and text to display for a link.

    It is then added to the HTML file using the method add_link in the BuildHTMLFile class.

    :param string link_path: The URL or file path to create a link for.

    :param string display_text: The text the link should display in the HTML file

    """

    def __init__(self, link_path, display_text):
        """Initialize the links."""
        self.link_path = link_path
        self.display_text = display_text
        if not os.path.exists(link_path):
            Trace.print_info("Cannot find path to Link at {0}! Link Omitted".format(link_path))
            self.link_path = ""
            self.display_text = "Linked Doc Not Found At {0}!".format(link_path)
        if display_text.rstrip() == "":
            Trace.print_info("Display Text String is Null.")
            self.display_text = link_path


class BuildHTMLFile(object):
    """
    The BuildHTMLFile class is used to generate an HTML using the included functions.

    :param string output_file_path: The absolute output path to where the html file
        should be stored.  This includes the HTML file name.

    """

    def __init__(self, output_file_path):
        """Initialize the build html class."""
        self.output_file_path = output_file_path
        self.num_columns = 0
        self.html_code = ["<!DOCTYPE html>"]
        self.html_code.append("<html>")
        self.table_in_progress = False
        self.body_in_progress = False
        self.plot_report_in_progress = False

    def add_title(self, title_string):
        """
        Add a title to the file.

        :param string title_string: Title of the file
        """
        if self.body_in_progress is True:
            Trace.print_error_header("Body in Progress. Cannot add header inside the Body.", True)
        self.html_code.append("<title>" + title_string + "</title>")

    def add_header(self, header_list):
        """
        Add a list of strings to a header section.

        :param list header_list: list of strings which should be added as headers
        """
        if self.body_in_progress is True:
            Trace.print_error_header("Body in Progress. Cannot add header inside the Body.", True)
        self.html_code.append("<header>")
        for element in header_list:
            self.html_code.append("<b>" + element + "</b>")
        self.html_code.append("</header>")

    def add_text(self, text_string, bold=False):
        """
        Add a text string.

        :param string text_string: a string which should be added as text to the HTML file
        :param boolean bold: True to make the text bold.
        """
        self.html_code.append("<p>")
        if bold is True:
            self.html_code.append("<b>")
        self.html_code.append(text_string)
        if bold is True:
            self.html_code.append("</b>")
        self.html_code.append("</p>")

    def add_image(self, image_path):
        """
        Add an image to the HTML file.
        """
        data_uri = base64.b64encode(open(image_path, "rb").read()).decode("utf-8")
        img_tag = '<img src="data:image/png;base64,{0}">'.format(data_uri)
        self.html_code.append(img_tag)

    def add_hor_line(self):
        """
        Add a horizontal line break to the HTML file.
        """
        self.html_code.append("<hr>")

    def start_body(self):
        """
        Start the body section of the HTML.

        The body is where tables and primary text should occur.
        """
        self.html_code.append("<body>")
        self.body_in_progress = True

    def end_body(self):
        """
        End the body section of the HTML.
        """
        self.body_in_progress = False
        self.html_code.append("</body>")

    def create_table(self, num_columns):
        """
        Start the creation of a table.

        :param int num_columns: The number of columns you need in your table.
        """
        if self.table_in_progress is True:
            Trace.print_error_header("Can only build one HTML table at a time!", True)
        self.num_columns = num_columns
        self.html_code.append("<table border=3>")
        self.table_in_progress = True

    def add_dividor_row_to_table(self, row_class):
        """
        Add a dividor row to the HTML file.
        """
        if self.table_in_progress is not True:
            Trace.print_error_header(
                "Table Not Created! Must \
                create table before adding a row",
                True,
            )
        if not isinstance(row_class, Row):
            Trace.print_info("Input was not a Row Class type!  Row Omitted")
        else:

            self.html_code.append("<tr>")

            self.html_code.append(
                '<th colspan="{0}" style="color: {1}">'.format(self.num_columns, "Black")
                + row_class.cell_text_list[0]
                + "</th>"
            )
            self.html_code.append("</tr>")

    def add_header_row_to_table(self, row_class):
        """
        Add a header row to a table.

        A table must be started before this function can be called.

        :param Row row_class: Instance of the Row class which
            has the cell contents for the header row
        """
        if self.table_in_progress is not True:
            Trace.print_error_header(
                "Table Not Created! Must \
                create table before adding a row",
                True,
            )
        if not isinstance(row_class, Row):
            Trace.print_info("Input was not a Row Class type!  Row Omitted")
        else:
            if self.num_columns != len(row_class.cell_text_list):
                Trace.print_error_header(
                    "Number of cells in Row class does not match number \
                    of cells defined in the table!",
                    True,
                )

            self.html_code.append("<tr>")
            for i in range(len(row_class.cell_text_list)):
                self.html_code.append(
                    '<th style="color: {0};background-color:{1}">'.format(
                        row_class.color_list[i], "Gray"
                    )
                    + row_class.cell_text_list[i]
                    + "</th>"
                )
            self.html_code.append("</tr>")

    def add_row_to_table(self, row_class, bg_color_sel=0):
        """
        Add a header row to a table.

        A table must be started beforethis function can be called.

        :param Row row_class: Instance of the Row class which
            has the cell contents for the header row
        """
        if bg_color_sel == 1:
            bgColor = "#fff2e6"  # E0F4EB"
        elif bg_color_sel == 2:
            bgColor = "#e6f2ff"
        elif bg_color_sel == 3:
            bgColor = "#c4c4c4"
        else:
            bgColor = "white"

        if self.table_in_progress is not True:
            Trace.print_error_header(
                "Table Not Created! \
                Must create table before adding a row",
                True,
            )
        if not isinstance(row_class, Row):
            Trace.print_info("Input was not a Row Class type!  Row Omitted")
        else:
            if self.num_columns != len(row_class.cell_text_list):
                Trace.print_error_header(
                    "Number of cells in Row class does not match \
                    number of cells defined in the table!",
                    True,
                )

            self.html_code.append("<tr>")
            for i in range(len(row_class.cell_text_list)):
                # self.html_code.append("<td style=\"color:{0}\" align=\"{1}\">"
                #                       .format(row_class.color_list[i], row_class.align_list[i]) +
                #                       row_class.cell_text_list[i] + "</td>")
                self.html_code.append(
                    '<td style="color:{0};background-color:{1}" align="{2}">'.format(
                        row_class.color_list[i], bgColor, row_class.align_list[i]
                    )
                    + row_class.cell_text_list[i]
                    + "</td>"
                )
            self.html_code.append("</tr>")

    def close_table(self):
        """
        End the creation of a table.
        """
        if self.table_in_progress is not True:
            Trace.print_info("No Open Table!")
        else:
            self.html_code.append("</table>")
            self.table_in_progress = False
            self.num_columns = 0

    def add_link(self, link_class):
        """
        Add a link to the HTML file.

        :param Link link_class: Instance of the Link class which
            has all the necessary information to create the link.
        """
        if isinstance(link_class, Link):
            self.html_code.append(
                '<a href="{0}">'.format(link_class.link_path) + link_class.display_text + "</a>"
            )
            self.html_code.append("<br>")
        else:
            Trace.print_info("Input was not a Link Class type!  Link Omitted")

    def write_html_to_file(self):
        """
        Write the built HTML file to its path.
        """
        self.html_code.append("</html>")
        # Join list into single string for writing to the HTML file
        str_html = "\n".join(self.html_code)

        # Write to the file
        report_html = open(self.output_file_path, "w")
        Trace.print_info("HTML Report written at {0}".format(self.output_file_path))
        report_html.write(str_html)
        report_html.close()

    def open_html_file(self):
        """
        Open the built HTML file to its path.

        The HTML file must be generated first.
        """
        if os.path.isfile(self.output_file_path):
            webbrowser.open(self.output_file_path)
        else:
            Trace.print_error_header("Cannot find the HTML file. Did you write it to a file?")


###############################################################################
#                               Revision History
###############################################################################
#
# MM/DD/YYYY  Name/       JIRA AAA-####
#             Initials    Explanation of changes done here.
#    Date        By             Description
# ----------  ---------   -----------------------
# 07/02/2018  Luke W.     APS-6809 Initial Creation
#
###############################################################################
