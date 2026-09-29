
#include "olp_iface.h"
#include "olp.h"

namespace olp
{
   static olp_core olp_core_obj;

#ifdef __cplusplus
   extern "C" unsigned char OLP_Get_Sw_Major_Version(void)
#else
   unsigned char OLP_Get_Sw_Major_Version(void)
#endif
   {
      return (OLP_SW_MAJOR_VERSION);
   }

#ifdef __cplusplus
   extern "C" unsigned char OLP_Get_Sw_Minor_Version(void)
#else
   unsigned char OLP_Get_Sw_Minor_Version(void)
#endif
   {
      return (OLP_SW_MINOR_VERSION);
   }

#ifdef __cplusplus
   extern "C" unsigned char OLP_Get_Sw_Patch_Version(void)
#else
   unsigned char OLP_Get_Sw_Patch_Version(void)
#endif
   {
      return (OLP_SW_PATCH_VERSION);
   }

#ifdef __cplusplus
extern "C" void OLP_Init(void)
#else
void OLP_Init(void)
#endif
   {
      olp_core_obj.OLP_Init_Hook();
   }

#ifdef __cplusplus
   extern "C" void OLP_Main_Run_50ms(Olp_Data_T *olp_data_ref)
#else
   void OLP_Main_Run_50ms(Olp_Data_T &olp_data_ref)
#endif
   {
      olp_core_obj.OLP_Main_Hook(*olp_data_ref);
   }
}
