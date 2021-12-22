/***********************************************************************
    created:    Mon Jan 12 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIRenderingContext_h_
#define _CEGUIRenderingContext_h_

#include "RenderingSurface.h" // FIXME: for RenderQueueID, otherwise could be forward declared

namespace CEGUI
{
/*!
\brief
    struct that holds some context relating to a RenderingSurface object.
*/
struct RenderingContext
{
    //! RenderingSurface to be used for drawing
    RenderingSurface* surface = nullptr;
    //! The Window object that owns the RenederingSurface (0 for default root)
    const Window* owner = nullptr;
    //! The offset of the owning window on the root RenderingSurface.
    glm::vec2 offset;
    //! The queue that rendering should be added to.
    RenderQueueID queue = RenderQueueID::Base;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIRenderingContext_h_
