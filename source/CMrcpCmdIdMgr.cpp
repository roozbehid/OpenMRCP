///////////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2006, Dialogic Corporation
//	All rights reserved.
//
//	Redistribution and use in source and binary forms, with or without
//	modification, are permitted provided that the following conditions are met:
//		* Redistributions of source code must retain the above copyright notice,
//		  this list of conditions and the following disclaimer.
//		* Redistributions in binary form must reproduce the above copyright
//		  notice, this list of conditions and the following disclaimer in the
//		  documentation and/or other materials provided with the distribution.
//		* Neither the name of the Dialogic Corporation nor the names of its
//		  contributors may be used to endorse or promote products derived from
//		  this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
//	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
//	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//	POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////////
//	File Name:		MrcpCmdIdMgr.cpp
//
//	Description:	MrcpCmdIdMgr class implementation
//					Provides unique request ids for all MRCP commands sent from library
//					Provides a map of request ids to callback routines amd a lookup into map
//				    Provides means to unregister cmd id and callbacks
//					Provides method to queue callbacks for processing to callback task processor
//					
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpCmdIdMgr::CMrcpCmdIdMgr()
//		CMrcpCmdIdMgr::~CMrcpCmdIdMgr()
//		MrcpCallBackFunction CMrcpCmdIdMgr::GetCommandObj(long a_id)
//		int CMrcpCmdIdMgr::RegisterCommandObj(long a_id, MrcpCallBackFunction a_cmdobj)
//		int CMrcpCmdIdMgr::UnRegisterObject(long a_id)

///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  1/2/07      TMB         Modified to return handles in callback structure
//  3/3/21		Roozbeh G	Boost removal
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpCmdIdMgr.h"
#include "ClientInterfaceDefs.h"
#include "CMrcpCallbackTask.h"
#include "CMrcpTaskProcessor.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor 
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

CMrcpCmdIdMgr::CMrcpCmdIdMgr()
	:m_sequenceID (1001)
	,m_recSeqId (1001)
	,m_synSeqId (1001)

{
	ClassName("CMrcpCmdIdMgr");
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

CMrcpCmdIdMgr::~CMrcpCmdIdMgr()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - gets command object 
// Input - Mrcp Request ID
//         
// Output - returns pointer to object issuing command
//          Performs a lookup in the map of request ids to callbacks
//			returns callback routine
///////////////////////////////////////////////////////////////////////////

MrcpCallBackFunction CMrcpCmdIdMgr::GetCommandObj(long a_id)
{  //scope for lock
	std::lock_guard<std::mutex> lock( m_criticalSection);
	if ( m_cmdIdMap.find(a_id) == m_cmdIdMap.end())
		return MrcpSuccess;
	else
		return m_cmdIdMap[a_id];

}

////////////////////////////////////////////////////////////////////////// 
//
// Input - Mrcp Request ID, pointer to command object
//         
// Output - success or failure
//     registers the object issuing command
//      adds a map entry of command id to command sequence id    
///////////////////////////////////////////////////////////////////////////
int CMrcpCmdIdMgr::RegisterCommandObj(long a_id, MrcpCallBackFunction a_cmdobj)
{
		Name("RegisterCommandObj");

	if ( a_cmdobj == NULL)
	{
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Command Object is NULL");
		return -1;
	}

	if ( GetCommandObj(a_id) )
	{
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Command Object not found on lookup");
		return -1;
	}
	{//scope for lock
	std::lock_guard<std::mutex> lock( m_criticalSection);
	m_cmdIdMap[a_id] = a_cmdobj;
	}
	return MrcpSuccess;

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - unregisters a command object 
// Input - Mrcp Request ID
//         
// Output - success or failure
//			unregisters the object issuing command
//         remove entry from map of sequence id to callback 
///////////////////////////////////////////////////////////////////////////

int CMrcpCmdIdMgr::UnRegisterObject(long a_id)
{
    Name("UnRegisterObject");
	CmdByIDMap::iterator l_pos;

	{ //scope for lock
	std::lock_guard<std::mutex> lock( m_criticalSection);
	if ( (l_pos = m_cmdIdMap.find(a_id)) == m_cmdIdMap.end())
	{
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Command Object not found");
		return -1;
	}
	m_cmdIdMap.erase(l_pos);
	}
	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - returns a unique requestID to be used to by command obj 
// Input - None
//         
// Output - requestId
//       returns a unique sequence id to be used in the MRCP call to the server
//		 sequence id should be used by client application to associate asynch
//		 statuses to MRCP commands issued.
///////////////////////////////////////////////////////////////////////////
int CMrcpCmdIdMgr::GetReferenceId()
{  //scope for lock
	std::lock_guard<std::mutex> lock( m_controlMutex);
	m_sequenceID++;

	return m_sequenceID;

}
////////////////////////////////////////////////////////////////////////// 
//
// Input - command result structure
//         command complete indicator
//		   callback function
// Output - success or failure
//       looks up callback routine associated with a sequence id,
//       queue's callback command and submits to callback task processor queue
//               takes precedance over default call back for Server 
///////////////////////////////////////////////////////////////////////////
int CMrcpCmdIdMgr::PerformCallbackProcessing(MRCP_RESULT_STRUCT a_callResults,bool a_commandComplete, MrcpCallBackFunction a_defaultCallBack)
{
	Name("PerformCallbackProcessing");
	CmdByIDMap::iterator l_callBackItr;
	MrcpCallBackFunction l_callBackRtn;
	
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Entering Routine");

	{//scope for lock
	std::lock_guard<std::mutex> lock (m_criticalSection);

	if ( (l_callBackItr = m_cmdIdMap.find(a_callResults.referenceNumber)) == m_cmdIdMap.end())
		if (a_defaultCallBack != NULL)
			l_callBackRtn = a_defaultCallBack;
		else
		{
		   CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Callback not found - default not set");
			return -1;
		}
	else
	{
		l_callBackRtn = (*l_callBackItr).second;

		if (a_commandComplete)
			m_cmdIdMap.erase(l_callBackItr);
	}
	}//end scope lock

	CMrcpCallbackTask* l_callbackTask = new CMrcpCallbackTask();
	l_callbackTask->CallBackRtn(l_callBackRtn);
	l_callbackTask->CallResults(a_callResults);
	CMrcpTaskProcessor::Instance("CALLBACK")->QueueTask( l_callbackTask);

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Exiting Routine");

	return MrcpSuccess;
}
}// end namespace