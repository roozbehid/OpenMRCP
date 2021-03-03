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
//	File Name:		CMrcpSession.cpp
//
//	Description:	CMrcpSession class implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpSession(RESOURCE_CFG_STRUCT* a_resourceCfg, bool l_asrResource, bool l_ttsResource);
//		virtual ~CMrcpSession();
//		int StartSession();
//		int StopSession();
//		int AddResourceToSession(MrcpResourceType a_resourceToAdd);
//		int DefineGrammar(DEFINE_GRAMMAR_STRUCT* a_defineGrammarParams);
//		int Recognize(RECOGNIZE_STRUCT* a_recognizeParams);
//		int Interpret(INTERPRET_STRUCT* a_interpretParams);
//		int StartInputTimers(MrcpCallBackFunction a_callBack, int& a_referenceNum);
//		int GetResult(MrcpCallBackFunction a_callBack,int& a_referenceNum);
//		int StopRecog(MrcpCallBackFunction a_callBack,int& a_referenceNum);
//		int Speak(SPEAK_STRUCT* a_speakParams);
//		int StopSpeak(MrcpCallBackFunction a_callBack,int& a_referenceNum);
//		int Pause(MrcpCallBackFunction a_callBack,int& a_referenceNum);
//		int Resume(MrcpCallBackFunction a_callBack,int& a_referenceNum);
//		int BargeIn(MrcpCallBackFunction a_callBack,int& a_referenceNum);
//		int GetParams(MrcpCallBackFunction a_callBack, GET_PARAMETERS a_parameters, int& a_referenceNumber);
//		int SetParams(MrcpCallBackFunction a_callBack, SET_PARAMETERS a_parameters, int& a_referenceNumber);
//		void HandleEvent(CMrcpEvent* a_event);
//		void PerformSessionCallback();
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  1/2/07      TMB         Modified to return handles in callback structure
//  4/1/07      TMB         Cleanup
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpSession.h"
#include "CMrcpSipSignal.h"
#include "CMrcpRtspSignal.h"
#include "CMrcpEvent.h"
#include "CMrcpRtpMgr.h"
#include "CMrcpHandleMgr.h"
#include "MrcpClientEvents.h"
#include "ClientInterfaceDefs.h"
#include "CRecognitionCommands.h"
#include "CSpeakCommands.h"
#include "CMrcpCmdIdMgr.h"
#include "CMrcpTaskProcessor.h"
#include "CSpeechRecognizer.h"
#include "CSpeechSynthesizer.h"
#include "CMrcpAudioStream.h"
#include "CMrcpRtp.h"
#include "CGenericCommands.h"
#include "CmrcpGeneric.h"
#include "MrcpUtils.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - config structure,
//		   bool indicating asr
//		   bool indicating tts
//         
// Output - None
//			Can support asr and tts resource per session	       
///////////////////////////////////////////////////////////////////////////
CMrcpSession::CMrcpSession(RESOURCE_CFG_STRUCT* a_resourceCfg, bool a_asrResource, bool a_ttsResource)
	: m_handleMgr(CMrcpHandleMgr::Instance())
	, m_resourceCfg(a_resourceCfg)
	, m_clientAddress(a_resourceCfg->clientAddress)
	, m_asrCallback(a_resourceCfg->asrServerConfig->callBack)
	, m_ttsCallback(a_resourceCfg->ttsServerConfig->callBack)
	, m_cmdIdMgr(CMrcpCmdIdMgr::Instance())
	, m_rtpMgr (CMrcpRtpMgr::Instance())
	, m_sessionCmdID(0)
	, m_asrResource(a_asrResource)
	, m_ttsResource(a_ttsResource)
	, m_sessionHandle(0)
	, m_speechSynthResource(NULL)
	, m_speechRecResource(NULL)
{
	ClassName("CMrcpSession");

	if (m_asrResource)
	{
		m_serverAddress = a_resourceCfg->asrServerConfig->serverAddress;
		m_serverPort = a_resourceCfg->asrServerConfig->serverPort;
	}
	if (m_ttsResource)
	{
		m_serverAddress = a_resourceCfg->ttsServerConfig->serverAddress;
		m_serverPort = a_resourceCfg->ttsServerConfig->serverPort;
	}

	m_state= SESSION_STARTING;
	m_mrcpAsrVersion = a_resourceCfg->asrServerConfig->mrcpVersion;
	m_mrcpTtsVersion = a_resourceCfg->ttsServerConfig->mrcpVersion;
	
	m_mrcpTaskProcessor = MrcpV2RefLib::CMrcpTaskProcessor::Instance("TASK");

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSession::~CMrcpSession()
{
	m_signalObj->Unregister(this, SIGNALING_PROTOCOL_CONNECTED);
	m_signalObj->Unregister(this, SIGNALING_PROTOCOL_DISCONNECTED);
	m_signalObj->Unregister(this, SIGNALING_PROTOCOL_FAILED);

	delete m_genericCommandResource;
	delete m_signalObj;
//
	if (m_asrResource)
		delete m_speechRecResource;
	if (m_ttsResource)
		delete m_speechSynthResource;

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StartSession
// Input - None
//         
// Output - success
//    start the signaling 
//    register for notification from signaling when started
//    return a status 
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::StartSession()
{
	Name("StartSession");
    CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Entering start session");

	MrcpV2RefLib::CSignalingStartCommand* l_startCommand = NULL;
	m_state = SESSION_CONNECTING;
	
	m_sessionHandle = m_handleMgr->RegisterPointer(this);
	if ( m_ttsResource && m_asrResource  )
		m_resourceCfg->asrSessionHandle  = m_resourceCfg->ttsSessionHandle  = m_sessionHandle;
	else
	if (m_asrResource)
		m_resourceCfg->asrSessionHandle = m_sessionHandle;
	else
	if (m_ttsResource)
		m_resourceCfg->ttsSessionHandle = m_sessionHandle;

	if (m_mrcpAsrVersion == 1 || m_mrcpTtsVersion == 1)
		m_signalObj = new MrcpV2RefLib::CMrcpRtspSignal(this);
	if (m_mrcpAsrVersion  == 2 || m_mrcpTtsVersion == 2)
		m_signalObj = new MrcpV2RefLib::CMrcpSipSignal(this);

	m_signalObj->TtsSessionHandle(m_resourceCfg->ttsSessionHandle);
	m_signalObj->AsrSessionHandle(m_resourceCfg->asrSessionHandle);
	
	m_genericCommandResource = new CMrcpGeneric(m_resourceCfg, m_signalObj);

	if (m_asrResource)
	{
			m_speechRecResource = new CSpeechRecognizer(m_resourceCfg, m_signalObj); 
			m_speechRecResource->RegisterForNotification(this, RECOGNIZER_EXITED);
		 	m_sessionCmdID = m_cmdIdMgr->GetReferenceId();
			int l_status = m_cmdIdMgr->RegisterCommandObj(m_sessionCmdID, m_resourceCfg->asrServerConfig->callBack);
	}
	if (m_ttsResource)
	{
		m_speechSynthResource = new CSpeechSynthesizer(m_resourceCfg, m_signalObj);
		m_speechSynthResource->RegisterForNotification(this, SYNTHESIZER_EXITED);
		m_sessionCmdID = m_cmdIdMgr->GetReferenceId();
		int l_status = m_cmdIdMgr->RegisterCommandObj(m_sessionCmdID, m_resourceCfg->ttsServerConfig->callBack);
	}
	
	m_signalObj->RegisterForNotification(this, SIGNALING_PROTOCOL_CONNECTED);
	m_signalObj->RegisterForNotification(this, SIGNALING_PROTOCOL_DISCONNECTED);
	m_signalObj->RegisterForNotification(this, SIGNALING_PROTOCOL_FAILED);

	l_startCommand = new CSignalingStartCommand(m_signalObj);
	m_mrcpTaskProcessor->QueueTask(l_startCommand);

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StopSession
// Input - None
//         
// Output - Success
//    stop signaling 
//    state  unregister for notification from signaling when started
//    return a status 
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::StopSession()
{
	Name("StopSession");
    CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Entering StopSession");
	time_t   cmdIssued, elapsedTime;

	MrcpV2RefLib::CSignalingDisconnectCommand* l_disconnectCommand = NULL;
	m_state = SESSION_STOPPING;


	int l_stat = m_handleMgr->UnregisterPointer(m_sessionHandle,"Session");
	if (0> l_stat)
	{		
		std::string l_errorInfo = "Unregistering Handle failed " + AsString(m_sessionHandle);
    	CLogger::Instance()->Log( LOG_LEVEL_ERROR, *this,l_errorInfo);
		return -1;
	}
	l_disconnectCommand = new CSignalingDisconnectCommand(m_signalObj);
	m_mrcpTaskProcessor->QueueTask(l_disconnectCommand);

	time(&cmdIssued);
	while (m_state != SESSION_DISCONNECTED)
	{
		MrcpUtils::MsSleep(500);
		time(&elapsedTime);
		if (difftime(elapsedTime, cmdIssued) > 30)
		{
	///The following forces a cleanup on a missed SIP BYE
			CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Timeout on disconnect");

			CSignalingOnDisconnectedEvent* l_disconnectedEvent = new CSignalingOnDisconnectedEvent(m_signalObj);
			CMrcpTaskProcessor::Instance()->QueueTask(l_disconnectedEvent);
			MrcpUtils::MsSleep(1000);

			CSignalingStopCommand* l_stopCommand = new CSignalingStopCommand(m_signalObj);
			CMrcpTaskProcessor::Instance()->QueueTask(l_stopCommand);
			m_state = SESSION_DISCONNECTED;	
// The following triggers a resend of a missed SIP BYE
/*				
			l_disconnectCommand = new CSignalingDisconnectCommand(m_signalObj);
			m_mrcpTaskProcessor->QueueTask(l_disconnectCommand);
*/
			break;
		}
	}


	CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Exiting StopSession");

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - AddResourcesToSession
// Input - resource type
//         
// Output - success or failure
//    verify session exists and resource not already in use
//    Can only have 1 resource of a type per session.
//    return a status 
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::AddResourceToSession(MrcpResourceType a_resourceToAdd)
{
	Name("AddResourceToSession");
    CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Entering AddResourceToSession");

	//must be in a connected state to add resources
	if (m_state != SESSION_CONNECTED)
		return -1;

	//only 1 instance of a resource type per session
	if((a_resourceToAdd == speechsynth && m_ttsResource) ||
		(a_resourceToAdd == speechrecog && m_asrResource))
	{
		std::string l_errorInfo = "Resource already attached to session ";
    	CLogger::Instance()->Log( LOG_LEVEL_ERROR, *this,l_errorInfo);
		return -1;
	}

	if (m_mrcpAsrVersion == 2 || m_mrcpTtsVersion == 2)
	{
		CSignalingReinviteCommand* l_reinviteTask = new CSignalingReinviteCommand(m_signalObj, a_resourceToAdd);
		CMrcpTaskProcessor::Instance()->QueueTask( l_reinviteTask);
	}
	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - HandleEvent
// Input - pointer to event object
//         
// Output - None
//      process observer notification.
///////////////////////////////////////////////////////////////////////////
void CMrcpSession::HandleEvent(CMrcpEvent* a_event)
{
	Name("HandleEvent");

	switch (a_event->EventID())
	{
	case SIGNALING_PROTOCOL_CONNECTED:
    	CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Protocol Connected");

		m_rtpMgr->SetupSession(m_resourceCfg,m_signalObj->ClientRtpPort(), m_signalObj->ServerRtpPort());
		m_state = SESSION_CONNECTED;
		PerformSessionCallback(0);
		break;
	case SIGNALING_PROTOCOL_FAILED:
		CLogger::Instance()->Log(LOG_LEVEL_WARNING, *this, "Connection Failed");
		PerformSessionCallback(-1);
		break;
	case SIGNALING_PROTOCOL_DISCONNECTED:
    	CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Protocol Disconnected");
		if(m_resourceCfg->inboundAudioHandle)
			m_rtpMgr->StopSession(m_resourceCfg->inboundAudioHandle);			
		if (m_resourceCfg->outboundAudioHandle)
			m_rtpMgr->StopSession(m_resourceCfg->outboundAudioHandle);
		m_state = SESSION_DISCONNECTED;
		PerformSessionCallback(1);
		break;
	case SYNTHESIZER_EXITED:
    	CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Synthesizer Exited");

		m_speechSynthResource->Unregister(this, SYNTHESIZER_EXITED);
		delete m_speechSynthResource;
		break;
	case RECOGNIZER_EXITED:
    	CLogger::Instance()->Log( LOG_LEVEL_INFO, *this,"Recognizer exited");

		m_speechRecResource->Unregister(this, RECOGNIZER_EXITED);
		delete m_speechRecResource;
		break;
	}
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PerformSessionCallback
// Input - None
//         
// Output - None
//     setup callback to the client application that the session is setup
///////////////////////////////////////////////////////////////////////////	
void CMrcpSession::PerformSessionCallback(int a_status)
{
	MRCP_RESULT_STRUCT l_callResults;

	l_callResults.MrcpReturnValue = a_status;
	l_callResults.referenceNumber = 0;
	l_callResults.asrSessionHandle = m_resourceCfg->asrSessionHandle;
	l_callResults.ttsSessionHandle = m_resourceCfg->ttsSessionHandle;
	if (a_status == 1)
		strcpy(l_callResults.MrcpReturnPkt,"Session Disconnected");
	else if (a_status == 0)
		strcpy(l_callResults.MrcpReturnPkt, "Session Connected");
	else if (a_status == -1)
		strcpy(l_callResults.MrcpReturnPkt, "Session not connected");
	MrcpCallBackFunction l_callBack = m_cmdIdMgr->GetCommandObj(m_sessionCmdID);
	m_cmdIdMgr->PerformCallbackProcessing(l_callResults,true,l_callBack);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - DefineGrammar
// Input - define grammar parameters
//         
// Output - success or failure
//  Description define grammar object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
//          
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::DefineGrammar(DEFINE_GRAMMAR_STRUCT* a_defineGrammarParams)
{
	if (m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CRecDefineGrammar* DefineGrammarCmd = new CRecDefineGrammar(l_cmdID,m_speechRecResource, a_defineGrammarParams);
	m_mrcpTaskProcessor->QueueTask(DefineGrammarCmd);

	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_defineGrammarParams->callBack);
	a_defineGrammarParams->referenceNumber = l_cmdID;

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Recognize
// Input - recognize parameters structure
//         
// Output - success or failure
// Description Recognized object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::Recognize(RECOGNIZE_STRUCT* a_recognizeParams)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CRecRecognize* l_RecognizeCmd = new CRecRecognize(l_cmdID, m_speechRecResource, a_recognizeParams->cancelQueue, a_recognizeParams->contentType, a_recognizeParams->gramContent, a_recognizeParams->startInputTimers);
	m_mrcpTaskProcessor->QueueTask(l_RecognizeCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_recognizeParams->callBack);

	a_recognizeParams->referenceNumber = l_cmdID;


	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Interpret
// Input - interpret structure
//         
// Output - success or failure
// Description interpret object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::Interpret(INTERPRET_STRUCT* a_interpretParams)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CRecInterpret* l_interpretCmd = new CRecInterpret(l_cmdID, m_speechRecResource, a_interpretParams->contentType, 
														a_interpretParams->interpretText,a_interpretParams->contentId,
														a_interpretParams->content);
	m_mrcpTaskProcessor->QueueTask(l_interpretCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_interpretParams->callBack);

	a_interpretParams->referenceNumber = l_cmdID;

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StartInputTimers
// Input - callback function
//		   reference number
//         
// Output - success or failure
//  Description StartInputTimers object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::StartInputTimers(MrcpCallBackFunction a_callBack, int& a_referenceNum)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CRecStartInputTimers* l_startInputTimersCmd = new CRecStartInputTimers(l_cmdID, m_speechRecResource);
	m_mrcpTaskProcessor->QueueTask(l_startInputTimersCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - GetResult
// Input - callback function
//         reference number
// Output - success or failure
// Description - Get Result object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::GetResult(MrcpCallBackFunction a_callBack, int& a_referenceNum)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CRecGetResult* l_getResultCmd = new CRecGetResult(l_cmdID, m_speechRecResource);
	m_mrcpTaskProcessor->QueueTask(l_getResultCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - StopRec
// Input - callback function
//         reference number
// Output - success or failure
// Description stop for recognition object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::StopRecog(MrcpCallBackFunction a_callBack, int& a_referenceNum)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CRecStopRec* l_stopRecCmd = new CRecStopRec(l_cmdID, m_speechRecResource);
	m_mrcpTaskProcessor->QueueTask(l_stopRecCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Speak
// Input - speak parameters structure
//         
// Output - success or failure
// Description - speak object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::Speak(SPEAK_STRUCT* a_speakParams)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CSpkSpeak* l_speakCmd = new CSpkSpeak(l_cmdID, m_speechSynthResource,a_speakParams);
	m_mrcpTaskProcessor->QueueTask(l_speakCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_speakParams->callBack);

	a_speakParams->referenceNumber = l_cmdID;

	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - StopSpeak
// Input - callback function
//         reference number
// Output - success or failure
// Description stop for speak object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::StopSpeak(MrcpCallBackFunction a_callBack, int& a_referenceNum)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CSpkStop* l_stopSpeakCmd = new CSpkStop(l_cmdID, m_speechSynthResource);
	m_mrcpTaskProcessor->QueueTask(l_stopSpeakCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Pause
// Input - callback function
//         reference number
// Output - success or failure
// Description -pause object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::Pause(MrcpCallBackFunction a_callBack, int& a_referenceNum)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CSpkPause* l_spkPauseCmd = new CSpkPause(l_cmdID, m_speechSynthResource);
	m_mrcpTaskProcessor->QueueTask(l_spkPauseCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Resume
// Input - callback function
//         reference number
// Output - success or failure
// description resume object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::Resume(MrcpCallBackFunction a_callBack, int& a_referenceNum)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CSpkResume* l_resumeCmd = new CSpkResume(l_cmdID, m_speechSynthResource);
	m_mrcpTaskProcessor->QueueTask(l_resumeCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - BargeIn
// Input - callback function
//         reference number
// Output - success or failure
// Description Barge In object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::BargeIn(MrcpCallBackFunction a_callBack, int& a_referenceNum)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	CSpkBargeIn* l_bargeInCmd = new CSpkBargeIn(l_cmdID, m_speechSynthResource);
	m_mrcpTaskProcessor->QueueTask(l_bargeInCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - GetParams
// Input - call back function
//		   structure of parameters
//         reference number
// Output - success or failure
// Description - Get Params object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::GetParams(MrcpCallBackFunction a_callBack,GET_PARAMETERS a_parameters, int& a_referenceNum)
{
	int l_sessionVersion;
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_cmdID = m_cmdIdMgr->GetReferenceId();
		if (m_asrResource && m_ttsResource)
		l_sessionVersion = m_mrcpAsrVersion;
	else 
		if (m_asrResource)
			l_sessionVersion = m_mrcpAsrVersion;
		else
			if (m_ttsResource)
				l_sessionVersion = m_mrcpTtsVersion;

	CGetParams* l_getParamsCmd = new CGetParams(l_cmdID, m_genericCommandResource, l_sessionVersion);
	l_getParamsCmd->m_parameters = a_parameters;
	m_mrcpTaskProcessor->QueueTask(l_getParamsCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SetParams
// Input - callback function
//         set parameters
//         reference number
// Output - success or failure
// Description - set params object instantiated and queued.  
//			returns the sequence number assigned for command processing at
//          MRCP server.  
///////////////////////////////////////////////////////////////////////////
int CMrcpSession::SetParams(MrcpCallBackFunction a_callBack,SET_PARAMETERS a_parameters, int& a_referenceNum)
{
	if(m_state != SESSION_CONNECTED)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this,"Session Not Connected");
		return -1;
	}
	int l_sessionVersion;

	int l_cmdID = m_cmdIdMgr->GetReferenceId();
	if (m_asrResource && m_ttsResource)
		l_sessionVersion = m_mrcpAsrVersion;
	else 
		if (m_asrResource)
			l_sessionVersion = m_mrcpAsrVersion;
		else
			if (m_ttsResource)
				l_sessionVersion = m_mrcpTtsVersion;

	CSetParams* l_setParamsCmd = new CSetParams(l_cmdID, m_genericCommandResource, l_sessionVersion);

	l_setParamsCmd->m_parameters = a_parameters;
	m_mrcpTaskProcessor->QueueTask(l_setParamsCmd);
	int l_status = m_cmdIdMgr->RegisterCommandObj(l_cmdID, a_callBack);

	a_referenceNum = l_cmdID;

	return MrcpSuccess;
	}

}//end namespace