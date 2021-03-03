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
//	File Name:		CGenericMrcpCommands.cpp
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

#ifndef CGenericCommands_H
#define CGenericCommands_H

#include <string>
#include "MrcpTasks.h"

#define MRCP_ENDL "\r\n"

namespace MrcpV2RefLib
{
	class AMrcpResource;

	class CGenericCommands 
	{
		friend class CMrcpGeneric;

	public:
		CGenericCommands(int a_version) : m_version (a_version)
		{
		set_params2 =
			"MRCP/2.0 (1) SET-PARAMS (2)" MRCP_ENDL
			"Channel-Identifier: (3)" MRCP_ENDL;

		get_params2 =
			"MRCP/2.0 (1) GET-PARAMS (2)" MRCP_ENDL
			"Channel-Identifier: (3)" MRCP_ENDL ;  //2 MRCP_ENDL lines needed
	
		get_params_arguments2 =
			"(1):" MRCP_ENDL;

		set_params_arguments2 =
			"(1): (2)" MRCP_ENDL;

		};
		virtual ~CGenericCommands(){ m_version = 0;};

	protected:

	private:
		std::string set_params2;
		std::string get_params2;
		std::string get_params_arguments2;
		std::string set_params_arguments2;
		int m_version;
		AMrcpSignaling* m_signalObj;

	};
	/////////
	class CGetParams : public MrcpTasks
	{
		friend class CMrcpGeneric;
	public:
		CGetParams(int a_cmdID, AMrcpResource* a_command, int a_version);
	    virtual ~CGetParams();
		void Execute();

		int m_cmdID;
		GET_PARAMETERS m_parameters;

	protected:
	private:
		AMrcpResource* m_command;
		CGetParams();
		int m_version;
	};
	/////////
	class CSetParams : public MrcpTasks
	{
		friend class CMrcpGeneric;
	public:
		CSetParams(int a_cmdID, AMrcpResource* a_command, int a_version);
	    virtual ~CSetParams();
		void Execute();

		int m_cmdID;
		SET_PARAMETERS m_parameters;

	protected:
	private:
		AMrcpResource* m_command;
		CSetParams();
		int m_version;
	};
}//end namespace

#endif