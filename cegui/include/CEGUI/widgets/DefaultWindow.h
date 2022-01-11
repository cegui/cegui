/***********************************************************************
    created:    5/6/2004
    author:     Paul D Turner

    purpose:    Interface to a default window
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
#ifndef _CEGUIDefaultWindow_h_
#define _CEGUIDefaultWindow_h_

#include "../Window.h"

namespace CEGUI
{
/*!
\brief
    Window class intended to be used as a simple, generic Window.
    It has no explicit graphics but may be used as a base for Falagard windows.
*/
class CEGUIEXPORT DefaultWindow : public Window
{
public:

    static const String WidgetTypeName; //!< The unique typename of this widget

    DefaultWindow(const String& type, const String& name);

    // Can't focus something that doesn't have an explicit visual appearance
    bool canFocus() const override { return !!d_windowRenderer; }

protected:

    void drawSelf(const RenderingContext& ctx, std::uint32_t drawModeMask) override;
};

}

#endif
