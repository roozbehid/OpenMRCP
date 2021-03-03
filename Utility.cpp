///////////////////////////////////////////////////////////////////////////////
//
// INTEL CONFIDENTIAL
//
// Copyright 2006 Intel Corporation. All Rights Reserved.
//
// The source code contained or described herein and all documents related to
// the source code ("Material") are owned by Intel Corporation or its suppliers
// or licensors. Title to the Material remains with Intel Corporation or its
// suppliers and licensors. The Material contains trade secrets and proprietary
// and confidential information of Intel or its suppliers and licensors. The
// Material is protected by worldwide copyright and trade secret laws and
// treaty provisions. No part of the Material may be used, copied, reproduced,
// modified, published, uploaded, posted, transmitted, distributed, or
// disclosed in any way without Intel’s prior express written permission.
//
// No license under any patent, copyright, trade secret or other intellectual
// property right is granted to or conferred upon you by disclosure or delivery
// of the Materials, either expressly, by implication, inducement, estoppel or
// otherwise. Any license under such intellectual property rights must be
// express and approved by Intel in writing.
//
///////////////////////////////////////////////////////////////////////////////
// File Name: Utility.cpp
//
// Description: Implementation of Utility functions
//
///////////////////////////////////////////////////////////////////////////////
#include "Utility.h"

#include <signal.h>

#ifdef _WIN32

#include <windows.h>

#else

#include <unistd.h>
#include <pthread.h>

#endif

///////////////////////////////////////////////////////////////////////////////
// Function: void ::MsSleep(unsigned int a_millisecs)
//
// Description: Operating System independent sleep function
//
// Return: void
//
// Parameters: unsigned int a_millisecs
//
///////////////////////////////////////////////////////////////////////////////
void MsSleep( unsigned int a_millisecs) 
{                       

#ifdef _WIN32

   Sleep( a_millisecs);     

#else

   usleep( a_millisecs * 1000);     

#endif
}

///////////////////////////////////////////////////////////////////////////////
// Function: unsigned long ::ThreadID()
//
// Description: Operating System independent thread ID accessor
//
// Return: unsigned long
//
// Parameters: none
//
///////////////////////////////////////////////////////////////////////////////
unsigned long ThreadID()
{
#ifdef _WIN32
   return GetCurrentThreadId();
#else
   return pthread_self();
#endif

}

/* Gobal */
bool g_ctrlCIntercepted = false;

#ifdef _WIN32

///////////////////////////////////////////////////////////////////////////////
// Function: void ::GotCtrlC(int)
//
// Description: Operating System independent Ctrl-C handler
//
// Return: void
//
// Parameters: int
//
///////////////////////////////////////////////////////////////////////////////
void GotCtrlC( int)

#else

void GotCtrlC( int )

#endif

{
   g_ctrlCIntercepted = true; 
}

///////////////////////////////////////////////////////////////////////////////
// Function: void ::EnableCtrlCHandler(void)
//
// Description: 
//
// Return: void
//
// Parameters: void
//
///////////////////////////////////////////////////////////////////////////////
void EnableCtrlCHandler( void)
{

#ifdef _WIN32

   signal( SIGINT, (void (__cdecl*)(int)) GotCtrlC);
   signal( SIGTERM, (void (__cdecl*)(int)) GotCtrlC);

#else

   signal( SIGHUP,  GotCtrlC);
   signal( SIGQUIT, GotCtrlC);
   signal( SIGINT, GotCtrlC);
   signal( SIGTERM, GotCtrlC);

#endif
}

///////////////////////////////////////////////////////////////////////////////
// Function: void ::MakeMailboxPath(std::string &a_mailboxFilePath, const std::string &a_mediaPath, const std::string &a_mailbox, bool a_videoIsActive)
//
// Description: Operating System independent Mailbox path composer
//
// Return: void
//
// Parameters: std::string &a_mailboxFilePath
//             const std::string &a_mediaPath
//             const std::string &a_mailbox
//             bool a_videoIsActive
//
///////////////////////////////////////////////////////////////////////////////
void MakeMailboxPath(   std::string& a_mailboxFilePath, 
                        const std::string& a_mediaPath, 
                        const std::string& a_mailbox, 
                        bool a_videoIsActive)
{

#ifdef _WIN32

   a_mailboxFilePath = a_mediaPath + "\\Mailbox_" + a_mailbox;

#else

   a_mailboxFilePath = a_mediaPath + "/Mailbox_" + a_mailbox;

#endif

   if ( a_videoIsActive)
   {
      a_mailboxFilePath += ".vid";
   }
   else
   {
      a_mailboxFilePath += ".wav";
   }
}

