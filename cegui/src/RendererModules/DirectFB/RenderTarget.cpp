/***********************************************************************
    created:    Tue Mar 10 2009
    author:     Paul D Turner (parts based on code by Keith Mok)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/DirectFB/RenderTarget.h"
#include "CEGUI/RendererModules/DirectFB/Renderer.h"
#include "CEGUI/RendererModules/DirectFB/GeometryBuffer.h"
#include "CEGUI/RenderQueue.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
DirectFBRenderTarget::DirectFBRenderTarget(DirectFBRenderer& owner,
                                           IDirectFBSurface& target) :
    d_owner(owner),
    d_target(target),
    d_area(0, 0, 0, 0)
{
    int w, h;
    d_target.GetSize(&d_target, &w, &h);
    setArea(Rectf(d_area.getPosition(),
                 Sizef(static_cast<float>(w), static_cast<float>(h))));
}

//----------------------------------------------------------------------------//
void DirectFBRenderTarget::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
void DirectFBRenderTarget::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
void DirectFBRenderTarget::setArea(const Rectf& area)
{
    d_area = area;

    RenderTargetEventArgs args(this);
    fireEvent(EventAreaChanged, args);
}

//----------------------------------------------------------------------------//
const Rectf& DirectFBRenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
bool DirectFBRenderTarget::isImageryCache() const
{
    return false;
}

//----------------------------------------------------------------------------//
void DirectFBRenderTarget::activate()
{
    d_owner.setTargetSurface(d_target);
}

//----------------------------------------------------------------------------//
void DirectFBRenderTarget::deactivate()
{
}

//----------------------------------------------------------------------------//
void DirectFBRenderTarget::unprojectPoint(const GeometryBuffer& buff,
    const Vector2f& p_in, Vector2f& p_out) const
{
    // TODO:
    p_out = p_in;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
