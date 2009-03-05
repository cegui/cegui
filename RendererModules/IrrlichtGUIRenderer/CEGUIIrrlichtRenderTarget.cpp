/***********************************************************************
    filename:   CEGUIIrrlichtRenderTarget.cpp
    created:    Tue Mar 3 2009
    author:     Paul D Turner (parts based on original code by Thomas Suter)
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
#include "CEGUIIrrlichtRenderTarget.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUIRenderQueue.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
IrrlichtRenderTarget::IrrlichtRenderTarget(IrrlichtRenderer& owner,
                                           irr::video::IVideoDriver& driver) :
    d_owner(owner),
    d_driver(driver),
    d_area(0, 0, 0, 0),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
IrrlichtRenderTarget::~IrrlichtRenderTarget()
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderTarget::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
void IrrlichtRenderTarget::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
void IrrlichtRenderTarget::setArea(const Rect& area)
{
    d_area = area;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
const Rect& IrrlichtRenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
void IrrlichtRenderTarget::activate()
{
    irr::core::rect<irr::s32> vp(d_area.d_left, d_area.d_top, 
                                 d_area.d_right, d_area.d_bottom);
    d_driver.setViewPort(vp);

    if (!d_matrixValid)
        updateMatrix();

    d_driver.setTransform(irr::video::ETS_PROJECTION, d_matrix);
    d_driver.setTransform(irr::video::ETS_VIEW, irr::core::matrix4());
}

//----------------------------------------------------------------------------//
void IrrlichtRenderTarget::deactivate()
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderTarget::unprojectPoint(const GeometryBuffer& buff,
                                          const Vector2& p_in,
                                          Vector2& p_out) const
{
    p_out = p_in;
}

//----------------------------------------------------------------------------//
void IrrlichtRenderTarget::updateMatrix() const
{
    const float w = d_area.getWidth();
    const float h = d_area.getHeight();
    const float aspect = w / h;
    const float midx = w * 0.5f;
    const float midy = h * 0.5f;
    d_viewDistance = midx / (aspect * 0.267949192431123f);

    const float nearZ = d_viewDistance * 0.5f;
    const float farZ = d_viewDistance * 2.0f;
    const float nr_sub_far = nearZ - farZ;

    d_matrix.buildProjectionMatrixPerspectiveFovRH(0.523598776f, aspect,
                                                   d_viewDistance * 0.5,
                                                   d_viewDistance * 2);

    irr::core::matrix4 tmp;
    tmp.buildCameraLookAtMatrixRH(irr::core::vector3df(-midx, midy, -d_viewDistance),
                                  irr::core::vector3df(-midx, midy, 1),
                                  irr::core::vector3df(0, -1, 0));

    d_matrix *= tmp;

//     d_matrix(0, 0) = 3.732050808f / aspect;
//     d_matrix(1, 1) = -3.732050808f;
//     d_matrix(2, 2) = -((farZ + nearZ) / nr_sub_far);
//     d_matrix(2, 3) = 1.0f;
//     d_matrix(3, 0) = -d_viewDistance;
//     d_matrix(3, 1) = d_viewDistance;
//     d_matrix(3, 3) = d_viewDistance;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
