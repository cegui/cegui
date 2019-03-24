/***********************************************************************
    created:    Wed Feb 16 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIImageManager_h_
#define _CEGUIImageManager_h_

#include "CEGUI/Singleton.h"
#include "CEGUI/ChainedXMLHandler.h"
#include "CEGUI/String.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/ImageFactory.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/IteratorBase.h"
#include <unordered_map>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIEXPORT ImageManager :
        public Singleton<ImageManager>,
        public ChainedXMLHandler
{
public:
    ImageManager();
    ~ImageManager();

    /*!
    \brief
        Register an Image subclass with the system and associate it with the
        identifier \a name.

        This registers a subclass of the Image class, such that instances of
        that subclass can subsequently be created by using the identifier
        \a name.

    \tparam T
        The Image subclass to be instantiated when an Image is created using the
        type identifier \a name.

    \param name
        String object describing the identifier that the Image subclass will be
        registered under.

    \exception AlreadyExistsException
        thrown if an Image subclass is already registered using \a name.
    */
    template <typename T>
    void addImageType(const String& name);

    /*!
    \brief
        Unregister the Image subclass that was registered under the identifier
        \a name.

    \param name
        String object describing the identifier of the Image subclass that is to
        be unregistered.  If no such identifier is known within the system, no
        action is taken.

    \note
        You should avoid removing Image subclass types that are still in use.
        Internally a factory system is employed for the creation and deletion
        of Image based objects; if an Image subclass - and therefore it's
        factory - is removed while instances of that class are still active, it
        will not be possible to safely delete those instances.
    */
    void removeImageType(const String& name);

    /*!
    \brief
        Return whether an Image subclass has been registered using the
        identifier \a name.

    \param name
        String object describing the identifier to test for.

    \return
        - true if an Image subclass is registered using the identifier \a name.
        - false if no Image subclass is registered using the identifier \a name.
    */
    bool isImageTypeAvailable(const String& name) const;

    /*!
    \brief
        Create an instance of Image subclass registered for identifier \a type
        using the name \a name.

    \param type
        String object describing the identifier of the Image subclass that is to
        be created.

    \param name
        String object describing the name that the newly created instance will
        be created with.  This name must be unique within the system. 

    \exception UnknownObjectException
        thrown if no Image subclass has been registered using identifier \a type.

    \exception AlreadyExistsException
        thrown if an Image instance named \a name already exists.
    */
    Image& create(const String& type, const String& name);

    Image& create(const XMLAttributes& attributes);

    void destroy(Image& image);
    void destroy(const String& name);
    void destroyAll();

    Image& get(const String& name) const;
    bool isDefined(const String& name) const;

    unsigned int getImageCount() const;

    void loadImageset(const String& filename, const String& resource_group = "");
    void loadImagesetFromString(const String& source);

    void destroyImageCollection(const String& prefix,
                                const bool delete_texture = true);

    void addBitmapImageFromFile(const String& name,
                          const String& filename,
                          const String& resource_group = "");

    /*!
    \brief
        Notify the ImageManager that the display size may have changed.

    \param size
        Size object describing the display resolution
    */
    void notifyDisplaySizeChanged(const Sizef& size);

    /*!
    \brief
        Sets the default resource group to be used when loading imageset data

    \param resourceGroup
        String describing the default resource group identifier to be used.
    */
    static void setImagesetDefaultResourceGroup(const String& resourceGroup)
        { d_imagesetDefaultResourceGroup = resourceGroup; }

    /*!
    \brief
        Returns the default resource group currently set for Imagesets.

    \return
        String describing the default resource group identifier that will be
        used when loading Imageset data.
    */
    static const String& getImagesetDefaultResourceGroup()
        { return d_imagesetDefaultResourceGroup; }

    // XMLHandler overrides
    const String& getSchemaName() const override;
    const String& getDefaultResourceGroup() const override;

    //! One entry in the image container.
    typedef std::pair<Image*, ImageFactory*> ImagePair;

    //! container type used to hold the images.
    typedef std::unordered_map<String, ImagePair> ImageMap;

    //! ConstBaseIterator type definition.
    typedef ConstMapIterator<ImageMap> ImageIterator;

    /*!
    \brief
        Return a ImageManager::ImageIterator object to iterate over the available
        Image objects.
    */
    ImageIterator getIterator() const;

private:
    // implement chained xml handler abstract interface
    void elementStartLocal(const String& element, const XMLAttributes& attributes) override;
    void elementEndLocal(const String& element) override;

    //! container type used to hold the registered Image types.
    typedef std::unordered_map<String, ImageFactory*> ImageFactoryRegistry;

    //! helper to delete an image given an map iterator.
    void destroy(ImageMap::iterator& iter);

    // XML parsing helper functions.
    void elementImagesetStart(const XMLAttributes& attributes);

    // Get or create the Imageset's texture
    void retrieveImagesetTexture(const String& name, const String& filename, const String &resource_group);

    // Get or create the Imageset's SVGData
    void retrieveImagesetSVGData(const String& name, const String& filename, const String &resource_group);

    void elementImageStart(const XMLAttributes& attributes);
    //! throw exception if file version is not supported.
    void validateImagesetFileVersion(const XMLAttributes& attrs);

    //! Default resource group specifically for Imagesets.
    static String d_imagesetDefaultResourceGroup;

    //! container holding the factories.
    ImageFactoryRegistry d_factories;
    //! container holding the images.
    ImageMap d_images;
};

//---------------------------------------------------------------------------//
template <typename T>
void ImageManager::addImageType(const String& name)
{
    if (isImageTypeAvailable(name))
        throw AlreadyExistsException(
            "Image type already exists: " + name);

    d_factories[name] = new TplImageFactory<T>;

    Logger::getSingleton().logEvent(
        "[ImageManager] Registered Image type: " + name);
}

//---------------------------------------------------------------------------//
} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIImageManager_h_

