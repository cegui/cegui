/***********************************************************************
    filename:   CEGUIXMLEnumHelper.h
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
#ifndef _CEGUIFalXMLEnumHelper_h_
#define _CEGUIFalXMLEnumHelper_h_

#include "../String.h"
#include "../Window.h"
#include "CEGUI/falagard/Enums.h"

namespace CEGUI
{
template<typename T>
class FalagardXMLHelper;

template<>
class FalagardXMLHelper<VerticalFormatting>
{
public:
    typedef VerticalFormatting return_type;
    typedef VerticalFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case VF_BOTTOM_ALIGNED:
            return String("BottomAligned");

        case VF_CENTRE_ALIGNED:
            return String("CentreAligned");

        case VF_TILED:
            return String("Tiled");

        case VF_STRETCHED:
            return String("Stretched");

        default:
            return String("TopAligned");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "CentreAligned")
            return VF_CENTRE_ALIGNED;

        else if (str == "BottomAligned")
            return VF_BOTTOM_ALIGNED;

        else if (str == "Tiled")
            return VF_TILED;

        else if (str == "Stretched")
            return VF_STRETCHED;

        else
            return VF_TOP_ALIGNED;
    }
};

template<>
class FalagardXMLHelper<HorizontalFormatting>
{
public:
    typedef HorizontalFormatting return_type;
    typedef HorizontalFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case HF_RIGHT_ALIGNED:
            return String("RightAligned");

        case HF_CENTRE_ALIGNED:
            return String("CentreAligned");

        case HF_TILED:
            return String("Tiled");

        case HF_STRETCHED:
            return String("Stretched");

        default:
            return String("LeftAligned");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "CentreAligned")
            return HF_CENTRE_ALIGNED;

        else if (str == "RightAligned")
            return HF_RIGHT_ALIGNED;

        else if (str == "Tiled")
            return HF_TILED;

        else if (str == "Stretched")
            return HF_STRETCHED;

        else
            return HF_LEFT_ALIGNED;
    }
};

template<>
class FalagardXMLHelper<VerticalAlignment>
{
public:
    typedef VerticalAlignment return_type;
    typedef VerticalAlignment pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case VA_BOTTOM:
            return String("BottomAligned");

        case VA_CENTRE:
            return String("CentreAligned");

        default:
            return String("TopAligned");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "CentreAligned")
            return VA_CENTRE;

        else if (str == "BottomAligned")
            return VA_BOTTOM;

        else
            return VA_TOP;
    }
};

template<>
class FalagardXMLHelper<HorizontalAlignment>
{
public:
    typedef HorizontalAlignment return_type;
    typedef HorizontalAlignment pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case HA_RIGHT:
            return String("RightAligned");

        case HA_CENTRE:
            return String("CentreAligned");

        default:
            return String("LeftAligned");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "CentreAligned")
            return HA_CENTRE;

        else if (str == "RightAligned")
            return HA_RIGHT;

        else
            return HA_LEFT;
    }
};

template<>
class FalagardXMLHelper<DimensionType>
{
public:
    typedef DimensionType return_type;
    typedef DimensionType pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case DT_LEFT_EDGE:
            return String("LeftEdge");

        case DT_X_POSITION:
            return String("XPosition");

        case DT_TOP_EDGE:
            return String("TopEdge");

        case DT_Y_POSITION:
            return String("YPosition");

        case DT_RIGHT_EDGE:
            return String("RightEdge");

        case DT_BOTTOM_EDGE:
            return String("BottomEdge");

        case DT_WIDTH:
            return String("Width");

        case DT_HEIGHT:
            return String("Height");

        case DT_X_OFFSET:
            return String("XOffset");

        case DT_Y_OFFSET:
            return String("YOffset");

        default:
            return String("Invalid");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "LeftEdge")
            return DT_LEFT_EDGE;

        else if (str == "XPosition")
            return DT_X_POSITION;

        else if (str == "TopEdge")
            return DT_TOP_EDGE;

        else if (str == "YPosition")
            return DT_Y_POSITION;

        else if (str == "RightEdge")
            return DT_RIGHT_EDGE;

        else if (str == "BottomEdge")
            return DT_BOTTOM_EDGE;

        else if (str == "Width")
            return DT_WIDTH;

        else if (str == "Height")
            return DT_HEIGHT;

        else if (str == "XOffset")
            return DT_X_OFFSET;

        else if (str == "YOffset")
            return DT_Y_OFFSET;

        else
            return DT_INVALID;
    }
};

template<>
class FalagardXMLHelper<VerticalTextFormatting>
{
public:
    typedef VerticalTextFormatting return_type;
    typedef VerticalTextFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case VTF_BOTTOM_ALIGNED:
            return String("BottomAligned");

        case VTF_CENTRE_ALIGNED:
            return String("CentreAligned");

        default:
            return String("TopAligned");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "CentreAligned")
            return VTF_CENTRE_ALIGNED;

        else if (str == "BottomAligned")
            return VTF_BOTTOM_ALIGNED;

        else
            return VTF_TOP_ALIGNED;
    }
};

template<>
class FalagardXMLHelper<HorizontalTextFormatting>
{
public:
    typedef HorizontalTextFormatting return_type;
    typedef HorizontalTextFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case HTF_RIGHT_ALIGNED:
            return String("RightAligned");

        case HTF_CENTRE_ALIGNED:
            return String("CentreAligned");

        case HTF_JUSTIFIED:
            return String("Justified");

        case HTF_WORDWRAP_LEFT_ALIGNED:
            return String("WordWrapLeftAligned");

        case HTF_WORDWRAP_RIGHT_ALIGNED:
            return String("WordWrapRightAligned");

        case HTF_WORDWRAP_CENTRE_ALIGNED:
            return String("WordWrapCentreAligned");

        case HTF_WORDWRAP_JUSTIFIED:
            return String("WordWrapJustified");

        default:
            return String("LeftAligned");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "CentreAligned")
            return HTF_CENTRE_ALIGNED;

        else if (str == "RightAligned")
            return HTF_RIGHT_ALIGNED;

        else if (str == "Justified")
            return HTF_JUSTIFIED;

        else if (str == "WordWrapLeftAligned")
            return HTF_WORDWRAP_LEFT_ALIGNED;

        else if (str == "WordWrapCentreAligned")
            return HTF_WORDWRAP_CENTRE_ALIGNED;

        else if (str == "WordWrapRightAligned")
            return HTF_WORDWRAP_RIGHT_ALIGNED;

        else if (str == "WordWrapJustified")
            return HTF_WORDWRAP_JUSTIFIED;

        else
            return HTF_LEFT_ALIGNED;
    }
};

template<>
class FalagardXMLHelper<FontMetricType>
{
public:
    typedef FontMetricType return_type;
    typedef FontMetricType pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case FMT_BASELINE:
            return String("Baseline");

        case FMT_HORZ_EXTENT:
            return String("HorzExtent");

        default:
            return String("LineSpacing");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "LineSpacing")
            return FMT_LINE_SPACING;

        else if (str == "Baseline")
            return FMT_BASELINE;

        else
            return FMT_HORZ_EXTENT;
    }
};

template<>
class FalagardXMLHelper<DimensionOperator>
{
public:
    typedef DimensionOperator return_type;
    typedef DimensionOperator pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case DOP_ADD:
            return String("Add");

        case DOP_SUBTRACT:
            return String("Subtract");

        case DOP_MULTIPLY:
            return String("Multiply");

        case DOP_DIVIDE:
            return String("Divide");

        default:
            return String("Noop");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "Add")
            return DOP_ADD;

        else if (str == "Subtract")
            return DOP_SUBTRACT;

        else if (str == "Multiply")
            return DOP_MULTIPLY;

        else if (str == "Divide")
            return DOP_DIVIDE;

        else
            return DOP_NOOP;
    }
};

template<>
class FalagardXMLHelper<FrameImageComponent>
{
public:
    typedef FrameImageComponent return_type;
    typedef FrameImageComponent pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case FIC_TOP_LEFT_CORNER:
            return String("TopLeftCorner");

        case FIC_TOP_RIGHT_CORNER:
            return String("TopRightCorner");

        case FIC_BOTTOM_LEFT_CORNER:
            return String("BottomLeftCorner");

        case FIC_BOTTOM_RIGHT_CORNER:
            return String("BottomRightCorner");

        case FIC_LEFT_EDGE:
            return String("LeftEdge");

        case FIC_RIGHT_EDGE:
            return String("RightEdge");

        case FIC_TOP_EDGE:
            return String("TopEdge");

        case FIC_BOTTOM_EDGE:
            return String("BottomEdge");

        default:
            return String("Background");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "TopLeftCorner")
            return FIC_TOP_LEFT_CORNER;

        if (str == "TopRightCorner")
            return FIC_TOP_RIGHT_CORNER;

        if (str == "BottomLeftCorner")
            return FIC_BOTTOM_LEFT_CORNER;

        if (str == "BottomRightCorner")
            return FIC_BOTTOM_RIGHT_CORNER;

        if (str == "LeftEdge")
            return FIC_LEFT_EDGE;

        if (str == "RightEdge")
            return FIC_RIGHT_EDGE;

        if (str == "TopEdge")
            return FIC_TOP_EDGE;

        if (str == "BottomEdge")
            return FIC_BOTTOM_EDGE;

        else
            return FIC_BACKGROUND;
    }
};

template<>
class FalagardXMLHelper<ChildEventAction>
{
public:
    typedef ChildEventAction return_type;
    typedef ChildEventAction pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case CEA_REDRAW:
            return String("Redraw");

        case CEA_LAYOUT:
            return String("Layout");

        default:
            CEGUI_THROW(InvalidRequestException(
                "Invalid enumeration value given."));
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == "Redraw")
            return CEA_REDRAW;

        if (str == "Layout")
            return CEA_LAYOUT;

        CEGUI_THROW(InvalidRequestException(
            "'" + str +
            "' does not represent a ChildEventAction enumerated value."));
    }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// PropertyHelper specialisations
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<>
class PropertyHelper<VerticalFormatting>
{
public:
    typedef VerticalFormatting return_type;
    typedef return_type safe_method_return_type;
    typedef VerticalFormatting pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static String type("VerticalFormatting");

        return type;
    }

    static return_type fromString(const String& str)
    {
        return FalagardXMLHelper<return_type>::fromString(str);
    }

    static string_return_type toString(pass_type val)
    {
        return FalagardXMLHelper<return_type>::toString(val);
    }
};

template<>
class PropertyHelper<HorizontalFormatting>
{
public:
    typedef HorizontalFormatting return_type;
    typedef return_type safe_method_return_type;
    typedef HorizontalFormatting pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static String type("HorizontalFormatting");

        return type;
    }

    static return_type fromString(const String& str)
    {
        return FalagardXMLHelper<return_type>::fromString(str);
    }

    static string_return_type toString(pass_type val)
    {
        return FalagardXMLHelper<return_type>::toString(val);
    }
};

template<>
class PropertyHelper<VerticalTextFormatting>
{
public:
    typedef VerticalTextFormatting return_type;
    typedef return_type safe_method_return_type;
    typedef VerticalTextFormatting pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static String type("VerticalTextFormatting");

        return type;
    }

    static return_type fromString(const String& str)
    {
        return FalagardXMLHelper<return_type>::fromString(str);
    }

    static string_return_type toString(pass_type val)
    {
        return FalagardXMLHelper<return_type>::toString(val);
    }
};

template<>
class PropertyHelper<HorizontalTextFormatting>
{
public:
    typedef HorizontalTextFormatting return_type;
    typedef return_type safe_method_return_type;
    typedef HorizontalTextFormatting pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static String type("HorizontalTextFormatting");

        return type;
    }

    static return_type fromString(const String& str)
    {
        return FalagardXMLHelper<return_type>::fromString(str);
    }

    static string_return_type toString(pass_type val)
    {
        return FalagardXMLHelper<return_type>::toString(val);
    }
};

}


#endif
