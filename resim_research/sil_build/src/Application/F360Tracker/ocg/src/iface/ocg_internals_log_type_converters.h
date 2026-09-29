/*================================================================================*\
 * Copyright 2023 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OCG_INTERNALS_LOG_TYPE_H
#define OCG_INTERNALS_LOG_TYPE_H

#include "ocg_internals_log_type.h"
#include "ocg_position.h"

namespace ocg
{
   inline OCG_Position_T CreateOCGPosition(const OCG_Internals_Log_T& log)
   {
      OCG_Position_T position;
      position.x = log.ogcs_host_rear_axle_initial_position_x;
      position.y = log.ogcs_host_rear_axle_initial_position_y;
      position.z = log.ogcs_host_rear_axle_initial_position_z;
      position.yaw = log.ogcs_host_rear_axle_initial_position_yaw;
      return position;
   }
}
#endif
