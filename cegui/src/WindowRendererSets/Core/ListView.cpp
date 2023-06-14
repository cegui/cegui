/***********************************************************************
    created:    Mon May 26 2014
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
#include "CEGUI/WindowRendererSets/Core/ListView.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/Colour.h"
#include "CEGUI/text/Font.h"
#include "CEGUI/ImageManager.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
const String FalagardListView::TypeName("Core/ListView");
static ColourRect ICON_COLOUR_RECT(Colour(1, 1, 1, 1));

//----------------------------------------------------------------------------//
FalagardListView::FalagardListView(const String& type) :
    ItemViewWindowRenderer(type)
{
}

//----------------------------------------------------------------------------//
void FalagardListView::createRenderGeometry()
{
    const StateImagery* imagery;
    const WidgetLookFeel& wlf = getLookNFeel();
    ListView* list_view = static_cast<ListView*>(d_window);

    list_view->prepareForRender();

    bool has_focused_state =
        list_view->isFocused() && wlf.isStateImageryPresent("EnabledFocused");
    imagery = &wlf.getStateImagery(
        list_view->isEffectiveDisabled() ? "Disabled" :
            (has_focused_state ? "EnabledFocused" : "Enabled"));
    imagery->render(*list_view);

    createRenderGeometry(list_view);
}

//----------------------------------------------------------------------------//
void FalagardListView::createRenderGeometry(ListView* list_view)
{
    Rectf items_area(getViewRenderArea());
    glm::vec2 item_pos(getItemRenderStartPosition(list_view, items_area));

    for (size_t i = 0; i < list_view->getItems().size(); ++i)
    {
        ListViewItemRenderingState* item = list_view->getItems().at(i);
        Sizef size(item->d_size);

        size.d_width = std::max(items_area.getWidth(), size.d_width);

        Rectf item_rect;
        item_rect.left(item_pos.x);
        item_rect.top(item_pos.y);
        item_rect.setSize(size);

        if (!item->d_icon.empty())
        {
            Image& img = ImageManager::getSingleton().get(item->d_icon);

            Rectf icon_rect(item_rect);
            icon_rect.setWidth(size.d_height);
            icon_rect.setHeight(size.d_height);

            Rectf icon_clipper(icon_rect.getIntersection(items_area));

            ImageRenderSettings renderSettings(icon_rect, &icon_clipper, ICON_COLOUR_RECT, 1.0f);

            img.createRenderGeometry(list_view->getGeometryBuffers(), renderSettings);

            item_rect.left(item_rect.left() + icon_rect.getWidth());
        }

        Rectf item_clipper(item_rect.getIntersection(items_area));

        createRenderGeometryAndAddToItemView(list_view, item->d_renderedText,
            item_rect, list_view->getEffectiveFont(), &list_view->getTextColourRect(), &item_clipper, item->d_isSelected);

        item_pos.y += size.d_height;
    }
}

//----------------------------------------------------------------------------//
Rectf FalagardListView::getViewRenderArea(void) const
{
    return ItemViewRenderer::getViewRenderArea(getView());
}

//----------------------------------------------------------------------------//
void FalagardListView::resizeViewToContent(bool fit_width, bool fit_height) const
{
    ItemViewRenderer::resizeViewToContent(getView(), fit_width, fit_height);
}
}
