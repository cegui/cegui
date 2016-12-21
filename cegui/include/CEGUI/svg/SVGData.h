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
#ifndef _SVGData_h_
#define _SVGData_h_

#include "CEGUI/ChainedXMLHandler.h"
#include "CEGUI/Base.h"
#include "CEGUI/String.h"
#include "CEGUI/svg/SVGPaintStyle.h"

#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


namespace CEGUI
{
class SVGBasicShape;

/*!
\brief
    Defines a class for the SVG data.

    The SVGData class stores data of a vector graphics image based on the SVG Tiny 1.2
    file standard. The SVGData is utilised by the SVGImage class.

    The data for this class can be either be created by parsing it from an SVG file
    or be added manually by a user to draw custom geometry.
*/
class CEGUIEXPORT SVGData :
    public ChainedXMLHandler
{
public:
    //! Enumerator describing the available unit types in the SVG standard for the length type
    enum class SvgLengthUnit : int
    {
        Undefined,
        In,
        Cm,
        Mm,
        Pt,
        Pc,
        Px,
        Percent,

        Count
    };

    struct SVGLength
    {
        SVGLength() :
            d_value(0.f),
            d_unit(SvgLengthUnit::Undefined)
        {}

        float           d_value;
        SvgLengthUnit   d_unit;
    };

    SVGData(const String& name);

    SVGData(const String& name,
            const String& filename,
            const String& resourceGroup);

    ~SVGData();

    
    /*!
    \brief
        Returns the name given to the SVGData when it was created.

    \return
        Reference to a String object that holds the name of the SVGData.
    */
    const String& getName() const;

    /*!
    \brief
        Loads and parses the specified SVG file into this SVGData object.

    \param file_name
        The filename of the SVG file that is to be loaded.

    \param resource_group
        Resource group identifier to be passed to the resource provider when
        loading the image file.

    \return
        Nothing.
    */
    void loadFromFile(const String& file_name,
                      const String& resource_group);

    /*!
    \brief
        Adds a SVGBasicShape to the list of shapes of this class. This class takes ownership
        of the passed object and will free the memory itself.
    \param svg_shape
        The SVGBasicShape that will be added.
    */
    void addShape(SVGBasicShape* svg_shape);

    /*!
    \brief
        Deletes all shapes in the list and clears the list.
    */
    void destroyShapes();

    /*!
    \brief
        Returns the list of shapes of this class.
    \return
        The list of shapes of this class.
    */
    const std::vector<SVGBasicShape*>& getShapes() const;

    /*!
    \brief
        Returns the SVGData's width in pixels.
    \return
        The SVGData's width in pixels.
    */
    float getWidth() const;

    /*!
    \brief
        Sets the SVGData's width in pixels.
    \param width
        The width in pixels.
    */
    void setWidth(float width);

    /*!
    \brief
        Returns the SVGData's height in pixels.
    \return
        The SVGData's height in pixels.
    */
    float getHeight() const;

    /*!
    \brief
        Sets the SVGData's height in pixels.
    \param width
        The height in pixels.
    */
    void setHeight(float height);

protected:
    // implement chained xml handler abstract interface
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes) override;
    void elementEndLocal(const String& element) override;

    /*!
    \brief
        Function that handles the opening SVG element.

    \note
        This function processes the SVG document fragment which contains
        the SVG graphics elements, container elements, etc. ...

    */
    void elementSVGStart(const XMLAttributes& attributes);

    /*!
    \brief
        Function that handles opening SVG 'rect' elements.

    \note
        This function processes the SVG 'rect' element.
    */
    void elementSVGRect(const XMLAttributes& attributes);

    /*!
    \brief
        Function that handles opening SVG 'circle' elements.

    \note
        This function processes the SVG 'circle' element.
    */
    void elementSVGCircle(const XMLAttributes& attributes);

    /*!
    \brief
        Function that handles opening SVG 'ellipse' elements.

    \note
        This function processes the SVG 'ellipse' element.
    */
    void elementSVGEllipse(const XMLAttributes& attributes);
    
    /*!
    \brief
        Function that handles opening SVG 'line' elements.

    \note
        This function processes the SVG 'line' element.
    */
    void elementSVGLine(const XMLAttributes& attributes);

    /*!
    \brief
        Function that handles opening SVG 'polyline' elements.

    \note
        This function processes the SVG 'polyline' element.
    */
    void elementSVGPolyline(const XMLAttributes& attributes);

    /*!
    \brief
        Function that handles opening SVG 'polygon' elements.

    \note
        This function processes the SVG 'polygon' element.
    */
    void elementSVGPolygon(const XMLAttributes& attributes);

    //! Name of this SVGData objects
    CEGUI::String d_name;
    /*!
    \brief
        The SVGData's width in pixels.
        
        This is the value representing the intrinsic width of the 'SVG document fragment'. It is used in CEGUI
        to determine the clipping area of the SVG image and to scale the image elements in case the Image is
        rendered with horizontal stretching.
    */
    float d_width;
    /*!
    \brief
        The SVGData's height in pixels.
        
        This is the value representing the intrinsic height of the 'SVG document fragment'. It is used in CEGUI
        to determine the clipping area of the SVG image and to scale the image elements in case the Image is
        rendered with vertical stretching.
    */
    float d_height;

    //! The basic shapes that were added to the SVGData
    std::vector<SVGBasicShape*> d_svgBasicShapes;

private:
    /*!
    \brief
        Function that parses the paint style (fill and stroke parameters) from an SVG graphics element.
    \param attributes
        The XML attributes from which the values will be parsed.
    */
    static SVGPaintStyle parsePaintStyle(const XMLAttributes& attributes);

    //! Parses the String value of a 'fill' property 
    static void parsePaintStyleFill(const String& fillString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'fill-rule' property 
    static void parsePaintStyleFillRule(const String& fillRuleString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'fill-opacity' property
    static void parsePaintStyleFillOpacity(const String& fillOpacityString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'stroke' property 
    static void parsePaintStyleStroke(const String& strokeString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'stroke-width' property 
    static void parsePaintStyleStrokeWidth(const String& strokeWidthString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'stroke-linecap' property 
    static void parsePaintStyleStrokeLinecap(const String& strokeLinecapString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'stroke-linejoin' property 
    static void parsePaintStyleStrokeLinejoin(const String& strokeLinejoinString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'stroke-miterlimit' property 
    static void parsePaintStyleMiterlimitString(const String& strokeMiterLimitString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'stroke-dasharray' property 
    static void parsePaintStyleStrokeDashArray(const String& strokeDashArrayString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'stroke-dashoffset' property 
    static void parsePaintStyleStrokeDashOffset(const String& strokeDashOffsetString, SVGPaintStyle& paint_style);

    //! Parses the String value of a 'stroke-opacity' property 
    static void parsePaintStyleStrokeOpacity(const String& strokeOpacityString, SVGPaintStyle& paint_style);

    /*!
    \brief
        Function that parses a String into an SVG length object and returns it.

    \param length_string
        The String containing the characters that should be parsed into an SVG length.

    \return
        The SVGLength object.

    \exception SVGParsingException          thrown if there was some problem parsing the String.
    */
    static SVGData::SVGLength parseLengthDataType(const String& length_string);

    //! Parses the String value of a 'points' property 
    static void parsePointsString(const String &pointsString, std::vector<glm::vec2>& points);

    /*!
    \brief
        Function that parses a 'transform' attribute and creates a mat3x3 from it.
    \param attributes
        The XML attributes from which the values will be parsed.
    */
    static glm::mat3x3 parseTransform(const XMLAttributes& attributes);

    /*!
    \brief
        Function that parses the an SVG list of lengths from a String.
    \param list_of_lengths_string
        The String containing the list of lengths.
    */
    static std::vector<float> parseListOfLengths(const String& list_of_lengths_string);

    /*!
    \brief
        Function that parses an SVG paint colour and returns the CEGUI Colour created from it.
    \param colour_string
        The colour String value from which the colour values will be parsed;
    */
    static glm::vec3 parseColour(const CEGUI::String& colour_string);

    static glm::vec3 parseRgbHexColour(const CEGUI::String& colour_mod_string,
                                       const CEGUI::String& colour_string);

};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif

