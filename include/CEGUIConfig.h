/************************************************************************
	filename: 	CEGUIConfig.h
	created:	1/10/2004
	author:		Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
when compiling Crazy Eddie's GUI System and associated components.

Each item in here has a comment to describe what it's for.

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
#ifndef _CEGUIConfig_h_
#define _CEGUIConfig_h_


//////////////////////////////////////////////////////////////////////////
// Uncomment this line if you are using the 0.14.x or earlier release of Ogre.
//////////////////////////////////////////////////////////////////////////
//#define CEGUI_USEOLDOGRESTRING 1


//////////////////////////////////////////////////////////////////////////
// comment this line to suppress the auto appending of '_d' to loaded module
// names under Win32.
//////////////////////////////////////////////////////////////////////////
#define CEGUI_LOAD_MODULE_APPEND_SUFFIX_FOR_DEBUG 1


//////////////////////////////////////////////////////////////////////////
// this is what gets appended to module names when 
// CEGUI_LOAD_MODULE_APPEND_FOR_DEBUG is defined
//////////////////////////////////////////////////////////////////////////
#define CEGUI_LOAD_MODULE_DEBUG_SUFFIX	"_d"


//////////////////////////////////////////////////////////////////////////
// Comment this line to remove the alignment of elements to pixel
// boundaries.  This may give you a performance boost at the expense
// of visual quality
//////////////////////////////////////////////////////////////////////////
#define CEGUI_ALIGN_ELEMENTS_TO_PIXELS 1


#endif	// end of guard _CEGUIConfig_h_
