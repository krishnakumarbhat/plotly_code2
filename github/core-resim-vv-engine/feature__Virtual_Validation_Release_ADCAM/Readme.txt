Prerequisites :
1. Singularity (required)
2. Docker (optional — only needed to also produce a .tar Docker image)

Command to install Singularity on Linux/WSL:
   See https://docs.sylabs.io/guides/latest/user-guide/quick_start.html

Command to install Docker on Linux/WSL (optional):
1. sudo apt install docker.io
2. sudo usermod -aG docker $USER
   (log out and back in, or run: newgrp docker)

Note on Docker vs Singularity:
- The same Generate_Singularity.sh script works with or without Docker.
- If Docker is present  : script auto-detects it and builds a Docker image, saves it
                          as a .tar, then converts to .simg. Both are delivered.
- If Docker is absent   : script builds the .simg directly using Singularity's
                          built-in OCI client (pulls Ubuntu from DockerHub).
                          Only the .simg is produced. Docker is NOT required on HPC.

Steps to generate Singularity image:
1. Copy required LM2 FMU(s) to 03_LOGIC_MODEL/<customer>/
2. Copy required SM2 FMU(s) to 02_SENSOR_MODEL/FMU/
3. Update 01_VV_ENGINE/Config/Flist_file.txt with sample or real OSI log path(s).
4. Run Generate_Singularity.sh from the Core_RESIM_VV_Engine/ directory.
5. Select customer (1=CEER, 2=ADCAM) and variant when prompted.
6. Output .simg (and optionally .tar) are generated inside 08_CloudBinaries_<customer>_<variant>/
   e.g. 08_CloudBinaries_adcam_ifv600/ for ADCAM, 08_CloudBinaries_ceer_p600/ for CEER P600.

Steps to run Singularity (recommended — use generated script):
1. cd 08_CloudBinaries_<customer>_<variant>/
2. Edit Flist_file.txt — add full absolute paths to OSI log files, one per line.
3. ./Run_Singularity.sh
   (this uses --writable-tmpfs so LM2 outputs like cp_buffer.bin are saved to OUTPUT_DIR/)

Steps to run Singularity (manual command):
1. singularity exec --writable-tmpfs <simg_path> /RUN_VV.sh <Flist_file.txt_path> <Output_path>
   Note: --writable-tmpfs is required for LM2 working-dir outputs (cp_buffer.bin etc.)
         If omitted, a warning is printed and simulation still runs but those outputs are skipped.
         Always use full absolute paths for Flist_file.txt and Output_path.

Steps to run tar file (Docker required):
1. Place OSI logs inside 08_CloudBinaries_<customer>_<variant>/INPUT_LOGS/
   (sample logs already present in INPUT_LOGS/Sample_Trace/ for testing)
2. Edit 08_CloudBinaries_<customer>_<variant>/Flist_file.txt — use paths relative to INPUT_LOGS/
   e.g. Sample_Trace/CAR_forward_5kmph_debris.txt
3. cd 08_CloudBinaries_<customer>_<variant>/
4. sudo ./GEN_LOG.sh /output/Flist_file.txt /output/OUTPUT_DIR

Note:
    1. Flist_file.txt in 08_CloudBinaries_<variant>/ must be used for .tar runs (relative paths).
    2. For Singularity runs, use full absolute paths in Flist_file.txt.
    3. LM2 outputs (cp_buffer.bin, interface .txt/.mf4) are written to OUTPUT_DIR/LogicModelId*/
