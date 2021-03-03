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
//	File Name:		CMrcpRtpMgr.h
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
#ifndef CMrcpRtpMgr_H
#define CMrcpRtpMgr_H

#include "singleton.h"
#include "ClientInterfaceDefs.h"
#include "CLogger.h"
#include "AMrcpObserverSubject.h"
#include "AMrcpEventHandler.h"

#include <mutex>
#include <map>

namespace MrcpV2RefLib
{
	class CMrcpRtp;
	class CMrcpAudioStream;
	class CMrcpHandleMgr;
	class CMrcpEvent;

	class CMrcpRtpMgr : public Singleton<CMrcpRtpMgr>,
						public AMrcpEventHandler,
						public AMrcpObserverSubject
						
	{
		friend class Singleton<CMrcpRtpMgr>;
		friend class CMrcpSipSignal;
	public:
		virtual ~CMrcpRtpMgr();
		int SetupSession(RESOURCE_CFG_STRUCT* a_resourceCfg,int a_clientRtpPort, int a_serverRtpPort);
		int StopSession(MrcpAudioHandle l_audioHandle);
		int Shutdown();

		const char* StateDesc() { return "N/A";}		
		const std::string& ClassName(){return m_className;};
		void ClassName(const std::string& a_className) {m_className = a_className;};
		const std::string& Name(){return m_name;};
		void Name( const std::string& a_name){m_name = a_name;};
		LogObjectTypeEnum LogObjectType(){return LogObject_System;};

	protected:
		typedef std::map<int, CMrcpRtp*> StreamToRtpPointerMap;
		StreamToRtpPointerMap m_streamToRtpMap;
		int GetRtpPort();

	private:
		CMrcpRtpMgr();

		int m_rtpAudioPort;

		MrcpV2RefLib::CMrcpHandleMgr* m_handleMgr;
		std::mutex m_controlMutex;
		std::mutex m_criticalSection;
	    std::string m_className;
	    std::string m_name;
		int m_rtpSessionCnt;
		bool m_shutdownIndicator;

		void HandleEvent(CMrcpEvent* a_event);

	};

}//end namespace
#endif