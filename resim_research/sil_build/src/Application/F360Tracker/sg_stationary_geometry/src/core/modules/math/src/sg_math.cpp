/*=============================================================================================*\
* FILE: sg_math.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of functions that do mathematical calculations widely used in
* Stationary Geometries functionality.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "sg_math.h"

#include <cmath>

namespace sg
{
   void propagate_uncertainty(const float (&jacobian)[2][2], const float (&input_variance)[2][2], float (&output_variance)[2][2])
   {
      float temp_array[2][2];
      float sum_of_products;

      // Calculate J*E*J'
      // First calculate temp_array = J*E.

      for (unsigned i = 0U; i < 2U; i++)
      {
         for (unsigned j = 0U; j < 2U; j++)
         {
            sum_of_products = 0.0F;
            for (unsigned k = 0U; k < 2U; k++)
            {
               sum_of_products += (jacobian[i][k]) * (input_variance[k][j]);
            }
            temp_array[i][j] = sum_of_products;
         }
      }

      // E_out = J*E*J' = temp_array*J'
      for (unsigned i = 0U; i < 2U; i++)
      {
         for (unsigned j = 0U; j < 2U; j++)
         {
            sum_of_products = 0.0F;
            for (unsigned k = 0U; k < 2U; k++)
            {
               // J'[k][j] = J[j][k]
               sum_of_products += temp_array[i][k] * (jacobian[j][k]);
            }
            output_variance[i][j] = sum_of_products;
         }
      }
   }

   void propagate_uncertainty(const float constant, float (&covariance)[2][2])
   {
      const float constant_square = constant * constant;

      for (unsigned i = 0U; i < 2U; i++)
      {
         for (unsigned j = 0U; j < 2U; j++)
         {
            covariance[i][j] *= constant_square;
         }
      }
   }
}
