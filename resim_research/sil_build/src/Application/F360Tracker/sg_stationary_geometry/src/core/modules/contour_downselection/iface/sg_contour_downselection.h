/*=============================================================================================*\
* FILE: sg_contour_downselection.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains ContourDownselection class declaration.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_CONTOUR_DOWNSELECTION_H
#define SG_CONTOUR_DOWNSELECTION_H

#include <limits>
#include <tuple>

#include "dc_fused_contour_storage.h"
#include "sg_calibrations.h"

namespace sg
{
   class ContourDownselection
   {
     private:
      static const std::uint8_t unclassified_idx  = static_cast<std::uint8_t>(SG_Drivability_Class_T::UNCLASSIFIED);
      static const std::uint8_t overdrivable_idx  = static_cast<std::uint8_t>(SG_Drivability_Class_T::OVERDRIVABLE);
      static const std::uint8_t nondrivable_idx   = static_cast<std::uint8_t>(SG_Drivability_Class_T::NONDRIVABLE);
      static const std::uint8_t underdrivable_idx = static_cast<std::uint8_t>(SG_Drivability_Class_T::UNDERDRIVABLE);
      static const std::uint8_t all_class_idx     = static_cast<std::uint8_t>(SG_Drivability_Class_T::COUNT);

     public:
      ContourDownselection() = delete;
      ContourDownselection(const dc::FusedContourStorage &fused_contours) : m_fused_contours(fused_contours)
      {
      }

      /**
       * @brief       Marks contours as valid for reduced output based on its priority.
       *
       * @param[in]   calibrations - set of calibration parameters refering to current timestamp
       * @param[in]   host - set of host parameters refering to current timestamp
       *
       * @return      void
       **/
      void run(const Contour_Downselection_Calibrations_T &calibrations, const RSPP_Host_T &host);

      /**
       * @brief    Reset all fields
       *
       * @return   void
       **/
      void reset();

     public: // TODO: https://jiraprod.aptiv.com/browse/FZD-1899 - change this label to private when unit tests are reworked to
             // testing exclusively through public interface.
      /**
       * @brief       Sets contours validity for output based on its priority and number of free slots for contours and vertices.
       *
       * @param[in]   calibrations - set of calibration parameters refering to current timestamp
       *
       * @return      void
       **/
      void mark_contours_for_reduced_output(const Contour_Downselection_Calibrations_T &calibrations);

     private:
      /**
       * @brief       Resets contour iterators and priorities.
       *
       * @return      void
       **/
      void reset_contour_iterators();

      /**
       * @brief       Modifies previously calculated priorities. It makes previously selected contours a highly prioritised.
       *
       * @param[in]   calibrations - set of calibration parameters refering to current timestamp
       *
       * @return      void
       **/
      void modify_priorities(const Contour_Downselection_Calibrations_T &calibrations);

      /**
       * @brief       Selects right contours for output and saves currently selected contour ids.
       *
       * @return      void
       **/
      void select_contours_for_output();

     public:
      /**
       * @brief       Calculates contours priority.
       *
       * @param[in]   calibrations - set of calibration parameters refering to current timestamp
       * @param[in]   host - set of host parameters refering to current timestamp
       *
       * @return      void
       **/
      void calculate_contours_priority(const Contour_Downselection_Calibrations_T &calibrations, const RSPP_Host_T &host);

      /**
       * @brief       Determines whether contour may be downselected based on their drivability.
       *
       * @param[in]   calibrations - set of calibration parameters refering to current timestamp
       * @param[in]   contour - current contour to be validated
       *
       * @return      bool
       **/
      static bool is_contour_drivability_ok_to_downselect(const Contour_Downselection_Calibrations_T &calibrations,
                                                          const sg::dc::Fused_Contour_T &contour);

      /**
       * @brief       Calculate shares of drivability classes based on their length.
       *              Classes' lengths are expected to be provided with the passed array,
       *              they are than recalculated as the share rates returned in the updated array.
       *
       * @param[in]   drivability_stats - array with length statistics of drivability classes.
       *
       * @return      void
       **/
      static void calculate_contour_drivability_shares(float (&drivability_stats)[all_class_idx + 1U]);

      /**
       * @brief       Contour position importance and its weight based on vertices positions and contour x_span.
       *
       * @param[in]   contour - contour to calculate the position importance and the position importance weight
       * @param[in]   calibrations - set of calibration parameters refering to current timestamp
       * @param[in]   host - set of host parameters refering to current timestamp
       *
       * @return      std::pair with:
       *                contour position importance [0, 1],
       *                weight of position importance >= 0
       **/
      static std::pair<float, float> calculate_contour_position_importance(const sg::dc::Fused_Contour_T &contour,
                                                                           const Contour_Downselection_Calibrations_T &calibrations,
                                                                           const RSPP_Host_T &host);

     protected:
      using FusedContourIterator = sg::dc::FusedContourStorage::FusedContourList::iterator;
      struct ContourToPriority
      {
         ContourToPriority() = default;
         ContourToPriority(const FusedContourIterator _it, const float _priority) : it(_it), priority(_priority)
         {
         }

         FusedContourIterator it{nullptr};
         float priority{};
      };
      using ContourToPriorityArray  = std::array<ContourToPriority, SG_MAX_NUM_FUSED_CONTOURS>;
      using SelectedContourIdsArray = std::array<uint32_t, SG_MAX_NUM_REDUCED_OUTPUT_CONTOURS>;
      const dc::FusedContourStorage &m_fused_contours;
      SelectedContourIdsArray m_previously_selected_contour_ids{INVALID_CONTOUR_ID};
      std::size_t m_number_of_selected_contour_ids{};
      ContourToPriorityArray m_contour_priorities{};
      std::size_t m_number_of_contours{};
   };
}
#endif
