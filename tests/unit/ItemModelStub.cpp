/***********************************************************************
 *    created:    Sat Jun 07 2014
 *    author:     Timotei Dolean <timotei21@gmail.com>
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
#include "ItemModelStub.h"
#include <cassert>
#include <iterator>

using namespace CEGUI;

//----------------------------------------------------------------------------//
bool ItemModelStub::isValidIndex(const ModelIndex& model_index) const
{
    return model_index.d_modelData != nullptr;
}

//----------------------------------------------------------------------------//
ModelIndex ItemModelStub::makeIndex(size_t child, const ModelIndex&)
{
    assert(child < d_items.size());
    return ModelIndex(&d_items.at(child));
}

//----------------------------------------------------------------------------//
size_t ItemModelStub::getChildCount(const ModelIndex&) const
{
    return d_items.size();
}

//----------------------------------------------------------------------------//
ModelIndex ItemModelStub::getParentIndex(const ModelIndex&) const
{
    return ModelIndex();
}

//----------------------------------------------------------------------------//
ModelIndex ItemModelStub::getRootIndex() const
{
    return ModelIndex();
}

//----------------------------------------------------------------------------//
String ItemModelStub::getData(const ModelIndex& model_index, ItemDataRole /*role = ItemDataRole::Text*/)
{
    assert(model_index.d_modelData != nullptr);
    return *(static_cast<String*>(model_index.d_modelData));
}

//----------------------------------------------------------------------------//
bool ItemModelStub::areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const
{
    return index1.d_modelData == index2.d_modelData;
}

//----------------------------------------------------------------------------//
int ItemModelStub::compareIndices(const ModelIndex& index1, const ModelIndex& index2) const
{
    return (*static_cast<String*>(index1.d_modelData)).compare(
        *static_cast<String*>(index2.d_modelData));
}

//----------------------------------------------------------------------------//
int ItemModelStub::getChildId(const ModelIndex& model_index) const
{
    String* str = static_cast<String*>(model_index.d_modelData);
    std::vector<String>::const_iterator itor = std::find(d_items.begin(), d_items.end(), *str);
    if (itor == d_items.end())
        return -1;

    return std::distance(d_items.begin(), itor);
}
