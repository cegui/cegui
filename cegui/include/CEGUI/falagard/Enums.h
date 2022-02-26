/***********************************************************************
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
#ifndef _CEGUIFalEnums_h_
#define _CEGUIFalEnums_h_

#include <cstdint>

namespace CEGUI
{
    /*!
    \brief
        Enumeration of possible values to indicate what a given dimension represents.
    */
    enum class DimensionType : int
    {
        LeftEdge,       //!< Dimension represents the left edge of some entity (same as XPosition).
        XPosition,      //!< Dimension represents the x position of some entity (same as LeftEdge).
        TopEdge,        //!< Dimension represents the top edge of some entity (same as YPosition).
        YPosition,      //!< Dimension represents the y position of some entity (same as TopEdge).
        RightEdge,      //!< Dimension represents the right edge of some entity.
        BottomEdge,     //!< Dimension represents the bottom edge of some entity.
        Width,          //!< Dimension represents the width of some entity.
        Height,         //!< Dimension represents the height of some entity.
        XOffset,        //!< Dimension represents the x offset of some entity (usually only applies to an Image entity).
        YOffset,        //!< Dimension represents the y offset of some entity (usually only applies to an Image entity).
        Invalid         //!< Invalid / uninitialised DimensionType.
    };

    /*!
    \brief
        Enumeration of possible values to indicate the vertical formatting to be used for an image component.
    */
    enum class VerticalImageFormatting : uint8_t
    {
        TopAligned,         //!< Top of Image should be aligned with the top of the destination area.
        CentreAligned,      //!< Image should be vertically centred within the destination area.
        BottomAligned,      //!< Bottom of Image should be aligned with the bottom of the destination area.
        Stretched,          //!< Image should be stretched vertically to fill the destination area.
        Tiled               //!< Image should be tiled vertically to fill the destination area (bottom-most tile may be clipped).
    };

    /*!
    \brief
        Enumeration of possible values to indicate the horizontal formatting to be used for an image component.
    */
    enum class HorizontalFormatting : uint8_t
    {
        LeftAligned,        //!< Left of Image should be aligned with the left of the destination area.
        CentreAligned,      //!< Image should be horizontally centred within the destination area.
        RightAligned,       //!< Right of Image should be aligned with the right of the destination area.
        Stretched,          //!< Image should be stretched horizontally to fill the destination area.
        Tiled               //!< Image should be tiled horizontally to fill the destination area (right-most tile may be clipped).
    };

    /*!
    \brief
        Enumeration of possible values to indicate the vertical formatting to be used for a text component.
    */
    enum class VerticalTextFormatting : uint8_t
    {
        TopAligned,         //!< Top of text should be aligned with the top of the destination area.
        CentreAligned,      //!< text should be vertically centred within the destination area.
        BottomAligned       //!< Bottom of text should be aligned with the bottom of the destination area.
    };

    /*!
    \brief
        Enumeration of possible values to indicate the horizontal formatting to be used for a text component.
    */
    enum class HorizontalTextFormatting : uint8_t
    {
        LeftAligned,        //!< Left of text should be aligned with the left of the destination area.
        RightAligned,       //!< Right of text should be aligned with the right of the destination area.
        CentreAligned,      //!< Text should be horizontally centred within the destination area.
        Justified,          //!< Text should be spaced so that it takes the full width of the destination area.
        Bidi,               //!< LeftAligned or RightAligned based on the paragraph BIDI direction.

        // TODO: deprecated, remove later when users migrate their data
        WordWrapLeftAligned,
        WordWrapRightAligned,
        WordWrapCentreAligned,
        WordWrapJustified
    };

    // TODO: deprecated, remove later when users migrate their data
    inline HorizontalTextFormatting decomposeHorizontalFormatting(HorizontalTextFormatting fmt, bool* wordWrap = nullptr)
    {
        if (wordWrap)
            *wordWrap = true;

        switch (fmt)
        {
            case HorizontalTextFormatting::WordWrapLeftAligned:
                return HorizontalTextFormatting::LeftAligned;
            case HorizontalTextFormatting::WordWrapRightAligned:
                return HorizontalTextFormatting::RightAligned;
            case HorizontalTextFormatting::WordWrapCentreAligned:
                return HorizontalTextFormatting::CentreAligned;
            case HorizontalTextFormatting::WordWrapJustified:
                return HorizontalTextFormatting::Justified;
        }

        if (wordWrap)
            *wordWrap = false;

        return fmt;
    }

    /*!
    \brief
        Enumeration of possible values to indicate a particular font metric.
    */
    enum class FontMetricType : int
    {
        LineSpacing,        //!< Vertical line spacing value for font.
        Baseline,           //!< Vertical baseline value for font.
        HorzExtent          //!< Horizontal extent of a string.
    };

    /*!
    \brief
        Enumeration of values representing mathematical operations on dimensions.
    */
    enum class DimensionOperator : int
    {
        NoOp,       //!< Do nothing operator.
        Add,        //!< Dims should be added.
        Subtract,   //!< Dims should be subtracted.
        Multiply,   //!< Dims should be multiplied.
        Divide,     //!< Dims should be divided.
        Max,        //!< Return max dim.
        Min         //!< Return min dim.
    };

    /*!
    \brief
        Enumeration of values referencing available images forming a frame component.
    */
    enum class FrameImageComponent : int
    {
        Background,         //!< References image used for the background.
        TopLeftCorner,      //!< References image used for the top-left corner.
        TopRightCorner,     //!< References image used for the top-right corner.
        BottomLeftCorner,   //!< References image used for the bottom-left corner.
        BottomRightCorner,  //!< References image used for the bottom-right corner.
        LeftEdge,           //!< References image used for the left edge.
        RightEdge,          //!< References image used for the right edge.
        TopEdge,            //!< References image used for the top edge.
        BottomEdge,         //!< References image used for the bottom edge.
        FrameImageCount     //!< Max number of images for a frame.
    };

    /*!
    \brief
        Enumeration of actions a WidgetLook can take in response to some event
        firing on a child WidgetComponent.
    */
    enum class ChildEventAction : int
    {
        //! The window with the WidgetComponent as child is redrawn.
        Redraw,
        //! The window with the WidgetComponent as child performs child layout.
        Layout
    };


} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalEnums_h_
