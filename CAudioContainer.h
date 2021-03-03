///////////////////////////////////////////////////////////////////////////////
//
// Dialogic CONFIDENTIAL
//
// Copyright 2006 Dialogic Corporation. All Rights Reserved.
//
// The source code contained or described herein and all documents related to
// the source code ("Material") are owned by Dialogic Corporation or its suppliers
// or licensors. Title to the Material remains with Dialogic Corporation or its
// suppliers and licensors. The Material contains trade secrets and proprietary
// and confidential information of Dialogic or its suppliers and licensors. The
// Material is protected by worldwide copyright and trade secret laws and
// treaty provisions. No part of the Material may be used, copied, reproduced,
// modified, published, uploaded, posted, transmitted, distributed, or
// disclosed in any way without Dialogic’s prior express written permission.
//
// No license under any patent, copyright, trade secret or other Dialogiclectual
// property right is granted to or conferred upon you by disclosure or delivery
// of the Materials, either expressly, by implication, inducement, estoppel or
// otherwise. Any license under such Dialogiclectual property rights must be
// express and approved by Dialogic in writing.
//
///////////////////////////////////////////////////////////////////////////////
// File Name: CAudioContainer.h
//
// Description: Common type definitions
//
///////////////////////////////////////////////////////////////////////////////
#ifndef CAudioContainer_h
#define CAudioContainer_h

#include <string>

namespace MrcpV2RefLib
{
	class CMrcpRtp;

	class CAudioContainer
	{
	public:
		std::string m_audioBuffer;
		CMrcpRtp* m_rtpObj;

	};

}

#endif 