/*===================================================================================*\
* FILE: sg_basic.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes for use with SG component resim.
*   They are: Inputs, Intenals (so called debug data) and Outputs.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


#ifndef SG_BASIC_H
#define SG_BASIC_H

namespace sg
{
   // it defines a covariance matrix of a single vertex position
   struct Covariance_2D
   {
      float x;
      float y;
      float xy;
   };

   // for block symmetric 4x4 covariance matrix this structure describes off diagonal 2x2 block
   // i.e. covariances between two vertices defining single edge of a contour
   struct Cross_Covariance_2D
   {
      float x1x2;
      float y1y2;
      float x1y2;
      float y1x2;
   };

   struct Deviation_3D_T
   {
      float dx;
      float dy;
      float dz;
   };

   using Pos_2D_Cov       = Covariance_2D;
   using Pos_2D_Cross_Cov = Cross_Covariance_2D;
}

#endif
