#ifndef DVL_MESSAGE_H
#define DVL_MESSAGE_H

#include <stdint.h>

#define DVL_MESSAGE_TYPE_EVENT  (0)
#define DVL_MESSAGE_TYPE_CAN    (1)
#define DVL_MESSAGE_TYPE_FLEX   (2)
#define DVL_MESSAGE_TYPE_CAN_FD (3)

#define DVL_MESSAGE_PAYLOAD_SIZE_CAN     (8)
#define DVL_MESSAGE_PAYLOAD_SIZE_CANFD   (64)
#define DVL_MESSAGE_PAYLOAD_SIZE_FLEXRAY (254)
#define DVL_MESSAGE_PAYLOAD_SIZE_EVENT   (10)

#define DVL_MESSAGE_FLAG_OVERRUN (0x01)
#define DVL_MESSAGE_FLAG_ERROR   (0x02)

#pragma pack(push, save_pack)
#pragma pack(push, 2)

// FlexRay HdrFlags
static const unsigned short DVL_FR_FRAMEFLAG_STARTUP          = 0x0001; //!< indicates a startup frame
static const unsigned short DVL_FR_FRAMEFLAG_SYNC             = 0x0002; //!< indicates a sync frame
static const unsigned short DVL_FR_FRAMEFLAG_NULLFRAME        = 0x0004; //!< indicates a nullframe
static const unsigned short DVL_FR_FRAMEFLAG_PAYLOAD_PREAMBLE = 0x0008; //!< indicates a present payload preamble bit
static const unsigned short DVL_FR_FRAMEFLAG_FR_RESERVED      = 0x0010; //!< reserved by Flexray protocol

typedef enum {
   E_DVL_EVENT_TIMESTAMP = 0,
   E_DVL_EVENT_CAN,         // ERRORFRAME, OVERRUN, etc
   E_DVL_EVENT_FLEXRAY,     // ERRORFRAME, OVERRUN, etc
   E_DVL_EVENT_VIDEOSYNC,   // SYNC
   E_DVL_EVENT_VIDEOERROR,  // FRAMEDROP, etc
   E_DVL_EVENT_APPLICATION, // CANTRIGGER, USER ACTION, etc
   E_DVL_EVENT_UNDEFINED
} dvlEventType;

typedef struct {
   uint64_t timestamp;   // microseconds
   uint32_t messageSize; //
   uint16_t messageType; // DVL_MESSAGE_TYPE_XXX
   uint16_t reserved;    //
} dvlMessageInfo;

typedef struct {
   dvlMessageInfo info; //
   // put the rest of this in dvlCanFrame?
   uint32_t id;                                // CAN id
   uint8_t extFrame;                           // 1=extended, 0=standard
   uint8_t channel;                            // CAN channel
   uint8_t length;                             // data length in bytes
   uint8_t data[DVL_MESSAGE_PAYLOAD_SIZE_CAN]; // message data//DVL_MESSAGE_PAYLOAD_SIZE_CAN
   uint8_t flags;                              // DVL_MESSAGE_FLAG_OVERRUN, etc
} dvlCanMessage;

typedef struct {
   dvlMessageInfo info; //
   // put the rest of this in dvlCanFrame?
   uint32_t id;                                  // CAN id
   uint8_t extFrame;                             // 1=extended, 0=standard
   uint8_t channel;                              // CAN channel
   uint8_t length;                               // data length in bytes
   uint8_t data[DVL_MESSAGE_PAYLOAD_SIZE_CANFD]; // message data//DVL_MESSAGE_PAYLOAD_SIZE_CAN
   uint8_t flags;                                // DVL_MESSAGE_FLAG_OVERRUN, etc
   uint8_t fd_brs;                               // 0 = Classical CAN, 1 = Second bit-rate applied to data field
} dvlCanMessageFD;

typedef struct {
   dvlMessageInfo info; //
   dvlEventType eventType;
   uint16_t subType;
   uint8_t data[DVL_MESSAGE_PAYLOAD_SIZE_EVENT];
} dvlEventMessage;

typedef struct {
   dvlMessageInfo info;  //
   uint16_t slotID;      // {0..2047}
   uint16_t headerFlags; // might not need this... check data (lots of bits for not much info)
   uint8_t cycleCount;   // {0..63}
   uint8_t channel;      // {0..15}
   uint8_t payloadLen;   // bytes {0..254}
   uint8_t flags;        // DVL_MESSAGE_FLAG_OVERRUN, etc
   uint8_t data[DVL_MESSAGE_PAYLOAD_SIZE_FLEXRAY];
} dvlFlexrayMessage;

typedef union {
   dvlMessageInfo info; //
   dvlEventMessage dvlEvent;
   dvlCanMessage dvlCAN;
   dvlFlexrayMessage dvlFlex;
   dvlCanMessageFD dvlCANFD;
} dvlMessageUnion;
#pragma pack(pop, save_pack)
#endif // DVL_MESSAGE_H
