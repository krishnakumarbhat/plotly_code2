Calibration Structure Generate Tool:

The xlsm files present in the folder are for Calibration Structure Generation for Cores


> Z0_cal_Gen_Utils.xlsm is the Excel sheet with Cal data.

> Z0_cal_Gen_Utils.xla is the tool which is act as a plugin.


For Source code generation:
> open the Z0_cal_Gen_Utils.xlsm and Z0_cal_Gen_Utils.xla.
> Make the necessary changes in the corresponding Cal worksheets taking the current one's as example.
> Go to Add-Ins Tab->Utils->Z0_Cals-> Z0 Cal C File Gen.
> This will create the corresponding Calibration files (Ex:- Z0_Cal.h, Diag_Cal_file.c & .h, DTC_Cal_file.c & .h, Mode_Manager_Cal_file.c & .h, Volt_Monitor_Cal_file.c & .h) in the Source_File Folder.
> After successful generation of code, Replace the files present in the inc and src folder with the generated files.


For PTP Generation:
> open the Z0_cal_Gen_Utils.xlsm and Z0_cal_Gen_Utils.xla.
> Make the necessary changes in the worksheet as the existing ones.
> Go to Add-Ins Tab->Utils->Z0_Cals-> Z0 Cal PTP Gen
> This will create the corresponding PTP File.
