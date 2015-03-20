/***********************************************************************
    created:    Tue Feb 17 2009
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
#include "CEGUI/RendererModules/Ogre/WindowTarget.h"

#include <OgreRenderTarget.h>
#include <OgreViewport.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OgreWindowTarget::OgreWindowTarget(OgreRenderer& owner,
                                   Ogre::RenderSystem& rs,
                                   Ogre::RenderTarget& target) :
    OgreRenderTarget<>(owner, rs)
{
    initRenderTarget(target);
}

//----------------------------------------------------------------------------//
OgreWindowTarget::~OgreWindowTarget()
{
}

//----------------------------------------------------------------------------//
void OgreWindowTarget::setOgreRenderTarget(Ogre::RenderTarget& target)
{
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    // Setting this should properly change everything
    d_renderTargetUpdated = true;
#else
    // cleanup viewport since it's RT dependent.
    OGRE_DELETE d_viewport;
    d_viewport = 0;
    d_viewportValid = false;
#endif    


    initRenderTarget(target);
}

//----------------------------------------------------------------------------//
bool OgreWindowTarget::isImageryCache() const
{
    return false;
}

//----------------------------------------------------------------------------//
void OgreWindowTarget::initRenderTarget(Ogre::RenderTarget& target)
{
    d_renderTarget = &target;

    Rectf init_area(
        Vector2f(0, 0),
        Sizef(d_renderTarget->getWidth(), d_renderTarget->getHeight())
    );

    setArea(init_area);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

//----------------------------------------------------------------------------//
// Implementation of template base class
#include "./RenderTarget.inl"

