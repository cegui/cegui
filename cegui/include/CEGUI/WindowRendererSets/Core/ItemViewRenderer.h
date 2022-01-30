/***********************************************************************
    created:    Mon Jun 02 2014
    author:     Timotei Dolean <timotei21@gmail.com>
    purpose:    Contains common rendering-related routines
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _FalItemViewRenderer_h_
#define _FalItemViewRenderer_h_

#include "CEGUI/WindowRendererSets/Core/Module.h"
#include "CEGUI/views/ItemView.h"
#include "CEGUI/Rectf.h"

namespace CEGUI
{
class RenderedText;

/*!
\brief
    Base class that contains common routines for view renderers.
*/
class COREWRSET_API ItemViewRenderer
{
public:
    virtual ~ItemViewRenderer();

protected:
    virtual Rectf getViewRenderArea(const ItemView* renderer) const;
    Rectf getViewRenderArea(const ItemView* renderer,
        bool hscroll, bool vscroll) const;
    glm::vec2 getItemRenderStartPosition(ItemView* view, const Rectf& items_area) const;
    void createRenderGeometryAndAddToItemView(ItemView* view, const RenderedText& renderedText,
        const Rectf& draw_rect, const Font* font, const ColourRect* modColours,
        const Rectf* item_clipper, bool is_selected);
    void resizeViewToContent(ItemView* view, bool fit_width, bool fit_height) const;
};

}

#endif
