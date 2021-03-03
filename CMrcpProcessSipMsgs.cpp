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
//	File Name:		CMrcpProcessSipMsgs.cpp
//
//	Description:	CMrcpProcessSipMsgs class implementation
//					Process the SIP responses received
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CSipProcessMessage::CSipProcessMessage
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  8/23/06     CMB         String optimization and cleanup
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpProcessSipMsgs.h"
#include "CMrcpSipSignal.h"
#include "CMrcpSendCommand.h"
#include "CMrcpTaskProcessor.h"
#include "MrcpUtils.h"
#include "AMrcpSignalingState.h"
#include <time.h>

#include <iostream>

namespace MrcpV2RefLib
{


////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - pointer to signaling object
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

CSipProcessMessage::CSipProcessMessage(AMrcpSignaling* a_signalObj)
		: MrcpTasks( a_signalObj)
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
CSipProcessMessage::~CSipProcessMessage()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute
// Input - None
//         
// Output - None
//       
///////////////////////////////////////////////////////////////////////////
void CSipProcessMessage::Execute()
{
	int l_callStat = 0;
	int l_commandPort = 0;
	int l_audioPort = 0;
	std::string l_tempChannel;
	std::string l_toAddressValue;
	std::string l_toCommandPort;
	std::string l_toAudioPort;
	std::string l_toChannelId;
	std::string l_toChannelId2;
	std::string::size_type l_cmdPos = std::string::npos;
	std::string::size_type l_pos3 = std::string::npos;
	std::string::size_type l_okPos = std::string::npos;

		std::string::size_type l_statPos = m_messageBuffer.find("SIP");
		if (l_statPos != std::string::npos) 
		{
			std::string l_statVal = (m_messageBuffer.substr((l_statPos + 8), 3));
			l_callStat = atoi(l_statVal.c_str());
			l_cmdPos = m_messageBuffer.find("BYE");
			if ((200 == l_callStat)||(l_cmdPos != std::string::npos))
			{	
				l_okPos = m_messageBuffer.find("OK");
				if (l_cmdPos != std::string::npos )//&& l_okPos == std::string::npos)
				{
					CSignalingOnDisconnectedEvent* l_disconnectedEvent = new CSignalingOnDisconnectedEvent(m_signalObj);
					CMrcpTaskProcessor::Instance()->QueueTask(l_disconnectedEvent);
					MrcpUtils::MsSleep(1000);

					CSignalingStopCommand* l_stopCommand = new CSignalingStopCommand(m_signalObj);
					CMrcpTaskProcessor::Instance()->QueueTask(l_stopCommand);
				}

			}
			if ((l_callStat > 199 && l_callStat < 300) && l_cmdPos == std::string::npos) 
			{  //process continues for success only
				std::string::size_type l_pos1 = m_messageBuffer.find("To:");
				if (l_pos1 != std::string::npos)
				{
					std::string::size_type l_pos2 = m_messageBuffer.find(M_ENDL, l_pos1);
					l_toAddressValue.assign(m_messageBuffer.substr(l_pos1, (l_pos2 - l_pos1)));
					l_toAddressValue.append(M_ENDL);
				}

				std::string::size_type l_cmdPortPos = m_messageBuffer.find("m=application", 0);
				if (l_cmdPortPos != std::string::npos)
				{
					l_cmdPortPos += 13;  //add the size of the search string m=application to adjust pointer
					std::string::size_type l_pos2 = m_messageBuffer.find("TCP/MRCP");
					l_toCommandPort.assign(m_messageBuffer.substr(l_cmdPortPos, (l_pos2 - l_cmdPortPos)));
					l_commandPort = atoi(l_toCommandPort.c_str());
					m_signalObj->SetupCommandProcessing(l_commandPort);
				}
				std::string::size_type l_audioPortPos = m_messageBuffer.find("m=audio", 0);
				if (l_cmdPortPos != std::string::npos)
				{
					l_audioPortPos += 7;  //add the size of the search string m=application to adjust pointer
					std::string::size_type l_pos3 = m_messageBuffer.find("RTP/AVP");
					l_toAudioPort.assign(m_messageBuffer.substr(l_audioPortPos, (l_pos3 - l_audioPortPos)));
					l_audioPort = atoi(l_toAudioPort.c_str());
					m_signalObj->ServerRtpPort(l_audioPort);
				}

				std::string::size_type l_chanIdPos = m_messageBuffer.find("a=channel:", 0);
				if (l_chanIdPos != std::string::npos)
				{
					l_chanIdPos +=10;
					l_pos3 = m_messageBuffer.find(M_ENDL, l_chanIdPos);
					l_tempChannel.assign(m_messageBuffer,l_chanIdPos, (l_pos3 - l_chanIdPos));
					std::string::size_type l_pos4 = l_tempChannel.find("speechrecog");
					std::string::size_type l_pos5 = l_tempChannel.find("speechsynth");
					l_toChannelId.assign(m_messageBuffer.substr(l_chanIdPos, (l_pos3 - l_chanIdPos)));
					if (l_pos4 != std::string::npos)
						m_signalObj->ComRecogChannelID(l_toChannelId);
					else
					if (l_pos5 != std::string::npos)
						m_signalObj->ComSynthChannelID(l_toChannelId);
				}
			    l_tempChannel.erase();
				std::string::size_type l_chanIdPos2 = m_messageBuffer.find("a=channel:", l_pos3);
				if (l_chanIdPos2 != std::string::npos)
				{
					l_chanIdPos2 +=10;
					std::string::size_type l_pos6 = m_messageBuffer.find(M_ENDL, l_chanIdPos2);
					l_tempChannel.assign(m_messageBuffer, l_chanIdPos2, (l_pos6 - l_chanIdPos2));
					std::string::size_type l_pos7 = l_tempChannel.find("speechsynth");
					std::string::size_type l_pos8 = l_tempChannel.find("speechrecog");
					l_toChannelId2.assign(m_messageBuffer.substr(l_chanIdPos2, (l_pos6 - l_chanIdPos2)));
					if (l_pos7 != std::string::npos)
						m_signalObj->ComSynthChannelID(l_toChannelId2);
					else
					if (l_pos8 != std::string::npos)
						m_signalObj->ComRecogChannelID(l_toChannelId2);

				}
					
				if(l_cmdPos == std::string::npos)
				{
					CSignalingAckCommand* l_signalingAckMessage = new CSignalingAckCommand( m_signalObj);
					m_signalObj->SetAckToValue(l_toAddressValue);
					CMrcpTaskProcessor::Instance()->QueueTask(l_signalingAckMessage);

					CSignalingOnConnectedEvent* l_onConnectededTask = new CSignalingOnConnectedEvent(m_signalObj);	
					CMrcpTaskProcessor::Instance()->QueueTask( l_onConnectededTask);

				}
			}
		} 
}

}//end namespace



