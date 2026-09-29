#ifndef FBK_OUTPUT_BOUNDARY_CHECK_H
#define FBK_OUTPUT_BOUNDARY_CHECK_H

/**
 * @file fbk_output_boundary_check.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Module of feature building kit which checks whether its outputs are within expected boundaries.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_iface_types.h"
#include "fbk_index_lookup.h"
#include "pa_reuse.h"

/*===========================================================================*\
 * Global function prototypes
\*===========================================================================*/

/**
 * @brief Checks whether all outputs are in given boundaries
 *
 * @return True when containing outputs are within their boundaries
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-218659}
 * @verification{Create a superordinate test to check whether all outputs are in given boundaries}
 **/
boolean_T Fbk_Are_Outputs_In_Boundary(const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table,
                                      const Fbk_Age_Ctr_T *p_age_properties);


#endif
