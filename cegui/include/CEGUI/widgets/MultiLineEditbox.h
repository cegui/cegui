/***********************************************************************
    created:    30/6/2004
    author:        Paul D Turner

    purpose:    Interface to the Multi-lien edit box base class.
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIMultiLineEditbox_h_
#define _CEGUIMultiLineEditbox_h_

#include "CEGUI/widgets/EditboxBase.h"
#include "CEGUI/WindowRenderer.h"

namespace CEGUI
{

//! \brief Base class for multi-line edit box window renderer objects.
class CEGUIEXPORT MultiLineEditboxWindowRenderer : public WindowRenderer
{
public:

    MultiLineEditboxWindowRenderer(const String& name);

    //! Editbox text parsing is forcefully disabled
    virtual bool isTextParsingEnabled() const override { return false; }

    /*!
    \brief
        Return a Rect object describing, in un-clipped pixels, the window relative area
        that the text should be rendered in to.

    \return
        Rect object describing the area of the Window to be used for rendering text.
    */
    virtual Rectf getTextRenderArea() const = 0;
};

//! \brief Base class for the multi-line edit box widget.
class CEGUIEXPORT MultiLineEditbox : public EditboxBase
{
public:

    static const String EventNamespace; //!< Namespace for global events
    static const String WidgetTypeName; //!< Window factory name
    static const String VertScrollbarName; //!< Widget name for the vertical scrollbar component.
    static const String HorzScrollbarName; //!< Widget name for the horizontal scrollbar component.

    MultiLineEditbox(const String& type, const String& name);
    virtual ~MultiLineEditbox() override;

    /*!
    \brief
        Return whether the vertical scroll bar is always shown.

    \return
        - true if the scroll bar will always be shown even if it is not required.
        - false if the scroll bar will only be shown when it is required.
    */
    bool isVertScrollbarAlwaysShown() const { return d_forceVertScroll; }
    bool isHorzScrollbarAlwaysShown() const { return d_forceHorzScroll; }

    /*!
    \brief
        Return a pointer to the vertical scrollbar component widget for this
        MultiLineEditbox.

    \return
        Pointer to a Scrollbar object.

    \exception UnknownObjectException
        Thrown if the vertical Scrollbar component does not exist.
    */
    Scrollbar* getVertScrollbar() const;

    /*!
    \brief
        Return a pointer to the horizontal scrollbar component widget for this
        MultiLineEditbox.

    \return
        Pointer to a Scrollbar object.

    \exception UnknownObjectException
        Thrown if the horizontal Scrollbar component does not exist.
    */
    Scrollbar* getHorzScrollbar() const;

    /*!
    \brief
        Initialise the Window based object ready for use.

    \note
        This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.
    */
    void initialiseComponents() override;

    virtual void ensureCaretIsVisible() override;

    /*!
    \brief
        Set whether the vertical scroll bar should always be shown.

    \param setting
        true if the vertical scroll bar should be shown even when it is not required.  false if the vertical
        scroll bar should only be shown when it is required.
    */
    void setShowVertScrollbar(bool setting);

    virtual void setTextFormatting(HorizontalTextFormatting format) override;

    //! Enable or disable word wrapping.
    void setWordWrapEnabled(bool wrap);

    /*!
    \brief
        Return whether word-wrapping is turned on, which means words are
        wrapped to multiple lines as needed.
    */
    bool isWordWrapEnabled() const { return d_wordWrap; }

protected:

    void updateFormatting() override;
    size_t getTextIndexFromPosition(const glm::vec2& pt) override;
    bool processSemanticInputEvent(const SemanticEventArgs& e) override;
    void onScroll(CursorInputEventArgs& e) override;

    /*!
    \brief
        Processing to move caret up a line.

    \param select
        when true, a line up will be also selected
    */
    void handleLineUp(bool select);

    /*!
    \brief
        Processing to move caret down a line.

    \param select
        when true, a line down will be also selected
    */
    void handleLineDown(bool select);

    /*!
    \brief
        Processing to move caret one page up

    \param select
        when true, a page up will be also selected
    */
    void handlePageUp(bool select);

    /*!
    \brief
        Processing to move caret one page down

    \param select
        when true, a page down will be also selected
    */
    void handlePageDown(bool select);

    bool handleScrollChange(const EventArgs& args);
    bool validateWindowRenderer(const WindowRenderer* renderer) const override;

    bool d_forceVertScroll = false; //!< true if vertical scrollbar should always be displayed
    bool d_forceHorzScroll = false; //!< true if horizontal scrollbar should always be displayed
    bool d_wordWrap = true;

private:

    void addMultiLineEditboxProperties();
};

}

#endif
