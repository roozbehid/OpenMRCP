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
//	File Name:		CMrcpGeneric.cpp
//
//	Description:	CMrcpGeneric class implementation
//                Implements primitives for get and set parameters
//				  defined as generic commands in the MRCP SPecification
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpGeneric::CMrcpGeneric(RESOURCE_CFG_STRUCT* a_resourceCfg,AMrcpSignaling* a_signalObj)
//		CMrcpGeneric::~CMrcpGeneric()
//		int	CMrcpGeneric::PrimGetParams(CGetParams& a_task)
//		int	CMrcpGeneric::PrimSetParams(CSetParams& a_task)
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpGeneric.h"
#include "CGenericCommands.h"
#include "CMrcpCmdIdMgr.h"
#include "MrcpUtils.h"
#include "AMrcpSignaling.h"
#include "ClientInterfaceDefs.h"
#include "CMrcpSendCommand.h"
#include "CMrcpTaskProcessor.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - config structure
//         signaling object
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpGeneric::CMrcpGeneric(RESOURCE_CFG_STRUCT* a_resourceCfg,AMrcpSignaling* a_signalObj)
	: m_resourceCfg (a_resourceCfg)
{ 
	m_signalObj = a_signalObj;
	BasicInitialization();
	ClassName("CMrcpGeneric");
	m_cmdIdMgr = MrcpV2RefLib::CMrcpCmdIdMgr::Instance();
	m_genericCmds = new CGenericCommands(m_mrcpVersion);

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpGeneric::~CMrcpGeneric()
{
	delete m_genericCmds;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - PrimGetParams
// Input - task object
//         
// Output - Success or failure
//          
///////////////////////////////////////////////////////////////////////////
int	CMrcpGeneric::PrimGetParams(CGetParams& a_task)
{
	Name("CMrcpGeneric");
	std::string l_src;
	std::string l_params;
	std::string l_comChannelID;
	int i;

	if(a_task.m_version == 2)
		l_src = m_genericCmds->get_params2;
	else
	{
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Invalid version");
		return -1;
	}
	
	l_comChannelID = m_signalObj->ComSynthChannelID();
	if (l_comChannelID.empty())
		l_comChannelID = m_signalObj->ComRecogChannelID();


	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + l_comChannelID + l_src.substr (l_channelId + 3);

	if(a_task.m_parameters.numParams)
	{
		for (i=0; i < a_task.m_parameters.numParams; i++)
		{
			l_params = m_genericCmds->get_params_arguments2;
			int l_sequence = l_params.find("(1)");
			l_params = l_params.substr (0,l_sequence) + a_task.m_parameters.parameterName[i] + l_params.substr (l_sequence + 3);
			l_src += l_params;
			l_params.erase();
		}
		l_src += MRCP_ENDL;
	}

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
// Description - PrimSetParams
// Input - task object
//         
// Output - success or failure
//          
///////////////////////////////////////////////////////////////////////////
int	CMrcpGeneric::PrimSetParams(CSetParams& a_task)
{
	Name ("PrimSetParams");
	std::string l_src;
	std::string l_params;
	std::string l_comChannelID;
	int i;

	if(a_task.m_version == 2)
		l_src = m_genericCmds->set_params2;
	else
	{
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Invalid Version");
		return -1;
	}

	l_comChannelID = m_signalObj->ComSynthChannelID();
	if (l_comChannelID.empty())
		l_comChannelID = m_signalObj->ComRecogChannelID();

	int l_sequence2 = l_src.find("(2)");
	l_src = l_src.substr (0,l_sequence2) + MrcpUtils::itos (a_task.m_cmdID ) + l_src.substr (l_sequence2 + 3);

	int l_channelId = l_src.find("(3)");
	if (l_channelId)
		l_src = l_src.substr(0,l_channelId) + l_comChannelID + l_src.substr (l_channelId + 3);

	if(a_task.m_parameters.numParams)
	{
		for (i=0; i < a_task.m_parameters.numParams; i++)
		{
			l_params = m_genericCmds->set_params_arguments2;
			int l_sequence = l_params.find("(1)");
			l_params = l_params.substr (0,l_sequence) + a_task.m_parameters.parameterName[i] + l_params.substr (l_sequence + 3);
			int l_sequence2 = l_params.find("(2)");
			l_params = l_params.substr (0,l_sequence2) + a_task.m_parameters.parameterValue[i] + l_params.substr (l_sequence2 + 3);
			l_src += l_params;
			l_params.erase();
		}
		l_src += MRCP_ENDL;
	}

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
} //end namespace