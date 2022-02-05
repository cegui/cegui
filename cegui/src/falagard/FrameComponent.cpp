/***********************************************************************
    created:    Mon Jul 18 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/falagard/FrameComponent.h"
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/falagard/XMLHandler.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/Image.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{

//! Default values
const HorizontalFormatting FrameComponent::HorizontalFormattingDefault(HorizontalFormatting::Stretched);
const VerticalImageFormatting FrameComponent::VerticalFormattingDefault(VerticalImageFormatting::Stretched);

//----------------------------------------------------------------------------//
FrameComponent::FrameComponent() :
    d_leftEdgeFormatting(VerticalFormattingDefault),
    d_rightEdgeFormatting(VerticalFormattingDefault),
    d_topEdgeFormatting(HorizontalFormattingDefault),
    d_bottomEdgeFormatting(HorizontalFormattingDefault),
    d_backgroundVertFormatting(VerticalFormattingDefault),
    d_backgroundHorzFormatting(HorizontalFormattingDefault)
{
}

//----------------------------------------------------------------------------//
void FrameComponent::setLeftEdgeFormatting(VerticalImageFormatting fmt)
{
    d_leftEdgeFormatting.set(fmt);
}

//----------------------------------------------------------------------------//
void FrameComponent::setRightEdgeFormatting(VerticalImageFormatting fmt)
{
    d_rightEdgeFormatting.set(fmt);
}

//----------------------------------------------------------------------------//
void FrameComponent::setTopEdgeFormatting(HorizontalFormatting fmt)
{
    d_topEdgeFormatting.set(fmt);
}

//----------------------------------------------------------------------------//
void FrameComponent::setBottomEdgeFormatting(HorizontalFormatting fmt)
{
    d_bottomEdgeFormatting.set(fmt);
}

//----------------------------------------------------------------------------//
void FrameComponent::setBackgroundVerticalFormatting(VerticalImageFormatting fmt)
{
    d_backgroundVertFormatting.set(fmt);
}

//----------------------------------------------------------------------------//
void FrameComponent::setBackgroundHorizontalFormatting(HorizontalFormatting fmt)
{
    d_backgroundHorzFormatting.set(fmt);
}

//----------------------------------------------------------------------------//
void FrameComponent::setLeftEdgeFormattingPropertySource(
                                                    const String& property_name)
{
    d_leftEdgeFormatting.setPropertySource(property_name);
}

//----------------------------------------------------------------------------//
void FrameComponent::setRightEdgeFormattingPropertySource(
                                                    const String& property_name)
{
    d_rightEdgeFormatting.setPropertySource(property_name);
}

//----------------------------------------------------------------------------//
void FrameComponent::setTopEdgeFormattingPropertySource(
                                                    const String& property_name)
{
    d_topEdgeFormatting.setPropertySource(property_name);
}

//----------------------------------------------------------------------------//
void FrameComponent::setBottomEdgeFormattingPropertySource(
                                                    const String& property_name)
{
    d_bottomEdgeFormatting.setPropertySource(property_name);
}

//----------------------------------------------------------------------------//
void FrameComponent::setBackgroundVerticalFormattingPropertySource(
                                                    const String& property_name)
{
    d_backgroundVertFormatting.setPropertySource(property_name);
}

//----------------------------------------------------------------------------//
void FrameComponent::setBackgroundHorizontalFormattingPropertySource(
                                                    const String& property_name)
{
    d_backgroundHorzFormatting.setPropertySource(property_name);
}

//----------------------------------------------------------------------------//
VerticalImageFormatting FrameComponent::getLeftEdgeFormatting(const Window& wnd) const
{
    return d_leftEdgeFormatting.get(wnd);
}

//----------------------------------------------------------------------------//
VerticalImageFormatting FrameComponent::getRightEdgeFormatting(const Window& wnd) const
{
    return d_rightEdgeFormatting.get(wnd);
}

//----------------------------------------------------------------------------//
HorizontalFormatting FrameComponent::getTopEdgeFormatting(const Window& wnd) const
{
    return d_topEdgeFormatting.get(wnd);
}

//----------------------------------------------------------------------------//
HorizontalFormatting FrameComponent::getBottomEdgeFormatting(const Window& wnd) const
{
    return d_bottomEdgeFormatting.get(wnd);
}

//----------------------------------------------------------------------------//
VerticalImageFormatting FrameComponent::getBackgroundVerticalFormatting(
                                                    const Window& wnd) const
{
    return d_backgroundVertFormatting.get(wnd);
}

//----------------------------------------------------------------------------//
HorizontalFormatting FrameComponent::getBackgroundHorizontalFormatting(
                                                    const Window& wnd) const
{
    return d_backgroundHorzFormatting.get(wnd);
}

//----------------------------------------------------------------------------//
const Image* FrameComponent::getImage(FrameImageComponent imageComponent,
                                      const Window& wnd) const
{
    assert(imageComponent < FrameImageComponent::FrameImageCount);

    const int frameImageIndex = static_cast<int>(imageComponent);

    if (!d_frameImages[frameImageIndex].d_specified)
        return nullptr;

    if (d_frameImages[frameImageIndex].d_propertyName.empty())
        return d_frameImages[frameImageIndex].d_image;

    return wnd.getProperty<Image*>(d_frameImages[frameImageIndex].d_propertyName);
}

//----------------------------------------------------------------------------//
const Image* FrameComponent::getImage(FrameImageComponent imageComponent) const
{
    assert(imageComponent < FrameImageComponent::FrameImageCount);

    const int frameImageIndex = static_cast<int>(imageComponent);

    if (!d_frameImages[frameImageIndex].d_specified)
        return nullptr;

    if (d_frameImages[frameImageIndex].d_propertyName.empty())
        return d_frameImages[frameImageIndex].d_image;

    return nullptr;
}

//----------------------------------------------------------------------------//
void FrameComponent::setImage(FrameImageComponent part, const Image* image)
{
    assert(part < FrameImageComponent::FrameImageCount);

    const int frameImageIndex = static_cast<int>(part);

    d_frameImages[frameImageIndex].d_image = image;
    d_frameImages[frameImageIndex].d_specified = image != nullptr;
    d_frameImages[frameImageIndex].d_propertyName.clear();
}

//----------------------------------------------------------------------------//
void FrameComponent::setImage(FrameImageComponent part, const String& name)
{
    const Image* image;
    try
    {
        image = &ImageManager::getSingleton().get(name);
    }
    catch (UnknownObjectException&)
    {
        image = nullptr;
    }

    setImage(part, image);
}

//----------------------------------------------------------------------------//
void FrameComponent::setImagePropertySource(FrameImageComponent part,
                                            const String& name)
{
    assert(part < FrameImageComponent::FrameImageCount);

    const int frameImageIndex = static_cast<int>(part);

    d_frameImages[frameImageIndex].d_image = nullptr;
    d_frameImages[frameImageIndex].d_specified = !name.empty();
    d_frameImages[frameImageIndex].d_propertyName = name;
}

//----------------------------------------------------------------------------//
bool FrameComponent::isImageSpecified(FrameImageComponent part) const
{
    assert(part < FrameImageComponent::FrameImageCount);

    const int frameImageIndex = static_cast<int>(part);

    return d_frameImages[frameImageIndex].d_specified;
}

//----------------------------------------------------------------------------//
bool FrameComponent::isImageFetchedFromProperty(FrameImageComponent part) const
{
    assert(part < FrameImageComponent::FrameImageCount);

    const int frameImageIndex = static_cast<int>(part);

    return d_frameImages[frameImageIndex].d_specified &&
           !d_frameImages[frameImageIndex].d_propertyName.empty();
}

//----------------------------------------------------------------------------//
const String& FrameComponent::getImagePropertySource(
                                    FrameImageComponent part) const
{
    assert(part < FrameImageComponent::FrameImageCount);

    const int frameImageIndex = static_cast<int>(part);

    return d_frameImages[frameImageIndex].d_propertyName;
}

//----------------------------------------------------------------------------//
void FrameComponent::addImageRenderGeometryToWindow_impl(
    Window& srcWindow, Rectf& destRect,
    const CEGUI::ColourRect* modColours,
    const Rectf* clipper) const
{
    Rectf backgroundRect(destRect);
    Sizef imageSize;
    glm::vec2 imageOffsets;
    float leftfactor, rightfactor, topfactor, bottomfactor;
    bool calcColoursPerImage;

    // vars we use to track what to do with the side pieces.
    float topOffset = 0, bottomOffset = 0, leftOffset = 0, rightOffset = 0;
    float topWidth, bottomWidth, leftHeight, rightHeight;
    topWidth = bottomWidth = destRect.getWidth();
    leftHeight = rightHeight = destRect.getHeight();

    // calculate final overall colours to be used
    ColourRect renderSettingFinalColours;
    initColoursRect(srcWindow, modColours, renderSettingFinalColours);

    ImageRenderSettings renderSettings(
        Rectf(), clipper, renderSettingFinalColours);

    Rectf& renderSettingDestArea = renderSettings.d_destArea;
    ColourRect& renderSettingMultiplyColours = renderSettings.d_multiplyColours;

    calcColoursPerImage = !renderSettingFinalColours.isMonochromatic();

    const auto canCombineFromIdx = srcWindow.getGeometryBuffers().size();
    
    // top-left image
    if (const Image* const componentImage = getImage(FrameImageComponent::TopLeftCorner, srcWindow))
    {
        // calculate final destination area
        imageSize = componentImage->getRenderedSize();
        imageOffsets = componentImage->getRenderedOffset();
        renderSettingDestArea.d_min = destRect.d_min;
        renderSettingDestArea.setSize(imageSize);
        renderSettingDestArea = destRect.getIntersection(renderSettingDestArea);

        // update adjustments required to edges do to presence of this element.
        topOffset  += imageSize.d_width + imageOffsets.x;
        leftOffset += imageSize.d_height + imageOffsets.y;
        topWidth   -= topOffset;
        leftHeight -= leftOffset;

        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (renderSettingDestArea.left() + imageOffsets.x) / destRect.getWidth();
            rightfactor  = leftfactor + renderSettingDestArea.getWidth() / destRect.getWidth();
            topfactor    = (renderSettingDestArea.top() + imageOffsets.y) / destRect.getHeight();
            bottomfactor = topfactor + renderSettingDestArea.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle(leftfactor, rightfactor, topfactor, bottomfactor);
        }

        // create render geometry for this element and append it to the Window's geometry
        componentImage->createRenderGeometry(srcWindow.getGeometryBuffers(), renderSettings, canCombineFromIdx);
    }

    // top-right image
    if (const Image* const componentImage = getImage(FrameImageComponent::TopRightCorner, srcWindow))
    {
        // calculate final destination area
        imageSize = componentImage->getRenderedSize();
        imageOffsets = componentImage->getRenderedOffset();
        renderSettingDestArea.left(destRect.right() - imageSize.d_width);
        renderSettingDestArea.top(destRect.top());
        renderSettingDestArea.setSize(imageSize);
        renderSettingDestArea = destRect.getIntersection(renderSettingDestArea);

        // update adjustments required to edges do to presence of this element.
        rightOffset += imageSize.d_height + imageOffsets.y;
        topWidth    -= imageSize.d_width - imageOffsets.x;
        rightHeight -= rightOffset;

        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (renderSettingDestArea.left() + imageOffsets.x) / destRect.getWidth();
            rightfactor  = leftfactor + renderSettingDestArea.getWidth() / destRect.getWidth();
            topfactor    = (renderSettingDestArea.top() + imageOffsets.y) / destRect.getHeight();
            bottomfactor = topfactor + renderSettingDestArea.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle(leftfactor, rightfactor, topfactor, bottomfactor);
        }

        // create render geometry for this element and append it to the Window's geometry
        componentImage->createRenderGeometry(srcWindow.getGeometryBuffers(), renderSettings, canCombineFromIdx);
    }

    // bottom-left image
    if (const Image* const componentImage = getImage(FrameImageComponent::BottomLeftCorner, srcWindow))
    {
        // calculate final destination area
        imageSize = componentImage->getRenderedSize();
        imageOffsets = componentImage->getRenderedOffset();
        renderSettingDestArea.left(destRect.left());
        renderSettingDestArea.top(destRect.bottom() - imageSize.d_height);
        renderSettingDestArea.setSize(imageSize);
        renderSettingDestArea = destRect.getIntersection(renderSettingDestArea);

        // update adjustments required to edges do to presence of this element.
        bottomOffset += imageSize.d_width + imageOffsets.x;
        bottomWidth  -= bottomOffset;
        leftHeight   -= imageSize.d_height - imageOffsets.y;

        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (renderSettingDestArea.left() + imageOffsets.x) / destRect.getWidth();
            rightfactor  = leftfactor + renderSettingDestArea.getWidth() / destRect.getWidth();
            topfactor    = (renderSettingDestArea.top() + imageOffsets.y) / destRect.getHeight();
            bottomfactor = topfactor + renderSettingDestArea.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle(leftfactor, rightfactor, topfactor, bottomfactor);
        }

        // create render geometry for this element and append it to the Window's geometry
        componentImage->createRenderGeometry(srcWindow.getGeometryBuffers(), renderSettings, canCombineFromIdx);
    }

    // bottom-right image
    if (const Image* const componentImage = getImage(FrameImageComponent::BottomRightCorner, srcWindow))
    {
        // calculate final destination area
        imageSize = componentImage->getRenderedSize();
        imageOffsets = componentImage->getRenderedOffset();
        renderSettingDestArea.left(destRect.right() - imageSize.d_width);
        renderSettingDestArea.top(destRect.bottom() - imageSize.d_height);
        renderSettingDestArea.setSize(imageSize);
        renderSettingDestArea = destRect.getIntersection(renderSettingDestArea);

        // update adjustments required to edges do to presence of this element.
        bottomWidth -= imageSize.d_width - imageOffsets.x;
        rightHeight -= imageSize.d_height - imageOffsets.y;

        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (renderSettingDestArea.left() + componentImage->getRenderedOffset().x) / destRect.getWidth();
            rightfactor  = leftfactor + renderSettingDestArea.getWidth() / destRect.getWidth();
            topfactor    = (renderSettingDestArea.top() + componentImage->getRenderedOffset().y) / destRect.getHeight();
            bottomfactor = topfactor + renderSettingDestArea.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
        }

        // create render geometry for this element and append it to the Window's geometry
        componentImage->createRenderGeometry(srcWindow.getGeometryBuffers(), renderSettings, canCombineFromIdx);
    }

    // top image
    if (const Image* const componentImage = getImage(FrameImageComponent::TopEdge, srcWindow))
    {
        // calculate final destination area
        imageSize = componentImage->getRenderedSize();
        renderSettingDestArea.left(destRect.left() + topOffset);
        renderSettingDestArea.right(renderSettingDestArea.left() + topWidth);
        renderSettingDestArea.top(destRect.top());
        renderSettingDestArea.bottom(renderSettingDestArea.top() + imageSize.d_height);
        renderSettingDestArea = destRect.getIntersection(renderSettingDestArea);

        // adjust background area to miss this edge
        backgroundRect.d_min.y += imageSize.d_height + componentImage->getRenderedOffset().y;

        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (renderSettingDestArea.left() + componentImage->getRenderedOffset().x) / destRect.getWidth();
            rightfactor  = leftfactor + renderSettingDestArea.getWidth() / destRect.getWidth();
            topfactor    = (renderSettingDestArea.top() + componentImage->getRenderedOffset().y) / destRect.getHeight();
            bottomfactor = topfactor + renderSettingDestArea.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
        }

        // create render geometry for this image and append it to the Window's geometry
        createRenderGeometryForImage(srcWindow.getGeometryBuffers(), componentImage,
            VerticalImageFormatting::TopAligned, d_topEdgeFormatting.get(srcWindow),
                renderSettingDestArea, renderSettingMultiplyColours, clipper, canCombineFromIdx);
    }

    // bottom image
    if (const Image* const componentImage = getImage(FrameImageComponent::BottomEdge, srcWindow))
    {
        // calculate final destination area
        imageSize = componentImage->getRenderedSize();
        renderSettingDestArea.left(destRect.left() + bottomOffset);
        renderSettingDestArea.right(renderSettingDestArea.left() + bottomWidth);
        renderSettingDestArea.bottom(destRect.bottom());
        renderSettingDestArea.top(renderSettingDestArea.bottom() - imageSize.d_height);
        renderSettingDestArea = destRect.getIntersection (renderSettingDestArea);

        // adjust background area to miss this edge
        backgroundRect.d_max.y -= imageSize.d_height - componentImage->getRenderedOffset().y;

        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (renderSettingDestArea.left() + componentImage->getRenderedOffset().x) / destRect.getWidth();
            rightfactor  = leftfactor + renderSettingDestArea.getWidth() / destRect.getWidth();
            topfactor    = (renderSettingDestArea.top() + componentImage->getRenderedOffset().y) / destRect.getHeight();
            bottomfactor = topfactor + renderSettingDestArea.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle(leftfactor, rightfactor, topfactor, bottomfactor);
        }

        // create render geometry for this image and append it to the Window's geometry
        createRenderGeometryForImage(srcWindow.getGeometryBuffers(), componentImage,
            VerticalImageFormatting::BottomAligned, d_bottomEdgeFormatting.get(srcWindow),
                renderSettingDestArea, renderSettingMultiplyColours, clipper, canCombineFromIdx);
    }

    // left image
    if (const Image* const componentImage = getImage(FrameImageComponent::LeftEdge, srcWindow))
    {
        // calculate final destination area
        imageSize = componentImage->getRenderedSize();
        renderSettingDestArea.left(destRect.left());
        renderSettingDestArea.right(renderSettingDestArea.left() + imageSize.d_width);
        renderSettingDestArea.top(destRect.top() + leftOffset);
        renderSettingDestArea.bottom(renderSettingDestArea.top() + leftHeight);
        renderSettingDestArea = destRect.getIntersection(renderSettingDestArea);

        // adjust background area to miss this edge
        backgroundRect.d_min.x += imageSize.d_width + componentImage->getRenderedOffset().x;

        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (renderSettingDestArea.left() + componentImage->getRenderedOffset().x) / destRect.getWidth();
            rightfactor  = leftfactor + renderSettingDestArea.getWidth() / destRect.getWidth();
            topfactor    = (renderSettingDestArea.top() + componentImage->getRenderedOffset().y) / destRect.getHeight();
            bottomfactor = topfactor + renderSettingDestArea.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
        }

        // create render geometry for this image and append it to the Window's geometry
        createRenderGeometryForImage(srcWindow.getGeometryBuffers(), componentImage,
            d_leftEdgeFormatting.get(srcWindow), HorizontalFormatting::LeftAligned,
                renderSettingDestArea, renderSettingMultiplyColours, clipper, canCombineFromIdx);
    }

    // right image
    if (const Image* const componentImage = getImage(FrameImageComponent::RightEdge, srcWindow))
    {
        // calculate final destination area
        imageSize = componentImage->getRenderedSize();
        renderSettingDestArea.top(destRect.top() + rightOffset);
        renderSettingDestArea.bottom(renderSettingDestArea.top() + rightHeight);
        renderSettingDestArea.right(destRect.right());
        renderSettingDestArea.left(renderSettingDestArea.right() - imageSize.d_width);
        renderSettingDestArea = destRect.getIntersection (renderSettingDestArea);

        // adjust background area to miss this edge
        backgroundRect.d_max.x -= imageSize.d_width - componentImage->getRenderedOffset().x;

        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (renderSettingDestArea.left() + componentImage->getRenderedOffset().x) / destRect.getWidth();
            rightfactor  = leftfactor + renderSettingDestArea.getWidth() / destRect.getWidth();
            topfactor    = (renderSettingDestArea.top() + componentImage->getRenderedOffset().y) / destRect.getHeight();
            bottomfactor = topfactor + renderSettingDestArea.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
        }

        // create render geometry for this image and append it to the Window's geometry
        createRenderGeometryForImage(srcWindow.getGeometryBuffers(), componentImage,
            d_rightEdgeFormatting.get(srcWindow), HorizontalFormatting::RightAligned,
                renderSettingDestArea, renderSettingMultiplyColours, clipper, canCombineFromIdx);
    }

    if (const Image* const componentImage = getImage(FrameImageComponent::Background, srcWindow))
    {
        // calculate colours that are to be used to this component image
        if (calcColoursPerImage)
        {
            leftfactor   = (backgroundRect.left() + componentImage->getRenderedOffset().x) / destRect.getWidth();
            rightfactor  = leftfactor + backgroundRect.getWidth() / destRect.getWidth();
            topfactor    = (backgroundRect.top() + componentImage->getRenderedOffset().y) / destRect.getHeight();
            bottomfactor = topfactor + backgroundRect.getHeight() / destRect.getHeight();

            renderSettingMultiplyColours = renderSettingFinalColours.getSubRectangle( leftfactor, rightfactor, topfactor, bottomfactor);
        }

        const HorizontalFormatting horzFormatting =
            d_backgroundHorzFormatting.get(srcWindow);

        const VerticalImageFormatting vertFormatting =
            d_backgroundVertFormatting.get(srcWindow);

        // create render geometry for this image and append it to the Window's geometry
        createRenderGeometryForImage(srcWindow.getGeometryBuffers(), componentImage,
                vertFormatting, horzFormatting,
                backgroundRect, renderSettingMultiplyColours, clipper, canCombineFromIdx);
    }
}

//----------------------------------------------------------------------------//
void FrameComponent::createRenderGeometryForImage(
    std::vector<GeometryBuffer*>& out,
    const Image* image,
    VerticalImageFormatting vertFmt,
    HorizontalFormatting horzFmt,
    Rectf& destRect, const ColourRect& colours,
    const Rectf* clipper, size_t canCombineFromIdx) const
{
    unsigned int horzTiles, vertTiles;
    float xpos, ypos;

    Sizef imgSz(image->getRenderedSize());

    // calculate initial x co-ordinate and horizontal tile count according to formatting options
    switch (horzFmt)
    {
        case HorizontalFormatting::Stretched:
            imgSz.d_width = destRect.getWidth();
            xpos = destRect.left();
            horzTiles = 1;
            break;

        case HorizontalFormatting::Tiled:
            xpos = destRect.left();
            horzTiles = std::abs(static_cast<int>(
                (destRect.getWidth() + (imgSz.d_width - 1)) / imgSz.d_width));
            break;

        case HorizontalFormatting::LeftAligned:
            xpos = destRect.left();
            horzTiles = 1;
            break;

        case HorizontalFormatting::CentreAligned:
            xpos = destRect.left() + CoordConverter::alignToPixels((destRect.getWidth() - imgSz.d_width) * 0.5f);
            horzTiles = 1;
            break;

        case HorizontalFormatting::RightAligned:
            xpos = destRect.right() - imgSz.d_width;
            horzTiles = 1;
            break;

        default:
            throw InvalidRequestException(
                "An unknown HorizontalFormatting value was specified.");
    }

    // calculate initial y co-ordinate and vertical tile count according to formatting options
    switch (vertFmt)
    {
        case VerticalImageFormatting::Stretched:
            imgSz.d_height = destRect.getHeight();
            ypos = destRect.top();
            vertTiles = 1;
            break;

        case VerticalImageFormatting::Tiled:
            ypos = destRect.top();
            vertTiles = std::abs(static_cast<int>(
                (destRect.getHeight() + (imgSz.d_height - 1)) / imgSz.d_height));
            break;

        case VerticalImageFormatting::TopAligned:
            ypos = destRect.top();
            vertTiles = 1;
            break;

        case VerticalImageFormatting::CentreAligned:
            ypos = destRect.top() + CoordConverter::alignToPixels((destRect.getHeight() - imgSz.d_height) * 0.5f);
            vertTiles = 1;
            break;

        case VerticalImageFormatting::BottomAligned:
            ypos = destRect.bottom() - imgSz.d_height;
            vertTiles = 1;
            break;

        default:
            throw InvalidRequestException(
                "An unknown VerticalFormatting value was specified.");
    }

    ImageRenderSettings renderSettings(Rectf(), nullptr, colours);

    Rectf& renderSettingDestArea = renderSettings.d_destArea;
    renderSettingDestArea.d_min.y = ypos;
    renderSettingDestArea.d_max.y = ypos + imgSz.d_height;

    for (unsigned int row = 0; row < vertTiles; ++row)
    {
        renderSettingDestArea.d_min.x = xpos;
        renderSettingDestArea.d_max.x = xpos + imgSz.d_width;

        for (unsigned int col = 0; col < horzTiles; ++col)
        {
            Rectf clipperRect;

            // use custom clipping for right and bottom edges when tiling the imagery
            if (((vertFmt == VerticalImageFormatting::Tiled) && row == vertTiles - 1) ||
                ((horzFmt == HorizontalFormatting::Tiled) && col == horzTiles - 1))
            {
                if(clipper)
                {
                    clipperRect = clipper->getIntersection(destRect);
                    renderSettings.d_clipArea = &clipperRect;
                }
                else
                {
                    renderSettings.d_clipArea = &destRect;
                }
            }
            // not tiling, or not on far edges, just used passed in clipper (if any).
            else
            {
                renderSettings.d_clipArea = clipper;
            }

            image->createRenderGeometry(out, renderSettings, canCombineFromIdx);

            renderSettingDestArea.d_min.x += imgSz.d_width;
            renderSettingDestArea.d_max.x += imgSz.d_width;
        }

        renderSettingDestArea.d_min.y += imgSz.d_height;
        renderSettingDestArea.d_max.y += imgSz.d_height;
    }
}

//----------------------------------------------------------------------------//
void FrameComponent::writeXMLToStream(XMLSerializer& xml_stream) const
{
    // opening tag
    xml_stream.openTag(Falagard_xmlHandler::FrameComponentElement);
    // write out area
    d_area.writeXMLToStream(xml_stream);

    // write images
    const int imageCount = static_cast<int>(FrameImageComponent::FrameImageCount);
    for (int i = 0; i < imageCount; ++i)
    {
        if (d_frameImages[i].d_specified)
        {
            if (d_frameImages[i].d_propertyName.empty())
                xml_stream.openTag(Falagard_xmlHandler::ImageElement)
                    .attribute(Falagard_xmlHandler::ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::toString(static_cast<FrameImageComponent>(i)))
                    .attribute(Falagard_xmlHandler::NameAttribute, d_frameImages[i].d_image->getName())
                    .closeTag();
            else
                xml_stream.openTag(Falagard_xmlHandler::ImagePropertyElement)
                    .attribute(Falagard_xmlHandler::ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::toString(static_cast<FrameImageComponent>(i)))
                    .attribute(Falagard_xmlHandler::NameAttribute, d_frameImages[i].d_propertyName)
                    .closeTag();
        }
    }

    // get base class to write colours
    writeColoursXML(xml_stream);

    if(d_leftEdgeFormatting.getValue() != VerticalFormattingDefault)
    {
        d_leftEdgeFormatting.writeXMLTagToStream(xml_stream);
        xml_stream.attribute(Falagard_xmlHandler::ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::LeftEdge);
        d_leftEdgeFormatting.writeXMLAttributesToStream(xml_stream);
        xml_stream.closeTag();
    }

    if(d_rightEdgeFormatting.getValue() != VerticalFormattingDefault)
    {
        d_rightEdgeFormatting.writeXMLTagToStream(xml_stream);
        xml_stream.attribute(Falagard_xmlHandler::ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::RightEdge);
        d_rightEdgeFormatting.writeXMLAttributesToStream(xml_stream);
        xml_stream.closeTag();
    }

    if(d_backgroundHorzFormatting.getValue() != HorizontalFormattingDefault)
    {
        d_backgroundHorzFormatting.writeXMLTagToStream(xml_stream);
        xml_stream.attribute(Falagard_xmlHandler::ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::Background);
        d_backgroundHorzFormatting.writeXMLAttributesToStream(xml_stream);
        xml_stream.closeTag();
    }

    if(d_topEdgeFormatting.getValue() != HorizontalFormattingDefault)
    {
        d_topEdgeFormatting.writeXMLTagToStream(xml_stream);
        xml_stream.attribute(Falagard_xmlHandler::ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::TopEdge);
        d_topEdgeFormatting.writeXMLAttributesToStream(xml_stream);
        xml_stream.closeTag();
    }

    if(d_bottomEdgeFormatting.getValue() != HorizontalFormattingDefault)
    {
        d_bottomEdgeFormatting.writeXMLTagToStream(xml_stream);
        xml_stream.attribute(Falagard_xmlHandler::ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::BottomEdge);
        d_bottomEdgeFormatting.writeXMLAttributesToStream(xml_stream);
        xml_stream.closeTag();
    }

    if(d_backgroundVertFormatting.getValue() != VerticalFormattingDefault)
    {
        d_backgroundVertFormatting.writeXMLTagToStream(xml_stream);
        xml_stream.attribute(Falagard_xmlHandler::ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::Background);
        d_backgroundVertFormatting.writeXMLAttributesToStream(xml_stream);
        xml_stream.closeTag();
    }

    // closing tag
    xml_stream.closeTag();
}

//----------------------------------------------------------------------------//
bool FrameComponent::operator==(const FrameComponent& rhs) const
{
    if (d_backgroundVertFormatting != rhs.d_backgroundVertFormatting ||
        d_backgroundHorzFormatting != rhs.d_backgroundHorzFormatting)
            return false;

    const int frameImageCount = static_cast<int>(FrameImageComponent::FrameImageCount);

    for (int i = 0; i < frameImageCount; ++i)
        if (d_frameImages[i] != rhs.d_frameImages[i])
            return false;

    return true;
}

//----------------------------------------------------------------------------//

}

