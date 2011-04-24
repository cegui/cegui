/***********************************************************************
    filename:   CEGUIImageManager.h
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

#include "CEGUISingleton.h"
#include "CEGUIXMLHandler.h"
#include "CEGUIString.h"
#include "CEGUISize.h"
#include <map>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIEXPORT ImageManager :
        public Singleton<ImageManager>,
        public AllocatedObject<ImageManager>,
        public XMLHandler
{
public:
    ImageManager();
    ~ImageManager();

    //! add a /copy/ (via Image::clone) of \a image.
    void add(const Image& image);
    void destroy(Image& image);
    void destroy(const String& name);
    void destroyAll();

    Image& get(const String& name) const;
    bool isDefined(const String& name) const;

    uint getImageCount() const;

    void loadImageset(const String& filename, const String& resource_group = "");
    void destroyImageCollection(const String& prefix,
                                const bool delete_texture = true);

    void addFromImageFile(const String& name,
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
    const String& getSchemaName() const;
    const String& getDefaultResourceGroup() const;

    void elementStart(const String& element, const XMLAttributes& attributes);

private:
    //! container type used to hold the images.
    typedef std::map<String, Image*, StringFastLessCompare
                     CEGUI_MAP_ALLOC(String, Image*)> ImageMap;

    //! helper to delete an image given an map iterator.
    void destroy(ImageMap::iterator& iter);

    // XML parsing helper functions.
    void elementImagesetStart(const XMLAttributes& attributes);
    void elementImageStart(const XMLAttributes& attributes);
    //! Default resource group specifically for Imagesets.
    static String d_imagesetDefaultResourceGroup;

    //! container holding the images.
    ImageMap d_images;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIImageManager_h_

