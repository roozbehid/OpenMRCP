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
//	File Name:		CGenericCommands.cpp
//
//	Description:	 CGeneric class implementation
//					 Task queue processors for get/set params
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CGetParams::CGetParams(int a_cmdID, AMrcpResource* a_command, int a_version)
//		CGetParams::~CGetParams()
//		void CGetParams::Execute()
//		CSetParams::CSetParams(int a_cmdID, AMrcpResource* a_command, int a_version)
//		CSetParams::~CSetParams()
//		void CSetParams::Execute()
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//
///////////////////////////////////////////////////////////////////////////////
#include "ClientInterfaceDefs.h"
#include "CGenericCommands.h"
#include "AMrcpResource.h"


namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command ID
//         command
//		   mrcp version
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CGetParams::CGetParams(int a_cmdID, AMrcpResource* a_command, int a_version)
	:m_command(a_command)
	,m_cmdID(a_cmdID)
	,m_version(a_version)
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
CGetParams::~CGetParams()
{
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute
// Input - None
//         
// Output - None
//          call command using state machine
///////////////////////////////////////////////////////////////////////////
void CGetParams::Execute()
{	
	m_command->GetParams(*m_command, *this);
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - command ID, 
//         command,
//		   mrcp version
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
	CSetParams::CSetParams(int a_cmdID, AMrcpResource* a_command, int a_version)
		:m_command(a_command)
		,m_cmdID(a_cmdID)
		,m_version(a_version)
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
	CSetParams::~CSetParams()
{
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute
// Input - None
//         
// Output - None
//          call command using state machine
///////////////////////////////////////////////////////////////////////////
	void CSetParams::Execute()
{	
	m_command->SetParams(*m_command, *this);
}
} //end namespace