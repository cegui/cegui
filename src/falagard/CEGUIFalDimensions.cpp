/***********************************************************************
    filename:   CEGUIFalDimensions.cpp
    created:    Mon Jun 13 2005
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
#include "falagard/CEGUIFalDimensions.h"
#include "falagard/CEGUIFalXMLEnumHelper.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"
#include "CEGUIWindowManager.h"
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIPropertyHelper.h"
#include <cassert>

// Start of CEGUI namespace section
namespace CEGUI
{
    BaseDim::BaseDim() :
        d_operator(DOP_NOOP),
        d_operand(0)
    {
    }

    BaseDim::~BaseDim()
    {
        delete d_operand;
    }

    float BaseDim::getValue(const Window& wnd) const
    {
        // get sub-class to return value for this dimension.
        float val = getValue_impl(wnd);

        // if we have an attached operand, perform math on value as needed
        if (d_operand)
        {
            switch (d_operator)
            {
            case DOP_ADD:
                val += d_operand->getValue(wnd);
                break;
            case DOP_SUBTRACT:
                val -= d_operand->getValue(wnd);
                break;
            case DOP_MULTIPLY:
                val *= d_operand->getValue(wnd);
                break;
            case DOP_DIVIDE:
                val /= d_operand->getValue(wnd);
                break;
            default:
                // No-op.
                break;
            }
        }

        return val;
    }

    float BaseDim::getValue(const Window& wnd, const Rect& container) const
    {
        // get sub-class to return value for this dimension.
        float val = getValue_impl(wnd, container);

        // if we have an attached operand, perform math on value as needed
        if (d_operand)
        {
            switch (d_operator)
            {
            case DOP_ADD:
                val += d_operand->getValue(wnd, container);
                break;
            case DOP_SUBTRACT:
                val -= d_operand->getValue(wnd, container);
                break;
            case DOP_MULTIPLY:
                val *= d_operand->getValue(wnd, container);
                break;
            case DOP_DIVIDE:
                val /= d_operand->getValue(wnd, container);
                break;
            default:
                // No-op.
                break;
            }
        }

        return val;
    }

    BaseDim* BaseDim::clone() const
    {
        // get sub-class to return a cloned object
        BaseDim* o = clone_impl();

        // fill in operator for cloned object
        o->d_operator = d_operator;

        // now clone any attached operand dimension
        if (d_operand)
            o->d_operand = d_operand->clone();

        return o;
    }

    DimensionOperator BaseDim::getDimensionOperator() const
    {
        return d_operator;
    }

    void BaseDim::setDimensionOperator(DimensionOperator op)
    {
        d_operator = op;
    }

    const BaseDim* BaseDim::getOperand() const
    {
        return d_operand;
    }

    void BaseDim::setOperand(const BaseDim& operand)
    {
        // release old operand, if any.
        if(d_operand) delete d_operand;

        d_operand = operand.clone();
    }

    void BaseDim::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        // get sub-class to output the data for this single dimension
        writeXMLElementName_impl(xml_stream);
        writeXMLElementAttributes_impl(xml_stream);
        if (d_operand)
        {
            // write out the DimOperator
            xml_stream.openTag("DimOperator")
                .attribute("op", FalagardXMLHelper::dimensionOperatorToString(d_operator));
            // write out the other operand
            d_operand->writeXMLToStream(xml_stream);
            // write closing tag for DimOperator element
            xml_stream.closeTag();
        }
        xml_stream.closeTag();
    }

    ////////////////////////////////////////////////////////////////////////////////

    AbsoluteDim::AbsoluteDim(float val) :
        d_val(val)
    {}

    void AbsoluteDim::setValue(float val)
    {
        d_val = val;
    }

    float AbsoluteDim::getValue_impl(const Window& wnd) const
    {
        return d_val;
    }

    float AbsoluteDim::getValue_impl(const Window& wnd, const Rect& container) const
    {
        return d_val;
    }

    BaseDim* AbsoluteDim::clone_impl() const
    {
        AbsoluteDim* ndim = new AbsoluteDim(d_val);
        return ndim;
    }

    void AbsoluteDim::writeXMLElementName_impl(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("AbsoluteDim");
    }

    void AbsoluteDim::writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const
    {
        xml_stream.attribute("value", PropertyHelper::floatToString(d_val));
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

    float ImageDim::getValue_impl(const Window& wnd) const
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

    float ImageDim::getValue_impl(const Window& wnd, const Rect& container) const
    {
        // This dimension type does not alter when whithin a container Rect.
        return getValue(wnd);
    }


    BaseDim* ImageDim::clone_impl() const
    {
        ImageDim* ndim = new ImageDim(d_imageset, d_image, d_what);
        return ndim;
    }

    void ImageDim::writeXMLElementName_impl(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("ImageDim");
    }

    void ImageDim::writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const
    {
        xml_stream.attribute("imageset", d_imageset)
            .attribute("image", d_image)
            .attribute("dimension", FalagardXMLHelper::dimensionTypeToString(d_what));
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

    float WidgetDim::getValue_impl(const Window& wnd) const
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

        // get size of parent; required to extract pixel values
        Size parentSize(widget->getParentPixelSize());

        switch (d_what)
        {
            case DT_WIDTH:
                return widget->getPixelSize().d_width;
                break;

            case DT_HEIGHT:
                return widget->getPixelSize().d_height;
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
                return widget->getPosition().d_x.asAbsolute(parentSize.d_width);
                break;

            case DT_TOP_EDGE:
            case DT_Y_POSITION:
                return widget->getPosition().d_y.asAbsolute(parentSize.d_height);
                break;

            case DT_RIGHT_EDGE:
                return widget->getArea().d_max.d_x.asAbsolute(parentSize.d_width);
                break;

            case DT_BOTTOM_EDGE:
                return widget->getArea().d_max.d_y.asAbsolute(parentSize.d_height);
                break;

            default:
                throw InvalidRequestException("WidgetDim::getValue - unknown or unsupported DimensionType encountered.");
                break;
        }
    }

    float WidgetDim::getValue_impl(const Window& wnd, const Rect& container) const
    {
        // This dimension type does not alter when whithin a container Rect.
        return getValue(wnd);
    }

    BaseDim* WidgetDim::clone_impl() const
    {
        WidgetDim* ndim = new WidgetDim(d_widgetName, d_what);
        return ndim;
    }

    void WidgetDim::writeXMLElementName_impl(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("WidgetDim");
    }

    void WidgetDim::writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const
    {
        if (!d_widgetName.empty())
            xml_stream.attribute("widget", d_widgetName);

        xml_stream.attribute("dimension", FalagardXMLHelper::dimensionTypeToString(d_what));
    }

    ////////////////////////////////////////////////////////////////////////////////

    FontDim::FontDim(const String& name, const String& font, const String& text, FontMetricType metric, float padding) :
        d_font(font),
        d_text(text),
        d_childSuffix(name),
        d_metric(metric),
        d_padding(padding)
    {
    }

    float FontDim::getValue_impl(const Window& wnd) const
    {
        // get window to use.
        const Window& sourceWindow = d_childSuffix.empty() ? wnd : *WindowManager::getSingleton().getWindow(wnd.getName() + d_childSuffix);
        // get font to use
        Font* fontObj = d_font.empty() ? sourceWindow.getFont() : FontManager::getSingleton().getFont(d_font);

        if (fontObj)
        {
            switch (d_metric)
            {
                case FMT_LINE_SPACING:
                    return fontObj->getLineSpacing() + d_padding;
                    break;
                case FMT_BASELINE:
                    return fontObj->getBaseline() + d_padding;
                    break;
                case FMT_HORZ_EXTENT:
                    return fontObj->getTextExtent(d_text.empty() ? sourceWindow.getText() : d_text) + d_padding;
                    break;
                default:
                    throw InvalidRequestException("FontDim::getValue - unknown or unsupported FontMetricType encountered.");
                    break;
            }
        }
        // no font, return padding value only.
        else
        {
            return d_padding;
        }
    }

    float FontDim::getValue_impl(const Window& wnd, const Rect& container) const
    {
        return getValue(wnd);
    }

    BaseDim* FontDim::clone_impl() const
    {
        FontDim* ndim = new FontDim(d_childSuffix, d_font, d_text, d_metric, d_padding);
        return ndim;
    }

    void FontDim::writeXMLElementName_impl(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("FontDim");
    }

    void FontDim::writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const
    {
        if (!d_childSuffix.empty())
            xml_stream.attribute("widget", d_childSuffix);

        if (!d_font.empty())
            xml_stream.attribute("font", d_font);

        if (!d_text.empty())
            xml_stream.attribute("string", d_text);

        if (d_padding != 0)
            xml_stream.attribute("padding", PropertyHelper::floatToString(d_padding));

        xml_stream.attribute("type", FalagardXMLHelper::fontMetricTypeToString(d_metric));
    }

    ////////////////////////////////////////////////////////////////////////////////

    PropertyDim::PropertyDim(const String& name, const String& property,
	    DimensionType type) :
        d_property(property),
        d_childSuffix(name),
		d_type (type)
    {
    }

    float PropertyDim::getValue_impl(const Window& wnd) const
    {
        // get window to use.
        const Window& sourceWindow = d_childSuffix.empty() ? wnd : *WindowManager::getSingleton().getWindow(wnd.getName() + d_childSuffix);

        if (d_type == DT_INVALID)
            // return float property value.
            return PropertyHelper::stringToFloat(sourceWindow.getProperty(d_property));

        UDim d = PropertyHelper::stringToUDim(sourceWindow.getProperty(d_property));
        Size s = sourceWindow.getPixelSize();

        switch (d_type)
        {
            case DT_WIDTH:
                return d.asAbsolute(s.d_width);

            case DT_HEIGHT:
                return d.asAbsolute(s.d_height);

            default:
                throw InvalidRequestException("PropertyDim::getValue - unknown or unsupported DimensionType encountered.");
        }
    }

    float PropertyDim::getValue_impl(const Window& wnd, const Rect& container) const
    {
        return getValue(wnd);
    }

    BaseDim* PropertyDim::clone_impl() const
    {
        PropertyDim* ndim = new PropertyDim(d_childSuffix, d_property, d_type);
        return ndim;
    }

    void PropertyDim::writeXMLElementName_impl(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("PropertyDim");
    }

    void PropertyDim::writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const
    {
        if (!d_childSuffix.empty())
            xml_stream.attribute("widget", d_childSuffix);
        xml_stream.attribute("name", d_property);
        if (d_type != DT_INVALID)
            xml_stream.attribute("type", FalagardXMLHelper::dimensionTypeToString(d_type));
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
        // release old value, if any.
        if (d_value)  delete d_value;

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
        // release old value, if any.
        if (d_value)  delete d_value;

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

    void Dimension::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("Dim")
            .attribute("type", FalagardXMLHelper::dimensionTypeToString(d_type));

        if (d_value)
            d_value->writeXMLToStream(xml_stream);
        xml_stream.closeTag();
    }

    ////////////////////////////////////////////////////////////////////////////////

    UnifiedDim::UnifiedDim(const UDim& value, DimensionType dim) :
        d_value(value),
        d_what(dim)
    {
    }

    float UnifiedDim::getValue_impl(const Window& wnd) const
    {
        switch (d_what)
        {
            case DT_LEFT_EDGE:
            case DT_RIGHT_EDGE:
            case DT_X_POSITION:
            case DT_X_OFFSET:
            case DT_WIDTH:
                return d_value.asAbsolute(wnd.getPixelSize().d_width);
                break;

            case DT_TOP_EDGE:
            case DT_BOTTOM_EDGE:
            case DT_Y_POSITION:
            case DT_Y_OFFSET:
            case DT_HEIGHT:
                return d_value.asAbsolute(wnd.getPixelSize().d_height);
                break;

            default:
                throw InvalidRequestException("UnifiedDim::getValue - unknown or unsupported DimensionType encountered.");
                break;
        }
    }

    float UnifiedDim::getValue_impl(const Window& wnd, const Rect& container) const
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

    BaseDim* UnifiedDim::clone_impl() const
    {
        UnifiedDim* ndim = new UnifiedDim(d_value, d_what);
        return ndim;
    }

    void UnifiedDim::writeXMLElementName_impl(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("UnifiedDim");
    }

    void UnifiedDim::writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const
    {
        if (d_value.d_scale != 0)
            xml_stream.attribute("scale", PropertyHelper::floatToString(d_value.d_scale));

        if (d_value.d_offset != 0)
            xml_stream.attribute("offset", PropertyHelper::floatToString(d_value.d_offset));

        xml_stream.attribute("type", FalagardXMLHelper::dimensionTypeToString(d_what));
    }

    ////////////////////////////////////////////////////////////////////////////////

    Rect ComponentArea::getPixelRect(const Window& wnd) const
    {
        Rect pixelRect;

        // use a property?
        if (isAreaFetchedFromProperty())
        {
            pixelRect = PropertyHelper::stringToURect(wnd.getProperty(d_areaProperty)).asAbsolute(wnd.getPixelSize());
        }
        // not via property - calculate using Dimensions
        else
        {
            // sanity check, we mus be able to form a Rect from what we represent.
            assert(d_left.getDimensionType() == DT_LEFT_EDGE || d_left.getDimensionType() == DT_X_POSITION);
            assert(d_top.getDimensionType() == DT_TOP_EDGE || d_top.getDimensionType() == DT_Y_POSITION);
            assert(d_right_or_width.getDimensionType() == DT_RIGHT_EDGE || d_right_or_width.getDimensionType() == DT_WIDTH);
            assert(d_bottom_or_height.getDimensionType() == DT_BOTTOM_EDGE || d_bottom_or_height.getDimensionType() == DT_HEIGHT);

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
        }

        return pixelRect;
    }

    Rect ComponentArea::getPixelRect(const Window& wnd, const Rect& container) const
    {
        Rect pixelRect;

        // use a property?
        if (isAreaFetchedFromProperty())
        {
            pixelRect = PropertyHelper::stringToURect(wnd.getProperty(d_areaProperty)).asAbsolute(wnd.getPixelSize());
        }
        // not via property - calculate using Dimensions
        else
        {
            // sanity check, we mus be able to form a Rect from what we represent.
            assert(d_left.getDimensionType() == DT_LEFT_EDGE || d_left.getDimensionType() == DT_X_POSITION);
            assert(d_top.getDimensionType() == DT_TOP_EDGE || d_top.getDimensionType() == DT_Y_POSITION);
            assert(d_right_or_width.getDimensionType() == DT_RIGHT_EDGE || d_right_or_width.getDimensionType() == DT_WIDTH);
            assert(d_bottom_or_height.getDimensionType() == DT_BOTTOM_EDGE || d_bottom_or_height.getDimensionType() == DT_HEIGHT);

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
        }

        return pixelRect;
    }

    void ComponentArea::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("Area");

        // see if we should write an AreaProperty element
        if (isAreaFetchedFromProperty())
        {
            xml_stream.openTag("AreaProperty")
                .attribute("name", d_areaProperty)
                .closeTag();
        }
        // not a property, write out individual dimensions explicitly.
        else
        {
            d_left.writeXMLToStream(xml_stream);
            d_top.writeXMLToStream(xml_stream);
            d_right_or_width.writeXMLToStream(xml_stream);
            d_bottom_or_height.writeXMLToStream(xml_stream);
        }
        xml_stream.closeTag();
    }

    bool ComponentArea::isAreaFetchedFromProperty() const
    {
        return !d_areaProperty.empty();
    }

    const String& ComponentArea::getAreaPropertySource() const
    {
        return d_areaProperty;
    }

    void ComponentArea::setAreaPropertySource(const String& property)
    {
        d_areaProperty = property;
    }

} // End of  CEGUI namespace section
