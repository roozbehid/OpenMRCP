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
//	File Name:		CSpeakCommands.cpp
//
//	Description:	class implementation of the speak related classes
//				part of the command state machine
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CSpkSpeak::CSpkSpeak(int a_cmdID, AMrcpResource* a_command,SPEAK_STRUCT* a_speakParams)
//		CSpkSpeak::~CSpkSpeak()
//		CSpkSpeak::Execute()
//		CSpkStop::CSpkStop(int a_cmdID, AMrcpResource* a_command)
//		CSpkStop::~CSpkStop()
//		CSpkStop::Execute()
//		CSpkPause::CSpkPause(int a_cmdID, AMrcpResource* a_command)
//		CSpkPause::~CSpkPause()
//		CSpkPause::Execute()
//		CSpkResume::CSpkResume(int a_cmdID, AMrcpResource* a_command)
//		CSpkResume::~CSpkResume()
//		CSpkResume::Execute()
//		CSpkBargeIn::CSpkBargeIn(int a_cmdID, AMrcpResource* a_command)
//		CSpkBargeIn::~CSpkBargeIn()
//		CSpkBargeIn::Execute()
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
///////////////////////////////////////////////////////////////////////////////
#include "CSpeakCommands.h"
#include "AMrcpResource.h"


namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command id
//         resource pointer
//		   speak parameters structure
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpkSpeak::CSpkSpeak(int a_cmdID, AMrcpResource* a_command,SPEAK_STRUCT* a_speakParams)
	: m_cmdID(a_cmdID)
	, m_command(a_command)
	, m_killOnBargeIn(a_speakParams->killOnBargeIn)
	, m_voiceName(a_speakParams->voiceName)
	, m_speechLanguage(a_speakParams->speechLanguage)
	, m_contentType(a_speakParams->contentType)
	, m_content(a_speakParams->content)
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
CSpkSpeak::~CSpkSpeak()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute
// Input - None
//         
// Output - None
//      code that is called as task processor calls request from queue
///////////////////////////////////////////////////////////////////////////
void CSpkSpeak::Execute()
{
	m_command->Speak(*m_command, *this);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command ID
//         resource pointer
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpkStop::CSpkStop(int a_cmdID, AMrcpResource* a_command)
	: m_cmdID(a_cmdID)
	, m_command(a_command)
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
CSpkStop::~CSpkStop()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output -
//       code that is called as task processor calls request from queue                     
///////////////////////////////////////////////////////////////////////////
void CSpkStop::Execute()
{
	m_command->StopSpeak(*m_command, *this);
}
	
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - Command ID
//         resource pointer
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpkPause::CSpkPause(int a_cmdID, AMrcpResource* a_command)
	: m_cmdID(a_cmdID)
	, m_command(a_command)
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
CSpkPause::~CSpkPause()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output - None
//     code that is called as task processor calls request from queue                     
///////////////////////////////////////////////////////////////////////////
void CSpkPause::Execute()
{
	m_command->Pause(*m_command, *this);
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - Command ID
//         Resource pointer
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpkResume::CSpkResume(int a_cmdID, AMrcpResource* a_command)
	: m_cmdID(a_cmdID)
	, m_command(a_command)
{
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor 
// Input - None
//         
// Output - requestId
//          
///////////////////////////////////////////////////////////////////////////
CSpkResume::~CSpkResume()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output - None
// code that is called as task processor calls request from queue                        
///////////////////////////////////////////////////////////////////////////
void CSpkResume::Execute()
{
	m_command->Resume(*m_command, *this);
}
	
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - Command ID
//         resource pointer
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CSpkBargeIn::CSpkBargeIn(int a_cmdID, AMrcpResource* a_command)
	: m_cmdID(a_cmdID)
	, m_command(a_command)
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
CSpkBargeIn::~CSpkBargeIn()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output - None
//  code that is called as task processor calls request from queue                        
///////////////////////////////////////////////////////////////////////////
void CSpkBargeIn::Execute()
{
	m_command->BargeIn(*m_command, *this);
}
}//end namespace
