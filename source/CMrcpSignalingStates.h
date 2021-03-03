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
//	File Name:		CMrcpSignalingStates.h
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

#ifndef CMrcpSignalingStates_h
#define CMrcpSignalingStates_h

#include "AMrcpSignalingState.h"
#include <string>

namespace MrcpV2RefLib
{
	class CMrcpSignalingUninitializedState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingUninitializedState();
		virtual ~CMrcpSignalingUninitializedState();

		// Accessors
		char* Description()
		{
			return "Uninitialized";
		};

		// Operations
		int Start(AMrcpSignaling& subject);
	};

//////
	class CMrcpSignalingStartingState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingStartingState();
		virtual ~CMrcpSignalingStartingState();

		// Accessors
		char* Description()
		{
			return "Starting";
		};

		// Operations
		int OnSignalingStarted(AMrcpSignaling& a_subject);
	};


//////
	class CMrcpSignalingStartedState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingStartedState();
		virtual ~CMrcpSignalingStartedState();

		// Accessors
		char* Description()
		{
			return "Started";
		};

		// Operations
		int Connect(AMrcpSignaling& a_subject);

		int OnSignalingConnecting(AMrcpSignaling& a_subject);
	};

	class CMrcpSignalingConnectingState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingConnectingState();
		virtual ~CMrcpSignalingConnectingState();

		// Accessors
		char* Description()
		{
			return "Connecting";
		};

		// Operations
		int OnSignalingConnected(AMrcpSignaling& a_subject);
		int SignalingAck(AMrcpSignaling& a_subject);
	};

/////////////////////
	class CMrcpSignalingConnectedState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingConnectedState();
		virtual ~CMrcpSignalingConnectedState();

		// Accessors
		char* Description()
		{
			return "Connected";
		};

		// Operations
		int OnSignalingAvailable(AMrcpSignaling& a_subject);
	};

/////
	class CMrcpSignalingAvailableState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingAvailableState();
		virtual ~CMrcpSignalingAvailableState();

		// Accessors
		char* Description()
		{
			return "Available";
		};

		// Operations
		int SendCommand(AMrcpSignaling& a_subject, const std::string& a_message );
		int Reinvite(AMrcpSignaling& subject, MrcpResourceType a_resourceType);
		int SignalingDisconnect(AMrcpSignaling& a_subject);
	};

//////////////
	class CMrcpSignalingDisconnectingState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingDisconnectingState();
		virtual ~CMrcpSignalingDisconnectingState();

		// Accessors
		char* Description()
		{
			return "Disconnecting";
		};

		// Operation&
		int OnSignalingDisconnected(AMrcpSignaling& a_subject);
	};
//////////////
	class CMrcpSignalingDisconnectedState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingDisconnectedState();
		virtual ~CMrcpSignalingDisconnectedState();

		// Accessors
		char* Description()
		{
			return "Disconnected";
		};

		// Operation&
		int SignalingStop(AMrcpSignaling& a_subject);
	};

//////////////
	class CMrcpSignalingStoppingState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingStoppingState();
		virtual ~CMrcpSignalingStoppingState();

		// Accessors
		char* Description()
		{
			return "Stopping";
		};

		// Operation&
		int OnSignalingStopped(AMrcpSignaling& a_subject);
	};
////////
	//////////////
	class CMrcpSignalingStoppedState : public AMrcpSignalingState
	{
	public:
		CMrcpSignalingStoppedState();
		virtual ~CMrcpSignalingStoppedState();

		// Accessors
		char* Description()
		{
			return "Stopped";
		};

		// Operation&
		int OnSignalingUnintialized(AMrcpSignaling& a_subject); 
	};

} //end namespace
#endif