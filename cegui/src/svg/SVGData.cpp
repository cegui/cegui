/***********************************************************************
    created:    30th July 2013
    author:     Lukas Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/svg/SVGData.h"
#include "CEGUI/svg/SVGBasicShape.h"
#include "CEGUI/ResourceProvider.h"
#include "CEGUI/System.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/SharedStringStream.h"
// for the XML parsing part.
#include "CEGUI/XMLParser.h"
#include "CEGUI/XMLAttributes.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
// Internal Strings holding XML element and attribute names
const String ImagesetSchemaName("Imageset.xsd");
//SVG elements and attributes
const String SVGElement( "svg" );
const String SVGElementAttributeVersion( "version" );
const String SVGElementAttributeWidth( "width" );
const String SVGElementAttributeHeight( "height" );

//SVG Basic Shapes
const String SVGRectElement( "rect" );
const String SVGCircleElement( "circle" );
const String SVGEllipseElement( "ellipse" );
const String SVGLineElement( "line" );
const String SVGPolylineElement( "polyline" );
const String SVGPolygonElement( "polygon" );

// SVG graphics elements paint attributes
const String SVGGraphicsElementAttributeFill( "fill" );
const String SVGGraphicsElementAttributeFillRule( "fill-rule" );
const String SVGGraphicsElementAttributeFillOpacity( "fill-opacity" );
const String SVGGraphicsElementAttributeStroke( "stroke" );
const String SVGGraphicsElementAttributeStrokeWidth( "stroke-width" );
const String SVGGraphicsElementAttributeStrokeLinecap( "stroke-linecap" );
const String SVGGraphicsElementAttributeStrokeLinejoin( "stroke-linejoin" );
const String SVGGraphicsElementAttributeStrokeMiterLimit( "stroke-miterlimit" );
const String SVGGraphicsElementAttributeStrokeDashArray( "stroke-dasharray" );
const String SVGGraphicsElementAttributeStrokeDashOffset( "stroke-dashoffset" );
const String SVGGraphicsElementAttributeStrokeOpacity( "stroke-opacity" );

// SVG transform attribute
const String SVGTransformAttribute( "transform" );

// SVG 'rect' element attributes
const String SVGRectAttributeXPos( "x" );
const String SVGRectAttributeYPos( "y" );
const String SVGRectAttributeWidth( "width" );
const String SVGRectAttributeHeight( "height" );
const String SVGRectAttributeRoundedX( "rx" );
const String SVGRectAttributeRoundedY( "ry" );
// SVG 'circle' element attributes
const String SVGCircleAttributeCX( "cx" );
const String SVGCircleAttributeCY( "cy" );
const String SVGCircleAttributeRadius( "r" );
// SVG 'ellipse' element attributes
const String SVGEllipseAttributeCX( "cx" );
const String SVGEllipseAttributeCY( "cy" );
const String SVGEllipseAttributeRX( "rx" );
const String SVGEllipseAttributeRY( "ry" );
// SVG 'polyline' element attributes
const String SVGPolylineAttributePoints( "points" );
// SVG 'polyline' element attributes
const String SVGLineAttributeX1( "x1" );
const String SVGLineAttributeY1( "y1" );
const String SVGLineAttributeX2( "x2" );
const String SVGLineAttributeY2( "y2" );

//----------------------------------------------------------------------------//
SVGData::SVGData(const String& name)
    : d_name(name)
    , d_width(0.f)
    , d_height(0.f)
{
}

//----------------------------------------------------------------------------//
SVGData::SVGData(const String& name,
                 const String& filename,
                 const String& resourceGroup) :
    d_name(name)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
SVGData::~SVGData()
{
    destroyShapes();
}

//----------------------------------------------------------------------------//
const String& SVGData::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
void SVGData::loadFromFile(const String& file_name,
                           const String& resource_group)
{
    System::getSingleton().getXMLParser()->parseXMLFile(
            *this, file_name,
            "", resource_group, false);
}

//----------------------------------------------------------------------------//
void SVGData::addShape(SVGBasicShape* svg_shape)
{
    d_svgBasicShapes.push_back(svg_shape);
}

//----------------------------------------------------------------------------//
void SVGData::destroyShapes()
{
    for (auto shape : d_svgBasicShapes)
        delete shape;

    d_svgBasicShapes.clear();
}

//----------------------------------------------------------------------------//
const std::vector<SVGBasicShape*>& SVGData::getShapes() const
{
    return d_svgBasicShapes;
}

//----------------------------------------------------------------------------//
float SVGData::getWidth() const
{
    return d_width;
}

//----------------------------------------------------------------------------//
void SVGData::setWidth(float width)
{
    d_width = width;
}

//----------------------------------------------------------------------------//
float SVGData::getHeight() const
{
    return d_height;
}

//----------------------------------------------------------------------------//
void SVGData::setHeight(float height)
{
    d_height = height;
}

//----------------------------------------------------------------------------//
void SVGData::elementStartLocal(const String& element,
                                const XMLAttributes& attributes)
{
    // handle SVG document fragment element
    if (element == SVGElement)
    {
        elementSVGStart(attributes);
    }

    // handle SVG 'rect' element
    else if(element == SVGRectElement)
    {
        elementSVGRect(attributes);
    }
    // handle SVG 'circle' fragment element
    else if(element == SVGCircleElement)
    {
        elementSVGCircle(attributes);
    }
    // handle SVG 'ellipse' element
    else if(element == SVGEllipseElement)
    {
        elementSVGEllipse(attributes);
    }
    // handle SVG 'line' element
    else if(element == SVGLineElement)
    {
        elementSVGLine(attributes);
    }
    // handle SVG 'polyline' element
    else if(element == SVGPolylineElement)
    {
        elementSVGPolyline(attributes);
    }
    // handle SVG document fragment element
    else if(element == SVGPolygonElement)
    {
        elementSVGPolygon(attributes);
    }
}

//----------------------------------------------------------------------------//
void SVGData::elementSVGStart(const XMLAttributes& attributes)
{
    // Get the SVG version
    const String version(
        attributes.getValueAsString(SVGElementAttributeVersion));

    // Todo: Currently we only support pixels as units and interpret everything as
    // such, probably some default conversion from inch/mm to pixels should happen
    const String width(
        attributes.getValueAsString(SVGElementAttributeWidth));
    d_width = parseLengthDataType(width).d_value;

    const String height(
        attributes.getValueAsString(SVGElementAttributeHeight));
    d_height = parseLengthDataType(height).d_value;
}

//----------------------------------------------------------------------------//
void SVGData::elementSVGRect(const XMLAttributes& attributes)
{
    SVGPaintStyle paint_style = parsePaintStyle(attributes);
    glm::mat3x3 transform = parseTransform(attributes);

    const String xString(
        attributes.getValueAsString(SVGRectAttributeXPos, "0"));
    float x = parseLengthDataType(xString).d_value;

    const String yString(
        attributes.getValueAsString(SVGRectAttributeYPos, "0"));
    float y = parseLengthDataType(yString).d_value;

    const String widthString(
        attributes.getValueAsString(SVGRectAttributeWidth, "0"));
    float width = parseLengthDataType(widthString).d_value;

    const String heightString(
        attributes.getValueAsString(SVGRectAttributeHeight, "0"));
    float height = parseLengthDataType(heightString).d_value;

    const String rxString(
        attributes.getValueAsString(SVGRectAttributeRoundedX, "0"));
    float rx = parseLengthDataType(rxString).d_value;

    const String ryString(
        attributes.getValueAsString(SVGRectAttributeRoundedY, "0"));
    float ry = parseLengthDataType(ryString).d_value;

    SVGRect* rect = new SVGRect(paint_style, transform, x, y, width, height, rx, ry);
    addShape(rect);
}

//----------------------------------------------------------------------------//
void SVGData::elementSVGCircle(const XMLAttributes& attributes)
{
    SVGPaintStyle paint_style = parsePaintStyle(attributes);
    glm::mat3x3 transform = parseTransform(attributes);

    const String cxString(
        attributes.getValueAsString(SVGCircleAttributeCX, "0"));
    float cx = parseLengthDataType(cxString).d_value;

    const String cyString(
        attributes.getValueAsString(SVGCircleAttributeCY, "0"));
    float cy = parseLengthDataType(cyString).d_value;

    const String radiusString(
        attributes.getValueAsString(SVGCircleAttributeRadius, "0"));
    float radius = parseLengthDataType(radiusString).d_value;

    SVGCircle* circle = new SVGCircle(paint_style, transform, cx, cy, radius);
    addShape(circle);  
}

//----------------------------------------------------------------------------//
void SVGData::elementSVGEllipse(const XMLAttributes& attributes)
{
    SVGPaintStyle paint_style = parsePaintStyle(attributes);
    glm::mat3x3 transform = parseTransform(attributes);

    const String cxString(
        attributes.getValueAsString(SVGEllipseAttributeCX, "0"));
    float cx = parseLengthDataType(cxString).d_value;

    const String cyString(
        attributes.getValueAsString(SVGEllipseAttributeCY, "0"));
    float cy = parseLengthDataType(cyString).d_value;

    const String rxString(
        attributes.getValueAsString(SVGEllipseAttributeRX, "0"));
    float rx = parseLengthDataType(rxString).d_value;

    const String ryString(
        attributes.getValueAsString(SVGEllipseAttributeRY, "0"));
    float ry = parseLengthDataType(ryString).d_value;

    SVGEllipse* ellipse = new SVGEllipse(paint_style, transform, cx, cy, rx, ry);
    addShape(ellipse);  
}

//----------------------------------------------------------------------------//
void SVGData::elementSVGLine(const XMLAttributes& attributes)
{
    SVGPaintStyle paint_style = parsePaintStyle(attributes);
    glm::mat3x3 transform = parseTransform(attributes);

    const String x1String(
        attributes.getValueAsString(SVGLineAttributeX1, "0"));
    float x1 = parseLengthDataType(x1String).d_value;

    const String y1String(
        attributes.getValueAsString(SVGLineAttributeY1, "0"));
    float y1 = parseLengthDataType(y1String).d_value;

    const String x2String(
        attributes.getValueAsString(SVGLineAttributeX2, "0"));
    float x2 = parseLengthDataType(x2String).d_value;

    const String y2String(
        attributes.getValueAsString(SVGLineAttributeY2, "0"));
    float y2 = parseLengthDataType(y2String).d_value;

    SVGLine* line = new SVGLine(paint_style, transform,
                                         x1, y1, x2, y2);
    addShape(line);
}

//----------------------------------------------------------------------------//
void SVGData::elementSVGPolyline(const XMLAttributes& attributes)
{
    SVGPaintStyle paint_style = parsePaintStyle(attributes);
    glm::mat3x3 transform = parseTransform(attributes);

    const String pointsString(
        attributes.getValueAsString(SVGPolylineAttributePoints, ""));

    std::vector<glm::vec2> points;
    parsePointsString(pointsString, points);

    SVGPolyline* polyline = new SVGPolyline(paint_style, transform, points);
    addShape(polyline);
}

//----------------------------------------------------------------------------//
void SVGData::elementSVGPolygon(const XMLAttributes& attributes)
{
    SVGPaintStyle paint_style = parsePaintStyle(attributes);
    glm::mat3x3 transform = parseTransform(attributes);

    const String pointsString(
        attributes.getValueAsString(SVGPolylineAttributePoints, ""));

    std::vector<glm::vec2> points;
    parsePointsString(pointsString, points);

    SVGPolygon* polygon = new SVGPolygon(paint_style, transform, points);
    addShape(polygon);
}

//----------------------------------------------------------------------------//
void SVGData::elementEndLocal(const String& /*element*/)
{
}

//----------------------------------------------------------------------------//
SVGData::SVGLength SVGData::parseLengthDataType(const String& length_string)
{
    SVGLength length;
    String unitString;

    std::stringstream& strStream = SharedStringstream::GetPreparedStream(length_string);

    strStream >> length.d_value;
    if(strStream.fail())
        throw SVGParsingException(
            "SVG file parsing was aborted because of an invalid value of an SVG"
            " 'length' type (float value): " + length_string);

    strStream >> unitString;

    if(unitString.empty())
        return length;
    else if(unitString.length() == 2)
    {
        if(unitString.compare("in") == 0)
            length.d_unit = SvgLengthUnit::In;
        else if(unitString.compare("cm") == 0)
            length.d_unit = SvgLengthUnit::Cm;
        else if(unitString.compare("mm") == 0)
            length.d_unit = SvgLengthUnit::Mm;
        else if(unitString.compare("pt") == 0)
            length.d_unit = SvgLengthUnit::Pt;
        else if(unitString.compare("pc") == 0)
            length.d_unit = SvgLengthUnit::Pc;
        else if(unitString.compare("px") == 0)
            length.d_unit = SvgLengthUnit::Px;
    }
    else if(unitString.length() == 1)
    {
        if(unitString.compare("%") == 0)
            length.d_unit = SvgLengthUnit::Percent;
    }
    else
    // Parse error
        throw SVGParsingException(
            "SVG file parsing was aborted because of an invalid value of an SVG 'length' type");

    return length;
}

//----------------------------------------------------------------------------//
SVGPaintStyle SVGData::parsePaintStyle(const XMLAttributes& attributes)
{
    SVGPaintStyle paint_style;

    //TODO: unsupported/unspecified values should be inherited from the parents if possible, this
    // however would require adding an additional bool to every attribute member variable to check if
    // it is to be inherited or not
    const String fillString(
        attributes.getValueAsString(SVGGraphicsElementAttributeFill));
    parsePaintStyleFill(fillString, paint_style);

    const String fillRuleString(
        attributes.getValueAsString(SVGGraphicsElementAttributeFillRule));
    parsePaintStyleFillRule(fillRuleString, paint_style);

    const String fillOpacityString(
        attributes.getValueAsString(SVGGraphicsElementAttributeFillOpacity));
    parsePaintStyleFillOpacity(fillOpacityString, paint_style);

    const String strokeString(
        attributes.getValueAsString(SVGGraphicsElementAttributeStroke));
    parsePaintStyleStroke(strokeString, paint_style);

    const String strokeWidthString(
        attributes.getValueAsString(SVGGraphicsElementAttributeStrokeWidth, "1"));
    parsePaintStyleStrokeWidth(strokeWidthString, paint_style);

    const String strokeLinecapString(
        attributes.getValueAsString(SVGGraphicsElementAttributeStrokeLinecap, "butt"));
    parsePaintStyleStrokeLinecap(strokeLinecapString, paint_style);

    const String strokeLinejoinString(
        attributes.getValueAsString(SVGGraphicsElementAttributeStrokeLinejoin, "miter"));
    parsePaintStyleStrokeLinejoin(strokeLinejoinString, paint_style);

    const String strokeMiterLimitString(
        attributes.getValueAsString(SVGGraphicsElementAttributeStrokeMiterLimit, "4"));
    parsePaintStyleMiterlimitString(strokeMiterLimitString, paint_style);

    const String strokeDashArrayString(
        attributes.getValueAsString(SVGGraphicsElementAttributeStrokeDashArray));
    parsePaintStyleStrokeDashArray(strokeDashArrayString, paint_style);

    const String strokeDashOffsetString(
        attributes.getValueAsString(SVGGraphicsElementAttributeStrokeDashOffset));
    parsePaintStyleStrokeDashOffset(strokeDashOffsetString, paint_style);

    const String strokeOpacityString(
        attributes.getValueAsString(SVGGraphicsElementAttributeStrokeOpacity));
    parsePaintStyleStrokeOpacity(strokeOpacityString, paint_style);

    return paint_style;
}

//----------------------------------------------------------------------------//
glm::vec3 SVGData::parseColour(const CEGUI::String& colour_string)
{
    if(colour_string.at(0) == '#')
    {
        if(colour_string.size() == 7)
        {
            const CEGUI::String modifiedColourStr = colour_string.substr(1,7);

            return parseRgbHexColour(modifiedColourStr, colour_string);
        }
        else if(colour_string.size() == 4)
        {
            CEGUI::String modifiedColourStr = CEGUI::String("") +
                colour_string[1] + colour_string[1] +
                colour_string[2] + colour_string[2] +
                colour_string[3] + colour_string[3];

            return parseRgbHexColour(modifiedColourStr, colour_string);
        }
    }
    else if(colour_string.compare("rgb(") > 0)
    {
        CEGUI::String rgbColours = colour_string.substr(4, colour_string.length() - 5);
        std::stringstream& strStream = SharedStringstream::GetPreparedStream(rgbColours);

        int r, g, b;
        strStream >> r >> mandatoryChar<','>  >> g >> mandatoryChar<','>  >> b;
        if (strStream.fail())
            throw SVGParsingException("SVG file parsing was aborted because of an invalid "
                                      "rgb-colour value: " + colour_string);

        return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
    }
    // SVG's default colours
    else if(colour_string.compare("black") == 0)
        return glm::vec3(0.0f, 0.0f, 0.0f);
    else if(colour_string.compare("green") == 0)
        return glm::vec3(0.0f, 128.0f / 255.0f, 0.0f);
    else if(colour_string.compare("silver") == 0)
        return glm::vec3(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
    else if(colour_string.compare("lime") == 0)
        return glm::vec3(0.0f, 1.0f, 0.0f);
    else if(colour_string.compare("gray") == 0)
        return glm::vec3(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
    else if(colour_string.compare("olive") == 0)
        return glm::vec3(128.0f / 255.0f, 128.0f / 255.0f, 0.f);
    else if(colour_string.compare("white") == 0)
        return glm::vec3(1.0f, 1.0f, 1.0f);
    else if(colour_string.compare("yellow") == 0)
        return glm::vec3(1.0f, 1.0f, 0.0f);
    else if(colour_string.compare("maroon") == 0)
        return glm::vec3(128.0f / 255.0f, 0.0f, 0.0f);
    else if(colour_string.compare("navy") == 0)
        return glm::vec3(0.0f, 0.0f, 128.0f / 255.0f);
    else if(colour_string.compare("red") == 0)
        return glm::vec3(1.0f, 0.0f, 0.0f);
    else if(colour_string.compare("blue") == 0)
        return glm::vec3(0.0f, 0.0f, 1.0f);
    else if(colour_string.compare("purple") == 0)
        return glm::vec3(128.0f / 255.0f, 0.0f, 128.0f / 255.0f);
    else if(colour_string.compare("teal") == 0)
        return glm::vec3(0.0f, 128.0f / 255.0f, 128.0f / 255.0f);
    else if(colour_string.compare("fuchsia") == 0)
        return glm::vec3(1.0f, 0.0f, 1.0f);
    else if(colour_string.compare("aqua") == 0)
        return glm::vec3(0.0f, 1.0f, 1.0f);

    // No matching format was found, let's throw an error.
    throw SVGParsingException("SVG file parsing was aborted because of an invalid colour value");
    return glm::vec3();
}

glm::vec3 SVGData::parseRgbHexColour(const CEGUI::String& colourString,
                                     const CEGUI::String& origString)
{
    glm::vec3 colour;
    int value;

    std::stringstream& strStream = SharedStringstream::GetPreparedStream();
    strStream << std::hex;

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    std::string currentSubStr = colourString.substr(0, 2);
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    std::string currentSubStr = colourString.getString().substr(0, 2);
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    std::string currentSubStr = String::convertUtf32ToUtf8(colourString.substr(0, 2).getString());
#endif
    strStream.clear();
    strStream.str(currentSubStr);

    strStream >> value;
    if (strStream.fail())
        throw SVGParsingException("SVG file parsing was aborted because of an invalid "
            "colour value: " + origString);

    colour.x = value / 255.0f;

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    currentSubStr = colourString.substr(2, 2);
#elif(CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    currentSubStr = colourString.getString().substr(2, 2);
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    currentSubStr = String::convertUtf32ToUtf8(colourString.substr(2, 2).getString());
#endif
    strStream.clear();
    strStream.str(currentSubStr);

    strStream >> value;
    if (strStream.fail())
        throw SVGParsingException("SVG file parsing was aborted because of an invalid "
            "colour value: " + origString);

    colour.y = value / 255.0f;

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    currentSubStr = colourString.substr(4, 2);
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    currentSubStr = colourString.getString().substr(4, 2);
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    currentSubStr = String::convertUtf32ToUtf8(colourString.substr(4, 2).getString());
#endif
    strStream.clear();
    strStream.str(currentSubStr);

    strStream >> value;
    if (strStream.fail())
        throw SVGParsingException("SVG file parsing was aborted because of an invalid "
            "colour value: " + origString);

    colour.z = value / 255.0f;

    strStream << std::dec;

    return colour;
}

//----------------------------------------------------------------------------//
std::vector<float> SVGData::parseListOfLengths(const String& list_of_lengths_string)
{
    std::vector<float> list_of_lengths;
    std::stringstream& sstream = SharedStringstream::GetPreparedStream(list_of_lengths_string);

    float currentValue;
    sstream >> currentValue;
    while (!sstream.fail())
    {
        list_of_lengths.push_back(currentValue);
        sstream >> mandatoryChar<','> >> currentValue;
    }

    return list_of_lengths;
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleFill(const String& fillString, SVGPaintStyle& paint_style)
{
    if (fillString.compare("none") == 0)
        paint_style.d_fill.d_none = true;
    else if(fillString.empty())
    {
        // Inherit value or use default
        paint_style.d_fill.d_none = false;
        paint_style.d_fill.d_colour = parseColour("black");
    }
    else
    {
        paint_style.d_fill.d_none = false;
        paint_style.d_fill.d_colour = parseColour(fillString);
    }
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleFillRule(const String& fillRuleString, SVGPaintStyle& paint_style)
{
    if(fillRuleString.empty())
        // Inherit value or use default
        paint_style.d_fillRule = PolygonFillRule::NonZero;
    else if(fillRuleString.compare("nonzero"))
        paint_style.d_fillRule = PolygonFillRule::NonZero;
    else if(fillRuleString.compare("evenodd"))
        paint_style.d_fillRule = PolygonFillRule::EvenOdd;
}


//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleFillOpacity(const String& fillOpacityString, SVGPaintStyle& paint_style)
{
    if(fillOpacityString.empty())
        // Inherit value or use default
        paint_style.d_fillOpacity = 1.0f;
    else
    {
        std::stringstream& sstream = SharedStringstream::GetPreparedStream(fillOpacityString);
        sstream >> paint_style.d_fillOpacity;
        if(sstream.fail())
            throw SVGParsingException("SVG file parsing was aborted because of an invalid "
                                      "fill opacity value: " + fillOpacityString);

        //! Clamp value in each case without throwing a warning if the values are below 0 or above 1
        paint_style.d_fillOpacity = std::min( std::max(0.0f, paint_style.d_fillOpacity), 1.0f );
    }
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStroke(const String& strokeString, SVGPaintStyle& paint_style)
{
    if (strokeString.compare("none") == 0 || strokeString.empty())
        paint_style.d_stroke.d_none = true;
    else
    {
        paint_style.d_stroke.d_none = false;
        paint_style.d_stroke.d_colour = parseColour(strokeString);
    }
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStrokeWidth(const String& strokeWidthString, SVGPaintStyle& paint_style)
{
    std::stringstream& sstream = SharedStringstream::GetPreparedStream(strokeWidthString);
    sstream >> paint_style.d_strokeWidth;
    if(sstream.fail())
        throw SVGParsingException(
            "SVG file parsing was aborted because of an invalid value for the SVG 'stroke-width' "
            "type : " + strokeWidthString);

    if(paint_style.d_strokeWidth < 0.0f)
    {
        paint_style.d_strokeWidth = 1.0f;
        Logger::getSingleton().logEvent("SVGData::parsePaintStyle - An unsupported value for "
                                        "stroke-width was specified in the SVG file. The value was "
                                        "set to the initial value '1'. String: " +
                                        strokeWidthString, LoggingLevel::Error);
    }
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStrokeLinecap(const String& strokeLinecapString, SVGPaintStyle& paint_style)
{
    if(strokeLinecapString.compare("butt") == 0)
        paint_style.d_strokeLinecap = SVGPaintStyle::SvgLinecap::Butt;
    else if(strokeLinecapString.compare("round") == 0)
        paint_style.d_strokeLinecap = SVGPaintStyle::SvgLinecap::Round;
    else if(strokeLinecapString.compare("square") == 0)
        paint_style.d_strokeLinecap = SVGPaintStyle::SvgLinecap::Square;
    else
        throw SVGParsingException(
        "SVG file parsing was aborted because of an invalid value for the SVG 'linecap' type");
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStrokeLinejoin(const String& strokeLinejoinString, SVGPaintStyle& paint_style)
{
    if(strokeLinejoinString.compare("miter") == 0)
        paint_style.d_strokeLinejoin = SVGPaintStyle::SVGLinejoin::Miter;
    else if(strokeLinejoinString.compare("round") == 0)
        paint_style.d_strokeLinejoin = SVGPaintStyle::SVGLinejoin::Round;
    else if(strokeLinejoinString.compare("bevel") == 0)
        paint_style.d_strokeLinejoin = SVGPaintStyle::SVGLinejoin::Bevel;
    else
        throw SVGParsingException(
        "SVG file parsing was aborted because of an invalid value for the SVG 'linejoin' type");
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleMiterlimitString(const String& strokeMiterLimitString, SVGPaintStyle& paint_style)
{
    std::stringstream& sstream = SharedStringstream::GetPreparedStream(strokeMiterLimitString);
    sstream >> paint_style.d_strokeMiterlimit;
    if (sstream.fail())
        throw SVGParsingException(
            "SVG file parsing was aborted because of an invalid value for the SVG "
            "'stroke-miterlimit' type : " + strokeMiterLimitString);

    if(paint_style.d_strokeMiterlimit < 1.0f)
    {
        paint_style.d_strokeMiterlimit = 4.0f;
        Logger::getSingleton().logEvent("SVGData::parsePaintStyle - An unsupported value for "
                                        "stroke-miterlimit was specified in the SVG file. The "
                                        " value was set to the initial value '4'. String: " +
                                        strokeMiterLimitString, LoggingLevel::Error);
    }
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStrokeDashArray(const String& strokeDashArrayString, SVGPaintStyle& paint_style)
{
    if(strokeDashArrayString.compare("none") == 0)
    {
        paint_style.d_strokeDashArrayNone = true;
        paint_style.d_strokeDashArray.clear();
    }
    else
    {
        paint_style.d_strokeDashArray = parseListOfLengths(strokeDashArrayString);

        const size_t dashArraySize = paint_style.d_strokeDashArray.size();
        paint_style.d_strokeDashArrayNone = dashArraySize != 0;
        //! If an odd number of values is provided, then the list of values shall be repeated to yield an even number of values
        if(paint_style.d_strokeDashArrayNone == false && (dashArraySize % 2) == 1)
            paint_style.d_strokeDashArray.insert( paint_style.d_strokeDashArray.end(), paint_style.d_strokeDashArray.begin(), paint_style.d_strokeDashArray.end() );
    }
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStrokeOpacity(const String& strokeOpacityString, SVGPaintStyle& paint_style)
{
    if(strokeOpacityString.empty())
        paint_style.d_strokeOpacity = 1.0f;
    else
    {
        std::stringstream& sstream = SharedStringstream::GetPreparedStream(strokeOpacityString);
        sstream >> paint_style.d_strokeOpacity;
        if (sstream.fail())
            throw SVGParsingException(
                "SVG file parsing was aborted because of an invalid value for the SVG "
                "'stroke-opacity' type : " + strokeOpacityString);

        //! Clamp value without ever throwing a warning
        paint_style.d_strokeOpacity = std::min( std::max(0.0f, paint_style.d_strokeOpacity), 1.0f );
    }
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStrokeDashOffset(const String& strokeDashOffsetString, SVGPaintStyle& paint_style)
{
    if(strokeDashOffsetString.empty())
        paint_style.d_strokeDashOffset = 0.0f;
    else
    {
        std::stringstream& sstream = SharedStringstream::GetPreparedStream(strokeDashOffsetString);
        sstream >> paint_style.d_strokeDashOffset;
        if (sstream.fail())
            throw SVGParsingException(
                "SVG file parsing was aborted because of an invalid value for the SVG "
                "'stroke-opacity' type : " + strokeDashOffsetString);
    }
}

//----------------------------------------------------------------------------//
glm::mat3x3 SVGData::parseTransform(const XMLAttributes& attributes)
{
    const String transformString(attributes.getValueAsString(SVGTransformAttribute));

    std::stringstream& sstream = SharedStringstream::GetPreparedStream(transformString);
    // Unity matrix is our default/basis
    glm::mat3x3 currentMatrix(1.0f);

    sstream >> MandatoryString("matrix") >> mandatoryChar<'('>;
    if(!sstream.fail())
    {
        float matrixValues[6];

        int i = 0;
        while( (i < 6) && !sstream.fail() )
        {
            // We allow either comma or spaces as separators
            sstream >> optionalChar<','>;
            sstream >> matrixValues[i];
            ++i;
        }
        
        //If we parsed the expected amount of matrix elements we will multiply the matrix to our transformation
        if(i == 6)
        {
            currentMatrix *= glm::mat3x3(matrixValues[0], matrixValues[2], matrixValues[4],
                                         matrixValues[1], matrixValues[3], matrixValues[5],
                                         0.0f, 0.0f, 1.0f);
        }
        else
        {
            throw SVGParsingException(
                "SVG file parsing was aborted because of an invalid value for the SVG "
                "'transform' type : " + transformString);
        }

    }

    return currentMatrix;
}

//----------------------------------------------------------------------------//
void SVGData::parsePointsString(const String& pointsString, std::vector<glm::vec2>& points)
{

    std::stringstream& sstream = SharedStringstream::GetPreparedStream(pointsString);

    while(true)
    {
        glm::vec2 currentPoint;

        sstream >> currentPoint.x;
        //Check if a new pair can be read, if not then break the loop, the input so far is assumed
        //to be valid
        if(sstream.fail())
        {
            break;
        }

        sstream >> mandatoryChar<','>;
        if (sstream.fail())
        {
            throw SVGParsingException("SVG file parsing was aborted because of an invalid value "
                                      "for the SVG 'points' type (missing comma separator): " +
                                      pointsString);
            //Clearing the invalid list of points
            points.clear();
        }
        sstream >> currentPoint.y;
        if (sstream.fail())
        {
            throw SVGParsingException("SVG file parsing was aborted because of an invalid value "
                                      "for the SVG 'points' type (missing second value of the pair"
                                      "): " + pointsString);
            //Clearing the invalid list of points
            points.clear();
        }

        points.push_back(currentPoint);
    }
}

//----------------------------------------------------------------------------//
}

