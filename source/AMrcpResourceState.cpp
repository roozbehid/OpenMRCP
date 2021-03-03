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
//	File Name:		AMrcpResourceState.cpp
//
//	Description:	AMrcpResourceState class implementation -
//				    Methods in this class are called if the command object is in a bad state
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//  AMrcpResourceState::AMrcpResourceState()
//	AMrcpResourceState::~AMrcpResourceState()
//	int AMrcpResourceState::Unitialized(AMrcpResource& a_state)
// 	int AMrcpResourceState::DefineGrammar(AMrcpResource& a_state,CRecDefineGrammar& a_task)
//	int AMrcpResourceState::Recognize(AMrcpResource& a_state,CRecRecognize& a_task)
//	int AMrcpResourceState::Interpret(AMrcpResource& a_state,CRecInterpret& a_task)
//  int AMrcpResourceState::GetResult(AMrcpResource& a_state,CRecGetResult& a_task)
//	int AMrcpResourceState::StartInputTimers(AMrcpResource& a_state,CRecStartInputTimers& a_task)
//	int AMrcpResourceState::StopRec(AMrcpResource& a_state,CRecStopRec& a_task)
//	int AMrcpResourceState::Idle(AMrcpResource& a_state)
//	int AMrcpResourceState::DefiningGrammar(AMrcpResource& a_state)
//	int AMrcpResourceState::GettingResult(AMrcpResource& a_state)
//	int AMrcpResourceState::Interpreting(AMrcpResource& a_state)
//	int AMrcpResourceState::StartingInputTimers(AMrcpResource& a_state)
//	int AMrcpResourceState::StoppingCommand(AMrcpResource& a_state)
//	int AMrcpResourceState::Recognizing(AMrcpResource& a_state)
//	int AMrcpResourceState::Speak(AMrcpResource& a_state,CSpkSpeak& a_task)
//  int AMrcpResourceState::Speaking(AMrcpResource& a_state)	
//	int AMrcpResourceState::StopSpeak(AMrcpResource& a_state,CSpkStop& a_task)
//  int AMrcpResourceState::StopSpeaking(AMrcpResource& a_state)
//  int AMrcpResourceState::Pause(AMrcpResource& a_state,CSpkPause& a_task)
//  int AMrcpResourceState::Pausing(AMrcpResource& a_state)
//  int AMrcpResourceState::Resume(AMrcpResource& a_state,CSpkResume& a_task)
//  int AMrcpResourceState::Resuming(AMrcpResource& a_state)
//  int AMrcpResourceState::BargeIn(AMrcpResource& a_state,CSpkBargeIn& a_task)
//  int AMrcpResourceState::BargingIn(AMrcpResource& a_state)
//  int AMrcpResourceState::Stopped(AMrcpResource& a_state)
//	int AMrcpResourceState::GetParams(AMrcpResource& a_state,CGetParams& a_task)
//	int AMrcpResourceState::SetParams(AMrcpResource& a_state,CSetParams& a_task)
//	void AMrcpResourceState::PerformBadStateCallback(int a_cmdID)		
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  1/2/07      TMB         Modified to return handles in callback structure
//  4/1/07      TMB         Cleanup
///////////////////////////////////////////////////////////////////////////////

#include "AMrcpResourceState.h"
#include "AMrcpResource.h"
#include "CRecognitionCommands.h"
#include "CSpeakCommands.h"
#include "CMrcpCmdIdMgr.h"
#include "CGenericCommands.h"


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
AMrcpResourceState::AMrcpResourceState()
{
	ClassName("AMrcpResourceState");
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
AMrcpResourceState::~AMrcpResourceState()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Unintialized
// Input - command passed by reference
//         
// Output - None
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Unintialized(AMrcpResource& a_state)
{
	StateDesc("Uninitalized");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - DefineGrammar
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//         
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::DefineGrammar(AMrcpResource& a_state,CRecDefineGrammar& a_task)
{
	StateDesc("DefineGrammar");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Recognize
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//         
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Recognize(AMrcpResource& a_state,CRecRecognize& a_task)
{
	StateDesc("Recognize");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Interpret
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Interpret(AMrcpResource& a_state,CRecInterpret& a_task)
{
	StateDesc("Interpret");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - GetResult
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//       
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::GetResult(AMrcpResource& a_state,CRecGetResult& a_task)
{
	StateDesc("Get Result");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StartInputTimers
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::StartInputTimers(AMrcpResource& a_state,CRecStartInputTimers& a_task)
{
	StateDesc("Start Input Timers");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StopRec
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::StopRec(AMrcpResource& a_state,CRecStopRec& a_task)
{
	StateDesc("Stop Rec");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Idle
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Idle(AMrcpResource& a_state)
{
	StateDesc("Idle");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - DefiningGrammar
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::DefiningGrammar(AMrcpResource& a_state)
{
	StateDesc("DefiningGrammar");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - GettingResult
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::GettingResult(AMrcpResource& a_state)
{
	StateDesc("GettingResult");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Interpreting
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Interpreting(AMrcpResource& a_state)
{
	StateDesc("Interpreting");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StartingInputTimers
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::StartingInputTimers(AMrcpResource& a_state)
{
	StateDesc("StartingInputTimers");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StoppingCommand
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::StoppingCommand(AMrcpResource& a_state)
{
	StateDesc("StoppingCommand");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Recognize
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Recognizing(AMrcpResource& a_state)
{
	StateDesc("Recognizing");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Speak
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Speak(AMrcpResource& a_state,CSpkSpeak& a_task)
{
	StateDesc("Speak");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Speaking
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Speaking(AMrcpResource& a_state)
{
	StateDesc("Speaking");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - StopSpeak
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::StopSpeak(AMrcpResource& a_state,CSpkStop& a_task)
{
	StateDesc("Stop Speak");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - StopSpeaking
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::StopSpeaking(AMrcpResource& a_state)
{
	StateDesc("Stop Speaking");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Pause
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Pause(AMrcpResource& a_state,CSpkPause& a_task)
{
	StateDesc("Pause");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Pausing
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Pausing(AMrcpResource& a_state)
{
	StateDesc("Pausing");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Resume
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Resume(AMrcpResource& a_state,CSpkResume& a_task)
{
	StateDesc("Resume");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Resuming
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Resuming(AMrcpResource& a_state)
{
	StateDesc("Resuming");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - BargeIn
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::BargeIn(AMrcpResource& a_state,CSpkBargeIn& a_task)
{
	StateDesc("BargeIn");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());
	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - BargingIn
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::BargingIn(AMrcpResource& a_state)
{
	StateDesc("BargingIn");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Stopped
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::Stopped(AMrcpResource& a_state)
{
	StateDesc("Stopped");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnBargeInComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnBargeInComplete(AMrcpResource& a_state)
{
	StateDesc("BargeIn Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnResumingComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnResumingComplete(AMrcpResource& a_state)
{
	StateDesc("OnResuming Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnPausingComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnPausingComplete(AMrcpResource& a_state)
{
	StateDesc("OnPausing Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnStopSpeakComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnStopSpeakComplete(AMrcpResource& a_state)
{
	StateDesc("OnStopSpeak Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSpeakingComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnSpeakingComplete(AMrcpResource& a_state)
{
	StateDesc("OnSpeaking Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnDefineGrammarComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnDefineGrammarComplete(AMrcpResource& a_state)
{
	StateDesc("On Define Grammar Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnRecognitionComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnRecognitionComplete(AMrcpResource& a_state)
{
	StateDesc("On Recognition Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnInterpretingComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnInterpretingComplete(AMrcpResource& a_state)
{
	StateDesc("On Interpreting Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnStartInputTimersComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnStartInputTimersComplete(AMrcpResource& a_state)
{
	StateDesc("On Start Input Timers Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnGettingResultComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnGettingResultComplete(AMrcpResource& a_state)
{
	StateDesc("On Getting Result Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnStopRecComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnStopRecComplete(AMrcpResource& a_state)
{
	StateDesc("On Stop Rec Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}	
////////////////////////////////////////////////////////////////////////// 
//
// Description - GetParams
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::GetParams(AMrcpResource& a_state,CGetParams& a_task)
{
	StateDesc("GetParams");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SetParams
// Input - command by reference
//         task by reference
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::SetParams(AMrcpResource& a_state,CSetParams& a_task)
{
	StateDesc("SetParams");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	PerformBadStateCallback(a_task.m_cmdID,a_state.m_signalObj->AsrSessionHandle(), a_state.m_signalObj->TtsSessionHandle());

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SettingParams
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::SettingParams(AMrcpResource& a_state)
{
	StateDesc("SettingParams");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");

	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSettingParamsComplete
// Input - command by reference
//         
// Output - -1 indicating bad state
//          This method should only be called if the command state
//          machine is in a state that prevents the command primitive
//		    from executing.
//          
///////////////////////////////////////////////////////////////////////////
int AMrcpResourceState::OnSettingParamsComplete(AMrcpResource& a_state)
{
	StateDesc("OnSettingParams Complete");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}	
////////////////////////////////////////////////////////////////////////// 
//
// Description - PerformBadStateCallback
// Input - command id, asr session handle, tts session handle
//         
// Output - None
//    This method reports the bad state back to the client application
//    via the callback mechanism
//
///////////////////////////////////////////////////////////////////////////
void AMrcpResourceState::PerformBadStateCallback(int a_cmdID, MrcpSessionHandle a_asrSessionHandle, MrcpSessionHandle a_ttsSessionHandle)
{
	MRCP_RESULT_STRUCT l_callResults;

	l_callResults.MrcpReturnValue = -1;
	l_callResults.referenceNumber = a_cmdID;
	l_callResults.asrSessionHandle = a_asrSessionHandle;
	l_callResults.ttsSessionHandle = a_ttsSessionHandle;
	std::string statusMessage = "Bad state for command " + StateDesc();
	strncpy(l_callResults.MrcpReturnPkt,statusMessage.c_str(), statusMessage.size());
	MrcpCallBackFunction l_callBack = CMrcpCmdIdMgr::Instance()->GetCommandObj(a_cmdID);
	CMrcpCmdIdMgr::Instance()->PerformCallbackProcessing(l_callResults,true,l_callBack);
}
}//end namespace