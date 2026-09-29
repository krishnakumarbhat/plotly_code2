/*===================================================================================*\
* FILE: ocg_position.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of structure describing location and orientation of the
* host in OGCS (Occupancy Grid Coordinate System). To be precise x,y describe where the
* middle of rear axle of the host is located on the grid.
* OGCS is a cartesian coordinate system with origin located at the middle of the bottom
* edge of the grid on ground level. See explanatory drawing in documentation.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef OCG_POSITION_H
#define OCG_POSITION_H

namespace ocg
{
   struct OCG_Position_T
   {
      float x; // [m] x position in grid coordinate system (longitudinal for yaw = 0)
      float y; // [m] y position in grid coordinate system (lateral for yaw = 0)
      float z; // [m] z position in grid coordinate system (vertical)
      float yaw; // [rad] yaw angle of the host wrt. X-axis of grid coordinate system 
   };
}
#endif
