/*===================================================================================*\
* FILE: ocg_version.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains OCGVersion() class definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef OCG_VERSION_H
#define OCG_VERSION_H

namespace ocg {

   class OCGVersion
   {
      public:
         static unsigned int getMajor() { return m_major; };
         static unsigned int getMinor() { return m_minor; };
         static unsigned int getPatch() { return m_patch; };

      private:
         static constexpr unsigned int m_major = 1;
         static constexpr unsigned int m_minor = 10;
         static constexpr unsigned int m_patch = 1;
   };

}

#endif
