/***********************************************************************
    filename:   XMLParserModule.h
    created:    Fri Sep 02 2016
    author:     Vladimir "Niello" Orlov
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2016 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUITinyXML2ParserModule_h_
#define _CEGUITinyXML2ParserModule_h_

#include "CEGUI/XMLParserModules/TinyXML2/XMLParser.h"

/*!
\brief
    exported function that creates an XMLParser based object and returns
    a pointer to that object.
*/
extern "C" CEGUITINYXML2PARSER_API CEGUI::XMLParser* createParser(void);

/*!
\brief
    exported function that deletes an XMLParser based object previously
    created by this module.
*/
extern "C" CEGUITINYXML2PARSER_API void destroyParser(CEGUI::XMLParser* parser);

#endif // end of guard _CEGUITinyXMLParserModule_h_
