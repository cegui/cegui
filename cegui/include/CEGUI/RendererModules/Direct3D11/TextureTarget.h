/***********************************************************************
    created:    Sun, 6th April 2014
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIDirect3D11TextureTarget_h_
#define _CEGUIDirect3D11TextureTarget_h_

#include "CEGUI/RendererModules/Direct3D11/RenderTarget.h"
#include "../../TextureTarget.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4250)
#   pragma warning(disable : 4251)
#endif

// d3d forward refs
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

// Start of CEGUI namespace section
namespace CEGUI
{
class Direct3D11Texture;

//! Direct3D11TextureTarget - allows rendering to Direct3D 10 textures.
class D3D11_GUIRENDERER_API Direct3D11TextureTarget : public Direct3D11RenderTarget, public TextureTarget
{
public:
    Direct3D11TextureTarget(Direct3D11Renderer& owner, bool addStencilBuffer);
    virtual ~Direct3D11TextureTarget();

    // overrides from the superclass
    void activate();
    void deactivate();
    // implementation of RenderTarget interface
    bool isImageryCache() const;
    // implementation of TextureTarget interface
    void clear();
    Texture& getTexture() const;
    void declareRenderSize(const Sizef& sz);

protected:
    //! default size of created texture objects
    static const float DEFAULT_SIZE;
    //! static data used for creating texture names
    static std::uint32_t s_textureNumber;
    //! helper to generate unique texture names
    static String generateTextureName();

    //! allocate and set up the texture used for rendering.
    void initialiseRenderTexture();
    //! clean up the texture used for rendering.
    void cleanupRenderTexture();
    //! resize the texture
    void resizeRenderTexture();
    //! switch to the texture surface & depth buffer
    void enableRenderTexture();
    //! switch back to previous surface
    void disableRenderTexture();

    //! The texture that we will render to
    ID3D11Texture2D* d_texture;
    //! render target view for d_texture
    ID3D11RenderTargetView* d_renderTargetView;
    //! we use this to wrap d_texture so it can be used by the core CEGUI lib.
    Direct3D11Texture* d_CEGUITexture;
    //! render target view that was bound before this target was activated
    ID3D11RenderTargetView* d_previousRenderTargetView;
    //! depth stencil view that was bound before this target was activated
    ID3D11DepthStencilView* d_previousDepthStencilView;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIDirect3D11TextureTarget_h_
