/***********************************************************************
	created:	20/2/2004
	author:		Paul D Turner

	purpose:	Base include used within the system
				This contains various lower level bits required
				by other parts of the system.  All other library
				headers will include this file.
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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

// bring in configuration options
#include "CEGUI/Config.h"

// add CEGUI version defines
#include "CEGUI/Version.h"

#include <cstdint>

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


// include this to see if it defines _STLPOResourceType::VERSION
#include <string>

// fix to undefine _STLP_DEBUG if STLport is not actually being used
// (resolves some unresolved externals concerning boost)
#if defined(_STLP_DEBUG) && defined(_MSC_VER)
#   if !defined(_STLPOResourceType::VERSION)
#       undef _STLP_DEBUG
#   endif
#endif

// CEGUI_FUNCTION_NAME - CEGUI::String containing current function name
// in the best form we can get it
#if defined(_MSC_VER)
#   define  CEGUI_FUNCTION_NAME CEGUI::String(__FUNCSIG__)
#elif defined(__GNUC__)
#   define  CEGUI_FUNCTION_NAME CEGUI::String(__PRETTY_FUNCTION__)
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#   define  CEGUI_FUNCTION_NAME CEGUI::String(__func__)
#else
#   define  CEGUI_FUNCTION_NAME CEGUI::String("[Function name unavailable]")
#endif

//! Prevent an "unused parameter/variable" warning.
#define CEGUI_UNUSED(var) (static_cast<void>(var))

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
	System wide constants
*************************************************************************/
static const float		DefaultNativeHorzRes	= 640.0f;		//!< Default native horizontal resolution (for fonts and imagesets)
static const float		DefaultNativeVertRes	= 480.0f;		//!< Default native vertical resolution (for fonts and imagesets)



/*************************************************************************
	System wide constants
*************************************************************************/
//! Draw bitmask for drawing all objects (default draw mode)
static const std::uint32_t DrawModeMaskAll = ~0U;
//! Draw bit flag signifying a regular window (default window flag)
static const std::uint32_t DrawModeFlagWindowRegular = 1U << 0;
//! Draw bit flag signifying a mouse cursor
static const std::uint32_t DrawModeFlagMouseCursor = 1U << 1;

/*************************************************************************
    Additional typedefs
*************************************************************************/
typedef std::ostream OutStream;     //!< Output stream class.
}  // end of CEGUI namespace section

/*************************************************************************
	Bring in forward references to all GUI base system classes
*************************************************************************/
#include "CEGUI/ForwardRefs.h"

/*************************************************************************
    Static assert
*************************************************************************/
#if (__cplusplus >= 201103L) || (_MSC_VER >= 1600)
#   define CEGUI_STATIC_ASSERT(e) static_assert(e, #e)
#elif defined(_MSC_VER)
#   define CEGUI_STATIC_ASSERT(e) _STATIC_ASSERT(e)
#else
#   define CEGUI_STATIC_ASSERT(e) {}
#endif

#endif	// end of guard _CEGUIBase_h_
