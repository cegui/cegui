/***********************************************************************
    created:    2022-01-30 (rewrite OgreRenderTarget for Ogre >= 2.2)
    author:     Robert Paciorek
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
#include "CEGUI/RenderTarget.h"
#include "CEGUI/TextureTarget.h"
#include "CEGUI/Rectf.h"
#include <OgreMatrix4.h>

namespace Ogre {
    class RenderPassDescriptor;
}

namespace CEGUI
{
class OGRE_GUIRENDERER_API OgreRenderTextureTarget : virtual public RenderTarget, public TextureTarget
{
public:
    //! Destructor
    virtual ~OgreRenderTextureTarget();

    // implement parts of CEGUI::RenderTarget interface:

    void activate();
    void deactivate();

    void draw(const GeometryBuffer& buffer, std::uint32_t drawModeMask = DrawModeMaskAll);
    void draw(const RenderQueue& queue, std::uint32_t drawModeMask = DrawModeMaskAll);

    void setArea(const Rectf& area);
    bool isImageryCache() const;

    void updateMatrix() const override;

    OgreRenderer& getOwner();

    // implement CEGUI::TextureTarget interface:

    void clear();
    Texture& getTexture() const;
    void declareRenderSize(const Sizef& sz);

protected:
    static std::uint32_t s_textureNumber;

    OgreTexture*                d_texture;
    OgreRenderer&               d_owner;

    Ogre::RenderSystem*         d_renderSystem;
    Ogre::RenderPassDescriptor* d_renderPassDescriptor;
    Ogre::Vector4               d_viewportSize;
    bool                        d_isImageryCache;
    char                        d_needClear;

    friend class OgreRenderer;

    OgreRenderTextureTarget(
        OgreRenderer& owner, Ogre::RenderSystem* renderSystem, Ogre::TextureGpu* texture = 0,
        bool addStencilBuffer = false, bool takeOwnership = false, bool isImageryCache = true
    );

    void setOgreTexture(Ogre::TextureGpu*, bool takeOwnership = false);
    void setArea(float width, float height);

    void createRenderPassDescriptor();
    void manageClear();
    String generateTextureName();
};

} // End of  CEGUI namespace section
#endif  // end of guard _CEGUIOgreRenderTarget_h_
