/***********************************************************************
    filename:   CEGUIFalFrameComponent.cpp
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
#include "falagard/CEGUIFalFrameComponent.h"
#include "falagard/CEGUIFalXMLEnumHelper.h"
#include "CEGUIExceptions.h"
#include "CEGUIImageManager.h"
#include "CEGUIImage.h"
#include <iostream>
#include <cstdlib>

// Start of CEGUI namespace section
namespace CEGUI
{
    FrameComponent::FrameComponent() :
        d_vertFormatting(VF_STRETCHED),
        d_horzFormatting(HF_STRETCHED)
    {
        for (int i = 0; i < FIC_FRAME_IMAGE_COUNT; ++i)
            d_frameImages[i] = 0;
    }

    VerticalFormatting FrameComponent::getBackgroundVerticalFormatting() const
    {
        return d_vertFormatting;
    }

    void FrameComponent::setBackgroundVerticalFormatting(VerticalFormatting fmt)
    {
        d_vertFormatting = fmt;
    }

    HorizontalFormatting FrameComponent::getBackgroundHorizontalFormatting() const
    {
        return d_horzFormatting;
    }

    void FrameComponent::setBackgroundHorizontalFormatting(HorizontalFormatting fmt)
    {
        d_horzFormatting = fmt;
    }

    const Image* FrameComponent::getImage(FrameImageComponent part) const
    {
        assert(part < FIC_FRAME_IMAGE_COUNT);

        return d_frameImages[part];
    }

    void FrameComponent::setImage(FrameImageComponent part, const Image* image)
    {
        assert(part < FIC_FRAME_IMAGE_COUNT);

        d_frameImages[part] = image;
    }

    void FrameComponent::setImage(FrameImageComponent part, const String& name)
    {
        assert(part < FIC_FRAME_IMAGE_COUNT);

        CEGUI_TRY
        {
            d_frameImages[part] = &ImageManager::getSingleton().get(name);
        }
        CEGUI_CATCH (UnknownObjectException&)
        {
            d_frameImages[part] = 0;
        }
    }

    void FrameComponent::render_impl(Window& srcWindow, Rectf& destRect, const CEGUI::ColourRect* modColours, const Rectf* clipper, bool clipToDisplay) const
    {
        Rectf backgroundRect(destRect);
        Rectf finalRect;
        Sizef imageSize;
        Vector2f imageOffsets;
        ColourRect imageColours;
        float leftfactor, rightfactor, topfactor, bottomfactor;
        bool calcColoursPerImage;

        // vars we use to track what to do with the side pieces.
        float topOffset = 0, bottomOffset = 0, leftOffset = 0, rightOffset = 0;
        float topWidth, bottomWidth, leftHeight, rightHeight;
        topWidth = bottomWidth = destRect.getWidth();
        leftHeight = rightHeight = destRect.getHeight();

        // calculate final overall colours to be used
        ColourRect finalColours;
        initColoursRect(srcWindow, modColours, finalColours);

        if (finalColours.isMonochromatic())
        {
            calcColoursPerImage = false;
            imageColours = finalColours;
        }
        else
        {
            calcColoursPerImage = true;
        }

        // top-left image
        if (d_frameImages[FIC_TOP_LEFT_CORNER])
        {
            // calculate final destination area
            imageSize = d_frameImages[FIC_TOP_LEFT_CORNER]->getRenderedSize();
            imageOffsets = d_frameImages[FIC_TOP_LEFT_CORNER]->getRenderedOffset();
            finalRect.d_min = destRect.d_min;
            finalRect.setSize(imageSize);
            finalRect = destRect.getIntersection (finalRect);

            // update adjustments required to edges do to presence of this element.
            topOffset  += imageSize.d_width + imageOffsets.d_x;
            leftOffset += imageSize.d_height + imageOffsets.d_y;
            topWidth   -= topOffset;
            leftHeight -= leftOffset;

            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (finalRect.left() + imageOffsets.d_x) / destRect.getWidth();
                rightfactor  = leftfactor + finalRect.getWidth() / destRect.getWidth();
                topfactor    = (finalRect.top() + imageOffsets.d_y) / destRect.getHeight();
                bottomfactor = topfactor + finalRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // draw this element.
            d_frameImages[FIC_TOP_LEFT_CORNER]->render(srcWindow.getGeometryBuffer(), finalRect, clipper, imageColours);
        }

        // top-right image
        if (d_frameImages[FIC_TOP_RIGHT_CORNER])
        {
            // calculate final destination area
            imageSize = d_frameImages[FIC_TOP_RIGHT_CORNER]->getRenderedSize();
            imageOffsets = d_frameImages[FIC_TOP_RIGHT_CORNER]->getRenderedOffset();
            finalRect.left(destRect.right() - imageSize.d_width);
            finalRect.top(destRect.top());
            finalRect.setSize(imageSize);
            finalRect = destRect.getIntersection(finalRect);

            // update adjustments required to edges do to presence of this element.
            rightOffset += imageSize.d_height + imageOffsets.d_y;
            topWidth    -= imageSize.d_width - imageOffsets.d_x;
            rightHeight -= rightOffset;

            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (finalRect.left() + imageOffsets.d_x) / destRect.getWidth();
                rightfactor  = leftfactor + finalRect.getWidth() / destRect.getWidth();
                topfactor    = (finalRect.top() + imageOffsets.d_y) / destRect.getHeight();
                bottomfactor = topfactor + finalRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle(leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // draw this element.
            d_frameImages[FIC_TOP_RIGHT_CORNER]->render(srcWindow.getGeometryBuffer(), finalRect, clipper, imageColours);
        }

        // bottom-left image
        if (d_frameImages[FIC_BOTTOM_LEFT_CORNER])
        {
            // calculate final destination area
            imageSize = d_frameImages[FIC_BOTTOM_LEFT_CORNER]->getRenderedSize();
            imageOffsets = d_frameImages[FIC_BOTTOM_LEFT_CORNER]->getRenderedOffset();
            finalRect.left(destRect.left());
            finalRect.top(destRect.bottom() - imageSize.d_height);
            finalRect.setSize(imageSize);
            finalRect = destRect.getIntersection(finalRect);

            // update adjustments required to edges do to presence of this element.
            bottomOffset += imageSize.d_width + imageOffsets.d_x;
            bottomWidth  -= bottomOffset;
            leftHeight   -= imageSize.d_height - imageOffsets.d_y;

            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (finalRect.left() + imageOffsets.d_x) / destRect.getWidth();
                rightfactor  = leftfactor + finalRect.getWidth() / destRect.getWidth();
                topfactor    = (finalRect.top() + imageOffsets.d_y) / destRect.getHeight();
                bottomfactor = topfactor + finalRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle(leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // draw this element.
            d_frameImages[FIC_BOTTOM_LEFT_CORNER]->render(srcWindow.getGeometryBuffer(), finalRect, clipper, imageColours);
        }

        // bottom-right image
        if (d_frameImages[FIC_BOTTOM_RIGHT_CORNER])
        {
            // calculate final destination area
            imageSize = d_frameImages[FIC_BOTTOM_RIGHT_CORNER]->getRenderedSize();
            imageOffsets = d_frameImages[FIC_BOTTOM_RIGHT_CORNER]->getRenderedOffset();
            finalRect.left(destRect.right() - imageSize.d_width);
            finalRect.top(destRect.bottom() - imageSize.d_height);
            finalRect.setSize(imageSize);
            finalRect = destRect.getIntersection(finalRect);

            // update adjustments required to edges do to presence of this element.
            bottomWidth -= imageSize.d_width - imageOffsets.d_x;
            rightHeight -= imageSize.d_height - imageOffsets.d_y;

            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (finalRect.left() + d_frameImages[FIC_BOTTOM_RIGHT_CORNER]->getRenderedOffset().d_x) / destRect.getWidth();
                rightfactor  = leftfactor + finalRect.getWidth() / destRect.getWidth();
                topfactor    = (finalRect.top() + d_frameImages[FIC_BOTTOM_RIGHT_CORNER]->getRenderedOffset().d_y) / destRect.getHeight();
                bottomfactor = topfactor + finalRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // draw this element.
            d_frameImages[FIC_BOTTOM_RIGHT_CORNER]->render(srcWindow.getGeometryBuffer(), finalRect, clipper, imageColours);
        }

        // top image
        if (d_frameImages[FIC_TOP_EDGE])
        {
            // calculate final destination area
            imageSize = d_frameImages[FIC_TOP_EDGE]->getRenderedSize();
            finalRect.left(destRect.left() + topOffset);
            finalRect.right(finalRect.left() + topWidth);
            finalRect.top(destRect.top());
            finalRect.bottom(finalRect.top() + imageSize.d_height);
            finalRect = destRect.getIntersection(finalRect);

            // adjust background area to miss this edge
            backgroundRect.d_min.d_y += imageSize.d_height + d_frameImages[FIC_TOP_EDGE]->getRenderedOffset().d_y;

            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (finalRect.left() + d_frameImages[FIC_TOP_EDGE]->getRenderedOffset().d_x) / destRect.getWidth();
                rightfactor  = leftfactor + finalRect.getWidth() / destRect.getWidth();
                topfactor    = (finalRect.top() + d_frameImages[FIC_TOP_EDGE]->getRenderedOffset().d_y) / destRect.getHeight();
                bottomfactor = topfactor + finalRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // draw this element.
            d_frameImages[FIC_TOP_EDGE]->render(srcWindow.getGeometryBuffer(), finalRect, clipper, imageColours);
        }

        // bottom image
        if (d_frameImages[FIC_BOTTOM_EDGE])
        {
            // calculate final destination area
            imageSize = d_frameImages[FIC_BOTTOM_EDGE]->getRenderedSize();
            finalRect.left(destRect.left() + bottomOffset);
            finalRect.right(finalRect.left() + bottomWidth);
            finalRect.bottom(destRect.bottom());
            finalRect.top(finalRect.bottom() - imageSize.d_height);
            finalRect = destRect.getIntersection (finalRect);

            // adjust background area to miss this edge
            backgroundRect.d_max.d_y -= imageSize.d_height - d_frameImages[FIC_BOTTOM_EDGE]->getRenderedOffset().d_y;;

            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (finalRect.left() + d_frameImages[FIC_BOTTOM_EDGE]->getRenderedOffset().d_x) / destRect.getWidth();
                rightfactor  = leftfactor + finalRect.getWidth() / destRect.getWidth();
                topfactor    = (finalRect.top() + d_frameImages[FIC_BOTTOM_EDGE]->getRenderedOffset().d_y) / destRect.getHeight();
                bottomfactor = topfactor + finalRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle(leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // draw this element.
            d_frameImages[FIC_BOTTOM_EDGE]->render(srcWindow.getGeometryBuffer(), finalRect, clipper, imageColours);
        }

        // left image
        if (d_frameImages[FIC_LEFT_EDGE])
        {
            // calculate final destination area
            imageSize = d_frameImages[FIC_LEFT_EDGE]->getRenderedSize();
            finalRect.left(destRect.left());
            finalRect.right(finalRect.left() + imageSize.d_width);
            finalRect.top(destRect.top() + leftOffset);
            finalRect.bottom(finalRect.top() + leftHeight);
            finalRect = destRect.getIntersection(finalRect);

            // adjust background area to miss this edge
            backgroundRect.d_min.d_x += imageSize.d_width + d_frameImages[FIC_LEFT_EDGE]->getRenderedOffset().d_x;

            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (finalRect.left() + d_frameImages[FIC_LEFT_EDGE]->getRenderedOffset().d_x) / destRect.getWidth();
                rightfactor  = leftfactor + finalRect.getWidth() / destRect.getWidth();
                topfactor    = (finalRect.top() + d_frameImages[FIC_LEFT_EDGE]->getRenderedOffset().d_y) / destRect.getHeight();
                bottomfactor = topfactor + finalRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // draw this element.
            d_frameImages[FIC_LEFT_EDGE]->render(srcWindow.getGeometryBuffer(), finalRect, clipper, imageColours);
        }

        // right image
        if (d_frameImages[FIC_RIGHT_EDGE])
        {
            // calculate final destination area
            imageSize = d_frameImages[FIC_RIGHT_EDGE]->getRenderedSize();
            finalRect.top(destRect.top() + rightOffset);
            finalRect.bottom(finalRect.top() + rightHeight);
            finalRect.right(destRect.right());
            finalRect.left(finalRect.right() - imageSize.d_width);
            finalRect = destRect.getIntersection (finalRect);

            // adjust background area to miss this edge
            backgroundRect.d_max.d_x -= imageSize.d_width - d_frameImages[FIC_RIGHT_EDGE]->getRenderedOffset().d_x;

            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (finalRect.left() + d_frameImages[FIC_RIGHT_EDGE]->getRenderedOffset().d_x) / destRect.getWidth();
                rightfactor  = leftfactor + finalRect.getWidth() / destRect.getWidth();
                topfactor    = (finalRect.top() + d_frameImages[FIC_RIGHT_EDGE]->getRenderedOffset().d_y) / destRect.getHeight();
                bottomfactor = topfactor + finalRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // draw this element.
            d_frameImages[FIC_RIGHT_EDGE]->render(srcWindow.getGeometryBuffer(), finalRect, clipper, imageColours);
        }

        if (d_frameImages[FIC_BACKGROUND])
        {
            // calculate colours that are to be used to this component image
            if (calcColoursPerImage)
            {
                leftfactor   = (backgroundRect.left() + d_frameImages[FIC_BACKGROUND]->getRenderedOffset().d_x) / destRect.getWidth();
                rightfactor  = leftfactor + backgroundRect.getWidth() / destRect.getWidth();
                topfactor    = (backgroundRect.top() + d_frameImages[FIC_BACKGROUND]->getRenderedOffset().d_y) / destRect.getHeight();
                bottomfactor = topfactor + backgroundRect.getHeight() / destRect.getHeight();

                imageColours = finalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
            }

            // render background image.
            doBackgroundRender(srcWindow, backgroundRect, imageColours, clipper, clipToDisplay);
        }
    }

    void FrameComponent::doBackgroundRender(Window& srcWindow, Rectf& destRect, const ColourRect& colours, const Rectf* clipper, bool /*clipToDisplay*/) const
    {
        HorizontalFormatting horzFormatting = d_horzFormatPropertyName.empty() ? d_horzFormatting :
            FalagardXMLHelper::stringToHorzFormat(srcWindow.getProperty(d_horzFormatPropertyName));

        VerticalFormatting vertFormatting = d_vertFormatPropertyName.empty() ? d_vertFormatting :
            FalagardXMLHelper::stringToVertFormat(srcWindow.getProperty(d_vertFormatPropertyName));

        uint horzTiles, vertTiles;
        float xpos, ypos;

        Sizef imgSz(d_frameImages[FIC_BACKGROUND]->getRenderedSize());

        // calculate initial x co-ordinate and horizontal tile count according to formatting options
        switch (horzFormatting)
        {
            case HF_STRETCHED:
                imgSz.d_width = destRect.getWidth();
                xpos = destRect.left();
                horzTiles = 1;
                break;

            case HF_TILED:
                xpos = destRect.left();
                horzTiles = std::abs(static_cast<int>(
                    (destRect.getWidth() + (imgSz.d_width - 1)) / imgSz.d_width));
                break;

            case HF_LEFT_ALIGNED:
                xpos = destRect.left();
                horzTiles = 1;
                break;

            case HF_CENTRE_ALIGNED:
                xpos = destRect.left() + PixelAligned((destRect.getWidth() - imgSz.d_width) * 0.5f);
                horzTiles = 1;
                break;

            case HF_RIGHT_ALIGNED:
                xpos = destRect.right() - imgSz.d_width;
                horzTiles = 1;
                break;

            default:
                CEGUI_THROW(InvalidRequestException("FrameComponent::doBackgroundRender - An unknown HorizontalFormatting value was specified."));
        }

        // calculate initial y co-ordinate and vertical tile count according to formatting options
        switch (vertFormatting)
        {
            case VF_STRETCHED:
                imgSz.d_height = destRect.getHeight();
                ypos = destRect.top();
                vertTiles = 1;
                break;

            case VF_TILED:
                ypos = destRect.top();
                vertTiles = std::abs(static_cast<int>(
                    (destRect.getHeight() + (imgSz.d_height - 1)) / imgSz.d_height));
                break;

            case VF_TOP_ALIGNED:
                ypos = destRect.top();
                vertTiles = 1;
                break;

            case VF_CENTRE_ALIGNED:
                ypos = destRect.top() + PixelAligned((destRect.getHeight() - imgSz.d_height) * 0.5f);
                vertTiles = 1;
                break;

            case VF_BOTTOM_ALIGNED:
                ypos = destRect.bottom() - imgSz.d_height;
                vertTiles = 1;
                break;

            default:
                CEGUI_THROW(InvalidRequestException("FrameComponent::doBackgroundRender - An unknown VerticalFormatting value was specified."));
        }

        // perform final rendering (actually is now a caching of the images which will be drawn)
        Rectf finalRect;
        Rectf finalClipper;
        const Rectf* clippingRect;
        finalRect.d_min.d_y = ypos;
        finalRect.d_max.d_y = ypos + imgSz.d_height;

        for (uint row = 0; row < vertTiles; ++row)
        {
            finalRect.d_min.d_x = xpos;
            finalRect.d_max.d_x = xpos + imgSz.d_width;

            for (uint col = 0; col < horzTiles; ++col)
            {
                // use custom clipping for right and bottom edges when tiling the imagery
                if (((vertFormatting == VF_TILED) && row == vertTiles - 1) ||
                    ((horzFormatting == HF_TILED) && col == horzTiles - 1))
                {
                    finalClipper = clipper ? clipper->getIntersection(destRect) : destRect;
                    clippingRect = &finalClipper;
                }
                // not tiliing, or not on far edges, just used passed in clipper (if any).
                else
                {
                    clippingRect = clipper;
                }

                // add image to the rendering cache for the target window.
                d_frameImages[FIC_BACKGROUND]->render(srcWindow.getGeometryBuffer(), finalRect, clippingRect, colours);

                finalRect.d_min.d_x += imgSz.d_width;
                finalRect.d_max.d_x += imgSz.d_width;
            }

            finalRect.d_min.d_y += imgSz.d_height;
            finalRect.d_max.d_y += imgSz.d_height;
        }
    }

    void FrameComponent::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        // opening tag
        xml_stream.openTag("FrameComponent");
        // write out area
        d_area.writeXMLToStream(xml_stream);

        // write images
        for (int i = 0; i < FIC_FRAME_IMAGE_COUNT; ++i)
        {
            if (d_frameImages[i])
            {
                xml_stream.openTag("Image")
                    .attribute("name", d_frameImages[i]->getName())
                    .attribute("type", FalagardXMLHelper::frameImageComponentToString(static_cast<FrameImageComponent>(i)))
                    .closeTag();
            }
        }

        // get base class to write colours
        writeColoursXML(xml_stream);

        // write vert format, allowing base class to do this for us if a propety is in use
        if (!writeVertFormatXML(xml_stream))
        {
            // was not a property, so write out explicit formatting in use
            xml_stream.openTag("VertFormat")
                .attribute("type", FalagardXMLHelper::vertFormatToString(d_vertFormatting))
                .closeTag();

        }

        // write horz format, allowing base class to do this for us if a propety is in use
        if (!writeHorzFormatXML(xml_stream))
        {
            // was not a property, so write out explicit formatting in use
            xml_stream.openTag("HorzFormat")
                .attribute("type", FalagardXMLHelper::horzFormatToString(d_horzFormatting))
                .closeTag();
        }

        // closing tag
        xml_stream.closeTag();
    }

} // End of  CEGUI namespace section
