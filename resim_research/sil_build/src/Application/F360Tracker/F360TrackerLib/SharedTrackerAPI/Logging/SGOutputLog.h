#ifndef SG_OUTPUT_LOG_H
#define SG_OUTPUT_LOG_H
/*===========================================================================*\
* FILE: SGOUTPUTLOG.h
*===========================================================================
* Copyright 2024 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains SG output
*
* ABBREVIATIONS:
*
*
* TRACEABILITY INFO:
*   Design Document(s):
*   Requirements Document(s): PDD-10024333-012_(CADS4_VFP_Ethernet_Communication).doc
*   (Design & Requirements)
*
*   Applicable Standards (in order of precedence: highest first):
*     SW REF 264.15D "Delphi C Coding Standards" [12-Mar-2006]
*
*
* DEVIATIONS FROM STANDARDS:
*   None
*
\*===========================================================================*/


#include "../Types/f360_reuse.h"
/*===========================================================================*\
* Other Header Files
\*===========================================================================*/

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
static const int SG_OUTPUT_LOG_STREAM_NUM  = 180;
static const int SG_OUTPUT_STREAM_VERSION  = 7;

static const int SG_MAX_NUM_OUTPUT_VERTICES_LOG  = 400;
static const int SG_MAX_NUM_OUTPUT_CONTOURS_LOG  = 100;

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack(4)
#elif defined(__DCC__)
#pragma pack(4,4)
#elif defined(__TMS320C6X__)
#pragma pack(push,4)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, save_pack, 4)
#else
#endif

   enum SG_Drivability_Class_Log_T : uint8_t
   {
      UNCLASSIFIED = (0),
      OVERDRIVABLE = (1),      // the host can drive over an obstacle
      NONDRIVABLE = (2),       // the host cannot drive through
      UNDERDRIVABLE = (3),      // the host can drive under an obstacle
      COUNT = (4)
   };

    enum SG_Contour_Type_Log_T : uint8_t
    {
        SG_INVALID = (0),
        SG_POINT = (1),
        SG_POLYLINE = (2),
        SG_POLYGON = (3)
    };
   struct SG_Contour_Out_Log_T
   {
      uint32_t unique_id;                          // [-] unique ID of the contour (0 means invalid contour)
      uint16_t num_vertices;                       // [-] number of vertices
      uint16_t cycles_since_created;               // [-] number of cycles since created
      SG_Contour_Type_Log_T type;                  // [-] type of a contour
      uint8_t padding[3];
   };
   struct SG_SW_Version_Out_Log_T
   {
      std::uint16_t major{};
      std::uint16_t minor{};
      std::uint16_t patch{};
      char name[20]{};
   };
   struct SG_Vertex_Out_Log_T
   {
      float position_x;                      // [m] longitudinal position
      float position_y;                      // [m] lateral position
      float position_variance_x;             // [m^2] longitudinal position variance
      float position_variance_y;             // [m^2] lateral position variance
      float position_covariance_xy;          // [m^2] longitudinal/lateral position covariance
      uint16_t cycles_since_created;      // [-] number of SG cycles this vertex has been tracked (since its creation)
      uint16_t cycles_since_coasted;      // [-] number of SG cycles while this vertex was not updated by measurement
      SG_Drivability_Class_Log_T drivability;// [-] drivability classification   
      uint8_t drivability_confidence;        // [-] confidence in the drivability status from 0 to 100 
      uint8_t padding[2];
   };
   struct SG_Output_Log_T
   {
      uint64_t execution_timestamp_us;                       // [us] timestamp
      uint64_t measurement_timestamp_us;                     // [us] timestamp
      SG_Vertex_Out_Log_T vertices[SG_MAX_NUM_OUTPUT_VERTICES_LOG];  // [-] array of vertices
      SG_Contour_Out_Log_T contours[SG_MAX_NUM_OUTPUT_CONTOURS_LOG]; // [-] array of contours
      SG_SW_Version_Out_Log_T software_version;                            // [-] SG software version
      uint32_t cycle_index;                                  // [-] internal cycle index
      uint16_t num_contours;                                 // [-] number of contours
      bool f_valid;                                          // [-] Flag telling that SG output is usable in current tracking cycle
      uint8_t padding[5];
   };

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

static_assert(sizeof(SG_Output_Log_T) == 12456, "SG_Output_Log_T: Wrong size");

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack()
#elif defined(__DCC__)
#pragma pack(0)
#elif defined(__TMS320C6X__)
#pragma pack(pop)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop, save_pack)
#else
#endif

#endif 
