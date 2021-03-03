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
// disclosed in any way without Dialogicís prior express written permission.
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

#include <boost/shared_ptr.hpp>

#include <sstream>
#include <map>

class CConfigOptions;
typedef boost::shared_ptr< CConfigOptions> CConfigOptionsSptr;

class ATask;
typedef boost::shared_ptr< ATask> ATaskSptr;

class CTaskProcessor;
typedef boost::shared_ptr<CTaskProcessor> CTaskProcessorSptr;

class CLogger;
typedef boost::shared_ptr<CLogger> CLoggerSptr;

typedef boost::shared_ptr< std::stringstream> LogItemSptr;

class ACallflow;
typedef boost::shared_ptr<ACallflow> ACallflowSptr;

class CCallflow;
typedef boost::shared_ptr<CCallflow> CCallflowSptr;

class CCallflowManager;
typedef boost::shared_ptr<CCallflowManager> CCallflowManagerSptr;

class AResourceGroup;
typedef boost::shared_ptr<AResourceGroup> AResourceGroupSptr;

class CResourceGroup;
typedef boost::shared_ptr<CResourceGroup> CResourceGroupSptr;

class CResourceGroupManager;
typedef boost::shared_ptr<CResourceGroupManager> CResourceGroupManagerSptr;

class ASipResource;
typedef boost::shared_ptr<ASipResource> ASipResourceSptr;

class ASipBoardResource;
typedef boost::shared_ptr<ASipBoardResource> ASipBoardResourceSptr;

class CSipResource;
typedef boost::shared_ptr<CSipResource> CSipResourceSptr;

class CSipBoardResource;
typedef boost::shared_ptr<CSipBoardResource> CSipBoardResourceSptr;

class CSipResourceManager;
typedef boost::shared_ptr<CSipResourceManager> CSipResourceManagerSptr;

class CResourceManager;
typedef boost::shared_ptr<CResourceManager> CResourceManagerSptr;

class CSrlEventManager;
typedef boost::shared_ptr< CSrlEventManager> CSrlEventManagerSptr;

class ATaskHandler;
typedef std::map< ATaskHandler*, ATaskSptr> ATaskHandlerList;
typedef std::map< int, ATaskHandlerList *> ATaskHandlerListByNotificationMap;
typedef std::map< long, ATaskHandlerList *> ATaskHandlerListByEventMap;

class CMetaEvent;
typedef boost::shared_ptr< CMetaEvent> CMetaEventSptr;

class CSipCallInfo;
typedef boost::shared_ptr< CSipCallInfo> CSipCallInfoSptr;

class AGlobalCallManager;
typedef boost::shared_ptr<AGlobalCallManager> AGlobalCallManagerSptr;

class CGlobalCallManager;
typedef boost::shared_ptr<CGlobalCallManager> CGlobalCallManagerSptr;

class ADialog;
typedef boost::shared_ptr<ADialog> ADialogSptr;

class CMxmlEvent;
typedef boost::shared_ptr<CMxmlEvent> CMxmlEventSptr;

#endif

