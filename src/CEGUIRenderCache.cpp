/************************************************************************
    filename:   CEGUIRenderCache.cpp
    created:    Fri Jun 17 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "CEGUIRenderCache.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    RenderCache::RenderCache()
    {}

    RenderCache::~RenderCache()
    {}

    bool RenderCache::hasCachedImagery() const
    {
        return !d_cachedImages.empty();
    }

    void RenderCache::render(const Point& basePos, float baseZ, const Rect& clipper) const
    {
        Rect finalRect;

        for(ImageryList::const_iterator curr = d_cachedImages.begin(); curr != d_cachedImages.end(); ++curr)
        {
            finalRect = (*curr).target_area;
            finalRect.offset(basePos);
            (*curr).source_image->draw(finalRect, baseZ + (*curr).z_offset, clipper, (*curr).colours);
        }
    }

    void RenderCache::clearCachedImagery()
    {
        d_cachedImages.clear();
    }

    void RenderCache::cacheImage(const Image& image, const Rect& destArea, float zOffset, const ColourRect& cols)
    {
        ImageInfo imginf;
        imginf.source_image = &image;
        imginf.target_area  = destArea;
        imginf.z_offset     = zOffset;
        imginf.colours      = cols;

        d_cachedImages.push_back(imginf);
    }

} // End of  CEGUI namespace section
