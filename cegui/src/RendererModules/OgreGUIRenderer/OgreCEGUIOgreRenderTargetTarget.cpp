/***********************************************************************
    filename:   OgreCEGUIOgreRenderTargetTarget.cpp
    created:    Sat Mar 8 2008
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "OgreCEGUIOgreRenderTargetTarget.h"
#include "OgreCEGUIRenderQueueListener.h"
#include <CEGUI/CEGUIExceptions.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OgreRenderTargetTarget::OgreRenderTargetTarget(Ogre::RenderTarget* target)
{
    // default to first viewport on the auto-created window.
    if (!target)
        target = d_ogreRoot->getAutoCreatedWindow();

    // always default to fist vp for now, will probably do something else later
    setViewport(target->getViewport(0));
    initialiseTargetArea();
}

//----------------------------------------------------------------------------//
OgreRenderTargetTarget::~OgreRenderTargetTarget()
{
}

//----------------------------------------------------------------------------//
void OgreRenderTargetTarget::queueToTarget(const Rect& src, const Rect& dst,
    float z, const ColourRect& cols, const QuadSplitMode quad_split_mode,
    RenderTarget& target)
{
    throw InvalidRequestException("Function is currently unimplemented!");
}

//----------------------------------------------------------------------------//
void OgreRenderTargetTarget::renderToTarget(const Rect& src, const Rect& dst,
    float z, const ColourRect& cols, const QuadSplitMode quad_split_mode,
    RenderTarget& target)
{
    throw InvalidRequestException("Function is currently unimplemented!");
}

//----------------------------------------------------------------------------//
void OgreRenderTargetTarget::setArea(const Rect& area)
{
    // area is gleaned from Ogre::Viewport automagically.  So this does nothing.
}

//----------------------------------------------------------------------------//
void OgreRenderTargetTarget::declareRenderSize(const Size& sz)
{
    // Nothing to do here for now.
}

//----------------------------------------------------------------------------//
bool OgreRenderTargetTarget::isImageryCache() const
{
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
