#pragma once

#include "DCEnums.h"
#include "AutoGenSMCParameter.h"

/**
 * Contains information related to radar SMC
 */
class DCSMCParameters : public AutoGenSMCParameter {
 public:
   DCSMCParameters();
   ~DCSMCParameters();

   /**
    * Initializes SMC parameters based on radar type
    */
   void initDCSMCParameters(const DCCustomerVariant variant);
};
