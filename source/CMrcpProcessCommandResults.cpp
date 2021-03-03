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
//	File Name:		CMrcpSendCommand.cpp
//
//	Description:	CMrcpSendCommand class implementation
//                object instantiated to process command results from task queue
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpProcessCommandResults::CMrcpProcessCommandResults(AMrcpSignaling* a_signaling)
//		CMrcpProcessCommandResults::~CMrcpProcessCommandResults()
//		void CMrcpProcessCommandResults::Execute()
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
///////////////////////////////////////////////////////////////////////////////


#include "MrcpTasks.h"
#include "AMrcpSignaling.h"


namespace MrcpV2RefLib
{
////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - 
//         
// Output - 
//          
///////////////////////////////////////////////////////////////////////////

CMrcpProcessCommandResults::CMrcpProcessCommandResults(AMrcpSignaling* a_signaling)
	:  MrcpTasks( a_signaling)
{
	m_messageBuffer.erase();
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - 
//          
///////////////////////////////////////////////////////////////////////////
CMrcpProcessCommandResults::~CMrcpProcessCommandResults()
{

}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Execute
// Input - None
//         
// Output - 
//          
///////////////////////////////////////////////////////////////////////////
void CMrcpProcessCommandResults::Execute()
{
	m_signalObj->PrimProcessResults( MessageBuffer());
}

} //end namespace