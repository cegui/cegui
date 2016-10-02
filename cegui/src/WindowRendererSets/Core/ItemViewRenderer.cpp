/***********************************************************************
    created:    Mon Jun 02 2014
    author:     Timotei Dolean <timotei21@gmail.com>
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
#include "CEGUI/WindowRendererSets/Core/ItemViewRenderer.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/Font.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
ItemViewRenderer::~ItemViewRenderer()
{
}

//----------------------------------------------------------------------------//
Rectf ItemViewRenderer::getViewRenderArea(const ItemView* item_view) const
{
    return getViewRenderArea(
        item_view,
        item_view->getHorzScrollbar()->isVisible(),
        item_view->getVertScrollbar()->isVisible());
}

//----------------------------------------------------------------------------//
Rectf ItemViewRenderer::getViewRenderArea(const ItemView* item_view,
    bool hscroll, bool vscroll) const
{
    const WidgetLookFeel& wlf =
        WidgetLookManager::getSingleton().getWidgetLook(item_view->getLookNFeel());
    String scroll_suffix;

    if (vscroll)
        scroll_suffix += "V";

    if (hscroll)
        scroll_suffix += "H";

    if(!scroll_suffix.empty())
        scroll_suffix += "Scroll";

    const String area_names[] = { "ItemRenderingArea", "ItemRenderArea" };
    const String suffixes[] = { scroll_suffix, "" };

    for (size_t suffix_id = 0; suffix_id < 2; suffix_id++)
    {
        const String& suffix = suffixes[suffix_id];

        for (size_t area_id = 0; area_id < 2; ++area_id)
        {
            const String full_area_name(area_names[area_id] + suffix);

            if (wlf.isNamedAreaPresent(full_area_name))
                return wlf.getNamedArea(full_area_name).getArea().
                    getPixelRect(*item_view);
        }
    }

    throw UnknownObjectException("There is no item rendering area defined!");
}

//----------------------------------------------------------------------------//
void ItemViewRenderer::createRenderGeometryAndAddToItemView(
    ItemView* view, RenderedString& rendered_string,
    Rectf draw_rect, const Font* font, const Rectf* item_clipper, bool is_selected)
{
    if (view->getSelectionBrushImage() != nullptr && is_selected)
    {
        ImageRenderSettings renderSettings(
            draw_rect,
            item_clipper,
            true,
            view->getSelectionColourRect());

        auto brushGeomBuffers = view->getSelectionBrushImage()->createRenderGeometry(
            renderSettings);

        view->appendGeometryBuffers(brushGeomBuffers);
    }

    glm::vec2 draw_pos(draw_rect.getPosition());
    for (size_t i = 0; i < rendered_string.getLineCount(); ++i)
    {
        draw_pos.y += CoordConverter::alignToPixels(
            (font->getLineSpacing() - font->getFontHeight()) * 0.5f);

        auto stringGeomBuffers = rendered_string.createRenderGeometry(
            view, i,
            draw_pos, nullptr, item_clipper, 0.0f);

        view->appendGeometryBuffers(stringGeomBuffers);

        draw_pos.y += rendered_string.getPixelSize(view, i).d_height;
    }
}

//----------------------------------------------------------------------------//
glm::vec2 ItemViewRenderer::getItemRenderStartPosition(ItemView* view,
    const Rectf& items_area) const
{
    return glm::vec2(
        items_area.left() - view->getHorzScrollbar()->getScrollPosition(),
        items_area.top() - view->getVertScrollbar()->getScrollPosition()
        );
}

//----------------------------------------------------------------------------//
void ItemViewRenderer::resizeViewToContent(ItemView* view, bool fit_width,
    bool fit_height) const
{
    const Rectf totalArea(view->getUnclippedOuterRect().get());
    const Rectf contentArea(getViewRenderArea(view,
        fit_width ? false : view->getHorzScrollbar()->isVisible(),
        fit_height ? false : view->getVertScrollbar()->isVisible()));
    const Rectf withScrollContentArea(getViewRenderArea(view, true, true));

    const Sizef frameSize(totalArea.getSize() - contentArea.getSize());
    const Sizef withScrollFrameSize(totalArea.getSize() -
        withScrollContentArea.getSize());
    const Sizef contentSize(view->getRenderedMaxWidth(),
        view->getRenderedTotalHeight());

    const Sizef parentSize(view->getParentPixelSize());
    const Sizef maxSize(
        parentSize.d_width -
        CoordConverter::asAbsolute(view->getXPosition(), parentSize.d_width),
        parentSize.d_height -
        CoordConverter::asAbsolute(view->getYPosition(), parentSize.d_height));

    Sizef requiredSize(frameSize + contentSize + Sizef(1, 1));

    if (fit_height && requiredSize.d_height > maxSize.d_height)
    {
        requiredSize.d_height = maxSize.d_height;
        requiredSize.d_width = std::min(
            maxSize.d_width,
            requiredSize.d_width - frameSize.d_width + withScrollFrameSize.d_width);
    }

    if (fit_width && requiredSize.d_width > maxSize.d_width)
    {
        requiredSize.d_width = maxSize.d_width;
        requiredSize.d_height = std::min(
            maxSize.d_height,
            requiredSize.d_height - frameSize.d_height + withScrollFrameSize.d_height);
    }

    if (fit_height)
        view->setHeight(UDim(0, requiredSize.d_height));

    if (fit_width)
        view->setWidth(UDim(0, requiredSize.d_width));
}
}
