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
//	File Name:		CMrcpRtp.cpp
//
//	Description:	CMrcpRtp class implementation
//				   contains the methods necessary to process inbound and outbound 
//				   RTP packets
//				   1 instance per session
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpRtp::CMrcpRtp(RESOURCE_CFG_STRUCT* a_resourceCfg, int a_basePort)
//		CMrcpRtp::~CMrcpRtp()
//		int CMrcpRtp::Start(CMrcpAudioStream* a_audioStream )
//		int CMrcpRtp::Stop()
//		int CMrcpRtp::InboundPortSetup( )
//		int CMrcpRtp::OutboundPortSetup()
//		void CMrcpRtp::ProcessInboundRtp()
//		void CMrcpRtp::ProcessOutboundRtp(std::string a_buffer)
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  4/1/07      TMB         cleanup 
///////////////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#include <time.h>

#ifdef _DEBUG
#include <fstream>
//added for test
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "CMrcpRtp.h"
#include "CMrcpAudioStream.h"
#include "MrcpUtils.h"
#include "ClientInterfaceDefs.h"
#include "MrcpClientEvents.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
// Description - Constructor 
// Input - config structure
//         base rtp port
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpRtp::CMrcpRtp(RESOURCE_CFG_STRUCT* a_resourceCfg, int a_basePort, int a_clientPort)
	: m_resourceCfg(a_resourceCfg)
	, m_basePort(a_basePort)
	, m_clientAsrRtpPort(a_clientPort)
	, m_rtpSocket(0)
	, m_rtcpSocket(0)
	, m_clientRtpSocket(0)
	, m_clientRtcpSocket(0)
	, m_outputBlockSize(0)
	, m_lastRtcpTransmit(0)
	, m_seqNum (0)
	, m_outLen(0)
	, m_chanNum(21451067)
	, m_asrSession(false)
	, m_ttsSession(false)
	, m_inboundRequestStop(false)
	, m_outboundRequestStop(false)
	, m_outboundPacketsReady(false)
{
	ClassName("CMrcpRtp");
}
////////////////////////////////////////////////////////////////////////// 
// Description - Destructor 
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpRtp::~CMrcpRtp()
{

}
////////////////////////////////////////////////////////////////////////// 
// Description - Start 
// Input - audio stream object pointer
//         
// Output - success or failure
//          Starts the rtp processing
///////////////////////////////////////////////////////////////////////////
int CMrcpRtp::Start(CMrcpAudioStream* a_audioStream )
{
	Name("Start");
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"entering");
	m_audioStream = a_audioStream;
	int l_result;

	m_state = RTP_STARTING;
	if (strcmp(m_resourceCfg->asrServerConfig->serverAddress, "\n"))
	{
		m_asrSession = true;
		if (OutboundPortSetup() == INVALID_SOCKET)
		{
			CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this,"Error creating ASR thread");
			return -1;
		}
	}
	if (strcmp(m_resourceCfg->ttsServerConfig->serverAddress, "\n"))
	{
		m_ttsSession = true;
//the following is needed because TTS and asr packets travel the same socket when sendrecv specified
		if (!strcmp(m_resourceCfg->asrServerConfig->serverAddress, m_resourceCfg->ttsServerConfig->serverAddress))
		{
			m_rtpSocket = m_clientRtpSocket;
			m_rtcpSocket = m_clientRtcpSocket;
		}
		else
		{
			l_result = InboundPortSetup();
			if (l_result == INVALID_SOCKET)
			{
				CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this,"Error creating TTS thread");
				return -1;
			}
		}
		m_inboundThread = boost::shared_ptr<boost::thread> ( new boost::thread( boost::bind( &CMrcpRtp::ProcessInboundRtp, this)));
	}

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - Stop 
// Input - None
//         
// Output - return success
//          Stops rtp object and processing
///////////////////////////////////////////////////////////////////////////
int CMrcpRtp::Stop()
{		
	Name("Stop");
	m_state = RTP_STOPPING;

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"Entering");
	{//scope for lock
	boost::mutex::scoped_lock l_controlLock( m_controlMutex);
	if ( m_asrSession )
	{
		MrcpUtils::CloseSocket(m_clientRtpSocket);
		MrcpUtils::CloseSocket(m_clientRtcpSocket);
		m_clientRtpSocket = 0;
		m_clientRtcpSocket = 0;
		CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"ASR Exited");

	}
	if ( m_ttsSession )
	{
		shutdown(m_rtpSocket, SD_RECEIVE);
		MrcpUtils::CloseSocket(m_rtpSocket);
		MrcpUtils::CloseSocket(m_rtcpSocket);
		m_inboundRequestStop = true;
		m_semaphore.notify_one();
		m_inboundThread->join();

	}
	}

	NotifyObservers(RTP_SESSION_STOPPED);
	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - InboundPortSetup 
// Input - None
//         
// Output - port setup status
//       Opens UDP listener port for inbound audio   
///////////////////////////////////////////////////////////////////////////

int CMrcpRtp::InboundPortSetup( )
{
	Name("InboundPortSetup");
	int l_portSetupStatus = 0;
	std::string l_errorMsg;
	SOCKET l_rtpSocket;
	SOCKET l_rtcpSocket;

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"Entering");

	m_state = RTP_CONNECTING;
	l_rtpSocket = MrcpUtils::OpenUdpListener( m_basePort, false ) ; 
	if ( l_rtpSocket == INVALID_SOCKET ) 
	{
		l_portSetupStatus = MrcpUtils::GetLastSocketError( "port setup - rtp port" );
		l_errorMsg = "Port Setup Error - " + AsString(l_portSetupStatus);
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this,l_errorMsg);
		l_portSetupStatus = SocketError;
	}

	//no sense opening rtcp port if rtp port open failed.
	if (!l_portSetupStatus)
	{
		l_rtcpSocket = MrcpUtils::OpenUdpListener(m_basePort+1, false ) ; 
		if ( l_rtcpSocket == INVALID_SOCKET ) 
		{
			l_portSetupStatus = MrcpUtils::GetLastSocketError( "Port Setup - rtcp port" );
			l_errorMsg = "Port Setup Error - " + AsString(l_portSetupStatus);
			CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this,l_errorMsg);

			l_portSetupStatus = SocketError;
		}
		else
			m_rtcpSocket = l_rtcpSocket;

	}
		m_rtpSocket = l_rtpSocket;
		if (m_rtpSocket != SocketError && m_rtcpSocket != SocketError)
			m_state = RTP_CONNECTED;

		CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"Exiting");
	return l_portSetupStatus;
}
////////////////////////////////////////////////////////////////////////// 
// Description - OutboundPortSetup 
// Input - None
//         
// Output - port setup status
//          Opens outbound audio socket to send audio to MRCP server on
///////////////////////////////////////////////////////////////////////////

int CMrcpRtp::OutboundPortSetup()
{
	Name("OutboundPortSetup");
	int l_portSetupStatus = 0;
	std::string l_errorMsg;
	SOCKET l_rtpSocket;
	SOCKET l_rtcpSocket;

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"Entering");

	//bind the socket to ensure the identified client port is used
	SOCKADDR_IN l_localAddr;
	memset (&l_localAddr, 0, sizeof(l_localAddr));
	l_localAddr.sin_port = htons (m_clientAsrRtpPort );
	l_localAddr.sin_family = AF_INET;
	l_localAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	std::string l_blockSize = "1000" ; 
	m_outputBlockSize = atoi ( l_blockSize.c_str() ) ; 

	m_state = RTP_CONNECTING;
	// if we'll need to stream out, create a socket to do that
	m_lastRtcpTransmit = time ( 0 ) ; 

	// create rtp socket
		l_rtpSocket = socket ( AF_INET, SOCK_DGRAM, 0 ) ;
	if ( l_rtpSocket == INVALID_SOCKET )
	{
		l_portSetupStatus = MrcpUtils::GetLastSocketError ( "OutboundPort setup - getting socket" ) ; 
		l_errorMsg = "Port Setup Error - " + AsString(l_portSetupStatus);
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this,l_errorMsg);
		l_portSetupStatus = SocketError;
		return l_portSetupStatus;
	}
	else
	{
		l_portSetupStatus = bind(l_rtpSocket, (PSOCKADDR)&l_localAddr, sizeof (l_localAddr));
		if (l_portSetupStatus != 0)
		{
			l_portSetupStatus = MrcpUtils::GetLastSocketError ("CMrcpRtp::OutboundPortSetup - bind error");
			l_errorMsg = "Port Setup Error - " + AsString(l_portSetupStatus);
			CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, l_errorMsg);
			l_portSetupStatus = SocketError;
			return l_portSetupStatus;
		}
	}
			
	memset ( &m_outRtpAddress, 0, sizeof ( sockaddr_in ) );
	m_outRtpAddress.sin_family = AF_INET;
	m_outRtpAddress.sin_port = htons(m_basePort);
	m_outRtpAddress.sin_addr.s_addr = inet_addr(m_resourceCfg->asrServerConfig->serverAddress);

	// create rtcp socket
	l_rtcpSocket = socket ( AF_INET, SOCK_DGRAM, 0 ) ;
	if ( l_rtcpSocket == INVALID_SOCKET )
	{
		l_portSetupStatus = MrcpUtils::GetLastSocketError ( "MrcpUtils::UdpSend - getting socket" ) ; 
		l_errorMsg = "Port Setup Error - " + AsString(l_portSetupStatus);
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this,l_errorMsg);
		l_portSetupStatus = SocketError;
		return l_portSetupStatus;
	}
			
	memset ( &m_outRtcpAddress, 0, sizeof ( sockaddr_in ) );
	m_outRtcpAddress.sin_family = AF_INET;
	m_outRtcpAddress.sin_port = htons(l_rtcpSocket);
	m_outRtcpAddress.sin_addr.s_addr = inet_addr(m_resourceCfg->asrServerConfig->serverAddress);

	m_clientRtpSocket = l_rtpSocket;
	m_clientRtcpSocket = l_rtcpSocket;

	m_state = RTP_CONNECTED;

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"Exiting");

	return l_portSetupStatus;
}
////////////////////////////////////////////////////////////////////////// 
// Description - ProcessInboundRtpPackets 
// Input - None
//         
// Output - None
//          Strip rtp headers from packets and call audio callback to the
//			Client application
//      _DEBUG triggers a recording on inbound G711
///////////////////////////////////////////////////////////////////////////

void CMrcpRtp::ProcessInboundRtp()
{
	Name("ProcessInboundRtp");
	std::string l_errorMsg;
	std::string l_statusInfo;
	int l_numReady = 0; 
	char l_buff[10000], l_origBuff[2000] ; 
	std::string l_silence(160, -1);
	fd_set	l_checkSet;

	//records the audio received to a file
	#ifdef _DEBUG
	int l_buffCnt = 0;
	std::fstream file_inStream("C:\\work\\TtsAudio.au",std::ios::out);
	#endif

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"Entering");

	struct timeval time_interval = { 0, 1000 } ;
	while (!m_inboundRequestStop)
	{
		while (!l_numReady && !m_inboundRequestStop)
		{
			l_numReady = 0;
			FD_ZERO( &l_checkSet );
			FD_SET(m_rtpSocket, &l_checkSet);
			l_numReady = select ( FD_SETSIZE, &l_checkSet, NULL, NULL, &time_interval ) ;
		}
		if (l_numReady && !m_inboundRequestStop)
		{
			int l_len = MrcpUtils::UdpGet ( m_rtpSocket , l_buff, sizeof ( l_buff ), l_origBuff, sizeof ( l_origBuff ) ) ; 
			if ( l_len <= 0 ) 
			{ 
				MrcpUtils::CloseSocket(m_rtpSocket);
				m_rtpSocket = 0;
				m_inboundRequestStop = true;
				CLogger::Instance()->Log(LOG_LEVEL_DEBUG, *this,"UDP Get returned less than 0");
				break;
			}
			else
			{
			std::string l_tempBuff;
			l_tempBuff.assign( l_buff+12, (l_len-12));  // this indexes past the headers to get to the rtp payload.
			m_audioStream->AddToInboundAudioStream( l_tempBuff ) ; 

	#ifdef _DEBUG
			//the code below captures the tts audio in a seperate file for review with a sound editor
			//audio = 8k sample rate, mono, 16bit resolution 8 bit mu-law compressed
			// need some trailing silence to indicate speech complete.
			if (l_tempBuff == l_silence && ++l_buffCnt < 100) 
				file_inStream.write (l_buff + 12, l_len - 12);
			else
			if (l_tempBuff != l_silence)
			{
				file_inStream.write (l_buff + 12, l_len - 12);
				l_buffCnt = 0;
			}
	#endif
			}
		}
	}
	MrcpUtils::CloseSocket(m_rtcpSocket);
	m_rtpSocket = 0;
	m_rtcpSocket = 0;

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"Exiting");

	#ifdef _DEBUG
	file_inStream.close();
	#endif
}
///////////////////////////////////////////////////////////////////////////////////
//
//Description method used to send rtp buffers from client to MRCP server 
//
// Input: audio string buffer
// Output - none
//
//  called by the audio stream obejct when audio ready for sending to MRCP server
//  adds rtp header to packets sent over socket
///////////////////////////////////////////////////////////////////////////////////

void CMrcpRtp::ProcessOutboundRtp(std::string a_buffer)
{
	Name("ProcessOutboundRtp");
	unsigned char* l_thisPack = NULL ; 
	int l_actualLen = 0 ; 
	struct sockaddr *l_sockAddr =(struct sockaddr*)&m_outRtpAddress ; 
	char l_buff[10000]; 
	bool l_beginList = true;

	std::string l_tempBuff;
	std::string l_errorMsg;


	int l_buffLen = a_buffer.size();
	if ( l_buffLen > 1 && m_state == RTP_CONNECTED)	// speech data
	{
		memcpy(l_buff,a_buffer.c_str(),l_buffLen);
		l_thisPack =  (unsigned char * ) new char[l_buffLen + 12];
		l_thisPack[0] = 0x80 ; 
		l_thisPack[1] = 0x00 ; 
		*(unsigned short *) &l_thisPack[2] = htons (m_seqNum++) ; 
		*(unsigned int *) &l_thisPack[4] = htonl ( m_outLen ) ;
		m_outLen += l_buffLen; 
		*(unsigned int *) &l_thisPack[8] = htonl ( m_chanNum ) ;
		memcpy ( l_thisPack + 12, l_buff, l_buffLen ) ; 
		l_actualLen = l_buffLen + 12 ; 

		int l_bytesSent = sendto(m_clientRtpSocket, (char*)l_thisPack, l_actualLen,0 , l_sockAddr, sizeof(struct sockaddr));
		if ( l_bytesSent != (int)l_actualLen ) 
		{
			int error = MrcpUtils::GetLastSocketError( "streaming to server - rtp port" ) ; 
			l_errorMsg = "Socket Error - " + AsString(error);
			CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this,l_errorMsg);
		}
		if (l_thisPack)
			delete l_thisPack;
	}
	else
		CLogger::Instance()->Log(LOG_LEVEL_WARNING, *this, "Sending on a non connected socket");

}

}//end namespace