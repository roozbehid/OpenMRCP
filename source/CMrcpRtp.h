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
//	File Name:		CMrcpRtp.h
//
//	Description:	class implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
///////////////////////////////////////////////////////////////////////////////
#ifndef CMrcpRtp_H
#define CMrcpRtp_H

#include "ClientInterfaceDefs.h"
#include "CLogger.h"
#include "AMrcpObserverSubject.h"

#include <winsock2.h>
//#include <boost/bind.hpp>
//#include <boost/std::shared_ptr.hpp>
//#include <boost/thread/mutex.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/condition.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>


///////////////////////////////////////////////////////////////////////////////
// Enumeration: ::RTPStateEnum
//
// Description: Logger states
//
///////////////////////////////////////////////////////////////////////////////
typedef enum 
{
   RTP_STARTING,
   RTP_STARTED,
   RTP_CONNECTING,
   RTP_CONNECTED,
   RTP_STOPPING
} RtpStateEnum;

namespace MrcpV2RefLib
{
	class CMrcpAudioStream;

	class CMrcpRtp : public AMrcpObserverSubject
	{
		friend class CMrcpAudioStream;
	public:
		CMrcpRtp(RESOURCE_CFG_STRUCT* a_resourceCfg,int a_basePort, int a_clientAsrPort=0);
		virtual ~CMrcpRtp();
		int Start(CMrcpAudioStream* a_audioStream);
		int Stop();

		//accessor
		int GetRtpPort(){return m_basePort;};
		void BufferState(bool a_bufferStat) {m_outboundPacketsReady = a_bufferStat;m_semaphore.notify_one();};
		bool BufferState() {return m_outboundPacketsReady;};

	    const char* StateDesc() { return "N/A";}		
		const std::string& ClassName(){return m_className;};
		void ClassName(const std::string& a_className) {m_className = a_className;};
		const std::string& Name(){return m_name;};
		void Name( const std::string& a_name){m_name = a_name;};
		LogObjectTypeEnum LogObjectType(){return LogObject_System;};

	protected:

	private:
		int InboundPortSetup( );
		int OutboundPortSetup();
		void ProcessInboundRtp();
		void ProcessOutboundRtp(std::string a_buffer);
		bool OutboundRTPSendBye();

		SOCKET m_rtpSocket;
		SOCKET m_rtcpSocket;
		SOCKET m_clientRtpSocket;
		SOCKET m_clientRtcpSocket;
		RtpStateEnum m_state;

		bool m_asrSession;
		bool m_ttsSession;
		bool m_outboundPacketsReady;

		CMrcpAudioStream* m_audioStream;

		unsigned int m_outputBlockSize;
		unsigned int m_lastRtcpTransmit;
		struct sockaddr_in m_outRtpAddress;
		struct sockaddr_in m_outRtcpAddress;
		bool m_inboundRequestStop;
		bool m_outboundRequestStop;
		unsigned int m_seqNum;
		unsigned int m_outLen;
		unsigned int m_chanNum;
		int m_basePort;
		int m_clientAsrRtpPort;

		RESOURCE_CFG_STRUCT* m_resourceCfg;

	    std::shared_ptr<std::thread> m_outboundThread;
	    std::shared_ptr<std::thread> m_inboundThread;
		std::mutex m_controlMutex;
		std::condition_variable m_semaphore;

		std::string m_className;
	    std::string m_name;
		
	};

}//end namespace
#endif