# SFL Calibration Tool

## Calibration .c- and .h-file generation based on calibration xml files

This python script (ct_main.py) generates so called core files (calibration header and printing of cals) and
also customer dependent calibration c-files and .xml-datastreams for flashing calibrations on an embedded hardware.

## Why should I use this project ?

It generates misra compliant calibration file code and is easily maintainable as well as extendable.

## Setup

You need `python>=3.7` to run this script.

## Prerequisites?

### Folder layout

The calibration tool assumes the folder structure of a module as shown below.
```
Module
├── Calibration                          # Calibration folder within a module
│   ├── Customer_A                       # Folder for customer specific cals of customer A
│   │   └── Customer_Specific_Cal.xml    # xml file which contains values of cals to overwrite for customer A
│   ├── Module_Core                      # Folder which contains the definition of the set of cals
│   │   └── module_cal.xml               # xml file which contains the super set of all cals.
│   ├── Customer_B                       # Folder for customer specific cals of customer B
│   │   └── Customer_Specific_Cal.xml    # xml file which contains values of cals to overwrite for customer B
│   ├── ...                              # As many other customers as you like to deliver to
│   └── Customer_Z                       # Folder for customer specific cals of customer Z
│       └── Customer_Specific_Cal.xml    # xml file which contains values of cals to overwrite for customer Z
└── ...
```
A module called Module shall have a Calibration folder. Within this calibration folder different subfolders named like the customers, for which calibrations shall be generated, are located.
Each customer folder shall contain a Customer_Specific_Cal.xml in which the calibrations can be overwritten to customer specific values.
The module itself shall contain a folder Module_Core in which calibrations can be registered.

## Content of the files

### module_cal.xml:

Some prerequisites are required in this cal file. General information in the xml element "CALIBRATION_SCHEMA" is needed as shown
below. This includes especially a list of the customers for which the calibration tool shall be executed.
This list is connected with the customer folders as shown in the section "Folder Layout".
```XML
<?xml version="1.0" encoding="utf-8"?>
<data-set xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <section><!-- This section is required by the calibration tool! --></section>
  <CALIBRATION_GENERIC_INFO>
    <BASIC_TYPE_INCLUDE_FILE>reuse.h</BASIC_TYPE_INCLUDE_FILE>
    <COMPONENT_NAME>Ff</COMPONENT_NAME>
    <CUSTOMERS>[Customer_A Customer_B Customer_C ... Customer_Z]</CUSTOMERS>
    <EXPORT_ASIL_CHECK>True</EXPORT_ASIL_CHECK> <!-- True | False -->
  </CALIBRATION_GENERIC_INFO>
  <section><!-- In this section calibration internal properties can be overwritten. This is currently only used for versioning. --></section>
  <CALIBRATION_HEADER>
    <record>
      <NAME>version</NAME>
      <VALUE>21<VALUE>
    </record>
  </CALIBRATION_HEADER>
  <section><!-- This section is for the definitions of component calibrations --></section>
  <CALIBRATION_COMPONENT>
      <record>
      <NAME>k_calibration_1</NAME>
      <DESCRIPTION>This is a cool calibration owned by the component.</DESCRIPTION>
      <Comp>Component</Comp>
      <UNIT>unit of k_calibration_1</UNIT> <!-- Unit is optional and can be omitted in case that no meaningful unit is given. -->
      <DATA_TYPE>data type of k_calibration_1</DATA_TYPE>
      <DATA_RESOLUTION>resolution of k_calibration_1</DATA_RESOLUTION>
      <RANGE_MIN>range minimum of k_calibration_1</RANGE_MIN>
      <RANGE_MAX>range maximum of k_calibration_1</RANGE_MAX>
      <DEFAULT_VALUE>default value of k_calibration_1</DEFAULT_VALUE>
      <CONSTANT>true</CONSTANT><!-- True | False -->
    </record>
    ...
  </CALIBRATION_COMPONENT>
</data-set>
```
The second section shows how to add a calibration to the xml sheet. The xml elements shown here are required for the xml parser in the calibration tool.

#### Content of generic calibration information

| Xml-tag | Description |
| --- | --- |
| **BASIC_TYPE_INCLUDE_FILE** | Specifies the file in which basic datatype definitions are given. This is dependent on the underlying component. |
| **COMPONENT_NAME** | Specifies the component name. This string is used for function prefixes of functions and macros to be generated. |
| **CUSTOMERS** | Specifies the customers of a component for which calibrations shall be generated. Keep in mind that this should contain the complete supported customer list of a given component as soon as core files are adapted |
| **EXPORT_ASIL_CHECK** | Enable/Disable a initialization check for the underlying component. The function generated is recommended to be called at the start of the ignition cycle by the component itself before any changes to the calibration parameters are applied. It returns true when all given customer specific values are set to the respective calibrations |


### Customer_Specific_Cal.xml:
Let's assume for the Customer_Specific_Cal.xml to generate one for customer_X. The general format of this file is given below.
Overwriting cal values is optional but assigning different values to cals can be accomplished to simply list the calibration
name as well as another value to it as shown below.
```XML
<?xml version="1.0" encoding="utf-8"?>
<data-set xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <CALIBRATION_COMPONENT>
    <record>
      <NAME>k_calibration_1</NAME>
      <VALUE>overwrite_to_this_fantasy_value_1_for_customer_X</VALUE>
    </record>
    ...
    <record>
      <NAME>k_calibration_2</NAME>
      <VALUE>overwrite_to_this_fantasy_value_2_for_customer_X</VALUE>
    </record>
  </CALIBRATION_COMPONENT>
</data-set>
```

### Format for special data structures:

If you want to create a one dimensional array, you can do so with the following format in the default value field:
```XML
    <record>
      <DEFAULT_VALUE>[a11 a12 a13 a14 a15 a16]</DEFAULT_VALUE>
    </record>
```
This will resolve in an array with 6 elements like array[6]={a11, a12, a13, a14, a15, a16};

In case of a two dimensional array, the following format should be applied:
```XML
    <record>
      <DEFAULT_VALUE>[[a11 a12 a13 a14][a21 a22 a23 a24][a31 a32 a33 a34]]</DEFAULT_VALUE>
    </record>
```
This will resolve in an array like array[3][4]={{a11, a12, a13, a14},{a21, a22, a23, a24},{a31, a32, a33, a34}}. Or when considering
the memory layout of C (Row-Major-Order): a11, a21, a31, a12, a22, a32, a13, a23, a33, a14, a24, a34.

## How to integrate calibration tool

### A cmake sample integration
The integration strategy is that the xlink can be set directly to CT_CalibrationTool\c_src subfolder such that projects
can be unaware of python source code as well as testing of the respective release.
Let's assume that you are xlinking against the 'c_src' folder such that its content is at a project specific folder name
'CalibrationTool'. Another assumption is that a target exists (in the following called SRR_CORE_LIB) which is containing 
standard type definitions (float32_T, uint64_t, int64_t, uint32_t, int32_t, uint16_t, int16_t, uint8_t, int8_t, boolean_T).
With this assumption a sample cmake integration could look like this:

```CMake
# Include SRR_CORE_LIB target (reuse.h) with path pointing to it.
add_subdirectory("${PROJECT_ROOT_PATH}/<path_to_srr_core_lib>" "${CMAKE_CURRENT_BINARY_DIR}/SRR_CORE_LIB")

# Include calibration specific sources (ct_endianness_switch.h, ct_calibration_header_t.h)
add_subdirectory("${PROJECT_ROOT_PATH}/<path_to_sources>" "${CMAKE_CURRENT_BINARY_DIR}/Calibration_Tool")

# Link Calibration tool to SRR_CORE_LIB for reuse.h
target_link_libraries(CT_CALTOOL SRR_CORE_LIB)
```

Since the target containing reuse.h can differ across projects, the calibration tool is not linking against
the target SRR_CORE_LIB internally. This needs to be accomplished by the project.

Ensure that every component which is using the calibration tool is added afterwards since they might link on their own
to Calibration tool.

### Supported configurations
When using the Calibration tool several macros can be activated. Those are summarized in the tuple \{CT_INITIALIZE_COMPONENT_CALIBRATION, CT_BIG_ENDIAN, CT_ACTIVATE_CAL_PRINT\}.
They can be actived/deactivated in every possible permutation of this tuple.

| Macro | Description |
| --- | --- |
| **CT_INITIALIZE_COMPONENT_CALIBRATION** | Set this macro in case that the update routine should not be called and components calibration shall already be initialized out of the box. |
| **CT_BIG_ENDIAN** | Set this macro explicitly in case that a big endian embedded system is given. This enables another order of the calibration typedef such that the big endian datastream of the customer specific datastream.xml is matching to the underlying datatype. |
| **CT_ACTIVATE_CAL_PRINT** | Set this macro for additional diagnostics. This should not be set for embedded devices, since stdio is included and is strongly dependent on the underlying system. If you feel like you need more diagnostics in resimulation, define this macro. |

**Note**: Default behavior is little endian and users need to update the components calibration on their own!

# Developer information

## How to run?

You can run the script from the command line using
```
python ct_main.py <<<path_to_main_cal_xml_sheet>>>
```
with <<<path_to_main_cal_xml_sheet>>> pointing to your main calibration xml sheet.
Or you can use the executable in \dist folder directly via command line like
```
<<<path_to_calibration_tool>>>\dist\ct_main.exe <<<path_to_main_cal_xml_sheet>>>
```

## How to build an executable?

For generation of a newer calibration tool, pyinstaller shall be called from a virtual environment.
For this the package venv needs to be installed.
```
pip install virtualenv
```
After this a virtual environment needs to be created with
```
python -m venv env
```
and activated with
```
.\env\Scripts\activate
```
for Windows or
```
source env/bin/activate
```
in Linux.
Within the virtual environment you are independent on all the system packages and need to install the needed
packages on your own. In order to build the cal tool you need to gather the required packages first:
```Python
python -m pip install -r requirements.txt

```
For building of an executable you use this command
```Python
pyinstaller ct_main.spec
```
or in case that ct_main.spec is not existing,
```Python
pyinstaller --onefile ct_main.py
```

In case of error "Failed to execute script ''pyi_rth_pkgres":
```
pip uninstall pyinstaller
pip install https://github.com/pyinstaller/pyinstaller/archive/develop.zip
```

## How to execute the pytest testsuite

Unit tests for the calibration tool are located in the testing folder.
A requirement.txt is also given here which can be installed via pip.
In order to run the unit tests with coverage information the following call can be used:
```Python
pytest --cov=python_src --cov-branch testing/python_testing/
```

More information on pytest can be found here https://docs.pytest.org/en/7.1.x/contents.html

## How to execute static code analysis
The required package (pylint) for the static code analysis is included in the requirements.txt from the testing folder.
The analysis can be invoked via command line with
```Python
pylint --rcfile=pylintrc python_src python_src/file_skeletons testing/python_testing/
```
The linter config file is inspired by https://google.github.io/styleguide/pyguide.html and slightly modified e.g.
- 80->160 characters per line as a limit
- indentation for every 4 character
- regular expressions for variables and arguments allow leading underscore.

## How to generate a docstring documentation
The requirements for documentation generation is located in ./docs/requirements.txt.As a prerequisite it is assumed that you are
currently located in the ./docs/ folder with your command line. For installation of the required packages execute
```Python
python -m pip install -r requirements.txt
```
from inside the ./docs folder and also the requirements.txt of the testing environment (plus the requirements for the codebase itself) since Sphinx is compiling the codebase for 
documentation generation purpose. For the documentation generation
```Python
sphinx-build -b html .\source\ .\build\
```
is called. Based on the sources you are building an html documentation in the ./build/ folder. The main file of the generated documentation is
called ./build/index.html.

## Copyright and author

Copyright (Python) 2023 Aptiv. All rights reserved.
Author: SFL (Side Feature Logic) scrum team
