/***********************************************************************
    filename:   FalStaticText.h
    created:    Tue Jul 5 2005
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
#ifndef _FalStaticText_h_
#define _FalStaticText_h_

#include "FalModule.h"
#include "FalStatic.h"
#include "FalStaticTextProperties.h"
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        StaticText class for the FalagardBase module.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States:
            - Enabled                     - basic rendering for enabled state.
            - Disabled                    - basic rendering for disabled state.
            - EnabledFrame                - frame rendering for enabled state
            - DisabledFrame               - frame rendering for disabled state.
            - WithFrameEnabledBackground  - backdrop rendering for enabled state with frame enabled.
            - WithFrameDisabledBackground - backdrop rendering for disabled state with frame enabled.
            - NoFrameEnabledBackground    - backdrop rendering for enabled state with frame disabled.
            - NoFrameDisabledBackground   - backdrop rendering for disabled state with frame disabled.

        Named Areas (missing areas will default to 'WithFrameTextRenderArea'):
            WithFrameTextRenderArea
            WithFrameTextRenderAreaHScroll
            WithFrameTextRenderAreaVScroll
            WithFrameTextRenderAreaHVScroll
            NoFrameTextRenderArea
            NoFrameTextRenderAreaHScroll
            NoFrameTextRenderAreaVScroll
            NoFrameTextRenderAreaHVScroll
    */
    class FALAGARDBASE_API FalagardStaticText : public FalagardStatic
    {
    public:
        static const utf8   TypeName[];       //!< type name for this widget.

        /*************************************************************************
	        Formatting Enumerations
        *************************************************************************/
        /*!
        \brief
            Enumeration of horizontal formatting options for falagard static text widgets
        */
        enum HorzFormatting
        {
            LeftAligned,            //!< Text is output as a single line of text with the first character aligned with the left edge of the widget.
            RightAligned,           //!< Text is output as a single line of text with the last character aligned with the right edge of the widget.
            HorzCentred,            //!< Text is output as a single line of text horizontally centred within the widget.
            HorzJustified,          //!< Text is output as a single line of text with the first and last characters aligned with the edges of the widget.
            WordWrapLeftAligned,    //!< Text is output as multiple word-wrapped lines of text with the first character of each line aligned with the left edge of the widget.
            WordWrapRightAligned,   //!< Text is output as multiple word-wrapped lines of text with the last character of each line aligned with the right edge of the widget.
            WordWrapCentred,        //!< Text is output as multiple word-wrapped lines of text with each line horizontally centered within the widget.
            WordWrapJustified       //!< Text is output as multiple word-wrapped lines of text with the first and last characters of each line aligned with the edges of the widget.
        };

        /*!
        \brief
            Enumeration of vertical formatting options for a falagard static text widgets
        */
        enum VertFormatting
        {
            TopAligned,     //!< Text is output with the top of first line of text aligned with the top edge of the widget.
            BottomAligned,  //!< Text is output with the bottom of last line of text aligned with the bottom edge of the widget.
            VertCentred     //!< Text is output vertically centred within the widget.
        };

        /*************************************************************************
            Child Widget name suffix constants
        *************************************************************************/
        static const String VertScrollbarNameSuffix;   //!< Widget name suffix for the vertical scrollbar component.
        static const String HorzScrollbarNameSuffix;   //!< Widget name suffix for the horizontal scrollbar component.

        /************************************************************************
            Constructor / Destructor
        *************************************************************************/
        /*!
        \brief
            Constructor
        */
        FalagardStaticText(const String& type);

        /************************************************************************
            Scrolled text implementation
        *************************************************************************/
        /*!
        \brief
            Return a ColourRect object containing the colours used when rendering this widget.
        */
        ColourRect    getTextColours(void) const                {return d_textCols;}

        /*!
        \brief
            Return the current horizontal formatting option set for this widget.
        */
        HorzFormatting    getHorizontalFormatting(void) const   {return    d_horzFormatting;}

        /*!
        \brief
            Return the current vertical formatting option set for this widget.
        */
        VertFormatting    getVerticalFormatting(void) const     {return d_vertFormatting;}

        /*!
        \brief
            Sets the colours to be applied when rendering the text.
        */
        void    setTextColours(const ColourRect& colours);

        /*!
        \brief
            Set the vertical formatting required for the text.
        */
        void    setVerticalFormatting(VertFormatting v_fmt);

        /*!
        \brief
            Set the horizontal formatting required for the text.
        */
        void    setHorizontalFormatting(HorzFormatting h_fmt);

        /*!
        \brief
            Return whether the vertical scroll bar is set to be shown if needed.
        */
        bool    isVerticalScrollbarEnabled(void) const  {return d_enableVertScrollbar;}

        /*!
        \brief
            Return whether the horizontal scroll bar is set to be shown if needed.
        */
        bool    isHorizontalScrollbarEnabled(void) const    {return d_enableHorzScrollbar;}

        /*!
	    \brief
            Set whether the vertical scroll bar will be shown if needed.
        */
        void    setVerticalScrollbarEnabled(bool setting);

        /*!
        \brief
            Set whether the horizontal scroll bar will be shown if needed.
        */
        void    setHorizontalScrollbarEnabled(bool setting);

        void render(void);

    protected:
        // overridden from FalagardStatic base class
        void onLookNFeelAssigned();
        void onLookNFeelUnassigned();

        // text field with scrollbars methods
        void renderScrolledText(void);
        void configureScrollbars(void);
        Scrollbar* getVertScrollbar(void) const;
        Scrollbar* getHorzScrollbar(void) const;
        Rect getTextRenderArea(void) const;
        Size getDocumentSize(const Rect& renderArea) const;

        // overridden event handlers
        bool onTextChanged(const EventArgs& e);
        bool onSized(const EventArgs& e);
        bool onFontChanged(const EventArgs& e);
        bool onMouseWheel(const EventArgs& e);

        // event subscribers
        bool handleScrollbarChange(const EventArgs& e);

        // properties
        static FalagardStaticTextProperties::TextColours    d_textColoursProperty;
        static FalagardStaticTextProperties::VertFormatting d_vertFormattingProperty;
        static FalagardStaticTextProperties::HorzFormatting d_horzFormattingProperty;
        static FalagardStaticTextProperties::VertScrollbar  d_vertScrollbarProperty;
        static FalagardStaticTextProperties::HorzScrollbar  d_horzScrollbarProperty;

        // implementation data
        HorzFormatting  d_horzFormatting;       //!< Horizontal formatting to be applied to the text.
        VertFormatting  d_vertFormatting;       //!< Vertical formatting to be applied to the text.
        ColourRect      d_textCols;             //!< Colours used when rendering the text.
        bool            d_enableVertScrollbar;  //!< true if vertical scroll bar is enabled.
        bool            d_enableHorzScrollbar;  //!< true if horizontal scroll bar is enabled.

        typedef std::vector<Event::Connection> ConnectionList;
        ConnectionList  d_connections;
    };

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _FalStaticText_h_
