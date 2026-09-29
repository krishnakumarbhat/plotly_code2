// racam_enums.h
// RaCam enumerated type definitions

/*
 * DELPHI ELECTRONICS & SAFETY PROPRIETARY
 * © Copyright 2012, 2013 All Rights Reserved. Delphi Electronics & Safety
 *
 * THIS SOFTWARE IS DELPHI ELECTRONICS & SAFETY PROPRIETARY.
 * IT MAY BE DISTRIBUTED AND USED BY DELPHI OR DELPHI CUSTOMERS,  TO
 * PARTIES WHO NEED THIS INFORMATION TO PURSUE DELPHI RELATED ACTIVITIES
 * DIRECTED BY DELPHI OR DELPHI CUSTOMERS. THIS INFORMATION SHALL NOT BE
 * USED FOR ANY OTHER PURPOSES, NOR SHALL IT BE REDISTRIBUTED TO THIRD PARTIES.
 *
 * THIS SOFTWARE INCLUDES NO WARRANTIES, EXPRESS OR IMPLIED, WHETHER
 * ORAL, OR WRITTEN WITH RESPECT TO THE SOFTWARE OR OTHER MATERIAL,
 * INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OF
 * MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE, OR ARISING FROM
 * A COURSE OF PERFORMANCE OR DEALING, OR FROM USAGE OR TRADE, OR OF
 * NON-INFRINGEMENT OF ANY PATENTS OF THIRD PARTIES.
 */

#if !defined(RACAM_ENUMS_H)
#  define RACAM_ENUMS_H


/*radar types*/
#if !defined(enum_SCAN_TYPE)
#  define enum_SCAN_TYPE
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    SCAN_TYPE_SHORT   = 0,
    SCAN_TYPE_MEDIUM  = 1,
    SCAN_TYPE_LONG    = 2,
    SCAN_TYPE_UNKNOWN = 3
} SCAN_TYPE;
#endif


#if !defined(enum_DWELL)
#  define enum_DWELL
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    DWELL_MEDIUM_LOOK = 0,
    DWELL_LONG_LOOK   = 1,
    DWELL_UNKNOWN     = 2
} DWELL;
#endif


#if !defined(enum_RANGE_MODE)
#  define enum_RANGE_MODE
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    RANGE_MODE_NO_TARGET             = 0,
    RANGE_MODE_MEDIUM_RANGE_ONLY     = 1,
    RANGE_MODE_LONG_RANGE_ONLY       = 2,
    RANGE_MODE_LONG_AND_MEDIUM_RANGE = 3,
    RANGE_MODE_UNKNOWN               = 4
} RANGE_MODE;
#endif


#if !defined(enum_BF_CRITERIA)
#  define enum_BF_CRITERIA
#ifdef PCRESIM
typedef enum  : unsigned char
#else
typedef enum
#endif
{ // Criteria used for beamforming
    BF_CRITERIA_UNKNOWN=0, 
    BF_CRITERIA_ONE_FFT_DET, 
    BF_CRITERIA_MULT_FFT_DET, 
    BF_CRITERIA_MERGED_FFT_DET, 
    BF_CRITERIA_UNKNOWN_PARITY, 
    BF_CRITERIA_RANGE_LIMIT
} BF_CRITERIA;
#endif
/*radar types*/


/*vision types*/
#if !defined(enum_FCV_VALID)
#  define enum_FCV_VALID
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum 
#endif
{
	FCV_VALID_LOW_CONFIDENCE  = 0x3c,
    FCV_VALID_MED_CONFIDENCE  = 0x69,
    FCV_VALID_HIGH_CONFIDENCE = 0x96
} FCV_VALID;
#endif


#if !defined(enum_TAP)
#  define enum_TAP
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{ 
    TAP_NONE = 0,  
    TAP_LTAP, // left turn across path
    TAP_RTAP  // right turn across path
} TAP;
#endif


#if !defined(enum_TURN_INDICATOR)
#  define enum_TURN_INDICATOR
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    TURN_INDICATOR_NONE = 0,
    TURN_INDICATOR_LEFT,
    TURN_INDICATOR_RIGHT,
    TURN_INDICATOR_UNKNOWN
} TURN_INDICATOR;
#endif


#if !defined(enum_BRAKE_INDICATOR)
#  define enum_BRAKE_INDICATOR
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    BRAKE_INDICATOR_UNKNOWN = 0,
    BRAKE_INDICATOR_OFF,
    BRAKE_INDICATOR_ON
} BRAKE_INDICATOR;
#endif


#if !defined(enum_HAZARD_INDICATOR)
#  define enum_HAZARD_INDICATOR
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    HAZARD_INDICATOR_UNKNOWN = 0,
    HAZARD_INDICATOR_OFF,
    HAZARD_INDICATOR_STEADY_ACTIVE
} HAZARD_INDICATOR;
#endif
/*vision types*/


/*fusion types*/
#if !defined(enum_TRACK_STATUS)
#  define enum_TRACK_STATUS
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    TRACK_STATUS_INVALID = 0,
    TRACK_STATUS_MERGED,
    TRACK_STATUS_NEW,
    TRACK_STATUS_NEW_COASTED,
    TRACK_STATUS_NEW_UPDATED,
    TRACK_STATUS_UPDATED,
    TRACK_STATUS_COASTED
} TRACK_STATUS;
#endif


#if !defined(enum_ST4)
#  define enum_ST4
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    ST4_INVALID = 0,
    ST4_NEW,
    ST4_UPDATED,
    ST4_COASTED
} ST4;
#endif


#if !defined(enum_OBJECT_CLASS)
#  define enum_OBJECT_CLASS
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    OBJECT_CLASS_UNDETERMINED         = 0, 
    OBJECT_CLASS_CAR                  = 1, 
    OBJECT_CLASS_MOTORCYCLE           = 2, 
    OBJECT_CLASS_TRUCK                = 3,
    OBJECT_CLASS_PEDESTRIAN           = 4, 
    OBJECT_CLASS_POLE                 = 5,
    OBJECT_CLASS_TREE                 = 6,
    OBJECT_CLASS_ANIMAL               = 7,
    OBJECT_CLASS_GOD                  = 8,
    OBJECT_CLASS_BICYCLE              = 9,
    OBJECT_CLASS_UNIDENTIFIED_VEHICLE = 10
} OBJECT_CLASS;
#endif


#if !defined(enum_TRKFLTR)
#  define enum_TRKFLTR
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    TRKFLTR_VEHICLE,
    TRKFLTR_NON_VEHICLE
} TRKFLTR;
#endif


#if !defined(enum_PSEUDO_MSMT)
#  define enum_PSEUDO_MSMT
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    PSEUDO_MSMT_NONE = 0,
    PSEUDO_MSMT_RADAR,
    PSEUDO_MSMT_VISION,
    PSEUDO_MSMT_VISION_POSN_ONLY,
    PSEUDO_MSMT_RADARVISION,
    PSEUDO_MSMT_RV_VISION,
    PSEUDO_MSMT_RADAR_TAP,
    PSEUDO_MSMT_RADARVISION_TAP,
    PSEUDO_MSMT_VISION_ONLY_TAP,
    PSEUDO_MSMT_RV_VISION_TAP,
    PSEUDO_MSMT_RADARVISION_TAP_YAWRATE,
    PSEUDO_MSMT_RV_VISION_TAP_YAWRATE,
    PSEUDO_MSMT_RADARVISION_TAP_HEADING,
    PSEUDO_MSMT_RV_VISION_TAP_HEADING
} PSEUDO_MSMT;
#endif


#if !defined(enum_FUSSRC)
#  define enum_FUSSRC
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    FUSSRC_SINGLE_TRACKLET,
    FUSSRC_MULTIPLE_TRACKLET,
    FUSSRC_VISION_ONLY,
    FUSSRC_RADAR_VISION
} FUSSRC;
#endif


#if !defined(enum_FUS_MODE)
#  define enum_FUS_MODE
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    FUS_MODE_RADAR_VISION = 0,
    FUS_MODE_RADAR_ONLY   = 1,
    FUS_MODE_VISION_ONLY  = 2
} FUS_MODE;
#endif

 //Fused Object Vision Source
#if !defined(enum_FUS_SOURCES_VIS)
#  define enum_FUS_SOURCES_VIS
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum 
#endif
{
    FUS_VIS_SRC_NONE     = 0,   //    0x0  
	FUS_VIS_SRC_OBSTACLE = 1,   //    0x1 
	FUS_VIS_SRC_VD3D     = 2    //    0x2
}FUS_SOURCES_VIS;
#endif

//Fused Object Radar Sources (OR'd values)
#if !defined(enum_FUS_SOURCES_RADAR)
#  define enum_FUS_SOURCES_RADAR
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum 
#endif
{
    FUS_RADAR_SRC_NONE   = 0,   //    0x0  
	FUS_RADAR_SRC_FLR    = 1,   //    0x1 
	FUS_RADAR_SRC_SODFL  = 2,   //    0x2
	FUS_RADAR_SRC_SODFR  = 4,   //    0x4
	FUS_RADAR_SRC_SODRL  = 8,   //    0x8
	FUS_RADAR_SRC_SODRR  = 16   //    0x10
}FUS_SOURCES_RADAR;
#endif


#if !defined(enum_MATCH)
#  define enum_MATCH
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    MATCH_NONE     = 0,
    MATCH_SSPECIAL = 1,
    MATCH_ME       = 2
} MATCH;
#endif


#if !defined(enum_REFERENCE_POSITION_T_H)
#  define enum_REFERENCE_POSITION_T_H
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    REFERENCE_POSITION_FRONT_LEFT = (0),/**< 0*/
    REFERENCE_POSITION_FRONT = (1),/**< 1*/
    REFERENCE_POSITION_FRONT_RIGHT = (2),/**< 2*/
    REFERENCE_POSITION_RIGHT = (3),/**< 3*/
    REFERENCE_POSITION_REAR_RIGHT = (4),/**< 4*/
    REFERENCE_POSITION_REAR = (5),/**< 5*/
    REFERENCE_POSITION_REAR_LEFT = (6),/**< 6*/
    REFERENCE_POSITION_LEFT = (7),/**< 7*/
    REFERENCE_POSITION_CENTER = (8),/**< 8*/
    REFERENCE_POSITION_INVALID = (255)/**< 255*/
} reference_position_T;
#endif


#if !defined(enum_CENTROID)
#  define enum_CENTROID
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
	CENTROID_REAR = 0,
	CENTROID_FRONT = 1,
	CENTROID_RIGHT_SIDE = 2,
	CENTROID_LEFT_SIDE = 3,
	CENTROID_CENTER = 4,
	CENTROID_UNKNOWN = 5,
	CENTROID_FRONT_LEFT = 6,
	CENTROID_FRONT_RIGHT = 7,
	CENTROID_REAR_RIGHT = 8,
	CENTROID_REAR_LEFT = 9
} CENTROID;
#endif


#if !defined(enum_CORNER_POSN)
#  define enum_CORNER_POSN
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum 
#endif
{
    CORNER_POSN_INVALID   = 0,
    CORNER_POSN_REARL     = 1,
    CORNER_POSN_REARR     = 2,
    CORNER_POSN_FWDL      = 3,
    CORNER_POSN_FWDR      = 4
}CORNER_POSN; /*Fused Object Corner Positions*/
#endif


#if !defined(enum_CORNER_FAR)
#  define enum_CORNER_FAR
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    CORNER_FAR_INVALID = 0,
    CORNER_FAR_LEFT    = 1,
    CORNER_FAR_RIGHT   = 2
} CORNER_FAR;
#endif


#if !defined(enum_CONF9)
#  define enum_CONF9
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum 
#endif
{
    CONF9_NONE  = 0,
    CONF9_LOW1  = 1,
    CONF9_LOW2  = 2,
    CONF9_LOW3  = 3,
    CONF9_LOW4  = 4,
    CONF9_MED1  = 5,
    CONF9_MED2  = 6,
    CONF9_MED3  = 7,
    CONF9_MED4  = 8,
    CONF9_HIGH  = 9
} CONF9;
#endif


#if !defined(enum_CONF)
#  define enum_CONF
#ifdef PCRESIM
typedef enum: unsigned char
#else
typedef enum 
#endif
{
    CONF_NONE  = 0,
    CONF_LOW   = 1,
    CONF_MED   = 2,
    CONF_HIGH  = 3
} CONF;
#endif


#if !defined(enum_CONF2)
#  define enum_CONF2
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum 
#endif
{
    CONF2_NONE  = 0,
    CONF2_LOW   = 1,
    CONF2_HIGH  = 2
} CONF2;
#endif


#if !defined(enum_CONFR2)
#  define enum_CONFR2
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    CONFR2_NOT_RELIABLE  = 0,
    CONFR2_RELIABLE      = 1,
} CONFR2;
#endif


#if !defined(enum_CONFR3)
#  define enum_CONFR3
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    CONFR3_NOT_RELIABLE  = 0,
    CONFR3_RELIABLE_MED  = 1,
    CONFR3_RELIABLE_HI   = 2
} CONFR3;
#endif


#if !defined(enum_HAMMING_CONF)
#  define enum_HAMMING_CONF
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    HAMMING_CONF_LOW   = 0xf0,
    HAMMING_CONF_MED   = 0x69,
    HAMMING_CONF_MEDHI = 0x96,
    HAMMING_CONF_HIGH  = 0xc3
} HAMMING_CONF;
#endif


#if !defined(enum_REGULARIZE_CONF)
#  define enum_REGULARIZE_CONF
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum 
#endif
{
    REGULARIZE_CONF_NONE    = 0,
    REGULARIZE_CONF_LOW     = 1,
    REGULARIZE_CONF_MEDLOW  = 2,
    REGULARIZE_CONF_MEDHIGH = 3,
    REGULARIZE_CONF_HIGH    = 4
} REGULARIZE_CONF;
#endif

#if !defined(enum_PT_CONF)
#  define enum_PT_CONF
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    PT_CONF_NONE   = 0,
    PT_CONF_VEL    = 1,
    PT_CONF_EXTENT = 2
} PT_CONF;
#endif


#if !defined(enum_PASSIVE_CONF)
#  define enum_PASSIVE_CONF
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    PASSIVE_CONF_NONE   = 0,
    PASSIVE_CONF_LOW    = 1,
    PASSIVE_CONF_HIGH    = 2
} PASSIVE_CONF;
#endif
/*fusion types*/

#if !defined(enum_PASSIVE_SRC)
#  define enum_PASSIVE_SRC
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    PASSIVE_SRC_UNKNOWN  = 0,
    PASSIVE_SRC_RADAR    = 1,
    PASSIVE_SRC_VISION   = 2,
    PASSIVE_SRC_FUSED    = 3
} PASSIVE_SRC;
#endif


#if !defined(enum_UNIT)
#  define enum_UNIT
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    UNIT_UNKNOWN             = 0,
    UNIT_NONE                = 1,
    UNIT_METERS_PER_SECOND   = 2,
    UNIT_KILOMETERS_PER_HOUR = 3,
    UNIT_MILES_PER_HOUR      = 4
} UNIT;
#endif


#if !defined(enum_CADS4_SIGNAL_QF)
#  define enum_CADS4_SIGNAL_QF
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{ 
    CADS4_SIGNAL_QF_INVALID    = 255,  
    CADS4_SIGNAL_QF_NOTAVAIL   =  0,
    CADS4_SIGNAL_QF_VALID      =  1,  
} CADS4_SIGNAL_QF;
#endif


#if !defined(enum_QF_ECU)
#  define enum_QF_ECU
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{ 
	QF_ECU_UNDEFINED      = 0, // Undefined (Faulty)
	QF_ECU_TEMP_UNDEFINED = 1, // Temporarily Undefined
	QF_ECU_INACCURATE     = 2, // Inaccurate
	QF_ECU_ACCURATE       = 3  // Accurate
}   QF_ECU;
#endif

// Guardrail
#if !defined(enum_GUARD_REFCURV)
#  define enum_GUARD_REFCURV
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    GUARD_REFCURV_HOST,
    GUARD_REFCURV_NEAR_PARA,
    GUARD_REFCURV_NEAR_INDIV,
    GUARD_REFCURV_FAR,
    GUARD_REFCURV_ROAD_EDGE
} GUARD_REFCURV;
#endif


#if !defined(enum_GUARD_TRKFILT_MODE)
#  define enum_GUARD_TRKFILT_MODE
#ifdef PCRESIM
typedef enum : unsigned char
#else
typedef enum
#endif
{
    GUARD_TRKFILT_MODE_NORMAL,
    GUARD_TRKFILT_MODE_FILTERED_SCORE
} GUARD_TRKFILT_MODE ;
#endif

#endif /* RACAM_ENUMS_H*/
