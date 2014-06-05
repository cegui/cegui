/***********************************************************************
    filename:   ListView.cpp
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Implementation for a view that displays a list of model items.
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
#include "CEGUI/views/ListView.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String ListView::EventNamespace("ListView");
const String ListView::WidgetTypeName("CEGUI/ListView");

//----------------------------------------------------------------------------//
BasicRenderedStringParser ListView::d_stringParser;

//----------------------------------------------------------------------------//
ListView::ListView(const String& type, const String& name) :
    ItemView(type, name)
{

}

//----------------------------------------------------------------------------//
ListView::~ListView()
{

}

//----------------------------------------------------------------------------//
void ListView::prepareForRender()
{
    if (!d_renderingState.d_isDirty)
        return;

    ModelIndex root_index = d_itemModel->getRootIndex();
    size_t child_count = d_itemModel->getChildCount(root_index);

    d_renderingState.d_renderedStrings.clear();
    d_renderingState.d_renderedStrings.resize(child_count);

    d_renderingState.d_renderedStringSizes.clear();
    d_renderingState.d_renderedStringSizes.resize(child_count);

    // TODO: migrate ListboxTextItem colorness
    ColourRect colour_rect(Colour(1, 1, 1));

    for (size_t child = 0; child < child_count; ++child)
    {
        String text = d_itemModel->getData(d_itemModel->makeIndex(child, root_index));

        // TODO: migrate the ListboxTextItem string rendering
        RenderedString rendered_string =
            d_stringParser.parse(text, getFont(), &colour_rect);
        d_renderingState.d_renderedStrings.at(child) = rendered_string;

        d_renderingState.d_renderedStringSizes.at(child) = computeSizeOfRenderedString(rendered_string);
    }

    d_renderingState.d_isDirty = false;
}

//----------------------------------------------------------------------------//
Sizef ListView::computeSizeOfRenderedString(RenderedString &rendered_string)
{
    Sizef string_size(0.0f, 0.0f);

    for (size_t line = 0; line < rendered_string.getLineCount(); ++line)
    {
        const Sizef line_sz(rendered_string.getPixelSize(this, line));

        string_size.d_height += line_sz.d_height;
        if (line_sz.d_width > string_size.d_width)
            string_size.d_width = line_sz.d_width;
    }

    return string_size;
}
}