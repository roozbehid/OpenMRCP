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
//	File Name:		CRecognitionCommands.h
//
//	Description:	 class implementation
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
//
///////////////////////////////////////////////////////////////////////////////
#ifndef CRecognitionCommands_H
#define CRecognitionCommands_H

#include <string>
#include "MrcpTasks.h"

#define MRCP_ENDL "\r\n"
 
namespace MrcpV2RefLib
{
	class AMrcpResource;
	class CRecognitionCommands 
	{
		friend class CRecognitionIdleState;
		friend class CSpeechRecognizer;
	public:
		CRecognitionCommands(int a_version) : m_version (a_version)
		{
		 define_grammar1 =
		"DEFINE-GRAMMAR (2) MRCP/1.0" MRCP_ENDL  
		"Content-Type: (4)" MRCP_ENDL
		"Content-Id: (5)" MRCP_ENDL
		"Content-Length:  (6)" MRCP_ENDL
		MRCP_ENDL
		"(7)";

		define_grammar2 =
		"MRCP/2.0 (1) DEFINE-GRAMMAR (2)" MRCP_ENDL
		"Channel-Identifier: (3)" MRCP_ENDL
		"Content-Type: (4)" MRCP_ENDL
		"Content-Length:  (5)" MRCP_ENDL
		"Content-Id:  (6)" MRCP_ENDL
		MRCP_ENDL
		"(7)";

		recognize_1 =
		"RECOGNIZE (2) MRCP/1.0" MRCP_ENDL 
		 MRCP_ENDL
		"Recognize (3) MRCP/1.0" MRCP_ENDL;


		recognize_2 = 
		"MRCP/2.0 (1) RECOGNIZE (2)" MRCP_ENDL
		"Cancel-If-Queue: (3)" MRCP_ENDL
		"Channel-Identifier: (4)" MRCP_ENDL
		"Content-Type: (5)" MRCP_ENDL
		"Start-Input-Timers: (6)" MRCP_ENDL
		"Content-Length: (7)" MRCP_ENDL
		MRCP_ENDL
		"(8)";

		interpret_2 = 
		"MRCP/2.0 (1) INTERPRET (2)" MRCP_ENDL
		"Channel-Identifier: (3)" MRCP_ENDL
		"Interpret-Text: (4)" MRCP_ENDL
		"Content-Type: (5)" MRCP_ENDL
		"Content-ID: (6)" MRCP_ENDL
		"Content-Length: (7)" MRCP_ENDL
		MRCP_ENDL
		"(8)"
		MRCP_ENDL;

		stopRec_1 =
			"STOP (1) MRCP/1.0" MRCP_ENDL MRCP_ENDL;

		stopRec_2 =
			"MRCP/2.0 (1) STOP (2)" MRCP_ENDL
			"Channel-Identifier: (4)" MRCP_ENDL MRCP_ENDL;

		getResult_2 =
			"MRCP/2.0 (1) GET-RESULT (2)" MRCP_ENDL
			"Channel-Identifier:(3)" MRCP_ENDL MRCP_ENDL;

		startInputTimers_2 =
			"MRCP/2.0 (1) START-INPUT-TIMERS (2)" MRCP_ENDL
            "Channel-Identifier:(3)" MRCP_ENDL MRCP_ENDL;

		};
		virtual ~CRecognitionCommands(){ m_version = 0;};


	protected:
	private:
		std::string define_grammar1;
		std::string define_grammar2;
		std::string recognize_1;
		std::string recognize_2;
		std::string interpret_2;
		std::string stopRec_1;
		std::string stopRec_2;
		std::string getResult_2;
		std::string startInputTimers_2;
		int m_version;
	};

	class CRecDefineGrammar : public MrcpTasks
	{
	public:
		CRecDefineGrammar(int a_cmdID, AMrcpResource* a_command, DEFINE_GRAMMAR_STRUCT* a_defineGrammarParams);
	    virtual ~CRecDefineGrammar();
		void Execute();

		int m_cmdID;
		std::string m_contentType;
		std::string m_contentID;
		std::string m_gramContent;

	protected:
	private:
		AMrcpResource* m_command;
		CRecDefineGrammar();
	};

	class CRecRecognize : public MrcpTasks
	{
	public:
		CRecRecognize(int cmdID, AMrcpResource* a_command, bool a_cancelQueue,  char* a_contentType,  char* a_gramContent, bool a_startInputTimers);
	    virtual ~CRecRecognize();
		void Execute();

		int m_cmdID;
		bool m_cancelQueue;
		bool m_startInputTimers;
		std::string m_contentType;
		std::string m_contentID;
		std::string m_gramContent;

	protected:
	private:
		AMrcpResource* m_command;
		CRecRecognize();
	};

	class CRecInterpret : public MrcpTasks
	{
	public:
		CRecInterpret(int cmdID, AMrcpResource* a_command, char* a_contentType,  char* a_interpretText,char* a_contentId, char* a_content);
	    virtual ~CRecInterpret();
		void Execute();

		int m_cmdID;
		std::string m_contentType;
		std::string m_contentID;
		std::string m_interpretText;
		std::string m_interpretContent;
		std::string m_content;


	protected:
	private:
		AMrcpResource* m_command;
		CRecInterpret();
	};

	class CRecStopRec : public MrcpTasks
	{
	public:
		CRecStopRec(int cmdID, AMrcpResource* a_command);
	    virtual ~CRecStopRec();
		void Execute();
		int m_cmdID;

	protected:
	private:
		AMrcpResource* m_command;
		CRecStopRec();
	};

	class CRecGetResult : public MrcpTasks
	{
	public:
		CRecGetResult(int cmdID, AMrcpResource* a_command);
	    virtual ~CRecGetResult();
		void Execute();

		int m_cmdID;

	protected:
	private:
		AMrcpResource* m_command;
		CRecGetResult();
	};

	class CRecStartInputTimers : public MrcpTasks
	{
	public:
		CRecStartInputTimers(int cmdID, AMrcpResource* a_command);
	    virtual ~CRecStartInputTimers();
		void Execute();

		int m_cmdID;

	protected:
	private:
		AMrcpResource* m_command;
		CRecStartInputTimers();
	};


}//end namespace

#endif