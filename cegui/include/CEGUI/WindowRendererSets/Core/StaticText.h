/***********************************************************************
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

#include "CEGUI/WindowRendererSets/Core/Module.h"
#include "CEGUI/WindowRendererSets/Core/Static.h"
#include "CEGUI/falagard/Enums.h"
#include "CEGUI/ColourRect.h"
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
    class COREWRSET_API FalagardStaticText : public FalagardStatic
    {
    public:
        static const String TypeName;       //!< type name for this widget.

        /*************************************************************************
            Child Widget name constants
        *************************************************************************/
        static const String VertScrollbarName;   //!< Widget name for the vertical scrollbar component.
        static const String HorzScrollbarName;   //!< Widget name for the horizontal scrollbar component.

        /************************************************************************
            Constructor / Destructor
        *************************************************************************/
        /*!
        \brief
            Constructor
        */
        FalagardStaticText(const String& type);

        //! Destructor.
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
        HorizontalTextFormatting getHorizontalFormatting(void) const   {return    d_horzFormatting;}

        /*!
        \brief
            Return the current vertical formatting option set for this widget.
        */
        VerticalTextFormatting getVerticalFormatting(void) const     {return d_vertFormatting;}

        /*!
        \brief
            Sets the colours to be applied when rendering the text.
        */
        void    setTextColours(const ColourRect& colours);

        /*!
        \brief
            Set the vertical formatting required for the text.
        */
        void    setVerticalFormatting(VerticalTextFormatting v_fmt);

        /*!
        \brief
            Set the horizontal formatting required for the text.
        */
        void    setHorizontalFormatting(HorizontalTextFormatting h_fmt);

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

        //! return the current horizontal formatted text extent in pixels.
        float getHorizontalTextExtent() const;

        //! return the current vertical formatted text extent in pixels.
        float getVerticalTextExtent() const;

        // overridden from base class
        bool handleFontRenderSizeChange(const Font* const font);
        void render(void);

    protected:
        //! update string formatting (gets area size to use from looknfeel)
        void updateFormatting() const;
        //! update string formatting using given area size.
        void updateFormatting(const Sizef& sz) const;

        // overridden from FalagardStatic base class
        void onLookNFeelAssigned();
        void onLookNFeelUnassigned();

        // text field with scrollbars methods
        void renderScrolledText(void);
        void configureScrollbars(void);
        Scrollbar* getVertScrollbar(void) const;
        Scrollbar* getHorzScrollbar(void) const;
        Rectf getTextRenderArea(void) const;
        Sizef getDocumentSize(const Rectf& renderArea) const;
        void setupStringFormatter() const;

        // overridden event handlers
        bool onTextChanged(const EventArgs& e);
        bool onSized(const EventArgs& e);
        bool onFontChanged(const EventArgs& e);
        bool onMouseWheel(const EventArgs& e);

        // event subscribers
        bool handleScrollbarChange(const EventArgs& e);

        // implementation data
        //! Horizontal formatting to be applied to the text.
        HorizontalTextFormatting d_horzFormatting;
        //! Vertical formatting to be applied to the text.
        VerticalTextFormatting d_vertFormatting;
        ColourRect      d_textCols;             //!< Colours used when rendering the text.
        bool            d_enableVertScrollbar;  //!< true if vertical scroll bar is enabled.
        bool            d_enableHorzScrollbar;  //!< true if horizontal scroll bar is enabled.

        //! Class that renders RenderedString with some formatting.
        mutable FormattedRenderedString* d_formattedRenderedString;

        typedef std::vector<Event::Connection> ConnectionList;
        ConnectionList  d_connections;

        //! true when string formatting is up to date.
        mutable bool d_formatValid;
    };

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _FalStaticText_h_
