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
#ifndef _CEGUIDirect3D11RenderTarget_h_
#define _CEGUIDirect3D11RenderTarget_h_

#include "../../RenderTarget.h"
#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "../../Rect.h"


#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//! Implementation of an ntermediate RenderTarget for the Direct3D 10 API
template <typename T = RenderTarget>
class D3D11_GUIRENDERER_API Direct3D11RenderTarget : public T
{
public:
    //! Constructor
    Direct3D11RenderTarget(Direct3D11Renderer& owner);

    // implement parts of RenderTarget interface
    void activate();
    void unprojectPoint(const GeometryBuffer& buff,
                        const glm::vec2& p_in,
                        glm::vec2& p_out) const;
    virtual Renderer& getOwner();

protected:
    //! helper that initialises the cached matrix
    void updateMatrix() const;
    //! helper to initialise the viewport \a vp for this target.
    void setupViewport(D3D11_VIEWPORT& vp) const;

    //! Renderer that created and owns the render target.
    Direct3D11Renderer& d_owner;

    //! The D3D Device
    ID3D11Device& d_device;
    //! The D3D DeviceContext
    ID3D11DeviceContext& d_deviceContext;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
