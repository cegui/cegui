/***********************************************************************
    filename:   CEGUIFalExpressionDim.h
    created:    Tue Dec 29 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIFalExpressionDim_h
#define _CEGUIFalExpressionDim_h_

#include "CEGUIFalDimensions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// forward declare implementation data struct (used internally only)
struct ExpressionDim_impl;

//! Dimension class that encapsulates a text based expression.
class CEGUIEXPORT ExpressionDim : public BaseDim
{
public:
    //! Constructor.
    ExpressionDim(const String& expression);
    //! Destructor.
    ~ExpressionDim();

    //! Set / change the expression string for this dimension.
    void setExpression(const String& expression);
    //! Return the current expression string for this dimension.
    const String& getExpression() const;

protected:
    // Implementation of the base class interface
    float getValue_impl(const Window& wnd) const;
    float getValue_impl(const Window& wnd, const Rect& container) const;
    void writeXMLElementName_impl(XMLSerializer& xml_stream) const;
    void writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const;
    BaseDim* clone_impl() const;

    //! function to perform the string scan.
    void scanString() const;
    //! function to clean the stack of scanned items.
    void cleanScanStack() const;

    //! ptr to struct holding implementation details.
    ExpressionDim_impl* d_pimpl;
};


} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIFalExpressionDim_h_

