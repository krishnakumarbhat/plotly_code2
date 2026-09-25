# Gen7v2_Resim_KPI_Scripts

## Virtual Environment
Create a virtual environment folder "venv" within the "gen7v2_resim_kpi_scripts" repo

## Python Version
Base: python 3.12

## Packages
Run "pip install -r requirements.txt" from within the virtual environment.

## Prerequisite
The input csvs and output csvs should be placed in a folder. The output csvs should be from the same version of RESIM.\
Expected input file name is xx_xx_<RADAR_POSITION>_xx_xx.csv. E.g. Gen7v2_sw_6_0_101_logs_00002_20250410135804_1_ORCAS_FC_UDP_GEN7_DET_CORE.csv
Expected output(RESIM) file name is xx_xx_r00XXYYZZ_<RADAR_POSITION>_xx_xx.csv. E.g. Gen7v2_sw_6_0_101_logs_00002_20250410135804_1_ORCAS_r00060003_FC_UDP_GEN7_DET_CORE.csv

## Command to run the scripts
<script.py> log_path.txt meta_data.json <output_path> \
- <script.py> could be one of the detection/alignment/tracker matching scripts
- log_path.txt: user should provide the folder path where the input and output csvs are present
- meta_data.json: user can modify the information accordingly
   - SiL_Engine - SiL Engine version
   - SW - embedded SW version
   - RSP_SiL - RSP SiL version
   - Tracker - Tracker version
   - Mode - RESIM mode
   - Max_CDC_Records - Maximum number of CDC records
   - Range_Saturation_Thresh_Front_Radar - Range saturation threshold for front radar
   - Range_Saturation_Thresh_Corner_Radar - Range saturation threshold for corner radar
- <output_path>: folder path where the user wants the KPI results to be generated

E.g. detection_matching_kpi_script.py log_path.txt meta_data.json C:\Project\Logs\Gen7\v2\WupVeh_Gen7V2_R6.0.3_04042025

## Workflow
For KPI matching, in all modes, only below scans will be considered
  - Scan indices which are non-zero 
  - Scans with non-zero AF detections 
  - Scans with non-zero RDD1 detections
  - Scans present in both AF stream and RDD stream
  - Scans present in both vehicle log and ressimmed log 
  - Additionally, in CDC mode:
    - Scans without CDC saturation(i.e. number of CDC records < 5016)

For KPI yield:
  - All scans of AF stream is considered as scans available in vehicle
  - Only those scans which are not CDC saturated, present in AF and RDD streams of both vehicle and resim streams is considered as scans available in simmulation
  
For Mileage yield:
  - All scans of VSE stream is considered as scans available in vehicle
  - Only those scans which are not CDC saturated, present in AF, RDD and VSE streams of both vehicle and resim streams is considered as scans available in simmulation
