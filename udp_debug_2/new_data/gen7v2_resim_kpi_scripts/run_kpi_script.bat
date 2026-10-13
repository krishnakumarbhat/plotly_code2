@echo off
echo Select the KPI script to run:
echo 1. Detection KPI
echo 2. Alignment KPI
echo 3. Tracker KPI
set /p choice=Enter your choice (1/2/3): 

if "%choice%"=="1" (
    set script_name=detection_matching_kpi_script.py
) else if "%choice%"=="2" (
    set script_name=alignment_matching_kpi_script.py
) else if "%choice%"=="3" (
    set script_name=tracker_matching_kpi_script.py
) else (
    echo Invalid choice. Exiting.
    pause
    exit /b
)

python %script_name% log_path.txt meta_data.json C:\Project\Logs\Gen7\v2\ASTAZERO_GEN7v2_TESTING_w19\AF_Cloud
pause
