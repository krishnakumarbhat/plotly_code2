#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "sg_stream.h"
#include "sg_output.h"

DC_SG_Stream_T gen7_sg_stream_obj;
DC_SG_Stream_T *Get_SG_Data_Ptr();

DC_SG_Stream_T *Get_SG_Data_Ptr() {
   return &gen7_sg_stream_obj;
}

sg::SG_Output_T *GetSGOutputPtr();

void copySGDataToSGStream() {
   sg::SG_Output_T *sg_f360_ptr = GetSGOutputPtr();
   DC_SG_Stream_T *sg_str_ptr   = Get_SG_Data_Ptr();

   sg_str_ptr->execution_timestamp_us.ts_seconds       = static_cast<uint32_t>(sg_f360_ptr->execution_timestamp_us * 1E-6);
   sg_str_ptr->execution_timestamp_us.ts_nanoseconds   = static_cast<uint32_t>(sg_f360_ptr->execution_timestamp_us * 1E3);
   sg_str_ptr->measurement_timestamp_us.ts_seconds     = static_cast<uint32_t>(sg_f360_ptr->measurement_timestamp_us * 1E-6);
   sg_str_ptr->measurement_timestamp_us.ts_nanoseconds = static_cast<uint32_t>(sg_f360_ptr->measurement_timestamp_us * 1E3);
   sg_str_ptr->f_valid                                 = 1;
   sg_str_ptr->num_contours                            = sg_f360_ptr->num_contours;
   for (int i = 0; i < DC_SG_MAX_NUM_OUTPUT_VERTICES; i++) {
      sg_str_ptr->vertices[i].position_x             = sg_f360_ptr->vertices[i].position_x;
      sg_str_ptr->vertices[i].position_y             = sg_f360_ptr->vertices[i].position_y;
      sg_str_ptr->vertices[i].position_variance_x    = sg_f360_ptr->vertices[i].position_variance_x;
      sg_str_ptr->vertices[i].position_variance_y    = sg_f360_ptr->vertices[i].position_variance_y;
      sg_str_ptr->vertices[i].position_covariance_xy = sg_f360_ptr->vertices[i].position_covariance_xy;
      sg_str_ptr->vertices[i].cycles_since_created   = sg_f360_ptr->vertices[i].cycles_since_created;
      sg_str_ptr->vertices[i].cycles_since_coasted   = sg_f360_ptr->vertices[i].cycles_since_coasted;
      sg_str_ptr->vertices[i].drivability            = static_cast<uint8_t>(sg_f360_ptr->vertices[i].drivability);
      sg_str_ptr->vertices[i].drivability_confidence = sg_f360_ptr->vertices[i].drivability_confidence;
   }
   for (int i = 0; i < DC_SG_MAX_NUM_OUTPUT_CONTOURS; i++) {
      sg_str_ptr->contours[i].num_vertices = sg_f360_ptr->contours[i].num_vertices;
      sg_str_ptr->contours[i].unique_id    = sg_f360_ptr->contours[i].unique_id;
      sg_str_ptr->contours[i].type         = static_cast<uint8_t>(sg_f360_ptr->contours[i].type);
   }
}