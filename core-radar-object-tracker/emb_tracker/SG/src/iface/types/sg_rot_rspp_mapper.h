/*===================================================================================*\
* FILE: sg_rot_rspp_mapper.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains ROT/RSPP version mapper logic.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_ROT_RSPP_MAPPER_H
#define SG_ROT_RSPP_MAPPER_H

#include "f360_log_types.h"
#include "rspp_detection_list.h"

namespace sg
{
#if defined(RSPP_VARIANT_DEFINITION_A_H) && defined(F360_VARIANT_DEFINITION_A_H)
namespace rspp = rspp_variant_A;
namespace rot  = f360_variant_A;
#elif defined(RSPP_VARIANT_DEFINITION_B_H) && defined(F360_VARIANT_DEFINITION_B_H)
namespace rspp = rspp_variant_B;
namespace rot  = f360_variant_B;
#elif defined(RSPP_VARIANT_DEFINITION_C_H) && defined(F360_VARIANT_DEFINITION_C_H)
namespace rspp = rspp_variant_C;
namespace rot  = f360_variant_C;
#elif defined(RSPP_VARIANT_DEFINITION_D_H) && defined(F360_VARIANT_DEFINITION_D_H)
namespace rspp = rspp_variant_D;
namespace rot  = f360_variant_D;
#elif defined(RSPP_VARIANT_DEFINITION_E_H) && defined(F360_VARIANT_DEFINITION_E_H)
namespace rspp = rspp_variant_E;
namespace rot  = f360_variant_E;
#elif defined(RSPP_VARIANT_DEFINITION_F_H) && defined(F360_VARIANT_DEFINITION_F_H)
namespace rspp = rspp_variant_F;
namespace rot  = f360_variant_F;
#elif defined(RSPP_VARIANT_DEFINITION_G_H) && defined(F360_VARIANT_DEFINITION_G_H)
namespace rspp = rspp_variant_G;
namespace rot  = f360_variant_G;
#elif defined(RSPP_VARIANT_DEFINITION_H_H) && defined(F360_VARIANT_DEFINITION_H_H)
namespace rspp = rspp_variant_H;
namespace rot  = f360_variant_H;
#elif defined(RSPP_VARIANT_DEFINITION_I_H) && defined(F360_VARIANT_DEFINITION_I_H)
namespace rspp = rspp_variant_I;
namespace rot  = f360_variant_I;
#elif defined(RSPP_VARIANT_DEFINITION_J_H) && defined(F360_VARIANT_DEFINITION_J_H)
namespace rspp = rspp_variant_J;
namespace rot  = f360_variant_J;
#elif defined(RSPP_VARIANT_DEFINITION_K_H) && defined(F360_VARIANT_DEFINITION_K_H)
namespace rspp = rspp_variant_K;
namespace rot  = f360_variant_K;
#elif defined(RSPP_VARIANT_DEFINITION_L_H) && defined(F360_VARIANT_DEFINITION_L_H)
namespace rspp = rspp_variant_L;
namespace rot  = f360_variant_L;
#elif defined(RSPP_VARIANT_DEFINITION_M_H) && defined(F360_VARIANT_DEFINITION_M_H)
namespace rspp = rspp_variant_M;
namespace rot  = f360_variant_M;
#elif defined(RSPP_VARIANT_DEFINITION_N_H) && defined(F360_VARIANT_DEFINITION_N_H)
namespace rspp = rspp_variant_N;
namespace rot  = f360_variant_N;
#elif defined(RSPP_VARIANT_DEFINITION_O_H) && defined(F360_VARIANT_DEFINITION_O_H)
namespace rspp = rspp_variant_O;
namespace rot  = f360_variant_O;
#elif defined(RSPP_VARIANT_DEFINITION_P_H) && defined(F360_VARIANT_DEFINITION_P_H)
namespace rspp = rspp_variant_P;
namespace rot  = f360_variant_P;
#elif defined(RSPP_VARIANT_DEFINITION_Q_H) && defined(F360_VARIANT_DEFINITION_Q_H)
namespace rspp = rspp_variant_Q;
namespace rot  = f360_variant_Q;
#else
   #error "Problem with variants setup of RSPP and ROT"
#endif
} // namespace sg

#endif
