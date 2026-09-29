#include "SMCParameters.h"
#include <string.h> /*used for memset*/
#include <iostream> /* used for cout/cin */

DCSMCParameters::DCSMCParameters() {
}

DCSMCParameters::~DCSMCParameters() {
}

void DCSMCParameters::initDCSMCParameters(const DCCustomerVariant variant) {
   switch (variant) {
   case DC_BMW_SRR5_PLUS_SENSOR_VARIANT:
      init_BMW_SRR5PLUS_DC_SMC_Parameters(this);
      break;
   case DC_BMW_MRR3_SENSOR_VARIANT:
      init_BMW_MRR3_DC_SMC_Parameters(this);
      break;
   case DC_FORD_MRR3_SENSOR_VARIANT:
      init_FORD_MRR3_DC_SMC_Parameters(this);
      break;
   case DC_CHANGAN_SRR5_SENSOR_VARIANT:
      init_CHANGAN_SRR5_DC_SMC_Parameters(this);
      break;
   case DC_RNA_SRR5_SENSOR_VARIANT:
      init_RNA_SRR5_DC_SMC_Parameters(this);
      break;
   case DC_SCANIA_SRR3_SENSOR_VARIANT:
      init_SCANIA_SRR3_DC_SMC_Parameters(this);
      break;
   case DC_NISSAN_SRR6_SENSOR_VARIANT:
      init_NISSAN_SRR6_DC_SMC_Parameters(this);
      break;
   case DC_HONDA_SRR6PLUS_SENSOR_VARIANT:
      init_HONDA_SRR6PLUS_DC_SMC_Parameters(this);
      break;
   case DC_HKMC_SRR5_SENSOR_VARIANT:
      init_HKMC_SRR5_DC_SMC_Parameters(this);
      break;
   case DC_TML_SRR5_SENSOR_VARIANT:
      init_TML_SRR5_DC_SMC_Parameters(this);
      break;
   case DC_STLA_FLR4PLUS_SENSOR_VARIANT:
      init_STLA_FLR4PLUS_DC_SMC_Parameters(this);
      break;
   case DC_STLA_FLR4_SENSOR_VARIANT:
      init_STLA_FLR4_DC_SMC_Parameters(this);
      break;
   case DC_STLA_SRR6PLUS_SENSOR_VARIANT:
      init_STLA_SRR6PLUS_DC_SMC_Parameters(this);
      break;
   case DC_MTNL_FLR4PLUS_SENSOR_VARIANT:
      init_MTNL_FLR4PLUS_DC_SMC_Parameters(this);
      break;
   case DC_BMW_SRR7PLUS_SENSOR_VARIANT:
      init_BMW_SRR7PLUS_DC_SMC_Parameters(this);
      break;
   case DC_BMW_FLR7_SENSOR_VARIANT:
      init_BMW_FLR7_DC_SMC_Parameters(this);
      break;
   case DC_TRATON_SRR6PLUS_SENSOR_VARIANT:
      init_TRATON_SRR6PLUS_DC_SMC_Parameters(this);
      break;
   case DC_CEER_SRR7PLUS_SENSOR_VARIANT:
      init_CEER_SRR7PLUS_DC_SMC_Parameters(this);
      break;
   case DC_CEER_FLR7_SENSOR_VARIANT:
      init_CEER_FLR7_DC_SMC_Parameters(this);
      break;
   case DC_GPO_SRR7PLUS_UWB_SENSOR_VARIANT:
      init_GPO_SRR7PLUS_UWB_DC_SMC_Parameters(this);
      break;
   case DC_STLA_SRR7PLUS_SENSOR_VARIANT:
      init_STLA_SRR7PLUS_DC_SMC_Parameters(this);
      break;
   case DC_STLA_FLR7_SENSOR_VARIANT:
      init_STLA_FLR7_DC_SMC_Parameters(this);
      break;
   case DC_GPO_SRR8PLUS_SENSOR_VARIANT:
      init_GPO_SRR8Plus_SM2_SMC_Parameters(this);
      break;
   case DC_GPO_FLR8_SENSOR_VARIANT:
      init_GPO_FLR8_SM2_SMC_Parameters(this);
      break;   
   default:
      std::cout << "[DC ERROR]: [Initialize SMC] Code should not reach here. Exiting!" << std::endl;
      exit(1);
      break;
   }
}
