#pragma once
#include <vector>
#include <string>

/**
 * Declares auto generated SMC parameters
 * auto generated SMC parameters cpp file has been moved from single to multiple file due to the fatal error:number of sections exceeded object file format limit
 */
struct AutoGenSMCParameter {
   std::vector<std::vector<double>> antenna_gain_db;
   std::vector<std::vector<double>> K_Std_AZ_LUT;
   std::vector<std::vector<double>> K_Std_El_LUT;
   std::vector<std::vector<double>> Az_quality_LUT;
   std::vector<std::vector<double>> Exist_Prob_Det_LUT;
   std::vector<double> sens_thold_db;
   std::vector<double> sens_thold_range;
   std::vector<double> angle_range;
   std::vector<double> cc_range;
   std::vector<double> cfar_offset_db;
   std::vector<double> k_temp_sens_db;
   std::vector<double> look_parameters_LRLL;
   std::vector<double> look_parameters_LRML;
   std::vector<double> look_parameters_MRLL;
   std::vector<double> look_parameters_MRML;
   std::vector<double> sfw_v1_LRLL;
   std::vector<double> sfw_v1_LRML;
   std::vector<double> sfw_v1_MRLL;
   std::vector<double> sfw_v1_MRML;
   std::vector<double> sfw_v2_LRLL;
   std::vector<double> sfw_v2_LRML;
   std::vector<double> sfw_v2_MRLL;
   std::vector<double> sfw_v2_MRML;
   std::vector<double> fov_parameters;
   double Delta_AZ_Zone2;
   double Delta_El;
   double snr_m_db;
   double snr_m_peak_db;
   double max_num_of_rbins;
   double max_num_of_dbins;
   double radar_frequency;
   double temperature_m;
   double doppler_sll;
   double k_end_rbin_CI;
   double k_start_rbin_CI;
   double ci_gain_db;
   double K_Std_Range;
   double K_Std_RR;
   double range_m;
   double rcs_m_db;
   double K_Std_Max_SNR;
   double K_Min_AZ_Zone_1;
   double K_Max_AZ_Zone_1;
   double Delta_AZ_Zone1;
   double blockage_mask_length;
   double azimuth_resolution;
};

extern void init_BMW_FLR7_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_BMW_MRR3_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_BMW_SRR5PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_BMW_SRR7PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_CEER_FLR7_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_CEER_SRR7PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_CHANGAN_SRR5_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_FORD_MRR3_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_GPO_SRR7PLUS_UWB_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_HKMC_SRR5_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_HONDA_SRR6PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_MTNL_FLR4PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_NISSAN_SRR6_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_RNA_SRR5_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_SCANIA_SRR3_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_STLA_FLR4_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_STLA_FLR4PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_STLA_FLR7_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_STLA_SRR6PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_STLA_SRR7PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_TML_SRR5_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_TRATON_SRR6PLUS_DC_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_GPO_FLR8_SM2_SMC_Parameters(AutoGenSMCParameter *ptr);
extern void init_GPO_SRR8Plus_SM2_SMC_Parameters(AutoGenSMCParameter *ptr);
