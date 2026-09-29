/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include <iostream>
#include "ml_exp.h"
#include "ml_set_trigonometric_table.h"
#include "ml_trigonometry.h"
#include "ml_serial_buffer_t.h"
#include "ml_serial_buffer_t.h"

int main(int argc, char* argv[])
{
   ::testing::InitGoogleTest(&argc, argv);

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   enum Table_To_Be_Used_T {
      compute, /**< compute the trigonometric tables */
      predef1    /**< use a predefined trigonometric table */
   };
   Table_To_Be_Used_T table_to_be_used = compute;

   /* if any other options are introduced some command line option library should be used! */
   for (int i = 1; i != argc; i++) {
      if (0 == strcmp(argv[1], "--help"))
      {
         std::cout << "\n";
         std::cout << "\n";
         std::cout << "MathLibrary Unit Test Options\n";
         std::cout << "================================\n";
         std::cout << "\n";
         std::cout << "The MathLibrary allows to have several sources for the trigonometric tables.\n";
         std::cout << "The available options are:\n";
         std::cout << "* To compute the tables on start up once use:\n";
         std::cout << "  --st_table=predef1\n";
         std::cout << "* To use tables computed on HW using COMPILER etc  use:\n";
         std::cout << "  --st_table=compute\n";
         std::cout << "\n";
         std::cout << "If no option is given '--st_table=compute' is used.\n";
         std::cout << "\n";
      }
      if (0 == strcmp(argv[1], "--st_table=predef1"))
      {
         table_to_be_used = predef1;
      }

      if (0 == strcmp(argv[1], "--st_table=compute"))
      {
         table_to_be_used = compute;
      }

      /* Shift the remainder of the argv list left by one.  Note
         that argv has (*argc + 1) elements, the last one always being
         NULL.  The following loop moves the trailing NULL element as
         well.*/
      for (int j = i; j != argc; j++) {
         argv[j] = argv[j + 1];
      }

      /* Decrements the argument count. */
      argc--;

      /* We also need to decrement the iterator as we just removed an element.*/
      i--;
   }
   if (compute == table_to_be_used)
   {
      std::cout << "Trigonometric tables are computed now.\n";
      Compute_Trig_Tables();
      Compute_Exp_Table();
   }
   else
   {
      std::cout << "Predefined Trigonometric tables are set now.\n";
      uint32_t checksum_stream_le[] = { 0xfafbfcfd, 0xc362f617, 0x9c203371 };
      Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_le_checksum;
      st_serialized_buffer_le_checksum.p_data = (char*)&checksum_stream_le[0];
      st_serialized_buffer_le_checksum.length = 12;
      Set_Trig_Table_By_Checksum(&st_serialized_buffer_le_checksum);
   }
#endif

   return RUN_ALL_TESTS();
}