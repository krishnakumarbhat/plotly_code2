/***
 *udp_record.h - definitions/declarations for UDP Logging
 *
 *       Copyright (c) Delphi Corporation. All rights reserved.
 *
 *Purpose:
 *       This file defines the project- and sensor-independent structures,
 *       values, macros, and functions for UDP logging from Delphi production
 *       and development systems.
 *
 *       [Public]
 *
 *Revision: 0.5
 *
 ****/
#ifndef UDP_RECORD_A1_H
#define UDP_RECORD_A1_H

#include "fixmac.h"
/* UDP Record Header Version
 *    Upper 4 bits contains magic number (0xA).
 *    Lower 4 bits contains revision [0:9].
 */

#define UDP_RECORD_VERSION ((unsigned8_T)0xA3)

/* UDPRecord_Header's Version Info, which is a combination of the version and size of this UDP record header. */
#define UDP_RECORD_VERSIONINFO ((uint16_t)((((uint16_t)(UDP_RECORD_VERSION)) << 8) | sizeof(UDPRecord_Header_T)))

/* \brief A UDPRecord_Header is sent at the beginning of each UDP logging
 *      transmission.  This header describes the source and other attributes of
 *      the data payload that follows it.
 *
 *      An Application should provide one high-level UDP transmit function that
 *      supplies the application-layer info in the header.  A separate UDPStream
 *      transmit function should be provided for for each stream sent.
 *
 */
typedef struct UDPRecord_Header_Tag {
   /*!
    *  Application Layer Info
    */
   uint16_t versionInfo;   //!< Version of this header specification [set to UDP_RECORD_VERSIONINFO]
   uint16_t sourceTxCnt;   //!< Incremented on every transmission from this source.
   uint32_t sourceTxTime;  //!< Timestamp (ms) when packet queued for transmit.
   uint8_t Platform;       //!< Constant identifying sending application [0:63], see udp_sources.h.
   uint8_t Radar_Position; //!< (Set to zero)
   uint8_t reservedSrc2;   //!< (Set to zero)
   uint8_t reservedSrc3;   //!< (Set to zero)

   /*!
    *  Process Layer Info
    */
   uint32_t streamRefIndex; //!< Stream-specific index used for data retrieval (e.g. Look Index for radar)
   uint16_t streamDataLen;  //!< Stream data payload size (bytes).  Size for each stream must be constant.
   uint8_t streamTxCnt;     //!< Increments on each transmission of associated stream number.
   uint8_t streamNumber;    //!< Stream number [0:31]. One source may transmit multiple, asynchronous streams.
   uint8_t streamVersion;   //!< Stream data structure format/version (how to interpret data).
   uint8_t streamChunks;    /*!< A Stream may be transmitted from the process layer as M equal-size
                                 chunks that can be reconstructed on the receive side.
                                 Set to 0 if transmission is not part of a series of chunks (normal mode).
                                 Set to M (>=2) to indicate stream is sent as a series of M chunks.
                                 NOTE: streamTxCnt must increment for each chunk while streamRefIndex
                                 would be the same for each chunk of a series. */
   uint8_t streamChunkIdx;  /*!< Zero when streamChunks is zero. Otherwise, represents this chunk's
                                 position in the reconstruction queue [0:(streamChunks-1)]. */
   uint8_t customerId;      //!< (Set to zero)
} UDPRecord_Header_T;

/* Sample layout of UDP Logging transmission.
 *    This structure is not necessarily use in any source code.
 *    The payload source, size and format is described by the header.
 */
typedef struct UDPRecord_Tag {
   UDPRecord_Header_T header; //!< Record header
   void *payload;             //!< Data payload
} UDPRecord_T;

typedef struct UDP_Release_Version_Tag {
   uint16_t MajorRevision; /*Modified for weekly releases*/
   uint16_t MinorRevision; /*Modify for customer changes*/
} UDP_Release_Version_T;

typedef struct udp_custom_frame_header_Tag {
   UDP_Release_Version_T UDP_Version_info;
   uint32_t utc_time;           /* Latest UTC time available on Flexray at the middle of the radar dwell */
   uint32_t timestamp;          /* Time signal based upon an internal clock that is synchronized to the Flexray Signal SC_Masterzeit */
   uint16_t packetnumber;       /* For Cycle count in case of internal logging */
   uint8_t sensorid;            /* Radar Position */
   uint8_t sensorstatus;        /* 0 - Sensor working, 1- Sensor blind, 2 - Error */
   uint8_t detectioncnt;        /* Number of detections in current scan */
   uint8_t Cal_Running_Cnt;     /* In case of muxed Cal v2, current running count: In case of internal logging this filed is to keep the track of each packet*/
   uint8_t Total_Cal_Chunk_Cnt; /* In case of muxed Cal v2, it is total chunks in the calib data:In case of internal logging it is total number of chunk count  */
   uint8_t CalSource;           /* In case of muxed Cal v2, this is the calsource */
} UDP_CUST_FRAME_HEADER_T;
#endif
