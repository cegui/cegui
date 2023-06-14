/***********************************************************************
    created:    Tue Mar 3 2009
    author:     Paul D Turner (parts based on original code by Thomas Suter)
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
#include "CEGUI/RendererModules/Irrlicht/RenderTarget.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/RendererModules/Irrlicht/GeometryBuffer.h"

// Start of CEGUI namespace section
namespace CEGUI
{

IrrlichtRenderTarget::IrrlichtRenderTarget(IrrlichtRenderer& owner,
                                           irr::video::IVideoDriver& driver) :
    d_owner(owner),
    d_driver(driver),
    d_area(0, 0, 0, 0),
    d_matrixValid(false),
    d_viewDistance(0),
#if IRRLICHT_VERSION_MAJOR > 1 || (IRRLICHT_VERSION_MAJOR == 1 && IRRLICHT_VERSION_MINOR >= 8)
    d_xViewDir(1.0f)
#else
    d_xViewDir(driver.getDriverType() != irr::video::EDT_OPENGL ? 1.0f : -1.0f)
#endif
{
}


IrrlichtRenderTarget::~IrrlichtRenderTarget()
{
}


void IrrlichtRenderTarget::draw(const GeometryBuffer& buffer, std::uint32_t drawModeMask)
{
    buffer.draw(drawModeMask);
}


void IrrlichtRenderTarget::draw(const RenderQueue& queue, std::uint32_t drawModeMask)
{
    queue.draw(drawModeMask);
}


void IrrlichtRenderTarget::setArea(const Rectf& area)
{
    d_area = area;
    d_matrixValid = false;

    RenderTargetEventArgs args(this);
    T::fireEvent(RenderTarget::EventAreaChanged, args);
}


const Rectf& IrrlichtRenderTarget::getArea() const
{
    return d_area;
}


void IrrlichtRenderTarget::activate()
{
    irr::core::rect<irr::s32> vp(static_cast<irr::s32>(d_area.left()),
                                 static_cast<irr::s32>(d_area.top()),
                                 static_cast<irr::s32>(d_area.right()),
                                 static_cast<irr::s32>(d_area.bottom()));
    d_driver.setViewPort(vp);

    if (!d_matrixValid)
        updateMatrix();

    d_driver.setTransform(irr::video::ETS_PROJECTION, d_matrix);
    d_driver.setTransform(irr::video::ETS_VIEW, irr::core::matrix4());
}


void IrrlichtRenderTarget::deactivate()
{
}


void IrrlichtRenderTarget::updateMatrix() const
{
    const float w = d_area.getWidth();
    const float h = d_area.getHeight();
    const float aspect = w / h;
    const float midx = w * 0.5f;
    const float midy = h * 0.5f;
    d_viewDistance = midx / (aspect * 0.267949192431123f);

    d_matrix.buildProjectionMatrixPerspectiveFovRH(0.523598776f, aspect,
                                                   d_viewDistance * 0.5f,
                                                   d_viewDistance * 2.0f);

    irr::core::matrix4 tmp;
    tmp.buildCameraLookAtMatrixRH(
        irr::core::vector3df(d_xViewDir * midx, midy, -d_viewDistance),
        irr::core::vector3df(d_xViewDir * midx, midy, 1),
        irr::core::vector3df(0, -1, 0));

    d_matrix *= tmp;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
