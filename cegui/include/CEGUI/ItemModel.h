/***********************************************************************
    filename:   ItemModel.h
    created:    Tue May 20 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Provides ModelIndex and ItemModel which are the basis of the
                Model-View architecture.
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
#ifndef _CEGUIItemModel_h_
#define _CEGUIItemModel_h_

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Structure that is holds an index to a model item, specified by row and column.
*/
struct ModelIndex
{
    //! Constructs a default, invalid index
    ModelIndex() :
        d_row(-1), d_column(-1), modelData(0)
    {
    }

    //! Constructs an index with the specified row and column
    ModelIndex(int row, int column, void* modelData = 0) :
        d_row(row), d_column(column), modelData(modelData)
    {
    }

    //! The row represented by this index.
    int d_row;
    //! The column represented by this index.
    int d_column;

    /*!
    \brief
        Pointer to model-specific data.

    \remark
        DO NOT USE/INTERPRET in view. This is just a simple way for the model
        to be able to manage its data and logic.
    */
    void* modelData;

    //! Returns true if this model index represents a valid index, false otherwise.
    bool isValid() const { return d_row > 0 && d_column > 0; }
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIItemModel_h
