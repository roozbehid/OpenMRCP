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
//	File Name:		CMrcpSession.h
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
#ifndef CMrcpSession_H
#define CMrcpSession_H

#include "AMrcpEventHandler.h"
#include "ClientInterfaceDefs.h"
#include "CLogger.h"
//////////////////////

#include <string>

typedef enum 
{
   SESSION_STARTING,   
   SESSION_CONNECTING,
   SESSION_CONNECTED,
   SESSION_DISCONNECTED,
   SESSION_STOPPING
} SessionStateEnum;

namespace MrcpV2RefLib
{
	class AMrcpSignaling;
	class CMrcpTaskProcessor;
	class CMrcpCmdIdMgr;
	class CSpeechRecognizer;
	class CSpeechSynthesizer;
	class CMrcpRtpMgr;
	class CMrcpRtp;
	class CMrcpEvent;
	class CMrcpAudioStream;
	class CMrcpGeneric;
	class CMrcpHandleMgr;


	class CMrcpSession :public AMrcpEventHandler

	{

	public:
		CMrcpSession(RESOURCE_CFG_STRUCT* a_resourceCfg, bool a_asrResource, bool a_ttsResource);
		virtual ~CMrcpSession();
		int StartSession();
		int StopSession();
		int AddResourceToSession(MrcpResourceType a_resourceToAdd);
		// commands
		int DefineGrammar(DEFINE_GRAMMAR_STRUCT* a_defineGrammarParams);
		int Recognize(RECOGNIZE_STRUCT* a_recognizeParams);
		int Interpret(INTERPRET_STRUCT* a_interpretParams);
		int StartInputTimers(MrcpCallBackFunction a_callBack, int& a_referenceNum);
		int GetResult(MrcpCallBackFunction a_callBack,int& a_referenceNum);
		int StopRecog(MrcpCallBackFunction a_callBack,int& a_referenceNum);
		/////////
		int Speak(SPEAK_STRUCT* a_speakParams);
		int StopSpeak(MrcpCallBackFunction a_callBack,int& a_referenceNum);
		int Pause(MrcpCallBackFunction a_callBack,int& a_referenceNum);
		int Resume(MrcpCallBackFunction a_callBack,int& a_referenceNum);
		int BargeIn(MrcpCallBackFunction a_callBack,int& a_referenceNum);
		////////
		int GetParams(MrcpCallBackFunction a_callBack, GET_PARAMETERS a_parameters, int& a_referenceNumber);
		int SetParams(MrcpCallBackFunction a_callBack, SET_PARAMETERS a_parameters, int& a_referenceNumber);

		std::string ServerAddress(){return m_serverAddress;};
		int ServerPort() {return m_serverPort;};
		std::string ClientAddress() {return m_clientAddress;};
		MrcpCallBackFunction AsrCallBack() { return m_asrCallback;};
		MrcpCallBackFunction TtsCallBack() { return m_ttsCallback;};
		bool AsrResource() {return m_asrResource;};
		bool TtsResource() {return m_ttsResource;};
		CMrcpRtp* GetSessionPtr() {return m_rtpPtr;};

		const char* StateDesc() { return "N/A";}		
		const std::string& ClassName(){return m_className;};
		void ClassName(const std::string& a_className) {m_className = a_className;};
		const std::string& Name(){return m_name;};
		void Name( const std::string& a_name){m_name = a_name;};
		LogObjectTypeEnum LogObjectType(){return LogObject_System;};

	protected:

	private:
		SessionStateEnum m_state;
		RESOURCE_CFG_STRUCT* m_resourceCfg;
		void HandleEvent(CMrcpEvent* a_event);
		void PerformSessionCallback(int a_status);
		
		MrcpV2RefLib::CMrcpCmdIdMgr* m_cmdIdMgr;
		MrcpV2RefLib::CMrcpTaskProcessor* m_mrcpTaskProcessor;
		MrcpV2RefLib::CSpeechRecognizer* m_speechRecResource;
		MrcpV2RefLib::CSpeechSynthesizer* m_speechSynthResource;
		MrcpV2RefLib::CMrcpGeneric* m_genericCommandResource;
		MrcpV2RefLib::CMrcpRtpMgr* m_rtpMgr;
		MrcpV2RefLib::CMrcpRtp* m_rtpPtr;
		MrcpV2RefLib::AMrcpSignaling* m_signalObj;
		MrcpV2RefLib::CMrcpHandleMgr* m_handleMgr;


		std::string m_serverAddress;
		int m_serverPort;
		std::string m_clientAddress;
		MrcpCallBackFunction m_asrCallback;
		MrcpCallBackFunction m_ttsCallback;
		int  m_mrcpAsrVersion;
		int m_mrcpTtsVersion;
		int m_sessionCmdID;
		bool m_asrResource;
		bool m_ttsResource;
		MrcpSessionHandle m_sessionHandle;

	    std::string m_className;
	    std::string m_name;

	};

}//end namespace

#endif