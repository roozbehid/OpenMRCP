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
//	File Name:		CMrcpSessionMgr.cpp
//
//	Description:	CMrcpSessionMgr class implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpSessionMgr::~CMrcpSessionMgr();
//		int OpenSession(RESOURCE_CFG_STRUCT* a_resourceCfg);
//		int AddSessionResources(CMrcpSession* a_session,MrcpResourceType a_resourceToAdd);
//		int CloseSession(MrcpSessionHandle a_sessionHandle);
//		void Shutdown();
//		void HandleEvent(CMrcpEvent* a_event);
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  4/1/07      TMB         Cleanup
///////////////////////////////////////////////////////////////////////////////
#include "CMrcpSessionMgr.h"
#include "CMrcpSession.h"
#include "ClientInterfaceDefs.h"
#include "CMrcpHandleMgr.h"
#include "MrcpClientEvents.h"
#include "CMrcpEvent.h"
#include "MrcpUtils.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          Register as observer for session objects stopping
///////////////////////////////////////////////////////////////////////////
CMrcpSessionMgr::CMrcpSessionMgr()
	: m_handleMgr(CMrcpHandleMgr::Instance())
{
	m_handleMgr->RegisterForNotification(this, SESSIONS_STOPPED);
	m_state = SESSION_MANGR_STARTING;
	ClassName("CMrcpSessionMgr");
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSessionMgr::~CMrcpSessionMgr()
{
	m_handleMgr->Unregister(this, SESSIONS_STOPPED);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StopAllSessions
// Input - None
//         
// Output - None
//          clean up - when in stopped state, notify observers
///////////////////////////////////////////////////////////////////////////
void CMrcpSessionMgr::Shutdown()
{
	Name("Shutdown");

    CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering Shutdown");		
	m_handleMgr->Cleanup();
	while (m_state != SESSION_MANGR_STOPPED)
	{
		MrcpUtils::MsSleep(500);
	}
    CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Exiting Shutdown");				
	NotifyObservers(SESSION_MGR_STOPPED);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OpenSession
// Input - resource config structure
//         
// Output - None
//      instantiate a CMrcpSession object if same server address
//      else 2 separate session objects
//      instantiate audiostream object
//		instantiate rtp stream object 
//      start CMrcpSession(audio stream ptr, rtp stream ptr)
// 
///////////////////////////////////////////////////////////////////////////
int CMrcpSessionMgr::OpenSession(RESOURCE_CFG_STRUCT* a_resourceCfg)
{
	Name("OpenSession");
	bool l_ttsServer = true;
	bool l_asrServer = true;
	CMrcpSession* l_sessionPtr;

	CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Entering OpenSession");

	if (!strcmp(a_resourceCfg->asrServerConfig->serverAddress, "\n"))
		l_asrServer = false;
	if (!strcmp(a_resourceCfg->ttsServerConfig->serverAddress, "\n"))
		l_ttsServer = false;

	if (l_asrServer && l_ttsServer)
	{
		if (!strcmp(a_resourceCfg->asrServerConfig->serverAddress, a_resourceCfg->ttsServerConfig->serverAddress ))
		{
			l_sessionPtr = new CMrcpSession(a_resourceCfg, l_asrServer, l_ttsServer);
			l_sessionPtr->StartSession();
		}
	}
	else
	{
		if (l_asrServer)
		{
			l_sessionPtr = new CMrcpSession(a_resourceCfg, l_asrServer,l_ttsServer);
			l_sessionPtr->StartSession();
		}
		if (l_ttsServer)
		{				
			l_sessionPtr = new CMrcpSession(a_resourceCfg, l_asrServer, l_ttsServer);
			l_sessionPtr->StartSession();
		}
	}
	m_state = SESSION_MANGR_STARTED;
	return MrcpSuccess;
	}

////////////////////////////////////////////////////////////////////////// 
//
// Description - AddSessionResources
// Input - session pointer
//         resource type
//
// Output - status of call to add resource to session
//
// validate that at least and only one session pointer is sent
// use that session ptr to add configured resource to the session
///////////////////////////////////////////////////////////////////////////
int CMrcpSessionMgr::AddSessionResources(CMrcpSession* a_session,MrcpResourceType a_resourceToAdd)
{
	Name("AddSessionResources");
    CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Entering AddSessionResources");

	return a_session->AddResourceToSession(a_resourceToAdd);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - CloseSession
// Input - Session handle
//         
// Output - success or failure
//      get object ptr using session handle
//      stop CMrcpSession
//      unregister session ptr with handle manager
//		delete object - will need observer notification
///////////////////////////////////////////////////////////////////////////
int CMrcpSessionMgr::CloseSession(MrcpSessionHandle a_sessionHandle)
{
	Name("CloseSession");
	CMrcpSession* l_mrcpSession = NULL;
    CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Entering CloseSession");
	int l_status;

	l_mrcpSession = m_handleMgr->RetrievePointer(a_sessionHandle);
	if (NULL == l_mrcpSession)
	{			
		std::string l_errorInfo = "Handle not found " + AsString(a_sessionHandle);
    	CLogger::Instance()->Log( LOG_LEVEL_ERROR, *this,l_errorInfo);
		return -1;
	}

	l_status = l_mrcpSession->StopSession();

	if (MrcpSuccess == l_status)
		delete l_mrcpSession;

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - HandleEvent
// Input - pointer to event object
//         
// Output - None
//      notify observers that the signaling session has been established.
///////////////////////////////////////////////////////////////////////////
void CMrcpSessionMgr::HandleEvent(CMrcpEvent* a_event)
{
	Name("HandleEvent");

	switch (a_event->EventID())
	{
	case SESSIONS_STOPPED:
    	CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Sessions Stopped received");
		m_state = SESSION_MANGR_STOPPED;
		break;
	}
}
}//end namespace