
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
//	File Name:		MrcpTasks.h
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
//
///////////////////////////////////////////////////////////////////////////////
#ifndef MrcpTasks_h
#define MrcpTasks_h

#include "ClientInterfaceDefs.h"
#include "AMrcpSignaling.h"
#include <string>

namespace MrcpV2RefLib
{
	class AMrcpSignaling;
/////////
	class MrcpTasks
	{
	   friend class CMrcpTaskProcessor;
	   friend class CSpeechRecognizer;

	public:
	   MrcpTasks(AMrcpSignaling* a_signalObj);
	   virtual ~MrcpTasks();
	   MrcpSessionHandle TtsSessionHandle() {return m_signalObj->TtsSessionHandle();};
	   MrcpSessionHandle AsrSessionHandle() {return m_signalObj->AsrSessionHandle();};


	protected:
		MrcpTasks(){}; // no implementation
	   AMrcpSignaling* m_signalObj;

	private:
	   virtual void Execute() = 0;
	};
//////////

	class CSignalingStartCommand : public MrcpTasks
	{
	public:
	   CSignalingStartCommand( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingStartCommand();
	   void Execute();

	private:
	   CSignalingStartCommand(); // no implementation
	};

//////////
	class CSignalingConnectCommand : public MrcpTasks
	{
	public:
	   CSignalingConnectCommand( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingConnectCommand();
	   void Execute();

	private:
	   CSignalingConnectCommand(); // no implementation
	};
//////////
	class CSignalingReinviteCommand : public MrcpTasks
	{
	public:
	   CSignalingReinviteCommand( AMrcpSignaling* a_signalObj,MrcpResourceType a_resourceToAdd);
	   virtual ~CSignalingReinviteCommand();
	   void Execute();
	private:
	   MrcpResourceType m_resourceToAdd;
	   CSignalingReinviteCommand(); // no implementation
	};
//////////////

	class CSignalingOnStartedEvent : public MrcpTasks
	{
	public:
	   CSignalingOnStartedEvent( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingOnStartedEvent();
	   void Execute();

	private:
	   CSignalingOnStartedEvent(); // no implementation
	};
	//////////////

	class CSignalingOnFailedEvent : public MrcpTasks
	{
	public:
	   CSignalingOnFailedEvent( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingOnFailedEvent();
	   void Execute();

	private:
	   CSignalingOnFailedEvent(); // no implementation
	};
/////////
	class CSignalingOnConnectedEvent : public MrcpTasks
	{
	public:
	   CSignalingOnConnectedEvent( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingOnConnectedEvent();
	   void Execute();

	private:
	   CSignalingOnConnectedEvent(); // no implementation
	};
/////////
	class CSignalingOnAvailableEvent : public MrcpTasks
	{
	public:
	   CSignalingOnAvailableEvent( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingOnAvailableEvent();
	   void Execute();

	private:
	   CSignalingOnAvailableEvent(); // no implementation
	};


//////////
	class CSignalingStopCommand : public MrcpTasks
	{
	public:
	   CSignalingStopCommand( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingStopCommand();
	   void Execute();

	private:
	   CSignalingStopCommand(); // no implementation
	};
/////////
	class CSignalingAckCommand : public MrcpTasks
	{
	public:
	   CSignalingAckCommand( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingAckCommand();
	   void Execute();
	private:
	   CSignalingAckCommand(); // no implementation
	   std::string m_messageBuffer;
	};
//////////

	class CSignalingOnStoppedEvent : public MrcpTasks
	{
	public:
	   CSignalingOnStoppedEvent( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingOnStoppedEvent();
	   void Execute();

	private:
	   CSignalingOnStoppedEvent(); // no implementation
	};
//////////

	class CSignalingDisconnectCommand : public MrcpTasks
	{
	public:
	   CSignalingDisconnectCommand( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingDisconnectCommand();
	   void Execute();

	private:
	   CSignalingDisconnectCommand(); // no implementation
	};
//////////

	class CSignalingOnDisconnectedEvent : public MrcpTasks
	{
	public:
	   CSignalingOnDisconnectedEvent( AMrcpSignaling* a_signalObj);
	   virtual ~CSignalingOnDisconnectedEvent();
	   void Execute();

	private:
	   CSignalingOnDisconnectedEvent(); // no implementation
	};
	class CMrcpProcessCommandResults : public MrcpTasks
	{
	public:       
		CMrcpProcessCommandResults(AMrcpSignaling* a_signalObj);
		virtual ~CMrcpProcessCommandResults();

	   void Execute();
	   void MessageBuffer(const std::string& a_messageBuffer) { m_messageBuffer = a_messageBuffer;}
	   const std::string& MessageBuffer() { return m_messageBuffer; }

	private:
		std::string m_messageBuffer;
	};

}//end namespace
#endif

