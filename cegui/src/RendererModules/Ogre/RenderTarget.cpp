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
#include "CEGUI/RendererModules/Ogre/RenderTarget.h"
#ifdef CEGUI_OGRE_NEXT
#include "CEGUI/RendererModules/Ogre/GeometryBuffer.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RenderQueue.h"

#include <OgreRenderSystem.h>
#include <OgreTextureGpu.h>
#include <OgreCamera.h>
#include <OgreViewport.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{
	
OgreRenderTarget::OgreRenderTarget(OgreRenderer& owner, Ogre::RenderSystem& rs, bool clearTextureBeforeRendering) :
    d_owner(owner),
    d_renderSystem(rs),
	d_textureGpuTarget(0),
	d_renderPassDescForFirstDraw(0),
	d_renderPassDescForAddionalDraw(0),
	d_clearTextureBeforeRendering(clearTextureBeforeRendering),
	d_viewportSize(0, 0, 1, 1)
{
}

OgreRenderTarget::~OgreRenderTarget()
{
	if(d_renderPassDescForFirstDraw)
		d_renderSystem.destroyRenderPassDescriptor(d_renderPassDescForFirstDraw);
		
	if(d_renderPassDescForAddionalDraw)
		d_renderSystem.destroyRenderPassDescriptor(d_renderPassDescForAddionalDraw);
}

void OgreRenderTarget::updateRenderPassDescriptor()
{
	Ogre::TextureGpu* textureTarget = d_textureGpuTarget;

	if(!textureTarget)
	{
		throw std::runtime_error("Error no texture ready (OgreRenderTarget::updateRenderPassDescriptor)");
	}

	if(!d_renderPassDescForFirstDraw)
	{
		d_renderPassDescForFirstDraw = d_renderSystem.createRenderPassDescriptor();
		d_renderPassDescForFirstDraw->mColour[0].texture = textureTarget;
		
		if(d_clearTextureBeforeRendering)
			d_renderPassDescForFirstDraw->mColour[0].loadAction = Ogre::LoadAction::Clear;
		else
			d_renderPassDescForFirstDraw->mColour[0].loadAction = Ogre::LoadAction::DontCare;
		
		d_renderPassDescForFirstDraw->mColour[0].clearColour = Ogre::ColourValue(0.0f, 0.0f, 0.0f, 0.0f);
		d_renderPassDescForFirstDraw->mColour[0].storeAction = Ogre::StoreAction::StoreOrResolve;
		//Note that resolveTexture should be nullptr if texture isn't msaa.
		//Also if texture->hasMsaaExplicitResolves() == false, then resolveTexture = texture
		//is allowed, as the texture will have an internal texture to be used for resolving
		d_renderPassDescForFirstDraw->mColour[0].resolveTexture = 0;
		d_renderPassDescForFirstDraw->mDepth.texture = d_renderSystem.getDepthBufferFor(textureTarget, textureTarget->getDepthBufferPoolId(), textureTarget->getPreferDepthTexture(), textureTarget->getDesiredDepthBufferFormat());
		d_renderPassDescForFirstDraw->mDepth.loadAction = Ogre::LoadAction::Clear;
		d_renderPassDescForFirstDraw->mDepth.storeAction = Ogre::StoreAction::DontCare;
		d_renderPassDescForFirstDraw->mStencil.texture = nullptr;
		d_renderPassDescForFirstDraw->mStencil.loadAction = Ogre::LoadAction::Clear;
		d_renderPassDescForFirstDraw->mStencil.storeAction = Ogre::StoreAction::DontCare;
		d_renderPassDescForFirstDraw->entriesModified(Ogre::RenderPassDescriptor::All);
	}

	if(!d_renderPassDescForAddionalDraw)
	{
		d_renderPassDescForAddionalDraw = d_renderSystem.createRenderPassDescriptor();
		d_renderPassDescForAddionalDraw->mColour[0].texture = textureTarget;
		d_renderPassDescForAddionalDraw->mColour[0].loadAction = Ogre::LoadAction::DontCare;
		d_renderPassDescForAddionalDraw->mColour[0].clearColour = Ogre::ColourValue(0.5f, 1.0f, 0.0f); //does not matter
		d_renderPassDescForAddionalDraw->mColour[0].storeAction = Ogre::StoreAction::StoreOrResolve;
		//Note that resolveTexture should be nullptr if texture isn't msaa.
		//Also if texture->hasMsaaExplicitResolves() == false, then resolveTexture = texture
		//is allowed, as the texture will have an internal texture to be used for resolving
		d_renderPassDescForAddionalDraw->mColour[0].resolveTexture = 0;
		d_renderPassDescForAddionalDraw->mDepth.texture = d_renderSystem.getDepthBufferFor(textureTarget, textureTarget->getDepthBufferPoolId(), textureTarget->getPreferDepthTexture(), textureTarget->getDesiredDepthBufferFormat());
		d_renderPassDescForAddionalDraw->mDepth.loadAction = Ogre::LoadAction::Clear;
		d_renderPassDescForAddionalDraw->mDepth.storeAction = Ogre::StoreAction::DontCare;
		d_renderPassDescForAddionalDraw->mStencil.texture = nullptr;
		d_renderPassDescForAddionalDraw->mStencil.loadAction = Ogre::LoadAction::Clear;
		d_renderPassDescForAddionalDraw->mStencil.storeAction = Ogre::StoreAction::DontCare;
		d_renderPassDescForAddionalDraw->entriesModified(Ogre::RenderPassDescriptor::All);
	}
}


void OgreRenderTarget::setOgreViewportDimensions(const Rectf& area)
{
	d_viewportSize = Ogre::Vector4(
			area.left() / d_textureGpuTarget->getWidth(),
			area.top() / d_textureGpuTarget->getHeight(),
			area.getWidth() / d_textureGpuTarget->getWidth(),
			area.getHeight() / d_textureGpuTarget->getHeight()
		);
}

void OgreRenderTarget::activate()
{
    if (!RenderTarget::d_matrixValid)
        updateMatrix();

    d_owner.setViewProjectionMatrix(RenderTarget::d_matrix);
	updateRenderPassDescriptor();
	d_owner.initialiseRenderStateSettings(d_textureGpuTarget, getRenderPassDescriptorForAddionalDraw());

    RenderTarget::activate();
}

void OgreRenderTarget::draw(const GeometryBuffer& buffer, std::uint32_t drawModeMask)
{
	GeometryBuffer& bufferNoConst = const_cast<GeometryBuffer&>(buffer);
	OgreGeometryBuffer& ogreBuffer = static_cast<OgreGeometryBuffer&>(bufferNoConst);
	ogreBuffer.setCurrentRenderTarget(this);
	
	ogreBuffer.setFirstDrawOfFrame(false);
	buffer.draw(drawModeMask);

#if 0
	ogreBuffer.setCurrentRenderTarget(0);
#endif
}

void OgreRenderTarget::draw(const RenderQueue& queue, std::uint32_t drawModeMask)
{
	RenderQueue& queueNoConst = const_cast<RenderQueue&>(queue);
	RenderQueue::BufferList& buffers = queueNoConst.getBuffers();
	
	bool isFirst = true;
	for(RenderQueue::BufferList::iterator iter = buffers.begin(); iter != buffers.end(); ++iter)
	{
		OgreGeometryBuffer* ogreBuffer = static_cast<OgreGeometryBuffer*>(*iter);
		ogreBuffer->setCurrentRenderTarget(this);
		ogreBuffer->setFirstDrawOfFrame(isFirst);
		isFirst = false;
	}

	queue.draw(drawModeMask);

#if 0
	for(RenderQueue::BufferList::iterator iter = buffers.begin(); iter != buffers.end(); ++iter)
	{
		OgreGeometryBuffer* ogreBuffer = static_cast<OgreGeometryBuffer*>(*iter);
		ogreBuffer->setCurrentRenderTarget(0);
	}
#endif
}

void OgreRenderTarget::unprojectPoint(const GeometryBuffer& buff,
                                         const glm::vec2& p_in,
                                         glm::vec2& p_out) const
{
    if (!RenderTarget::d_matrixValid)
        updateMatrix();

    const OgreGeometryBuffer& gb = static_cast<const OgreGeometryBuffer&>(buff);

    const Ogre::Real midx = RenderTarget::d_area.getWidth() * 0.5f;
    const Ogre::Real midy = RenderTarget::d_area.getHeight() * 0.5f;

    // viewport matrix
    const Ogre::Matrix4 vpmat(
        midx,    0,    0,    RenderTarget::d_area.left() + midx,
         0,    -midy,  0,    RenderTarget::d_area.top() + midy,
         0,      0,    1,    0,
         0,      0,    0,    1
    );

    // matrices used for projecting and unprojecting points

    const Ogre::Matrix4 proj(OgreRenderer::glmToOgreMatrix(gb.getModelMatrix() * RenderTarget::d_matrix) * vpmat);
    const Ogre::Matrix4 unproj(proj.inverse());

    Ogre::Vector3 in;

    // unproject the ends of the ray
    in.x = midx;
    in.y = midy;
    in.z = -RenderTarget::d_viewDistance;
    const Ogre::Vector3 r1(unproj * in);
    in.x = p_in.x;
    in.y = p_in.y;
    in.z = 0;
    // calculate vector of picking ray
    const Ogre::Vector3 rv(r1 - unproj * in);

    // project points to orientate them with GeometryBuffer plane
    in.x = 0.0;
    in.y = 0.0;
    const Ogre::Vector3 p1(proj * in);
    in.x = 1.0;
    in.y = 0.0;
    const Ogre::Vector3 p2(proj * in);
    in.x = 0.0;
    in.y = 1.0;
    const Ogre::Vector3 p3(proj * in);

    // calculate the plane normal
    const Ogre::Vector3 pn((p2 - p1).crossProduct(p3 - p1));
    // calculate distance from origin
    const Ogre::Real plen = pn.length();
    const Ogre::Real dist = -(p1.x * (pn.x / plen) +
                              p1.y * (pn.y / plen) +
                              p1.z * (pn.z / plen));

    // calculate intersection of ray and plane
    const Ogre::Real pn_dot_rv = pn.dotProduct(rv);
    const Ogre::Real tmp = pn_dot_rv != 0.0 ?
                            (pn.dotProduct(r1) + dist) / pn_dot_rv :
                            0.0f;

    p_out.x = static_cast<float>(r1.x - rv.x * tmp);
    p_out.y = static_cast<float>(r1.y - rv.y * tmp);
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
#else	//CEGUI_OGRE_NEXT
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

		if(d_viewport)
			updateOgreViewportDimensions(d_viewport->getTarget());

		d_viewportValid = false;
	}


	void OgreRenderTarget::updateOgreViewportDimensions(
		const Ogre::RenderTarget* const rt)
	{
		if(rt)
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
		if(!RenderTarget::d_matrixValid)
			updateMatrix();

		if(!d_viewportValid)
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

	void OgreRenderTarget::unprojectPoint(const GeometryBuffer& buff,
		const glm::vec2& p_in,
		glm::vec2& p_out) const
	{
		if(!RenderTarget::d_matrixValid)
			updateMatrix();

		const OgreGeometryBuffer& gb = static_cast<const OgreGeometryBuffer&>(buff);

		const Ogre::Real midx = RenderTarget::d_area.getWidth() * 0.5f;
		const Ogre::Real midy = RenderTarget::d_area.getHeight() * 0.5f;

		// viewport matrix
		const Ogre::Matrix4 vpmat(
			midx, 0, 0, RenderTarget::d_area.left() + midx,
			0, -midy, 0, RenderTarget::d_area.top() + midy,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		// matrices used for projecting and unprojecting points

		const Ogre::Matrix4 proj(OgreRenderer::glmToOgreMatrix(gb.getModelMatrix() * RenderTarget::d_matrix) * vpmat);
		const Ogre::Matrix4 unproj(proj.inverse());

		Ogre::Vector3 in;

		// unproject the ends of the ray
		in.x = midx;
		in.y = midy;
		in.z = -RenderTarget::d_viewDistance;
		const Ogre::Vector3 r1(unproj * in);
		in.x = p_in.x;
		in.y = p_in.y;
		in.z = 0;
		// calculate vector of picking ray
		const Ogre::Vector3 rv(r1 - unproj * in);

		// project points to orientate them with GeometryBuffer plane
		in.x = 0.0;
		in.y = 0.0;
		const Ogre::Vector3 p1(proj * in);
		in.x = 1.0;
		in.y = 0.0;
		const Ogre::Vector3 p2(proj * in);
		in.x = 0.0;
		in.y = 1.0;
		const Ogre::Vector3 p3(proj * in);

		// calculate the plane normal
		const Ogre::Vector3 pn((p2 - p1).crossProduct(p3 - p1));
		// calculate distance from origin
		const Ogre::Real plen = pn.length();
		const Ogre::Real dist = -(p1.x * (pn.x / plen) +
			p1.y * (pn.y / plen) +
			p1.z * (pn.z / plen));

		// calculate intersection of ray and plane
		const Ogre::Real pn_dot_rv = pn.dotProduct(rv);
		const Ogre::Real tmp = pn_dot_rv != 0.0 ?
			(pn.dotProduct(r1) + dist) / pn_dot_rv :
			0.0f;

		p_out.x = static_cast<float>(r1.x - rv.x * tmp);
		p_out.y = static_cast<float>(r1.y - rv.y * tmp);
	}


	void OgreRenderTarget::updateMatrix() const
	{
		if(d_owner.usesOpenGL())
			RenderTarget::updateMatrix(RenderTarget::createViewProjMatrixForOpenGL());
		else if(d_owner.usesDirect3D())
			RenderTarget::updateMatrix(RenderTarget::createViewProjMatrixForDirect3D());
		else
			throw RendererException("An unsupported RenderSystem is being used by Ogre. Please contact the CEGUI team.");
	}


	void OgreRenderTarget::updateViewport()
	{
		if(!d_viewport)
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
#endif	//CEGUI_OGRE_NEXT
