"""Parse the exported object file of a CAN dbc and create a can_signals.h header file."""

######################################################
#  This script parses the exported object file of CAN
#  dbc and creates a header file
######################################################

import sys

obj_list = sys.argv[1]
outfile = "can_signals.h"

header_info = """#ifndef CAN_SIGNALS_H
#define CAN_SIGNALS_H
/*===========================================================================*/
/**
 * @file can_signals.h
 *
 * This file contains attributes of all the signals from the dbc as macros
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * The attributes of a signal e.g. MAX/MIN/OFFSET/INIT/Signed/Unsigned etc.
 * are all captured from dbc as a macro here
 *
 * @section ABBR ABBREVIATIONS:
 *   - @todo List any abbreviations, precede each with a dash ('-').
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - @todo Update list of design document(s).
 *
 *   - Requirements Document(s):
 *     - @todo Update list of requirements document(s)
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - <a href="https://spo.aptiv.com/sites/0307-eoses/GlobalRegional/ESGW_4-2_PE-SWx_00-01-A02_EN.docx">
 *       ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]</a>
 *     - @todo Update list of other applicable standards
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - @todo List of deviations from standards in this file, or "None".
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 *
 * @defgroup template Provide API description and define/delete next line
 * @ingroup <parent_API> (OPTIONAL USE if part of another API, else delete)
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/

/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/
"""

footer_info = """
/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/
#define MIN_MAX_SATURATION(x, sig)   (((x)<(sig##_MIN)) ? (sig##_MIN) : (((x)>(sig##_MAX)) ? (sig##_MAX) : (x)))

#define CAN_TX_SIGNAL_CORRECTION(sig_phy, sig)   (((sig_phy) - (sig##_OFFSET)) / (sig##_FACTOR))

#define CAN_RX_SIGNAL_CORRECTION(sig_val, sig)   (((sig_val) * (sig##_FACTOR)) + (sig##_OFFSET))

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Const Object Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/

/*===========================================================================*
 * Exported Inline Function Definitions and #define Function-Like Macros
 *===========================================================================*/
#endif  /* CAN_SIGNALS_H */

/*===========================================================================*
 * File Revision History (top to bottom: first revision to last revision)
 *===========================================================================*
 *
 *    Date        ID     SCR    (Description on following lines: SCR #, etc.)
 * -----------  ------  -----   ---------------------------------------------
 * 23-Sep-2020  bz571t  DND-410: Script to extract signals into a header file
 * 09-Oct-2020  bz571t  DND-450: Improved the script
 * ==========================================================================*/

"""

prev_signal = ""


def process_signals(curr_signal, fields, out):
    """Write signal property macros to the output file, skipping duplicate consecutive signals."""
    global prev_signal
    if prev_signal != curr_signal:
        prev_signal = curr_signal

        sign_len = "(" + fields[1] + "U)"
        mask_val = "(" + str(2 ** int(fields[1]) - 1) + "U)"

        if "." in fields[5]:  # check for factor - is it fractional?

            def fmt_frac(v):
                return "(" + v + "F)" if "." in v else "(" + v + ".0F)"

            init_val = fmt_frac(fields[4])
            fact_val = fmt_frac(fields[5])
            ofst_val = fmt_frac(fields[6])
            mini_val = fmt_frac(fields[7])
            maxi_val = fmt_frac(fields[8])
        else:
            if float(fields[4]) < 0 or float(fields[7]) < 0 or float(fields[8]) < 0:
                # Init < 0 OR Min < 0 OR Max < 0
                init_val = "(" + fields[4] + ")"
                fact_val = "(" + fields[5] + ")"
                ofst_val = "(" + fields[6] + ")"
                mini_val = "(" + fields[7] + ")"
                maxi_val = "(" + fields[8] + ")"
            else:
                init_val = "(" + fields[4] + "U)"
                fact_val = "(" + fields[5] + "U)"
                ofst_val = "(" + fields[6] + "U)"
                mini_val = "(" + fields[7] + "U)"
                maxi_val = "(" + fields[8] + "U)"

        sign_len_line = "#define " + curr_signal + "_LENGTH   " + sign_len
        mask_val_line = "#define " + curr_signal + "_MASK   " + mask_val
        init_val_line = "#define " + curr_signal + "_INIT   " + init_val
        fact_val_line = "#define " + curr_signal + "_FACTOR   " + fact_val
        ofst_val_line = "#define " + curr_signal + "_OFFSET   " + ofst_val
        mini_val_line = "#define " + curr_signal + "_MIN   " + mini_val
        maxi_val_line = "#define " + curr_signal + "_MAX   " + maxi_val

        out.write("\n/* " + curr_signal + ", Unit: " + fields[9] + " */\n")
        out.write(sign_len_line + "\n")
        out.write(mask_val_line + "\n")
        out.write(init_val_line + "\n")
        out.write(fact_val_line + "\n")
        out.write(ofst_val_line + "\n")
        out.write(mini_val_line + "\n")
        out.write(maxi_val_line + "\n")


with open(obj_list, "r") as obj, open(outfile, "w") as out:
    out.write(header_info)

    curr_signal = ""

    for row in obj:
        fields = row.split(";")

        if fields[0].startswith("DET_"):
            if fields[0].startswith("DET_MH_"):
                # Excluded CRC and CYCLE_COUNTER signals
                if fields[0].startswith("DET_MH_SCAN_"):
                    curr_signal = fields[0][:-8]
                else:
                    curr_signal = ""
            else:
                # Detection signals
                # Read one signal per detection
                # Exclude the last 4 characters that contain numbers
                curr_signal = fields[0][:-4]
        elif fields[0].startswith("HED_"):
            if fields[0].startswith("HED_MH_"):
                # Excluded CRC and CYCLE_COUNTER signals
                curr_signal = ""
            else:
                # Header signals
                curr_signal = fields[0]
        elif fields[0].startswith("STS_"):
            if fields[0].startswith("STS_MH_") or fields[0].startswith("STS_XCP_"):
                # Excluded CRC and CYCLE_COUNTER signals
                # And XCP messages
                curr_signal = ""
            else:
                # Status signals
                curr_signal = fields[0]
        elif fields[0].startswith("SSTS_"):
            if fields[0].startswith("SSTS_MH_"):
                # Excluded CRC and CYCLE_COUNTER signals
                curr_signal = ""
            else:
                # Status signals
                curr_signal = fields[0]
        elif fields[0].startswith("OBJ_"):
            if fields[0].startswith("OBJ_MH_"):
                # Excluded CRC and CYCLE_COUNTER signals
                curr_signal = ""
            else:
                curr_signal = fields[0][:-4]
        elif fields[0].startswith("AL_"):
            if fields[0].startswith("AL_MH_"):
                # Excluded CRC and CYCLE_COUNTER signals
                curr_signal = ""
            else:
                curr_signal = fields[0]
        elif fields[0].startswith("CAP_"):
            if fields[0].startswith("CAP_MH_"):
                # Excluded CRC and CYCLE_COUNTER signals
                curr_signal = ""
            else:
                curr_signal = fields[0]
        else:
            pass  # retain curr_signal from previous iteration (matches Perl behavior)

        if curr_signal != "":
            process_signals(curr_signal, fields, out)

    out.write(footer_info)

# ===========================================================================*
# File Revision History (top to bottom: first revision to last revision)
# ===========================================================================*
#
#    Date        ID     Name    (Description on following lines: SCR #, etc.)
# -----------  ------  ------   ---------------------------------------------
# 24-Sep-2020  bz571t  Umesh    Initial version
# 09-Oct-2020  bz571t  Umesh    Signed/Unsigned warnings taken care
# 09-Oct-2020  bz571t  Umesh    Added saturation check macro
# 11-Mar-2026  xjvkxp  Mounir   Converted from Perl to Python
# ==========================================================================*
