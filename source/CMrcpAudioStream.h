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
//	File Name:		CMrcpAudioStream.h
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
//  4/1/07      TMB         Stubbed out statics for outbound audio
///////////////////////////////////////////////////////////////////////////////
#ifndef CMrcpAudioStream_H
#define CMrcpAudioStream_H

#include "Clogger.h"
#include "ClientInterfaceDefs.h"

#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <list>
#include <queue>


typedef enum 
{
   AUDIO_STARTING,
   AUDIO_STARTED,   
   AUDIO_STOPPING
}AudioStateEnum;

namespace MrcpV2RefLib
{
	class CMrcpRtp;
	class CAudioContainer;
	class CMrcpAudioStream
	{
		friend class CMrcpRtp;
	public:
		CMrcpAudioStream(CMrcpRtp* l_rtpPtr);
		virtual ~CMrcpAudioStream();
		int AddToInboundAudioStream(std::string a_buuferToAdd);
		int AddToOutboundAudioStream(std::string a_buuferToAdd);

		int Stop();
		int Start();

		const char* StateDesc() { return "N/A";}		
		const std::string& ClassName(){return m_className;};
		void ClassName(const std::string& a_className) {m_className = a_className;};
		const std::string& Name(){return m_name;};
		void Name( const std::string& a_name){m_name = a_name;};
		LogObjectTypeEnum LogObjectType(){return LogObject_System;};

	protected:
	private:

		RESOURCE_CFG_STRUCT* m_resourceCfg;
		void ManageOutboundAudioStream();
		AudioStateEnum m_state;
		std::list<std::string> m_audioStreamIn;
		boost::mutex m_bufferOutboundAudioMutex;

		static std::queue<CAudioContainer*> m_bufferQueue;	
		static boost::mutex m_bufferOutboundQueueMutex;
		static boost::mutex m_bufferOutboundQueueConditionMutex;
		static boost::mutex m_staticParamSetMutex;
		static boost::condition m_bufferOutboundQueueCondition;
	//	static boost::shared_ptr<boost::thread> CMrcpAudioStream::m_bufferOutboundQueueProcessingThread;
		static bool m_mangeBufferOutboundQueueFlag;
		static int m_activeThreads;


		MrcpAudioHandle m_audioHandle;

		struct ManageOutboundQueueFunction 
		{
			ManageOutboundQueueFunction(CMrcpAudioStream *a_buffer)
				:m_buffer(a_buffer)
			{
			};
			void operator()()
			{
				m_buffer->ManageOutboundAudioStream();
			}
			CMrcpAudioStream* m_buffer;
		};

		CMrcpRtp* m_rtpObj;

		boost::mutex m_controlMutex;
		std::string m_inboundAudioBuffer;
		std::string m_className;
	    std::string m_name;
		int m_inboundReferenceNumber;
	
	};
}//end namespace
#endif