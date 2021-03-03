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
//	File Name:		CMrcpAudioStream.cpp
//
//	Description:	CMrcpAudioStream class implementation
//
//	Declared in this file:
//		none
//
//	Implemented in this file:
//		CMrcpAudioStream::CMrcpAudioStream(CMrcpRtp* a_rtpObj)
//		CMrcpAudioStream::~CMrcpAudioStream()
//		int CMrcpAudioStream::Start()
//		int CMrcpAudioStream::Stop()
//		int CMrcpAudioStream::AddToOutboundAudioStream(std::string a_buffer)
//		int CMrcpAudioStream::AddToInboundAudioStream(std::string a_buffer)
//		void CMrcpAudioStream::ManageOutboundAudioStream()

///////////////////////////////////////////////////////////////////////////////
//	Revisions:
//	Date		Initial 	Description
//	----		------- 	-----------
//	6/21/06 	TMB 		Initial Version
//  1/2/07      TMB         Modified to return handles in callback structure
//  4/1/07      TMB         Stubbed out static thread for outbound audio - should
//					        revisit
///////////////////////////////////////////////////////////////////////////////

#include "CMrcpRtp.h"
#include "CMrcpAudioStream.h"
#include "CMrcpCallbackTask.h"
#include "CMrcpTaskProcessor.h"
#include "CMrcpCmdIdMgr.h"
#include "ClientInterfaceDefs.h"
#include "CAudioContainer.h"

namespace MrcpV2RefLib
{
	std::queue<CAudioContainer*> CMrcpAudioStream::m_bufferQueue;
	boost::mutex CMrcpAudioStream::m_bufferOutboundQueueMutex;
	boost::mutex CMrcpAudioStream::m_bufferOutboundQueueConditionMutex;
	boost::mutex CMrcpAudioStream::m_staticParamSetMutex;
	boost::condition CMrcpAudioStream::m_bufferOutboundQueueCondition;
//	boost::shared_ptr<boost::thread> CMrcpAudioStream::m_bufferOutboundQueueProcessingThread;
	bool CMrcpAudioStream::m_mangeBufferOutboundQueueFlag = false;
	int CMrcpAudioStream::m_activeThreads = 0;

////////////////////////////////////////////////////////////////////////// 
// Description - Constructor 
// Input - pointer to related rtp object
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpAudioStream::CMrcpAudioStream(CMrcpRtp* a_rtpObj)
	:m_rtpObj(a_rtpObj)

{
	m_state = AUDIO_STARTING;
	ClassName("CMrcpAudioStream");
	m_resourceCfg = a_rtpObj->m_resourceCfg;

}
////////////////////////////////////////////////////////////////////////// 
// Description - Destructor 
// Input - None
//         
// Output - None
//          
///////////////////////////////////////////////////////////////////////////
CMrcpAudioStream::~CMrcpAudioStream()
{
}
////////////////////////////////////////////////////////////////////////// 
// Description - Start 
// Input - None
//         
// Output - None
//          start sets up the object for audio processing
///////////////////////////////////////////////////////////////////////////
int CMrcpAudioStream::Start()
{
	Name("Start");
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Entering");

/*	if (!m_bufferOutboundQueueProcessingThread)
	{
		{
			boost::mutex::scoped_lock l_controlLock(m_staticParamSetMutex );	
			m_mangeBufferOutboundQueueFlag = true;
			m_bufferOutboundQueueProcessingThread = boost::shared_ptr<boost::thread>(new boost::thread(ManageOutboundQueueFunction(this)));
		}
	}
	*/
	{
		boost::mutex::scoped_lock l_controlLock(m_staticParamSetMutex );
		m_activeThreads++;
	}

	m_state = AUDIO_STARTED;
	m_audioStreamIn.clear();

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - Stop 
// Input - None
//         
// Output - None
//         stop shutdowns the audio stream object 
///////////////////////////////////////////////////////////////////////////
int CMrcpAudioStream::Stop()
{
	Name("Stop");
	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Entering");
	m_state = AUDIO_STOPPING;
	if(m_state == AUDIO_STOPPING && m_activeThreads == 1)
	{
		{
		boost::mutex::scoped_lock l_controlLock(m_bufferOutboundQueueMutex );
	/*		m_mangeBufferOutboundQueueFlag = false;
			m_bufferOutboundQueueCondition.notify_one();
			m_bufferOutboundQueueProcessingThread->join();
			m_bufferOutboundQueueProcessingThread.reset();
	*/
			m_audioStreamIn.clear();
		}
	}
	{
		boost::mutex::scoped_lock l_controlLock(m_staticParamSetMutex );
		m_activeThreads--;
	}

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this, "Exiting");

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - AddToInboundAudioStream 
// Input - string containing and audio buffer
//         
// Output - None
//          Prefred method is callbacks to send audio to client application
// 
///////////////////////////////////////////////////////////////////////////
int CMrcpAudioStream::AddToInboundAudioStream(std::string a_buffer)
{
	Name("AddToInboundAudioStream");
	m_inboundAudioBuffer = a_buffer;
	MRCP_RESULT_STRUCT l_callResults;

	
	if (m_state != AUDIO_STARTED)
	{
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this, "Bad State -");
		return -1;
	}
//the following differs from other calls to CMrcpCmdIdMgr::PerformCallbackProcessing for
//Performance reasons.

	CMrcpCallbackTask* l_callbackTask = new CMrcpCallbackTask();
	l_callbackTask->CallBackRtn(m_resourceCfg->audioCallback);
	strcpy(l_callResults.MrcpReturnPkt,a_buffer.c_str());
	l_callResults.MrcpReturnValue = 0;
	l_callResults.referenceNumber = 0;
	l_callResults.asrSessionHandle = m_resourceCfg->asrSessionHandle;
	l_callResults.ttsSessionHandle = m_resourceCfg->ttsSessionHandle;
	l_callbackTask->CallResults(l_callResults);
	CMrcpTaskProcessor::Instance("CALLBACK")->QueueTask(l_callbackTask);

	return MrcpSuccess;
}
////////////////////////////////////////////////////////////////////////// 
// Description - AddToOutboundAudioStream 
// Input - string containing an audio buffer
//         
// Output - None
//        called by the client application to add audio streams for processing
//      NOTE - queuing both audio and a pointer to object
///////////////////////////////////////////////////////////////////////////
int CMrcpAudioStream::AddToOutboundAudioStream(std::string a_buffer)
{
	Name("AddToOutboundAudioStream");
	bool isStreamEmpty = true;
//	CAudioContainer* l_audioContainer;

	if (m_state != AUDIO_STARTED)
	{
		CLogger::Instance()->Log(LOG_LEVEL_ERROR, *this,"Bad state - exiting");
		return -1;
	}

	m_rtpObj->ProcessOutboundRtp(a_buffer);
	/*
	l_audioContainer = new CAudioContainer;
	l_audioContainer->m_audioBuffer = a_buffer;
	l_audioContainer->m_rtpObj = m_rtpObj; 

	{ //scope for lock
	boost::mutex::scoped_lock l_controlLock( m_bufferOutboundQueueMutex);
		isStreamEmpty = m_bufferQueue.empty();
		m_bufferQueue.push(l_audioContainer);
	}
	
	
	if (!isStreamEmpty)
	{
		m_bufferOutboundQueueCondition.notify_one();
	}

*/
	return MrcpSuccess;
}

////////////////////////////////////////////////////////////////////////// 
// Description - ManageOutboundAudioStream 
// Input - None
//         
// Output - None
//       Intention is to have this thread of execution handle the audio for
//	     multiple streams
///////////////////////////////////////////////////////////////////////////
void CMrcpAudioStream::ManageOutboundAudioStream()
{
	Name("ManageOutboundAudioStream");
	std::string l_audioBuffer;

	while (m_mangeBufferOutboundQueueFlag)
	{
		bool isStreamEmpty;
		{ //scope for lock
			boost::mutex::scoped_lock lock(m_bufferOutboundQueueMutex);
			isStreamEmpty = m_bufferQueue.empty(); 
		}
		if (isStreamEmpty)
		{
			boost::mutex::scoped_lock lock(m_bufferOutboundQueueConditionMutex);
			m_bufferOutboundQueueCondition.wait(lock);
			if (!m_mangeBufferOutboundQueueFlag)
			{
				break;
			}
		}
		CAudioContainer* l_audio;
		do
		{
				{
				boost::mutex::scoped_lock lock(m_bufferOutboundQueueMutex);
					l_audio = m_bufferQueue.front();
					m_bufferQueue.pop();
					isStreamEmpty = m_bufferQueue.empty();
				}

				try
				{
					l_audio->m_rtpObj->ProcessOutboundRtp(l_audio->m_audioBuffer);
				}
				catch(...)
				{
					CLogger::Instance()->Log( LOG_LEVEL_INFO, *this, "ManageOutboundAudioStream", "exception outbound audio");
				}
			delete l_audio;
		} while(!isStreamEmpty );
	}

	CLogger::Instance()->Log(LOG_LEVEL_INFO, *this,"Exiting outbound audio**");

}

}//end namespace