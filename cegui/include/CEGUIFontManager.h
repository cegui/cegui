/***********************************************************************
	filename: 	CEGUIFontManager.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for the FontManager class
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
#ifndef _CEGUIFontManager_h_
#define _CEGUIFontManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUIIteratorBase.h"
#include <map>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Class providing a shared library of Font objects to the system.

	The FontManager is used to create, access, and destroy Font objects.  The idea is that the
	FontManager will function as a central repository for Font objects used within the GUI system,
	and that those Font objects can be accessed, via a unique name, by any interested party within
	the system.
*/
class CEGUIEXPORT FontManager : public Singleton<FontManager>
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
	~FontManager(void);


	/*!
	\brief
		Creates a new font from a font definition file, and returns a pointer to the new Font object.

	\param filename
		String object containing the filename of a 'font definition file' what will be used to create the new font

    \param resourceGroup
        Resource group identifier to pass to the resource provider when loading the font definition file.

	\return
		Pointer the the newly created Font object

	\exception	FileIOException				thrown if there was some problem accessing or parsing the file \a filename
	\exception	InvalidRequestException		thrown if an invalid filename was provided.
	\exception	AlreadyExistsException		thrown if a Font already exists with the name specified, or if a font Imageset clashes with one already defined in the system.
	\exception	GenericException			thrown if something goes wrong while accessing a true-type font referenced in file \a filename.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the requested glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
    Font *createFont(const String& filename, const String& resourceGroup = "");


	/*!
	\brief
		Creates a new Font based on a true-type font, and returns a pointer to the new Font object.

	\param type
		String object containing the type of the font to be created (same as
		in the "Type" attribute of the font XML).

	\param name
		String object containing a unique name for the new font.

	\param fontname
		String object containing the name and path of the true-type font to access.

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when loading the font definition file.

	\return
		Pointer to the newly created Font object.

	\exception	AlreadyExistsException		thrown if a Font already exists with the name specified, or if a font Imageset clashes with one already defined in the system.
	\exception	GenericException			thrown if something goes wrong while accessing a true-type font referenced in file \a fontname.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the requested glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
    Font *createFont (const String &type, const String& name, const String& fontname,
                      const String& resourceGroup = "");


    /*!
    \brief
        Create a new font object given its type and the XML attributes.

    \return
        The new font object or NULL.
    */
    Font *createFont (const String &type, const XMLAttributes& attributes);


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
		Destroys the given Font object

	\param font
		Pointer to the Font to be destroyed.  If no such Font exists, nothing happens.

	\return
		Nothing.
	*/
	void	destroyFont(Font* font);


	/*!
	\brief
		Destroys all Font objects registered in the system

	\return
		Nothing
	*/
	void	destroyAllFonts(void);


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


	/*!
	\brief
		Notify the FontManager of the current (usually new) display resolution.

	\param size
		Size object describing the display resolution

	\return
		Nothing
	*/
	void	notifyScreenResolution(const Size& size);


    /*!
    \brief
        Writes a full XML font file for the specified Font to the given OutStream.

    \param name
        String holding the name of the Font to be written to the stream.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.

    \return
        Nothing.
    */
    void writeFontToStream(const String& name, OutStream& out_stream) const;


private:
    /*************************************************************************
        Implementation Data
    *************************************************************************/
    typedef std::map<String, Font*, String::FastLessCompare> FontRegistry;
    FontRegistry d_fonts;

public:
    /*************************************************************************
        Iterator stuff
    *************************************************************************/
    typedef ConstBaseIterator<FontRegistry> FontIterator;

    /*!
    \brief
        Return a FontManager::FontIterator object to iterate over the available Font objects.
    */
    FontIterator getIterator (void) const;
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIFontManager_h_
