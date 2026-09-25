For the script to run below modules needs to be installed
	> Install Python version > 3.7 just to make sure everything works correctly
	> Installing Pandas module (pip install pandas)
	> Installing excel extension (pip install xlrd)
	> Installing excel extension (pip install fsspec)

how to run:
	python.exe ./SMCParameterCppFileGenerator.py <path_to_pdd_xlsm> 
	
Note:
	> delete AutoGenSMCParamter file if updating the same variable in AutoGenSMCParamter.cpp
	> else just run as mentioned in (how to run) section to update AutoGenSMCParamter.cpp file with new variable
	> .h file will be created new for every run

Updates:
<29-Jan-2024 Abhishek U H>
- NISSAN_SRR6 SMC is updated from smc_cal_41_20_1_102.json file and Antenna gain pattern are taken from SRR6_V1_Az_avg2way_C1B.csv and SRR6_V1_El_avg2way_C1B.csv file(archive folder)

<16-Jun-2023 A Rakesh Kumar>: 
- HONDA_SRR6Plus is updated from SRR6Plus SMC. The source for this is smc_cal_35_22_1_5.json
- NISSAN_SRR6 is updated from SRR6Plus SMC. The source for this is smc_cal_33_20_1_4.json

<07-Jun-2023 Abhishek U H>: 
- To accomodate new SFW implementation , all excel files are added with new parameters
- STLA_SRR6Plus is updated with new SFW2 parameters from SRR6Plus SMC. The source for this is smc_cal_37_22_2_0.json
- STLA_FLR4 is updated with new SFW2 parameters from platform FLR4 SMC. The source for this is smc_cal_37_21_0_0.json

<07-Jun-2023 A Rakesh Kumar>: 
- STLA_SRR6Plus is updated from SRR6Plus SMC. The source for this is smc_cal_37_22_2_0.json
- STLA_FLR4Plus is updated from FLR4Plus SMC. The source for this is smc_cal_43_23_2_0.json
- STLA_FLR4 is updated from platform FLR4 SMC. The source for this is smc_cal_37_21_0_0.json

<02-Nov-2022 Ananthesh J Shet>: 
- STLA_SRR6Plus is updated from SRR6Plus platform SMC. The source for this is smc_cal_27_22_0_0.json

<29-Oct-2022 Ananthesh J Shet>: 
- BMW_SRR7Plus is added from BMW SRR7P SMC. The source for this is smc_cal_12_72_1_3.json
- BMW_FLR7 is added from BMW FLR7 SMC. The source for this is smc_cal_12_73_1_0.json

<19-Oct-2022 Ananthesh J Shet>: 
- STLA_FLR4 is updated from platform FLR4 SMC. The source for this is smc_cal_27_21_0_0.json
- BMW_FLR4 is removed as it was used only as a placeholder for FLR4 

<17-Oct-2022 Ananthesh J Shet>: 
- MTNL_FLR4Plus is added from Motional FLR4P SMC. The source for this is smc_cal_23_23_1_8.json

<14-Oct-2022 Ananthesh J Shet>: 
- K_Min_AZ_Zone_1 and K_Max_AZ_Zone_1 are set in degrees and not radians
- STLA_FLR4Plus is added. The source for this is smc_cal_23_23_1_1.json

<12-Oct-2022 Ananthesh J Shet>: 
- Correct k_temp_sens_db in Scania SRR3 to dB scale

<19-Sept-2022 Ananthesh J Shet>: 
- HKMC_SRR5 and TML_SRR5 are added. The source for few variables are updated

<30-Sept-2021 Abhishek U H>: 
- BMW_MRR3 is same as FORD_MRR3 so as to avoid issue coming from SMvisualiser or from other if present

<28-Sept-2021 Abhishek U H>: 
- The SMC file for BMW_SRR5plus, FORD_MRR3 was changed , to contain only SM2 required sheet/parameter and remove all not needed sheet/parameter
- Added CHANGAN_SRR5, RNA_SRR5 xlsm files