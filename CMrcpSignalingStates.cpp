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
//	File Name:		CMrcpSignalingStates.cpp
//
//	Description:	CMrcpSignalingStates class implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpSignalingUninitializedState::CMrcpSignalingUninitializedState;
//		CMrcpSignalingUninitializedState::~CMrcpSignalingUninitializedState;
//	    CMrcpSignalingStartingState::CMrcpSignalingStartingState
//		CMrcpSignalingStartingState::~CMrcpSignalingStartingState
//		CMrcpSignalingStartedState::CMrcpSignalingStartedState
//		CMrcpSignalingStartedState::~CMrcpSignalingStartedState
//		CMrcpSignalingConnectingState::CMrcpSignalingConnectingState
//		CMrcpSignalingConnectingState::~CMrcpSignalingConnectingState
//	    CMrcpSignalingConnectedState::CMrcpSignalingConnectedState
//	    CMrcpSignalingConnectedState::~CMrcpSignalingConnectedState
//		CMrcpSignalingAvailableState::CMrcpSignalingAvailableState
//		CMrcpSignalingAvailableState::~CMrcpSignalingAvailableState
//		CMrcpSignalingDisconnectingState::CMrcpSignalingDisconnectingState
//		CMrcpSignalingDisconnectingState::~CMrcpSignalingDisconnectingState
//		CMrcpSignalingDisconnectedState::CMrcpSignalingDisconnectedState
//		CMrcpSignalingDisconnectedState::~CMrcpSignalingDisconnectedState
//		CMrcpSignalingStoppingState::CMrcpSignalingStoppingState
//		CMrcpSignalingStoppingState::~CMrcpSignalingStoppingState
//		CMrcpSignalingStoppedState::CMrcpSignalingStoppedState
//		CMrcpSignalingStoppedState::~CMrcpSignalingStoppedState
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  4/1/07      TMB         Cleanup
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpSignalingStates.h"
#include "MrcpclientEvents.h"
#include "ClientInterfaceDefs.h"
#include "AMrcpSignaling.h"


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
CMrcpSignalingUninitializedState::CMrcpSignalingUninitializedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingUninitializedState::~CMrcpSignalingUninitializedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Start
// Input - signaling subject by reference
//         
// Output - success
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingUninitializedState::Start(AMrcpSignaling& a_subject)
{

	if (a_subject.PrimStart() != -1)
		a_subject.SetStateToStarting();
	//else and trigger callback
	return MrcpSuccess;
}   
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingStartingState::CMrcpSignalingStartingState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingStartingState::~CMrcpSignalingStartingState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingStarted
// Input - signaling object by reference
//         
// Output - success or failure
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingStartingState::OnSignalingStarted(AMrcpSignaling& a_subject)
{
	int l_rc = a_subject.PrimOnSignalingStarted();

	if (l_rc != -1)
	{
		a_subject.SetStateToStarted();
	}

	return l_rc;  
}
 ////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingStartedState::CMrcpSignalingStartedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingStartedState::~CMrcpSignalingStartedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Connect
// Input - signaling object by reference
//         
// Output - success or failure
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingStartedState::Connect(AMrcpSignaling& a_subject)
{   
	int l_rc = a_subject.PrimConnect();
	if (l_rc != -1)
	{
		a_subject.SetStateToConnecting();
	}

	return l_rc;

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingConnectingState::CMrcpSignalingConnectingState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingConnectingState::~CMrcpSignalingConnectingState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingConnected
// Input - signaling object by reference
//         
// Output - success
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingConnectingState::OnSignalingConnected(AMrcpSignaling& a_subject)
{
	a_subject.SetStateToConnected();
	return MrcpSuccess;
}   
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingConnected - sends ACK to server
// Input - Signaling object by reference
//         
// Output - success
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingConnectingState::SignalingAck(AMrcpSignaling& a_subject)
{
	int result = a_subject.PrimSendAck();
	return MrcpSuccess;
}   
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingConnectedState::CMrcpSignalingConnectedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingConnectedState::~CMrcpSignalingConnectedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingAvailable
// Input - signaling object by reference
//         
// Output - success
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingConnectedState::OnSignalingAvailable(AMrcpSignaling& a_subject)
{
	a_subject.SetStateToAvailable();
	return MrcpSuccess;
}  
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingAvailableState::CMrcpSignalingAvailableState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingAvailableState::~CMrcpSignalingAvailableState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SendCommand
// Input - signaling object by reference
//         message string by reference
// Output - None
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingAvailableState::SendCommand(AMrcpSignaling& a_subject, const std::string& a_message )
{
	return a_subject.PrimSendCommand(a_message);
} 

////////////////////////////////////////////////////////////////////////// 
//
// Description - Reconnect
// Input - signaling subject by reference
//         resource type
// Output - status from primreinvite call
//         part of the signaling state machine 
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingAvailableState::Reinvite(AMrcpSignaling& a_subject, MrcpResourceType a_resourceType)
{
	int l_result = a_subject.PrimReinvite(a_resourceType);
	if ( l_result == 0)
	{
		a_subject.SetStateToConnecting();
	}
	return l_result;
} 
////////////////////////////////////////////////////////////////////////// 
//
// Description - SignalingDisconnect
// Input - signaling object by reference
//         
// Output - result of call to PrimDisconnect
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingAvailableState::SignalingDisconnect(AMrcpSignaling& a_subject)
{
	int l_result = a_subject.PrimDisconnect();
	if ( l_result == 0 )
	{
		a_subject.SetStateToDisconnecting();
	}
	return l_result;
}   
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingDisconnectingState::CMrcpSignalingDisconnectingState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingDisconnectingState::~CMrcpSignalingDisconnectingState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingDisconnected
// Input - signaling object by reference
//         
// Output - success
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingDisconnectingState::OnSignalingDisconnected(AMrcpSignaling& a_subject)
{
	a_subject.SetStateToDisconnected();
	return MrcpSuccess;
}   
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingDisconnectedState::CMrcpSignalingDisconnectedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingDisconnectedState::~CMrcpSignalingDisconnectedState()
{

}	
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingDisconnected
// Input - Signaling object by reference
//         
// Output - success
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingDisconnectedState::SignalingStop(AMrcpSignaling& a_subject)
{
	a_subject.SetStateToStopping();
	int result = a_subject.PrimStop();
	return MrcpSuccess;
}   
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingStoppingState::CMrcpSignalingStoppingState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingStoppingState::~CMrcpSignalingStoppingState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingStopped
// Input - signaling object by reference
//         
// Output - success
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingStoppingState::OnSignalingStopped(AMrcpSignaling& a_subject)
{
	a_subject.SetStateToStopped();
	int result = a_subject.PrimOnStopped();

	return MrcpSuccess;
}  
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingStoppedState::CMrcpSignalingStoppedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpSignalingStoppedState::~CMrcpSignalingStoppedState()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingUninitialized
// Input - signaling object by reference
//         
// Output - success
//          part of the signaling state machine
///////////////////////////////////////////////////////////////////////////
int CMrcpSignalingStoppedState::OnSignalingUnintialized(AMrcpSignaling& a_subject)
{
	a_subject.SetStateToUninitialized();
	return MrcpSuccess;
}   
} //end namespace
