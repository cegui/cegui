/***********************************************************************
    created:    Sun Jul 19 2015
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/Singleton.h"
#include "CEGUI/ResourceEventSet.h"
#include "CEGUI/XmlResourceExistsAction.h"
#include "CEGUI/text/FontSizeUnit.h"
#include "CEGUI/Image.h" // for AutoScaledMode
#include <unordered_map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class FreeTypeFont;
class PixmapFont;


/*!
\brief
    Class providing a shared library of Font objects to the system.

    The FontManager is used to create, access, and destroy Font objects.  The
    idea is that the FontManager will function as a central repository for Font
    objects used within the GUI system, and that those Font objects can be
    accessed, via a unique name, by any interested party within the system.
*/
class CEGUIEXPORT FontManager :
        public Singleton<FontManager>,
        public ResourceEventSet
{
public:
    //! Constructor.
    FontManager();

    //! Destructor.
    ~FontManager();
    //! Map container that maps Font names (String) to Font pointers
    typedef std::unordered_map<String, Font*> FontRegistry;

    //! List of fonts
    typedef std::vector<Font*> FontList;

    /*!
    \brief
        Creates new Font instances from a RawDataContainer and adds them to the collection.

        Use an instance of the XML resource loading class \a U to process the
        XML source thereby creating Font instances and adding them to the collection.

    \param source
        RawDataContainer holding the XML source to be used when creating the
        new Font instances.

    \param resourceExistsAction
        One of the XmlResourceExistsAction enumerated values indicating what
        action should be taken when a Font with the specified name already 
        exists within the collection.
    */
    static FontList createFromContainer(const RawDataContainer& source,
        XmlResourceExistsAction resourceExistsAction = XmlResourceExistsAction::Return);

    /*!
    \brief
        Creates new Font instances from an XML file and adds them to the collection.

        Use an instance of the XML resource loading class \a U to process the
        XML file \a xml_filename from resource group \a resource_group thereby
        creating an instance of class \a T and add it to the collection under
        the name specified in the XML file.

    \param xml_filename
        String holding the filename of the XML file to be used when creating the
        new Font instances.

    \param resource_group
        String holding the name of the resource group identifier to be used
        when loading the XML file described by \a xml_filename.

    \param resourceExistsAction
        One of the XmlResourceExistsAction enumerated values indicating what
        action should be taken when a Font with the specified name already
        exists within the collection.
    */
    static FontList createFromFile(const String& xml_filename, const String& resource_group = "",
        XmlResourceExistsAction resourceExistsAction = XmlResourceExistsAction::Return);

    /*!
    \brief
        Creates Font instances from a string and adds them to the collection.

        Use an instance of the XML resource loading class \a U to process the
        XML source thereby creating an instance of class \a T and add it to the collection under
        the name specified in the XML file.

    \param source
        String holding the XML source to be used when creating the
        new Font instances.

    \param resourceExistsAction
        One of the XmlResourceExistsAction enumerated values indicating what
        action should be taken when an Scheme with the specified name
        already exists within the collection.
    */
    static FontList createFromString(const String& source,
        XmlResourceExistsAction resourceExistsAction = XmlResourceExistsAction::Return);

       /*!
    \brief
        Destroy the Font named \a font_name, or do nothing if such a
        Font does not exist in the collection.

    \param font_name
        String holding the name of the Font to be destroyed.
    */
    void destroy(const String& font_name);

    /*!
    \brief
        Destroy the Font \a font, or do nothing if such an
        Font does not exist in the collection.

    \param font
        The Font to be destroyed (beware of keeping references to this Font
        once it's been destroyed!)
    */
    void destroy(const Font& font);

    //! Destroy all Fonts.
    void destroyAll();

    /*!
    \brief
        Return a reference to the Font named \a font_name.

    \param font_name
        String holding the name of the Font to be returned.

    \exception UnknownObjectException
        thrown if no Font named \a font_name exists within the collection.
    */
    Font& get(const String& font_name) const;

    //! Return whether an object named \a font_name exists.
    bool isDefined(const String& font_name) const;

    //! Create new Font instances from files with names matching \a pattern in \a resource_group
    void createAll(const String& pattern, const String& resource_group);

    /*!
    \brief
        Creates a FreeType type font.

    \param font_name
        The name that the font will use within the CEGUI system.

    \param size
        Specifies the size that the font is to be rendered at.

    \param sizeUnit
        Specifies the size unit of the Font size.

    \param anti_aliased
        Specifies whether the font should be rendered using anti aliasing.

    \param font_filename
        The filename of an font file that will be used as the source for
        glyph images for this font.

    \param resource_group
        The resource group identifier to use when loading the font file
        specified by \a font_filename.

    \param auto_scaled
        Specifies whether the font imagery should be automatically scaled to
        maintain the same physical size (which is calculated by using the
        native resolution setting).

    \param native_horz_res
        The horizontal native resolution value.  This is only significant when
        auto scaling is enabled.

    \param native_vert_res
        The vertical native resolution value.  This is only significant when
        auto scaling is enabled.

    \param action
        One of the XmlResourceExistsAction enumerated values indicating what
        action should be taken when a Font with the specified name
        already exists.

    \return
        Reference to the newly create Font object.
    */
    Font& createFreeTypeFont(
        const String& font_name,
        const float size,
        const FontSizeUnit sizeUnit,
        const bool anti_aliased,
        const String& font_filename,
        const String& resource_group = "",
        const AutoScaledMode auto_scaled = AutoScaledMode::Disabled,
        const Sizef& native_res = Sizef(640.0f, 480.0f),
        const float specificLineSpacing = 0.0f,
        XmlResourceExistsAction resourceExistsAction = XmlResourceExistsAction::Return);

    /*!
    \brief
        Creates a Pixmap type font.

    \param font_name
        The name that the font will use within the CEGUI system.

    \param imageset_filename
        The filename of an Imageset to load that will be used as the source for
        glyph images for this font.  If \a resource_group is the special value
        of "*", this parameter may instead refer to the name of an already
        loaded Imageset.

    \param resource_group
        The resource group identifier to use when loading the Imageset file
        specified by \a imageset_filename.  If this group is set to the special
        value of "*", then \a imageset_filename instead will refer to the name
        of an existing Imageset.

    \param auto_scaled
        Specifies whether the font imagery should be automatically scaled to
        maintain the same physical size (which is calculated by using the
        native resolution setting).

    \param native_horz_res
        The horizontal native resolution value.  This is only significant when
        auto scaling is enabled.

    \param native_vert_res
        The vertical native resolution value.  This is only significant when
        auto scaling is enabled.

    \param resourceExistsAction
        One of the XmlResourceExistsAction enumerated values indicating what
        action should be taken when a Font with the specified name
        already exists.

    \return
        Reference to the newly create Font object.
    */
    Font& createPixmapFont(const String& font_name,
                           const String& imageset_filename,
                           const String& resource_group = "",
                           const AutoScaledMode auto_scaled = AutoScaledMode::Disabled,
                           const Sizef& native_res = Sizef(640.0f, 480.0f),
                           XmlResourceExistsAction resourceExistsAction = XmlResourceExistsAction::Return);

    /*!
    \brief
        Notify the FontManager that display size may have changed.

    \param size
        Size object describing the display resolution
    */
    void notifyDisplaySizeChanged(const Sizef& size);

    /*!
    \brief
        Writes a full XML font file for the specified Font to the given
        OutStream.

    \param name
        String holding the name of the Font to be written to the stream.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.
    */
    void writeFontToStream(const String& name, OutStream& out_stream) const;

    /*!
    \brief
        Returns a reference to the map of registered Fonts
    \return
        A map containing the names of and the pointers to the registered Fonts 
    */
    const FontRegistry& getRegisteredFonts() const;

    //! Updates all Fonts, this means that all Fonts will be regenerated.
    void updateAllFonts();

    //! The name of the resource type handled by this class
    static const String ResourceTypeName;

protected:
    //! implementation of object destruction.
    void destroyObject(FontRegistry::iterator fontRegistry);

    /*!
    \brief
        Helper function to check if a resource exists and act accordingly, i.e.
        return a Font, throw an error or return a nullptr
    */
    Font* handleResourceExistsAction(const String& font_name,
        XmlResourceExistsAction resourceExistsAction, String& event_name);

    //! Map of registered Fonts, containing name and pointer to the instance
    FontRegistry d_registeredFonts;
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIFontManager_h_
