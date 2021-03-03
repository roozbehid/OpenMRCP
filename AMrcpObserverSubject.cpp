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
//	File Name:		AMrcpObserver.cpp
//
//	Description:	class implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//    AMrcpObserverSubject::AMrcpObserverSubject()
//    AMrcpObserverSubject::~AMrcpObserverSubject()
//    void AMrcpObserverSubject::RegisterForNotification(AMrcpEventHandler* a_observer, int a_notificationID)
//    void AMrcpObserverSubject::Unregister(AMrcpEventHandler* a_observer, int a_notificationID)
//    void AMrcpObserverSubject::RemoveObserver(AMrcpEventHandler* a_observerTarget,
//                                              AMrcpTaskHandlerList* a_observerList)
//    void AMrcpObserverSubject::NotifyObservers(int a_notificationID, void* objPtr)
//    
// 
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  4/1/07      TMB         removed RemoveObserver - function in unregister
///////////////////////////////////////////////////////////////////////////////

#include "AMrcpObserverSubject.h"
#include "AMrcpEventHandler.h"
#include "CMrcpEvent.h"


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
 AMrcpObserverSubject::AMrcpObserverSubject()
 {

 }
////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
 AMrcpObserverSubject::~AMrcpObserverSubject()
 {

 }
////////////////////////////////////////////////////////////////////////// 
//
// Description - RegisterForNotification
// Input - pointer to event handler
//         event registering for
// Output - None
//          Objects register themselves by event in order to receive events 
//          broadcast from objects they are observing
///////////////////////////////////////////////////////////////////////////
void AMrcpObserverSubject::RegisterForNotification(AMrcpEventHandler* a_observer, int a_notificationID)
{

    boost::mutex::scoped_lock lock(m_observerMutex);

	AMrcpNotificationListMap::iterator l_observerList;

    l_observerList = m_notifications.find(a_notificationID);
    if (l_observerList == m_notifications.end())
    {
        AMrcpTaskHandlerList* l_newObserverList = new AMrcpTaskHandlerList;
        l_newObserverList->push_back(a_observer);
        m_notifications[a_notificationID] = l_newObserverList;
    }
    else
    {
        m_notifications[a_notificationID]->push_back(a_observer);
    }
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Unregister
// Input - pointer to event handler
//         event unregistering for
// Output - None
//          Objects will unregister for events, this should be done before
//          destructing.
///////////////////////////////////////////////////////////////////////////

void AMrcpObserverSubject::Unregister(AMrcpEventHandler* a_observer, int a_notificationID)
{
	boost::mutex::scoped_lock lock(m_observerMutex);

    AMrcpNotificationListMap::iterator l_observerList;

    if (a_notificationID == ALL_NOTIFICATIONS)
    {
        for ( l_observerList = m_notifications.begin();
            l_observerList != m_notifications.end();
            ++l_observerList)
        {
           RemoveObserver( a_observer, l_observerList->second);
		}
    }
    else
    {
        l_observerList = m_notifications.find(a_notificationID);
        if (l_observerList != m_notifications.end())
        {
           RemoveObserver( a_observer, l_observerList->second);
        }
    }
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - RemoveObserver
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

void AMrcpObserverSubject::RemoveObserver(AMrcpEventHandler* a_observerTarget,
                                         AMrcpTaskHandlerList* a_observerList)
{
    AMrcpTaskHandlerList::iterator l_observer;

    for ( l_observer = a_observerList->begin();
        l_observer != a_observerList->end();
        ++l_observer)
    {
        if ((*l_observer) == a_observerTarget)
        {
            break;
        }
    }
    if (l_observer != a_observerList->end())
    {
        a_observerList->erase(l_observer);
    }
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - NotifyObservers
// Input - ID of event 
//         pointer to object to receive notification
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
void AMrcpObserverSubject::NotifyObservers(int a_notificationID, void* objPtr)
{
    AMrcpNotificationListMap::iterator l_observerList;
    AMrcpEventHandler* l_obj = NULL;

    l_observerList = m_notifications.find(a_notificationID);
    if (l_observerList != m_notifications.end())
    {
        AMrcpTaskHandlerList::iterator l_observer;
        for ( l_observer = (l_observerList->second)->begin();
            l_observer != (l_observerList->second)->end();
            ++l_observer)
        {
            l_obj = (*l_observer);
            if (l_obj != NULL)
            {
				CMrcpEvent* l_mrcpEvent = new CMrcpEvent();
				l_mrcpEvent->EventID(a_notificationID);
				l_mrcpEvent->EventHandler(l_obj);

                l_obj->PostEvent(l_mrcpEvent);
            }
        }
    }
}
}//end namespace