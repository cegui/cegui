/************************************************************************
	filename: 	CEGUIFontManager.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for the FontManager class
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
#ifndef _CEGUIFontManager_h_
#define _CEGUIFontManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUIFont.h"
#include <map>


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Enumerated type that contains the valid flags that can be passed to createFont when creating a new font.
*/
enum CEGUIBASE_API FontFlag
{
	TestFlag			//!< This is a value in the FontFlag enumeration, it means nothing and will be deleted.
};


/*!
\brief
	Class providing a shared library of Font objects to the system.

	The FontManager is used to create, access, and destroy Font objects.  The idea is that the
	FontManager will function as a central repository for Font objects used within the GUI system,
	and that those Font objects can be accessed, via a unique name, by any interested party within
	the system.
*/
class CEGUIBASE_API FontManager : public Singleton<FontManager>
{
public:
	/*!
	\brief
		Constructor for FontManager objects
	*/
	FontManager(void);


	/*!
	\brief
		Destructor for FontManager objects
	*/
	virtual ~FontManager(void);


	/*!
	\brief
		Return singleton FontManager object

	\return
		Singleton FontManager object
	*/
	static	FontManager&	getSingleton(void)			{return Singleton<FontManager>::getSingleton();}


	/*!
	\brief
		Return pointer to singleton FontManager object

	\return
		Pointer to singleton FontManager object
	*/
	static	FontManager*	getSingletonPtr(void)			{return Singleton<FontManager>::getSingletonPtr();}


	/*!
	\brief
		Creates a new font from a font definition file, and returns a pointer to the new Font object.

	\param filename
		String object containing the filename of a 'font definition file' what will be used to create the new font

	\param flags
		Some combination of FontFlag values used for creation of the font.  Values specified here can be overridden in the font definition file

	\return
		Pointer the the newly created Font object

	\exception AlreadyExistsException	Thrown if a Font already exists with the name specified for the font in the file \a filename
	\exception FileIOException			Thrown if some error occurred accessing the file \a filename
	\exception InvalidRequestException	Thrown if some invalid or illogical combination of FontFlag values was given
	*/
	Font*	createFont(const String& filename, uint flags);


	/*!
	\brief
		Creates a new Font based on a true-type font, and returns a pointer to the new Font object.

	\param name
		String object containing a unique name for the new font.

	\param fontname
		String object containing the name and path of the true-type font to access.

	\param flags
		Some combination of FontFlag values to be used for the creation of this font.

	\param size
		Specifies the glyph size (point-size) for the new font.

	\param dynamic
		When true, the new Font will automatically adjust it's content for all glyphs supported by the true-type font,
		when false, the Font is populated with the 7-bit ASCII glyphs only.

	\return
		Pointer to the newly created Font object.

	\exception AlreadyExistsException	Thrown if a Font already exists with the name \a name
	\exception InvalidRequestException	Thrown if some invalid or illogical combination of FontFlag values was given
	\exception GenericException			Usually thrown if some error occurred accessing the OS font specified
	*/
	Font*	createFont(const String& name, const String& fontname, uint flags, uint size, bool dynamic);


	/*!
	\brief
		Destroy's the font with the given name

	\param name
		String object containing the name of the font to be destroyed.  If the specified font does not exist, nothing happens.

	\return
		Nothing
	*/
	void	destroyFont(const String& name);


	/*!
	\brief
		Checks the existence of a given font.

	\param name
		String object holding the name of the Font object to look for.

	\return
		true if a Font object named \a name exists in the system, false if no such font exists.
	*/
	bool	isFontPresent(const String& name) const;


	/*!
	\brief
		Returns a pointer to the font object with the specified name

	\param name
		String object containing the name of the Font object to be returned

	\return
		Pointer to the requested Font object

	\exception UnknownObjectException	Thrown if no font with the given name exists.
	*/
	Font*	getFont(const String& name) const;

private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef	std::map<String, Font*>		FontRegistry;
	FontRegistry		d_fonts;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIFontManager_h_
