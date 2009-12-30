/***********************************************************************
    filename:   CEGUIFalExpressionDimParser.y
    created:    Mon Dec 28 2009
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
%skeleton "lalr1.cc"
%require "2.3"
%output="CEGUIFalExpressionDimParser.cpp"
%name-prefix="CEGUI::ED_"
%define "namespace" "CEGUI"
%define "parser_class_name" "ExpressionDimParser"
%defines

%{
#include "../../include/falagard/CEGUIFalDimensions.h"
#include "../../include/CEGUILogger.h"

namespace CEGUI {
struct ExpressionDim_impl;
}
%}

%parse-param { ExpressionDim_impl* ctx }
%lex-param { CEGUI::ExpressionDim_impl* ctx }

%union {
    float val;
    BaseDim* dim;
}

%token DIMVAL
%token EXP_END 0

%left '-' '+'
%left '*' '/'

%{
namespace CEGUI {
ExpressionDimParser::token_type ED_lex(ExpressionDimParser::semantic_type* yylval,
                                       ExpressionDim_impl* ctx);

}
#include "CEGUIFalExpressionDim_impl.h"
%}

%%
input:  exp   { ctx->d_parsedValue = $<val>1; }
;

exp:    DIMVAL {
        $<val>$ = ctx->d_area ?
                    $<dim>1->getValue(*ctx->d_window, *ctx->d_area) :
                    $<dim>1->getValue(*ctx->d_window); }
        | exp '+' exp       { $<val>$ = $<val>1 + $<val>3;      }
        | exp '-' exp       { $<val>$ = $<val>1 - $<val>3;      }
        | exp '*' exp       { $<val>$ = $<val>1 * $<val>3;      }
        | exp '/' exp       { $<val>$ = $<val>1 / $<val>3;      }
        | '(' exp ')'       { $<val>$ = $<val>2                 }
;
%%

namespace CEGUI
{
// Implementation of parser error handler function.
// To throw, or not to throw; that is the question ;)
void ExpressionDimParser::error(const ExpressionDimParser::location_type& l,
                                const std::string& m)
{
    Logger::getSingleton().logEvent("ExpressionDim::parser: Error parsing "
        "expression.  Additional Information: " + String(m.c_str()), Errors);
}

}

