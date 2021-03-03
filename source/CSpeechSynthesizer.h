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
//	File Name:		CSpeechRecognizer.h
//
//	Description:	class implementation
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
///////////////////////////////////////////////////////////////////////////////

#ifndef CSpeechSynthesizer_H
#define CSpeechSynthesizer_H

#include "AMrcpResource.h"
#include "ClientInterfaceDefs.h"
#include "AMrcpObserverSubject.h"

namespace MrcpV2RefLib
{
	class CMrcpSession;
	class CSpeakCommands;
	class CMrcpCmdIdMgr;
	class AMrcpSignaling;
	class CMrcpEvent;
	
	class CSpeechSynthesizer : public AMrcpResource,
							   public AMrcpObserverSubject
	{
	public:
		CSpeechSynthesizer(RESOURCE_CFG_STRUCT* a_resourceCfg, AMrcpSignaling* a_signalObj);
		virtual ~CSpeechSynthesizer();

	protected:

	private:
		RESOURCE_CFG_STRUCT* m_resourceCfg;  
		MrcpV2RefLib::CMrcpCmdIdMgr* m_cmdIdMgr;
		MrcpV2RefLib::CSpeakCommands* m_speakCmds;

		MrcpResourceType m_resourceType;
		int m_mrcpVersion;

		int PrimSpeak(CSpkSpeak& a_task);
		int PrimPause(CSpkPause& a_task);
		int PrimResume(CSpkResume& a_task);
		int PrimBargeIn(CSpkBargeIn& a_task);
		int PrimStopSpk(CSpkStop& a_task);

		int PrimDefineGrammar(CRecDefineGrammar& a_task) {return 0;};
		int PrimRecognize(CRecRecognize& a_task){return 0;};
		int PrimInterpret(CRecInterpret& a_task){return 0;};
		int PrimStartInputTimers(CRecStartInputTimers& a_task){return 0;};
		int PrimGetResult(CRecGetResult& a_task){return 0;};
		int PrimStopRec(CRecStopRec& a_task){return 0;};

		int PrimGetParams(CGetParams& a_task) {return 0;};
		int PrimSetParams(CSetParams& a_task) {return 0;};
		


	};


}//end namespace

#endif