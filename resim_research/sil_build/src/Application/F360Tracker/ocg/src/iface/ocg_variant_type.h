/*================================================================================*\
 * Copyright 2023 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OCG_VARIANT_H
#define OCG_VARIANT_H

#include "ocg_reuse.h"

namespace ocg
{
   enum OCG_Variant_T : uint8_t
   {
      OCG_VARIANT_FLR4P_X1_V1 = (0),
      OCG_VARIANT_FLR4_X1_V1 = (1),
      OCG_VARIANT_FLR7_X1_V1 = (2),
      OCG_VARIANT_FLR4P_PLT_X1_V1 = (3),
      OCG_VARIANT_SRR7P_X2_V1 = (4)
   };
}

#endif
