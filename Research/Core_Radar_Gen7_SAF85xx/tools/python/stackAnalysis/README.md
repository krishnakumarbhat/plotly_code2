# stackAnalysis readme

This tool assists with determining the maximum stack usage via static analysis.
One of the major problems with this type of analysis is that the compiler is
often not able to determine what functions might be called when function
pointers or longjmps are used.
To support these use cases, this tool takes a configuration ini file as input.
This configuration file can describe the heirarchy of indirect function calls.
The tool will then evaluate each indirect call as its own entry point and use
the maximal stack size of the child branches as the value to propagate upwards.

## Supported Toolchains

Currently, only xtensa bbe32 is supported, but the goal is to be modular and
support other analysis methods for different toolchains in the future.
The tool used to analyze the stack is xt-stack-usage. This tool expects a .elf
file as input.

## Command Line Parameters

There are two positional parameters used for the command line:
 * configuration file
   - this describes further configuration to be applied
 * input file
   - this is the file that will be processed by the tool
   - it may be a .elf or a .map file, for example, depending on the toolchain
   - currently, xt-stack-usage expects a .elf file

Example:
```sh
python stackAnalysis.py bbe32App_config.ini ../../../bazel-bin/outputs/flr7/bbe32App.elf
```

There are also some optional command line parameters:
 * -r, --exec_root EXEC_ROOT
   - specify the bazel exec root
   - many of the paths used by the tool will be relative to this location
   - if not specified, it will be queried using `bazel info`
 * -o, --output REPORT_FILE
   - specify an output file where a report will be written

Additionaly, there are a few utility options:
 * -v, --verbose
   - enable verbose debug output
 * -h, --help
   - display usage information

## Configuration File Syntax

The configuration file is a simple .ini file. However, since the python
configParser is used, [extended interpolation](https://docs.python.org/3/library/configparser.html#interpolation-of-values) is available.

```ini
[config]
# optional (default main): define the toplevel entry point
entry_point = main

# optional: list of category 1 interrupts (hardware/non-rtos interrupts that
# can occur at any time and use whatever stack is currently active)
# multiline entries are supported as long as they are indented
cat1_interrupts:
  my_int_handler

# optional (default 1 GB): specify a maximum stack size target
# if this is exceeded, the script will exit with an error code
# max_stack = 0x3fffffff

# optional (default 0): specify a fixed amount to add to the final computed
# stack size
# this can help if there is some stack already used before your main entry
# point or if you want to include some additional buffer
# extra_stack = 0

# required: specify the toolchain executable to use to perform the analysis
# currently only supported value is 'xt-stack-usage'
tool = xt-stack-usage

# specify the toolchain path where the tool can be found
# this will be relative to the execroot
tool_path_prefix_linux = external/bbe_linux
tool_path_prefix_windows = external/bbe_windows
tool_path_linux = ${tool_path_prefix_linux}/tools/RI-2021.7-linux/XtensaTools/bin
tool_path_windows = ${tool_path_prefix_windows}/tools/RI-2021.7-win32/XtensaTools/bin

# these options are specific to xt-stack-usage
xtensa_core = spt_bbe32_vfpu_gold_r1p3
xtensa_system_linux = ${tool_path_prefix_linux}/build/RI-2021.7-linux/${xtensa_core}/config
xtensa_system_windows = ${tool_path_prefix_windows}/build/RI-2021.7-win32/${xtensa_core}/config

# the entry point and each indirect branch target can have their own section
[main]
# specify any additional indirect branches (function pointers or longjmp targets)
# entries are separated by whitespace
# multiline entries are supported as long as they are indented
indirect_calls:
    my_indirect_func_foo
    my_indirect_func_bar

[my_indirect_func_bar]
# example: additional stack can be specified just for this subtree
extra_stack = 1024

# example: further indirect calls can be specified
indirect_calls: my_indirect_func_baz

[my_indirect_func_baz]
# it is ok to have empy section labels

# likewise, it is allowed to skip section labels that would be empty
# as was done for _foo here
```
