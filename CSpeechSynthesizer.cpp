
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
//	File Name:		CSpeechSynthesizer.cpp
//
//	Description:	CSpeechSynthesizer class implementation
//				 Implements the primitives for the synthesizer commands
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CSpeechSynthesizer::CSpeechSynthesizer(RESOURCE_CFG_STRUCT* a_resourceCfg, AMrcpSignaling* a_signalObj);
//		CSpeechSynthesizer::~CSpeechSynthesizer();
//		int CSpeechSynthesizer::PrimSpeak(CSpkSpeak& a_task);
//		int CSpeechSynthesizer::PrimPause(CSpkPause& a_task);
//		int CSpeechSynthesizer::PrimResume(CSpkResume& a_task);
//		int CSpeechSynthesizer::PrimBargeIn(CSpkBargeIn& a_task);
//		int CSpeechSynthesizer::PrimStopSpk(CSpkStop& a_task);

///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
///////////////////////////////////////////////////////////////////////////////

#include "CSpeechSynthesizer.h"
#include "CSpeakCommands.h"
#include "CMrcpCmdIdMgr.h"
#include "AMrcpSignaling.h"
#include "CMrcpTaskProcessor.h"
#include "CMrcpSendCommand.h"
#include "MrcpClientEvents.h"
#include "CMrcpEvent.h"

#include "MrcpUtils.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - Resource COnfig Struct
//         singaling object pointer
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpeechSynthesizer::CSpeechSynthesizer(RESOURCE_CFG_STRUCT* a_resourceCfg,AMrcpSignaling* a_signalObj)
	: m_resourceCfg (a_resourceCfg)
	, m_resourceType (speechsynth)
	, m_mrcpVersion (m_resourceCfg->ttsServerConfig->mrcpVersion)

{
	m_signalObj = a_signalObj;
	BasicInitialization();
	m_cmdIdMgr = MrcpV2RefLib::CMrcpCmdIdMgr::Instance();
	m_speakCmds = new CSpeakCommands(m_mrcpVersion);

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpeechSynthesizer::~CSpeechSynthesizer()
{
	delete m_speakCmds;
	NotifyObservers(SYNTHESIZER_EXITED);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimSpeakk
// Input - speak task object
//         
// Output - sucess or failure
//   code that is called as task processor calls request from queue                                          
///////////////////////////////////////////////////////////////////////////
int CSpeechSynthesizer::PrimSpeak(CSpkSpeak& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //to be implemented - supported V1
	else
		l_src = m_speakCmds->Speak_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	l_src = l_src.substr(0,l_channelId) + m_signalObj->ComSynthChannelID() + l_src.substr (l_channelId + 3);

	int l_bargeIn = l_src.find("(4)");
	if (strcmp(a_task.m_killOnBargeIn.c_str(), "\n"))
		l_src = l_src.substr(0,l_bargeIn) +"Kill-On-Barge-In: "+ a_task.m_killOnBargeIn + MRCP_ENDL + l_src.substr(l_bargeIn + 3);
	else
		l_src.erase(l_bargeIn, 3);
	
	int l_voiceName = l_src.find("(5)");
	if (strcmp(a_task.m_voiceName.c_str(), "\n"))
		l_src = l_src.substr(0,l_voiceName) +"Voice-Name: " + a_task.m_voiceName + MRCP_ENDL + l_src.substr(l_voiceName + 3);
	else
		l_src.erase(l_voiceName, 3);

	int l_speechLanguage = l_src.find("(6)");
	if (strcmp(a_task.m_speechLanguage.c_str(), "\n"))
		l_src = l_src.substr(0,l_speechLanguage) +"Speech-Language: "+ a_task.m_speechLanguage + MRCP_ENDL + l_src.substr(l_speechLanguage + 3);
	else
		l_src.erase(l_speechLanguage, 3);

	int l_contType = l_src.find("(7)");
	l_src = l_src.substr(0,l_contType) + a_task.m_contentType + l_src.substr (l_contType + 3);

	int l_contLen = l_src.find("(8)");
	l_src = l_src.substr(0,l_contLen) + MrcpUtils::itos(a_task.m_content.size()) + l_src.substr(l_contLen + 3);

	int l_contLength = l_src.find("(9)");
	l_src = l_src.substr(0,l_contLength) + a_task.m_content + l_src.substr(l_contLength + 3);

	//the following goes last
	int l_sequence1 = l_src.find ("(1)");
	if (l_sequence1)
	{
		int l_packetLength = l_src.size();
		l_src = l_src.substr(0,l_sequence1) + MrcpUtils::itos(l_packetLength) + l_src.substr (l_sequence1 + 3);
	}

	if(m_signalObj)
	{
		CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(m_signalObj);
		l_sendCommand->MessageBuffer(l_src);
		CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);
		return MrcpSuccess;
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimPause
// Input - speak pause command object 
//         
// Output - success or failure
//   code that is called as task processor calls request from queue                                      
///////////////////////////////////////////////////////////////////////////
int CSpeechSynthesizer::PrimPause(CSpkPause& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //to be implemented - supported V1
	else
		l_src = m_speakCmds->Pause_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj->ComSynthChannelID() + l_src.substr (l_channelId + 3);

	//the following goes last
	int l_sequence1 = l_src.find ("(1)");
	if (l_sequence1)
	{
		int l_packetLength = l_src.size() - 1;
		l_src = l_src.substr(0,l_sequence1) + MrcpUtils::itos(l_packetLength) + l_src.substr (l_sequence1 + 3);
	}

	if(m_signalObj)
	{
		CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(m_signalObj);
		l_sendCommand->MessageBuffer(l_src);
		CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);
		return MrcpSuccess;
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimResume
// Input - speak resume object
//         
// Output - success or failure
//   code that is called as task processor calls request from queue                                      
///////////////////////////////////////////////////////////////////////////
int CSpeechSynthesizer::PrimResume(CSpkResume& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //to be implemented - supported V1
	else
		l_src = m_speakCmds->Resume_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj->ComSynthChannelID() + l_src.substr (l_channelId + 3);

	//the following goes last
	int l_sequence1 = l_src.find ("(1)");
	if (l_sequence1)
	{
		int l_packetLength = l_src.size()- 1;
		l_src = l_src.substr(0,l_sequence1) + MrcpUtils::itos(l_packetLength) + l_src.substr (l_sequence1 + 3);
	}

	if(m_signalObj)
	{
		CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(m_signalObj);
		l_sendCommand->MessageBuffer(l_src);
		CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);
		return MrcpSuccess;
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimBargeIn
// Input - barge in task object
//         
// Output - success or failure
//   code that is called as task processor calls request from queue                                       
///////////////////////////////////////////////////////////////////////////
int CSpeechSynthesizer::PrimBargeIn(CSpkBargeIn& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //to be implemented - supported V1
	else
		l_src = m_speakCmds->BargeIn_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);



	int l_channelId = l_src.find("(4)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj->ComSynthChannelID() + l_src.substr (l_channelId + 3);

	//the following goes last
	int l_sequence1 = l_src.find ("(1)");
	if (l_sequence1)
	{
		int l_packetLength = l_src.size() - 1;
		l_src = l_src.substr(0,l_sequence1) + MrcpUtils::itos(l_packetLength) + l_src.substr (l_sequence1 + 3);
	}

	if(m_signalObj)
	{
		CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(m_signalObj);
		l_sendCommand->MessageBuffer(l_src);
		CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);
		return MrcpSuccess;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimStopSpk
// Input - speak stop task object
//         
// Output - success or failure
//   code that is called as task processor calls request from queue                                          
///////////////////////////////////////////////////////////////////////////
int CSpeechSynthesizer::PrimStopSpk(CSpkStop& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //to be implemented - supported V1
	else
		l_src = m_speakCmds->Stop_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(4)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj->ComSynthChannelID() + l_src.substr (l_channelId + 3);

	//the following goes last
	int l_sequence1 = l_src.find ("(1)");
	if (l_sequence1)
	{
		int l_packetLength = l_src.size()- 1;
		l_src = l_src.substr(0,l_sequence1) + MrcpUtils::itos(l_packetLength) + l_src.substr (l_sequence1 + 3);
	}

	if(m_signalObj)
	{
		CMrcpSendCommand* l_sendCommand = new CMrcpSendCommand(m_signalObj);
		l_sendCommand->MessageBuffer(l_src);
		CMrcpTaskProcessor::Instance()->QueueTask( l_sendCommand);
		return MrcpSuccess;
	}

	return -1;
}

}//end namespace