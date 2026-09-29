#ifndef DC_VERSION_H
#define DC_VERSION_H

#include <cinttypes>
#include "dc_input_data.h"

typedef struct DC_Version_Info_Tag {
   // SRR3 SW version
   uint8_t Release_Revision; // Ascii of X/Y
   uint8_t Promote_Revision; // Ascii of Major
   uint8_t Field_Revision;   // Ascii of Minor
   // SRR3 HW Version
   uint32_t hw_Version;
   // Various version for each stream(avail in MUDP stream header)
   uint8_t strm_version[MAX_ECU_LOGGING_SOURCE];
} DC_Version_Info_T;
#endif // DC_VERSION_H
