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
//	File Name:		CRecognitionCommands.cpp
//
//	Description:	 class implementation recognition related command objects
//				  Part of the command state machine
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CRecDefineGrammar::CRecDefineGrammar(int a_cmdID, AMrcpResource* a_command,DEFINE_GRAMMAR_STRUCT* a_defineGrammarParams)
//		CRecDefineGrammar::~CRecDefineGrammar()
//		void CRecDefineGrammar::Execute()
//		CRecRecognize::CRecRecognize(int a_cmdID, AMrcpResource* a_command,bool a_cancelQueue, char* a_contentType, char* a_gramContent,bool a_startInputTimers)
//		CRecRecognize::~CRecRecognize()
//		void CRecRecognize::Execute()
//		CRecInterpret::CRecInterpret(int a_cmdID, AMrcpResource* a_command, char* a_contentType, char* a_interpretText, char* a_contentId, char* a_content)
//		CRecInterpret::~CRecInterpret()
//		void CRecInterpret::Execute()
//		CRecGetResult::CRecGetResult(int a_cmdID, AMrcpResource* a_command)
//		CRecGetResult::~CRecGetResult()
//		CRecGetResult::Execute()
//		CRecStopRec::CRecStopRec(int a_cmdID, AMrcpResource* a_command)
//		CRecStopRec::~CRecStopRec()
//		CRecStopRec::Execute()
//		CRecStartInputTimers::CRecStartInputTimers(int a_cmdID, AMrcpResource* a_command)
//		CRecStartInputTimers::~CRecStartInputTimers()
//		CRecStartInputTimers::Execute()

///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//
///////////////////////////////////////////////////////////////////////////////

#include "CRecognitionCommands.h"
#include "MrcpUtils.h"
#include "AMrcpResource.h"


namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command ID
//         define grammar parameter structure
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CRecDefineGrammar::CRecDefineGrammar(int a_cmdID, AMrcpResource* a_command,DEFINE_GRAMMAR_STRUCT* a_defineGrammarParams)
	: m_cmdID(a_cmdID)
	, m_command(a_command)
	, m_contentType(a_defineGrammarParams->contentType)
	, m_contentID(a_defineGrammarParams->contentID)
	, m_gramContent (a_defineGrammarParams->gramContent)
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
CRecDefineGrammar::~CRecDefineGrammar()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output - None
//		code that is called as task processor calls request from queue
///////////////////////////////////////////////////////////////////////////
void CRecDefineGrammar::Execute()
{
	m_command->DefineGrammar(*m_command, *this);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command ID
//         cancel queue - indicates bargein support
//		   content type - this is the xml or plain text related identifiers
//		   grammar content - actual grammar formatted by client application
//		   start input timers - indicates if starting the input timers is manual
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CRecRecognize::CRecRecognize(int a_cmdID, AMrcpResource* a_command,bool a_cancelQueue, char* a_contentType, char* a_gramContent,bool a_startInputTimers)
	: m_cmdID(a_cmdID)
	, m_cancelQueue(a_cancelQueue)
	, m_startInputTimers(a_startInputTimers)
	, m_command(a_command)
	, m_contentType(a_contentType)
	, m_gramContent (a_gramContent)
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
CRecRecognize::~CRecRecognize()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output - None
//  
//		code that is called as task processor calls request from queue
///////////////////////////////////////////////////////////////////////////
void CRecRecognize::Execute()
{
	m_command->Recognize(*m_command, *this);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command ID
//         command
//		   content type - identifies the format of the grammar 
//		   interpret text - what to look for
//		   content id
//		   content
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CRecInterpret::CRecInterpret(int a_cmdID, AMrcpResource* a_command, char* a_contentType, char* a_interpretText, char* a_contentId, char* a_content)
	: m_cmdID(a_cmdID)
	, m_command(a_command)
	, m_contentType(a_contentType)
	, m_interpretText (a_interpretText)
	, m_contentID (a_contentId)
	, m_content(a_content)
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
CRecInterpret::~CRecInterpret()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output -
//		code that is called as task processor calls request from queue        
///////////////////////////////////////////////////////////////////////////
void CRecInterpret::Execute()
{
	m_command->Interpret(*m_command, *this);
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command id
//         pointer to resource 
// Output - None
//          Need to add confidence and n-best-list parameters
///////////////////////////////////////////////////////////////////////////
CRecGetResult::CRecGetResult(int a_cmdID, AMrcpResource* a_command)
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
CRecGetResult::~CRecGetResult()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output - None
//		code that is called as task processor calls request from queue        
///////////////////////////////////////////////////////////////////////////
	void CRecGetResult::Execute()
	{
		m_command->GetResult(*m_command, *this);
	}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command id
//         resource pointer
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CRecStopRec::CRecStopRec(int a_cmdID, AMrcpResource* a_command)
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
CRecStopRec::~CRecStopRec()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output -None
//		code that is called as task processor calls request from queue       
///////////////////////////////////////////////////////////////////////////
void CRecStopRec::Execute()
{
	m_command->StopRec(*m_command, *this);
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command id
//         resource command pointer
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CRecStartInputTimers::CRecStartInputTimers(int a_cmdID, AMrcpResource* a_command)
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
CRecStartInputTimers::~CRecStartInputTimers()
{

}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute 
// Input - None
//         
// Output -
//		code that is called as task processor calls request from queue        
///////////////////////////////////////////////////////////////////////////
void CRecStartInputTimers::Execute()
{
	m_command->StartInputTimers(*m_command, *this);
}
}//end namespace
