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
#ifndef _CEGUITestsItemModelStub_h_
#define _CEGUITestsItemModelStub_h_

#include "CEGUI/views/ItemModel.h"
#include <vector>

class ItemModelStub : public CEGUI::ItemModel
{
public:
    bool isValidIndex(const CEGUI::ModelIndex& model_index) const override;
    CEGUI::ModelIndex makeIndex(size_t child, const CEGUI::ModelIndex& model_index) override;
    bool areIndicesEqual(const CEGUI::ModelIndex& index1, const CEGUI::ModelIndex& index2) const override;
    int compareIndices(const CEGUI::ModelIndex& index1, const CEGUI::ModelIndex& index2) const override;
    CEGUI::ModelIndex getParentIndex(const CEGUI::ModelIndex& model_index) const override;
    int getChildId(const CEGUI::ModelIndex& model_index) const override;
    size_t getChildCount(const CEGUI::ModelIndex& model_index) const override;
    CEGUI::String getData(const CEGUI::ModelIndex& model_index, CEGUI::ItemDataRole role = CEGUI::ItemDataRole::Text) override;
    CEGUI::ModelIndex getRootIndex() const override;

    std::vector<CEGUI::String> d_items;
};

#endif
