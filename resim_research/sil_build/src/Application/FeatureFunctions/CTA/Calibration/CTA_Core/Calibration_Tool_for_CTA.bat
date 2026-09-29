@echo off

REM Determine repo main folder
set CORE_CAL_PATH=%~dp0%
set BASE_PATH=%CORE_CAL_PATH%..\..\..

REM Run Calibration Tool
echo ####### Running calibration tool...
%BASE_PATH%\Calibration_Tool\dist\ct_main.exe %CORE_CAL_PATH%\cta_cal.xml

pause
