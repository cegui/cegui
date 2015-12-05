/***********************************************************************
    created:    Wed May 23 2014
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
#ifndef _CEGUIStandardItemModel_h_
#define _CEGUIStandardItemModel_h_

#include "CEGUI/views/ItemModel.h"
#include "CEGUI/views/GenericItemModel.h"
#include <vector>

namespace CEGUI
{

/*!
\brief
    This is an implementation of GenericItem that has an additional id.
*/
class CEGUIEXPORT StandardItem : public GenericItem
{
public:
    StandardItem();
    explicit StandardItem(const String& text, unsigned int id = 0);
    StandardItem(const String& text, const String& icon, unsigned int id = 0);

    //! Id of this item
    unsigned int getId() const { return d_id; }
    void setId(unsigned int val) { d_id = val; }

protected:
    unsigned int d_id;
};

/*!
\brief
    This is an example model that inherits the default GenericItemModel
    functionality over a specified model type (StandardItem).

    Besides that we added a new function that updates a given item's text.
*/
class CEGUIEXPORT StandardItemModel : public GenericItemModel<StandardItem>
{
public:
    StandardItemModel();

    /*!
    \brief
        Updates the specified \a item's text with the new one.

        This function will notify the listeners that the specified \a item's
        text has been changed, via the EventChildrenDataWillChange and
        EventChildrenDataChanged events.
    */
    void updateItemText(StandardItem* item, const String& new_text);
};

}

#endif
