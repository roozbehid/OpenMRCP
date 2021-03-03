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
//	File Name:		AMrcpSignalingState.cpp
//
//	Description:	 AMrcpSignalingState class implementation
//					 Part of the signaling state machine
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		AMrcpSignalingState::AMrcpSignalingState()
//		AMrcpSignalingState::~AMrcpSignalingState()
//		int AMrcpSignalingState::Start(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::OnSignalingStarted(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::Connect(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::OnSignalingConnected(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::OnSignalingAvailable(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::SendCommand(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::Reinvite(AMrcpSignaling& a_signaling,MrcpResourceType a_resourceType)
//		int AMrcpSignalingState::SignalingDisconnect(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::OnSignalingDisconnected(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::SignalingStop(AMrcpSignaling& a_signaling)
//		int AMrcpSignalingState::OnSignalingStopped(AMrcpSignaling& a_signaling)
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//
///////////////////////////////////////////////////////////////////////////////

#include "AMrcpSignalingState.h"

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

AMrcpSignalingState::AMrcpSignalingState()
{
	ClassName("AMrcpSignalingState");
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
AMrcpSignalingState::~AMrcpSignalingState()
{
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Start 
// Input - signaling object by reference
//         
// Output - -1 indicating bad state
//          called if start called with signaling object in wrong state
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::Start(AMrcpSignaling& a_signaling)
{
	Name("Start");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingStarting 
// Input - signaling object by reference
//         
// Output - -1 signaling bad state
//      called if onsignalingstarted called with signaling object in wrong state    
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::OnSignalingStarted(AMrcpSignaling& a_signaling)
{
	Name("OnSignalingStarted");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Connect 
// Input - signaling object by reference
//         
// Output - -1 signaling bad state
//         called if connect called with signaling object in wrong state
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::Connect(AMrcpSignaling& a_signaling)
{
	Name("Connect");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingConnected 
// Input - signaling object by reference
//         
// Output - -1 signaling bad state
//       called if onSignalingConnected called with signaling object in wrong state   
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::OnSignalingConnected(AMrcpSignaling& a_signaling)
{
	Name("OnSignalingConnected");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingAvailable 
// Input - signaling object
//         
// Output - -1 signaling bad state
//         called if onsignalingavailable called with signaling object in wrong state 
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::OnSignalingAvailable(AMrcpSignaling& a_signaling)
{
	Name("OnSignalingAvailable");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SendCommand 
// Input - signaling object
//         
// Output - -1 signaling bad state
//       called if SendCommand called with signaling object in wrong state   
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::SendCommand(AMrcpSignaling& a_signaling)
{
	Name("SendCommand");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Reinvite 
// Input - signaling object
//         resource type
// Output - -1 signaling bad state
//          called if reinvite called with signaling object in wrong state
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::Reinvite(AMrcpSignaling& a_signaling,MrcpResourceType a_resourceType)
{
	Name("AMrcpSignalingState::Reinvite");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SignalingDisconnect 
// Input - signaling object
//         
// Output - -1 signaling bad state
//          called if signalingdisconnect called with signaling object in wrong state
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::SignalingDisconnect(AMrcpSignaling& a_signaling)
{
	Name("SignalingDisconnect");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnSignalingDisconnected 
// Input - signaling object
//         
// Output - -1 signaling bad state
//      called if onsignalingdisconnected called with signaling object in wrong state    
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::OnSignalingDisconnected(AMrcpSignaling& a_signaling)
{
	Name("OnSignalingDisconnect");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Stop 
// Input - signaling object
//         
// Output - -1 signaling bad state
//        called if signalingstop called with signaling object in wrong state  
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::SignalingStop(AMrcpSignaling& a_signaling)
{
	Name("SignalingStop");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - SginalingAck 
// Input - signaling object
//         
// Output - -1 signaling bad state
//        called if signalingAck called with signaling object in wrong state  
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::SignalingAck(AMrcpSignaling& a_signaling)
{
	Name("SignalingAck");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state ");
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - OnStopped 
// Input - signaling object
//         
// Output - -1 indicating a bad state
//        called if onsignalingstopped called with signaling object in wrong state  
///////////////////////////////////////////////////////////////////////////

int AMrcpSignalingState::OnSignalingStopped(AMrcpSignaling& a_signaling)
{
	Name("OnSignalingStopped");
	CLogger::Instance()->Log( LOG_LEVEL_WARNING, *this, "Bad state - ");
	return -1;
}


} //end namespace
