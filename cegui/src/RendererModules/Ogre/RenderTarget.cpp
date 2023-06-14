/***********************************************************************
    created:    Tue Feb 17 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/RendererModules/Ogre/Renderer.h"
#ifndef CEGUI_USE_OGRE_TEXTURE_GPU

#include "CEGUI/RendererModules/Ogre/RenderTarget.h"
#include "CEGUI/RendererModules/Ogre/GeometryBuffer.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RenderQueue.h"

#include <OgreRenderSystem.h>
#include <OgreRenderTarget.h>
#include <OgreCamera.h>
#include <OgreViewport.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{


OgreRenderTarget::OgreRenderTarget(OgreRenderer& owner,
                                      Ogre::RenderSystem& rs) :
    d_owner(owner),
    d_renderSystem(rs),
    d_renderTarget(0),
    d_viewport(0),
    d_ogreViewportDimensions(0, 0, 0, 0),
    d_viewportValid(false)
{
}


OgreRenderTarget::~OgreRenderTarget()
{
    delete d_viewport;
}


void OgreRenderTarget::setOgreViewportDimensions(const Rectf& area)
{
    d_ogreViewportDimensions = area;

    if (d_viewport)
        updateOgreViewportDimensions(d_viewport->getTarget());

    d_viewportValid = false;
}


void OgreRenderTarget::updateOgreViewportDimensions(
                                            const Ogre::RenderTarget* const rt)
{
    if (rt)
    {
        if(d_viewport)
            d_viewport->setDimensions(
            d_ogreViewportDimensions.left() / rt->getWidth(),
            d_ogreViewportDimensions.top() / rt->getHeight(),
            d_ogreViewportDimensions.getWidth() / rt->getWidth(),
            d_ogreViewportDimensions.getHeight() / rt->getHeight());
    }
}


void OgreRenderTarget::activate()
{
    if (!RenderTarget::d_matrixValid)
        updateMatrix();

    if (!d_viewportValid)
        updateViewport();

    d_renderSystem._setViewport(d_viewport);

    d_owner.setViewProjectionMatrix(RenderTarget::d_matrix);
#ifdef CEGUI_USE_OGRE_HLMS
    d_owner.initialiseRenderStateSettings(d_renderTarget);
#else
    d_owner.initialiseRenderStateSettings();
#endif

    RenderTarget::activate();
}

void OgreRenderTarget::draw(const GeometryBuffer& buffer, std::uint32_t drawModeMask)
{
    buffer.draw(drawModeMask);
}

void OgreRenderTarget::draw(const RenderQueue& queue, std::uint32_t drawModeMask)
{
    queue.draw(drawModeMask);
}

void OgreRenderTarget::updateMatrix() const
{
    if (d_owner.usesOpenGL())
        RenderTarget::updateMatrix( RenderTarget::createViewProjMatrixForOpenGL() );
    else if (d_owner.usesDirect3D())
        RenderTarget::updateMatrix( RenderTarget::createViewProjMatrixForDirect3D() );
    else
        throw RendererException("An unsupported RenderSystem is being used by Ogre. Please contact the CEGUI team.");
}


void OgreRenderTarget::updateViewport()
{
    if (!d_viewport)
    {
#ifdef CEGUI_USE_OGRE_COMPOSITOR2

        d_viewport = OGRE_NEW Ogre::Viewport(d_renderTarget, 0, 0, 1, 1);
#else
        d_viewport = OGRE_NEW Ogre::Viewport(0, d_renderTarget, 0, 0, 1, 1, 0);
#endif // CEGUI_USE_OGRE_COMPOSITOR2

        updateOgreViewportDimensions(d_renderTarget);
    }

    d_viewport->_updateDimensions();

    d_viewportValid = true;
}


OgreRenderer& OgreRenderTarget::getOwner()
{
    return d_owner;
}


void OgreRenderTarget::setArea(const Rectf& area)
{
    setOgreViewportDimensions(area);

    RenderTarget::setArea(area);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

#endif // CEGUI_USE_OGRE_TEXTURE_GPU
