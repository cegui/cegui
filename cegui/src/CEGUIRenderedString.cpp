/***********************************************************************
    filename:   CEGUIRenderedString.cpp
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
#include "CEGUIRenderedString.h"
#include "CEGUIRenderedStringComponent.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
RenderedString::RenderedString()
{
}

//----------------------------------------------------------------------------//
RenderedString::~RenderedString()
{
}

//----------------------------------------------------------------------------//
void RenderedString::draw(GeometryBuffer& buffer, const Vector2& position,
                          const Rect* clip_rect) const
{
    const float render_height = getPixelSize().d_height;

    Vector2 comp_pos(position);

    ComponentList::const_iterator i = d_components.begin();
    for (; i != d_components.end(); ++i)
    {
        (*i)->draw(buffer, comp_pos, clip_rect, render_height);
        comp_pos.d_x += (*i)->getPixelSize().d_width;
    }
}

//----------------------------------------------------------------------------//
Size RenderedString::getPixelSize() const
{
    Size sz(0, 0);

    ComponentList::const_iterator i = d_components.begin();
    for (; i != d_components.end(); ++i)
    {
        const Size comp_sz((*i)->getPixelSize());
        sz.d_width += comp_sz.d_width;
        
        if (comp_sz.d_height > sz.d_height)
            sz.d_height = comp_sz.d_height;
    }

    return sz;
}

//----------------------------------------------------------------------------//
void RenderedString::appendComponent(const RenderedStringComponent& component)
{
    d_components.push_back(component.clone());
}

//----------------------------------------------------------------------------//
void RenderedString::clearComponents()
{
    clearComponentList(d_components);
}

//----------------------------------------------------------------------------//
size_t RenderedString::getComponentCount() const
{
    return d_components.size();
}

//----------------------------------------------------------------------------//
RenderedString::RenderedString(const RenderedString& other)
{
    cloneComponentList(other.d_components);
}

//----------------------------------------------------------------------------//
RenderedString& RenderedString::operator=(const RenderedString& rhs)
{
    cloneComponentList(rhs.d_components);
    return *this;
}

//----------------------------------------------------------------------------//
void RenderedString::cloneComponentList(const ComponentList& list)
{
    clearComponentList(d_components);

    for (size_t i = 0; i < list.size(); ++i)
        d_components.push_back(list[i]->clone());    
}

//----------------------------------------------------------------------------//
void RenderedString::clearComponentList(ComponentList& list)
{
    for (size_t i = 0; i < list.size(); ++i)
        delete list[i];

    list.clear();
}

//----------------------------------------------------------------------------//
void RenderedString::split(float split_point, RenderedString& left)
{
    left.clearComponents();

    if (d_components.empty())
        return;

    // find the component where the requested split point lies.
    float partial_extent = 0;

    size_t idx = 0;
    for (; idx < d_components.size(); ++idx)
    {
        partial_extent += d_components[idx]->getPixelSize().d_width;
    
        if (split_point <= partial_extent)
            break;
    }

    // case where split point is past the end
    if (idx >= d_components.size())
    {
        // transfer our components to the 'left' string.
        left.d_components.swap(d_components);
        return; 
    }

    // Everything up to 'idx' is xfered to 'left'
    for (size_t i = 0; i < idx; ++i)
    {
        left.d_components.push_back(d_components[0]);
        d_components.erase(d_components.begin());
    }

    // now to split item 'idx' putting half in left and leaving half in this.
    RenderedStringComponent* c = d_components[0];
    if (c->canSplit())
    {
        RenderedStringComponent* lc = 
            c->split(split_point - (partial_extent - c->getPixelSize().d_width),
                     idx == 0);

        if (lc)
            left.d_components.push_back(lc);
    }
}

//----------------------------------------------------------------------------//
    
} // End of  CEGUI namespace section
