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
#   define CEGUI_DEFAULT_IMAGE_CODEC TGAImageCodec
#endif 

#endif // defined(__APPLE__)

#endif	// end of guard _CEGUIConfig_h_
