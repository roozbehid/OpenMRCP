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
//	File Name:		CMrcpSipSignal.h
//
//	Description:	 class implementation
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
//  4/1/07      TMB         Cleanup
///////////////////////////////////////////////////////////////////////////////
#ifndef CMrcpSipSignal_h
#define CMrcpSipSignal_h

#include "AMrcpSignaling.h"
#include "CLogger.h"

#include <list>
#include <map>
#include <winsock2.h>

#include <thread>
#include <mutex>
//#include <boost/thread/condition.hpp>
//#include <boost/bind.hpp>
#include <functional>

namespace MrcpV2RefLib
{
	class CMrcpSession;
	class CMrcpCmdIdMgr;
	class CMrcpSipSignal : public AMrcpSignaling
	{
	public:
		CMrcpSipSignal (CMrcpSession* a_session);
		virtual ~CMrcpSipSignal(); 

		bool IsSipEventReceiverRunning(){return m_sipEventReceiverRunning;};
		void IsSipEventReceiverRunning(bool a_sipEventReceiverRunning) {m_sipEventReceiverRunning = a_sipEventReceiverRunning;};
		bool IsCommandEventReceiverRunning() {return m_commandEventReceiverRunning;};
		void IsCommandEventReceiverRunning(bool a_commandEventReceiverRunning) {m_commandEventReceiverRunning = a_commandEventReceiverRunning;};
		
		const char* StateDesc() { return "N/A";}		
		const std::string& ClassName(){return m_className;};
		void ClassName(const std::string& a_className) {m_className = a_className;};
		const std::string& Name(){return m_name;};
		void Name( const std::string& a_name){m_name = a_name;};
		LogObjectTypeEnum LogObjectType(){return LogObject_System;};

		void SetAckToValue(std::string a_ackToVal) { m_ackToValue = a_ackToVal;};

	protected:

	private:
		CMrcpSession* m_session;
		CMrcpCmdIdMgr* m_cmdIdMgr;

		std::shared_ptr<std::thread> m_sipThread;
		std::shared_ptr<std::thread>m_commandThread;

		typedef struct sipInfoStruct
		{
			std::string callID;
			std::string branchVal;
			std::string toVal;
		};
		typedef std::map<std::string, sipInfoStruct> SipInfoMap;
		SipInfoMap m_sipInfoMap;

	   int PrimStart();
	   int PrimConnect();
	   int PrimOnSignalingStarted();

	   int PrimSendCommand(const std::string& a_message);
	   int PrimProcessResults(const std::string& a_message);
	   int PrimStop();
	   int PrimSendAck();
	   int PrimOnStopped();
	   int PrimDisconnect();
	   int PrimReinvite(MrcpResourceType a_resourceType);

	   int StartSipListenThread();
	   int ProcessSipThreadEvents();
	   int SetupCommandProcessing(int a_commandSocket);
	   int StartCommandListenThread();
	   int ProcessCommandThreadEvents();

	   std::mutex m_controlMutex;
	   std::mutex m_commandMutex;
       std::mutex m_semaphoreMutex;
	   std::mutex m_semaphore2Mutex;

	   MrcpCallBackFunction m_defaultCallBack;
	   
       std::condition_variable m_semaphore;
	   bool m_sipEventReceiverRunning;
	   bool m_commandEventReceiverRunning;
	   bool m_asrResource;
	   bool m_ttsResource;

	   std::string m_callIDVal;
	   std::string m_callFromVal;
	   std::string m_callBranchVal;
	   std::string m_ackToValue;

	   std::string m_className;
	   std::string m_name;

	};
}//end namespace
#endif