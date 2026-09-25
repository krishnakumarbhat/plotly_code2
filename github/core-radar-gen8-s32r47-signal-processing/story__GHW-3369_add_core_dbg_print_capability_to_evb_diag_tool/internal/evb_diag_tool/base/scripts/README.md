BBE Conversion scripts
-----
The "dsp_hex_file_gen.sh" script file was initially copied from the NXP S32 Design Studio install of the S32R47 Diagnostics Tool v1 installation.
Minor tweaks were made so it didn't try to copy the output to another (nonexistent) directory. See overview below. Search the code for APTIV.
The "dsp_hex_file_gen.bat" script file is an Aptiv port of the tweaked dsp_hex_file_gen.sh to run on Windows.

KQ8 Conversion scripts
-----
The "dsp_hex_file_gen_kq8.sh" script file was initially copied from the NXP S32 Design Studio install of the S32R47 Diagnostics Tool v1 installation.
Minor tweaks were made so it didn't try to copy the output to another (nonexistent) directory. See overview below. Search the code for APTIV.
The "dsp_hex_file_gen_kq8.bat" script file is an Aptiv port of the tweaked dsp_hex_file_gen.sh to run on Windows.

Overview
The original script has some limitations that must be considered in a genrule:
-it creates temporary filenames from the input file name, which it gets by trying to strip the paths off of the input filepath.
---> This means: the input filename cannot include a directory.
===> This means: the script must run in the same directory as the input file
-it creates the output in its local directory
===> This means: the genrule needs to specify the output location correctly
-it copies the output to a fixed Design Studio location
===> This means: the genrule needs to create that location in the sandbox before the script runs
NOTE: For now, the original script was modified to just comment out this extra copy.
