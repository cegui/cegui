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
#ifndef _CEGUIOgreRenderTarget_h_
#define _CEGUIOgreRenderTarget_h_

#include "CEGUI/RendererModules/Ogre/Renderer.h"

#ifdef CEGUI_OGRE_NEXT
#include "CEGUI/RenderTarget.h"
#include "CEGUI/Rectf.h"
#include <OgreMatrix4.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//! Intermediate RenderTarget implementing common parts for Ogre engine.
class OGRE_GUIRENDERER_API OgreRenderTarget : virtual public RenderTarget
{
public:
    //! Constructor
    OgreRenderTarget(OgreRenderer& owner, Ogre::RenderSystem& rs, bool clearTextureBeforeRendering);

    //! Destructor
    virtual ~OgreRenderTarget();
    
    // implement parts of CEGUI::RenderTarget interface
    void draw(const GeometryBuffer& buffer, std::uint32_t drawModeMask = DrawModeMaskAll);
    void draw(const RenderQueue& queue, std::uint32_t drawModeMask = DrawModeMaskAll);

    void activate();
    void unprojectPoint(const GeometryBuffer& buff, const glm::vec2& p_in, glm::vec2& p_out) const;

    virtual void setArea(const Rectf& area);
    // implementing the virtual function with a covariant return type
    virtual OgreRenderer& getOwner();

	// setup the RenderPassDescriptors to be used
	void updateRenderPassDescriptor();
	Ogre::RenderPassDescriptor* getRenderPassDescriptorForFirstDraw() const { return d_renderPassDescForFirstDraw; }
	Ogre::RenderPassDescriptor* getRenderPassDescriptorForAddionalDraw() const { return d_renderPassDescForAddionalDraw; }
	
	//! returns the TextureGpu
	Ogre::TextureGpu* getTextureGpuTarget() const { return d_textureGpuTarget; }

	//! return the viewport size to be used
	Ogre::Vector4 getViewportSize() const {	return d_viewportSize; }

protected:
    //! helper that initialises the cached matrix
    void updateMatrix() const;
    
	//! sets the viewport are to be used
	void setOgreViewportDimensions(const Rectf& area);

    //! OgreRenderer object that owns this RenderTarget
    OgreRenderer& d_owner;
    //! Ogre RendererSystem used to affect the rendering process
    Ogre::RenderSystem& d_renderSystem;
    //! Ogre render target that we are effectively wrapping
    Ogre::TextureGpu* d_textureGpuTarget;

	//! Ogre RenderPassDescriptor we use for drawing
	Ogre::RenderPassDescriptor* d_renderPassDescForFirstDraw;
	Ogre::RenderPassDescriptor* d_renderPassDescForAddionalDraw;
	//! secifies if the texture shall be cleared on frist draw of the GeometryBuffer in a queue. I.e. RenderTargets shall clear, but Windows where we draw on top and are cleard each frame shall not be cleared
	bool d_clearTextureBeforeRendering;

	//! holds set Ogre viewport dimensions (in Ogre Format) which is send to the GemoetryBuffer
	Ogre::Vector4 d_viewportSize;
};

} // End of  CEGUI namespace section
#else	//CEGUI_OGRE_NEXT
#include "CEGUI/RenderTarget.h"
#include "CEGUI/RendererModules/Ogre/Renderer.h"
#include "CEGUI/Rectf.h"
#include <OgreMatrix4.h>

 // Start of CEGUI namespace section
namespace CEGUI
{
	//! Intermediate RenderTarget implementing common parts for Ogre engine.
	class OGRE_GUIRENDERER_API OgreRenderTarget : virtual public RenderTarget
	{
	public:
		//! Constructor
		OgreRenderTarget(OgreRenderer& owner, Ogre::RenderSystem& rs);

		//! Destructor
		virtual ~OgreRenderTarget();

		/*!
		\brief
			Set the underlying viewport area directly - bypassing what the
			RenderTarget considers to be it's area - thus allowing the view port
			area used for rendering to be different to the area set for the target.

		\param area
			Rect object describing the area to use in pixels.

		\deprecated
			This function is deprecated and will be removed or changed considerably
			in future releases.
		*/
		void setOgreViewportDimensions(const Rectf& area);

		// implement parts of CEGUI::RenderTarget interface
		void draw(const GeometryBuffer& buffer, std::uint32_t drawModeMask = DrawModeMaskAll);
		void draw(const RenderQueue& queue, std::uint32_t drawModeMask = DrawModeMaskAll);

		void activate();
		void unprojectPoint(const GeometryBuffer& buff, const glm::vec2& p_in, glm::vec2& p_out) const;

		virtual void setArea(const Rectf& area);
		// implementing the virtual function with a covariant return type
		virtual OgreRenderer& getOwner();

	protected:
		//! helper that initialises the cached matrix
		void updateMatrix() const;
		//! helper that initialises the viewport
		void updateViewport();
		//! helper to update the actual Ogre viewport dimensions
		void updateOgreViewportDimensions(const Ogre::RenderTarget* const rt);

		//! OgreRenderer object that owns this RenderTarget
		OgreRenderer& d_owner;
		//! Ogre RendererSystem used to affect the rendering process
		Ogre::RenderSystem& d_renderSystem;
		//! Ogre render target that we are effectively wrapping
		Ogre::RenderTarget* d_renderTarget;

		//! Ogre viewport used for this target.
		Ogre::Viewport* d_viewport;
		//! holds set Ogre viewport dimensions
		Rectf d_ogreViewportDimensions;

		//! true when d_viewport is up to date and valid.
		//! \version Beginning from Ogre 2.0 this indicates whether the workspace is
		//! up to date
		bool d_viewportValid;
	};

} // End of  CEGUI namespace section
#endif	//CEGUI_OGRE_NEXT
#endif  // end of guard _CEGUIOgreRenderTarget_h_
