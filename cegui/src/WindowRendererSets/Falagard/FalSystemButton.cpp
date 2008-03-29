/***********************************************************************
    filename:   FalSystemButton.cpp
    created:    Sat Jul 9 2005
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
#include "FalSystemButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardSystemButton::TypeName[] = "Falagard/SystemButton";

    FalagardSystemButton::FalagardSystemButton(const String& type) :
        FalagardButton(type)
    {
    }

    Rect FalagardSystemButton::getPixelRect(void) const
    {
        // clip to grand-parent as needed
        if (d_window->getParent() && d_window->getParent()->getParent() && d_window->isClippedByParent())
        {
            return d_window->getParent()->getParent()->getInnerRect().getIntersection(d_window->getUnclippedPixelRect());
        }
        // clip to screen if no grand-parent, or if clipping has been disabled for us.
        else
        {
            return System::getSingleton().getRenderer()->getRect().getIntersection(d_window->getUnclippedPixelRect());
        }
    }

} // End of  CEGUI namespace section
