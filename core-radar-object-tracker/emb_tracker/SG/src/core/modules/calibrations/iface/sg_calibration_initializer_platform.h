/*===================================================================================*\
* FILE: sg_calibration_initializer_platform.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains CalibrationInitializerPlatform class declaration.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CALIBRATION_INITIALIZER_PLATFORM_H
#define SG_CALIBRATION_INITIALIZER_PLATFORM_H

#include "sg_calibration_initializer.h"
#include "sg_calibration_initializer_helpers.h"
#include "sg_calibrations.h"

namespace sg
{
/**
 * @brief          Initializes calibrations based on the host speed Scenario [PARKING, CITY, HIGHWAY].
 *                 Each Scenario is defined by host speed threshold in sg_constants.h
 *
 **/
class CalibrationInitializerPlatform : public ICalibrationInitializer
{
 public:
   using ICalibrationInitializer::initialize;
   CalibrationInitializerPlatform()                                                  = default;
   CalibrationInitializerPlatform(const CalibrationInitializerPlatform &initializer) = delete;
   CalibrationInitializerPlatform &operator=(const CalibrationInitializerPlatform &initializer) = delete;

 protected:
   /**
    * @brief               Initializes common calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(CommonCalibs &calibrations, const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes time_update calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(TimeUpdateCalibs &calibrations, const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes detection_processing calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(DetectionProcessingCalibs &calibrations,
                           const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes detection_clustering calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(DetectionClusteringCalibs &calibrations,
                           const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes measurement_association calibrations based on host speed Scenario [PARKING, CITY,
    *                      HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(MeasurementAssociationCalibs &calibrations,
                           const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes measurement_update calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(MeasurementUpdateCalibs &calibrations,
                           const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes contour_initialization calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(ContourInitializationCalibs &calibrations,
                           const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes contour_postprocessing calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(ContourPostprocessingCalibs &calibrations,
                           const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes contour_downselection calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(ContourDownselectionCalibs &calibrations,
                           const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes drivability_classification calibrations based on host speed Scenario [PARKING, CITY,
    *                      HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(DrivabilityClassificationCalibs &calibrations,
                           const Scenario &chosen_scenario = Scenario::PARKING) const override;

   /**
    * @brief               Initializes sg_dc_fusion calibrations based on host speed Scenario [PARKING, CITY, HIGHWAY]
    *
    * @param[in, out]      calibrations
    * @param[in]           chosen_scenario
    **/
   virtual void initialize(SGDCFusionCalibs &calibrations, const Scenario &chosen_scenario = Scenario::PARKING) const override;
};
} // namespace sg
#endif
