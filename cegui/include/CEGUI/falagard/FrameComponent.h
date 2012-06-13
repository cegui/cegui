/***********************************************************************
    filename:   CEGUIFrameComponent.h
    created:    Mon Jul 18 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIFalFrameComponent_h_
#define _CEGUIFalFrameComponent_h_

#include "./ComponentBase.h"
#include "../XMLSerializer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class that encapsulates information for a frame with background (9 images in total)

        Corner images are always drawn at their natural size, edges are stretched between the corner
        pieces for a particular edge, the background image will cover the inner rectangle formed by
        the edge images and can be stretched or tiled in either dimension.
    */
    class CEGUIEXPORT FrameComponent : public FalagardComponentBase
    {
    public:
        /*!
        \brief
            Constructor
        */
        FrameComponent();

        /*!
        \brief
            Return the current vertical formatting setting for this FrameComponent.

        \return
            One of the VerticalFormatting enumerated values.
        */
        VerticalFormatting getBackgroundVerticalFormatting() const;

        /*!
        \brief
            Set the vertical formatting setting for this FrameComponent.

        \param fmt
            One of the VerticalFormatting enumerated values.

        \return
            Nothing.
        */
        void setBackgroundVerticalFormatting(VerticalFormatting fmt);

        /*!
        \brief
            Return the current horizontal formatting setting for this FrameComponent.

        \return
            One of the HorizontalFormatting enumerated values.
        */
        HorizontalFormatting getBackgroundHorizontalFormatting() const;

        /*!
        \brief
            Set the horizontal formatting setting for this FrameComponent.

        \param fmt
            One of the HorizontalFormatting enumerated values.

        \return
            Nothing.
        */
        void setBackgroundHorizontalFormatting(HorizontalFormatting fmt);

        /*!
        \brief
            Return the Image object that will be drawn by this FrameComponent
            for a specified frame part.

        \param part
            One of the FrameImageComponent enumerated values specifying the
            component image to be accessed.

        \param wnd
            Reference to a Window object that will be accessed if the image
            component is fetched from a Property.

        \return
            pointer to an Image object, or 0 if the image had not been set
            or if the image is sourced from a property that returns an empty
            image name.
        */
        const Image* getImage(FrameImageComponent part,
                              const Window& wnd) const;

        /*!
        \brief
            Set an Image that will be drawn by this FrameComponent.

        \param part
            One of the FrameImageComponent enumerated values specifying the
            component image to be set.

        \param image
            Pointer to the Image object to be drawn.  If this is 0 then drawing
            of the component image specified by \a part will be disabled.
        */
        void setImage(FrameImageComponent part, const Image* image);

        /*!
        \brief
            Set an Image that will be drawn by this FrameComponent.

        \param part
            One of the FrameImageComponent enumerated values specifying the
            component image to be set.

        \param name
            String holding the name of an Image. The image should already exist,
            if the Image does not exist an Exception will be logged and
            drawing of the component image specified by \a part will be
            disabled.
        */
        void setImage(FrameImageComponent part, const String& name);

        /*!
        \brief
            Set the name of the Property that will be accesssed on the target
            Window to determine the Image that will be drawn by the
            FrameComponent.

        \param part
            One of the FrameImageComponent enumerated values specifying the
            component image to be set.

        \param name
            String holding the name of a property that will be accessed. If this
            is the empty string then drawing of the component image specified by
            \a part will be disabled.
        */
        void setImagePropertySource(FrameImageComponent part, const String& name);

        /*!
        \brief
            Return whether the given component image has been specified.

        \param part
            One of the FrameImageComponent enumerated values specifying the
            component image to check.

        \return
            - true if the image is specified and will be drawn.
            - false if the image is not specified.
        */
        bool isImageSpecified(FrameImageComponent part) const;

        /*!
        \brief
            Return whether the given component image is specified via a
            property.

        \param part
            One of the FrameImageComponent enumerated values specifying the
            component image to check.

        \return
            - true if the image is specified and fetched via a property.
            - false if the image is not fetched via a property
              (or is not specified)
        */
        bool isImageFetchedFromProperty(FrameImageComponent part) const;

        /*!
        \brief
            Return the name of the property that will be used to determine the
            image to use for the given component image.
            
            If the returned String is empty, it indicates either that the
            component image is not specified or that the component image is not
            sourced from a property.

        \see
            isImageFetchedFromProperty isImageSpecified

        \param part
            One of the FrameImageComponent enumerated values specifying the
            component image property source to return.
        */
        const String& getImagePropertySource(FrameImageComponent part) const;

        /*!
        \brief
            Writes an xml representation of this FrameComponent to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.


        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

        bool operator==(const FrameComponent& rhs) const;

    protected:
        struct FrameImageSource
        {
            FrameImageSource() :
                d_specified(false),
                d_image(0)
            {}

            bool operator==(const FrameImageSource& rhs) const
            {
                return d_specified == rhs.d_specified &&
                       d_image == rhs.d_image &&
                       d_propertyName == rhs.d_propertyName;
            }

            bool operator!=(const FrameImageSource& rhs) const
            {
                return !operator==(rhs);
            }
            
            bool d_specified;
            const Image* d_image;
            String d_propertyName;
        };

        // implemets abstract from base
        void render_impl(Window& srcWindow, Rectf& destRect, const CEGUI::ColourRect* modColours, const Rectf* clipper, bool clipToDisplay) const;

        // renders the background image (basically a clone of render_impl from
        // ImageryComponent - maybe we need a helper class?)
        void doBackgroundRender(Window& srcWindow, const Image* image,
                                Rectf& destRect, const ColourRect& colours,
                                const Rectf* clipper, bool clipToDisplay) const;

        // formatting options for background
        VerticalFormatting   d_vertFormatting;  //!< Vertical formatting to be applied when rendering the background for the component.
        HorizontalFormatting d_horzFormatting;  //!< Horizontal formatting to be applied when rendering the background for the component.
        //! FrameImageSource array describing images to be used.
        FrameImageSource d_frameImages[FIC_FRAME_IMAGE_COUNT];
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalFrameComponent_h_
