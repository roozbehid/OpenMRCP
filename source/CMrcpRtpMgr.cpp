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
//	File Name:		CMrcpRtpMgr.cpp
//
//	Description:	CMrcpRtpMgr class implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpRtpMgr::CMrcpRtpMgr()
//		CMrcpRtpMgr::~CMrcpRtpMgr()
//		int CMrcpRtpMgr::GetRtpPort()
//		int CMrcpRtpMgr::SetupSession(RESOURCE_CFG_STRUCT* a_resourceCfg, int a_clientRtpPort, int a_serverRtpPort)
//		int CMrcpRtpMgr::StopSession(MrcpAudioHandle l_audioHandle)
//		int CMrcpRtpMgr::Shutdown()
//		void CMrcpRtpMgr::HandleEvent(CMrcpEvent* a_event)
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  4/1/07      TMB         Cleanup
//  3/3/21		Roozbeh G	Boost removal
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpRtpMgr.h"
#include "CMrcpAudioStream.h"
#include "CMrcpRtp.h"
#include "ClientInterfaceDefs.h"
#include "CMrcpHandleMgr.h"
#include "CMrcpEvent.h"
#include "MrcpClientEvents.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
// Description - Constructor 
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

CMrcpRtpMgr::CMrcpRtpMgr()
:m_rtpAudioPort (41000)
,m_handleMgr(CMrcpHandleMgr::Instance())
,m_rtpSessionCnt(0)
,m_shutdownIndicator (false)
{
	ClassName("CMrcpRtpMgr");
}
////////////////////////////////////////////////////////////////////////// 
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

CMrcpRtpMgr::~CMrcpRtpMgr()
{
	Unregister(this, RTP_SESSION_STOPPED);
}
////////////////////////////////////////////////////////////////////////// 
// Description - GetRtpPort
// Input - None
//         
// Output - rtp audio port
//          Returns a rtp port for a session.  Lock ensures that the port
//		    is unique
///////////////////////////////////////////////////////////////////////////
int CMrcpRtpMgr::GetRtpPort()
{
	std::lock_guard<std::mutex> lock( m_controlMutex);

	if (m_rtpAudioPort > 51000)
		m_rtpAudioPort = 41000;
	m_rtpAudioPort += 2;  //need to account for Rtcp port

	return m_rtpAudioPort;

}

////////////////////////////////////////////////////////////////////////// 
// Description - SetupSession
// Input - config structure
//         client rtp port
//		   server rtp port	
// Output - success
//          Instantiates a audiostream and a rtp object for the session
//          sets the audio stream handle for use by client application
///////////////////////////////////////////////////////////////////////////
int CMrcpRtpMgr::SetupSession(RESOURCE_CFG_STRUCT* a_resourceCfg, int a_clientRtpPort, int a_serverRtpPort)
{
	MrcpAudioHandle l_audioHandle = 0;
	CMrcpAudioStream* l_audioStream;
	CMrcpRtp* l_mrcpRtp;

	Name("SetupSession");
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Entering");
	RegisterForNotification(this,RTP_SESSION_STOPPED);

	if (strcmp(a_resourceCfg->asrServerConfig->serverAddress, "\n"))
	{
		CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "ASR audio setup");
		l_mrcpRtp = new CMrcpRtp(a_resourceCfg, a_serverRtpPort, a_clientRtpPort);
		m_rtpSessionCnt++;
		l_audioStream = new CMrcpAudioStream(l_mrcpRtp);
		l_audioHandle = m_handleMgr->RegisterAudioPointer(l_audioStream);
		a_resourceCfg->outboundAudioHandle = l_audioHandle;
		if (strcmp(a_resourceCfg->ttsServerConfig->serverAddress, "\n"))
			a_resourceCfg->inboundAudioHandle = l_audioHandle;
		m_streamToRtpMap[l_audioHandle] = l_mrcpRtp;
		l_mrcpRtp->Start(l_audioStream);
	}

	if (strcmp(a_resourceCfg->ttsServerConfig->serverAddress, "\n")
		&& !strcmp(a_resourceCfg->asrServerConfig->serverAddress, "\n"))
	{
		CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "TTS audio setup");
		l_mrcpRtp = new CMrcpRtp(a_resourceCfg, a_clientRtpPort);
		m_rtpSessionCnt++;
		l_audioStream = new CMrcpAudioStream(l_mrcpRtp);
		l_audioHandle = m_handleMgr->RegisterAudioPointer(l_audioStream);
		a_resourceCfg->inboundAudioHandle = l_audioHandle;
		m_streamToRtpMap[l_audioHandle] = l_mrcpRtp;
		l_mrcpRtp->Start(l_audioStream);
	}

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - StopSession
// Input - audio handle
//         
// Output - success
//          Signals shutdown to the audiostream and a rtp object for a session
//          removes the audio handle to rtp map entry
///////////////////////////////////////////////////////////////////////////
int CMrcpRtpMgr::StopSession(MrcpAudioHandle l_audioHandle)
{
	Name("StopSession");
	StreamToRtpPointerMap::iterator l_pos;
	CMrcpAudioStream* l_audioStreamObj;

	{//scope for lock
	std::lock_guard<std::mutex> lock( m_criticalSection);
	if ((l_pos = m_streamToRtpMap.find(l_audioHandle)) == m_streamToRtpMap.end())
	{
		CLogger::Instance()->Log(LOG_LEVEL_WARNING, *this, " audio handle ptr not found");
		return -1;
	}
	l_pos->second->Stop();
	m_streamToRtpMap.erase(l_pos);
	}//end lock scope

	//the audiostream cleanup
		l_audioStreamObj = m_handleMgr->RetrieveAudioPointer(l_audioHandle);  //retrieve the audio object
	if(NULL != l_audioStreamObj)
	{
		l_audioStreamObj->Stop();  	//call the object stop method
		m_handleMgr->UnregisterPointer(l_audioHandle, "Audio");//cleanup handle mgr entry
	//	Sleep(2000);
		delete l_audioStreamObj; //delete obj
	}

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - Shutdown
// Input - None
//         
// Output - success
//          Signals shutdown to all audiostream and a rtp objects
//         
///////////////////////////////////////////////////////////////////////////
int CMrcpRtpMgr::Shutdown()
{
	Name("Shutdown");
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Entering ");

	StreamToRtpPointerMap::iterator l_pos;
	m_shutdownIndicator = true;

	for(l_pos = m_streamToRtpMap.begin(); l_pos != m_streamToRtpMap.end(); l_pos++)
	{
		StopSession(l_pos->first);
	}

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Exiting ");
	
return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - HandleEvent
// Input - event pointer
//         
// Output - None
//      notify observers that all the rtp sessions has been shutdown.
///////////////////////////////////////////////////////////////////////////
void CMrcpRtpMgr::HandleEvent(CMrcpEvent* a_event)
{
	Name("HandleEvent");

	switch (a_event->EventID())
	{
	case RTP_SESSION_STOPPED:
    	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this,"RTP Session Stopped");
		m_rtpSessionCnt--;
		if (!m_rtpSessionCnt && m_shutdownIndicator)
			NotifyObservers(RTP_MGR_STOPPED);
	break;
	}
}
}//end namespace