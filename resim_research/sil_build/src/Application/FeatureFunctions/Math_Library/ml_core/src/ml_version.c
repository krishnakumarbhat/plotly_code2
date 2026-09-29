/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_version.h"


Version_Number_T Get_Ml_Math_Library_Version(void)
{
   static Version_Number_T shared_toolbox_version = {ML_MATH_LIBRARY_VERSION_YEAR, ML_MATH_LIBRARY_VERSION_MONTH, ML_MATH_LIBRARY_VERSION_DAY, ML_MATH_LIBRARY_VERSION_ITERATION   };

   return shared_toolbox_version;
}
