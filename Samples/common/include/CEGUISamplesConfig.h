/************************************************************************
    filename:   CEGUISamplesConfig.h
    created:    Mon Sep 26 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************

This file can be used to set certain configuration options which are used
when compiling the samples for Crazy Eddie's GUI System.

Each item in here has a comment to describe what it's for.

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

#ifndef _CEGUISamplesConfig_h_
#define _CEGUISamplesConfig_h_

// Bring in main config file
#include "CEGUIConfig.h"

//////////////////////////////////////////////////////////////////////////
// The following are intended for MSVC++ and X-Code users.
// 
// The Linux build can both auto-configure itself, and also take
// configure parameters which control the settings presented below.
//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) || defined(__APPLE__)

//////////////////////////////////////////////////////////////////////////
// Comment/Uncomment these lines to control renderer use in samples where
// MSVC/XCode are used (since they don't auto-detect such things).
//////////////////////////////////////////////////////////////////////////
#define CEGUI_SAMPLES_USE_OPENGL
//#define CEGUI_SAMPLES_USE_OGRE
//#define CEGUI_SAMPLES_USE_IRRLICHT

#endif // defined(_MSC_VER) || defined(__APPLE__)


//////////////////////////////////////////////////////////////////////////
// The following are intended for MSVC++ users.
//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)

//////////////////////////////////////////////////////////////////////////
// Comment/Uncomment these lines to control renderer use in samples where
// MSVC is used (since it can't auto-detect such things).
//////////////////////////////////////////////////////////////////////////
//#define CEGUI_SAMPLES_USE_DIRECTX_8
//#define CEGUI_SAMPLES_USE_DIRECTX_9

#endif // defined(_MSC_VER)


#endif  // end of guard _CEGUISamplesConfig_h_
