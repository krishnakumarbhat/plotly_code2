"""
Parse ARXML file and generate Signal COMPU-METHODS properties for LINEAR type computation.
"""
import xml.etree.ElementTree as ET

# Define the input and output file names
input_file = "gen7_rdr_communication_extract_v2_0.arxml"
output_file = "someip_compu_methods.h"

# Parse the ARXML file
tree = ET.parse(input_file)
root = tree.getroot()

# Define the AUTOSAR namespace
ns = {"ns": "http://autosar.org/schema/r4.0"}

# Prepare header content
header_content = """#ifndef SOMEIP_COMPU_METHODS_H
#define SOMEIP_COMPU_METHODS_H
/*===========================================================================*/
/**
 * @file someip_compu_methods.h
 *
 * This file contains attributes of COMPU-METHODs from the ARXML as macros
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
 * are all captured from ARXML as a macro here
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

# Prepare footer content
footer_content = """/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/
#define MIN_MAX_BOUNDARY_LIMIT(x, sig)   (((x)<(sig##_MIN_ETH)) ? (sig##_MIN_ETH) : (((x)>(sig##_MAX_ETH)) ? (sig##_MAX_ETH) : (x)))

#define INTERNAL_TO_PHYSICAL_CONVERSION(sig_val, sig) (((sig_val) * (sig##_FACTOR_ETH)) + (sig##_OFFSET_ETH))

#define PHYSICAL_TO_INTERNAL_CONVERSION(sig_phy, sig) (((sig_phy) - (sig##_OFFSET_ETH)) / (sig##_FACTOR_ETH))

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
#endif  /* SOMEIP_COMPU_METHODS_H */

/*===========================================================================*
 * File Revision History (top to bottom: first revision to last revision)
 *===========================================================================*
 *
 *    Date        ID      (Description on following lines: SCR #, etc.)
 * -----------  ------   ---------------------------------------------
 * 4-Sep-2025  xjvkxp    Script to extract compu methods into a header file
 * ==========================================================================*/"""

# Open the output header file
with open(output_file, "w") as header_file:
    # write the header
    header_file.write(header_content)
    # Iterate through all COMPU-METHOD elements
    for compu_method in root.findall(".//ns:COMPU-METHOD", ns):
        category = compu_method.find("ns:CATEGORY", ns)
        if category is not None and category.text == "LINEAR":
            # Get the name of the Computation Method
            short_name = compu_method.find("ns:SHORT-NAME", ns)
            if short_name is None:
                continue
            name = short_name.text

            # Initialize values
            offset = factor = min_val = max_val = None

            # Extract OFFSET and FACTOR from COMPU-NUMERATOR
            numerator = compu_method.find(".//ns:COMPU-NUMERATOR", ns)
            if numerator is not None:
                v_elements = numerator.findall("ns:V", ns)
                if len(v_elements) >= 2:
                    offset = v_elements[0].text
                    factor = v_elements[1].text

            # Extract MIN and MAX from COMPU-SCALE
            compu_scale = compu_method.find(".//ns:COMPU-SCALE", ns)
            if compu_scale is not None:
                lower_limit = compu_scale.find("ns:LOWER-LIMIT", ns)
                upper_limit = compu_scale.find("ns:UPPER-LIMIT", ns)
                if lower_limit is not None:
                    min_val = lower_limit.text
                if upper_limit is not None:
                    max_val = upper_limit.text

            # Write the comment and macros to the header file
            header_file.write(f"/* Computation Method for signal: {name} */\n")
            if factor is not None:
                header_file.write(f"#define {name}_FACTOR_ETH ({factor})\n")
            if offset is not None:
                header_file.write(f"#define {name}_OFFSET_ETH ({offset})\n")
            if min_val is not None:
                header_file.write(f"#define {name}_MIN_ETH ({min_val})\n")
            if max_val is not None:
                header_file.write(f"#define {name}_MAX_ETH ({max_val})\n")
            header_file.write("\n")
    # write the footer
    header_file.write(footer_content)
