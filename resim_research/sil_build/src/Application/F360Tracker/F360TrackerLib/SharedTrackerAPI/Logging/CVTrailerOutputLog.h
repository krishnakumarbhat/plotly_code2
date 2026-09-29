#ifndef CV_TRAILER_Output_LOG_H
#define CV_TRAILER_Output_LOG_H
/*===========================================================================*\
* FILE: CVTraileroOutputLog.h
*===========================================================================
* Copyright 2024 Aptiv Technologies, Inc., All Rights Reserved.
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


#include "../Types/f360_reuse.h"
/*===========================================================================*\
* Other Header Files
\*===========================================================================*/
#include "../Types/T360_Types.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
static const int CV_TRAILER_OUTPUT_LOG_STREAM_NUM  = 88;
static const int CV_TRAILER_OUTPUT_STREAM_VERSION  = 3;

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

enum CV_Trailer_Filter_State_T
   : int32_t {
   TFS_NOT_STARTED = 0,                 // Default value
   TFS_NOT_ACTIVE,
   TFS_INIT_RUNNING,
   TFS_INIT_COMPLETE,
   TFS_SNA
};

enum CV_Joint_Model_T
   : int32_t {
   SNA_JM = 0,                          // Default value
   ONE_JOINT_JM,
   TWO_JOINT_JM
};
// Size structure
struct CV_Size_Vector_T
{
   // [m] Length of trailer
   float32_t trailer_length;

   // [m] Width of trailer
   float32_t trailer_width;
};

struct CV_Length_Filter_T
{
   float32_t value;
   uint32_t counter;
};

struct CV_Ekf_1_T
{
   float32_t x[3];
   float32_t P[9];
};

struct CV_Ekf_2_T
{
   float32_t x[5];
   float32_t P[25];
};


struct CV_One_Joint_T
{
   uint32_t n_updates;
   CV_Ekf_1_T ekf;
   CV_Length_Filter_T length_filter;
};


struct CV_Two_Joint_T
{
   uint32_t n_updates;
   CV_Ekf_2_T ekf;
   CV_Length_Filter_T length_filter;
};

struct CV_TE_Internal_States_T
{
   CV_One_Joint_T one_joint;
   CV_Two_Joint_T two_joint;
};
// Distance structure
struct CV_Pos_Vector_T
{
   // [m] Longitudinal distance
   float32_t lon;

   // [m] Lateral distance
   float32_t lat;
};

// Hesse Normal Form representation of a line
struct CV_Line_Parameters_T
{
   // Angle of the line
   float32_t theta;

   // Length of the shortest normal from origo to the line
   float32_t rho;
};

// Trailer angle representation
struct CV_Trailer_Angle_T
{
   // [rad] Angle value in VCS
   float32_t val;

   // Cos(angle value) in VCS
   float32_t cos_val;

   // Sin(angle value) in VCS
   float32_t sin_val;
};

// Trailer measurement structure
struct CV_Trailer_Measurement_Info_T
{
   float32_t debug_coordinate_vcs_x;
   float32_t debug_coordinate_vcs_y;
   float32_t primary_angle_vcs;
   float32_t primary_intersect;
   float32_t secondary_angle_vcs;
   float32_t secondary_intersect;
   bool f_primary_valid;
   bool f_secondary_valid;
};

// Estimation output for each trailer
struct CV_Trailer_Estimator_Output_T
{
   // Filter state
   CV_Trailer_Filter_State_T state;

   // Flag indicating if trailer present
   uint8_t f_present;

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
};

// External outputs (root outports fed by signals with default storage)
typedef struct CV_Trailer_Log_Tag
{
   CV_Trailer_Estimator_Output_T one_joint_model_output;
   CV_Trailer_Estimator_Output_T two_joint_model_output[2];
   CV_Joint_Model_T best_trailer_model;
   CV_TE_Internal_States_T trailer_estimator_internal_stat;
   CV_Trailer_Measurement_Info_T line_estimates;
   bool f_initiated_from_states;
   bool f_init_complete;
}CV_Trailer_Log_T;


//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

static_assert(sizeof(CV_Trailer_Log_T) == 384U, "CV_Trailer_Log_T: Wrong size");

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
