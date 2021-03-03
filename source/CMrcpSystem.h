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
//	File Name:		CMrcpSystem.h
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
#ifndef CMrcpSystem_H
#define CMrcpSystem_H

//#include <boost/thread/mutex.hpp>
#include <mutex>
#include <map>

#include "singleton.h"
#include "ClientInterfaceDefs.h"
#include "CLogger.h"
#include "AMrcpEventHandler.h"

namespace MrcpV2RefLib
{
///////////////////////////////////////////////////////////////////////////////
// Enumeration: ::MrcpSystemStateEnum
//
// Description: Mrcp System states
//
///////////////////////////////////////////////////////////////////////////////
	typedef enum 
	{	
		SYSTEM_STARTING,
		SYSTEM_RUNNING,
		SYSTEM_STOPPING,
		SYSTEM_STOPPED
	} MrcpSystemStateEnum;

	class CMrcpSession;
	class CMrcpSessionMgr;
	class CMrcpCmdIdMgr;
	class CMrcpTaskProcessor;
	class CMrcpHandleMgr;
	class CMrcpRtpMgr;
	class CMrcpEvent;
	
	class CMrcpSystem  :public Singleton <CMrcpSystem>
		               ,public AMrcpEventHandler
	{
		friend class Singleton<CMrcpSystem>;

	public:
		virtual ~CMrcpSystem();
		int Start();
		int Stop();
		int OpenSession(RESOURCE_CFG_STRUCT* a_resourceCfg);
		int AddSessionResources(MrcpSessionHandle a_sessionHandle,MrcpResourceType a_resourceToAdd);
		int CloseSession(MrcpSessionHandle a_sessionHandle);
		// commands
		int DefineGrammar(MrcpSessionHandle a_sessionHandle,DEFINE_GRAMMAR_STRUCT* a_defineGrammarParams);
		int Recognize(MrcpSessionHandle a_sessionHandle,RECOGNIZE_STRUCT* a_recognizeParams);
		int Interpret(MrcpSessionHandle a_sessionHandle,INTERPRET_STRUCT* a_interpretParams);
		int StartInputTimers(MrcpSessionHandle a_sessionHandle,MrcpCallBackFunction a_callBack, int& a_referenceNum);
		int GetResult(MrcpSessionHandle a_sessionHandle,MrcpCallBackFunction a_callBack,int& a_referenceNum);
		int StopRecog(MrcpSessionHandle a_sessionHandle,MrcpCallBackFunction a_callBack,int& a_referenceNum);
		/////////
		int Speak(MrcpSessionHandle a_sessionHandle,SPEAK_STRUCT* a_speakParams);
		int StopSpeak(MrcpSessionHandle a_sessionHandle,MrcpCallBackFunction a_callBack,int& a_referenceNum);
		int Pause(MrcpSessionHandle a_sessionHandle,MrcpCallBackFunction a_callBack,int& a_referenceNum);
		int Resume(MrcpSessionHandle a_sessionHandle,MrcpCallBackFunction a_callBack,int& a_referenceNum);
		int BargeIn(MrcpSessionHandle a_sessionHandle,MrcpCallBackFunction a_callBack,int& a_referenceNum);
		/////////
		int GetParams(MrcpSessionHandle a_sessionHandle, MrcpCallBackFunction a_callBack, GET_PARAMETERS a_parameters, int& a_referenceNum);
		int SetParams(MrcpSessionHandle a_sessionHandle, MrcpCallBackFunction a_callBack, SET_PARAMETERS a_parameters, int& a_referenceNum);
		////////
		int AddOutboundAudio(MrcpAudioHandle a_audioHandle, char& a_audioBuffer);
		int StartAudio(MrcpAudioHandle a_audioHandle);
		int StopAudio(MrcpAudioHandle a_audioHandle);

	    const char* StateDesc() { return "N/A";}		
		const std::string& ClassName(){return m_className;};
		void ClassName(const std::string& a_className) {m_className = a_className;};
		const std::string& Name(){return m_name;};
		void Name( const std::string& a_name){m_name = a_name;};
		LogObjectTypeEnum LogObjectType(){return LogObject_System;};

	protected:

	private:
		CMrcpSystem();
		void HandleEvent(CMrcpEvent* a_event);

		MrcpV2RefLib::CMrcpTaskProcessor* m_mrcpTaskProcessor;
		MrcpV2RefLib::CMrcpTaskProcessor* m_mrcpCallbackProcessor;
		MrcpV2RefLib::CMrcpCmdIdMgr* m_cmdIdMgr;
		MrcpV2RefLib::CMrcpSessionMgr* m_sessionMgr;
		MrcpV2RefLib::CMrcpHandleMgr* m_handleMgr;
		MrcpV2RefLib::CMrcpRtpMgr* m_rtpMgr;

		MrcpSystemStateEnum m_state;

		std::mutex m_controlMutex;
		std::string m_className;
	    std::string m_name;

	};
}//end namespace
#endif 