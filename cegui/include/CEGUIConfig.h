/***********************************************************************
	filename: 	CEGUIConfig.h
	created:	1/10/2004
	author:		Paul D Turner
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
// The following are intended for X-Code users.
// 
// The Linux build can both auto-configure itself, and also take
// configure parameters which control the settings presented below.
//
// The Premake solution will generate these for you according to the
// config.lua script.
//////////////////////////////////////////////////////////////////////////
#if defined(__APPLE__)

//////////////////////////////////////////////////////////////////////////
// Set this to the default XMLParser to be used.
// (XercesParser, ExpatParser, LibxmlParser or TinyXMLParser).
//////////////////////////////////////////////////////////////////////////
#ifndef CEGUI_DEFAULT_XMLPARSER
#	define CEGUI_DEFAULT_XMLPARSER ExpatParser
#endif

//////////////////////////////////////////////////////////////////////////
// Set this to the default ImageCodec to be used.
// CoronaImageCodec, DevILImageCodec, FreeImageImageCodec,
// SILLYImageCodec, TGAImageCodec 
//////////////////////////////////////////////////////////////////////////
#ifndef CEGUI_DEFAULT_IMAGE_CODEC
#   define CEGUI_DEFAULT_IMAGE_CODEC SILLYImageCodec
#endif

//////////////////////////////////////////////////////////////////////////
// The following are required to build the integrated copy of TinyXML.
// If for some reason you decide you need to use an external version of
// TinyXML, you can change these defines as required (you will also likely
// need to set up some linker stuff as well).
//////////////////////////////////////////////////////////////////////////
// Main tiny xml header file
#ifndef CEGUI_TINYXML_H
#   define CEGUI_TINYXML_H "ceguitinyxml/tinyxml.h"
#endif
// Namespace that contains TinyXML.
#ifndef CEGUI_TINYXML_NAMESPACE
#   define CEGUI_TINYXML_NAMESPACE CEGUITinyXML
#endif

//////////////////////////////////////////////////////////////////////////
// The following controls the version of Lua that is going to be used.
// Note that from 0.7.0 and up, Lua 5.0 is no longer supported.
// 51 is for Lua 5.1.x versions (and above?)
//////////////////////////////////////////////////////////////////////////
#ifndef CEGUI_LUA_VER
#   define CEGUI_LUA_VER 51
#endif

//////////////////////////////////////////////////////////////////////////
// The following controls whether freetype based font support will be
// compiled in (default).  If you want to build CEGUI /without/ freetype
// support, comment the following line.
//////////////////////////////////////////////////////////////////////////
#define CEGUI_HAS_FREETYPE

//////////////////////////////////////////////////////////////////////////
// PCRE library
// CEGUI uses the pcre library for it's regular expression based string
// validation as used in the Editbox (and derived classes, such as Spinner).
// To disable the use of PCRE (and therefore the validation factilities),
// comment the following line.  (Attempts to set validation string will throw).
//////////////////////////////////////////////////////////////////////////
#define CEGUI_HAS_PCRE_REGEX

//////////////////////////////////////////////////////////////////////////
// The following controls whether the default logger be (possibly) used.
// If you want to build CEGUI so that it does not use the default logger,
// comment the following line.
//
// Note: If you disable the default logger you MUST provide an alternative
// CEGUI::Logger based class and instantiate it before creating the main
// CEGUI::System object.
//////////////////////////////////////////////////////////////////////////
#define CEGUI_HAS_DEFAULT_LOGGER

//////////////////////////////////////////////////////////////////////////
// The following defines control bidirectional text support.
//
// Uncomment the CEGUI_BIDI_SUPPORT definition to enable bidirectional
// text in CEGUI.
//
// With bidirectional text support enabled, you must then choose to uncomment
// either the CEGUI_USE_MINIBIDI or CEGUI_USE_FRIBIDI definitions (but you
// should not uncomment both!)
//
// Uncommenting CEGUI_USE_MINIBIDI uses an integrated copy of minibidi to
// provide the bidirectional support.
//
// Uncommenting CEGUI_USE_FRIBIDI uses an external copy of fribidi to
// provide the bidirectional support (not supplied).
//////////////////////////////////////////////////////////////////////////
//#define CEGUI_BIDI_SUPPORT

// Uncomment this to use the embedded minibidi library.
//#define CEGUI_USE_MINIBIDI
// - or -
// Uncomment this to use an external fribidi library.
//#define CEGUI_USE_FRIBIDI

#endif // defined(__APPLE__)

#endif	// end of guard _CEGUIConfig_h_
