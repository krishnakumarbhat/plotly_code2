#ifndef OLP_WRAPPER_H
#define OLP_WRAPPER_H

#include "../../Application/F360Tracker/OLP_Core/includes/olp_iface.h"
#include "../../Application/F360Tracker/VSE_Core/include/VSE_Master_Model_L2_types.h"
#include "f360_host.h"
#include "f360_log_types.h"
#include "f360_rot_object_log.h"

/******************************************************************************
 *  Function prototypes
 *****************************************************************************/

void InitOLP(void);
void RunOLP(f360_variant_A::F360_Object_Log_Output_T *obj, ROT_Object_List_Info_T *rot_obj, VSE_OUT *vse_info, f360_variant_A::F360_Host_T *host);
Olp_Data_T *GetOLPObjectsData(void);

#endif
