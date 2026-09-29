/*===================================================================================*\
* FILE: sg_calibration_manager.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains CalibrationManager class declaration.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CALIBRATION_MANAGER_H
#define SG_CALIBRATION_MANAGER_H

#include "sg_calibrations.h"
#include "sg_constants.h"
#include "sg_interpolator.h"

namespace sg
{
   /*
    * Manages calibrations
    */
   class CalibrationManager
   {
     public:
      /*
       * Default Constructor for CalibrationManager
       */
      CalibrationManager();

      /*
       * The argument passed compares to the velocity threshold and chooses the appropriate interval of calibrations;
       * it interpolates its current values if it falls between two predefined calibrations
       * or gets one of the predefined if it falls on the edge cases.
       *
       * @param calibration_arg: current reference value for calculating interval calibrations
       */
      void update(const float host_speed);

      /**
       * @brief    Reset calibration manager.
       *
       * @return   None
       **/
      void reset();

      /*
       * Gets current calibrations.
       * @return reference to current calibrations
       */
      Calibrations_T &get(); // TODO: FZD-957: finally must be fixed for: const Calibrations_T& get() const;

     private:
      enum class CalibrationIndices : std::uint8_t
      {
         INDEX_PARKING = 0,
         INDEX_CITY,
         INDEX_HIGHWAY,
         NUMBER_OF_CALIBRATION_SETS, // keeps number of elements in the m_calibration_lut and m_velocity_thresholds arrays
      };

      void initialize_calibration_intervals();
      void initialize_calibration_interval_for_parking();
      void initialize_calibration_interval_for_city();
      void initialize_calibration_interval_for_highway();
      void set_calibration_indices(CalibrationIndices &index_low, CalibrationIndices &index_high, const float calibration_arg) const;
      void assign_current_calibration(const CalibrationIndices index_low, const CalibrationIndices index_high, const float speed_current);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Time_Update_Calibrations_T &low,
                                    const Time_Update_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Common_Calibrations_T &low,
                                    const Common_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Detection_Processing_Calibrations_T &low,
                                    const Detection_Processing_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Detection_Clustering_Calibrations_T &low,
                                    const Detection_Clustering_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Measurement_Association_Calibrations_T &low,
                                    const Measurement_Association_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Measurement_Update_Calibrations_T &low,
                                    const Measurement_Update_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Contour_Initialization_Calibrations_T &low,
                                    const Contour_Initialization_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Contour_Postprocessing_Calibrations_T &low,
                                    const Contour_Postprocessing_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const SG_DC_Fusion_Calibrations_T &low,
                                    const SG_DC_Fusion_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Drivability_Classification_Calibrations_T &low,
                                    const Drivability_Classification_Calibrations_T &high);
      void interpolate_calibrations(const Interpolator &the_interpolator,
                                    const Contour_Downselection_Calibrations_T &low,
                                    const Contour_Downselection_Calibrations_T &high);

      Calibrations_T m_resulting_calibration{};

      // component calibrations for different intervals, initialized with default values by Calibrations_T constructor
      Calibrations_T m_calibration_lut[static_cast<std::uint8_t>(CalibrationIndices::NUMBER_OF_CALIBRATION_SETS)]{};

      static constexpr float m_velocity_thresholds[static_cast<std::uint8_t>(CalibrationIndices::NUMBER_OF_CALIBRATION_SETS)]{
         STARTING_VELOCITY_THRESHOLD, PARKING_VELOCITY_THRESHOLD, CITY_VELOCITY_THRESHOLD}; // [m/s] => 0 [km/h], 10.8 [km/h], 50.4
                                                                                            // [km/h]
   };
}

#endif
