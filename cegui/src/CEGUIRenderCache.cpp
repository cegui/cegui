/***********************************************************************
    filename:   CEGUIRenderCache.cpp
    created:    Fri Jun 17 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIRenderCache.h"
#include "CEGUISystem.h"
#include "CEGUIRenderer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    RenderCache::RenderCache()
    {}

    RenderCache::~RenderCache()
    {}

    bool RenderCache::hasCachedImagery() const
    {
        return !(d_cachedImages.empty() && d_cachedTexts.empty());
    }

    void RenderCache::render(const Point& basePos, float baseZ, const Rect& clipper)
    {
        Rect displayArea(System::getSingleton().getRenderer()->getRect());
        Rect custClipper;
        const Rect* finalClipper;
        Rect finalRect;

        // Send all cached images to renderer.
        for(ImageryList::const_iterator image = d_cachedImages.begin(); image != d_cachedImages.end(); ++image)
        {
            if ((*image).usingCustomClipper)
            {
                custClipper = (*image).customClipper;
                custClipper.offset(basePos);
                custClipper = (*image).clipToDisplay ? displayArea.getIntersection(custClipper) : clipper.getIntersection(custClipper);
                finalClipper = &custClipper;
            }
            else
            {
                finalClipper = (*image).clipToDisplay ? &displayArea : &clipper;
            }

            finalRect = (*image).target_area;
            finalRect.offset(basePos);
            (*image).source_image->draw(finalRect, baseZ + (*image).z_offset, *finalClipper, (*image).colours);
        }

        // send all cached texts to renderer.
        for(TextList::const_iterator text = d_cachedTexts.begin(); text != d_cachedTexts.end(); ++text)
        {
            if ((*text).usingCustomClipper)
            {
                custClipper = (*text).customClipper;
                custClipper.offset(basePos);
                custClipper = (*text).clipToDisplay ? displayArea.getIntersection(custClipper) : clipper.getIntersection(custClipper);
                finalClipper = &custClipper;
            }
            else
            {
                finalClipper = (*text).clipToDisplay ? &displayArea : &clipper;
            }

            finalRect = (*text).target_area;
            finalRect.offset(basePos);
            (*text).source_font->drawText((*text).text, finalRect, baseZ + (*text).z_offset, *finalClipper, (*text).formatting, (*text).colours);
        }

    }

    void RenderCache::clearCachedImagery()
    {
        d_cachedImages.clear();
        d_cachedTexts.clear();
    }

    void RenderCache::cacheImage(const Image& image, const Rect& destArea, float zOffset, const ColourRect& cols, const Rect* clipper, bool clipToDisplay)
    {
        ImageInfo imginf;
        imginf.source_image = &image;
        imginf.target_area  = destArea;
        imginf.z_offset     = zOffset;
        imginf.colours      = cols;
        imginf.clipToDisplay = clipToDisplay;

        if (clipper)
        {
            imginf.customClipper = *clipper;
            imginf.usingCustomClipper = true;
        }
        else
        {
            imginf.usingCustomClipper = false;
        }

        d_cachedImages.push_back(imginf);
    }

    void RenderCache::cacheText(const String& text, Font* font, TextFormatting format, const Rect& destArea, float zOffset, const ColourRect& cols, const Rect* clipper, bool clipToDisplay)
    {
        TextInfo txtinf;
        txtinf.text         = text;
        txtinf.source_font  = font;
        txtinf.formatting   = format;
        txtinf.target_area  = destArea;
        txtinf.z_offset     = zOffset;
        txtinf.colours      = cols;
        txtinf.clipToDisplay = clipToDisplay;

        if (clipper)
        {
            txtinf.customClipper = *clipper;
            txtinf.usingCustomClipper = true;
        }
        else
        {
            txtinf.usingCustomClipper = false;
        }

        d_cachedTexts.push_back(txtinf);
    }


} // End of  CEGUI namespace section
