/************************************************************************
	filename: 	CEGUIFontManager.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the FontManager class
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
#include "CEGUIFontManager.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIFont.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> FontManager* Singleton<FontManager>::ms_Singleton	= NULL;



/*************************************************************************
	constructor
*************************************************************************/
FontManager::FontManager(void)
{
	if (FT_Init_FreeType(&d_ftlib))
	{
		throw GenericException((utf8*)"FontManager::FontManager - Failed to initialise the FreeType library.");
	}

	Logger::getSingleton().logEvent((utf8*)"CEGUI::FontManager singleton created.");
}


/*************************************************************************
	Destructor
*************************************************************************/
FontManager::~FontManager(void)
{
	Logger::getSingleton().logEvent((utf8*)"---- Begining cleanup of Font system ----");
	destroyAllFonts();

	FT_Done_FreeType(d_ftlib);

	Logger::getSingleton().logEvent((utf8*)"CEGUI::FontManager singleton destroyed.");
}


/*************************************************************************
	Create a font from a definition file
*************************************************************************/
Font* FontManager::createFont(const String& filename)
{
	Font* temp = new Font(filename);

	String name = temp->getName();

	if (isFontPresent(name))
	{
		delete temp;

		throw AlreadyExistsException((utf8*)"FontManager::createFont - A font named '" + name + "' already exists.");
	}

	d_fonts[name] = temp;

	Logger::getSingleton().logEvent((utf8*)"Font '" + name + "' has been created from the information specified in file '" + filename + "'.", Informative);

	return temp; 
}


/*************************************************************************
	Create a font from an installed OS font
*************************************************************************/
Font* FontManager::createFont(const String& name, const String& fontname, uint size, uint flags)
{
	// first ensure name uniqueness
	if (isFontPresent(name))
	{
		throw AlreadyExistsException((utf8*)"FontManager::createFont - A font named '" + name + "' already exists.");
	}

	Font* temp = new Font(name, fontname, size, flags);
	d_fonts[name] = temp;

	char strbuf[16];
	sprintf(strbuf, "%d", size);

	Logger::getSingleton().logEvent((utf8*)"Font '" + name + "' has been created using the font file '" + fontname + "' and a size of " + strbuf + ".");

	return temp; 
}


/*************************************************************************
	Destroy the named font
*************************************************************************/
void FontManager::destroyFont(const String& name)
{
	 FontRegistry::iterator	pos = d_fonts.find(name);

	if (pos != d_fonts.end())
	{
		Logger::getSingleton().logEvent((utf8*)"Font '" + name +"' has been destroyed.", Informative);

		delete pos->second;
		d_fonts.erase(pos);
	}

}


/*************************************************************************
	Destroys the given Font object
*************************************************************************/
void FontManager::destroyFont(Font* font)
{
	if (font != NULL)
	{
		destroyFont(font->getName());
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

} // End of  CEGUI namespace section
