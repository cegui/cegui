/***********************************************************************
filename:   CEGUIFont_xmlHandler.h
created:    21/2/2004
author:     Paul D Turner

purpose:    Handle the basic XML layout for .font files
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
#ifndef _CEGUIFont_xmlHandler_h_
#define _CEGUIFont_xmlHandler_h_

#include "CEGUIXMLHandler.h"

// Start of CEGUI namespace section
namespace CEGUI
{

class Font;

/*!
\brief
Handler class used to parse the Font XML files using SAX2
*/
class Font_xmlHandler : public XMLHandler
{
public:
    /*************************************************************************
    Implementation Data
    *************************************************************************/
    Font *d_font;         //!< Font object that we are helping to build

    /*************************************************************************
    Construction & Destruction
    *************************************************************************/
    /*!
    \brief
        Constructor for Font::xmlHandler objects

    \param font
        Pointer to the Font object creating this xmlHandler object
    */
    Font_xmlHandler() : d_font (0) {}

    /*!
    \brief
    Destructor for Font::xmlHandler objects
    */
    virtual ~Font_xmlHandler(void) {}

    /*************************************************************************
    SAX2 Handler overrides
    *************************************************************************/
    /*!
    \brief
    document processing (only care about elements, schema validates format)
    */
    virtual void elementStart(const String& element, const XMLAttributes& attributes);
    virtual void elementEnd(const String& element);

private:
    /*************************************************************************
    Implementation Constants
    *************************************************************************/

    /*!
    \brief
        Method that handles the opening Font XML element.
    */
    void elementFontStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the closing Font XML element.
    */
    void elementFontEnd();
};

} // End of  CEGUI namespace section

#endif
