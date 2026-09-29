/*===========================================================================*\
* FILE: f360_longi_stat_cluster.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of support functions 
*   related to f360_longi_stat_cluster.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_longi_stat_cluster.h" 

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Get_Cluster_Min_Long_Pos()
   *===========================================================================
   * RETURN VALUE:
   * float32_t
   *
   * PARAMETERS:
   * const F360_Longi_Stat_Cluster_T& cluster
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function returns the min longitudinal position of an object in the current
   * cluster.
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structure
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Get_Cluster_Min_Long_Pos(const F360_Longi_Stat_Cluster_T& cluster)
   {
      return cluster.first_object->bbox.Get_Center().x;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Cluster_Max_Long_Pos()
   *===========================================================================
   * RETURN VALUE:
   * float32_t
   *
   * PARAMETERS:
   * const F360_Longi_Stat_Cluster_T& cluster
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function returns the max longitudinal position of an object in the current
   * cluster.
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structure
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Get_Cluster_Max_Long_Pos(const F360_Longi_Stat_Cluster_T& cluster)
   {
      return cluster.last_object->bbox.Get_Center().x;
   }
}

