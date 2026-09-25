# GENERATING STREAM DEFINITIONS
-------------------------

# Table of contents

1. [Introduction](#1-introduction)
2. [Requirements](#2-requirements)
3. [Generating stream def through GUI](#3-generating-stream-def-through-gui)
4. [Generating stream def through CLI](#4-generating-stream-def-through-cli)
5. [Integration into Bazel build](#5-integration-into-bazel-build)
6. [File Revision History](#6-file-revision-history)
---------------------

## 1. Introduction:

The stream def tool will generate a stream defenition text file when given a stream header and other information about the stream, like source, version and number. This stream def file is used to parse brr log files.

DvrlStreamTool v2.0.2 is the version being used.
This tool was known as StructMapGenerator in the Gen5 project.

---------------
## 2. Requirements:

Regardless of the method used below to generate the stream def file, the stream header and all #includes need to be in the same folder.

---------------
## 3. Generating stream def through GUI:

1. Open the DvrlStreamTool.exe.
2. Provide the path to the stream header file.
3. Once the above is selected, the structure drop down will populate with possible structure names.
   The desired structure will likely have a name that matches the file name with a "_T" ending, ex: Debug_Stream_T, Status_Stream_T
4. Select output directory
5. Populate the output file name section to include the source, stream, and version.

    Examples:
    |Sources| Number |
    | --- | :-:|
    | srr7p  | 204 |
    | srr7hd   | 205 |
    | flr7      | 206 |

    Streams:
    |Name| Number|
    | --- | :---: |
    |DETECTION |  1|
    |HEADER    |  2|
    | STATUS    |  3|
    | RDD      |  4|
    | CDC      |  6|
    | DEBUG     |  7|
    | MMIC     |  8|
    |ALIGNMENT |  9|
    | RFFT     | 11|
    | CALIB     | 14|

6. Leave the default user-define #define values
 PCRESIM, PC_RESIM, DVTOOL_MODIFICATION
7. Set default pack size, unknown type size, and "long" type size to 4 bytes
8. Emulate Tasking TriCore set to checked
9. Reverse stream only used on stream 14

---------------
## 4. Generating stream def through CLI:

The parameters for the command line are similar to the gui with the main difference being that
source,stream,and version information are combined into the desired filename.

Example 1:
DvrlStreamTool.exe -f debug_stream.h -s Debug_Stream_T -o outputDir/streamdef_src204_str001_ver005.txt -d PCRESIM=true,PC_RESIM=true,DVTOOL_MODIFICATION=true -p 4 --tasking true

Example 2:
DvrlStreamTool.exe -f debug_stream.h -s Calib_Stream_T -o outputDir/streamdef_src204_str014_ver001.txt -d PCRESIM=true,PC_RESIM=true,DVTOOL_MODIFICATION=true -p 4 --tasking true -r true

---------------
## 5. Integration into Bazel build

The stream def generation is part of the bazel build. It is called at the top level BUILD file via "generate_stream_defs"
so it will be added to the output folder along with the binary files.
This "generate_stream_defs" calls "stream_defs" in the software\app\common folder.
In this "stream_defs" filegroup, it defines all the streams that are to be generated.
It is further broken down into a "generate_stream_def" for each stream def.


To update an existing stream version, find the "generate_stream_def" for the desired stream and update its "stream_ver"

```sh
generate_stream_def(
    name = "mmic_stream",
    logging_dir = ":stream_headers",
    logging_file = "mmic_stream.h",
    stream_num = 8,
    stream_struct = "MMIC_Stream_T",
    stream_ver = 5,    <-- UPDATE VERSION HERE
    visibility = ["//visibility:public"],
)
```

To add a new stream def add the folloing to this software\app\common\BUILD
```sh
generate_stream_def(
    name = "NEW_stream", <--- ADD NEW STREAM NAME HERE
    logging_dir = ":stream_headers",
    logging_file = "NEW_stream.h", <--- ADD NEW STREAM HEADER HERE
    stream_num = xx, <--- ADD NEW STREAM NUMBER HERE
    stream_struct = "NEW_Stream_T", <--- ADD NEW STREAM STRUCTURE HERE
    stream_ver = xx, <--- ADD NEW STREAM VERSION HERE
    visibility = ["//visibility:public"],
)
```
This new name is then added to "stream_headers" copy_to_dir srcs.
```sh
copy_to_dir(
    name = "stream_headers",
    srcs = [
        "calib_stream.h",
        "cdc_stream.h",
        "debug_stream.h",
        "detection_stream.h",
        "fixmac.h",
        "header_stream.h",
        "mmic_stream.h",
        "rdd_stream.h",
        "reuse.h",
        "rfft_stream.h",
        "status_stream.h",
        "stream_header.h",
        "//software/app/common/calibrations:fg_calibration",
        "//software/app/common/calibrations/smc:fg_smc_cal",
        "//software/app/common/calibrations/usc:fg_usc_cal",
        "<NEW STREAM HEADER.h HERE>", <--- ADD NEW STREAM HEADER HERE
    ],
    outputDir = "temp",
    visibility = ["//visibility:public"],
)
```
---------------
## 6. File Revision History
|Rev|Date|NetId|Name|SCR|
|-|-|-|-|-|
|0.1|28-Jul-2022| wj9p84| Fred| EII-933|
