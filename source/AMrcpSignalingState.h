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
//	File Name:		AMrcpSignalingState.h
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
#ifndef AMrcpSignalingState_h
#define AMrcpSignalingState_h

#include "CLogger.h"
#include "ClientInterfaceDefs.h"

namespace MrcpV2RefLib
{
   class AMrcpSignaling;

	class AMrcpSignalingState  
	{
	public:
		AMrcpSignalingState();
		virtual ~AMrcpSignalingState();
	    virtual int Connect( AMrcpSignaling& a_signaling);
		virtual int Start( AMrcpSignaling& a_signaling);
		virtual int OnSignalingStarted( AMrcpSignaling& a_signaling);
		virtual int OnSignalingConnected( AMrcpSignaling& a_signaling);
		virtual int OnSignalingAvailable( AMrcpSignaling& a_signaling);
		virtual int SendCommand( AMrcpSignaling& a_signaling);
		virtual int Reinvite( AMrcpSignaling& a_signaling,MrcpResourceType a_resourceType);
        virtual int SignalingStop( AMrcpSignaling& a_signaling);
		virtual int SignalingAck( AMrcpSignaling& a_signaling);
        virtual int OnSignalingStopped( AMrcpSignaling& a_signaling);
		virtual int SignalingDisconnect( AMrcpSignaling& a_signaling );
		virtual int OnSignalingDisconnected( AMrcpSignaling& a_signaling );
		virtual char* Description(){return "N/A";};
	
		const char* StateDesc() { return "N/A";}
		const std::string& ClassName(){return m_className;};
		void ClassName( const std::string& a_className){m_className = a_className;};
		const std::string& Name(){return m_name;};
		void Name( const std::string& a_name){m_name = a_name;};
		LogObjectTypeEnum LogObjectType(){return LogObject_System;};

	protected:

	private:
	   std::string m_name;
	   std::string m_className;

	};
} //end namespace

#endif