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
//	File Name:		CMrcpHandleMgr.cpp
//
//	Description:	CMrcpHandleMgr class implementation
//				  class provides handle to object pointer map
//			      method to generate and return a handle and then add it to map
//				  methods to retrive object point for audio and session based on handle
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpHandleMgr::CMrcpHandleMgr()
//		CMrcpHandleMgr::~CMrcpHandleMgr()
//		int CMrcpHandleMgr::RegisterPointer(CMrcpSession* a_objPointer)
//		int CMrcpHandleMgr::RegisterAudioPointer(CMrcpAudioStream* a_objPointer)
//		int CMrcpHandleMgr::UnregisterPointer(int a_handle,std::string a_pointerType)
//		CMrcpSession* CMrcpHandleMgr::RetrievePointer (int a_handle)
//		CMrcpAudioStream* CMrcpHandleMgr::RetrieveAudioPointer (int a_handle)
//		int CMrcpHandleMgr::Cleanup()
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//
///////////////////////////////////////////////////////////////////////////////
#include "CMrcpHandleMgr.h"
#include "CMrcpSession.h"
#include "MrcpClientEvents.h"
#include "CMrcpAudioStream.h"
#include "MrcpUtils.h"


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
CMrcpHandleMgr::CMrcpHandleMgr()
	:m_handleID(0)
{
	ClassName("CMrcpHandleMgr");
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor 
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpHandleMgr::~CMrcpHandleMgr()
{
	m_handleID=0;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - RegisterPointer
// Input - session object pointer
//         
// Output - success or failure
//          Generates handle and maps value to session pointer
//			returns handle
///////////////////////////////////////////////////////////////////////////
int CMrcpHandleMgr::RegisterPointer(CMrcpSession* a_objPointer)
{
	Name("RegisterPointer");
	
	if ( a_objPointer == NULL)
		{
			CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Object pointer is NULL");
			return -1;
		}
	{ //scope for lock
		boost::mutex::scoped_lock lock( m_criticalSection);
		m_handleID++;
		m_hndlePtrMap[m_handleID] = a_objPointer;
	}
		return m_handleID;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - RegisterAudioPointer
// Input - audio stream pointer 
//         
// Output - success or failure
//          generates a handle and maps handle to audio pointer
//		    returns handle to calling methods
///////////////////////////////////////////////////////////////////////////
int CMrcpHandleMgr::RegisterAudioPointer(CMrcpAudioStream* a_objPointer)
{
	Name("RegisterAudioPointer");
	
	if ( a_objPointer == NULL)
		{
			CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Object pointer is NULL");
			return -1;
		}
	{  //scope for lock
		boost::mutex::scoped_lock lock( m_criticalSection);
		m_handleID++;
		m_hndleAudioPtrMap[m_handleID] = a_objPointer;
	}
		return m_handleID;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - UnregisterPointer 
// Input - handle
//         pointer type
// Output - success or failure
//          removes session pointer and handle from the map
///////////////////////////////////////////////////////////////////////////

int CMrcpHandleMgr::UnregisterPointer(int a_handle,std::string a_pointerType)
{
	Name("UnregisterPointer");

	HandleToPointerMap::iterator l_pos;
	AudioHandleToPointerMap::iterator l_audioPos;
	if (a_pointerType == "Session")
	{
		{  //scope for lock
		boost::mutex::scoped_lock lock( m_criticalSection);
		if ( (l_pos = m_hndlePtrMap.find(a_handle)) == m_hndlePtrMap.end())
		{
			CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "handle not found");
			return -1;
		}
		m_hndlePtrMap.erase(l_pos);
		}
	}
	if (a_pointerType == "Audio")
	{
		{//scope for lock
		boost::mutex::scoped_lock lock( m_criticalSection);
		if ( (l_audioPos = m_hndleAudioPtrMap.find(a_handle)) == m_hndleAudioPtrMap.end())
		{
			CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "audio handle not found");
			return -1;
		}
		m_hndleAudioPtrMap.erase(l_audioPos);
		}
	}
	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - RetrievePointer 
// Input - handle
//         
// Output - NULL or object pointer
//         performs a lookup based on handle and returns pointer to session
//		   object
///////////////////////////////////////////////////////////////////////////
CMrcpSession* CMrcpHandleMgr::RetrievePointer (int a_handle)
{
	Name("RetrievePointer");
	HandleToPointerMap::iterator l_pos;
	{  //scope for lock
	boost::mutex::scoped_lock lock( m_criticalSection);
	if ((l_pos = m_hndlePtrMap.find(a_handle)) == m_hndlePtrMap.end())
	{
		CLogger::Instance()->Log(LOG_LEVEL_WARNING, *this, "handle ptr not found");
		return NULL;
	}
	else
		return l_pos->second;
	}
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - RetrieveAudioPointer 
// Input - handle
//         
// Output - NULL or object pointer
//          performs a lookup based on handle and returns pointer to audio object
///////////////////////////////////////////////////////////////////////////

CMrcpAudioStream* CMrcpHandleMgr::RetrieveAudioPointer (int a_handle)
{
	Name("RetrieveAudioPointer");
	AudioHandleToPointerMap::iterator l_pos;

	boost::mutex::scoped_lock lock( m_criticalSection);
	if ((l_pos = m_hndleAudioPtrMap.find(a_handle)) == m_hndleAudioPtrMap.end())
	{
		CLogger::Instance()->Log(LOG_LEVEL_WARNING, *this, "audio handle ptr not found");
		return NULL;
	}
	else
		return l_pos->second;
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Cleanup
// Input - None
//         
// Output - success
//          Cleans up map and notifies observers when complete
///////////////////////////////////////////////////////////////////////////
int CMrcpHandleMgr::Cleanup()
{
	Name("CleanUp");
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Entering ");

	HandleToPointerMap::iterator l_pos;

	NotifyObservers(SESSIONS_STOPPED);
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Exiting ");
	
	return MrcpSuccess;
}

}// end namespace