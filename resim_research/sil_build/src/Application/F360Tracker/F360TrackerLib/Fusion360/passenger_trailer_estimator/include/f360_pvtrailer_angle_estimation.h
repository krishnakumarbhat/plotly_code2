#ifndef F360_PVTRAILER_ANGLE_ESTIMATION_H
#define F360_PVTRAILER_ANGLE_ESTIMATION_H
/*===========================================================================*\
 * FILE: f360_pvtrailer_angle_estimation.h
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "f360_reuse.h"
#include "f360_host.h"
#include "f360_pvtrailer_data.h"

namespace f360_variant_A
{
   void PVTrailer_Estimate_Angle(
      const F360_Host_T& vehicle_data,
      const float32_t elapsed_time_s,
      const float32_t trailer_axle_length,
      F360_PVTrailer_Angle_Data_T& pvtrailer_angle);
}
#endif
