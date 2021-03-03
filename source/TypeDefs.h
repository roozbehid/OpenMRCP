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
// File Name: TypeDefs.h
//
// Description: Common type definitions
//
///////////////////////////////////////////////////////////////////////////////
#ifndef TypeDefs_h
#define TypeDefs_h

//#include <boost/std::shared_ptr.hpp>

#include <sstream>
#include <map>
#include <memory>

class CConfigOptions;
typedef std::shared_ptr< CConfigOptions> CConfigOptionsSptr;

class ATask;
typedef std::shared_ptr< ATask> ATaskSptr;

class CTaskProcessor;
typedef std::shared_ptr<CTaskProcessor> CTaskProcessorSptr;

class CLogger;
typedef std::shared_ptr<CLogger> CLoggerSptr;

typedef std::shared_ptr< std::stringstream> LogItemSptr;

class ACallflow;
typedef std::shared_ptr<ACallflow> ACallflowSptr;

class CCallflow;
typedef std::shared_ptr<CCallflow> CCallflowSptr;

class CCallflowManager;
typedef std::shared_ptr<CCallflowManager> CCallflowManagerSptr;

class AResourceGroup;
typedef std::shared_ptr<AResourceGroup> AResourceGroupSptr;

class CResourceGroup;
typedef std::shared_ptr<CResourceGroup> CResourceGroupSptr;

class CResourceGroupManager;
typedef std::shared_ptr<CResourceGroupManager> CResourceGroupManagerSptr;

class ASipResource;
typedef std::shared_ptr<ASipResource> ASipResourceSptr;

class ASipBoardResource;
typedef std::shared_ptr<ASipBoardResource> ASipBoardResourceSptr;

class CSipResource;
typedef std::shared_ptr<CSipResource> CSipResourceSptr;

class CSipBoardResource;
typedef std::shared_ptr<CSipBoardResource> CSipBoardResourceSptr;

class CSipResourceManager;
typedef std::shared_ptr<CSipResourceManager> CSipResourceManagerSptr;

class CResourceManager;
typedef std::shared_ptr<CResourceManager> CResourceManagerSptr;

class CSrlEventManager;
typedef std::shared_ptr< CSrlEventManager> CSrlEventManagerSptr;

class ATaskHandler;
typedef std::map< ATaskHandler*, ATaskSptr> ATaskHandlerList;
typedef std::map< int, ATaskHandlerList *> ATaskHandlerListByNotificationMap;
typedef std::map< long, ATaskHandlerList *> ATaskHandlerListByEventMap;

class CMetaEvent;
typedef std::shared_ptr< CMetaEvent> CMetaEventSptr;

class CSipCallInfo;
typedef std::shared_ptr< CSipCallInfo> CSipCallInfoSptr;

class AGlobalCallManager;
typedef std::shared_ptr<AGlobalCallManager> AGlobalCallManagerSptr;

class CGlobalCallManager;
typedef std::shared_ptr<CGlobalCallManager> CGlobalCallManagerSptr;

class ADialog;
typedef std::shared_ptr<ADialog> ADialogSptr;

class CMxmlEvent;
typedef std::shared_ptr<CMxmlEvent> CMxmlEventSptr;

#endif

