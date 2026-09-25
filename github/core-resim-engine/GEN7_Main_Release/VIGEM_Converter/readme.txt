run_log_conversion.exe : Stand alone exe which can be run in command prompt with the below given steps

1. Run the cmd prompt
2. Provide the path of run_log_conversion.exe 
3. Type run_log_conversion.exe followed by "/LOG_PATH=C:\HIL_Test\test1\testlog.pcap" Output_farmat 
		Output_farmat options - DNSU or MDF4
4. After running Exe with option DVSU ,Four DVSU's for four radars will be generated in the same input pcap path provided

5. Output Dvsu's generated from run_log_conversion.exe can be used as input for resimulation. 
6. After running Exe with option MDf4,MDf4 output file will be generated in the same input pcap path provided
7.Output mdf generated from run_log_conversion.exe can be used as input for resimulation. 

/******************************************************************************************************/
PCAP-to-DVSU/MDF4 Version 4.0.0
/******************************************************************************************************/