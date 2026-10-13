#ifndef VSE_CORE_H
#define VSE_CORE_H

/*=========================================================================
 *  FILE: VSE_Core.h
 *=========================================================================
 * Copyright � 2020 Aptiv. All rights reserved.
 * Confidential � Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------
 *
 *  DESCRIPTION:
 *    This file contains State Manager class declaration.
 *
 *
 *=========================================================================
 *------------------------------------------------------------------------------
 *
 * class:        VSE_CORE
 *
 * Description:  Wrapper for the Core VSE
 *
 * Deviations from standards: None
 *
 *========================================================================*/

/*************************
 * Include Files
 *************************/
#include "BMW_VSE_Master_Model.h"
#include "BMW_VSE_Master_Model_types.h"
#include "f360_host.h"
#include "f360_host_calib.h"
#include "f360_host_raw.h"

/*************************\
 *         Types
\*************************/

/*************************\
 *       Prototypes
\*************************/
namespace vse_core
{
class VSE_CORE
{
 public:
   VSE_CORE();
   ~VSE_CORE();

   void Initialize(const f360_variant_A::F360_Host_Calib_T &r_host_calib);
   void Step(const uint64_t timestamp_us, const float speed_correction_factor, const f360_variant_A::F360_Host_Raw_T &r_host_raw);
   void Get_Output(f360_variant_A::F360_Host_T &r_host);
   VSE_OUT Get_VSE_Output();
   VCAN_VSE Get_VCAN_VSE_Inputs();

 private:
   BMW_VSE_Master_ModelModelClass BMW_VSE;
   uint32_t vehicle_index;
};
} // namespace vse_core
/*****************************************************************************
 * Change history
 *
 * Date      By   Description
 * --------  ---  -----------
 *
 *
 *****************************************************************************/

#endif /* VSE_CORE_H */
