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
//	File Name:		CMrcpRtspSignal.h
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
#ifndef CMrcpRtspSignal_h
#define CMrcpRtspSignal_h

#include <winsock2.h>

#include "AMrcpSignaling.h"
#include <string>

#define MRCP_ENDL "\r\n" 

namespace MrcpV2RefLib
{
	class CMrcpSession;
	class CMrcpCmdIdMgr;
	class CMrcpRtspSignal : public AMrcpSignaling
	{
	public:
		CMrcpRtspSignal(CMrcpSession* a_session);

		virtual ~CMrcpRtspSignal();

	protected:
		MrcpResourceType m_resourceType;

		std::string setup_rtsp_hdr; 

	private:
		boost::mutex m_controlMutex;


	   int PrimStart();
	   int PrimConnect();
	   int PrimReinvite(MrcpResourceType a_resourceType){return 0;};
	   int PrimOnSignalingStarted();
	   int PrimSendCommand(const std::string& a_message);
	   int PrimProcessResults(const std::string& a_message);
	   int PrimStop();
	   int PrimSendAck();
	   int PrimOnStopped();
	   int PrimDisconnect();
	   void SetAckToValue(std::string a_value){std::string m_foo = a_value;};

		CMrcpSession* m_session;
		CMrcpCmdIdMgr* m_cmdIdMgr;
	    MrcpCallBackFunction m_defaultCallBack;

	   int SetupCommandProcessing(int a_commandSocket);
	   bool m_asrResource;
	   bool m_ttsResource;


	};

}//end namespace
#endif