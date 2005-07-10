/************************************************************************
    filename:   CEGUIFalDimensions.cpp
    created:    Mon Jun 13 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
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
#include "falagard/CEGUIFalDimensions.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"
#include "CEGUIWindowManager.h"
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include <cassert>

// Start of CEGUI namespace section
namespace CEGUI
{
    AbsoluteDim::AbsoluteDim(float val) :
        d_val(val)
    {}

    void AbsoluteDim::setValue(float val)
    {
        d_val = val;
    }

    float AbsoluteDim::getValue(const Window& wnd) const
    {
        return d_val;
    }

    float AbsoluteDim::getValue(const Window& wnd, const Rect& container) const
    {
        return d_val;
    }

    BaseDim* AbsoluteDim::clone() const
    {
        AbsoluteDim* ndim = new AbsoluteDim(d_val);
        return ndim;
    }

    ////////////////////////////////////////////////////////////////////////////////

    ImageDim::ImageDim(const String& imageset, const String& image, DimensionType dim) :
        d_imageset(imageset),
        d_image(image),
        d_what(dim)
    {}

    void ImageDim::setSourceImage(const String& imageset, const String& image)
    {
        d_imageset = imageset;
        d_image = image;
    }

    void ImageDim::setSourceDimension(DimensionType dim)
    {
        d_what = dim;
    }

    float ImageDim::getValue(const Window& wnd) const
    {
        const Image* img = &ImagesetManager::getSingleton().getImageset(d_imageset)->getImage(d_image);

        switch (d_what)
        {
            case DT_WIDTH:
                return img->getWidth();
                break;

            case DT_HEIGHT:
                return img->getHeight();
                break;

            case DT_X_OFFSET:
                return img->getOffsetX();
                break;

            case DT_Y_OFFSET:
                return img->getOffsetY();
                break;

            // these other options will not be particularly useful for most people since they return the edges of the
            // image on the source texture.
            case DT_LEFT_EDGE:
            case DT_X_POSITION:
                return img->getSourceTextureArea().d_left;
                break;

            case DT_TOP_EDGE:
            case DT_Y_POSITION:
                return img->getSourceTextureArea().d_top;
                break;

            case DT_RIGHT_EDGE:
                return img->getSourceTextureArea().d_right;
                break;

            case DT_BOTTOM_EDGE:
                return img->getSourceTextureArea().d_bottom;
                break;

            default:
                throw InvalidRequestException("ImageDim::getValue - unknown or unsupported DimensionType encountered.");
                break;
        }
    }

    float ImageDim::getValue(const Window& wnd, const Rect& container) const
    {
        // This dimension type does not alter when whithin a container Rect.
        return getValue(wnd);
    }


    BaseDim* ImageDim::clone() const
    {
        ImageDim* ndim = new ImageDim(d_imageset, d_image, d_what);
        return ndim;
    }

    ////////////////////////////////////////////////////////////////////////////////

    WidgetDim::WidgetDim(const String& name, DimensionType dim) :
        d_widgetName(name),
        d_what(dim)
    {}

    void WidgetDim::setWidgetName(const String& name)
    {
        d_widgetName = name;
    }

    void WidgetDim::setSourceDimension(DimensionType dim)
    {
        d_what = dim;
    }

    float WidgetDim::getValue(const Window& wnd) const
    {
        const Window* widget;

        // if target widget name is empty, then use the input window.
        if (d_widgetName.empty())
        {
            widget = &wnd;
        }
        // name not empty, so find window with required name
        else
        {
            widget = WindowManager::getSingleton().getWindow(wnd.getName() + d_widgetName);
        }

        switch (d_what)
        {
            case DT_WIDTH:
                return widget->getAbsoluteWidth();
                break;

            case DT_HEIGHT:
                return widget->getAbsoluteHeight();
                break;

            case DT_X_OFFSET:
                Logger::getSingleton().logEvent("WigetDim::getValue - Nonsensical DimensionType of DT_X_OFFSET specified!  returning 0.0f");
                return 0.0f;
                break;

            case DT_Y_OFFSET:
                Logger::getSingleton().logEvent("WigetDim::getValue - Nonsensical DimensionType of DT_Y_OFFSET specified!  returning 0.0f");
                return 0.0f;
                break;

            case DT_LEFT_EDGE:
            case DT_X_POSITION:
                return widget->getAbsolutePosition().d_x;
                break;

            case DT_TOP_EDGE:
            case DT_Y_POSITION:
                return widget->getAbsolutePosition().d_y;
                break;

            case DT_RIGHT_EDGE:
                return widget->getAbsoluteRect().d_right;
                break;

            case DT_BOTTOM_EDGE:
                return widget->getAbsoluteRect().d_bottom;
                break;

            default:
                throw InvalidRequestException("WidgetDim::getValue - unknown or unsupported DimensionType encountered.");
                break;
        }
    }

    float WidgetDim::getValue(const Window& wnd, const Rect& container) const
    {
        // This dimension type does not alter when whithin a container Rect.
        return getValue(wnd);
    }

    BaseDim* WidgetDim::clone() const
    {
        WidgetDim* ndim = new WidgetDim(d_widgetName, d_what);
        return ndim;
    }

    ////////////////////////////////////////////////////////////////////////////////

    Dimension::Dimension()
    {
        d_value = 0;
        d_type = DT_INVALID;
    }

    Dimension::~Dimension()
    {
        if (d_value)
            delete d_value;
    }

    Dimension::Dimension(const BaseDim& dim, DimensionType type)
    {
        d_value = dim.clone();
        d_type = type;
    }

    Dimension::Dimension(const Dimension& other)
    {
        d_value = other.d_value ? other.d_value->clone() : 0;
        d_type = other.d_type;
    }

    Dimension& Dimension::operator=(const Dimension& other)
    {
        d_value = other.d_value ? other.d_value->clone() : 0;
        d_type = other.d_type;

		return *this;
    }

    const BaseDim& Dimension::getBaseDimension() const
    {
        assert(d_value);
        return *d_value;
    }

    void Dimension::setBaseDimension(const BaseDim& dim)
    {
        d_value = dim.clone();
    }

    DimensionType Dimension::getDimensionType() const
    {
        return d_type;
    }

    void Dimension::setDimensionType(DimensionType type)
    {
        d_type = type;
    }

    ////////////////////////////////////////////////////////////////////////////////

    UnifiedDim::UnifiedDim(const UDim& value, DimensionType dim) :
        d_value(value),
        d_what(dim)
    {
    }

    float UnifiedDim::getValue(const Window& wnd) const
    {
        switch (d_what)
        {
            case DT_LEFT_EDGE:
            case DT_RIGHT_EDGE:
            case DT_X_POSITION:
            case DT_X_OFFSET:
            case DT_WIDTH:
                return d_value.asAbsolute(wnd.getAbsoluteWidth());
                break;

            case DT_TOP_EDGE:
            case DT_BOTTOM_EDGE:
            case DT_Y_POSITION:
            case DT_Y_OFFSET:
            case DT_HEIGHT:
                return d_value.asAbsolute(wnd.getAbsoluteHeight());
                break;

            default:
                throw InvalidRequestException("UnifiedDim::getValue - unknown or unsupported DimensionType encountered.");
                break;
        }
    }

    float UnifiedDim::getValue(const Window& wnd, const Rect& container) const
    {
        switch (d_what)
        {
            case DT_LEFT_EDGE:
            case DT_RIGHT_EDGE:
            case DT_X_POSITION:
            case DT_X_OFFSET:
            case DT_WIDTH:
                return d_value.asAbsolute(container.getWidth());
                break;

            case DT_TOP_EDGE:
            case DT_BOTTOM_EDGE:
            case DT_Y_POSITION:
            case DT_Y_OFFSET:
            case DT_HEIGHT:
                return d_value.asAbsolute(container.getHeight());
                break;

            default:
                throw InvalidRequestException("UnifiedDim::getValue - unknown or unsupported DimensionType encountered.");
                break;
        }
    }

    BaseDim* UnifiedDim::clone() const
    {
        UnifiedDim* ndim = new UnifiedDim(d_value, d_what);
        return ndim;
    }

    ////////////////////////////////////////////////////////////////////////////////

    Rect ComponentArea::getPixelRect(const Window& wnd) const
    {
        // sanity check, we mus be able to form a Rect from what we represent.
        assert(d_left.getDimensionType() == DT_LEFT_EDGE || d_left.getDimensionType() == DT_X_POSITION);
        assert(d_top.getDimensionType() == DT_TOP_EDGE || d_top.getDimensionType() == DT_Y_POSITION);
        assert(d_right_or_width.getDimensionType() == DT_RIGHT_EDGE || d_right_or_width.getDimensionType() == DT_WIDTH);
        assert(d_bottom_or_height.getDimensionType() == DT_BOTTOM_EDGE || d_bottom_or_height.getDimensionType() == DT_HEIGHT);

        Rect pixelRect;
        pixelRect.d_left = d_left.getBaseDimension().getValue(wnd);
        pixelRect.d_top = d_top.getBaseDimension().getValue(wnd);

        if (d_right_or_width.getDimensionType() == DT_WIDTH)
            pixelRect.setWidth(d_right_or_width.getBaseDimension().getValue(wnd));
        else
            pixelRect.d_right = d_right_or_width.getBaseDimension().getValue(wnd);

        if (d_bottom_or_height.getDimensionType() == DT_HEIGHT)
            pixelRect.setHeight(d_bottom_or_height.getBaseDimension().getValue(wnd));
        else
            pixelRect.d_bottom = d_bottom_or_height.getBaseDimension().getValue(wnd);

        return pixelRect;
    }

    Rect ComponentArea::getPixelRect(const Window& wnd, const Rect& container) const
    {
        // sanity check, we mus be able to form a Rect from what we represent.
        assert(d_left.getDimensionType() == DT_LEFT_EDGE || d_left.getDimensionType() == DT_X_POSITION);
        assert(d_top.getDimensionType() == DT_TOP_EDGE || d_top.getDimensionType() == DT_Y_POSITION);
        assert(d_right_or_width.getDimensionType() == DT_RIGHT_EDGE || d_right_or_width.getDimensionType() == DT_WIDTH);
        assert(d_bottom_or_height.getDimensionType() == DT_BOTTOM_EDGE || d_bottom_or_height.getDimensionType() == DT_HEIGHT);

        Rect pixelRect;
        pixelRect.d_left = d_left.getBaseDimension().getValue(wnd, container) + container.d_left;
        pixelRect.d_top = d_top.getBaseDimension().getValue(wnd, container) + container.d_top;

        if (d_right_or_width.getDimensionType() == DT_WIDTH)
            pixelRect.setWidth(d_right_or_width.getBaseDimension().getValue(wnd, container));
        else
            pixelRect.d_right = d_right_or_width.getBaseDimension().getValue(wnd, container) + container.d_left;

        if (d_bottom_or_height.getDimensionType() == DT_HEIGHT)
            pixelRect.setHeight(d_bottom_or_height.getBaseDimension().getValue(wnd, container));
        else
            pixelRect.d_bottom = d_bottom_or_height.getBaseDimension().getValue(wnd, container) + container.d_top;

        return pixelRect;
    }

} // End of  CEGUI namespace section
