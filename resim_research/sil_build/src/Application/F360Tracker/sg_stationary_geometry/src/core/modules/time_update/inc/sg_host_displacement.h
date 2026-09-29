/*=============================================================================================*\
* FILE: sg_host_displacement.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Host_Displacement_T class
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_HOST_DISPLACEMENT_H
#define SG_HOST_DISPLACEMENT_H

namespace sg
{
   class Host_Displacement_T
   {
     public:
      Host_Displacement_T(const float distance_in, const float cos_heading, const float sin_heading)
          : m_distance{distance_in}, m_longitudinal{distance_in * cos_heading}, m_lateral{distance_in * sin_heading}
      {
      }

      float longitudinal() const
      {
         return m_longitudinal;
      }
      float lateral() const
      {
         return m_lateral;
      }
      float distance() const
      {
         return m_distance;
      }

     private:
      float m_distance;
      float m_longitudinal;
      float m_lateral;
   };
}
#endif