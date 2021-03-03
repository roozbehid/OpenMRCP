
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
//	File Name:		AMrcpSignaling.h
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
#ifndef AMrcpSignaling_h
#define AMrcpSignaling_h

#include "AMrcpObserverSubject.h"
#include "ClientInterfaceDefs.h"
#include "CMrcpSignalingStates.h"

#include <string>
#include <winsock2.h>

namespace MrcpV2RefLib
{
	class AMrcpSignaling : public AMrcpObserverSubject
	{
	   friend class AMrcpSignalingState;
	   friend class CMrcpSignalingUninitializedState;
	   friend class CMrcpSignalingStartingState;
	   friend class CMrcpSignalingStartedState;  
	   friend class CMrcpSignalingConnectingState;
	   friend class CMrcpSignalingConnectedState;
	   friend class CMrcpSignalingAvailableState;
	   friend class CMrcpSignalingStoppingState;
	   friend class CMrcpSignalingStoppedState;
	   friend class CMrcpSignalingDisconnectingState;
	   friend class CMrcpSignalingDisconnectedState;

	   friend class CSignalingStartCommand;
	   friend class CSignalingConnectCommand;
	   friend class CSignalingOnStartedEvent;
	   friend class CSignalingOnFailedEvent;
	   friend class CSignalingOnAvailableEvent;
	   friend class CSignalingOnConnectedEvent;
	   friend class CSignalingStopCommand;
	   friend class CSignalingAckCommand;
	   friend class CSignalingDisconnectCommand;
	   friend class CSignalingOnStoppedEvent;
	   friend class CSignalingOnDisconnectedEvent;
	   friend class CSipInviteCommand;
	   friend class CSipReinviteCommand;
	   friend class CSignalingReinviteCommand;
	   friend class CSipProcessMessage;
	   friend class CSipAckResponseMessage;
	   friend class CSipByeMessage;
	   friend class CMrcpSipSignaling;
	   friend class CMrcpSendCommand;
	   friend class CMrcpProcessCommandResults;
	   friend class CSpeechRecognizer;
	   friend class CSpeechSynthesizer;
	   friend class CMrcpSipSignal;
	   friend class CMrcpRtspSignal;
	   friend class CMrcpGeneric;
	   friend class CMrcpSession;
	   friend class MrcpTasks;
	   friend class AMrcpResourceState;

	public:
		AMrcpSignaling();
		virtual ~AMrcpSignaling();
		void BasicInitialization();

	protected:
		AMrcpSignalingState* m_state;
	   AMrcpSignalingState* m_signalingUninitializedState;
	   AMrcpSignalingState* m_signalingStartingState;
	   AMrcpSignalingState* m_signalingStartedState;
	   AMrcpSignalingState* m_signalingConnectingState;
	   AMrcpSignalingState* m_signalingConnectedState;
	   AMrcpSignalingState* m_signalingAvailableState;
	   AMrcpSignalingState* m_signalingStoppingState;
	   AMrcpSignalingState* m_signalingStoppedState;
	   AMrcpSignalingState* m_signalingDisconnectingState;
	   AMrcpSignalingState* m_signalingDisconnectedState;

	   virtual int PrimConnect()   = 0;
	   virtual int PrimStart()     = 0;
	   virtual int PrimOnSignalingStarted() = 0;
	   virtual int PrimSendCommand(const std::string& a_message) = 0;
	   virtual int PrimProcessResults(const std::string& a_message) = 0;
	   virtual int PrimStop()      = 0;
	   virtual int PrimSendAck()   = 0;
	   virtual int PrimOnStopped() = 0;
	   virtual int PrimDisconnect() = 0;
	   virtual int PrimReinvite(MrcpResourceType a_resourceType) = 0;
	   virtual void SetAckToValue(std::string a_value) = 0;

	   RESOURCE_CFG_STRUCT m_cfgParameters;

	   SOCKET SetupSocket() { return m_mrcpSetupSocket;};
	   void SetupSocket(SOCKET a_mrcpSocket) { m_mrcpSetupSocket = a_mrcpSocket;};

	   SOCKET CommandSocket() { return m_mrcpCommandSocket;};
	   void CommandSocket(SOCKET a_mrcpSocket) {m_mrcpCommandSocket = a_mrcpSocket;};

	   SOCKET AsrSocket() { return m_asrSocket;};
	   void AsrSocket(SOCKET a_mrcpSocket) {m_asrSocket = a_mrcpSocket;};

	   SOCKET TtsSocket() { return m_ttsSocket;};
	   void TtsSocket(SOCKET a_mrcpSocket) {m_ttsSocket = a_mrcpSocket;};

	   MrcpSessionHandle TtsSessionHandle() {return m_ttsSessionHandle;};
	   void TtsSessionHandle(MrcpSessionHandle a_sessionHandle){m_ttsSessionHandle = a_sessionHandle;};

	   MrcpSessionHandle AsrSessionHandle() {return m_asrSessionHandle;};
	   void AsrSessionHandle(MrcpSessionHandle a_sessionHandle) {m_asrSessionHandle = a_sessionHandle;};

	   std::string ComRecogChannelID() { return m_recogChannelID; };
	   void ComRecogChannelID(std::string a_recogChannelID) { m_recogChannelID = a_recogChannelID; };

	   std::string ComSynthChannelID() { return m_synthChannelID; };
	   void ComSynthChannelID(std::string a_synthChannelID) { m_synthChannelID = a_synthChannelID; };


	   int ClientRtpPort() {return m_clientRtpPort;};
	   void ClientRtpPort(int a_clientRtpPort) {m_clientRtpPort = a_clientRtpPort;};

	   int ServerRtpPort() {return m_serverRtpPort;};
	   void ServerRtpPort(int a_serverRtpPort) {m_serverRtpPort = a_serverRtpPort;};

	   std::string ServerAddress() {return m_serverAddress;};
	   void ServerAddress(std::string a_serverAddress) {m_serverAddress = a_serverAddress;};

	   int ServerPort() {return m_serverPort;};
	   void ServerPort(int a_serverPort) {m_serverPort = a_serverPort;};

	   std::string ClientAddress() {return m_clientAddress;};
	   void ClientAddress(std::string a_clientAddress) {m_clientAddress = a_clientAddress;};


	private:
	   int SetState(AMrcpSignalingState* state);
	   int SetStateToStarting() { return SetState (m_signalingStartingState );};
	   int SetStateToStarted()	{ return SetState( m_signalingStartedState);};
	   int SetStateToConnecting() { return SetState( m_signalingConnectingState );};
	   int SetStateToConnected()  { return SetState( m_signalingConnectedState );};
	   int SetStateToAvailable() { return SetState( m_signalingAvailableState );};
	   int SetStateToStopping() { return SetState( m_signalingStoppingState );};
	   int SetStateToStopped() { return SetState( m_signalingStoppedState );};
	   int SetStateToDisconnecting() { return SetState( m_signalingDisconnectingState );};
	   int SetStateToDisconnected() { return SetState( m_signalingDisconnectedState ); };
	   int SetStateToUninitialized() { return SetState( m_signalingUninitializedState );};



	   int Start();
	   int Connect();
	   int Reinvite(MrcpResourceType a_resourceToAdd);
	   int OnSignalingStarted();
	   int OnSignalingFailed();
	   int OnSignalingConnected();
	   int OnSignalingAvailable();
	   int OnSignalingStopped();
	   int OnSignalingDisconnected();
	   int SendCommand();
	   int SignalingDisconnect();
	   int SignalingStop();
	   int SignalingAck();
	   int OnStopped();
	   virtual int SetupCommandProcessing(int a_commandSocket) = 0;


	    SOCKET m_mrcpSetupSocket;
		SOCKET m_mrcpCommandSocket;
		SOCKET m_asrSocket;
		SOCKET m_ttsSocket;

	   //config storage
	   std::string m_serverAddress;
	   int m_serverPort;
	   std::string m_clientAddress;
	   int m_clientRtpPort;
	   int m_serverRtpPort;
	   int m_cSeq;

	   MrcpSessionHandle m_ttsSessionHandle;
	   MrcpSessionHandle m_asrSessionHandle;
		std::string m_recogChannelID;
		std::string m_synthChannelID;

	};

} //end namespace
#endif
