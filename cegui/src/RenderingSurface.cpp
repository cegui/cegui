/***********************************************************************
    created:    Mon Jan 12 2009
    author:     Paul D Turner
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
#include "CEGUI/RenderingSurface.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/RenderingWindow.h"
#include "CEGUI/Renderer.h"
#include <algorithm>

namespace CEGUI
{
// Namespace for global events
const String RenderingSurface::EventNamespace("RenderingSurface");
// Event that signals the start of rendering for a queue.
const String RenderingSurface::EventRenderQueueStarted("RenderQueueStarted");
// Event that signals the end of rendering for a queue.
const String RenderingSurface::EventRenderQueueEnded("RenderQueueEnded");

//----------------------------------------------------------------------------//
RenderingSurface::RenderingSurface(RenderTarget& target)
    : d_target(&target)
{
}

//----------------------------------------------------------------------------//
RenderingSurface::~RenderingSurface()
{
    for (auto wnd : d_windows)
        delete wnd;
}

//----------------------------------------------------------------------------//
void RenderingSurface::addGeometryBuffers(const RenderQueueID queue,
    const std::vector<GeometryBuffer*>& geometry_buffers)
{
    d_queues[queue].addGeometryBuffers(geometry_buffers);
}

//----------------------------------------------------------------------------//
void RenderingSurface::addGeometryBuffer(const RenderQueueID queue,
     GeometryBuffer& geometry_buffer)
{
    d_queues[queue].addGeometryBuffer(geometry_buffer);
}

//----------------------------------------------------------------------------//
void RenderingSurface::removeGeometryBuffer(const RenderQueueID queue,
    const GeometryBuffer& geometry_buffer)
{
    d_queues[queue].removeGeometryBuffer(geometry_buffer);
}

//----------------------------------------------------------------------------//
void RenderingSurface::clearGeometry(const RenderQueueID queue)
{
    d_queues[queue].reset();
}

//----------------------------------------------------------------------------//
void RenderingSurface::clearGeometry()
{
    for (auto& pair : d_queues)
        pair.second.reset();
}

//----------------------------------------------------------------------------//
void RenderingSurface::draw(std::uint32_t drawMode)
{
    d_target->activate();
    d_target->getOwner().uploadBuffers(*this);
    drawContent(drawMode);
    d_target->deactivate();
}

//----------------------------------------------------------------------------//
void RenderingSurface::drawContent(std::uint32_t drawModeMask)
{
    RenderQueueEventArgs evt_args(RenderQueueID::User0);
    for (const auto& pair : d_queues)
    {
        evt_args.handled = 0;
        evt_args.queueID = pair.first;
        draw(pair.second, evt_args, drawModeMask);
    }
}

//----------------------------------------------------------------------------//
void RenderingSurface::draw(const RenderQueue& queue, RenderQueueEventArgs& args, std::uint32_t drawModeMask)
{
    fireEvent(EventRenderQueueStarted, args, EventNamespace);

    d_target->draw(queue, drawModeMask);

    args.handled = 0;
    fireEvent(EventRenderQueueEnded, args, EventNamespace);
}

//----------------------------------------------------------------------------//
bool RenderingSurface::isInvalidated() const
{
    return d_invalidated || !d_target->isImageryCache();
}

//----------------------------------------------------------------------------//
RenderingWindow& RenderingSurface::createRenderingWindow(TextureTarget& target)
{
    RenderingWindow* w = new RenderingWindow(target, *this);
    attachWindow(*w);
    return *w;
}

//----------------------------------------------------------------------------//
void RenderingSurface::destroyRenderingWindow(RenderingWindow& window)
{
    if (&window.getOwner() == this)
    {
        detachWindow(window);
        delete &window;
    }
}

//----------------------------------------------------------------------------//
void RenderingSurface::transferRenderingWindow(RenderingWindow& window)
{
    if (&window.getOwner() != this)
    {
        window.getOwner().detachWindow(window);
        attachWindow(window);
        window.setOwner(*this);
    }
}

//----------------------------------------------------------------------------//
void RenderingSurface::detachWindow(RenderingWindow& w)
{
    auto it = std::find(d_windows.cbegin(), d_windows.cend(), &w);
    if (it != d_windows.cend())
    {
        d_windows.erase(it);
        invalidate();
    }
}

//----------------------------------------------------------------------------//
void RenderingSurface::attachWindow(RenderingWindow& w)
{
    d_windows.push_back(&w);
    invalidate();
}

}
