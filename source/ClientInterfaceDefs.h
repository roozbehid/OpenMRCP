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
//	File Name:		ClientInterfaceDefs.h
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
#ifndef ClientInterfaceDefs_h
#define ClientInterfaceDefs_h

namespace MrcpV2RefLib
{

#define MrcpSuccess 0
#define MrcpError -1
#define SocketError -9999

typedef int MrcpSessionHandle; 
typedef int MrcpAudioHandle;


#define IP_ADDRESS_SIZE 50
#define SERVER_PORT_SIZE 5

  class CMrcpSession;

//The following structure is used to return values to client application
//via callback processing

	typedef struct{
		int referenceNumber;
		int MrcpReturnValue;
		MrcpSessionHandle asrSessionHandle;
		MrcpSessionHandle ttsSessionHandle;
		char MrcpReturnPkt [10000];
	}MRCP_RESULT_STRUCT;

typedef void(*MrcpCallBackFunction)(MRCP_RESULT_STRUCT*);

//The following enums are MRCP values used to identify the
//Server resources provided
	typedef enum  {
		speechrecog,
		dtmfrecog,
		speechsynth,
		basicsynth,
		recorder
	}MrcpResourceType;

// The following structure is used to configure
// MRCP resource Servers
	typedef struct{
		int   mrcpVersion;
		char* serverAddress;
		int   serverPort;
		MrcpCallBackFunction callBack;
	}MRCP_SERVER_CONFIG;

	typedef struct {
		char* clientAddress;
		MRCP_SERVER_CONFIG *asrServerConfig;
		MrcpSessionHandle asrSessionHandle;
		MrcpAudioHandle   outboundAudioHandle;
		MRCP_SERVER_CONFIG *ttsServerConfig;
		MrcpSessionHandle ttsSessionHandle;
		MrcpAudioHandle  inboundAudioHandle;
		MrcpCallBackFunction audioCallback;
	}RESOURCE_CFG_STRUCT;

	static __inline void INIT_MRCP_SERVER_CONFIG(MRCP_SERVER_CONFIG *aMrcpServerCfg)
	{
		aMrcpServerCfg->callBack = 0;
		aMrcpServerCfg->mrcpVersion = 0;
		aMrcpServerCfg->serverAddress = "\n";
		aMrcpServerCfg->serverPort = 0;
	}

	static __inline void INIT_RESOURCE_CFG_STRUCT(RESOURCE_CFG_STRUCT *aResourceCfg)
	{
		aResourceCfg->clientAddress = "\n";
		aResourceCfg->asrServerConfig = 0;
		aResourceCfg->asrSessionHandle = 0;
		aResourceCfg->outboundAudioHandle = 0;
		aResourceCfg->ttsServerConfig = 0;
		aResourceCfg->ttsSessionHandle = 0;
		aResourceCfg->inboundAudioHandle = 0;
		aResourceCfg->audioCallback = 0;
	}

// The following is used in the GetParams call
	typedef struct{
		int numParams;
		char** parameterName;
	}GET_PARAMETERS;

//The folowing is used in the SetParams call
	typedef struct{
		int numParams;
		char** parameterName;
		char** parameterValue;
	}SET_PARAMETERS;

//The following is used in the DefinGrammar client method
	typedef struct{
		int referenceNumber;
		char* contentType;
		char* contentID;
		char* gramContent;
		MrcpCallBackFunction callBack;
	}DEFINE_GRAMMAR_STRUCT;

//The following structure is used in the recognize client method
	typedef struct{
		int referenceNumber;
		bool cancelQueue;
		bool startInputTimers;
		char* contentType;
		char* gramContent;
		MrcpCallBackFunction callBack;
	}RECOGNIZE_STRUCT;

// The following structure is used in the interpret client method
	typedef struct{
		int referenceNumber;
		char* interpretText;
		char* contentType;
		char* contentId;
		char* content;
		MrcpCallBackFunction callBack;
	}INTERPRET_STRUCT;

//The following structure is used in the speak client method
	typedef struct {
		int referenceNumber;
		char* killOnBargeIn;
		char* voiceName;
		char* speechLanguage;
		char* contentType;
		char* content;
		MrcpCallBackFunction callBack;
	} SPEAK_STRUCT;

} //end namespace
#endif
