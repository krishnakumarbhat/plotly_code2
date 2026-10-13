/*===========================================================================*\
* FILE: sg_calibrations.h
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in Stationary Geometries.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_CALIBRATIONS_H
#define SG_CALIBRATIONS_H

#include "sg_common_calibrations.h"
#include "sg_contour_downselection_calibrations.h"
#include "sg_contour_initialization_calibrations.h"
#include "sg_contour_postprocessing_calibrations.h"
#include "sg_dc_fusion_calibrations.h"
#include "sg_detection_clustering_calibrations.h"
#include "sg_detection_processing_calibrations.h"
#include "sg_drivability_classification_calibrations.h"
#include "sg_measurement_association_calibrations.h"
#include "sg_measurement_update_calibrations.h"
#include "sg_time_update_calibrations.h"

namespace sg
{
struct Calibrations
{
   CommonCalibs common{};
   TimeUpdateCalibs time_update{};
   DetectionProcessingCalibs detection_processing{};
   DetectionClusteringCalibs detection_clustering{};
   MeasurementAssociationCalibs measurement_association{};
   MeasurementUpdateCalibs measurement_update{};
   ContourInitializationCalibs contour_initialization{};
   ContourPostprocessingCalibs contour_postprocessing{};
   DrivabilityClassificationCalibs drivability_classification{};
   SGDCFusionCalibs sg_dc_fusion{};
   ContourDownselectionCalibs contour_downselection{};
};
} // namespace sg
#endif
