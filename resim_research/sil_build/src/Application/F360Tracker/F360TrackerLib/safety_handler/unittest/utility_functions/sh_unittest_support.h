/** \file
 * This file contains shared support functions for safety handler unit tests
 */

#ifndef SH_UNITTEST_SUPPORT_H
#define SH_UNITTEST_SUPPORT_H

#include "f360_rot_object_log.h"
#include "f360_detection_log.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{

   /** \brief
    * Helper function to add an object with default values to the ROT_Object_List_Info_T.
    * Automatically increments the number of objects and fills the newly added object with default values.
    * The object ID is set to match the new number_of_objects count.
    *
    * \param rot_object_list_info Reference to ROT_Object_List_Info_T to add object to
    * \param is_moving Flag indicating if the object is moving (affects speed and movement_status)
    * \return Reference to the newly added object
    */
   ROT_Object_Output_T &Add_Default_Object(ROT_Object_List_Info_T &rot_object_list_info, const bool is_moving);

   /** \brief
    * Helper function to create a Processed_Detection_T with position relative to an object's bounding box.
    * The detection position is calculated based on the object's extended bounding box, which uses
    * the same logic as Object_Position_Plausible_Check:
    *   - longitudinal_offset = 3.1 meters
    *   - lateral_offset = max(3.33, range * 0.0555) meters
    *
    * \param processed_detection Reference to Processed_Detection_T to be filled
    * \param object Reference to the object used as the basis for detection placement
    * \param offset_scale_x Multiplier for the offset from the center of the object in the X direction (longitudinal).
    *                       Values with abs(offset_scale_x) > 1.0F place the detection outside the extended bounding box.
    *                       (default 0.0 = at center)
    * \param offset_scale_y Multiplier for the offset from the center of the object in the Y direction (lateral).
    *                       Values with abs(offset_scale_y) > 1.0F place the detection outside the extended bounding box.
    *                       (default 0.0 = at center)
    */
   void Create_Detection_From_Object(rspp_variant_A::Processed_Detection_T &processed_detection,
                                     const ROT_Object_Output_T &object,
                                     const float32_t offset_scale_x = 0.0f,
                                     const float32_t offset_scale_y = 0.0f);

   /** \brief
    * Helper function to create and associate multiple detections to an object.
    * Creates a set of detections positioned either inside or outside the object's extended bounding box,
    * and associates them to the object via the F360_Detection_Log_T array.
    *
    * \param raw_detect_list Reference to RSPP_Detection_List_T where detections will be created
    * \param f360_detection_list Array of F360_Detection_Log_T for detection-to-object associations
    * \param object Reference to the object to associate detections with
    * \param num_detections Number of detections to create and associate
    * \param inside_bbox If true, places all detections inside the extended bounding box; if false, places them outside
    *
    * \note This function updates:
    *       - raw_detect_list.detections[raw_detect_list.number_of_valid_detections ... raw_detect_list.number_of_valid_detections+num_detections-1]
    *       - f360_detection_list[raw_detect_list.number_of_valid_detections ... raw_detect_list.number_of_valid_detections+num_detections-1]
    *       - raw_detect_list.number_of_valid_detections (set to raw_detect_list.number_of_valid_detections + num_detections)
    *       - object.ndets (set to num_detections)
    */
   void Create_And_Associate_Detections(rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
                                        F360_Detection_Log_T (&f360_detection_list)[MAX_NUMBER_OF_DETECTIONS],
                                        ROT_Object_Output_T &object,
                                        const uint32_t num_detections,
                                        const bool inside_bbox);

} // namespace f360_variant_A

#endif // SH_UNITTEST_SUPPORT_H
