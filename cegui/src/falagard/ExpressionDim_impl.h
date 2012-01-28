/***********************************************************************
    filename:   CEGUIExpressionDim_impl.h
    created:    Wed Dec 30 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIFalExpressionDim_impl_h
#define _CEGUIFalExpressionDim_impl_h_

#include "CEGUI/falagard/Dimensions.h"
#include "./ExpressionDimParser.hpp"
#include <vector>
#include <utility>

// Start of CEGUI namespace section
namespace CEGUI
{

typedef std::pair<ExpressionDimParser::token_type, BaseDim*> ExpressionPart;
typedef std::vector<ExpressionPart> ExpressionStack;

struct ExpressionDim_impl
{
    ExpressionDim_impl() :
        d_performedScan(false),
        d_parser(this),
        d_stackPos(0),
        d_window(0),
        d_area(0),
        d_parsedValue(0.0f)
    {
    }

    //! String representation of the expression (i.e the original input).
    String d_expression;
    //! signifies whether d_expression has been scanned yet.
    bool d_performedScan;
    //! parser object that does the parse of pre-scanned tokens.
    ExpressionDimParser d_parser;
    //! stack of scanned expression parts.
    ExpressionStack d_stack;
    //! current parse location within the stack.
    size_t d_stackPos;
    //! CEGUI::Window for the current parse.
    const Window* d_window;
    //! CEGUI::Rect area for the current parse (may be 0).
    const Rectf* d_area;
    //! The final value from the last parse.
    float d_parsedValue;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIFalExpressionDim_impl_h_

