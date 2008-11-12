/***********************************************************************
filename:   CEGUIFont_xmlHandler.cpp
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
#include "CEGUIFont_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUIFont.h"
#include "CEGUIFontManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
static data definitions
*************************************************************************/

// XML related strings
static const String FontElement ("Font");
static const String MappingElement ("Mapping");
static const String FontTypeAttribute ("Type");

/*************************************************************************
    Main element-start despatcher
*************************************************************************/
void Font_xmlHandler::elementStart (const String& element, const XMLAttributes& attributes)
{
    // handle root Font element
    if (element == FontElement)
        elementFontStart (attributes);
    // handle a Mapping element
    else if ((element == MappingElement) && d_font)
        d_font->defineMapping (attributes);
    // anything else is really an error which *should* have already been
    // caught by XML validation
    else
        throw FileIOException ("Font::xmlHandler::startElement - Unexpected data was found while parsing the Font file: '" + element + "' is unknown.");
}

/*************************************************************************
    Main element-end despatcher
*************************************************************************/
void Font_xmlHandler::elementEnd (const String& element)
{
    if (element == FontElement)
        elementFontEnd ();
}

/*************************************************************************
    Method that handles the opening Font XML element.
*************************************************************************/
void Font_xmlHandler::elementFontStart (const XMLAttributes& attributes)
{
    // get type of font being created
    String font_type (attributes.getValueAsString (FontTypeAttribute));
    d_font = FontManager::getSingleton ().createFont (font_type, attributes);
}

/*************************************************************************
    Method that handles the closing Font XML element.
*************************************************************************/
void Font_xmlHandler::elementFontEnd ()
{
    d_font->load ();
    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(d_font));
    Logger::getSingleton ().logEvent ("Finished creation of Font '" +
        d_font->d_name + "' via XML file. " + addr_buff, Informative);
}

} // End of  CEGUI namespace section
