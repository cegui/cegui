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

#include "../VerticalAlignment.h"
#include "../HorizontalAlignment.h"
#include "../text/DefaultParagraphDirection.h"
#include "../Exceptions.h"
#include "CEGUI/falagard/Enums.h"

namespace CEGUI
{
template<typename T>
class FalagardXMLHelper;

template<>
class CEGUIEXPORT FalagardXMLHelper<VerticalImageFormatting>
{
public:
    typedef VerticalImageFormatting return_type;
    typedef VerticalImageFormatting pass_type;

    static String toString(pass_type val)
    {
        switch (val)
        {
        case VerticalImageFormatting::BottomAligned:
            return BottomAligned;

        case VerticalImageFormatting::CentreAligned:
            return CentreAligned;

        case VerticalImageFormatting::Tiled:
            return Tiled;

        case VerticalImageFormatting::Stretched:
            return Stretched;

        default:
            return TopAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return VerticalImageFormatting::CentreAligned;

        else if (str == BottomAligned)
            return VerticalImageFormatting::BottomAligned;

        else if (str == Tiled)
            return VerticalImageFormatting::Tiled;

        else if (str == Stretched)
            return VerticalImageFormatting::Stretched;

        else
            return VerticalImageFormatting::TopAligned;
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
        case HorizontalFormatting::RightAligned:
            return RightAligned;

        case HorizontalFormatting::CentreAligned:
            return CentreAligned;

        case HorizontalFormatting::Tiled:
            return Tiled;

        case HorizontalFormatting::Stretched:
            return Stretched;

        default:
            return LeftAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return HorizontalFormatting::CentreAligned;

        else if (str == RightAligned)
            return HorizontalFormatting::RightAligned;

        else if (str == Tiled)
            return HorizontalFormatting::Tiled;

        else if (str == Stretched)
            return HorizontalFormatting::Stretched;

        else
            return HorizontalFormatting::LeftAligned;
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
        case VerticalAlignment::Bottom:
            return BottomAligned;

        case VerticalAlignment::Centre:
            return CentreAligned;

        default:
            return TopAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return VerticalAlignment::Centre;

        else if (str == BottomAligned)
            return VerticalAlignment::Bottom;

        else
            return VerticalAlignment::Top;
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
        case HorizontalAlignment::Right:
            return RightAligned;

        case HorizontalAlignment::Centre:
            return CentreAligned;

        default:
            return LeftAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return HorizontalAlignment::Centre;

        else if (str == RightAligned)
            return HorizontalAlignment::Right;

        else
            return HorizontalAlignment::Left;
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
        case DimensionType::LeftEdge:
            return LeftEdge;

        case DimensionType::XPosition:
            return XPosition;

        case DimensionType::TopEdge:
            return TopEdge;

        case DimensionType::YPosition:
            return YPosition;

        case DimensionType::RightEdge:
            return RightEdge;

        case DimensionType::BottomEdge:
            return BottomEdge;

        case DimensionType::Width:
            return Width;

        case DimensionType::Height:
            return Height;

        case DimensionType::XOffset:
            return XOffset;

        case DimensionType::YOffset:
            return YOffset;

        default:
            return Invalid;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == LeftEdge)
            return DimensionType::LeftEdge;

        else if (str == XPosition)
            return DimensionType::XPosition;

        else if (str == TopEdge)
            return DimensionType::TopEdge;

        else if (str == YPosition)
            return DimensionType::YPosition;

        else if (str == RightEdge)
            return DimensionType::RightEdge;

        else if (str == BottomEdge)
            return DimensionType::BottomEdge;

        else if (str == Width)
            return DimensionType::Width;

        else if (str == Height)
            return DimensionType::Height;

        else if (str == XOffset)
            return DimensionType::XOffset;

        else if (str == YOffset)
            return DimensionType::YOffset;

        else
            return DimensionType::Invalid;
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
        case VerticalTextFormatting::BottomAligned:
            return BottomAligned;

        case VerticalTextFormatting::CentreAligned:
            return CentreAligned;

        default:
            return TopAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return VerticalTextFormatting::CentreAligned;

        else if (str == BottomAligned)
            return VerticalTextFormatting::BottomAligned;

        else
            return VerticalTextFormatting::TopAligned;
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
            case HorizontalTextFormatting::RightAligned:
                return RightAligned;
            case HorizontalTextFormatting::CentreAligned:
                return CentreAligned;
            case HorizontalTextFormatting::Justified:
                return Justified;
            case HorizontalTextFormatting::Bidi:
                return Bidi;

            // TODO: deprecated, remove later when users migrate their data
            case HorizontalTextFormatting::WordWrapLeftAligned:
                return WordWrapLeftAligned;
            case HorizontalTextFormatting::WordWrapCentreAligned:
                return WordWrapCentreAligned;
            case HorizontalTextFormatting::WordWrapRightAligned:
                return WordWrapRightAligned;
            case HorizontalTextFormatting::WordWrapJustified:
                return WordWrapJustified;
            ////////////////////////////////////////////////////////////////

            default:
                return LeftAligned;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == CentreAligned)
            return HorizontalTextFormatting::CentreAligned;
        else if (str == RightAligned)
            return HorizontalTextFormatting::RightAligned;
        else if (str == Justified)
            return HorizontalTextFormatting::Justified;
        else if (str == Bidi)
            return HorizontalTextFormatting::Bidi;

        // TODO: deprecated, remove later when users migrate their data
        else if (str == WordWrapLeftAligned)
            return HorizontalTextFormatting::WordWrapLeftAligned;
        else if (str == WordWrapCentreAligned)
            return HorizontalTextFormatting::WordWrapCentreAligned;
        else if (str == WordWrapRightAligned)
            return HorizontalTextFormatting::WordWrapRightAligned;
        else if (str == WordWrapJustified)
            return HorizontalTextFormatting::WordWrapJustified;
        ////////////////////////////////////////////////////////////////

        else
            return HorizontalTextFormatting::LeftAligned;
    }

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String LeftAligned;
    static const CEGUI::String CentreAligned;
    static const CEGUI::String RightAligned;
    static const CEGUI::String Justified;
    static const CEGUI::String Bidi;

    // TODO: deprecated, remove later when users migrate their data
    static const CEGUI::String WordWrapLeftAligned;
    static const CEGUI::String WordWrapRightAligned;
    static const CEGUI::String WordWrapCentreAligned;
    static const CEGUI::String WordWrapJustified;
};

template<>
class CEGUIEXPORT FalagardXMLHelper<DefaultParagraphDirection>
{
public:
    typedef DefaultParagraphDirection return_type;
    typedef DefaultParagraphDirection pass_type;

    static return_type fromString(const String& str)
    {
        if (str == "RightToLeft")
        {
            return DefaultParagraphDirection::RightToLeft;
        }

        if (str == "Automatic")
        {
            return DefaultParagraphDirection::Automatic;
        }

        return DefaultParagraphDirection::LeftToRight;
    }

    static String toString(pass_type val)
    {
        if (val == DefaultParagraphDirection::LeftToRight)
            return "LeftToRight";
        else if (val == DefaultParagraphDirection::RightToLeft)
            return "RightToLeft";
        else if (val == DefaultParagraphDirection::Automatic)
            return "Automatic";
        else
            return "LeftToRight";
    }
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
        case FontMetricType::Baseline:
            return Baseline;

        case FontMetricType::HorzExtent:
            return HorzExtent;

        default:
            return LineSpacing;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == LineSpacing)
            return FontMetricType::LineSpacing;

        else if (str == Baseline)
            return FontMetricType::Baseline;

        else
            return FontMetricType::HorzExtent;
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
        case DimensionOperator::Add:
            return Add;

        case DimensionOperator::Subtract:
            return Subtract;

        case DimensionOperator::Multiply:
            return Multiply;

        case DimensionOperator::Divide:
            return Divide;

        case DimensionOperator::Max:
            return Max;

        case DimensionOperator::Min:
            return Min;

        default:
            return Noop;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == Add)
            return DimensionOperator::Add;

        else if (str == Subtract)
            return DimensionOperator::Subtract;

        else if (str == Multiply)
            return DimensionOperator::Multiply;

        else if (str == Divide)
            return DimensionOperator::Divide;

        else if (str == Max)
            return DimensionOperator::Max;

        else if (str == Min)
            return DimensionOperator::Min;

        else
            return DimensionOperator::NoOp;
    }

    static const CEGUI::String Add;
    static const CEGUI::String Subtract;
    static const CEGUI::String Multiply;
    static const CEGUI::String Divide;
    static const CEGUI::String Max;
    static const CEGUI::String Min;
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
        case FrameImageComponent::TopLeftCorner:
            return TopLeftCorner;

        case FrameImageComponent::TopRightCorner:
            return TopRightCorner;

        case FrameImageComponent::BottomLeftCorner:
            return BottomLeftCorner;

        case FrameImageComponent::BottomRightCorner:
            return BottomRightCorner;

        case FrameImageComponent::LeftEdge:
            return LeftEdge;

        case FrameImageComponent::RightEdge:
            return RightEdge;

        case FrameImageComponent::TopEdge:
            return TopEdge;

        case FrameImageComponent::BottomEdge:
            return BottomEdge;

        default:
            return Background;
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == TopLeftCorner)
            return FrameImageComponent::TopLeftCorner;

        if (str == TopRightCorner)
            return FrameImageComponent::TopRightCorner;

        if (str == BottomLeftCorner)
            return FrameImageComponent::BottomLeftCorner;

        if (str == BottomRightCorner)
            return FrameImageComponent::BottomRightCorner;

        if (str == LeftEdge)
            return FrameImageComponent::LeftEdge;

        if (str == RightEdge)
            return FrameImageComponent::RightEdge;

        if (str == TopEdge)
            return FrameImageComponent::TopEdge;

        if (str == BottomEdge)
            return FrameImageComponent::BottomEdge;

        else
            return FrameImageComponent::Background;
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
        case ChildEventAction::Redraw:
            return Redraw;

        case ChildEventAction::Layout:
            return Layout;

        default:
            throw InvalidRequestException(
                "Invalid enumeration value given.");
        }
    }

    static return_type fromString(const String& str)
    {
        if (str == Redraw)
            return ChildEventAction::Redraw;

        if (str == Layout)
            return ChildEventAction::Layout;

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
class PropertyHelper<VerticalImageFormatting>
{
public:
    typedef VerticalImageFormatting return_type;
    typedef return_type safe_method_return_type;
    typedef VerticalImageFormatting pass_type;
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
        static const String type("HorizontalTextFormatting");
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
class CEGUIEXPORT PropertyHelper<DefaultParagraphDirection>
{
public:
    typedef DefaultParagraphDirection return_type;
    typedef return_type safe_method_return_type;
    typedef DefaultParagraphDirection pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static const String type("DefaultParagraphDirection");
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
