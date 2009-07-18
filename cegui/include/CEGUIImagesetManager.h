/***********************************************************************
    filename:   CEGUIImagesetManager.h
    created:    Sat Jul 18 2009
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
#ifndef _CEGUIImagesetManager_h_
#define _CEGUIImagesetManager_h_

#include "CEGUIBase.h"
#include "CEGUISingleton.h"
#include "CEGUINamedXMLResourceManager.h"
#include "CEGUIImageset.h"
#include "CEGUIImageset_xmlHandler.h"
#include "CEGUIIteratorBase.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4275)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class providing a shared library of Imageset objects to the system.

    The ImagesetManager is used to create, access, and destroy Imageset objects.
    The idea is that the ImagesetManager will function as a central repository
    for imagery used within the GUI system, and that such imagery can be
    accessed, via a unique name, by any interested party within the system.
*/
class CEGUIEXPORT ImagesetManager :
        public Singleton<ImagesetManager>,
        public NamedXMLResourceManager<Imageset, Imageset_xmlHandler>
{
public:
    //! Constructor for ImagesetManager objects
    ImagesetManager();

    //! Destructor for ImagesetManager objects
    ~ImagesetManager();

    /*!
    \brief
        Create a Imageset object with the given name and Texture

        The created Imageset will be of limited use, and will require one or
        more images to be defined for the set.

    \param name
        String object containing the unique name for the Imageset being created.

    \param texture
        Texture object to be associated with the Imageset

    \param action
        One of the XMLResourceExistsAction enumerated values indicating what
        action should be taken when an Imageset with the specified name
        already exists.

    \return
        Reference to the newly created Imageset object

    \exception AlreadyExistsException
        thrown if an Imageset named \a name is already present in the system.
    */
    Imageset& create(const String& name, Texture& texture,
                     XMLResourceExistsAction action = XREA_RETURN);

    /*!
    \brief
        Create an Imageset object from the specified image file.  The Imageset
        will initially have a single image defined named "full_image" which is
        an image that represents the entire area of the loaded image.

    \param name
        String object containing the unique name for the Imageset being created.

    \param filename
        String object holding the name of the image file to be loaded.

    \param resourceGroup
        Resource group identifier to be passed to the resource manager when
        loading the image file.

    \param action
        One of the XMLResourceExistsAction enumerated values indicating what
        action should be taken when an Imageset with the specified name
        already exists.

    \return
        Reference to the newly created Imageset object

    \exception AlreadyExistsException
        thrown if an Imageset named \a name is already present in the system.

    \exception FileIOException
        thrown if something goes wrong while reading the image file \a filename.
    */
    Imageset& createFromImageFile(const String& name, const String& filename,
                                const String& resourceGroup = "",
                                XMLResourceExistsAction action = XREA_RETURN);

    /*!
    \brief
        Notify the ImagesetManager that the display size may have changed.

    \param size
        Size object describing the display resolution
    */
    void notifyDisplaySizeChanged(const Size& size);

    /*!
    \brief
        Writes a full XML imageset for the specified Imageset to the given
        OutStream.

    \param imageset
        String holding the name of the Imageset to be written to the stream.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.

    \return
        Nothing.
    */
    void writeImagesetToStream(const String& imageset,
                               OutStream& out_stream) const;

    //! ImagesetIterator type definition.
    typedef ConstBaseIterator<ObjectRegistry> ImagesetIterator;

    /*!
    \brief
        Return a ImagesetManager::ImagesetIterator object to iterate over the
        available Imageset objects.
    */
    ImagesetIterator getIterator() const;

    // ensure we see overloads from template base class
    using NamedXMLResourceManager<Imageset, Imageset_xmlHandler>::create;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIImageSetManager_h_
