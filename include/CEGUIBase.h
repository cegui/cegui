/************************************************************************
	filename: 	CEGUIBase.h
	created:	20/2/2004
	author:		Paul D Turner
	
	purpose:	Base include used within the system
				This contains various lower level bits required
				by other parts of the system.  All other library 
				headers will include this file.
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
#ifndef _CEGUIBase_h_
#define _CEGUIBase_h_

// bring in configuration options
#include "CEGUIConfig.h"


/*************************************************************************
	Dynamic Library import / export control conditional
	(Define CEGUIBASE_EXPORTS to export symbols, else they are imported)
*************************************************************************/
#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUIBASE_EXPORTS
#       define CEGUIBASE_API __declspec(dllexport)
#   else
#       define CEGUIBASE_API __declspec(dllimport)
#   endif
#else
#       define CEGUIBASE_API
#endif


// totally kill this warning (debug info truncated to 255 chars etc...) on <= VC6
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#   pragma warning(disable : 4786)
#endif

// fix to undefine _STLP_DEBUG if STLport is not actually being used
// (resolves some unresolved externals concerning boost)
#if defined(_STLP_DEBUG) && defined(_MSC_VER) && (_MSC_VER >= 1300)
#	include <string>		// include this to see if it defines _STLPORT_VERION
#	if !defined(_STLPORT_VERSION)
#		undef _STLP_DEBUG
#	endif
#endif


/*************************************************************************
	Documentation for the CEGUI namespace itself
*************************************************************************/
/*!
\brief
	Main namespace for Crazy Eddie's GUI Library

	The CEGUI namespace contains all the classes and other items that comprise the core
	of Crazy Eddie's GUI system.
*/
namespace CEGUI
{

/*************************************************************************
	Simplification of some 'unsigned' types
*************************************************************************/
typedef	unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned char	uchar;


/*************************************************************************
	System wide constants
*************************************************************************/
static const float		DefaultNativeHorzRes	= 640.0f;		//!< Default native horizontal resolution (for fonts and imagesets)
static const float		DefaultNativeVertRes	= 480.0f;		//!< Default native vertical resolution (for fonts and imagesets)

}  // end of CEGUI namespace section


/*************************************************************************
	Bring in forward references to all GUI base system classes
*************************************************************************/
#include "CEGUIForwardRefs.h"


#endif	// end of guard _CEGUIBase_h_
