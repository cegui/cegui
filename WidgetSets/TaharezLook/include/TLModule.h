/************************************************************************
	filename: 	TLModule.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	System <-> GUI Module Interface
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
#ifndef _TLModule_h_
#define _TLModule_h_

#include "CEGUIString.h"

/*************************************************************************
	Import / Export control macros
*************************************************************************/
#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef TAHAREZLOOK_EXPORTS
#       define TAHAREZLOOK_API __declspec(dllexport)
#   else
#       define TAHAREZLOOK_API __declspec(dllimport)
#   endif
#else
#   define TAHAREZLOOK_API
#endif

/*************************************************************************
	Prototypes
*************************************************************************/
extern "C" TAHAREZLOOK_API void registerFactory(const CEGUI::String& type_name);
extern "C" TAHAREZLOOK_API CEGUI::uint registerAllFactories(void);

#endif	// end of guard _TLModule_h_
