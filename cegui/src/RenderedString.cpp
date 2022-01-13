/***********************************************************************
    created:    25/05/2009
    author:     Paul Turner
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
#include "CEGUI/RenderedString.h"
#include "CEGUI/RenderedStringComponent.h"
#include "CEGUI/Exceptions.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
RenderedString::RenderedString()
{
    // set up initial line info
    appendLineBreak();
}

/*
//----------------------------------------------------------------------------//
RenderedString::RenderedString(const RenderedString& other)
{
    d_components.clear();
    for (const auto& component : other.d_components)
        d_components.push_back(component->clone());

    d_lines = other.d_lines;
}

//----------------------------------------------------------------------------//
RenderedString& RenderedString::operator =(const RenderedString& rhs)
{
    d_components.clear();
    for (const auto& component : rhs.d_components)
        d_components.push_back(component->clone());

    d_lines = rhs.d_lines;
    return *this;
}
*/

//----------------------------------------------------------------------------//
RenderedString::~RenderedString() = default;
RenderedString::RenderedString(RenderedString&&) noexcept = default;
RenderedString& RenderedString::operator =(RenderedString&&) noexcept = default;

//----------------------------------------------------------------------------//
void RenderedString::appendComponent(const RenderedStringComponent& component)
{
    d_components.push_back(component.clone());
    ++d_lines.back().second;
}

//----------------------------------------------------------------------------//
void RenderedString::clearComponents()
{
    d_components.clear();
    d_lines.clear();
}

//----------------------------------------------------------------------------//
void RenderedString::appendLineBreak()
{
    const size_t first_component = d_lines.empty() ? 0 :
        d_lines.back().first + d_lines.back().second;

    d_lines.push_back({ first_component, 0 });
}

//----------------------------------------------------------------------------//
Sizef RenderedString::getLineExtent(const Window* refWnd, const size_t line) const
{
    if (line >= getLineCount())
        throw InvalidRequestException("line number specified is invalid.");

    Sizef sz(0.f, 0.f);

    size_t i = d_lines[line].first;
    const size_t end_component = i + d_lines[line].second;
    for (; i < end_component; ++i)
    {
        const Sizef comp_sz = d_components[i]->getPixelSize(refWnd);
        sz.d_width += comp_sz.d_width;
        if (comp_sz.d_height > sz.d_height)
            sz.d_height = comp_sz.d_height;
    }

    return sz;
}

//----------------------------------------------------------------------------//
Sizef RenderedString::getExtent(const Window* refWnd) const
{
    Sizef totalExtent(0.f, 0.f);

    for (size_t i = 0; i < d_lines.size(); ++i)
    {
        const Sizef lineExtent = getLineExtent(refWnd, i);
        totalExtent.d_height += lineExtent.d_height;
        if (totalExtent.d_width < lineExtent.d_width)
            totalExtent.d_width = lineExtent.d_width;
    }

    return totalExtent;
}

//----------------------------------------------------------------------------//
size_t RenderedString::getSpaceCount(const size_t line) const
{
    if (line >= getLineCount())
        throw InvalidRequestException("line number specified is invalid.");

    size_t space_count = 0;

    const size_t end_component = d_lines[line].first + d_lines[line].second;
    for (size_t i = d_lines[line].first; i < end_component; ++i)
        space_count += d_components[i]->getSpaceCount();

    return space_count;
}

//----------------------------------------------------------------------------//
std::vector<GeometryBuffer*> RenderedString::createRenderGeometry(
    const Window* refWnd, const size_t line, const glm::vec2& position,
    const ColourRect* mod_colours, const Rectf* clip_rect, const float space_extra) const
{
    if (line >= getLineCount())
        throw InvalidRequestException("line number specified is invalid.");

    const float render_height = getLineExtent(refWnd, line).d_height;

    std::vector<GeometryBuffer*> geomBuffers;

    glm::vec2 pos = position;
    const size_t end_component = d_lines[line].first + d_lines[line].second;
    for (size_t i = d_lines[line].first; i < end_component; ++i)
    {
        auto currGeom = d_components[i]->createRenderGeometry(refWnd, pos, mod_colours, clip_rect,
                render_height, space_extra);
        geomBuffers.insert(geomBuffers.end(), currGeom.begin(), currGeom.end());

        pos.x += d_components[i]->getPixelSize(refWnd).d_width;
    }

    return geomBuffers;
}

//----------------------------------------------------------------------------//
void RenderedString::setSelection(const Window* refWnd, float start, float end)
{
    const size_t last_component = d_lines[0].second;
    float partial_extent = 0;
    size_t idx = 0;

    // clear last selection from all components
    for (size_t i = 0; i < d_components.size(); i++)
        d_components[i]->setSelection(refWnd, 0, 0);

    for (; idx < last_component; ++idx)
    {
        if (start <= partial_extent + d_components[idx]->getPixelSize(refWnd).d_width)
            break;
         partial_extent += d_components[idx]->getPixelSize(refWnd).d_width;
    }

    start -= partial_extent;
    end -= partial_extent;

    while (end > 0.0f)
    {
        const float comp_extent = d_components[idx]->getPixelSize(refWnd).d_width;
        d_components[idx]->setSelection(refWnd,
                                        start,
                                        (end >= comp_extent) ? comp_extent : end);
        start = 0;
        end -= comp_extent;
        ++idx;
    }
}

//----------------------------------------------------------------------------//
bool RenderedString::split(const Window* refWnd, const size_t line,
    float splitPoint, RenderedString& left)
{
    if (line >= getLineCount())
        throw InvalidRequestException("line number specified is invalid.");

    left.clearComponents();

    if (d_components.empty())
        return false;

    // Move all components in lines prior to the line being split to the left
    if (line > 0)
    {
        // Move components to left side
        auto cb = d_components.begin();
        auto ce = cb + d_lines[line - 1].first + d_lines[line - 1].second;
        left.d_components.assign(std::make_move_iterator(cb), std::make_move_iterator(ce));
        d_components.erase(cb, ce);

        // Move lines to left side
        auto lb = d_lines.begin();
        auto le = lb + line;
        left.d_lines.assign(lb, le);
        d_lines.erase(lb, le);
    }

    // Find the component where the requested split point lies.
    size_t idx = 0;
    float partial_extent = 0.f;
    const size_t last_component = d_lines[0].second;
    for (; idx < last_component; ++idx)
    {
        partial_extent += d_components[idx]->getPixelSize(refWnd).d_width;
        if (splitPoint <= partial_extent)
            break;
    }

    // Components up to 'idx' are transfered to 'left'
    if (idx > 0)
    {
        auto cb = d_components.begin();
        auto ce = cb + idx;
        left.d_components.insert(left.d_components.end(), std::make_move_iterator(cb), std::make_move_iterator(ce));
        d_components.erase(cb, ce);
    }

    bool wasWordSplit = false;

    if (idx == last_component)
    {
        // Split point is past the end, move the whole line to left side
        left.d_lines.push_back(d_lines[0]);
        d_lines.erase(d_lines.begin());
    }
    else
    {
        left.appendLineBreak();
        auto& leftLine = left.d_lines[left.getLineCount() - 1];
        leftLine.second += idx;
        d_lines[0].second -= idx;

        // Now split item 'idx' between left and this (right).
        RenderedStringComponent* c = d_components[0].get();
        if (c->canSplit())
        {
            const float localPt = splitPoint - (partial_extent - c->getPixelSize(refWnd).d_width);
            if (auto lc = c->split(refWnd, localPt, idx == 0, wasWordSplit))
            {
                left.d_components.push_back(std::move(lc));
                ++leftLine.second;
            }
        }
        // Can't split, if component width is >= splitPoint xfer the whole
        // component to it's own line in the left part (FIX #306)
        else if (c->getPixelSize(refWnd).d_width >= splitPoint)
        {
            left.appendLineBreak();
            left.d_components.push_back(std::move(d_components[0]));
            d_components.erase(d_components.begin());
            ++left.d_lines[left.getLineCount() - 1].second;
            --d_lines[0].second;
        }
    }

    // Fix up lines in this object
    for (size_t comp = 0, i = 0; i < d_lines.size(); ++i)
    {
        d_lines[i].first = comp;
        comp += d_lines[i].second;
    }

    return wasWordSplit;
}

}
