/*===================================================================================*\
* FILE: sg_calibration_initializer_scania.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains CalibrationInitializerScania class declaration.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CALIBRATION_INITIALIZER_SCANIA_H
#define SG_CALIBRATION_INITIALIZER_SCANIA_H

#include "sg_calibration_initializer_platform.h"
#include "sg_calibrations.h"

namespace sg
{
/**
 * @brief          Initializes calibrations to default values based on host speed Scenario.
 *                 Adjusted specifically for Scania Traton project.
 *
 **/
class CalibrationInitializerScania : public CalibrationInitializerPlatform
{
 public:
   using CalibrationInitializerPlatform::initialize;

 protected:
   virtual void initialize(CommonCalibs &calibrations, const Scenario &chosen_scenario) const override;
   virtual void initialize(DetectionProcessingCalibs &calibrations, const Scenario &chosen_scenario) const override;
   virtual void initialize(DetectionClusteringCalibs &calibrations, const Scenario &chosen_scenario) const override;
   virtual void initialize(MeasurementAssociationCalibs &calibrations, const Scenario &chosen_scenario) const override;
   virtual void initialize(MeasurementUpdateCalibs &calibrations, const Scenario &chosen_scenario) const override;
   virtual void initialize(ContourInitializationCalibs &calibrations, const Scenario &chosen_scenario) const override;
   virtual void initialize(ContourPostprocessingCalibs &calibrations, const Scenario &chosen_scenario) const override;
   virtual void initialize(ContourDownselectionCalibs &calibrations, const Scenario &chosen_scenario) const override;
};
} // namespace sg
#endif
