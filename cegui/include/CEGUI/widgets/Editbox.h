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

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

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

    virtual float getTextOffsetX() const override { return d_textOffset.x; }
    virtual float getTextOffsetY() const override { return d_textOffset.y; }
    virtual void setTextOffsetX(float value) override;
    virtual void setTextOffsetY(float /*value*/) override { /* Auto-calculated */ }

    virtual void ensureCaretIsVisible() override;

protected:

    bool insertString(String&& strToInsert) override;
    void updateFormatting() override;
    void processKeyDownEvent(KeyEventArgs& e) override;

    /*!
    \brief
        Handler called when the user accepts the edit box text by pressing
        Return, Enter, or Tab.
    */
    virtual void onTextAcceptedEvent(WindowEventArgs& e);

    glm::vec2 d_textOffset = glm::vec2(0.f, 0.f); //<! Offset of the text graphics inside a client rect
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
