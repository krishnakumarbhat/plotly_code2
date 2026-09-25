The Lauterbach script files were initially copied from the NXP S32 Design Studio install of the S32R47 Diagnostics Tool v1 installation.
The only changes are to the S32R47_M7_n_load_elf.cmm file:
   - A breakpoint was added at main
   - The path to the M7 elf file was modified to point to the bazel "fastbuild" output
      - To debug with source code, run the bazelisk build with the "-c dbg" option and change the .cmm elf path
        to the "dbg" output.
