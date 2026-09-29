/*================================================================================*\
 * Copyright 2023 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OCG_CONSTANTS_H
#define OCG_CONSTANTS_H

#include "ocg_reuse.h"
#include "ocg_variant_type.h"

namespace ocg
{
    static constexpr OCG_Variant_T OCG_VARIANT_TYPE = OCG_VARIANT_FLR7_X1_V1;
   static constexpr uint8_t NUM_CELLS_X_FAR{ 60U };
   static constexpr uint8_t NUM_CELLS_X_MID{ 30U };
   static constexpr uint8_t NUM_CELLS_X_CLOSE{ 10U };
   static constexpr uint8_t NUM_CELLS_X{ NUM_CELLS_X_FAR + NUM_CELLS_X_MID + NUM_CELLS_X_CLOSE };

   static constexpr uint8_t NUM_CELLS_Y{ 1U };

   static constexpr float CELL_LENGTH{ 2.0F };
   static constexpr float CELL_WIDTH{ 6.0F };
   static constexpr float CELL_WIDTH_EXTENSION_FACTOR{ 1.2F };

   static constexpr float GRID_MIN_X_DIST{ 0.0F }; // initial distance between host front bumper and grid origin
   static constexpr float GRID_MAX_X_DIST{ GRID_MIN_X_DIST + (static_cast<float>(NUM_CELLS_X) * CELL_LENGTH) };

   static constexpr uint8_t NUM_BREAKPOINTS_CAN_PASS_ZONE_HIGH_LEV_2{ 4U };
   static constexpr uint8_t NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_HIGH_LEV_2{ 3U };

   static constexpr uint8_t NUM_BREAKPOINTS_CAN_PASS_ZONE_MED_LEV2{ 3U };
   static constexpr uint8_t NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_MED_LEV2{ 3U };
   static constexpr uint8_t NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_MED_LEV1{ 4U };

   static constexpr uint8_t NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_LOW_LEV1{ 4U };

   static constexpr int8_t NUM_PRECOMPUTED_FACTORS{ 15 };
}

#endif
