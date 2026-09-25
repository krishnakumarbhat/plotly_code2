Prerequisites :
1. Docker

Command to Install docker in the linux pc :
1. sudo apt install docker.io
2. sudo usermod -aG docker $USER

Steps to generate Singularity and tar:
1. Run Generate_Singularity.sh in terminal.
2. Singularity and tar will be generated inside 07_CloudBinaries folder.


Steps to run Singularity :
1. Update the 01_VV_ENGINE/Config/Flist_file.txt with full path of OSI logs.
2. Open a terminal in 08_CloudBinaries folder and use command : singularity exec <simg image path> /RUN_VV.sh <Flist_file.txt path> <Output_Path>                                

Note: Provide full path of requested file(s).


Steps to run tar file :
1. Keep OSI logs to run inside 07_CloudBinaries/INPUT_LOGS/  (Can use sample logs already prent in "07_CloudBinaries/INPUT_LOGS/Sample_Trace" for testing purpose).
2. Give the log path in 07_CloudBinaries/Flist_file.txt as input/Sample_Trace/Log_name.txt 
3. Open a terminal in 07_CloudBinaries folder and use command : sudo ./GEN_LOG.sh /output/Flist_file.txt /output/OUTPUT_DIR


Note: 
    1. To run .tar Flist_file.txt present in 07_CloudBinaries should be used only.
    2. To run singularity any Flist_file.txt with full paths of logs can be used.
