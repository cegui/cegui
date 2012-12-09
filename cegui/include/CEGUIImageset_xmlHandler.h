/***********************************************************************
    filename:   CEGUIImageset_xmlHandler.h
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
#ifndef _CEGUIImageset_xmlHandler_h_
#define _CEGUIImageset_xmlHandler_h_

#include "CEGUIXMLHandler.h"
#include "CEGUIString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//! Class used to parse the Imageset XML files to create Imageset objects
class CEGUIEXPORT Imageset_xmlHandler : public XMLHandler
{
public:
    //! Constructor.
    Imageset_xmlHandler(const String& filename, const String& resource_group);

    //! Destructor.
    ~Imageset_xmlHandler();

    //! Return string holding the name of the created Imageset.
    const String& getObjectName() const;

    //! Return reference to the created Imageset object.
    Imageset& getObject() const;

    // XMLHandler overrides
    void elementStart(const String& element, const XMLAttributes& attributes);
    void elementEnd(const String& element);

private:
    //! Method that handles the opening Imageset XML element.
    void elementImagesetStart(const XMLAttributes& attributes);
    //! Method that handles the Image XML element.
    void elementImageStart(const XMLAttributes& attributes);
    //! Method that handles the closing Imageset XML element.
    void elementImagesetEnd();

    //! Filename of the XML schema used for validating Imageset files.
    static const String ImagesetSchemaName;
    //! Tag name for Imageset elements.
    static const String ImagesetElement;
    //! Tag name for Image elements.
    static const String ImageElement;
    //! Attribute name that stores the name of the Imageset
    static const String ImagesetNameAttribute;
    //! Attribute name that stores the filename for the image file.
    static const String ImagesetImageFileAttribute;
    //! Attribute name that stores resource group used when loading image file.
    static const String ImagesetResourceGroupAttribute;
    //! Attribute that stores 'native' horizontal resolution for the Imageset.
    static const String ImagesetNativeHorzResAttribute;
    //! Attribute that stores 'native' vertical resolution for the Imageset.
    static const String ImagesetNativeVertResAttribute;
    //! Attribute that specifies whether the Imageset should be auto-scaled.
    static const String ImagesetAutoScaledAttribute;
    //! Attribute name that stores the name of the new Image.
    static const String ImageNameAttribute;
    //! Attribute name that stores the x position of the new Image.
    static const String ImageXPosAttribute;
    //! Attribute name that stores the y position of the new Image.
    static const String ImageYPosAttribute;
    //! Attribute name that stores the width of the new Image.
    static const String ImageWidthAttribute;
    //! Attribute name that stores the height of the new Image.
    static const String ImageHeightAttribute;
    //! Attribute name that stores the x rendering offset of the new Image.
    static const String ImageXOffsetAttribute;
    //!< Attribute name that stores the y rendering offset of the new Image.
    static const String ImageYOffsetAttribute;

    //! Pointer to the Imageset created.
    Imageset* d_imageset;
    //! inidcates whether client read the created object
    mutable bool d_objectRead;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIImageset_xmlHandler_h_
