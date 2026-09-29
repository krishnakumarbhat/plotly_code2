#ifndef STREAM_HEADER_H
#define STREAM_HEADER_H
/*===========================================================================*\
* FILE: stream_header.h
*===========================================================================
* Copyright � 2020 Aptiv Advanced Safety and User Experience. All rights
* reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*
* DESCRIPTION:
*       Provide a common definition of UDP source IDs across programs.
*       Stream allocation is application-specific and defined elsewhere.
*
* ABBREVIATIONS:
*   List of abbreviations used, or reference(s) to external document(s)
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [30-Mar-2018]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "reuse.h"

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/

/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/
/* stream header size same as Stream_Hdr_T */
#define LOG_DATA_HDR_LEN (uint16_t)(sizeof(Stream_Hdr_T))
/* Define MCARO FOR Unused Signal */
#define LOG_SIG_NOT_IMPLEMENTED (0U)
/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/
#ifndef LITTLE_ENDIAN_STRUCTURE
/* common stream header for all regular streams */
typedef struct Stream_Hdr_Tag {
   uint32_t size;           /* stream size */
   uint16_t version;        /* stream version */
   uint16_t checksum;       /* checksum of stream */
   uint16_t scan_index;     /* scan index */
   uint16_t error_info;     /* error information */
   uint32_t module_time_ms; /* module time in ms */
} Stream_Hdr_T;
/* Add common stream header for all dynamic streams */
#else
typedef struct Stream_Hdr_Tag {
   uint32_t module_time_ms; /* module time in ms */
   uint16_t error_info;     /* error information */
   uint16_t scan_index;     /* scan index */
   uint16_t checksum;       /* checksum of stream */
   uint16_t version;        /* stream version */
   uint32_t size;           /* stream size */
} Stream_Hdr_T;

typedef struct Dyn_Hdr_Tag {
   uint16_t scan_index;
   uint16_t records_cnt;
} Dyn_Hdr_T;
#endif

/*===========================================================================*
 * Exported Const Object Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/

/*===========================================================================*
 * Exported Inline Function Definitions and #define Function-Like Macros
 *===========================================================================*/

/*===========================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *===========================================================================
 *
 * Date        userid   (Description on following lines: SCR #, etc.)
 * --------    -----    ------------------------------------------------------
 * 30-Jun-2022 bz571t   DDR-1713: [SRR7p] Stream_Hdr_T shall be commonly defined
 * 14-Jul-2022 tjy9rb   DDR-1665: [SRR7p] Update the contents of stream header
 * 23-Aug-2024 vjvktc   DNP-5049: [SRR7P] Added a macro LOG_SIG_NOT_IMPLEMENTED
\*===========================================================================*/
#endif /* STREAM_HEADER_H */
