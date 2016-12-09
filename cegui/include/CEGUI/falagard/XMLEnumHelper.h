/***********************************************************************
    created:    Mon Jul 18 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software, to deal in the Software without restriction, including
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
class CEGUIEXPORT FalagardXMLHelper<VerticalFormatting>
{
public:
    typedef VerticalFormatting return_type;
    typedef VerticalFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case VerticalFormatting::BOTTOM_ALIGNED:
            return BottomAligned;

        case VerticalFormatting::CENTRE_ALIGNED:
            return CentreAligned;

        case VerticalFormatting::TILED:
            return Tiled;

        case VerticalFormatting::STRETCHED:
            return Stretched;

        default:
            return TopAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return VerticalFormatting::CENTRE_ALIGNED;

        else if (str == BottomAligned)
            return VerticalFormatting::BOTTOM_ALIGNED;

        else if (str == Tiled)
            return VerticalFormatting::TILED;

        else if (str == Stretched)
            return VerticalFormatting::STRETCHED;

        else
            return VerticalFormatting::TOP_ALIGNED;
    }

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String TopAligned;
    static const CEGUI::String CentreAligned;
    static const CEGUI::String BottomAligned;
    static const CEGUI::String Stretched;
    static const CEGUI::String Tiled;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<HorizontalFormatting>
{
public:
    typedef HorizontalFormatting return_type;
    typedef HorizontalFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case HorizontalFormatting::RIGHT_ALIGNED:
            return RightAligned;

        case HorizontalFormatting::CENTRE_ALIGNED:
            return CentreAligned;

        case HorizontalFormatting::TILED:
            return Tiled;

        case HorizontalFormatting::STRETCHED:
            return Stretched;

        default:
            return LeftAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return HorizontalFormatting::CENTRE_ALIGNED;

        else if (str == RightAligned)
            return HorizontalFormatting::RIGHT_ALIGNED;

        else if (str == Tiled)
            return HorizontalFormatting::TILED;

        else if (str == Stretched)
            return HorizontalFormatting::STRETCHED;

        else
            return HorizontalFormatting::LEFT_ALIGNED;
    }

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String LeftAligned;
    static const CEGUI::String CentreAligned;
    static const CEGUI::String RightAligned;
    static const CEGUI::String Stretched;
    static const CEGUI::String Tiled;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<VerticalAlignment>
{
public:
    typedef VerticalAlignment return_type;
    typedef VerticalAlignment pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case VA_BOTTOM:
            return BottomAligned;

        case VA_CENTRE:
            return CentreAligned;

        default:
            return TopAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return VA_CENTRE;

        else if (str == BottomAligned)
            return VA_BOTTOM;

        else
            return VA_TOP;
    }

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String TopAligned;
    static const CEGUI::String CentreAligned;
    static const CEGUI::String BottomAligned;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<HorizontalAlignment>
{
public:
    typedef HorizontalAlignment return_type;
    typedef HorizontalAlignment pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case HA_RIGHT:
            return RightAligned;

        case HA_CENTRE:
            return CentreAligned;

        default:
            return LeftAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return HA_CENTRE;

        else if (str == RightAligned)
            return HA_RIGHT;

        else
            return HA_LEFT;
    }

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String LeftAligned;
    static const CEGUI::String CentreAligned;
    static const CEGUI::String RightAligned;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<DimensionType>
{
public:
    typedef DimensionType return_type;
    typedef DimensionType pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case DimensionType::LEFT_EDGE:
            return LeftEdge;

        case DimensionType::X_POSITION:
            return XPosition;

        case DimensionType::TOP_EDGE:
            return TopEdge;

        case DimensionType::Y_POSITION:
            return YPosition;

        case DimensionType::RIGHT_EDGE:
            return RightEdge;

        case DimensionType::BOTTOM_EDGE:
            return BottomEdge;

        case DimensionType::WIDTH:
            return Width;

        case DimensionType::HEIGHT:
            return Height;

        case DimensionType::X_OFFSET:
            return XOffset;

        case DimensionType::Y_OFFSET:
            return YOffset;

        default:
            return Invalid;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == LeftEdge)
            return DimensionType::LEFT_EDGE;

        else if (str == XPosition)
            return DimensionType::X_POSITION;

        else if (str == TopEdge)
            return DimensionType::TOP_EDGE;

        else if (str == YPosition)
            return DimensionType::Y_POSITION;

        else if (str == RightEdge)
            return DimensionType::RIGHT_EDGE;

        else if (str == BottomEdge)
            return DimensionType::BOTTOM_EDGE;

        else if (str == Width)
            return DimensionType::WIDTH;

        else if (str == Height)
            return DimensionType::HEIGHT;

        else if (str == XOffset)
            return DimensionType::X_OFFSET;

        else if (str == YOffset)
            return DimensionType::Y_OFFSET;

        else
            return DimensionType::INVALID;
    }

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String LeftEdge;
    static const CEGUI::String RightEdge;
    static const CEGUI::String BottomEdge;
    static const CEGUI::String TopEdge;
    static const CEGUI::String XPosition;
    static const CEGUI::String YPosition;
    static const CEGUI::String Width;
    static const CEGUI::String Height;
    static const CEGUI::String XOffset;
    static const CEGUI::String YOffset;
    static const CEGUI::String Invalid;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<VerticalTextFormatting>
{
public:
    typedef VerticalTextFormatting return_type;
    typedef VerticalTextFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case VerticalTextFormatting::BOTTOM_ALIGNED:
            return BottomAligned;

        case VerticalTextFormatting::CENTRE_ALIGNED:
            return CentreAligned;

        default:
            return TopAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return VerticalTextFormatting::CENTRE_ALIGNED;

        else if (str == BottomAligned)
            return VerticalTextFormatting::BOTTOM_ALIGNED;

        else
            return VerticalTextFormatting::TOP_ALIGNED;
    }

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String TopAligned;
    static const CEGUI::String CentreAligned;
    static const CEGUI::String BottomAligned;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<HorizontalTextFormatting>
{
public:
    typedef HorizontalTextFormatting return_type;
    typedef HorizontalTextFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case HorizontalTextFormatting::RIGHT_ALIGNED:
            return RightAligned;

        case HorizontalTextFormatting::CENTRE_ALIGNED:
            return CentreAligned;

        case HorizontalTextFormatting::JUSTIFIED:
            return Justified;

        case HorizontalTextFormatting::WORDWRAP_LEFT_ALIGNED:
            return WordWrapLeftAligned;

        case HorizontalTextFormatting::WORDWRAP_RIGHT_ALIGNED:
            return WordWrapRightAligned;

        case HorizontalTextFormatting::WORDWRAP_CENTRE_ALIGNED:
            return WordWrapCentreAligned;

        case HorizontalTextFormatting::WORDWRAP_JUSTIFIED:
            return WordWrapJustified;

        default:
            return LeftAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return HorizontalTextFormatting::CENTRE_ALIGNED;

        else if (str == RightAligned)
            return HorizontalTextFormatting::RIGHT_ALIGNED;

        else if (str == Justified)
            return HorizontalTextFormatting::JUSTIFIED;

        else if (str == WordWrapLeftAligned)
            return HorizontalTextFormatting::WORDWRAP_LEFT_ALIGNED;

        else if (str == WordWrapCentreAligned)
            return HorizontalTextFormatting::WORDWRAP_CENTRE_ALIGNED;

        else if (str == WordWrapRightAligned)
            return HorizontalTextFormatting::WORDWRAP_RIGHT_ALIGNED;

        else if (str == WordWrapJustified)
            return HorizontalTextFormatting::WORDWRAP_JUSTIFIED;

        else
            return HorizontalTextFormatting::LEFT_ALIGNED;
    }

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String LeftAligned;
    static const CEGUI::String CentreAligned;
    static const CEGUI::String RightAligned;
    static const CEGUI::String Justified;
    static const CEGUI::String WordWrapLeftAligned;
    static const CEGUI::String WordWrapRightAligned;
    static const CEGUI::String WordWrapCentreAligned;
    static const CEGUI::String WordWrapJustified;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<FontMetricType>
{
public:
    typedef FontMetricType return_type;
    typedef FontMetricType pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case FontMetricType::BASELINE:
            return Baseline;

        case FontMetricType::HORZ_EXTENT:
            return HorzExtent;

        default:
            return LineSpacing;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == LineSpacing)
            return FontMetricType::LINE_SPACING;

        else if (str == Baseline)
            return FontMetricType::BASELINE;

        else
            return FontMetricType::HORZ_EXTENT;
    }

    static const CEGUI::String LineSpacing;
    static const CEGUI::String HorzExtent;
    static const CEGUI::String Baseline;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<DimensionOperator>
{
public:
    typedef DimensionOperator return_type;
    typedef DimensionOperator pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case DimensionOperator::ADD:
            return Add;

        case DimensionOperator::SUBTRACT:
            return Subtract;

        case DimensionOperator::MULTIPLY:
            return Multiply;

        case DimensionOperator::DIVIDE:
            return Divide;

        default:
            return Noop;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == Add)
            return DimensionOperator::ADD;

        else if (str == Subtract)
            return DimensionOperator::SUBTRACT;

        else if (str == Multiply)
            return DimensionOperator::MULTIPLY;

        else if (str == Divide)
            return DimensionOperator::DIVIDE;

        else
            return DimensionOperator::NOOP;
    }

    static const CEGUI::String Add;
    static const CEGUI::String Subtract;
    static const CEGUI::String Multiply;
    static const CEGUI::String Divide;
    static const CEGUI::String Noop;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<FrameImageComponent>
{
public:
    typedef FrameImageComponent return_type;
    typedef FrameImageComponent pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case FrameImageComponent::TOP_LEFT_CORNER:
            return TopLeftCorner;

        case FrameImageComponent::TOP_RIGHT_CORNER:
            return TopRightCorner;

        case FrameImageComponent::BOTTOM_LEFT_CORNER:
            return BottomLeftCorner;

        case FrameImageComponent::BOTTOM_RIGHT_CORNER:
            return BottomRightCorner;

        case FrameImageComponent::LEFT_EDGE:
            return LeftEdge;

        case FrameImageComponent::RIGHT_EDGE:
            return RightEdge;

        case FrameImageComponent::TOP_EDGE:
            return TopEdge;

        case FrameImageComponent::BOTTOM_EDGE:
            return BottomEdge;

        default:
            return Background;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == TopLeftCorner)
            return FrameImageComponent::TOP_LEFT_CORNER;

        if (str == TopRightCorner)
            return FrameImageComponent::TOP_RIGHT_CORNER;

        if (str == BottomLeftCorner)
            return FrameImageComponent::BOTTOM_LEFT_CORNER;

        if (str == BottomRightCorner)
            return FrameImageComponent::BOTTOM_RIGHT_CORNER;

        if (str == LeftEdge)
            return FrameImageComponent::LEFT_EDGE;

        if (str == RightEdge)
            return FrameImageComponent::RIGHT_EDGE;

        if (str == TopEdge)
            return FrameImageComponent::TOP_EDGE;

        if (str == BottomEdge)
            return FrameImageComponent::BOTTOM_EDGE;

        else
            return FrameImageComponent::BACKGROUND;
    }

    static const CEGUI::String TopLeftCorner;
    static const CEGUI::String TopRightCorner;
    static const CEGUI::String BottomLeftCorner;
    static const CEGUI::String BottomRightCorner;
    static const CEGUI::String TopEdge;
    static const CEGUI::String LeftEdge;
    static const CEGUI::String RightEdge;
    static const CEGUI::String BottomEdge;
    static const CEGUI::String Background;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<ChildEventAction>
{
public:
    typedef ChildEventAction return_type;
    typedef ChildEventAction pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case ChildEventAction::REDRAW:
            return Redraw;

        case ChildEventAction::LAYOUT:
            return Layout;

        default:
            throw InvalidRequestException(
                "Invalid enumeration value given.");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == Redraw)
            return ChildEventAction::REDRAW;

        if (str == Layout)
            return ChildEventAction::LAYOUT;

        throw InvalidRequestException(
            "'" + str +
            "' does not represent a ChildEventAction enumerated value.");
    }

    static const CEGUI::String Redraw;
    static const CEGUI::String Layout;
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
