/***********************************************************************
	filename: 	CEGUIBase.h
	created:	20/2/2004
	author:		Paul D Turner

	purpose:	Base include used within the system
				This contains various lower level bits required
				by other parts of the system.  All other library
				headers will include this file.
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _CEGUIBase_h_
#define _CEGUIBase_h_

#include <cassert>

// bring in configuration options
#include "CEGUIConfig.h"

// add CEGUI version defines
#include "CEGUIVersion.h"

/*************************************************************************
	Dynamic Library import / export control conditional
	(Define CEGUIBASE_EXPORTS to export symbols, else they are imported)
*************************************************************************/
#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef CEGUIBASE_EXPORTS
#       define CEGUIEXPORT __declspec(dllexport)
#   else
#       define CEGUIEXPORT __declspec(dllimport)
#   endif
#       define CEGUIPRIVATE
#else
#       define CEGUIEXPORT
#       define CEGUIPRIVATE
#endif


// totally kill this warning (debug info truncated to 255 chars etc...) on <= VC6
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#   pragma warning(disable : 4786)
#endif


// Detect macros for min / max and undefine (with a warning where possible)
#if defined(max)
#   if defined(_MSC_VER)
#       pragma message("Macro definition of max detected - undefining")
#   elif defined (__GNUC__)
#       warning ("Macro definition of max detected - undefining")
#   endif
#   undef max
#endif
#if defined(min)
#   if defined(_MSC_VER)
#       pragma message("Macro definition of min detected - undefining")
#   elif defined (__GNUC__)
#       warning ("Macro definition of min detected - undefining")
#   endif
#   undef min
#endif


// include this to see if it defines _STLPORT_VERION
#	include <string>

// fix to undefine _STLP_DEBUG if STLport is not actually being used
// (resolves some unresolved externals concerning boost)
#if defined(_STLP_DEBUG) && defined(_MSC_VER) && (_MSC_VER >= 1200)
#	if !defined(_STLPORT_VERSION)
#		undef _STLP_DEBUG
#	endif
#endif


// The following defines macros used within CEGUI for std::min/std::max
// usage, and is done as a compatibility measure for VC6 with native STL.
#if defined(_MSC_VER) && (_MSC_VER <= 1200) && !defined(_STLPORT_VERSION)
#    define ceguimin	std::_cpp_min
#    define ceguimax	std::_cpp_max
#else
#    define ceguimin	std::min
#    define ceguimax	std::max
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

typedef unsigned int    uint32;
typedef unsigned short  uint16;
typedef unsigned char   uint8;


/*************************************************************************
	System wide constants
*************************************************************************/
static const float		DefaultNativeHorzRes	= 640.0f;		//!< Default native horizontal resolution (for fonts and imagesets)
static const float		DefaultNativeVertRes	= 480.0f;		//!< Default native vertical resolution (for fonts and imagesets)


/*************************************************************************
    Additional typedefs
*************************************************************************/
typedef std::ostream OutStream;     //!< Output stream class.
}  // end of CEGUI namespace section


//////////////////////////////////////////////////////////////////////////
// Comment this line to remove the alignment of elements to pixel
// boundaries.  This may give you a performance boost at the expense
// of visual quality
//////////////////////////////////////////////////////////////////////////
#define CEGUI_ALIGN_ELEMENTS_TO_PIXELS 1

/*!
\brief
	Macro used to return a float value rounded to the nearest integer.

	This macro is used throughout the library to ensure that elements are
	kept at integer pixel positions on the display.

\param x
	Expression to be rounded to nearest whole number

\return
	\a x after having been rounded
*/
#if defined(CEGUI_ALIGN_ELEMENTS_TO_PIXELS)
#	define PixelAligned(x)	( (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) )
#else
#	define PixelAligned(x)	( x )
#endif


/*************************************************************************
	Bring in forward references to all GUI base system classes
*************************************************************************/
#include "CEGUIForwardRefs.h"


#endif	// end of guard _CEGUIBase_h_
