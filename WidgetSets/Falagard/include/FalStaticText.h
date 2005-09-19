/************************************************************************
    filename:   FalStaticText.h
    created:    Tue Jul 5 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _FalStaticText_h_
#define _FalStaticText_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalStatic.h"
#include "FalStaticTextProperties.h"

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
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

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

        /************************************************************************
            Constructor / Destructor
        *************************************************************************/
        /*!
        \brief
            Constructor
        */
        FalagardStaticText(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardStaticText();

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

    protected:
        // overridden from FalagardStatic base class.
        void initialise(void);
        void populateRenderCache(void);

        // text field with scrollbars methods
        void renderScrolledText(void);
        void configureScrollbars(void);
        Scrollbar* getVertScrollbar(void) const;
        Scrollbar* getHorzScrollbar(void) const;
        Rect getTextRenderArea(void) const;
        Size getDocumentSize(const Rect& renderArea) const;

        // overridden event handlers
        void onTextChanged(WindowEventArgs& e);
        void onSized(WindowEventArgs& e);
        void onFontChanged(WindowEventArgs& e);
        void onMouseWheel(MouseEventArgs& e);

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
    };

    /*!
    \brief
        WindowFactory for FalagardStaticText type Window objects.
    */
    class FALAGARDBASE_API FalagardStaticTextFactory : public WindowFactory
    {
    public:
        FalagardStaticTextFactory(void) : WindowFactory(FalagardStaticText::WidgetTypeName) { }
        ~FalagardStaticTextFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

} // End of  CEGUI namespace section


#endif  // end of guard _FalStaticText_h_
