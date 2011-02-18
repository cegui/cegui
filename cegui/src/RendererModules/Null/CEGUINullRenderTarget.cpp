/***********************************************************************
    filename:   CEGUINullRenderTarget.cpp
    created:    Sat Jan 16 2010
    author:     Eugene Marcotte
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "CEGUINullRenderTarget.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUIRenderQueue.h"
#include "CEGUINullGeometryBuffer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
NullRenderTarget::NullRenderTarget(NullRenderer& owner) :
    d_owner(owner),
    d_area(0, 0, 0, 0)
{
}

//----------------------------------------------------------------------------//
NullRenderTarget::~NullRenderTarget()
{
}

//----------------------------------------------------------------------------//
void NullRenderTarget::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
void NullRenderTarget::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
void NullRenderTarget::setArea(const Rect& area)
{
    d_area = area;
}

//----------------------------------------------------------------------------//
const Rect& NullRenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
void NullRenderTarget::activate()
{
}

//----------------------------------------------------------------------------//
void NullRenderTarget::deactivate()
{
}

//----------------------------------------------------------------------------//
void NullRenderTarget::unprojectPoint(const GeometryBuffer& buff,
                                      const Vector2<>& p_in, Vector2<>& p_out) const
{
	p_out = p_in;
}

//----------------------------------------------------------------------------//
bool NullRenderTarget::isImageryCache() const 
{
	return false;
}

} // End of  CEGUI namespace section
