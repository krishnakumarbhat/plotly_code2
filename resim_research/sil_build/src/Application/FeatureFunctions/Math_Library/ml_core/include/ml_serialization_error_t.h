#ifndef ML_SERIALIZATION_ERROR_T_H
#define ML_SERIALIZATION_ERROR_T_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/**
* Used to indicate result from serialization functions.
* \ingroup trigonometric_functions_serialization
*/
typedef enum Shared_Toolbox_Serialization_Error_Tag
{
   SHARED_TOOLBOX_SRL_SUCCESS     /**< Function returned successfully, operation completed */,
   SHARED_TOOLBOX_SRL_ERR_NOMEM   /**< Memory related error */,
   SHARED_TOOLBOX_SRL_ERR_PTR     /**< Invalid Pointer */,
   SHARED_TOOLBOX_SRL_ERR_PARSE   /**< Parsing related error */,
   SHARED_TOOLBOX_SRL_ERR_UNKNOWN /**< Unknown Error  */
} Shared_Toolbox_Serialization_Error_T;

#ifdef __cplusplus
}
#endif
#endif
