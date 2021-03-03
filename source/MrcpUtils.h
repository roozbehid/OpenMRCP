//Copyright (c) 2004 Dialogic Corporation
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//	-	Redistributions of source code must retain the above
//		copyright notice, this list of conditions and the 
//		following disclaimer. 
//	-	Redistributions in binary form must reproduce the above
//		copyright notice, this list of conditions and
//		the following disclaimer in the documentation and/or
//		other materials provided with the distribution. 
//	-	Neither the name of Dialogic Corporation nor the names of its
//		contributors may be used to endorse or promote products
//		derived from this software without specific prior
//		written permission. 
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
//DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
//OF SUCH DAMAGE.
//=============================================================================
//
//	File Name:		MrcpUtils.h
//
//	Description:	Mrcp Client Library support methods 
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MrcpUtils_H
#define MrcpUtils_H


#ifdef WIN32
#pragma warning( disable : 4786 )  
#pragma warning( disable : 4251 ) 

#include <wchar.h>
#include <stdarg.h>
#include <winsock2.h>
#else
#include <unistd.h>
#include <pthread.h>

#endif
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include <map>


#define snprintf _snprintf

typedef enum CRV { 
	CRV_ok	=	0,
	CRV_failure,
	CRV_not_handled,
	CRV_illegal_state
} ; 


class MrcpUtils  
{
public:
	static std::string itos(int data, int len = 0 ) ; 
	static const std::string ltos(
#ifdef WIN32
		__int64
#endif
		data) ; 
	int GetLengthDiff ( const char * input, int & num_subst ) ; 
	static bool ParseElement(char *target, size_t targetsize, const char *source, char const *tag, bool = false, bool = false ) ; 
	static bool isNumeric(char* str, int len);

	static void CloseSocket ( SOCKET sock );
	static int GetLastSocketError ( const char * what = NULL );
	static SOCKET ConnectServerSocket( std::string hostname, int port ) ; 
	static const CRV TcpGet ( SOCKET sock, char * data, int & len ) ;
	static const CRV TcpSend ( SOCKET sock, const char * const data, int length ) ; 
	static SOCKET OpenUdpListener( unsigned short this_port, bool broadcast = false ) ; 
	static char * stristr ( const char *, const char * ) ; 
	static int UdpGet( SOCKET socket, char * result, int res_len, char * origin, int orig_len );

	static void MsSleep( unsigned int a_millisecs) ;
	static unsigned long ThreadID();
};

#endif // !defined(AFX_MrcpUtils_H__E1DF8DD9_5E77_4848_B708_9FE7D7D086A8__INCLUDED_)
