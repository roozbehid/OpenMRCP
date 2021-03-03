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
//	File Name:		MrcpClientEvents.h
//
//	Description:	Defines all the event IDs for the MRCP Client Library
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

#ifndef MrcpClientEvents_H
#define MrcpClientEvents_H

#define SIGNALING_BASE			          0xE0000000
#define SIGNALING_CONNECTED		          SIGNALING_BASE + 0x00000001
#define SIGNALING_DISCONNECTED            SIGNALING_BASE + 0x00000002
#define SIGNALING_PROTOCOL_CONNECTED      SIGNALING_BASE + 0x00000003
#define SIGNALING_PROTOCOL_DISCONNECTED   SIGNALING_BASE + 0x00000004
#define SYNTHESIZER_EXITED                SIGNALING_BASE + 0x00000005
#define RECOGNIZER_EXITED                 SIGNALING_BASE + 0x00000006
#define SESSIONS_STOPPED                  SIGNALING_BASE + 0x00000007
#define SESSION_MGR_STOPPED               SIGNALING_BASE + 0x00000008
#define RTP_SESSION_STOPPED				  SIGNALING_BASE + 0x00000011
#define RTP_MGR_STOPPED					  SIGNALING_BASE + 0x00000012
#define SIGNALING_PROTOCOL_FAILED         SIGNALING_BASE + 0x00000013

#define COMMAND_BASE                      0xE0000010
#define COMMAND_COMPLETE                  COMMAND_BASE + 0x00000001
//#define COMMAND_IN_PROGRESS				  COMMAND_BASE + 0x00000002

#endif //MrcpClientEvents_H
