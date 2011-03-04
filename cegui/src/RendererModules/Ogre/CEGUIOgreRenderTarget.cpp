/***********************************************************************
    filename:   CEGUIOgreRenderTarget.cpp
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
#include "CEGUIOgreRenderTarget.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUIRenderQueue.h"
#include "CEGUIOgreGeometryBuffer.h"

#include <OgreRenderSystem.h>
#include <OgreCamera.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OgreRenderTarget::OgreRenderTarget(OgreRenderer& owner,
                                   Ogre::RenderSystem& rs) :
    d_owner(owner),
    d_renderSystem(rs),
    d_area(0, 0, 0, 0),
    d_renderTarget(0),
    d_viewport(0),
    d_matrix(Ogre::Matrix3::ZERO),
    d_matrixValid(false),
    d_viewportValid(false),
    d_ogreViewportDimensions(0, 0, 0, 0)
{
}

//----------------------------------------------------------------------------//
OgreRenderTarget::~OgreRenderTarget()
{
    delete d_viewport;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::setArea(const Rectf& area)
{
    d_area = area;
    setOgreViewportDimensions(area);

    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::setOgreViewportDimensions(const Rectf& area)
{
    d_ogreViewportDimensions = area;

    if (d_viewport)
        updateOgreViewportDimensions(d_viewport->getTarget());

    d_viewportValid = false;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::updateOgreViewportDimensions(
                                            const Ogre::RenderTarget* const rt)
{
    if (rt)
    {
        d_viewport->setDimensions(
            d_ogreViewportDimensions.left() / rt->getWidth(),
            d_ogreViewportDimensions.top() / rt->getHeight(),
            d_ogreViewportDimensions.getWidth() / rt->getWidth(),
            d_ogreViewportDimensions.getHeight() / rt->getHeight());
    }
}

//----------------------------------------------------------------------------//
const Rectf& OgreRenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::activate()
{
    if (!d_matrixValid)
        updateMatrix();

    if (!d_viewportValid)
        updateViewport();

    d_renderSystem._setViewport(d_viewport);
    d_renderSystem._setProjectionMatrix(d_matrix);
    d_renderSystem._setViewMatrix(Ogre::Matrix4::IDENTITY);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::deactivate()
{
    // currently nothing to do in the basic case
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::unprojectPoint(const GeometryBuffer& buff,
                                      const Vector2f& p_in,
                                      Vector2f& p_out) const
{
    if (!d_matrixValid)
        updateMatrix();

    const OgreGeometryBuffer& gb = static_cast<const OgreGeometryBuffer&>(buff);

    const Ogre::Real midx = d_area.getWidth() * 0.5f;
    const Ogre::Real midy = d_area.getHeight() * 0.5f;

    // viewport matrix
    const Ogre::Matrix4 vpmat(
        midx,    0,    0,    d_area.left() + midx,
         0,    -midy,  0,    d_area.top() + midy,
         0,      0,    1,    0,
         0,      0,    0,    1
    );

    // matrices used for projecting and unprojecting points
    const Ogre::Matrix4 proj(gb.getMatrix() * d_matrix * vpmat);
    const Ogre::Matrix4 unproj(proj.inverse());

    Ogre::Vector3 in;

    // unproject the ends of the ray
    in.x = midx;
    in.y = midy;
    in.z = -d_viewDistance;
    const Ogre::Vector3 r1(unproj * in);
    in.x = p_in.d_x;
    in.y = p_in.d_y;
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
                            0.0;

    p_out.d_x = static_cast<float>(r1.x - rv.x * tmp);
    p_out.d_y = static_cast<float>(r1.y - rv.y * tmp);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::updateMatrix() const
{
    const float w = d_area.getWidth();
    const float h = d_area.getHeight();
    const float aspect = w / h;
    const float midx = w * 0.5f;
    d_viewDistance = midx / (aspect * 0.267949192431123f);

    const float nearZ = d_viewDistance * 0.5f;
    const float farZ = d_viewDistance * 2.0f;
    const float nr_sub_far = nearZ - farZ;

    Ogre::Matrix4 tmp(Ogre::Matrix4::ZERO);
    tmp[0][0] = 3.732050808f / aspect;
    tmp[0][3] = -d_viewDistance;
    tmp[1][1] = -3.732050808f;
    tmp[1][3] = d_viewDistance;
    tmp[2][2] = -((farZ + nearZ) / nr_sub_far);
    tmp[3][2] = 1.0f;
    tmp[3][3] = d_viewDistance;
    d_renderSystem._convertProjectionMatrix(tmp, d_matrix);

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::updateViewport()
{
    if (!d_viewport)
    {
        d_viewport = new Ogre::Viewport(0, d_renderTarget, 0, 0, 1, 1, 0);
        updateOgreViewportDimensions(d_renderTarget);
    }

    d_viewport->_updateDimensions();
    d_viewportValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
