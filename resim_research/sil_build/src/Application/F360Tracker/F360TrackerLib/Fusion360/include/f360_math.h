/*===================================================================================*\
 * FILE: f360_math.h
*====================================================================================
* Copyright (C) 2021, 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains common math functions
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_MATH_H
#define F360_MATH_H

#ifdef __TASKING__

#include <cmath>
inline float F360_Acosf(const float x)                         {return static_cast<float>(acosf(x));}
inline float F360_Asinf(const float x)                         {return static_cast<float>(asinf(x));}
inline float F360_Atan2f(const float y, const float x)         {return static_cast<float>(atan2f(y, x));}
inline float F360_Atanf(const float x)                         {return static_cast<float>(atanf(x));}
inline float F360_Ceilf(const float x)                         {return static_cast<float>(ceilf(x));}
inline float F360_Copysignf(const float f, const float sign)   {return static_cast<float>(copysignf(f,sign));}
inline float F360_Cosf(const float x)                          {return static_cast<float>(cosf(x));}
inline float F360_Expf(const float x)                          {return static_cast<float>(expf(x));}
inline float F360_Floorf(const float x)                        {return static_cast<float>(floorf(x));}
inline float F360_Fmodf(const float x, const float y)          {return static_cast<float>(fmodf(x, y));}
inline float F360_Logf(const float x)                          {return static_cast<float>(logf(x));}
inline float F360_Log10f(const float x)                        {return static_cast<float>(log10f(x)); }
inline float F360_Powf(const float x, const float y)           {return static_cast<float>(powf(x,y));}
inline float F360_Roundf(const float x)                        {return static_cast<float>(roundf(x));}
inline float F360_Sinf(const float x)                          {return static_cast<float>(sinf(x));}
inline float F360_Sqrtf(const float x)                         {return static_cast<float>(sqrtf(x));}
inline float F360_Tanf(const float x)                          {return static_cast<float>(tanf(x));}

#else
#include <cmath>
inline float F360_Acosf(const float x)                {return static_cast<float>(acosf(x));}
inline float F360_Asinf(const float x)                {return static_cast<float>(asinf(x));}
inline float F360_Atan2f(const float y, const float x)       {return static_cast<float>(atan2f(y, x));}
inline float F360_Atanf(const float x)                {return static_cast<float>(atanf(x));}
inline float F360_Ceilf(const float x)                {return static_cast<float>(ceilf(x));}
inline float F360_Copysignf(const float f, const float sign){return static_cast<float>(copysignf(f,sign));}
inline float F360_Cosf(const float x)                 {return static_cast<float>(cosf(x));}
inline float F360_Expf(const float x)                 {return static_cast<float>(expf(x));}
inline float F360_Floorf(const float x)               {return static_cast<float>(floorf(x));}
inline float F360_Fmodf(const float x, const float y)        {return static_cast<float>(fmodf(x, y));}
inline float F360_Logf(const float x)                 {return static_cast<float>(logf(x));}
inline float F360_Log10f(const float x)               {return static_cast<float>(log10f(x)); }
inline float F360_Powf(const float x, const float y)        {return static_cast<float>(powf(x,y));}
inline float F360_Roundf(const float x)               {return static_cast<float>(roundf(x));}
inline float F360_Sinf(const float x)                 {return static_cast<float>(sinf(x));}
inline float F360_Sqrtf(const float x)                {return static_cast<float>(sqrtf(x));}
inline float F360_Tanf(const float x)                 {return static_cast<float>(tanf(x));}

#endif // F360MATH

#endif // F360_MATH_H
