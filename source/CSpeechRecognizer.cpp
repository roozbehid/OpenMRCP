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
//	File Name:		CSpeechRecognizer.cpp
//
//	Description:	CSpeechRecognizer class implementation
//				 Implements the primitives for the recognizer commands
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CSpeechRecognizer::CSpeechRecognizer(RESOURCE_CFG_STRUCT* a_resourceCfg, AMrcpSignaling* a_signalObj);
//		CSpeechRecognizer::~CSpeechRecognizer();
//		int CSpeechRecognizer::PrimDefineGrammar(CRecDefineGrammar& a_task);
//		int CSpeechRecognizer::PrimRecognize(CRecRecognize& a_task);
//		int CSpeechRecognizer::PrimInterpret(CRecInterpret& a_task);
//		int CSpeechRecognizer::PrimStartInputTimers(CRecStartInputTimers& a_task);
//		int CSpeechRecognizer::PrimGetResult(CRecGetResult& a_task);
//		int CSpeechRecognizer::PrimStopRec(CRecStopRec& a_task);
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
///////////////////////////////////////////////////////////////////////////////

#include "CSpeechRecognizer.h"
#include "AMrcpSignaling.h"
#include "ClientInterfaceDefs.h"
#include "MrcpUtils.h"
#include "CMrcpCmdIdMgr.h"
#include "AMrcpSignaling.h"
#include "CRecognitionCommands.h"
#include "CMrcpTaskProcessor.h"
#include "CMrcpSendCommand.h"
#include "MrcpClientEvents.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - resource config struct
//         pointer to signaling object
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpeechRecognizer::CSpeechRecognizer(RESOURCE_CFG_STRUCT* a_resourceCfg,AMrcpSignaling* a_signalObj)
	: m_resourceCfg (a_resourceCfg)
	, m_resourceType (speechrecog)
	, m_mrcpVersion(m_resourceCfg->asrServerConfig->mrcpVersion)
{
	m_signalObj = a_signalObj;
	BasicInitialization();

	m_cmdIdMgr = MrcpV2RefLib::CMrcpCmdIdMgr::Instance();
	m_recognizerCmds = new CRecognitionCommands(m_mrcpVersion);

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpeechRecognizer::~CSpeechRecognizer()
{
	delete m_recognizerCmds;
	NotifyObservers(RECOGNIZER_EXITED);
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimDefineGrammar
// Input - None
//         
// Output - None
//    code that is called as task processor calls request from queue                    
///////////////////////////////////////////////////////////////////////////
int CSpeechRecognizer::PrimDefineGrammar(CRecDefineGrammar& a_task)
{
	std::string l_src;

	if(m_mrcpVersion == 1)
		l_src = m_recognizerCmds->define_grammar1;
	else
		l_src = m_recognizerCmds->define_grammar2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj->ComRecogChannelID() + l_src.substr (l_channelId + 3);

	int l_contType = l_src.find("(4)");
	l_src = l_src.substr(0,l_contType) + a_task.m_contentType + l_src.substr (l_contType + 3);

	int l_gramLen = l_src.find("(5)");
	int l_GrammarLen = sizeof (a_task.m_gramContent);  //= 22
	l_src = l_src.substr (0,l_gramLen) + MrcpUtils::itos(l_GrammarLen) + l_src.substr(l_gramLen +3);

	int l_contId = l_src.find("(6)");
	l_src = l_src.substr(0,l_contId) + a_task.m_contentID + l_src.substr(l_contId + 3);//increase +3 to +4 to gen 2 blanks


	int l_grammarLen = l_src.find("(7)");
	l_src = l_src.substr (0,l_grammarLen) + a_task.m_gramContent;

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
// Description - PrimRecognize
// Input - recognize object
//         
// Output - Success or failure
//     code that is called as task processor calls request from queue                     
///////////////////////////////////////////////////////////////////////////
int CSpeechRecognizer::PrimRecognize(CRecRecognize& a_task)
{
	std::string l_src;
	std::string l_cancelStr;
	std::string l_startTimers;
	if(m_mrcpVersion == 1)
		l_src = m_recognizerCmds->recognize_1;
	else
		l_src = m_recognizerCmds->recognize_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_cancelQueue = l_src.find("(3)");
	if (l_cancelQueue)
	{
		if (a_task.m_cancelQueue == true)
			l_cancelStr = "true";
		else
			l_cancelStr = "false";
		l_src = l_src.substr(0,l_cancelQueue) + l_cancelStr + l_src.substr (l_cancelQueue + 3);
	}
	int l_channelId = l_src.find("(4)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj ->ComRecogChannelID() + l_src.substr (l_channelId + 3);

	int l_contType = l_src.find("(5)");
	l_src = l_src.substr(0,l_contType) + a_task.m_contentType + l_src.substr (l_contType + 3);

	int l_startTimer = l_src.find("(6)");
	if (a_task.m_startInputTimers)
		l_startTimers = "true";
	else
		l_startTimers = "false";
	l_src = l_src.substr(0,l_startTimer) + l_startTimers + l_src.substr(l_startTimer + 3);

	int l_gramLen = l_src.find("(7)");
	int l_GrammarLen = a_task.m_gramContent.size();
	l_src = l_src.substr (0,l_gramLen) + MrcpUtils::itos(l_GrammarLen) + l_src.substr(l_gramLen +3);

	int l_grammarLen = l_src.find("(8)");
	l_src = l_src.substr (0,l_grammarLen) + a_task.m_gramContent;

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
// Description - PrimInterpret
// Input - interpret object by reference
//         
// Output - success or failure
//    code that is called as task processor calls request from queue                        
///////////////////////////////////////////////////////////////////////////
int CSpeechRecognizer::PrimInterpret(CRecInterpret& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //will log here - not supported V1
	else
		l_src = m_recognizerCmds->interpret_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj ->ComRecogChannelID() + l_src.substr (l_channelId + 3);

	int l_interpretText = l_src.find("(4)");
	l_src = l_src.substr(0,l_interpretText) + a_task.m_interpretText + l_src.substr (l_interpretText + 3);

	int l_contentType = l_src.find("(5)");
	l_src = l_src.substr(0,l_contentType) + a_task.m_contentType + l_src.substr (l_contentType + 3);

	int l_contId = l_src.find("(6)");
	l_src = l_src.substr(0,l_contId) + a_task.m_contentID + l_src.substr(l_contId + 3);//increase +3 to +4 to gen 2 blanks

	int l_contLen = l_src.find("(7)");
	int l_contentLen = sizeof (a_task.m_content);  //22;
	l_src = l_src.substr (0,l_contLen) + MrcpUtils::itos(l_contentLen) + l_src.substr(l_contLen +3);

	int l_content = l_src.find("(8)");
	l_src = l_src.substr (0,l_content) + a_task.m_content;

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
// Description - PrimGetResult
// Input - Get Result object by referemce
//         
// Output - success or failure
//     code that is called as task processor calls request from queue                       
///////////////////////////////////////////////////////////////////////////
int CSpeechRecognizer::PrimGetResult(CRecGetResult& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //will log here - not supported V1
	else
		l_src = m_recognizerCmds->getResult_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj ->ComRecogChannelID() + l_src.substr (l_channelId + 3);

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
// Description - PrimStartInputTimers
// Input - start input timers by reference
//         
// Output - Success or failure
//     code that is called as task processor calls request from queue                     
///////////////////////////////////////////////////////////////////////////
int CSpeechRecognizer::PrimStartInputTimers(CRecStartInputTimers& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //to be implemented supported V1
	else
		l_src = m_recognizerCmds->startInputTimers_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj ->ComRecogChannelID() + l_src.substr (l_channelId + 3);
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
// Description - PrimStopRec
// Input - Stop Rec object by reference
//         
// Output - Success or failure
//   code that is called as task processor calls request from queue                      
///////////////////////////////////////////////////////////////////////////
int CSpeechRecognizer::PrimStopRec(CRecStopRec& a_task)
{
	std::string l_src;
	if(m_mrcpVersion == 1)
		return -1; //to be implemented - supported V1
	else
		l_src = m_recognizerCmds->stopRec_2;

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(4)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + m_signalObj ->ComRecogChannelID() + l_src.substr (l_channelId + 3);

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