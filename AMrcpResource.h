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
//	File Name:		AMrcpResource.h
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


#ifndef AMrcpResource_H
#define AMrcpResource_H

#include "AMrcpEventHandler.h"

namespace MrcpV2RefLib
{
	class CRecRecognize;
	class CRecDefineGrammar;
	class CRecInterpret;
	class CRecStopRec;
	class CRecStartInputTimers;
	class CRecGetResult;
	class CSpkSpeak;
	class CSpkStop;
	class CSpkPause;
	class CSpkResume;
	class CSpkBargeIn;
	class AMrcpResourceState;
	class CGetParams;
	class CSetParams;
	class AMrcpSignaling;
	class AMrcpResourceState;

	class AMrcpResource : public AMrcpEventHandler

	{
		friend class AMrcpClientCommand;
		friend class AMrcpResourceState;
		friend class CMrcpResourceUninitializedState;
		friend class CMrcpResourceIdleState;
		friend class CMrcpResourceStoppedState;
		friend class CMrcpResourceDefiningGrammarState;
		friend class CMrcpResourceRecognizingState;
		friend class CRecDefineGrammar;
		friend class CRecRecognize;
		friend class CRecInterpret;
		friend class CRecGetResult;
		friend class CRecStopRec;
		friend class CRecStartInputTimers;
		friend class CSpkSpeak;
		friend class CSpkStop;
		friend class CSpkPause;
		friend class CSpkResume;
		friend class CSpkBargeIn;
		friend class CGetParams;
		friend class CSetParams;
		friend class CMrcpResourceInterpretingState;
		friend class CMrcpResourceGettingResultState;
		friend class CMrcpResourceStartingInputTimersState;
		friend class CMrcpResourceStoppingRecState;
		friend class CMrcpResourceSpeakingState;
		friend class CMrcpResourceStoppingSpeakState;
		friend class CMrcpResourcePausingState;
		friend class CMrcpResourcePausedState;
		friend class CMrcpResourceResumingState;
		friend class CMrcpResourceBargingInState;
		friend class CMrcpResourceSettingParamsState;
		friend class CSpeechSynthesizer;
		friend class CSpeechRecognizer;
		friend class CMrcpGeneric;
		public:
			AMrcpResource();
			virtual ~AMrcpResource();
			void BasicInitialization();
			AMrcpSignaling* m_signalObj;
			 
		protected:
			AMrcpResourceState* m_state;
			virtual int PrimDefineGrammar(CRecDefineGrammar& a_task) = 0;
			virtual int PrimRecognize(CRecRecognize& a_task) = 0;
			virtual int PrimInterpret(CRecInterpret& a_task) = 0;
			virtual int PrimGetResult(CRecGetResult& a_task) = 0;
			virtual int PrimStopRec(CRecStopRec& a_task) = 0;
			virtual int PrimStartInputTimers(CRecStartInputTimers& a_task) = 0;
			virtual int PrimSpeak(CSpkSpeak& a_task) = 0;
			virtual int PrimStopSpk(CSpkStop& a_task) = 0;
			virtual int PrimPause(CSpkPause& a_task) = 0;
			virtual int PrimResume(CSpkResume& a_task) = 0;
			virtual int PrimBargeIn(CSpkBargeIn& a_task) = 0;
			virtual int PrimGetParams(CGetParams& a_task) = 0;
			virtual int PrimSetParams(CSetParams& a_task) = 0;
		private:
			int SetState(AMrcpResourceState* a_state);
			int SetStateToUninitialized() { return SetState( m_resourceUninitializedState ); };
			int SetStateToIdle() { return SetState( m_resourceIdleState ); };
			int SetStateToStopped() { return SetState( m_resourceStoppedState ); };
			int SetStateToDefiningGrammar() { return SetState( m_resourceDefiningGrammarState ); };
			int SetStateToRecognizing() { return SetState( m_resourceRecognizingState ); };
			int SetStateToInterpreting() { return SetState( m_resourceInterpretingState ); };
			int SetStateToGettingResult() { return SetState( m_resourceGettingResultState );};
			int SetStateToStartingInputTimers() { return SetState(m_resourceStartingInputTimersState);};
			int SetStateToStoppingRec() { return SetState(m_resourceStoppingRecState);};
			int SetStateToSpeaking() { return SetState(m_resourceSpeakingState);};
			int SetStateToStoppingSpk() { return SetState(m_resourceStoppingSpkState);};
			int SetStateToPausing() { return SetState(m_resourcePausingState);};
			int SetStateToPaused() { return SetState(m_resourcePausedState);};
			int SetStateToResuming() { return SetState(m_resourceResumingState);};
			int SetStateToBargingIn() { return SetState(m_resourceBargingInState);};
			int SetStateToSettingParams() {return SetState(m_resourceSettingParamsState);};

			AMrcpResourceState* m_resourceUninitializedState;
			AMrcpResourceState* m_resourceIdleState;
			AMrcpResourceState* m_resourceStoppedState;
			AMrcpResourceState* m_resourceDefiningGrammarState;
			AMrcpResourceState* m_resourceInterpretingState;
			AMrcpResourceState* m_resourceRecognizingState;
			AMrcpResourceState* m_resourceGettingResultState;
			AMrcpResourceState* m_resourceStartingInputTimersState;
			AMrcpResourceState* m_resourceStoppingRecState;
			AMrcpResourceState* m_resourceSpeakingState;
			AMrcpResourceState* m_resourceStoppingSpkState;
			AMrcpResourceState* m_resourcePausingState;
			AMrcpResourceState* m_resourcePausedState;
			AMrcpResourceState* m_resourceResumingState;
			AMrcpResourceState* m_resourceBargingInState;
			AMrcpResourceState* m_resourceSettingParamsState;

			void DefineGrammar(AMrcpResource& a_clientCommand, CRecDefineGrammar& a_speechCmd);
			void Recognize(AMrcpResource& a_clientCommand, CRecRecognize& a_speechCmd);
			void Interpret(AMrcpResource& a_clientCommand, CRecInterpret& a_speechCmd);
			void GetResult(AMrcpResource& a_clientCommand, CRecGetResult& a_speechCmd);
			void StopRec(AMrcpResource& a_clientCommand, CRecStopRec& a_speechCmd);
			void StartInputTimers(AMrcpResource& a_clientCommand, CRecStartInputTimers& a_speechCmd);
//Next The TTS Stuff
			void Speak(AMrcpResource& a_clientCommand, CSpkSpeak& a_speechCmd);
			void StopSpeak(AMrcpResource& a_clientCommand, CSpkStop& a_speechCmd);
			void Pause(AMrcpResource& a_clientCommand, CSpkPause& a_speechCmd);
			void Resume(AMrcpResource& a_clientCommand, CSpkResume& a_speechCmd);
			void BargeIn(AMrcpResource& a_clientCommand, CSpkBargeIn& a_speechCmd);
// finally the generics
			void GetParams(AMrcpResource& a_clientCommand, CGetParams& a_genericCmd);
			void SetParams(AMrcpResource& a_clientCommand, CSetParams& a_genericCmd);

			void HandleEvent(CMrcpEvent *a_event);

	};
}

#endif //ifndef