
Steps to run the SIL testing scripts
====================================

1. Build the SIL repo for required variant, prior to running the main.m script
2. Update the following paths in main.m
	1."sil_bin_file_path" - path to which AF input (rdd data) bin file.
		a. example: sil_bin_file_path = "C:\Users\oml7lo\wkspace\Gen7\ADVRADAR_GEN7_RSP_SIL_tmp\ADVRADAR_GEN7_RSP_SIL\gen7_sil_wrapper\data_bin\srr7hd\cdc_data";
	2."sil_exe_path" - path to SIL executable.
		a. example: sil_exe_path = "C:\Users\oml7lo\wkspace\Gen7\ADVRADAR_GEN7_RSP_SIL_tmp\ADVRADAR_GEN7_RSP_SIL\bazel-bin\gen7_sil_wrapper\main\rsp_wrapper_interface\cdc_interface\test\cdc_sil_Test.exe";
3. Select the pcap file.
4. Select the corresponding streamdef path.
5. For refernce created empty folder for log and streadefs. while testing update the corresponding pcap and streamdefs.
6. Input the required frame number to run.


Dependencies:
1. Here we need to update the AF_MAX_NUM_DET macro based on the variant in "read_af_bin_data.m". Please check the repo properly before running the scripts.

Future scopes:

1. RDD data bin generation needs to be handled based on parsing respective proper header file based on variant. Now its been hardcoded as of now.
2. Reading af output data from bin - need to be hanlded based on parsing respective proper header file based on variant.
3. Comparison logic need to be improved.
