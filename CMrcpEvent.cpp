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
//	File Name:		CMrcpEvent.cpp
//
//	Description:	CMrcpEvent class implementation
//					Event implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpEvent::CMrcpEvent()
//		CMrcpEvent::~CMrcpEvent()
//		void CMrcpEvent::Execute()
//		int CMrcpEvent::HandleEvent()
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpEvent.h"
#include "AMrcpEventHandler.h"

namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpEvent::CMrcpEvent()
{
	EventHandler(NULL);
	EventID(0);
	ClassName("CMrcpEvent");
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpEvent::~CMrcpEvent()
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
void CMrcpEvent::Execute()
{
	if (m_eventObserver)
		m_eventObserver->HandleEvent(this);
	else
		CLogger::Instance()->Log(LOG_LEVEL_WARNING, *this, "Observer is NULL");	
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - HandleEvent
// Input - None
//         
// Output - success
//          
///////////////////////////////////////////////////////////////////////////
int CMrcpEvent::HandleEvent()
{
	if (m_eventObserver)
		m_eventObserver->HandleEvent(this);
	else
		CLogger::Instance()->Log(LOG_LEVEL_WARNING, *this, "Observer is NULL");	

	return MrcpSuccess;
}
} //end namespace