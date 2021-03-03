//Copyright (c) 2005 Dialogic Corporation
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
//	File Name:		MrcpUtils.cpp
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


#include <time.h>
#include <stdio.h>

#include "MrcpUtils.h"

static const char * enc_start = "<s_encoded>", * enc_end = "</s_encoded>" ; 


//////////////////////////////////////////////////////////////////////////////////////
//
//Description - connects a socket to the server
// Input - hostname dotted ip address , port # to connect to
// Output - Socket number connected
//
/////////////////////////////////////////////////////////////////////////////////////

SOCKET MrcpUtils::ConnectServerSocket( std::string hostname, int port )
{
	time_t start_time = time ( 0 ) ; 
	#ifdef _WIN32 
		WSADATA WsaData;
		if (SOCKET_ERROR == WSAStartup(0x0202, &WsaData)) 
		{
			printf ( "Unable to initialize socket layer" ) ; 
			exit ( 0 ) ; 
		}
	#endif
		//
//FYI
	//SOCK_DGRAM == UDP socket
	//SOCK_STREAM == TCP socket

	SOCKET sock ;
	sock = socket ( AF_INET, SOCK_STREAM, 0 ) ;
	if ( sock < 0 )
	{
		int error_number = GetLastSocketError ( "MrcpUtils::GetConnectedSocket - getting a socket" ) ; 
		return INVALID_SOCKET ; 
	}
	
	SOCKADDR_IN remoteAddr;
	IN_ADDR address ;
	
	memset(&remoteAddr, 0, sizeof(remoteAddr));
		
		// See if the host is specified in "dot address" form
		int this_addr = inet_addr(hostname.c_str());
		memcpy ( &address, &this_addr, sizeof ( this_addr)) ;
		if ( this_addr == -1) 
		{
			CloseSocket ( sock ) ; 
			sock = INVALID_SOCKET ;
			return sock ; 
		}
		memcpy ( &remoteAddr.sin_addr, &this_addr, sizeof ( this_addr ) ) ;

	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(port);
	
	time_t connect_time = time(0) ; 
	
	int res = connect(sock, (PSOCKADDR)&remoteAddr, sizeof(remoteAddr) );              
	if ( res ) 
	{
		int error = GetLastSocketError( "MrcpUtils::Connect Server" ) ; 

		CloseSocket ( sock ) ; 
		sock = INVALID_SOCKET ;
		return sock ; 
	}
	
	int conn_duration = time ( 0 ) - start_time ; 
		
	return sock ;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//Description - converts integer to a string 
// Input - interger value and length
// Output - converted string
//
/////////////////////////////////////////////////////////////////////////////////////
std::string MrcpUtils::itos(int data, int len)
{
	char buf[100] ;
	
	char format[10] ; 
	
	if ( len )
		snprintf ( format, sizeof(format), "%%0%dd", len ) ; 
	else
		snprintf ( format, sizeof(format), "%%d" ) ; 
	
	snprintf ( buf, sizeof(buf), format, data ) ;
	return buf ;
}
//////////////////////////////////////////////////////////////////////////////////////
//
//Description - checks for numeric value in string
// Input - character pointer
// Output - integer length
//
/////////////////////////////////////////////////////////////////////////////////////
bool MrcpUtils::isNumeric(char* str, int len)
{
      for(int i=0;i<len && str[i];i++)
            if((str[i])<0x30 || (str[i]) >0x39)
                  return false;
      return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//Description - sends data to the mrcp server
// Input - socket connected to, pointer to data and length of data
// Output - Socket return status
//
/////////////////////////////////////////////////////////////////////////////////////

//the following is used to send to the mrcp server
const CRV MrcpUtils::TcpSend ( SOCKET sock, const char * const data, int length ) 
{
	
	CRV result = CRV_ok ;
	int res = 0 ; 
	
	if ( sock == INVALID_SOCKET )
		return CRV_failure ; 
	
#ifdef WIN32
	res = send ( sock, data, length, 0 ) ; 
#else
	res = write ( sock, data, length ) ; 
#endif
	
	if ( res < 0 ) 
	{
		int error = GetLastSocketError( "MrcpUtils::TcpSend - sending data" ) ; 
		
			printf( "Unable to send to remote host %d-%d - %d/%d\n",
				res, length, sock, error) ; 
		
		result = CRV_failure ;
	}
	
	return result ;
}

void MrcpUtils::CloseSocket(SOCKET sock)
{
	
	time_t start = time(0) ; 
	
	int res = 0 ; 
	
	// if the socket is already closed, just return 
	if ( sock == INVALID_SOCKET || sock == 0 ) 
	{
		return ; 
	}
	
#ifdef WIN32
	res = closesocket ( sock ) ; 
#else
	res = close ( sock ) ; 
#endif 
	
	if ( res )
	{
		GetLastSocketError ( "Closing socket" ) ; 
	}
	
	unsigned int duration = time(0) - start ; 
	
	
}
//////////////////////////////////////////////////////////////////////////////////////
//
//Description - gets the last socket error returned
// Input - not used
// Output - Socket error
//
/////////////////////////////////////////////////////////////////////////////////////

int MrcpUtils::GetLastSocketError( const char * what )
{
	
	int error_number ; 
	
	error_number = WSAGetLastError () ; 

	
	return error_number ; 
}
//////////////////////////////////////////////////////////////////////////////////////
//
//Description - finds a string within a string
// Input - string and pattern to find
// Output - char*
//
/////////////////////////////////////////////////////////////////////////////////////


char *MrcpUtils::stristr(const char *String, const char *Pattern)
{
      char *pptr, *sptr, *start;

      for (start = (char *)String; *start != NULL; start++)
      {
            /* find start of pattern in string */
            for ( ; ((*start!=NULL) && (toupper(*start) != toupper(*Pattern))); start++)
                  ;
            if (NULL == *start)
                  return NULL;

            pptr = (char *)Pattern;
            sptr = (char *)start;

            while (toupper(*sptr) == toupper(*pptr))
            {
                  sptr++;
                  pptr++;

                  /* if end of pattern then pattern was found */

                  if (NULL == *pptr)
                        return (start);
            }
      }
      return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//Description - parses for an element and returns after patten is found
// Input - ptr to target, sizeof target, ptr to source, ptr to tag to find, case sensitive,spaces before
// Output - ture false
//
/////////////////////////////////////////////////////////////////////////////////////

// puts into target a null terminated value of the element of ops transm. 
bool MrcpUtils::ParseElement(char *target, size_t targetsize, const char *source, char const *tag, 
							bool insensitive, bool spaces_before )
{
	bool ret = true;
	if ( !target || !source || !tag || !targetsize) return ret; 
	
	const char * orig = NULL ;
	
	if ( !insensitive )
		orig = strstr ( source, tag ) ; 
	else
		orig = MrcpUtils::stristr ( source, tag ) ; 

	if ( !orig )
		target[0] = 0 ; 
	else
	{
		const char * end = NULL ;
		orig += strlen ( tag ) ; 
		
		// if starts with a quote, go to the next char and end at the quote
		if ( *orig == '"' )
		{
			orig ++ ; 
			end = strchr ( orig, '"' ) ; 
		}
		else
		{
			if ( spaces_before )
				while ( *orig == ' ' ) 
					orig ++ ;	// move until we hit something

			end = strchr ( orig, ' ' ) ; 
			if ( !end )
				end = strchr ( orig, '\n' ) ; 
			if ( !end )
				end = strchr ( orig, '\r' ) ; 
			
			// see if there is an unprintable character prior to the end, if so end there.
			const char * cptr ; 
			for ( cptr = orig ; cptr <= end ; cptr ++ )
				if ( *cptr < 0x20 )
					break ; 
				
				if ( cptr < end ) end = cptr ; 
				
				if ( end == NULL ) end = (char*)source + strlen ( source ) ; 
		}
		size_t len = end - orig;
		if (len > (targetsize-1)) 
		{
			len = targetsize - 1;
			ret = false;
		}
		memcpy ( target, orig, len ) ; 
		target [len ] = 0 ; 
	}

	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////
//
//Description - gets packet from tcp socket
// Input - socket, ptr to data received, length of data received
// Output - Socket status
//
/////////////////////////////////////////////////////////////////////////////////////


const CRV MrcpUtils::TcpGet ( SOCKET sock, char * data, int & length )
{
	int len ; 
	CRV result = CRV_ok ; 
	
#ifdef WIN32
	len = recv ( sock, data, length, 0 ) ; 
#else
	len = read ( sock, data, length ) ; 
#endif
	
	if ( len == 0 ) 
	{
		result = CRV_illegal_state ;
		length = 0 ; 
	}
	else if ( len < 0 ) 
	{
		result = CRV_failure ;
		int err = GetLastSocketError( "SitUtils::TcpGet - getting data" ) ; 
		
	}
	else
	{
		result = CRV_ok ;
		length = len ; 
	}
	
	return result ;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//Description - opens a udp listener port
// Input - port # to connect open, boolean to indicate broadcast 
// Output - Socket number connected
//
/////////////////////////////////////////////////////////////////////////////////////

SOCKET MrcpUtils::OpenUdpListener( unsigned short this_port, bool broadcast )
{
	SOCKADDR_IN local_addr ; 
#ifdef WIN32
		WSADATA WsaData;
	if (SOCKET_ERROR == WSAStartup(0x0202, &WsaData))
	{
		return INVALID_SOCKET;
	}
#endif
	
	SOCKET descr = socket ( AF_INET, SOCK_DGRAM, 0 ) ; 
	if ( descr == INVALID_SOCKET ) 
		return INVALID_SOCKET ;
	
	memset ( &local_addr, 0, sizeof ( local_addr ) ) ;
	local_addr.sin_port = htons ( this_port ) ; 
	local_addr.sin_family = AF_INET ;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if ( broadcast )
	{
		BOOL value = 1 ; 
		setsockopt ( descr, SOL_SOCKET, SO_BROADCAST, (const char*)&value, sizeof ( value ) ) ; 
	}
	
	int err = bind ( descr, (PSOCKADDR)&local_addr, sizeof ( local_addr ) ) ;
	if ( err < 0 ) 
	{
	   	err = GetLastSocketError( "MrcpUtils::OpenUdpListener - binding" );
		printf ( "Error %d opening udp listener \n", err ) ; 
		return INVALID_SOCKET ; 
	}
		
	return descr ;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//Description - performs a udp get on socket
// Input - socket, result, result length, origin and origin length
// Output - number of bytes gotten
//
/////////////////////////////////////////////////////////////////////////////////////

int MrcpUtils::UdpGet( SOCKET socket, char * result, int res_len, char * origin, int orig_len )
{
	
	struct sockaddr srcS ; 
	struct sockaddr_in src ;
	
#ifdef WIN32
	int srcSize = sizeof ( srcS ) ; 
//try this
//	u_long mode = 1;
//	ioctlsocket(socket, FIONBIO, &mode);
#else
	socklen_t srcSize = sizeof ( srcS ) ; 
#endif
	
	int buf_len = recvfrom ( socket, result, res_len, 0, &srcS, &srcSize ) ;
	if (buf_len < 0 )  
	{
		int err = GetLastSocketError( "SitUtils::UdpGet - receiving" ) ;
		return -1 ;
	}
	
	memcpy(&src,&srcS,sizeof(src));
	
	char * from = inet_ntoa(src.sin_addr) ; 
	
	snprintf( origin, orig_len, "%s", from );
	
	return buf_len ; 
}

//////////////////////////////////////////////////////////////////////////////////////
//
//Description - returns the differance in length
// Input - ptr to input and size of substring
// Output - differance
//
/////////////////////////////////////////////////////////////////////////////////////

int MrcpUtils::GetLengthDiff ( const char * input, int & num_subst ) 
{
	int num_wide = 0, diff = 0 ; 
	const char * start_ptr = input ; 

	num_subst = 0 ; 
	while ( ( start_ptr = strstr ( start_ptr, enc_start ) ) != NULL ) 
	{
		char * end_ptr = strstr ( start_ptr, enc_end ) ; 
		if ( end_ptr )
		{
			num_wide += ( end_ptr - start_ptr - strlen ( enc_start ) ) / 4 ; 
			num_subst ++ ; 
		}
		else
		{
			return 0 ; 
		}

		start_ptr = end_ptr ; 
	}

	if ( num_subst != 0 ) 
	{
		// see what the increase in length is
		int was = num_subst * ( strlen ( enc_end ) + strlen ( enc_start ) ) + num_wide * 4 ; 
		int will_be = num_wide * sizeof ( wchar_t ) ; 
		diff = will_be - was ; 
	}

	return diff ; 
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

void MrcpUtils::MsSleep( unsigned int a_millisecs) 
{
#ifdef _WIN32

   Sleep( a_millisecs);     

#else

   usleep( a_millisecs * 1000);     

#endif     
}
///////////////////////////////////////////////////////////////////////////////
// Description: Operating System independent thread ID accessor
// Return: unsigned long
// Parameters: none
///////////////////////////////////////////////////////////////////////////////
unsigned long MrcpUtils::ThreadID()
{
#ifdef _WIN32
   return GetCurrentThreadId();
#else
   return pthread_self();
#endif

}


