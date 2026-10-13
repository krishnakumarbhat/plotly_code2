/*=============================================================================================*\
* FILE: sg_split_or_shorten_contour.h
* ====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains split_or_shorten_contour and relevant helper functions declarations.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_SPLIT_OR_SHORTEN_CONTOUR_H
#define SG_SPLIT_OR_SHORTEN_CONTOUR_H

#include <bitset>

#include "sg_contour_postprocessing_calibrations_helpers.h"
#include "sg_contour_storage.h"

namespace sg
{
/**
 * @brief             Splits or shortens the contour based on the vertices_to_remove bitmask.
 *
 * @param[in, out]    contour
 * @param[out]        all_new_contours
 * @param[in, out]    contours_to_remove_mask
 * @param[in, out]    num_empty_contour_slots
 * @param[in, out]    num_new_contours
 * @param[in]         vertices_to_remove_mask
 * @param[in]         contour_idx
 **/
void split_or_shorten_contour(Contour_T &contour, std::array<Contour_T, SG_MAX_NUM_CONTOURS> &all_new_contours,
                              std::bitset<SG_MAX_NUM_CONTOURS> &contours_to_remove_mask, uint16_t &num_empty_contour_slots,
                              uint16_t &num_new_contours,
                              const std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask,
                              const uint8_t contour_idx);

/**
 * @brief             Creates a subcontour as a result of contour split due to removal of
 *                    vertices.
 *
 * @param[out]        empty_contour
 * @param[in, out]    contour
 * @param[in, out]    subcontour_len
 **/
void fill_empty_contour(Contour_T &empty_contour, Contour_T &contour, uint16_t &subcontour_len);

/**
 * @brief        Iterates over vertices of created contour and moves them
 *               back to the original.
 *
 * @param[in, out]    contour
 * @param[in, out]    new_contour
 **/
void move_vertices_to_new_contour(Contour_T &contour, Contour_T &new_contour);

/**
 * @brief        Removes vertices which were marked as to be removed
 *               and resets the splitted contour length.
 *
 * @param[in, out]    contour
 * @param[in, out]    splitted_contour_len
 **/
void remove_obsolete_vertices(Contour_T &contour, uint16_t &splitted_contour_len);

/**
 * @brief        Moves created subcontour to the to-be-added contour list.
 *
 * @param[in, out]    new_contour
 * @param[out]        all_new_contours
 * @param[in, out]    num_empty_contour_slots
 * @param[in, out]    num_new_contours
 **/
void move_new_contour_to_list(Contour_T &new_contour, std::array<Contour_T, SG_MAX_NUM_CONTOURS> &all_new_contours,
                              uint16_t &num_empty_contour_slots, uint16_t &num_new_contours);

} // namespace sg
#endif
