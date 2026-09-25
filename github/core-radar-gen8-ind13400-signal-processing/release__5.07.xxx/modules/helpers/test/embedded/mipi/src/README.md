
## Module Specific Tests
Test procedure for testing the IDM-DC EstComp module, that resides inside the MIPI Wrapper Core. The proceedure
for this test is slightly convoluted since the simulator models done have test pattern generator and
we cannot use the mipi debug mode cause that does not go through the IDM core. So we have to use test pattern
generator in the embedded part to generate test data and then go back to specs and inject that into specs
to get a comparative. For that matter these manual steps are needed and automated checking is not possible
in the SPBB repo. But checking is done in the Exec Spec setup/Matlab environment.

0) Modify the code/idm config to disable the idm and dc compensation capability using the following
   for each channel in the mipi_idm_funteat.c file.
   idm_config_ptr->k_idm_chX_detection_ena              = 0;
   idm_config_ptr->dc_estimation_enable          =  0;
   idm_config_ptr->k_idm_chX_mitigation_byp             =  1;

1) (To build .elf for running on the Target EVB, load it via trace32 for xtensa)
       bazelisk build //modules/helpers/test/embedded/mipi:mipi_idm_test

2) Load the elf and run, use the mipi_bbe_windows_idm.cmm file to see some variables.

3) Dump out the variables Generated_Chirp_Data_Samples_Chrp1, Generated_Chirp_Data_Samples_Chrp0
   using these on trace32 for BBE.
   data.save.binary chrp1_noidmdc_4rampcycles.bin 0x84007f00--0x84009EFF // 8192 bytes.
   data.save.binary chrp0_noidmdc_4rampcycles.bin 0x84005f00--0x84007EFF

   We will be using this data in the specs as input data to verify the output of idm-dc modules
   in the specs.

4) Repeat 0), 1),2),3) after enabling the idm and dc capability using
   idm_config_ptr->k_idm_chX_detection_ena              = 1;
   idm_config_ptr->dc_estimation_enable          =  1;
   idm_config_ptr->k_idm_chX_mitigation_byp             =  0;

   Just save the data this time as  :
   data.save.binary chrp1_idmdc_4rampcycles.bin 0x84007f00--0x84009EFF // 8192 bytes.
   data.save.binary chrp0_idmdc_4rampcycles.bin 0x84005f00--0x84007EFF

   Also same idm_stat variable, it will have idm stats for both the chirps.

5) Now, use the Gen_AdcData.m file to generate the adcData variable that can be over-written in the specs.

6) Run the specs upto the beginning of the Range processing. i.e. till end of mipi wrapper :
   chandra.chandra_noc.mipi_wrapper.load_adc_data(adcData);
   post_mipi_adc_data = chandra.chandra_noc.mipi_wrapper.adc_data;

7) Now use the post_mipi_adc_data to match it against the data with chrp1_idmdc_4rampcycles.bin and
   chrp0_idmdc_4rampcycles.bin data. They should match

8) Compare the idm_stat variable with the one in specs. They should be close with regards to thrshold values, but
   the number of polluted samples must match between the specs and embedded for each chirp for each channel.
