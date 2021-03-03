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
//	File Name:		CMrcpTaskProcessor.cpp
//
//	Description:	 CMrcpTaskProcessor class implementation
//				  Provides the command and callback queue capabilities
//                Marshalls commands sent and events received
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpTaskProcessor::~CMrcpTaskProcessor();
//	    int CMrcpTaskProcessor::Start();
//	    int CMrcpTaskProcessor::Stop();
//	    int CMrcpTaskProcessor::QueueTask( MrcpTasks* a_mrcpTasks); 
//	    static CMrcpTaskProcessor* CMrcpTaskProcessor::Instance(std::string a_idString = "TASK");
//
///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  3/3/21		Roozbeh G	Boost removal
//
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpTaskProcessor.h"
#include "MrcpUtils.h"
#include "MrcpTasks.h"
#include "MrcpClientEvents.h"

#include <queue>
#include <map>

//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
//#include <boost/thread/condition.hpp>
//#include <boost/bind.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace MrcpV2RefLib
{
   CMrcpTaskProcessor::CMrcpTaskProcessorByStringMap CMrcpTaskProcessor::m_taskProcessors;
   std::mutex CMrcpTaskProcessor::m_singleton;

////////////////////////////////////////////////////////////////////////// 
//
// Description - Constructor
// Input - string containing ID for instance
//         
// Output - None
//          string identifies instance as either callback or task processor
///////////////////////////////////////////////////////////////////////////

CMrcpTaskProcessor::CMrcpTaskProcessor(std::string a_idString) 
   :m_eventProcessorRunning(false)
   ,m_idString(a_idString)
   ,m_name("CMrcpTaskProcessor")
{
	ClassName("CMrcpTaskProcessor");
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Destructor
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////

CMrcpTaskProcessor::~CMrcpTaskProcessor() 
{
	CMrcpTaskProcessorByStringMap::iterator l_pos;

	if ((l_pos = m_taskProcessors.find(m_idString)) != m_taskProcessors.end())
	{
		std::lock_guard<std::mutex> l_lock( m_singleton);
		m_taskProcessors.erase(m_idString);
	}
}
////////////////////////////////////////////////////////////////////////// 
//
// Description - Instance
// Input - string - id string identifies the instance
//         
// Output - returns pointer to task processor instance
//			Instantiates a task processir class by ID or returns pointer
//		    if object already instantiated
///////////////////////////////////////////////////////////////////////////
CMrcpTaskProcessor* CMrcpTaskProcessor::Instance(std::string a_idString)
{
	CMrcpTaskProcessorByStringMap::iterator l_pos;
	CMrcpTaskProcessor* l_taskProcessor;

	if ((l_pos = m_taskProcessors.find(a_idString)) == m_taskProcessors.end())
	{
		std::lock_guard<std::mutex> l_lock( m_singleton);
		if ((l_pos = m_taskProcessors.find(a_idString)) == m_taskProcessors.end())
		{
			l_taskProcessor = new CMrcpTaskProcessor(a_idString);
			m_taskProcessors.insert(std::make_pair( a_idString, l_taskProcessor));
			if ((l_pos = m_taskProcessors.find(a_idString)) == m_taskProcessors.end())
			{
			    std::string l_errorInfo = "Error creating Event Processor for: " + a_idString;
    			CLogger::Instance()->Log( LOG_LEVEL_ERROR, l_errorInfo);
				return NULL;
			}
		}
	}
	return l_pos->second;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Start
// Input - None
//         
// Output - success or failure
//          starts a thread that will process entries added to the queue
///////////////////////////////////////////////////////////////////////////

int CMrcpTaskProcessor::Start()
{
	Name("Start");

   { // scope for lock
      std::lock_guard<std::mutex> l_controlLock( m_controlMutex);

		if ( EventProcessorRunning())
		{
		    std::string l_statInfo = "Error starting task processor: " + m_idString;
			CLogger::Instance()->Log( LOG_LEVEL_ERROR, *this, l_statInfo);
			return -1;
		}
	    EventProcessorRunning(true);
		m_thread = std::shared_ptr<std::thread> (new std::thread( std::bind( &CMrcpTaskProcessor::ProcessTasks, this)));
   }
   while (!EventProcessorRunning())
   {
	   MrcpUtils::MsSleep(100);
   }

   return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - Stop
// Input - None
//         
// Output - Success or failure
//          signals the processing thread to shutdown
///////////////////////////////////////////////////////////////////////////

int CMrcpTaskProcessor::Stop()
{
	  Name("Stop");

   { // scope for lock
      std::lock_guard<std::mutex> l_controlLock( m_controlMutex);
		if ( !EventProcessorRunning() )
		{
	         std::string l_statusInfo = "Error Stopping Task Processor: " + m_idString;
	         CLogger::Instance()->Log( LOG_LEVEL_ERROR, *this, l_statusInfo);
	         return -1;
		}
        EventProcessorRunning(false);
		m_semaphore.notify_one();
        m_thread->join();
   }

   return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - QueueTask
// Input - task pointer
//         
// Output - success or fail
//          places a command or event in the queue and signals the processing
//		    thread.
///////////////////////////////////////////////////////////////////////////

int CMrcpTaskProcessor::QueueTask( MrcpTasks* a_mrcpTask) 
{ 
	Name("QueueTask");
   if ( EventProcessorRunning())
   {
      { // scope for lock
         std::lock_guard<std::mutex> l_lock( m_queueMutex);
         m_queue.push( a_mrcpTask);
      }
      m_semaphore.notify_one();
      return MrcpSuccess;
   }
   else
   {
	   CLogger::Instance()->Log( LOG_LEVEL_ERROR, *this, "Processor not running");
	   return -1;
   }
}

////////////////////////////////////////////////////////////////////////// 
//
// Description - ProcessTasks
// Input - None
//         
// Output - None
//          processes all command or event objects queued.  Calls their execute
//			methods for object processing
///////////////////////////////////////////////////////////////////////////

void CMrcpTaskProcessor::ProcessTasks()
{
   Name("ProcessTasks");
	MrcpTasks* l_task;
   std::unique_lock<std::mutex> semaphoreLock( m_semaphoreMutex);

   std::string l_statusInfo = "Entering Processing Thread: " + m_idString;
   CLogger::Instance()->Log( LOG_LEVEL_INFO, *this, l_statusInfo);

   { // scope for lock
      std::lock_guard<std::mutex> l_controlLock( m_controlMutex);
      EventProcessorRunning(true);
   }
   while( EventProcessorRunning())
   {
      if ( m_queue.empty())
      {
         m_semaphore.wait(semaphoreLock);
      }

      {  // scope for lock
         std::lock_guard<std::mutex> l_queueLock( m_queueMutex);
         if ( !m_queue.empty ())
         {
            l_task = m_queue.front();
            m_queue.pop();
         }
         else
         {
            continue;
         }
      }

      try
      {
         l_task->Execute();
      }
      catch(...)
      {
		 std::string l_debugInfo = "Caught Exception! debug info: " + AsString( l_task->m_signalObj);
         CLogger::Instance()->Log( LOG_LEVEL_ERROR, *this, "ProcessTasks", l_debugInfo);
      }
      delete l_task;
   }
   std::string l_stat2Info = "Exiting Processing Thread: " + m_idString;
   CLogger::Instance()->Log( LOG_LEVEL_INFO, *this, l_stat2Info);

  

}
}//end namespace