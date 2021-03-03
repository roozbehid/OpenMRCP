///////////////////////////////////////////////////////////////////////////////
//
// Dialogic CONFIDENTIAL
//
// Copyright 2006 Dialogic Corporation. All Rights Reserved.
//
// The source code contained or described herein and all documents related to
// the source code ("Material") are owned by Dialogic Corporation or its suppliers
// or licensors. Title to the Material remains with Dialogic Corporation or its
// suppliers and licensors. The Material contains trade secrets and proprietary
// and confidential information of Dialogic or its suppliers and licensors. The
// Material is protected by worldwide copyright and trade secret laws and
// treaty provisions. No part of the Material may be used, copied, reproduced,
// modified, published, uploaded, posted, transmitted, distributed, or
// disclosed in any way without Dialogic’s prior express written permission.
//
// No license under any patent, copyright, trade secret or other Dialogiclectual
// property right is granted to or conferred upon you by disclosure or delivery
// of the Materials, either expressly, by implication, inducement, estoppel or
// otherwise. Any license under such Dialogiclectual property rights must be
// express and approved by Dialogic in writing.
//
///////////////////////////////////////////////////////////////////////////////
// File Name: LoggerDefs.h
//
// Description: Log Level and Log Object Type definitions
//
///////////////////////////////////////////////////////////////////////////////
#ifndef LoggerDefs_h
#define LoggerDefs_h

///////////////////////////////////////////////////////////////////////////////
// Enumeration: ::LogLevelEnum
//
// Description: Log Levels used for setting the level of a message and the 
// logging threshold for an object
//
///////////////////////////////////////////////////////////////////////////////
typedef enum 
{
   LOG_LEVEL_INVALID = 0,
   LOG_LEVEL_ALWAYS,
   LOG_LEVEL_ERROR,
   LOG_LEVEL_WARNING,
   LOG_LEVEL_INFO,
   LOG_LEVEL_DEBUG,
} LogLevelEnum;

///////////////////////////////////////////////////////////////////////////////
// Enumeration: ::LogObjectTypeEnum
//
// Description: Log Object Types that each may have an independent log level 
// threshold settings.
//
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
   LogObject_Invalid = 0,
   LogObject_System,
   LogObject_Resource,
   LogObject_Session,
   LogObject_SessionMgr,
   LogObject_TaskProcessor,
   LogObject_HandleMgr,
   LogObject_CmdIdMgr,
   LogObject_RtpMgr,
   LogObject_Command,
   LogObject_MaxTypes
} LogObjectTypeEnum;

#endif
