/***********************************************************************
    created:    13/4/2004
    author:     Paul D Turner

    purpose:    Interface to base class for Editbox widget
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
#ifndef _CEGUIEditbox_h_
#define _CEGUIEditbox_h_

#include "CEGUI/widgets/EditboxBase.h"
#include "CEGUI/WindowRenderer.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! Base class for the EditboxWindowRenderer class
class CEGUIEXPORT EditboxWindowRenderer : public WindowRenderer
{
public:

    EditboxWindowRenderer(const String& name);

    /*!
    \brief
        Return the text code point index that is rendered closest to screen
        position \a pt.

    \param pt
        Point object describing a position on the screen in pixels.

    \return
        Code point index into the text that is rendered closest to screen
        position \a pt.
    */
    virtual size_t getTextIndexFromPosition(const glm::vec2& pt) const = 0;

    //! Editbox text parsing is forcefully disabled
    virtual bool isTextParsingEnabled() const { return false; }
};

//----------------------------------------------------------------------------//

//! Class for an Editbox widget
class CEGUIEXPORT Editbox : public EditboxBase
{
public:

    static const String EventNamespace;
    static const String WidgetTypeName;

    /** Event fired when the user accepts the current text by pressing Return,
     * Enter, or Tab.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox in which the user has accepted
     * the current text.
     */
    static const String EventTextAccepted;

    Editbox(const String& type, const String& name);

    /*!
    \brief
        Sets the horizontal text formatting to be used from now onwards.

    \param format
        Specifies the formatting to use.  Currently can only be one of the
        following HorizontalTextFormatting values:
            - HorizontalTextFormatting::LeftAligned (default)
            - HorizontalTextFormatting::RightAligned
            - HorizontalTextFormatting::CentreAligned
    */
    virtual void setTextFormatting(HorizontalTextFormatting format) override;

    virtual void ensureCaretIsVisible() override;

protected:

    size_t getTextIndexFromPosition(const glm::vec2& pt) const override;
    bool processSemanticInputEvent(const SemanticEventArgs& e) override;
    bool validateWindowRenderer(const WindowRenderer* renderer) const override;

    /*!
    \brief
        Handler called when the user accepts the edit box text by pressing
        Return, Enter, or Tab.
    */
    virtual void onTextAcceptedEvent(WindowEventArgs& e);
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
