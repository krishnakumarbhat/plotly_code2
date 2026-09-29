#ifndef SG_DUMMY_GENERATOR_H
#define SG_DUMMY_GENERATOR_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"
#include "sg_host_props.h"
#include "sg_input.h"

namespace sg
{
   // TODO (https://jiraprod.aptiv.com/browse/FZD-223) remove unneded functions
   void fill_detections_with_dummy_data(DetectionStorage &sg_det_store, const Calibrations_T &calibrations);

   void fill_single_det(Detection_T &sg_det,
                        const uint32_t det_idx,
                        const Calibrations_T &calibrations,
                        const geometry::Point3D_T &cluster_center,
                        const uint16_t clsuter_id);

   // TODO (https://jiraprod.aptiv.com/browse/FZD-223) remove unneded functions
   void fill_contours_with_dummy_data(ContourStorage &sg_contours);

   void fill_host_with_dummy_data(RSPP_Host_T &rspp_host);

   void fill_host_props_with_dummy_data(const float elapsed_time, const RSPP_Host_T &rspp_host, HostProps &host_properties);

   void fill_input_detections_with_dummy_data(SG_Input_Detections_T &detection_list);

   void fill_input_sensors_with_dummy_data(sg::rspp::F360_Radar_Sensor_T (&sensors)[sg::rspp::MAX_NUMBER_OF_SENSORS]);
}
#endif