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
//	File Name:		CMrcpSipSignal.cpp
//
//	Description:	CMrcpSipSignal class implementation
//				  Part of sip signaling state machine
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpSipSignal::CMrcpSipSignal (CMrcpSession* a_session);
//		CMrcpSipSignal::~CMrcpSipSignal(); 
//	   int CMrcpSipSignal::PrimStart();
//	   int CMrcpSipSignal::PrimConnect();
//	   int CMrcpSipSignal::PrimOnSignalingStarted();
//	   int CMrcpSipSignal::PrimSendCommand(const std::string& a_message);
//	   int CMrcpSipSignal::PrimProcessResults(const std::string& a_message);
//	   int CMrcpSipSignal::PrimStop();
//	   int CMrcpSipSignal::PrimSendAck();
//	   int CMrcpSipSignal::PrimOnStopped();
//	   int CMrcpSipSignal::PrimDisconnect();
//	   int CMrcpSipSignal::PrimReinvite(MrcpResourceType a_resourceType);
//	   int CMrcpSipSignal::StartSipListenThread();
//	   int CMrcpSipSignal::ProcessSipThreadEvents();
//	   int CMrcpSipSignal::SetupCommandProcessing(int a_commandSocket);
//	   int CMrcpSipSignal::StartCommandListenThread();
//	   int CMrcpSipSignal::ProcessCommandThreadEvents();
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  1/2/07      TMB         Modified to return handles in callback structure
//  4/1/07      TMB         Cleanup
//  8/3/07      TMB         Move setting of rand seed to constructor
//  3/3/21		Roozbeh G	Boost removal
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpSipSignal.h"
#include "MrcpClientEvents.h"
#include "CMrcpSession.h"
#include "CMrcpRtp.h"
#include "CMrcpRtpMgr.h"
#include "ClientInterfaceDefs.h"
#include "CMrcpProcessSipMsgs.h"
#include "CMrcpSendCommand.h"
#include "CMrcpCmdIdMgr.h"
#include "MrcpUtils.h"
#include "CMrcpTaskProcessor.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - session object pointer
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

CMrcpSipSignal::CMrcpSipSignal(CMrcpSession* a_session)
	: m_session (a_session)
	, m_sipEventReceiverRunning (false)
	, m_commandEventReceiverRunning (false)
{
	ClassName("CMrcpSipSignal");
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
	m_clientRtpPort = CMrcpRtpMgr::Instance()->GetRtpPort();
	srand((unsigned)m_session);

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSipSignal::~CMrcpSipSignal()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimStart
// Input - None
//         
// Output - success or failure
//        
//   Connects the sever socket and queues the on started event
///////////////////////////////////////////////////////////////////////////
int CMrcpSipSignal::PrimStart()
{
	Name("PrimStart");
	SOCKET l_setupSocket;
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	l_setupSocket = MrcpUtils::ConnectServerSocket(m_serverAddress,m_serverPort);
	if ( l_setupSocket == INVALID_SOCKET )
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING,*this, "Invalid Socket");
		CSignalingOnFailedEvent* l_onFailedTask = new CSignalingOnFailedEvent(this);
		CMrcpTaskProcessor::Instance()->QueueTask(l_onFailedTask);
		return -1;
	}
	else
		SetupSocket(l_setupSocket);
	
	if (StartSipListenThread() < 0)
	{
		CLogger::Instance()->Log( LOG_LEVEL_WARNING,*this, "Start Sip Listen Thread Issue");
		return -1;
	}

	CSignalingOnStartedEvent* l_onStartedTask = new CSignalingOnStartedEvent(this);
	CMrcpTaskProcessor::Instance()->QueueTask(l_onStartedTask);
    
	return MrcpSuccess;
}
//////////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimOnSignalingStarted 
// Input - None
//         
// Output - success
//		instantiates a connect command object and queues it
///////////////////////////////////////////////////////////////////////////
int CMrcpSipSignal::PrimOnSignalingStarted()
{
	Name("PrimOnSignalingStarted");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	CSignalingConnectCommand* l_onConnectTask = new CSignalingConnectCommand(this);
	CMrcpTaskProcessor::Instance()->QueueTask( l_onConnectTask);

	return MrcpSuccess;
}

//////////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimConnect - will queue a SIP invite command for processing
// Input - None
//         
// Output - success
//			instantiates a SIP invite command and queues it
///////////////////////////////////////////////////////////////////////////
int	CMrcpSipSignal::PrimConnect()
{
	Name("PrimConnect");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	srand(time(NULL));
	long l_callID = (rand());
	m_callIDVal = MrcpUtils::itos(l_callID) + "F2145@";
	m_callFromVal = MrcpUtils::itos(l_callID) + "214522";
	m_callBranchVal = "z9hg4bk0a" + MrcpUtils::itos(l_callID);

	bool l_allResourcesRequested = false;
	if (m_ttsResource && m_asrResource)
		l_allResourcesRequested = true;
	char l_serverPort[6];
	itoa(m_serverPort, l_serverPort, 10);

	std::string l_src1 = "INVITE sip:mresources@"+ m_serverAddress +":"+ l_serverPort +" SIP/2.0" M_ENDL;
	l_src1 += "via: SIP/2.0/TCP Mrcpv2Client;branch=" + m_callBranchVal + "" M_ENDL;
	l_src1 += "Max-Forwards: 6" M_ENDL;
	l_src1 += "To: MediaServer <sip:mresources@"+ m_serverAddress +":"+ l_serverPort +">" M_ENDL;//MediaServer <sip:mresources@205.214.15.94:5060>
	l_src1 += "From: \"MrcpClient\"<sip:"+m_clientAddress+">;tag=" + m_callFromVal +""  M_ENDL;

	l_src1 += "Contact: <Sip:" + m_clientAddress + ":"+l_serverPort + ";transport=TCP>" M_ENDL;
	l_src1 += "Call-ID: "+ m_callIDVal + m_clientAddress + M_ENDL;
	l_src1 += "CSeq: "+ MrcpUtils::itos(m_cSeq) + " INVITE" M_ENDL;
	l_src1 += "Content-Type: application/sdp" M_ENDL;
	l_src1 += "Content-Length: ";
	std::string l_src2 = "v=0" M_ENDL; 
	l_src2 += "o=- 33580 337654 IN IP4 " + m_clientAddress + M_ENDL; //need 2 numeric values before IN for valid SDP recommend time values
	l_src2 += "s= openMrcpSip"  M_ENDL;
	l_src2 += "c=IN IP4 " + m_clientAddress + M_ENDL;
	l_src2 += "m=application 9 TCP/MRCPv2" M_ENDL;
	l_src2 += "a=setup:active" M_ENDL;
	l_src2 += "a=connection:new" M_ENDL; //first call should be a new session
	if(m_asrResource)
		l_src2 += "a=resource:speechrecog" M_ENDL; //need to determine values here based on resources
	else if (m_ttsResource)
		l_src2 += "a=resource:speechsynth" M_ENDL;
	l_src2 += "a=cmid:1" M_ENDL;  //should be command ID
	if (l_allResourcesRequested)
	{
		l_src2 += "m=application 9 TCP/MRCPv2" M_ENDL;
		l_src2 += "a=setup:active" M_ENDL;
		l_src2 += "a=connection:new" M_ENDL; //first call should be a new session
		if (m_ttsResource)
			l_src2 += "a=resource:speechsynth" M_ENDL;
		l_src2 += "a=cmid:1" M_ENDL;  //should be command ID
	}
	l_src2 += "m=audio " + MrcpUtils::itos(m_clientRtpPort) +" RTP/AVP 0 96" M_ENDL;
	l_src2 += "a=rtpmap:0 pcmu/8000" M_ENDL;
	l_src2 += "a=rtpmap:96 l16/8000" M_ENDL;
	if (l_allResourcesRequested)
		l_src2 += "a=sendrecv" M_ENDL; 
	else if(m_asrResource)
		l_src2 += "a=sendonly" M_ENDL;
	else if (m_ttsResource)
		l_src2 += "a=recvonly" M_ENDL;
	l_src2 += "a=mid:1" M_ENDL;   //message ID

	int l_srcMessageLength = l_src2.length(); // it's the length of the message body that matters
	std::string l_src = l_src1 + MrcpUtils::itos(l_srcMessageLength) + M_ENDL + M_ENDL + l_src2;

	std::cout << l_src.c_str() <<std::endl;

	CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(this);
	l_sendCommand->MessageBuffer(l_src);
	CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, l_src);

	++m_cSeq;
	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimSendAck
// Input - None
//         
// Output - success
//          instantiates a on Ack Message object and queues it
///////////////////////////////////////////////////////////////////////////
int	CMrcpSipSignal::PrimSendAck()
{	
	std::string Astr = "ACK sip:"+m_serverAddress +":" + MrcpUtils::itos((int)m_mrcpSetupSocket) + " SIP/2.0" M_ENDL;
				Astr += "via: SIP/2.0/TCP Mrcpv2Client;branch=" + m_callBranchVal + "" M_ENDL;
				Astr += "Content-Length: 0" M_ENDL;
				Astr += "Call-ID: "+ m_callIDVal + m_clientAddress + M_ENDL;
			    Astr += "CSeq: "+ MrcpUtils::itos(m_cSeq) +" ACK" M_ENDL;
				Astr += "From: \"MrcpClient\"<sip:"+m_clientAddress+">;tag=" + m_callFromVal +""  M_ENDL;
				//Astr += "From: \"MrcpClient\"<sip:" + m_clientAddress + ">;tag=214522" M_ENDL;
				Astr += "Max-Forwards: 6" M_ENDL;
				Astr += m_ackToValue;
				Astr += "user-Agent: openMrcpSip/0.0.1 (omiT)" M_ENDL M_ENDL;

	CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(this);
	l_sendCommand->MessageBuffer(Astr);
	CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);

	CSignalingOnAvailableEvent* l_onAvailableTask = new CSignalingOnAvailableEvent(this);
	CMrcpTaskProcessor::Instance()->QueueTask( l_onAvailableTask);
	++m_cSeq;

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimStop
// Input - None
//         
// Output - success
//          instantiates a on stopped event object and queues it
///////////////////////////////////////////////////////////////////////////
int	CMrcpSipSignal::PrimStop()
{
	Name("PrimStop");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	IsSipEventReceiverRunning(false);
	IsCommandEventReceiverRunning(false);
	
	CSignalingOnStoppedEvent* l_onStoppedTask = new CSignalingOnStoppedEvent( this);
	   CMrcpTaskProcessor::Instance()->QueueTask( l_onStoppedTask);

	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimOnStopped
// Input - None
//         
// Output - success
//		closes the sockets but new state transition          
///////////////////////////////////////////////////////////////////////////
int	CMrcpSipSignal::PrimOnStopped()
{
	Name("PrimOnStopped");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");
	int l_status = 0;


	shutdown(SetupSocket(), SD_RECEIVE);
	MrcpUtils::CloseSocket(SetupSocket());
	m_sipThread->join();

	shutdown(CommandSocket(), SD_RECEIVE);
	MrcpUtils::CloseSocket(CommandSocket());
	m_commandThread->join();

	NotifyObservers(SIGNALING_PROTOCOL_DISCONNECTED);


	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimDisconnect
// Input - None
//         
// Output - success
//          instantiates a bye command object and queues the command object
///////////////////////////////////////////////////////////////////////////
int	CMrcpSipSignal::PrimDisconnect()//
{
	Name("PrimDisconnect");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	char l_serverSignalPort[6];
	itoa(m_serverPort, l_serverSignalPort, 10);

	std::string Astr = "BYE sip:" + m_serverAddress  + ":" + l_serverSignalPort + " SIP/2.0" M_ENDL;
				Astr += "Via: SIP/2.0/TCP "+ m_clientAddress +";rport;branch=" + m_callBranchVal + "" M_ENDL;
				Astr += "Content-Length: 0" M_ENDL;
				Astr += "Call-ID: "+ m_callIDVal  + m_clientAddress + M_ENDL;
				Astr += "CSeq: "+ MrcpUtils::itos(m_cSeq) + " BYE" M_ENDL;
				Astr += "From: \"MrcpClient\"<sip:"+ m_clientAddress +">;tag=" + m_callFromVal + "" M_ENDL;
				Astr += "Max-Forwards: 6" M_ENDL;
				Astr += "To: <sip:"+ m_serverAddress +">" M_ENDL;
				Astr += "user-Agent: openMrcpSip/0.0.1 (omiT)" M_ENDL M_ENDL;


	CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(this);
	l_sendCommand->MessageBuffer(Astr);
	CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);
	//reset the commands for the signaling object
	m_cSeq = 1;
	m_callIDVal.erase();
	m_clientAddress.erase();
	m_serverAddress.erase();
	m_serverPort = 0;
	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimReinvite
// Input - resource type to add
//         
// Output - success
//          instantiates a reinvite command object and queues it
///////////////////////////////////////////////////////////////////////////
int	CMrcpSipSignal::PrimReinvite(MrcpResourceType a_resourceToAdd)
{
	Name("PrimReinvite");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	char l_serverSignalPort[6]; 
	itoa(m_serverPort,l_serverSignalPort,10);
	
	std::string l_src1 = "INVITE sip:mresources@"+ m_serverAddress +":"+l_serverSignalPort +" SIP/2.0" M_ENDL;
	l_src1 += "via: SIP/2.0/TCP Mrcpv2Client;branch=" + m_callBranchVal + "" M_ENDL;
	l_src1 += "Max-Forwards: 6" M_ENDL;
	l_src1 += "To: <sip:"+ m_serverAddress +">" M_ENDL;
	l_src1 += "From: \"MrcpClient\"<sip:"+m_clientAddress+">;tag=" + m_callFromVal +""  M_ENDL;
	l_src1 += "Contact: <Sip:" + m_clientAddress + ":"+l_serverSignalPort + ">" M_ENDL;
	l_src1 += "CSeq: "+ MrcpUtils::itos(m_cSeq) + " INVITE" M_ENDL;
	l_src1 += "Content-Type: application/sdp" M_ENDL;
	l_src1 += "Content-Length: ";
	std::string l_src2 = "v=0" M_ENDL; 
	l_src2 += "o=- 33580 337654 IN IP4 " + m_clientAddress + M_ENDL; //need 2 numeric values before IN for valid SDP recommend time values
	l_src2 += "s= openMrcpSip"  M_ENDL;
	l_src2 += "m=application 9 TCP/MRCPv2" M_ENDL;
	l_src2 += "c=IN IP4 " + m_clientAddress + M_ENDL;
	l_src2 += "a=setup:active" M_ENDL;
	l_src2 += "a=connection:existing" M_ENDL;
	if (a_resourceToAdd == speechrecog)
		l_src2 += "a=resource:speechrecog" M_ENDL; //need to determine values here based on resources
	if (a_resourceToAdd == speechsynth)
		l_src2 += "a=resource:speechsynth" M_ENDL;
	l_src2 += "a=cmid:1" M_ENDL;  //should be command ID
	l_src2 += "m=audio " + MrcpUtils::itos(m_clientRtpPort) +" RTP/AVP 0 96" M_ENDL;
	l_src2 += "c=IN IP4 " + m_clientAddress + M_ENDL;
	l_src2 += "a=rtpmap:0 pcmu/8000" M_ENDL;
	l_src2 += "a=rtpmap:96 telephone-event/8000" M_ENDL;
	l_src2 += "a=sendrecv" M_ENDL;  //assume that since a session exists have to reuse stream
	l_src2 += "a=mid:1" M_ENDL;   //message ID

	int l_srcMessageLength = l_src2.length(); // it's the length of the message body that matters
	std::string l_src = l_src1 + MrcpUtils::itos(l_srcMessageLength) + M_ENDL + M_ENDL + l_src2;

	std::cout << l_src.c_str() <<std::endl;

	CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(this);
	l_sendCommand->MessageBuffer(l_src);
	CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);

	++m_cSeq;
	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimSendCommand
// Input - reference to message string
//         
// Output - success or failure
//          sends the command down the tcp socket
///////////////////////////////////////////////////////////////////////////
int	CMrcpSipSignal::PrimSendCommand(const std::string& a_message)//
{
	Name("PrimSendCommand");
	SOCKET l_mrcpSocket = 0;

	std::string::size_type l_statPos = a_message.find("sip");
	if (l_statPos != std::string::npos) 
		l_mrcpSocket = SetupSocket();
	else
	{
		l_mrcpSocket = CommandSocket();
		CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, a_message);
	}
	int l_rc;
	{
		std::lock_guard<std::mutex> lock( m_controlMutex);	
		l_rc = MrcpUtils::TcpSend( l_mrcpSocket, a_message.c_str(), a_message.size());
	}

	if (l_rc == CRV_failure)
	{
		CLogger::Instance()->Log( LOG_LEVEL_ERROR,*this, "CRV_failure");
		return -1;
	}

	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimProcessResults
// Input - reference to message string
//         
// Output - success
//          Processes the result packet received on the socket
///////////////////////////////////////////////////////////////////////////
int	CMrcpSipSignal::PrimProcessResults(const std::string& a_message)//
{
	Name("PrimProcessResults");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	std::string::size_type l_statPos;
	std::string::size_type l_endPos;
	std::string l_val;
	std::string l_statusValue;
	std::string l_sequenceNum;
	int l_strLoc1 = 0;
	int l_statusVal = 0;
	int l_sequenceID = 0;
	bool l_commandComplete = false;

	MRCP_RESULT_STRUCT l_callResults;

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, a_message);
      //first set the state
      if(a_message.find(" COMPLETE") != std::string::npos)
      {
            NotifyObservers(COMMAND_COMPLETE);
            l_commandComplete = true;
      }
      else
            l_commandComplete = false;
      //now find the reference number and status code.
      if ((l_statPos = a_message.find("MRCP/")) != std::string::npos)
      {
           //this is a valid MRCP message
          if ((l_endPos = a_message.find(" ",l_statPos + 1)) != std::string::npos)
		  {
           //skip the length field, 
                if ((l_statPos = a_message.find(" ",l_endPos + 1)) != std::string::npos)
                if ((l_endPos = a_message.find(" ",l_statPos + 1)) != std::string::npos)
                {
                     l_val.assign(a_message.substr(l_statPos+1, l_endPos-l_statPos-1));
					 if(!MrcpUtils::isNumeric((char*)l_val.c_str(),l_val.length()))
                     {
                         if ((l_statPos = a_message.find(" ",l_endPos + 1)) != std::string::npos)
                         {
                              l_val.assign(a_message.substr(l_endPos+1, l_statPos-l_endPos-1));
                              l_sequenceID = atoi(l_val.c_str());
                          }
                           //get the completion-cause
                          if ((l_statPos = a_message.find("Completion-Cause:")) != std::string::npos)
                          {
                              l_val.assign(a_message.substr(l_statPos+ 18,3));
                              l_statusVal = atoi(l_val.c_str());
                           }
                      }
                      else
                      {
                         //this is a response, current field is reference number, next is status-code
                         l_sequenceID = atoi(l_val.c_str());
                         if ((l_statPos = a_message.find(" ",l_endPos + 1)) != std::string::npos)
                         {
                               l_val.assign(a_message.substr(l_endPos + 1, l_statPos - l_endPos - 1));
                               l_statusVal = atoi(l_val.c_str());
                          }
                      }
				}
		  }
      }
    l_callResults.MrcpReturnValue = l_statusVal;
	l_callResults.referenceNumber = l_sequenceID;
	l_callResults.asrSessionHandle = AsrSessionHandle();
	l_callResults.ttsSessionHandle = TtsSessionHandle();
	strncpy(l_callResults.MrcpReturnPkt,a_message.c_str(),a_message.length());
	
	MrcpCallBackFunction l_callBack = m_cmdIdMgr->GetCommandObj(l_sequenceID);
	if (!l_callBack)
		l_callBack = m_defaultCallBack;

	m_cmdIdMgr->PerformCallbackProcessing(l_callResults,l_commandComplete,l_callBack);
	
	return MrcpSuccess;
}


////////////////////////////////////////////////////////////////////////// 
// Description - StartSocketListenThread - start thread to process socket events
// Input - None
//         
// Output - success or failure
//          starts a thread to receive command status messages from server
///////////////////////////////////////////////////////////////////////////
int CMrcpSipSignal::StartSipListenThread()
{			
	Name("StartSipListenThread");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

   { // scope for lock
      std::lock_guard<std::mutex> l_controlLock( m_controlMutex);
      if ( IsSipEventReceiverRunning())
      {
	  	 CLogger::Instance()->Log( LOG_LEVEL_WARNING,*this, "Already running");
         return -1;
      }
   }
//std::shared_ptr<std::thread>
   m_sipThread = std::shared_ptr<std::thread>(new std::thread( std::bind( &CMrcpSipSignal::ProcessSipThreadEvents, this)));	
   if (!m_sipThread)
	{	    
		CLogger::Instance()->Log( LOG_LEVEL_ERROR,*this, "Thread not started");
		return -1;
	}
	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
// Description - ProcessSipThreadEvents - Listen on socket for events
// Input - None
//         
// Output - success
//         Thread to collect results to commands sent to MRCP server 
///////////////////////////////////////////////////////////////////////////
int CMrcpSipSignal::ProcessSipThreadEvents()
{
	Name("ProcessSipThreadEvents");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	fd_set	l_check_set;
	struct timeval l_time_interval = { 0, 1000 } ;
	int l_packetsReady = 0; 
	char l_buff[10000], l_orig_buff[10000];
	std::string l_bufferReceived;
	std::string::size_type l_statPos;

   std::lock_guard<std::mutex> semaphoreLock( m_semaphoreMutex);

   { // scope for lock
      std::lock_guard<std::mutex> l_controlLock( m_controlMutex);
      m_sipEventReceiverRunning = true;
   }

   while( IsSipEventReceiverRunning())
   {
	   	while (!l_packetsReady) 
		{  

			FD_ZERO( &l_check_set );
			FD_SET(SetupSocket(), &l_check_set);
			l_packetsReady = select ( FD_SETSIZE, &l_check_set, NULL, NULL, &l_time_interval ) ;

		}
		   if (l_packetsReady && IsSipEventReceiverRunning())
		   {
				int len = MrcpUtils::UdpGet ( SetupSocket() , l_buff, sizeof ( l_buff ), l_orig_buff, sizeof ( l_orig_buff ) ) ; 
				if (len > 1)
					l_bufferReceived.assign( l_buff, sizeof (l_buff));  
				if ( l_statPos = (l_bufferReceived.find("SIP")) != std::string::npos) 
				{
				CSipProcessMessage* l_onSipMessage = new CSipProcessMessage( this);
				l_onSipMessage->SetMessageBuffer(l_bufferReceived);
				CMrcpTaskProcessor::Instance()->QueueTask( l_onSipMessage);
				l_bufferReceived.erase();
				strcpy(l_buff,"\n");
				strcpy(l_orig_buff,"\n"); 
				l_statPos = 0;
				}
				l_packetsReady = 0;
		   }
   }

   CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "SIP Event thread exited");

   return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - SetupCommandProcessing - start thread to process socket events
// Input - command socket
//         
// Output - results from starting command listen thread
//          connects a socket to the server , sets the command socket & starts 
//			listen thread
///////////////////////////////////////////////////////////////////////////
int CMrcpSipSignal::SetupCommandProcessing(int a_commandSocket)
{
	Name("SetupCommandProcessing");
	SOCKET l_cmdSocket = MrcpUtils::ConnectServerSocket(m_serverAddress,a_commandSocket);
	CommandSocket(l_cmdSocket);
	return StartCommandListenThread();
}
////////////////////////////////////////////////////////////////////////// 
// Description - StartCommandListenThread - start thread to process socket events
// Input - None
//         
// Output - success
//          starts a thread to process commands received
///////////////////////////////////////////////////////////////////////////
int CMrcpSipSignal::StartCommandListenThread()
{	
	Name("StartCommandListenThread");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

   { // scope for lock
      std::lock_guard<std::mutex> l_controlLock( m_controlMutex);
      if ( IsCommandEventReceiverRunning())
      {
		CLogger::Instance()->Log( LOG_LEVEL_WARNING,*this, "Command Listen Thread already running");
         return -1;
      }
   }

   m_commandThread = std::shared_ptr<std::thread> ( new std::thread( std::bind( &CMrcpSipSignal::ProcessCommandThreadEvents, this)));
	if (!m_commandThread)
	{
	    CLogger::Instance()->Log( LOG_LEVEL_ERROR,*this, "Error creating thread");
		return -1;
	}
	
	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - ProcessCommandThreadEvents - Listen on socket for events
// Input - None
//         
// Output - success
//        process sip events received on socket  
///////////////////////////////////////////////////////////////////////////
int CMrcpSipSignal::ProcessCommandThreadEvents()
{
	Name("ProcessCommandThreadEvents");
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "Entering");

	fd_set	l_check_set;
	struct timeval l_time_interval = { 0, 1000 } ;
	int l_packetsReady = 0; 
	char l_buff[10000];
	std::string l_bufferReceived;

   std::lock_guard<std::mutex> semaphoreLock( m_semaphore2Mutex);
   memset(l_buff,0,sizeof(l_buff));

   { // scope for lock
      std::lock_guard<std::mutex> l_controlLock( m_commandMutex);
      m_commandEventReceiverRunning = true;
   }


   while( IsCommandEventReceiverRunning())
   {
	   	while (!l_packetsReady) 
		{  
			FD_ZERO( &l_check_set );
			FD_SET(CommandSocket(), &l_check_set);
			l_packetsReady = select ( FD_SETSIZE, &l_check_set, NULL, NULL, &l_time_interval ) ;
		}
		int l_buf_len = sizeof ( l_buff ) - 1 ; 
		int l_len = MrcpUtils::TcpGet(CommandSocket(), l_buff, l_buf_len);
		if (CRV_ok ==l_len)
		{
			CMrcpProcessCommandResults* l_processResults = new CMrcpProcessCommandResults(this);
			l_processResults->MessageBuffer( l_buff);  
			CMrcpTaskProcessor::Instance("CALLBACK")->QueueTask( l_processResults);
			memset(l_buff,0,sizeof(l_buff));
		}
   }		
	CLogger::Instance()->Log( LOG_LEVEL_INFO,*this, "SIP Command Thread Exited");

	return MrcpSuccess;
}	

} //end namespace


