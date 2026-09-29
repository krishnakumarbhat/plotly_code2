#include "sg_measurement_association.h"

#include "sg_associate_detections_to_contour.h"

namespace sg
{
   void measurement_association(const ContourStorage &contours,
                                const DetectionStorage &detections,
                                const Measurement_Association_Calibrations_T &measurement_association_calibrations,
                                const Common_Calibrations_T &common_calibrations)
   {
      associate_detections_to_contour(contours, detections, measurement_association_calibrations, common_calibrations);
   }
}