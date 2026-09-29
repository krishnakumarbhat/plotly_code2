/*===================================================================================*\
* FILE: f360_reuse.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION
*
* ABBREVIATIONS
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_REUSE_H
#define F360_REUSE_H

#if defined _MSC_BUILD
#include <stdint.h>
#include <assert.h>
#define __CPTC__ __cplusplus

#elif defined __TASKING__
// Nothing

#elif defined __GNUC__
#include <stdint.h>
#include <assert.h>
#define __CPTC__ __cplusplus

#elif defined __WINDRIVER__
#include <stdint.h>

#else
#error Unrecognized platform!
#endif

#endif
