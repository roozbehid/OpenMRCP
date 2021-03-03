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
//	File Name:		CMrcpRtspSignal.cpp
//
//	Description:	CMrcpRtspSignal class implementation
//				  THIS MODULE IS WORK IN PROGRESS
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpRtspSignal::CMrcpRtspSignal(CMrcpSession* a_session)
//		CMrcpRtspSignal::~CMrcpRtspSignal()
//		int	CMrcpRtspSignal::PrimStart()
//		int CMrcpRtspSignal::PrimOnSignalingStarted()
//		int	CMrcpRtspSignal::PrimConnect()
//		int CMrcpRtspSignal::PrimSendAck()
//		int	CMrcpRtspSignal::PrimSendCommand(const std::string& a_message)//
//		int CMrcpRtspSignal::PrimStop()
//		int CMrcpRtspSignal::PrimOnStopped()
//		int CMrcpRtspSignal::PrimProcessResults(const std::string& a_message)
//		int CMrcpRtspSignal::PrimDisconnect()
//		int CMrcpRtspSignal::SetupCommandProcessing(int a_commandSocket)
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  3/3/21		Roozbeh G	Boost removal
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpRtspSignal.h"
#include "CMrcpSession.h"
#include "MrcpClientEvents.h"
#include "ClientInterfaceDefs.h"
#include "MrcpUtils.h"
#include "MrcpTasks.h"
#include "CSpeechRecognizer.h"
#include "CMrcpCmdIdMgr.h"
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
		CMrcpRtspSignal::CMrcpRtspSignal(CMrcpSession* a_session)
		: m_session (a_session)

	{
		m_cmdIdMgr = CMrcpCmdIdMgr::Instance();
		if (m_session->AsrResource())
			m_defaultCallBack = m_session->AsrCallBack();
		if (m_session->TtsResource())
			m_defaultCallBack = m_session->TtsCallBack();
		m_serverAddress = m_session->ServerAddress();
		m_serverPort = m_session->ServerPort();
		m_clientAddress = m_session->ClientAddress();
		m_asrResource = m_session->AsrResource();
		m_ttsResource = m_session->TtsResource();


	std::string setup_rtsp_hdr = 
			"SETUP rtsp://(1):(2)/(3) RTSP/1.0" MRCP_ENDL
			"CSeq: (4)" MRCP_ENDL
			"Transport: RTP/AVP;unicast;destination=(7);client_port=(8);mode=(9)" MRCP_ENDL
			"Content-Type: application/sdp" MRCP_ENDL
			"Content-Length: (6)  " MRCP_ENDL
			MRCP_ENDL;

	}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
	CMrcpRtspSignal::~CMrcpRtspSignal()
	{

	}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimStart
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

int	CMrcpRtspSignal::PrimStart()
	{
		SOCKET l_mrcpSock = MrcpUtils::ConnectServerSocket(m_cfgParameters.asrServerConfig->serverAddress,
			m_cfgParameters.asrServerConfig->serverPort);
	/*	if ( l_mrcpSock == INVALID_SOCKET )
		{
			return -1;
		}
	
		if (speechsynth == m_cfgParameters.resourceType)
			TtsSocket(l_mrcpSock);
		else
			AsrSocket(l_mrcpSock);//TODOD: assumes rec is the default?  Set a default?

	   CSignalingOnConnectedEvent* l_onConnectedTask = new CSignalingOnConnectedEvent( this);
	   CMrcpTaskProcessor::Instance()->QueueTask( l_onConnectedTask);

*/
		return 0;
	}

//////////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimOnSignalingStarted - will queue a SIP invite command for processing
// Input - None
//         
// Output - None
//	
///////////////////////////////////////////////////////////////////////////
int CMrcpRtspSignal::PrimOnSignalingStarted()
{
	CSignalingConnectCommand* l_onConnectTask = new CSignalingConnectCommand(this);
	CMrcpTaskProcessor::Instance()->QueueTask( l_onConnectTask);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimConnect - will queue a RTSP Open command for processing
// Input - Non
//         
// Output - None
//	
///////////////////////////////////////////////////////////////////////////
	int	CMrcpRtspSignal::PrimConnect()
	{
		return 0;
	}
//////////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimSendAck - will queue a RTSP Ack command for processing
// Input - Non
//         
// Output - None
//	
///////////////////////////////////////////////////////////////////////////
	int	CMrcpRtspSignal::PrimSendAck()
	{
		return 0;
	}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimSendCommand
// Input - Non
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
	int	CMrcpRtspSignal::PrimSendCommand(const std::string& a_message)//
	{

		int l_rc = 0;
		{
			std::lock_guard<std::mutex> lock( m_controlMutex);	
//			rc = MrcpUtils::TcpSend( m_mrcpCommandSocket, message.c_str(), message.size());
		}

		if (l_rc == CRV_failure)
		{
			return -1;
		}

		return 0;
	}
	
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimOnStarted
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
//int CMrcpRtspSignal::PrimOnStarted()
//{
//WTF	if (IsSpeechRecog)
//		m_resourceMgr->SpeechRecResource(cfgParameters);


		//TODO Socket opened send a RTSP announce to establish session
	    //	   Open signal and event sockets
		//	   Set State to ready
//		return 0;
//}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimStop
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
int CMrcpRtspSignal::PrimStop()
{
	//do we need to check state of any outstanding requests/processing here?
	if (AsrSocket())
		MrcpUtils::CloseSocket(AsrSocket());
	if (TtsSocket())
		MrcpUtils::CloseSocket(TtsSocket());

	   CSignalingOnStoppedEvent* l_onStoppedTask = new CSignalingOnStoppedEvent( this);
	   CMrcpTaskProcessor::Instance()->QueueTask( l_onStoppedTask);

	   return 0;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimOnStopped
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
int CMrcpRtspSignal::PrimOnStopped()
{
//do we have anything to do here?

		return 0;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimProcessResults
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
int CMrcpRtspSignal::PrimProcessResults(const std::string& a_message)
{
//do we have anything to do here?

		return 0;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimDisconnect
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
int CMrcpRtspSignal::PrimDisconnect()
{
//do we have anything to do here?

		return 0;
}
////////////////////////////////////////////////////////////////////////// 
// Description - SetupCommandProcessing - start thread to process socket events
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
int CMrcpRtspSignal::SetupCommandProcessing(int a_commandSocket)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////// 
// Description - HandleEvent - Listen on socket for events
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////



} //end namespace