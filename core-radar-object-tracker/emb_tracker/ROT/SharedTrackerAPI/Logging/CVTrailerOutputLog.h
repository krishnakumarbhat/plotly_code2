#ifndef CV_TRAILER_OUTPUT_LOG_H
#define CV_TRAILER_OUTPUT_LOG_H
/*===========================================================================*\
* FILE: CVTrailerOutputLog.h
*===========================================================================
* Copyright 2026 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains CV trailer output
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

#include "f360_reuse.h"
/*===========================================================================*\
* Other Header Files
\*===========================================================================*/
#include "T360_Types.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
static const int CV_TRAILER_OUTPUT_LOG_STREAM_NUM = 88;
static const int CV_TRAILER_OUTPUT_STREAM_VERSION = 4;

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
   #pragma pack(4)
#elif defined(__DCC__)
   #pragma pack(4, 4)
#elif defined(__TMS320C6X__)
   #pragma pack(push, 4)
#elif defined(_MSC_VER) || defined(__GNUC__)
   #pragma pack(push, save_pack, 4)
#else
#endif

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(error:4820)
#endif

typedef enum CV_Trailer_Filter_State_Tag
{
   TFS_NOT_STARTED = 0, // Default value
   TFS_NOT_ACTIVE,
   TFS_INIT_RUNNING,
   TFS_INIT_COMPLETE,
   TFS_SNA
} CV_Trailer_Filter_State_T;

typedef enum CV_Joint_Model_Tag
{
   SNA_JM = 0, // Default value
   ONE_JOINT_JM,
   TWO_JOINT_JM
} CV_Joint_Model_T;

// Size structure
typedef struct CV_Size_Vector_Tag
{
   // [m] Length of trailer
   float32_t trailer_length;

   // [m] Width of trailer
   float32_t trailer_width;
} CV_Size_Vector_T;

typedef struct CV_Length_Filter_Tag
{
   float32_t value;
   uint32_t counter;
} CV_Length_Filter_T;

typedef struct CV_Ekf_1_Tag
{
   float32_t x[3];
   float32_t P[9];
} CV_Ekf_1_T;

typedef struct CV_Ekf_2_Tag
{
   float32_t x[5];
   float32_t P[25];
} CV_Ekf_2_T;

typedef struct CV_One_Joint_Tag
{
   uint32_t n_updates;
   CV_Ekf_1_T ekf;
   CV_Length_Filter_T length_filter;
} CV_One_Joint_T;

typedef struct CV_Two_Joint_Tag
{
   uint32_t n_updates;
   CV_Ekf_2_T ekf;
   CV_Length_Filter_T length_filter;
} CV_Two_Joint_T;

typedef struct CV_TE_Internal_States_Tag
{
   CV_One_Joint_T one_joint;
   CV_Two_Joint_T two_joint;
} CV_TE_Internal_States_T;

// Distance structure
typedef struct CV_Pos_Vector_Tag
{
   // [m] Longitudinal distance
   float32_t lon;

   // [m] Lateral distance
   float32_t lat;
} CV_Pos_Vector_T;

// Hesse Normal Form representation of a line
typedef struct CV_Line_Parameters_Tag
{
   // Angle of the line
   float32_t theta;

   // Length of the shortest normal from origo to the line
   float32_t rho;
} CV_Line_Parameters_T;

// Trailer angle representation
typedef struct CV_Trailer_Angle_Tag
{
   // [rad] Angle value in VCS
   float32_t val;

   // Cos(angle value) in VCS
   float32_t cos_val;

   // Sin(angle value) in VCS
   float32_t sin_val;
} CV_Trailer_Angle_T;

// Trailer measurement structure
typedef struct CV_Trailer_Measurement_Info_Tag
{
   float32_t debug_coordinate_vcs_x;
   float32_t debug_coordinate_vcs_y;
   float32_t primary_angle_vcs;
   float32_t primary_intersect;
   float32_t secondary_angle_vcs;
   float32_t secondary_intersect;
   uint8_t f_primary_valid;
   uint8_t f_secondary_valid;
   uint8_t unused[2];
} CV_Trailer_Measurement_Info_T;

// Estimation output for each trailer
typedef struct CV_Trailer_Estimator_Output_Tag
{
   // Filter state [CV_Trailer_Filter_State_T]
   int32_t state;

   // [m] Trailer center of rotation in VCS
   CV_Pos_Vector_T joint_position;

   // [m] Joint position to center
   float32_t joint2center;

   // [m] Joint position to wheels
   float32_t joint2wheels;

   // Trailer angle
   CV_Trailer_Angle_T angle;

   // [rad/s] Trailer angle rate
   float32_t angle_rate;

   // [m/s] Trailer speed
   float32_t speed;

   // Trailer size
   CV_Size_Vector_T size;

   // Flag indicating if trailer present
   uint8_t f_present;

   uint8_t unused[3];
} CV_Trailer_Estimator_Output_T;

// External outputs (root outports fed by signals with default storage)
typedef struct CV_Trailer_Log_Tag
{
   CV_Trailer_Estimator_Output_T one_joint_model_output;
   CV_Trailer_Estimator_Output_T two_joint_model_output[2];
   int32_t best_trailer_model; // CV_Joint_Model_T
   CV_TE_Internal_States_T trailer_estimator_internal_stat;
   CV_Trailer_Measurement_Info_T line_estimates;
   uint8_t f_initiated_from_states;
   uint8_t f_init_complete;
   uint8_t unused[2];
} CV_Trailer_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(CV_Trailer_Log_T, 384U);

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

#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif
