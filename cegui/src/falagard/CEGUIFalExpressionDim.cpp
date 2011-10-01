/***********************************************************************
    filename:   CEGUIFalExpressionDim.cpp
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
#include "../../include/CEGUI/falagard/ExpressionDim.h"
#include "CEGUI/ExpressionDim_impl.h"

#define YY_DECL int lexScan(CEGUI::ExpressionDim_impl* ctx)
#include "CEGUI/ExpressionDimScanner.h"

YY_DECL;

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
ExpressionDimParser::token_type ED_lex(ExpressionDimParser::semantic_type* yylval,
                                       ExpressionDim_impl* ctx)
{
    if (ctx->d_stack.size() <= ctx->d_stackPos)
        return ExpressionDimParser::token::EXP_END;

    const ExpressionPart p(ctx->d_stack[ctx->d_stackPos++]);

    if (p.first == ExpressionDimParser::token::DIMVAL)
        yylval->dim = p.second;

    return p.first;
}

//----------------------------------------------------------------------------//
ExpressionDim::ExpressionDim(const String& expression) :
    d_pimpl(new ExpressionDim_impl)
{
    d_pimpl->d_expression = expression;
}

//----------------------------------------------------------------------------//
ExpressionDim::~ExpressionDim()
{
    cleanScanStack();
    delete d_pimpl;
}

//----------------------------------------------------------------------------//
void ExpressionDim::setExpression(const String& expression)
{
    d_pimpl->d_expression = expression;
    d_pimpl->d_performedScan = false;
}

//----------------------------------------------------------------------------//
const String& ExpressionDim::getExpression() const
{
    return d_pimpl->d_expression;
}

//----------------------------------------------------------------------------//
float ExpressionDim::getValue_impl(const Window& wnd) const
{
    if (!d_pimpl->d_performedScan)
        scanString();

    d_pimpl->d_window = &wnd;
    d_pimpl->d_area = 0;
    d_pimpl->d_stackPos = 0;
    d_pimpl->d_parser.parse();

    return d_pimpl->d_parsedValue;
}

//----------------------------------------------------------------------------//
float ExpressionDim::getValue_impl(const Window& wnd,
                                   const Rect& container) const
{
    if (!d_pimpl->d_performedScan)
        scanString();

    d_pimpl->d_window = &wnd;
    d_pimpl->d_area = &container;
    d_pimpl->d_stackPos = 0;
    d_pimpl->d_parser.parse();

    return d_pimpl->d_parsedValue;
}

//----------------------------------------------------------------------------//
void ExpressionDim::writeXMLElementName_impl(XMLSerializer& xml_stream) const
{
    xml_stream.openTag("ExpressionDim");
}

//----------------------------------------------------------------------------//
void ExpressionDim::writeXMLElementAttributes_impl(
        XMLSerializer& xml_stream) const
{
    xml_stream.attribute("value", d_pimpl->d_expression);
}

//----------------------------------------------------------------------------//
BaseDim* ExpressionDim::clone_impl() const
{
    return new ExpressionDim(d_pimpl->d_expression);
}

//----------------------------------------------------------------------------//
void ExpressionDim::scanString() const
{
    // prior to scanning, we must clear out any old tokens
    cleanScanStack();

    YY_BUFFER_STATE bs = CEGUI_yy_scan_string(d_pimpl->d_expression.c_str());
    lexScan(d_pimpl);
    CEGUI_yy_delete_buffer(bs);

    d_pimpl->d_performedScan = true;
}

//----------------------------------------------------------------------------//
void ExpressionDim::cleanScanStack() const
{
    ExpressionStack::iterator i(d_pimpl->d_stack.begin());
    for ( ; i != d_pimpl->d_stack.end(); ++i)
        delete (*i).second;

    d_pimpl->d_stack.clear();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

