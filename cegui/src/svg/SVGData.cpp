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
SVGData::SVGData(const String& name) :
    d_name(name)
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
    const unsigned int shape_count = d_svgBasicShapes.size();
    for (unsigned int i = 0; i < shape_count; ++i)
        delete d_svgBasicShapes[i];

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
void SVGData::elementEndLocal(const String& element)
{
}

//----------------------------------------------------------------------------//
SVGData::SVGLength SVGData::parseLengthDataType(const String& length_string)
{
    SVGLength length;
    char lengthEnding[3] = "";
    String unitString;

    sscanf(length_string.c_str(), "%f%2s", &length.d_value, lengthEnding);
    unitString = lengthEnding;

    if(unitString.empty())
        return length;
    else if(unitString.length() == 2)
    {
        if(unitString.compare("in") == 0)
            length.d_unit = SLU_IN;
        else if(unitString.compare("cm") == 0)
            length.d_unit = SLU_CM;
        else if(unitString.compare("mm") == 0)
            length.d_unit = SLU_MM;
        else if(unitString.compare("pt") == 0)
            length.d_unit = SLU_PT;
        else if(unitString.compare("pc") == 0)
            length.d_unit = SLU_PC;
        else if(unitString.compare("px") == 0)
            length.d_unit = SLU_PX;
    }
    else if(unitString.length() == 1)
    {
        if(unitString.compare("%") == 0)
            length.d_unit = SLU_PERCENT;
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
        glm::vec3 colour;

        if(colour_string.size() == 7)
        {
            int value;
            sscanf(colour_string.c_str() + 1, " %2X ", &value);
            colour.x = value / 255.0f;
            sscanf(colour_string.c_str() + 3, " %2X ", &value);
            colour.y = value / 255.0f;
            sscanf(colour_string.c_str() + 5, " %2X ", &value);
            colour.z = value / 255.0f;

            return colour;
        }
        else if(colour_string.size() == 4)
        {
            CEGUI::String mod_colour_string = CEGUI::String("") +
                colour_string[1] + colour_string[1] +
                colour_string[2] + colour_string[2] +
                colour_string[3] + colour_string[3];

            int value;
            sscanf(mod_colour_string.c_str(), " %2X ", &value);
            colour.x = value / 255.0f;
            sscanf(mod_colour_string.c_str() + 2, " %2X ", &value);
            colour.y = value / 255.0f;
            sscanf(mod_colour_string.c_str() + 4, " %2X ", &value);
            colour.z = value / 255.0f;

            return colour;
        }
    }
    else if(colour_string.compare("rgb(") > 0)
    {
        CEGUI::String mod_colour_string = colour_string.substr(4, colour_string.length() - 5);

        int r, g, b;
        sscanf(mod_colour_string.c_str(), " %i , %i , %i ", &r, &g, &b);

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

    // Parse error
    throw SVGParsingException(
        "SVG file parsing was aborted because of an invalid colour value");

    return glm::vec3();
}

//----------------------------------------------------------------------------//
std::vector<float> SVGData::parseListOfLengths(const String& list_of_lengths_string)
{
    std::vector<float> list_of_lengths;
    const char* lengths_array_pointer = list_of_lengths_string.c_str();
    float value;
    int offset;

    while (1 == sscanf(lengths_array_pointer, " %f ,%n", &value, &offset))
    {
        list_of_lengths.push_back(value);
        lengths_array_pointer += offset;
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
        paint_style.d_fillRule = PFR_NON_ZERO;
    else if(fillRuleString.compare("nonzero"))
        paint_style.d_fillRule = PFR_NON_ZERO;
    else if(fillRuleString.compare("evenodd"))
        paint_style.d_fillRule = PFR_EVEN_ODD;
}


//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleFillOpacity(const String& fillOpacityString, SVGPaintStyle& paint_style)
{
    if(fillOpacityString.empty())
        // Inherit value or use default
        paint_style.d_fillOpacity = 1.0f;
    else
    {
        sscanf(fillOpacityString.c_str(), "%f", &paint_style.d_fillOpacity);
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
    sscanf(strokeWidthString.c_str(), "%f", &paint_style.d_strokeWidth);
    if(paint_style.d_strokeWidth < 0.0f)
    {
        paint_style.d_strokeWidth = 1.0f;
        Logger::getSingleton().logEvent("SVGData::parsePaintStyle - An unsupported value for stroke-width was specified in the SVG file. The value is set to the initial value '1'.", Errors);
    }
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStrokeLinecap(const String& strokeLinecapString, SVGPaintStyle& paint_style)
{
    if(strokeLinecapString.compare("butt") == 0)
        paint_style.d_strokeLinecap = SVGPaintStyle::SLC_BUTT;
    else if(strokeLinecapString.compare("round") == 0)
        paint_style.d_strokeLinecap = SVGPaintStyle::SLC_ROUND;
    else if(strokeLinecapString.compare("square") == 0)
        paint_style.d_strokeLinecap = SVGPaintStyle::SLC_SQUARE;
    else
        throw SVGParsingException(
        "SVG file parsing was aborted because of an invalid value for the SVG 'linecap' type");
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleStrokeLinejoin(const String& strokeLinejoinString, SVGPaintStyle& paint_style)
{
    if(strokeLinejoinString.compare("miter") == 0)
        paint_style.d_strokeLinejoin = SVGPaintStyle::SLJ_MITER;
    else if(strokeLinejoinString.compare("round") == 0)
        paint_style.d_strokeLinejoin = SVGPaintStyle::SLJ_ROUND;
    else if(strokeLinejoinString.compare("bevel") == 0)
        paint_style.d_strokeLinejoin = SVGPaintStyle::SLJ_BEVEL;
    else
        throw SVGParsingException(
        "SVG file parsing was aborted because of an invalid value for the SVG 'linejoin' type");
}

//----------------------------------------------------------------------------//
void SVGData::parsePaintStyleMiterlimitString(const String& strokeMiterLimitString, SVGPaintStyle& paint_style)
{
    sscanf(strokeMiterLimitString.c_str(), "%f", &paint_style.d_strokeMiterlimit);
    if(paint_style.d_strokeMiterlimit < 1.0f)
    {
        paint_style.d_strokeMiterlimit = 4.0f;
        Logger::getSingleton().logEvent("SVGData::parsePaintStyle - An unsupported value for stroke-miterlimit was specified in the SVG file. The value is set to the initial value '4'.", Errors);
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
        sscanf(strokeOpacityString.c_str(), "%f", &paint_style.d_strokeOpacity);
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
        sscanf(strokeDashOffsetString.c_str(), "%f", &paint_style.d_strokeDashOffset);
}

//----------------------------------------------------------------------------//
glm::mat3x3 SVGData::parseTransform(const XMLAttributes& attributes)
{
    const String transformString(
        attributes.getValueAsString(SVGTransformAttribute));

    const char* transformStringSegment = transformString.c_str();
    int offset = 0;
    // Unity matrix is our default/basis
    glm::mat3x3 currentMatrix(1.0f);

    if(sscanf(transformStringSegment, " matrix( %n", &offset) == 0 && offset != 0)
    {
        transformStringSegment += offset;
        float matrixValues[6];

        int i = 0;
        while( (i < 6) && ( sscanf(transformStringSegment, " %f %n", &matrixValues[i],  &offset) == 1 || 
                            sscanf(transformStringSegment, " , %f %n", &matrixValues[i],  &offset) == 1 ) )
        {
            transformStringSegment += offset;
            ++i;
        }
        
        //If we parsed the expected amount of matrix elements we will multiply the matrix to our transformation
        if(i == 6)
            currentMatrix *= glm::mat3x3(matrixValues[0], matrixValues[2], matrixValues[4],
                                         matrixValues[1], matrixValues[3], matrixValues[5],
                                         0.0f, 0.0f, 1.0f);
    }

    return currentMatrix;
}

//----------------------------------------------------------------------------//
void SVGData::parsePointsString(const String& pointsString, std::vector<glm::vec2>& points)
{
    const char* currentStringSegment = pointsString.c_str();
    int offset;
    glm::vec2 currentPoint;

    while(true)
    {
        int successful_args = sscanf(currentStringSegment, " %f , %f%n", &currentPoint.x, &currentPoint.y, &offset);

        if(successful_args == 2)
        {
            points.push_back(currentPoint);
            currentStringSegment += offset;
        }
        else if(successful_args == 1)
        {
            points.clear();
            currentStringSegment += offset;
        }
        else
            break;
    }
}

//----------------------------------------------------------------------------//
}

