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

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Enumeration of possible values to indicate what a given dimension represents.
    */
    enum class DimensionType : int
    {
        LEFT_EDGE,       //!< Dimension represents the left edge of some entity (same as X_POSITION).
        X_POSITION,      //!< Dimension represents the x position of some entity (same as LEFT_EDGE).
        TOP_EDGE,        //!< Dimension represents the top edge of some entity (same as Y_POSITION).
        Y_POSITION,      //!< Dimension represents the y position of some entity (same as TOP_EDGE).
        RIGHT_EDGE,      //!< Dimension represents the right edge of some entity.
        BOTTOM_EDGE,     //!< Dimension represents the bottom edge of some entity.
        WIDTH,           //!< Dimension represents the width of some entity.
        HEIGHT,          //!< Dimension represents the height of some entity.
        X_OFFSET,        //!< Dimension represents the x offset of some entity (usually only applies to an Image entity).
        Y_OFFSET,        //!< Dimension represents the y offset of some entity (usually only applies to an Image entity).
        INVALID          //!< Invalid / uninitialised DimensionType.
    };

    /*!
    \brief
        Enumeration of possible values to indicate the vertical formatting to be used for an image component.
    */
    enum class VerticalFormatting : int
    {
        TOP_ALIGNED,         //!< Top of Image should be aligned with the top of the destination area.
        CENTRE_ALIGNED,      //!< Image should be vertically centred within the destination area.
        BOTTOM_ALIGNED,      //!< Bottom of Image should be aligned with the bottom of the destination area.
        STRETCHED,           //!< Image should be stretched vertically to fill the destination area.
        TILED                //!< Image should be tiled vertically to fill the destination area (bottom-most tile may be clipped).
    };

    /*!
    \brief
        Enumeration of possible values to indicate the horizontal formatting to be used for an image component.
    */
    enum class HorizontalFormatting : int
    {
        LEFT_ALIGNED,        //!< Left of Image should be aligned with the left of the destination area.
        CENTRE_ALIGNED,      //!< Image should be horizontally centred within the destination area.
        RIGHT_ALIGNED,       //!< Right of Image should be aligned with the right of the destination area.
        STRETCHED,           //!< Image should be stretched horizontally to fill the destination area.
        TILED                //!< Image should be tiled horizontally to fill the destination area (right-most tile may be clipped).
    };

    /*!
    \brief
        Enumeration of possible values to indicate the vertical formatting to be used for a text component.
    */
    enum class VerticalTextFormatting : int
    {
        TOP_ALIGNED,         //!< Top of text should be aligned with the top of the destination area.
        CENTRE_ALIGNED,      //!< text should be vertically centred within the destination area.
        BOTTOM_ALIGNED       //!< Bottom of text should be aligned with the bottom of the destination area.
    };

    /*!
    \brief
        Enumeration of possible values to indicate the horizontal formatting to be used for a text component.
    */
    enum class HorizontalTextFormatting : int
    {
        LEFT_ALIGNED,        //!< Left of text should be aligned with the left of the destination area (single line of text only).
        RIGHT_ALIGNED,       //!< Right of text should be aligned with the right of the destination area  (single line of text only).
        CENTRE_ALIGNED,      //!< text should be horizontally centred within the destination area  (single line of text only).
        JUSTIFIED,           //!< text should be spaced so that it takes the full width of the destination area (single line of text only).
        WORDWRAP_LEFT_ALIGNED,    //!< Left of text should be aligned with the left of the destination area (word wrapped to multiple lines as needed).
        WORDWRAP_RIGHT_ALIGNED,   //!< Right of text should be aligned with the right of the destination area  (word wrapped to multiple lines as needed).
        WORDWRAP_CENTRE_ALIGNED,  //!< text should be horizontally centred within the destination area  (word wrapped to multiple lines as needed).
        WORDWRAP_JUSTIFIED        //!< text should be spaced so that it takes the full width of the destination area (word wrapped to multiple lines as needed).
    };

    /*!
    \brief
        Enumeration of possible values to indicate a particular font metric.
    */
    enum class FontMetricType : int
    {
        LINE_SPACING,       //!< Vertical line spacing value for font.
        BASELINE,           //!< Vertical baseline value for font.
        HORZ_EXTENT         //!< Horizontal extent of a string.
    };

    /*!
    \brief
        Enumeration of values representing mathematical operations on dimensions.
    */
    enum class DimensionOperator : int
    {
        NOOP,       //!< Do nothing operator.
        ADD,        //!< Dims should be added.
        SUBTRACT,   //!< Dims should be subtracted.
        MULTIPLY,   //!< Dims should be multiplied.
        DIVIDE      //!< Dims should be divided.
    };

    /*!
    \brief
        Enumeration of values referencing available images forming a frame component.
    */
    enum class FrameImageComponent : int
    {
        BACKGROUND,             //!< References image used for the background.
        TOP_LEFT_CORNER,        //!< References image used for the top-left corner.
        TOP_RIGHT_CORNER,       //!< References image used for the top-right corner.
        BOTTOM_LEFT_CORNER,     //!< References image used for the bottom-left corner.
        BOTTOM_RIGHT_CORNER,    //!< References image used for the bottom-right corner.
        LEFT_EDGE,              //!< References image used for the left edge.
        RIGHT_EDGE,             //!< References image used for the right edge.
        TOP_EDGE,               //!< References image used for the top edge.
        BOTTOM_EDGE,            //!< References image used for the bottom edge.
        FRAME_IMAGE_COUNT       //!< Max number of images for a frame.
    };

    /*!
    \brief
        Enumeration of actions a WidgetLook can take in response to some event
        firing on a child WidgetComponent.
    */
    enum class ChildEventAction : int
    {
        //! The window with the WidgetComponent as child is redrawn.
        REDRAW,
        //! The window with the WidgetComponent as child performs child layout.
        LAYOUT
    };


} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalEnums_h_
