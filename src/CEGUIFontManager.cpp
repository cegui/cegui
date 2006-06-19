/***********************************************************************
	filename: 	CEGUIFontManager.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the FontManager class
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
#include "CEGUIFontManager.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIFont.h"
#include "CEGUISystem.h"
#include "CEGUIXMLParser.h"
#include "CEGUIFont_xmlHandler.h"
#include "CEGUIFreeTypeFont.h"
#include "CEGUIPixmapFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Static Data Definitions
*************************************************************************/

// Font schema filename
static const String FontSchemaName ("Font.xsd");
static const String FontTypeFreeType ("FreeType");
static const String FontTypePixmap ("Pixmap");

// singleton instance pointer
template<> FontManager* Singleton<FontManager>::ms_Singleton	= 0;


/*************************************************************************
	constructor
*************************************************************************/
FontManager::FontManager(void)
{
	Logger::getSingleton().logEvent("CEGUI::FontManager singleton created.");
}


/*************************************************************************
	Destructor
*************************************************************************/
FontManager::~FontManager(void)
{
	Logger::getSingleton().logEvent("---- Begining cleanup of Font system ----");
	destroyAllFonts();

	Logger::getSingleton().logEvent("CEGUI::FontManager singleton destroyed.");
}


/*************************************************************************
	Create a font from a definition file
*************************************************************************/
Font* FontManager::createFont(const String& filename, const String& resourceGroup)
{
    if (filename.empty ())
        throw InvalidRequestException ("FontManager::createFont - Filename supplied for Font loading must be valid");

    Logger::getSingleton ().logEvent ("Attempting to create Font from the information specified in file '" + filename + "'.");

    // create handler object
    Font_xmlHandler handler;

    // do parse (which uses handler to create actual data)
    try
    {
        System::getSingleton ().getXMLParser ()->parseXMLFile (
            handler, filename, FontSchemaName, resourceGroup.empty () ?
            Font::getDefaultResourceGroup () : resourceGroup);
    }
    catch (...)
    {
        if (handler.d_font)
            destroyFont (handler.d_font->getProperty ("Name"));

        Logger::getSingleton ().logEvent ("FontManager::createFont - loading of Font from file '" + filename +"' failed.", Errors);
        throw;
    }

    // if this was the first font created, set it as the default font
    if (d_fonts.size () == 1)
        System::getSingleton ().setDefaultFont (handler.d_font);

    return handler.d_font; 
}


/*************************************************************************
	Create a font from an installed OS font
*************************************************************************/
Font* FontManager::createFont (const String& type, const String& name, const String& fontname,
                               const String& resourceGroup)
{
    Logger::getSingleton ().logEvent ("Attempting to create Font '" + name + "' using the font file '" + fontname + ".");

    // first ensure name uniqueness
    if (isFontPresent (name))
        throw AlreadyExistsException ("FontManager::createFont - A font named '" + name + "' already exists.");

    Font *temp;
    // FreeType fonts
    if (type == FontTypeFreeType)
        temp = new FreeTypeFont (name, fontname, resourceGroup.empty () ?
            Font::getDefaultResourceGroup () : resourceGroup);
    // Imageset based font
    else if (type == FontTypePixmap)
        temp = new PixmapFont (name, fontname, resourceGroup.empty () ?
            Font::getDefaultResourceGroup () : resourceGroup);
    // error (should never happen)
    else
        throw FileIOException ("FontManager::createFont - The value '" + type + "' for the Font:Type attribute is unknown.");

    d_fonts [name] = temp;

    // if this was the first font created, set it as the default font
    if (d_fonts.size () == 1)
        System::getSingleton ().setDefaultFont (temp);

    return temp; 
}


/*************************************************************************
	Create a font given its type and the respective XML attributes
*************************************************************************/
Font *FontManager::createFont (const String &type, const XMLAttributes& attributes)
{
    Font *temp;

    // TrueType fonts
    if (type == FontTypeFreeType)
        temp = new FreeTypeFont (attributes);
    // static (Image based) font
    else if (type == FontTypePixmap)
        temp = new PixmapFont (attributes);
    // error (should never happen)
    else
        throw FileIOException ("FontManager::createFont - The value for the Font:Type attribute '" + type + "' is unknown.");

    String name = temp->getProperty ("Name");
    if (isFontPresent (name))
    {
        delete temp;
        throw AlreadyExistsException ("FontManager::createFont - A font named '" + name + "' already exists.");
    }

    d_fonts [name] = temp;

    return temp;
}


/*************************************************************************
	Destroy the named font
*************************************************************************/
void FontManager::destroyFont(const String& name)
{
    FontRegistry::iterator pos = d_fonts.find(name);

	if (pos != d_fonts.end())
	{
		String tmpName(name);

		delete pos->second;
		d_fonts.erase(pos);

		Logger::getSingleton().logEvent("Font '" + tmpName +"' has been destroyed.");
	}

}


/*************************************************************************
	Destroys the given Font object
*************************************************************************/
void FontManager::destroyFont(Font* font)
{
	if (font)
	{
		destroyFont(font->getProperty ("Name"));
	}

}


/*************************************************************************
	Destroys all Font objects registered in the system
*************************************************************************/
void FontManager::destroyAllFonts(void)
{
	while (!d_fonts.empty())
	{
		destroyFont(d_fonts.begin()->first);
	}

}


/*************************************************************************
	Check to see if a font is available
*************************************************************************/
bool FontManager::isFontPresent(const String& name) const
{
	return (d_fonts.find(name) != d_fonts.end());
}


/*************************************************************************
	Return a pointer to the named font
*************************************************************************/
Font* FontManager::getFont(const String& name) const
{
	FontRegistry::const_iterator pos = d_fonts.find(name);

	if (pos == d_fonts.end())
	{
		throw UnknownObjectException("FontManager::getFont - A Font object with the specified name '" + name +"' does not exist within the system");
	}

	return pos->second;
}


/*************************************************************************
	Notify the FontManager of the current (usually new) display
	resolution.
*************************************************************************/
void FontManager::notifyScreenResolution(const Size& size)
{
	// notify all attached Font objects of the change in resolution
	FontRegistry::iterator pos = d_fonts.begin(), end = d_fonts.end();

	for (; pos != end; ++pos)
	{
		pos->second->notifyScreenResolution(size);
	}

}


/*************************************************************************
	Return a FontManager::FontIterator object to iterate over the
	available Font objects.
*************************************************************************/
FontManager::FontIterator FontManager::getIterator(void) const
{
	return FontIterator(d_fonts.begin(), d_fonts.end());
}


void FontManager::writeFontToStream(const String& name, OutStream& out_stream) const
{
    const Font* font = getFont(name);
    XMLSerializer xml(out_stream);
    // output font data
    font->writeXMLToStream(xml);
}

} // End of  CEGUI namespace section
