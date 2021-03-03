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
//	File Name:		AMrcpSignaling.cpp
//
//	Description:	AMrcpSignaling class implementation
//					part of the signaling state machine
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		AMrcpSignaling::AMrcpSignaling()
//		AMrcpSignaling::~AMrcpSignaling()
//		void AMrcpSignaling::BasicInitialization()
//		int AMrcpSignaling::Start()
//		int AMrcpSignaling::Reinvite(MrcpResourceType a_resourceToAdd)
//		int AMrcpSignaling::Connect()
//		int AMrcpSignaling::SendCommand()
//		int AMrcpSignaling::OnSignalingStarted()
//		int AMrcpSignaling::OnSignalingConnected()
//		int AMrcpSignaling::OnSignalingAvailable()
//		int AMrcpSignaling::SignalingStop()
//		int AMrcpSignaling::OnSignalingStopped()
//		int AMrcpSignaling::SignalingDisconnect()
//		int AMrcpSignaling::OnSignalingDisconnected()
//		int AMrcpSignaling::SetState(AMrcpSignalingState* a_state)
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  4/1/07      TMB         Cleanup
///////////////////////////////////////////////////////////////////////////////

#include "AMrcpSignaling.h"
#include "MrcpClientEvents.h"
#include "ClientInterfaceDefs.h"
#include "AMrcpEventHandler.h"

#include "AMrcpObserverSubject.h"

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

AMrcpSignaling::AMrcpSignaling()
	: m_state(0)
	, m_signalingUninitializedState(0)
	, m_signalingStartingState(0)
	, m_signalingStartedState(0)
	, m_signalingConnectingState(0)
	, m_signalingConnectedState(0)
	, m_signalingAvailableState(0)
	, m_signalingStoppingState(0)
	, m_signalingStoppedState(0)
	, m_signalingDisconnectingState(0)
	, m_signalingDisconnectedState(0)
	, m_mrcpSetupSocket(0)
	, m_mrcpCommandSocket(0)
	, m_synthChannelID(" ")
	, m_recogChannelID(" ")
	, m_cSeq (1)

{
	BasicInitialization();
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
AMrcpSignaling::~AMrcpSignaling()
{
	m_state = 0;
	delete m_signalingUninitializedState;
	delete m_signalingStartingState;
	delete m_signalingStartedState;
	delete m_signalingConnectingState;
	delete m_signalingConnectedState;
	delete m_signalingAvailableState;
	delete m_signalingStoppingState;
	delete m_signalingStoppedState;
	delete m_signalingDisconnectingState;
	delete m_signalingDisconnectedState;

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - BasicInitialization
// Input - None
//         
// Output - None
//          Instantiate each of the state classes
//			set the initial signaling state
///////////////////////////////////////////////////////////////////////////
void AMrcpSignaling::BasicInitialization()
{
	m_signalingUninitializedState = new CMrcpSignalingUninitializedState();
	m_signalingStartingState = new CMrcpSignalingStartingState();
	m_signalingStartedState = new CMrcpSignalingStartedState();
	m_signalingConnectingState = new CMrcpSignalingConnectingState();
	m_signalingConnectedState = new CMrcpSignalingConnectedState();
	m_signalingAvailableState = new CMrcpSignalingAvailableState();
	m_signalingStoppingState = new CMrcpSignalingStoppingState();
	m_signalingStoppedState = new CMrcpSignalingStoppedState();
	m_signalingDisconnectingState = new CMrcpSignalingDisconnectingState();
	m_signalingDisconnectedState = new CMrcpSignalingDisconnectedState();

	m_state = m_signalingUninitializedState;

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Start
// Input - None
//         
// Output - call to start state
//        start the signaling object  
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::Start()
{
	return m_state->Start( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Reinvite
// Input - resource to add to a session
//         
// Output - results of set state ot reinvite
//      call reinvite method for the signaling state    
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::Reinvite(MrcpResourceType a_resourceToAdd)
{
	return m_state->Reinvite( *this ,a_resourceToAdd);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Connect
// Input - None
//         
// Output - results of setting state to connect
//          call the connect method for the signaling state
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::Connect()
{
	return m_state->Connect( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SendCommand
// Input - None
//         
// Output - result of setting state to send command
//         send a command for the signaling state
//			all signaling commands pass through here
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::SendCommand()
{
	return m_state->SendCommand( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingStarted
// Input - None
//         
// Output - result of setting state to signaling started
//         call signaling states on signaling started method 
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::OnSignalingStarted()
{
	return m_state->OnSignalingStarted( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingFailed
// Input - None
//         
// Output - result of setting state to signaling failed
//         call signaling states on signaling failed method 
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::OnSignalingFailed()
{
	NotifyObservers(SIGNALING_PROTOCOL_FAILED);
	return 0;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingConnected
// Input - None
//         
// Output - results of setting state to signaling connected
//          call signaling states on signaling connected method
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::OnSignalingConnected()
{	
	NotifyObservers(SIGNALING_PROTOCOL_CONNECTED);
	return m_state->OnSignalingConnected( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingAvailable
// Input - None
//         
// Output - results of setting state to signaling available
//         call signaling states on signaling available method 
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::OnSignalingAvailable()
{
	return m_state->OnSignalingAvailable( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Ack
// Input - None
//         
// Output - results of setting state to send Ack
//         call signaling objects Ack method  
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::SignalingAck()
{
	return m_state->SignalingAck( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Stop
// Input - None
//         
// Output - results of setting state to signaling stop
//         call signaling objects stop method  
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::SignalingStop()
{
	return m_state->SignalingStop( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingStopped
// Input - None
//         
// Output - rtesults of setting state to signaling stopped
//          call signaling states on signaling stopped method
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::OnSignalingStopped()
{
	return m_state->OnSignalingStopped( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SignalingDisconnect
// Input - None
//         
// Output - results of setting state to signaling disconnect
//          call signaling states signaling disconnect method
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::SignalingDisconnect()
{
	return m_state->SignalingDisconnect( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingDisconnected
// Input - None
//         
// Output - results of setting state to signaling disconnected
//         call signaling states on discconected method 
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::OnSignalingDisconnected()
{
	return m_state->OnSignalingDisconnected( *this );
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SetState
// Input - pointer to state to set
//         
// Output - success
//          used to set the state for the signaling object
///////////////////////////////////////////////////////////////////////////
int AMrcpSignaling::SetState(AMrcpSignalingState* a_state)
{
	m_state = a_state;

	return MrcpSuccess;
}


}//end namespace