/*================================================================================*\
 * Copyright 2026 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OCG_CONSTANTS_H
#define OCG_CONSTANTS_H

#include "ocg_reuse.h"
#include "ocg_variant_type.h"

static const OCG_Variant_T OCG_VARIANT_TYPE = OCG_VARIANT_SRR8P_FRONT;
#define NUM_CELLS_X_FAR   (60U)
#define NUM_CELLS_X_MID   (25U)
#define NUM_CELLS_X_CLOSE (15U)
#define NUM_CELLS_X       (NUM_CELLS_X_FAR + NUM_CELLS_X_MID + NUM_CELLS_X_CLOSE)

#define NUM_CELLS_Y (1U)

#define CELL_LENGTH                 (2.0F)
#define CELL_WIDTH                  (4.0F)
#define CELL_WIDTH_EXTENSION_FACTOR (1.8F)

#define GRID_MIN_X_DIST (0.0F) // initial distance between host front bumper and grid origin
#define GRID_MAX_X_DIST (GRID_MIN_X_DIST + ((float)(NUM_CELLS_X)*CELL_LENGTH))

#define NUM_BREAKPOINTS_CAN_PASS_ZONE_HIGH_LEV_2          (4U)
#define NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_HIGH_LEV_2 (3U)

#define NUM_BREAKPOINTS_CAN_PASS_ZONE_MED_LEV2          (3U)
#define NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_MED_LEV2 (3U)
#define NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_MED_LEV1 (4U)

#define NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_LOW_LEV1 (4U)

#define NUM_PRECOMPUTED_FACTORS (15)

#endif
