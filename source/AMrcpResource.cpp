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
//	File Name:		AMrcpResource.cpp
//
//	Description:	class implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//	    AMrcpResource::AMrcpResource()
//		AMrcpResource::~AMrcpResource()
//	    void AMrcpResource::BasicInitialization()
//	    void AMrcpResource::HandleEvent(CMrcpEvent *a_event)
//	    int AMrcpResource::SetState(AMrcpCommandState* a_state)
//	    void AMrcpResource::DefineGrammar(AMrcpResource& a_resourceState, CRecDefineGrammar& a_speechTask)
//	    void AMrcpResource::Recognize(AMrcpResource& a_resourceState, CRecRecognize& a_speechTask)
//	    void AMrcpResource::Interpret(AMrcpResource& a_resourceState, CRecInterpret& a_speechTask)
//	    void AMrcpResource::GetResult(AMrcpResource& a_resourceState, CRecGetResult& a_speechTask)
//	    void AMrcpResource::StartInputTimers(AMrcpResource& a_resourceState, CRecStartInputTimers& a_speechTask)
//	    void AMrcpResource::StopRec(AMrcpResource& a_resourceState, CRecStopRec& a_speechTask)
//	    void AMrcpResource::Speak(AMrcpResource& a_resourceState, CSpkSpeak& a_speechTask)
//	    void AMrcpResource::StopSpeak(AMrcpResource& a_resourceState, CSpkStop& a_speechTask)
//	    void AMrcpResource::Pause(AMrcpResource& a_resourceState, CSpkPause& a_speechTask)
//	    void AMrcpResource::Resume(AMrcpResource& a_resourceState, CSpkResume& a_speechTask)
//	    void AMrcpResource::BargeIn(AMrcpResource& a_resourceState, CSpkBargeIn& a_speechTask)
//	    void AMrcpResource::GetParams(AMrcpResource& a_resourceState, CGetParams& a_task)
//	    void AMrcpResource::SetParams(AMrcpResource& a_resourceState, CSetParams& a_task)
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  4/1/07      TMB         Cleanup
///////////////////////////////////////////////////////////////////////////////
//  The CMrcpSession module instantiates a command object & adds to task queue.
//  Task queue process comand calling objects execute method
//  Abstract AMrcpResource method called from the command objects execute method
//  State pointer calls command
//
//			   if state permits command
//					command state object calls primitive to execute command
//				else
//			        AMrcpCommandState method called to report bad state          
///////////////////////////////////////////////////////////////////////////////

#include "AMrcpResource.h"
#include "CMrcpResourceState.h"
#include "MrcpTasks.h"
#include "MrcpClientEvents.h"
#include "AMrcpObserverSubject.h"
#include "CMrcpEvent.h"
#include "AMrcpSignaling.h"
#include "ClientInterfaceDefs.h"

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
AMrcpResource::AMrcpResource()
	:m_state(0)
	,m_resourceUninitializedState(0)
	,m_resourceIdleState(0)
	,m_resourceStoppedState(0)
	,m_resourceDefiningGrammarState(0)
	,m_resourceRecognizingState(0)
	,m_resourceInterpretingState(0)
	,m_resourceGettingResultState(0)
	,m_resourceStartingInputTimersState(0)
	,m_resourceStoppingRecState(0)
	,m_resourceSpeakingState(0)
	,m_resourceStoppingSpkState(0)
	,m_resourcePausingState(0)
	,m_resourcePausedState(0)
	,m_resourceResumingState(0)
	,m_resourceBargingInState(0)
	,m_resourceSettingParamsState(0)

{
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//      clean up and unregister for event notification    
///////////////////////////////////////////////////////////////////////////
AMrcpResource::~AMrcpResource()
{
	m_state = 0;
	delete m_resourceUninitializedState;
	delete m_resourceIdleState;
	delete m_resourceStoppedState;
	delete m_resourceDefiningGrammarState;
	delete m_resourceRecognizingState;
	delete m_resourceInterpretingState;
	delete m_resourceGettingResultState;
	delete m_resourceStartingInputTimersState;
	delete m_resourceStoppingRecState;
	delete m_resourceSpeakingState;
	delete m_resourceStoppingSpkState;
	delete m_resourcePausingState;
	delete m_resourcePausedState;
	delete m_resourceResumingState;
	delete m_resourceBargingInState;
	delete m_resourceSettingParamsState;

	m_signalObj->Unregister(this, SIGNALING_DISCONNECTED);
	m_signalObj->Unregister(this, COMMAND_COMPLETE);

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - BasicInitialization
// Input - None
//         
// Output - None
//          Instantiates all command states and register as an observer 
//          with the signaling object
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::BasicInitialization()
{
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     	
	m_resourceUninitializedState = new CMrcpResourceUninitializedState;
	m_resourceIdleState = new CMrcpResourceIdleState;
	m_resourceStoppedState = new CMrcpResourceStoppedState;
	m_resourceDefiningGrammarState = new CMrcpResourceDefiningGrammarState;
	m_resourceRecognizingState = new CMrcpResourceRecognizingState;
	m_resourceInterpretingState = new CMrcpResourceInterpretingState;
	m_resourceGettingResultState = new CMrcpResourceGettingResultState;
	m_resourceStartingInputTimersState = new CMrcpResourceStartingInputTimersState;
	m_resourceStoppingRecState = new CMrcpResourceStoppingRecState;
	m_resourceSpeakingState = new CMrcpResourceSpeakingState;
	m_resourceStoppingSpkState = new CMrcpResourceStoppingSpeakState;
	m_resourcePausingState = new CMrcpResourcePausingState;
	m_resourcePausedState = new CMrcpResourcePausedState;
	m_resourceResumingState = new CMrcpResourceResumingState;
	m_resourceBargingInState = new CMrcpResourceBargingInState;
	m_resourceSettingParamsState = new CMrcpResourceSettingParamsState;

	m_signalObj->RegisterForNotification(this, SIGNALING_DISCONNECTED);
	m_signalObj->RegisterForNotification(this, COMMAND_COMPLETE);

	m_state = m_resourceIdleState;

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - SetState
// Input - pointer to the state
//         
// Output - success
//          sets the state for the command object
///////////////////////////////////////////////////////////////////////////

int AMrcpResource::SetState(AMrcpResourceState* a_state)
{
	m_state = a_state;
	return MrcpSuccess;
}
	////////////////////////////////////////////////////////////////////////// 
//
// Description - DefineGrammar
// Input - command by reference, 
//         task by reference
// Output - None
//  Mrcp Define Grammar
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::DefineGrammar(AMrcpResource& a_resourceState, CRecDefineGrammar& a_speechTask)
{ 
	m_state->DefineGrammar(a_resourceState, a_speechTask);	
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Recognize
// Input - command by reference
//         task by reference
// Output - None
//  Mrcp Recognize
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::Recognize(AMrcpResource& a_resourceState, CRecRecognize& a_speechTask)
{ 
	m_state->Recognize(a_resourceState, a_speechTask);
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Interpret
// Input - command by reference
//         task by reference
// Output - None
//  Mrcp interpert    
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::Interpret(AMrcpResource& a_resourceState, CRecInterpret& a_speechTask)
{ 
	m_state->Interpret(a_resourceState, a_speechTask);
	
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - GetResult
// Input - command by reference
//         task by reference
// Output - None
//  Mrcp Get Result 
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::GetResult(AMrcpResource& a_resourceState, CRecGetResult& a_speechTask)
{ 
	m_state->GetResult(a_resourceState, a_speechTask);
	
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - StartInputTimers
// Input - command by reference
//         task by reference
// Output - None
// Mrcp start input timers 
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::StartInputTimers(AMrcpResource& a_resourceState, CRecStartInputTimers& a_speechTask)
{ 
	m_state->StartInputTimers(a_resourceState, a_speechTask);
	
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - StopRec
// Input - Command by reference
//         task by reference
// Output - None
// equal to MRCP STOP
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::StopRec(AMrcpResource& a_resourceState, CRecStopRec& a_speechTask)
{ 
	m_state->StopRec(a_resourceState, a_speechTask);
	
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Speak
// Input - command by reference
//         task by reference
// Output - None
//  Mrcp Speak function
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::Speak(AMrcpResource& a_resourceState, CSpkSpeak& a_speechTask)
{ 
	m_state->Speak(a_resourceState, a_speechTask);
	
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - SpeakStop
// Input - command by reference
//         task by reference
// Output - None
// equal to MRCP STOP
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::StopSpeak(AMrcpResource& a_resourceState, CSpkStop& a_speechTask)
{ 
	m_state->StopSpeak(a_resourceState, a_speechTask);
	
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Pause
// Input - command by reference
//         task by reference
// Output - None
// Mrcp Pause function
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::Pause(AMrcpResource& a_resourceState, CSpkPause& a_speechTask)
{ 
	m_state->Pause(a_resourceState, a_speechTask);
	
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Resume
// Input - command by reference
//         task by reference
// Output - None
// Mrcp Resume function                       
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::Resume(AMrcpResource& a_resourceState, CSpkResume& a_speechTask)
{ 
	m_state->Resume(a_resourceState, a_speechTask);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - BargeIn
// Input - command by reference
//         task by reference
// Output - None
// Mrcp Barge In function
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::BargeIn(AMrcpResource& a_resourceState, CSpkBargeIn& a_speechTask)
{ 
	m_state->BargeIn(a_resourceState, a_speechTask);
	
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - GetParams
// Input - command by reference
//         task by reference
// Output - None
// Mrcp GetParams function                
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::GetParams(AMrcpResource& a_resourceState, CGetParams& a_task)
{ 
	m_state->GetParams(a_resourceState, a_task);
	
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SetParams
// Input - command by reference
//         task by reference
// Output - None
// MRCP SetParams function
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::SetParams(AMrcpResource& a_resourceState, CSetParams& a_task)
{ 
	m_state->SetParams(a_resourceState, a_task);
	
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - HandleEvent
// Input - pointer to event
//         
// Output - None
//          Process the events received as an observer of the signaling
//		    object
///////////////////////////////////////////////////////////////////////////
void AMrcpResource::HandleEvent(CMrcpEvent *a_event)
{
	switch (a_event->EventID())
	{
	case COMMAND_COMPLETE:
		if (m_state == m_resourceSpeakingState)
			m_state->OnSpeakingComplete(*this);
		if (m_state == m_resourceStoppingSpkState)
			m_state->OnStopSpeakComplete(*this);
		if (m_state == m_resourcePausingState)
			m_state->OnPausingComplete(*this);
		if (m_state == m_resourceResumingState)
			m_state->OnResumingComplete(*this);
		if (m_state == m_resourceBargingInState)
			m_state->OnBargeInComplete(*this);
		if (m_state == m_resourceDefiningGrammarState)
			m_state->OnDefineGrammarComplete(*this);
		if (m_state == m_resourceRecognizingState)
			m_state->OnRecognitionComplete(*this);
		if (m_state == m_resourceInterpretingState)
			m_state->OnInterpretingComplete(*this);
		if (m_state == m_resourceStartingInputTimersState)
			m_state->OnStartInputTimersComplete(*this);
		if (m_state == m_resourceGettingResultState)
			m_state->OnGettingResultComplete(*this);
		if (m_state == m_resourceStoppingRecState)
			m_state->OnStopRecComplete(*this);
		if (m_state == m_resourceSettingParamsState)
			m_state->OnSettingParamsComplete(*this);
		break;
	case SIGNALING_PROTOCOL_DISCONNECTED:
		SetState(m_resourceUninitializedState);
		break;
	}
}
}//end namespace