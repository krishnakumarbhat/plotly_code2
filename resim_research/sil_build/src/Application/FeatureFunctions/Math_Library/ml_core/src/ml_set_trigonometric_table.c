/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_bool.h"
#include "ml_macros.h"
#include "ml_set_trigonometric_table.h"
#include "ml_trigonometry.h"

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
#include "ml_serialization.h"
#endif

Shared_Toolbox_Serialization_Error_T Set_Trig_Table_By_Checksum(const Shared_Toolbox_Serial_Buffer_T *p_buffer)
{
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   Shared_Toolbox_Serialization_Error_T ret_val;
   if (NULL == p_buffer)
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_PTR;
   }
   /* Size of data to serialize + magic bytes + length of cos table + length of atan table */
   else if (p_buffer->length < (sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t)))
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_NOMEM;
   }
   else
   {
      size_t current_byte_local = 0;
      uint32_t magic_number = Serialize_Read_LE(&p_buffer->p_data[0]);
      uint32_t checksum1 = 0;
      uint32_t checksum2 = 0;
      boolean_T f_magic = FALSE;
      current_byte_local += 4;
      if (0xfafbfcfd == magic_number)
      {
         checksum1 = Serialize_Read_LE(&p_buffer->p_data[current_byte_local]);
         current_byte_local += 4;
         checksum2 = Serialize_Read_LE(&p_buffer->p_data[current_byte_local]);
         f_magic = TRUE;
      }
      else if (0xfdfcfbfa == magic_number)
      {
         checksum1 = Serialize_Read_BE(&p_buffer->p_data[current_byte_local]);
         current_byte_local += 4;
         checksum2 = Serialize_Read_BE(&p_buffer->p_data[current_byte_local]);
         f_magic = TRUE;
      }
      else
      {
         /* nothing */
      }
      if (Is_True(f_magic))
      {
#include "ml_trig_tbl_stream_s32r274_windriver.h"
         if ((SHARED_TOOLBOX_TRIG_TBL_STREAM_S32R274_WINDRIVER_CS1 == checksum1) &&
            (SHARED_TOOLBOX_TRIG_TBL_STREAM_S32R274_WINDRIVER_CS2 == checksum2))
         {
            Shared_Toolbox_Serial_Buffer_T st_serialized_buffer;
            st_serialized_buffer.p_data = (char*)&St_Trig_Tbl_Stream_S32r274_Windriver[0]; /* PRQA S 310*/ /* cast needed because serialized buffer needs this type. */
            st_serialized_buffer.length = sizeof(St_Trig_Tbl_Stream_S32r274_Windriver);
            ret_val = Deserialize_Trig_Table(&st_serialized_buffer);
         }
         else
         {
            ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
         }
      }
      else
      {
         ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
      }
   }
   return ret_val;
#else
   (void)p_buffer; /* PRQA S 3112*/ /* Intentionally, suppressing compiler warning in case above code is inactive */
   return SHARED_TOOLBOX_SRL_ERR_UNKNOWN;
#endif
}
