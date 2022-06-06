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

#include "CEGUI/WindowRendererSets/Core/Static.h"
#include "CEGUI/falagard/Enums.h"
#include "CEGUI/text/RenderedText.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/Event.h"
#include <sstream>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class ComponentArea;

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

    /*!
    \brief
        A helper class to represent the value of the property
        "NumOfTextLinesToShow".

        The reason for this is the following: That property can get either a
        non-negative floating point value, or "Auto". This class enables to
        create a specialization "PropertyHelper<NumOfTextLinesToShow>",
        which knows to convert the property to/from string representation.
    */
    class NumOfTextLinesToShow
    {
    public:

        NumOfTextLinesToShow(float value = -1.f) : d_value(value) {}
        //! Is the value "Auto"?
        bool isAuto() const { return d_value < 0.f; }
        //! Set the avlue to "Auto".
        void setAuto() { d_value = -1.f; }
        //! Get the value. Only use it it's not "Auto".
        float get() const { return d_value; }
        //! Set the value to "newValue".
        void set(float newValue) { d_value = newValue; }
        //! Return the value. Only use it it's not "Auto".
        operator float() const { return d_value; }

    private:

        float d_value;
    };

    static const String TypeName;       //!< type name for this widget.

    static const String VertScrollbarName;   //!< Widget name for the vertical scrollbar component.
    static const String HorzScrollbarName;   //!< Widget name for the horizontal scrollbar component.

    FalagardStaticText(const String& type);
    virtual ~FalagardStaticText() override;

    //! \brief Return a ColourRect object containing the colours used when rendering this widget.
    const ColourRect& getTextColours() const { return d_textCols; }

    //! \brief Return the current horizontal formatting option set for this widget.
    HorizontalTextFormatting getHorizontalFormatting() const { return d_horzFormatting; }

    /*!
    \brief
        Return whether word-wrapping is turned on, which means words are
        wrapped to multiple lines as needed.
    */
    bool isWordWrapEnabled() const { return d_wordWrap; }

    /*!
    \brief
        Get the actual horizontal text formatting used, which may be
        different from the horizontal text formatting set with
        "setHorizontalFormatting".

        There are cases when "getHorizontalFormatting()" is different than
        "HorizontalTextFormatting::WORDWRAP_CENTRE_ALIGNED" and "HorizontalTextFormatting::CENTRE_ALIGNED",
        "getWindow()->isWidthAdjustedToContent()" is "true", and the number
        of formatted text lines ("getNumOfFormattedTextLines()") is 1, and
        it's obvious that the single line of text takes the whole width of
        the area reserved for the text. Consider, for example, the case
        where "getHorizontalFormatting()" is "HorizontalTextFormatting::LEFT_ALIGNED", and the
        number of original text lines ("getNumOfOriginalTextLines()") is 1.
        Then the width of the widget is automatically adjusted to the width
        of that line. Therefore we know that the single line of text takes
        the whole width of the area reserved for the text. So using an
        acutal horizontal text formatting of "HorizontalTextFormatting::CENTRE_ALIGNED" wouldn't
        hurt. The reason we prefer center horizontal text formatting in this
        case is that even though theoretically the single line of text takes
        the whole width of the area reserved for the text, in practice the
        width of text might be a pixel or 2 less, due to various reasons
        rooted in the computations performed. By using an actual horizontal
        text formatting of "HorizontalTextFormatting::CENTRE_ALIGNED", we make sure the text
        appears in the center (horizontally). The spaces in the sides may be
        tiny, but it can still make a difference visually.

    \see getHorizontalFormatting
    \see getActualVerticalFormatting
    */
    HorizontalTextFormatting getActualHorizontalFormatting() const { return d_actualHorzFormatting; }

    /*!
    \brief
        Return the current vertical formatting option set for this widget.
    */
    VerticalTextFormatting getVerticalFormatting() const { return d_vertFormatting; }

    /*!
    \brief
        Get the actual vertical text formatting used, which may be different
        from the vertical text formatting set with "setVerticalFormatting".

        There are cases when "getVerticalFormatting()" is different than
        "VerticalTextFormatting::CENTRE_ALIGNED", "getWindow()->isHeightAdjustedToContent()" is
        "true", and it's obvious that the text takes the whole height of the
        area reserved for the text. Consider, for example, the case where
        "getVerticalFormatting()" is "VerticalTextFormatting::TOP_ALIGNED",
        "getHorizontalFormatting()" is "HorizontalTextFormatting::CENTRE_ALIGNED", and the
        "NumOfTextLinesToShow" property is set to "Auto". Then the height of
        the widget is automatically adjusted to the height of the text.
        Therefore we know that the text takes the whole height of the area
        reserved for the text. So using an acutal vertical text formatting
        of "VerticalTextFormatting::CENTRE_ALIGNED" wouldn't hurt. The reason we prefer center
        vertical text formatting in this
        case is thatThe reason this matters at all is that even though
        theoretically the text takes the whole height of the area reserved
        for the text, in practice the height of the text might be a pixel or
        2 less, due to various reasons rooted in the computations performed.
        By using an actual vertical text formatting of "VerticalTextFormatting::CENTRE_ALIGNED",
        we make sure the text appears in the center (vertically). The spaces
        in the top and bottom may be tiny, but it can still make a
        difference visually.

    \see getVerticalFormatting
    \see getActualHorizontalFormatting
    */
    VerticalTextFormatting getActualVerticalFormatting() const { return d_actualVertFormatting; }

    /*
    \brief
        Get the number of text lines in the original (non-formatted) string.

    \see getNumOfFormattedTextLines
    \see getNumOfTextLinesToShow
    */
    std::size_t getNumOfOriginalTextLines() const;

    /*
    \brief
        If the "AdjustHeightToContent" property of the window is "true",
        then this method returns the number of text lines to show when
        adjusting the height to the content.

        If the "AdjustHeightToContent" property of the window is "true", let
        the number of text lines to show be "x". Then the following applies:
            - If "x" is "Auto",  the height of the widget is adjusted so
                that the whole widget is visible, without a need for a
                vertical scrollbar
            - If "x" is a real number (not "Auto"), the height of the widget
                is adjusted so that exactly "x" text lines can be visible,
                regardless on the text.

    \see setNumOfTextLinesToShow
    */
    NumOfTextLinesToShow getNumOfTextLinesToShow() const { return d_numOfTextLinesToShow; }

    /*
    \brief
        Get the number of text lines in the formatted string.

        That takes into account e.g. word-wrapping.

    \see getNumOfOriginalTextLines
    \see getNumOfTextLinesToShow
    */
    std::size_t getNumOfFormattedTextLines() const;

    /*!
    \brief
        Sets the colours to be applied when rendering the text.
    */
    void setTextColours(const ColourRect& colours);

    //! Set the horizontal formatting required for the text.
    void setHorizontalFormatting(HorizontalTextFormatting h_fmt);
    //! Enable or disable word wrapping.
    void setWordWrapEnabled(bool wrap);

    /*!
    \brief
        Set the vertical formatting required for the text.
    */
    void setVerticalFormatting(VerticalTextFormatting v_fmt);

    /*
    \brief
        If the "AdjustHeightToContent" property of the window is "true",
        Then this method sets the number of text lines to show when
        adjusting the height to the content.

        See the description of "getNumOfTextLinesToShow" for more details.

    \see getNumOfTextLinesToShow
    */
    void setNumOfTextLinesToShow(NumOfTextLinesToShow newValue);

    /*!
    \brief
        Return whether the vertical scroll bar is set to be shown if needed.
    */
    bool isVerticalScrollbarEnabled() const { return d_enableVertScrollbar; }

    /*!
    \brief
        Return whether the horizontal scroll bar is set to be shown if needed.
    */
    bool isHorizontalScrollbarEnabled() const { return d_enableHorzScrollbar; }

    /*!
	\brief
        Set whether the vertical scroll bar will be shown if needed.
    */
    void setVerticalScrollbarEnabled(bool setting);

    /*!
    \brief
        Set whether the horizontal scroll bar will be shown if needed.
    */
    void setHorizontalScrollbarEnabled(bool setting);

    //! return the current formatted text extent in pixels.
    Sizef getTextExtent() const;

    //! Get the height of a line of text, in pixels.
    float getLineHeight() const;

    /*!
    \brief
        Get the vertical advance, i.e. the number of pixels to move the
        "pen" from one line of text to the next.
    */
    float getVerticalAdvance() const;

    // overridden from base class
    bool handleFontRenderSizeChange(const Font* const font) override;
    void createRenderGeometry() override;

    /*!
    \brief
        Get the current position of the horizontal scrollbar within the
        text.

        The range of the scrollbar is from 0 to the width of the text minus
        the width of a page
        (0 <= position <= (textSize - pageSize)).

    \param position
        Float value specifying the position of the scrollbar within the
        text.
    */
    float getHorizontalScrollPosition() const;

    /*!
    \brief
        Get the current position of the vertical scrollbar within the text.

        The range of the scrollbar is from 0 to the height of the text minus
        the height of a page
        (0 <= position <= (textSize - pageSize)).

    \param position
        Float value specifying the position of the scrollbar within the
        text.
    */
    float getVerticalScrollPosition() const;
        
    /*!
    \brief
        return the current horizontal scrollbar position as a value in the
        interval [0, 1].
    */
    float getUnitIntervalHorizontalScrollPosition() const;

    /*!
    \brief
        return the current vertical scrollbar position as a value in the
        interval [0, 1].
    */
    float getUnitIntervalVerticalScrollPosition() const;

    /*!
    \brief
        Set the current position of the horizontal scrollbar within the
        text.

        The range of the scrollbar is from 0 to the width of the text minus
        the width of a page
        (0 <= position <= (textSize - pageSize)), any attempt to set the
        position outside this range will be adjusted so that it falls within
        the legal range.

    \param position
        Float value specifying the position of the scrollbar within the 
        text.
    */
    void setHorizontalScrollPosition(float position);

    /*!
    \brief
        Set the current position of the vertical scrollbar within the text.

        The range of the scrollbar is from 0 to the height of the text minus
        the height of a page
        (0 <= position <= (textSize - pageSize)), any attempt to set the
        position outside this range will be adjusted so that it falls within
        the legal range.

    \param position
        Float value specifying the position of the scrollbar within the
        text.
    */
    void setVerticalScrollPosition(float position);

    /*!
    \brief
        set the current horizontal scrollbar position as a value in the
        interval [0, 1].
    */
    void setUnitIntervalHorizontalScrollPosition(float position);

    /*!
    \brief
        set the current vertical scrollbar position as a value in the
        interval [0, 1].
    */
    void setUnitIntervalVerticalScrollPosition(float position);

    void onIsFrameEnabledChanged() override;

    Sizef getContentSize() const override;
    UDim getWidthOfAreaReservedForContentLowerBoundAsFuncOfWindowWidth() const override;
    UDim getHeightOfAreaReservedForContentLowerBoundAsFuncOfWindowHeight() const override;

    /*!
    \brief
        Set the size of the widget to the minimal value in which the whole
        widget content (the text) is visible, without the need for
        scrollbars, and without the need to split a word between 2 or more
        lines.
        
        Depending on the various combinations of the values of
        "getWindow()->isWidthAdjustedToContent()",
        "getWindow()->isHeightAdjustedToContent()",
        "getWindow()->getAspectMode()", "getNumOfTextLinesToShow()" and
        "isWordWrapEnabled", one of 3 methods is used:

        1) Call "adjustSizeToContent_direct", which adjusts the width and
            the height independently of each other, and then, if necessary,
            fixes it to comply with "getWindow()->getAspectMode()".
        2) Call "adjustSizeToContent_wordWrap_notKeepingAspectRatio", which
            starts by adjusting the height if necessary, and then adjusts the
            width by try and error (using bisection). 
        3) Call "adjustSizeToContent_wordWrap_keepingAspectRatio", which
            works by try and error (using bisection): it tries various
            combinations of width and height, but only those in which the
            aspect ratio matches the desired one (which is
            "getWindow()->getAspectRatio()"). In practice the attempts are
            limited to those heights in which exactly a whole number of lines
            fits, because there's no point to try other heights.
    */
    void adjustSizeToContent() override;

    /*!
    \brief
        Return whether the size is automatically adjusted to the content
        (the text), in a way in which both the width and the height are
        adjusted together, keeping the desired aspect ratio.

        When this is true, "adjustSizeToContent" works by try and error
        (using bisection): it tries various combinations of width and
        height, but only those in which the aspect ratio matches the desired
        one (which is "getWindow()->getAspectRatio()"). In practice the
        attempts are limited to those heights in which exactly a whole
        number of lines fits, because there's no point to try other heights.
    */
    bool isSizeAdjustedToContentKeepingAspectRatio() const;

    /*!
    \brief
        Return whether setting the widget size to "window_size" would make
        the whole widget content (the text) visible, without the need for
        scrollbars, and without the need to split a word between 2 or more
        lines.
    */
    bool contentFitsForSpecifiedWindowSize(const Sizef& window_size) const override;

    /*!
    \brief
        Return whether the whole window content (the text) is visible,
        without the need for scrollbars, and without the need to split a
        word between 2 or more lines.
    */
    bool contentFits() const override;

    /*!
    \brief
        Invalidate string formatting, scrollbars visibility and actual
        vertical and horizontal text formatting.

        This sets a "dirty" flag which causes "updateFormatting" to actually
        perform an update.

    \see updateFormatting
    */
    void invalidateFormatting();

protected:

    /*!
    \brief
        Update string formatting, scrollbars visibility and actual vertical
        and horizontal text formatting.

        This only happens if a "dirty" flag is set. The flag is set by
        "invalidateFormatting".

    \see invalidateFormatting
    */
    void updateFormatting() const;

    // overridden from FalagardStatic base class
    void onLookNFeelAssigned() override;
    void onLookNFeelUnassigned() override;

    //! \brief Update string formatting and horizontal and vertical scrollbars visibility.
    void configureScrollbars() const;

    Scrollbar* getVertScrollbar() const;
    Scrollbar* getHorzScrollbar() const;
    Rectf getTextRenderArea() const;
    const ComponentArea& getTextComponentArea() const;

    // overridden event handlers
    bool onTextChanged(const EventArgs& e);
    bool onSized(const EventArgs& e);
    bool onFontChanged(const EventArgs& e);
    bool onScroll(const EventArgs& e);
    bool onIsSizeAdjustedToContentChanged(const EventArgs&);

    // event subscribers
    bool handleScrollbarChange(const EventArgs& e);


    mutable RenderedText d_renderedText;
    std::vector<Event::ScopedConnection> d_connections;
    ColourRect d_textCols = ColourRect(0xFFFFFFFF);       //!< Colours used when rendering the text.

    /*
    \brief
        If the "AdjustHeightToContent" property of the window is "true",
        then this is the number of text lines to show when adjusting the
        height to the content.

        See "getNumOfTextLinesToShow" for more details.

    \see getNumOfTextLinesToShow
    */
    NumOfTextLinesToShow d_numOfTextLinesToShow;

    //! Horizontal formatting to be applied to the text.
    HorizontalTextFormatting d_horzFormatting = HorizontalTextFormatting::LeftAligned;

    /*!
    \brief
        The actual horizontal text formatting used, which may be different
        from the horizontal text formatting set with
        "setHorizontalFormatting".

        See the documentation for "getActualHorizontalFormatting" for more
        details.

    \see getActualHorizontalFormatting
    \see d_actualVertFormatting
    */
    mutable HorizontalTextFormatting d_actualHorzFormatting = HorizontalTextFormatting::LeftAligned;

    //! Vertical formatting to be applied to the text.
    VerticalTextFormatting d_vertFormatting = VerticalTextFormatting::CentreAligned;

    /*!
    \brief
        The actual vertical text formatting used, which may be different
        from the vertical text formatting set with "setVerticalFormatting".

        See the documentation for "getActualVerticalFormatting" for more
        details.

    \see getActualVerticalFormatting
    \see d_actualHorzFormatting
    */
    mutable VerticalTextFormatting d_actualVertFormatting = VerticalTextFormatting::CentreAligned;

    bool d_enableVertScrollbar = false; //!< true if vertical scroll bar is enabled.
    bool d_enableHorzScrollbar = false; //!< true if horizontal scroll bar is enabled.
    bool d_wordWrap = false;

    //! True when string formatting, scrollbars visibility and actual text formatting are up to date.
    mutable bool d_formatValid = false;

private:

    // For property access, use getTextExtent in a client code
    float getHorizontalTextExtent() const { return getTextExtent().d_width; }
    float getVerticalTextExtent() const { return getTextExtent().d_height; }

    Scrollbar* getVertScrollbarWithoutUpdate() const;
    Scrollbar* getHorzScrollbarWithoutUpdate() const;
    Rectf getTextRenderAreaWithoutUpdate() const;
    const ComponentArea& getTextComponentAreaWithoutUpdate() const;
    const Sizef& getTextExtentWithoutUpdate() const;
    void adjustSizeToContent_wordWrap_keepingAspectRatio(const Sizef& contentMaxSize,
        USize& size_func, float window_max_width, float epsilon);
    void adjustSizeToContent_wordWrap_notKeepingAspectRatio(USize& size_func, float content_max_width,
        float window_max_width, float epsilon);
    void adjustSizeToContent_direct();
};

/*!
\brief
    A helper class to handle conversions to/from string of the property
    "FalagardStaticText::NumOfTextLinesToShow".

    The reason for this is the following: That property can get either a
    non-negative floating point value, or "Auto". So the conversion
    functions to/form string know to handle both cases.

\see NumOfTextLinesToShow
*/
template<>
class PropertyHelper<FalagardStaticText::NumOfTextLinesToShow>
{
public:

    typedef FalagardStaticText::NumOfTextLinesToShow return_type;
    typedef return_type safe_method_return_type;
    typedef return_type pass_type;
    typedef String string_return_type;

    static const String s_autoString;

    static const String& getDataTypeName()
    {
        static const String type("NumOfTextLinesToShow");
        return type;
    }

    static inline return_type fromString(const String& str)
    {
        if (str == s_autoString)
        {
            return FalagardStaticText::NumOfTextLinesToShow();
        }

        float val = 0.0f;
        std::stringstream sstream;
        sstream << str;
        sstream >> val;
        return FalagardStaticText::NumOfTextLinesToShow(val);
    }

    static inline string_return_type toString(pass_type val)
    {
        if (val.isAuto())
        {
            return s_autoString;
        }

        std::stringstream sstream;
        sstream << val.get();
        return String(sstream.str());
    }
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif
