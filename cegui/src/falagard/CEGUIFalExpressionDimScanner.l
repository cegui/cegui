/***********************************************************************
    filename:   CEGUIFalExpressionDimScanner.l
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
%option header-file="CEGUIFalExpressionDimScanner.h"
%option outfile="CEGUIFalExpressionDimScanner.cpp"
%option prefix="CEGUI_yy"
%option noyywrap
%option batch
%option 8bit

%{
#include "CEGUIFalExpressionDimParser.hpp"
#include "CEGUIFalExpressionDim_impl.h"
#include "../../include/falagard/CEGUIFalXMLEnumHelper.h"
#include "../../include/CEGUILogger.h"

#define YY_DECL int lexScan(CEGUI::ExpressionDim_impl* ctx)

void prepareString(char* str);
void pushDim(CEGUI::ExpressionDim_impl* ctx, CEGUI::BaseDim* dim);
%}

VALUE       -?[0-9]+\.?[0-9]*
SPACING     [ \t]*

%%
{VALUE} {
    CEGUI::AbsoluteDim* dim = new CEGUI::AbsoluteDim(atof(yytext));
    pushDim(ctx, dim);
}

"udim("{SPACING}{VALUE}{SPACING}","{SPACING}{VALUE}{SPACING}","{SPACING}[^)]+{SPACING}")" {
    prepareString(yytext);

    float scale, offset;
    char type[32];

    sscanf(&yytext[4], "( %g %g %31s )", &scale, &offset, type);

    CEGUI::UnifiedDim* dim = new CEGUI::UnifiedDim(
        CEGUI::UDim(scale, offset),
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"imagedim("[^,)]+","[^,)]+","[^,)]+")" {
    prepareString(yytext);

    char imageset[128];
    char image[128];
    char type[32];

    sscanf(&yytext[8], "( %127s %127s %31s )", imageset, image, type);

    CEGUI::ImageDim* dim = new CEGUI::ImageDim(
        imageset,
        image,
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"widgetdim("[^,)]+","[^),]+")" {
    prepareString(yytext);

    char widget[256];
    char type[32];

    sscanf(&yytext[9], "( %255s %31s )", widget, type);

    CEGUI::WidgetDim* dim = new CEGUI::WidgetDim(
        widget,
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"widgetdim("[^,)]+")" {
    prepareString(yytext);

    char type[32];

    sscanf(&yytext[9], "( %31s )", type);

    CEGUI::WidgetDim* dim = new CEGUI::WidgetDim(
        CEGUI::String(""),
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"fontdim("[^,)]+","[^,)]+","[^,)]+","[^,)]+")" {
    prepareString(yytext);

    char widget[256];
    char font[128];
    char type[32];
    float padding;

    sscanf(&yytext[7], "( %255s %127s %31s %g )",
           widget, font, type, &padding);

    CEGUI::FontDim* dim = new CEGUI::FontDim(
        widget,
        font,
        CEGUI::String(""),
        CEGUI::FalagardXMLHelper::stringToFontMetricType(type),
        padding);

    pushDim(ctx, dim);
}

"fontdim("[^,)]+","[^,)]+")" {
    prepareString(yytext);

    char type[32];
    float padding;

    sscanf(&yytext[7], "( %31s %g )", type, &padding);

    CEGUI::FontDim* dim = new CEGUI::FontDim(
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::FalagardXMLHelper::stringToFontMetricType(type),
        padding);

    pushDim(ctx, dim);
}

"fontdim("[^,)]+")" {
    prepareString(yytext);

    char type[32];

    sscanf(&yytext[7], "( %31s )", type);

    CEGUI::FontDim* dim = new CEGUI::FontDim(
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::FalagardXMLHelper::stringToFontMetricType(type),
        0.0f);

    pushDim(ctx, dim);
}

"propertydim("[^,)]+","[^,)]+","[^,)]+")" {
    prepareString(yytext);

    char widget[256];
    char property[128];
    char type[32];

    sscanf(&yytext[11], "( %255s %127s %31s )", widget, property, type);

    CEGUI::PropertyDim* dim = new CEGUI::PropertyDim(
        widget,
        property,
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"propertydim("[^,)]+","[^,)]+")" {
    prepareString(yytext);

    char property[128];
    char type[32];

    sscanf(&yytext[11], "( %127s %31s )", property, type);

    CEGUI::PropertyDim* dim = new CEGUI::PropertyDim(
        CEGUI::String(""),
        property,
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"propertydim("[^,)]+")" {
    prepareString(yytext);

    char property[128];

    sscanf(&yytext[11], "( %127s )", property);

    CEGUI::PropertyDim* dim = new CEGUI::PropertyDim(
        CEGUI::String(""),
        property,
        CEGUI::DT_INVALID);

    pushDim(ctx, dim);
}

"+"|"-"|"*"|"/"|"("|")"     {
    ctx->d_stack.push_back(
        std::make_pair(
            static_cast<CEGUI::ExpressionDimParser::token_type>(yytext[0]),
            static_cast<CEGUI::BaseDim*>(0)));
}

[ \t\n\r\f]+ 

. {
    CEGUI::Logger::getSingleton().logEvent("ExpressionDim::scanString: An "
        "error has occurred during lexical analysis of the expression.  "
        "Additional information: " + CEGUI::String(yytext), CEGUI::Errors);
}
%%

/* A couple of helper funcs to save repetition above */
void prepareString(char* str)
{
    const int sl = strlen(str);
    // replace commas with spaces
    for (int i = 0; i < sl; ++i)
        if (str[i] == ',')
            str[i] = ' ';

    // replace trailing ')' with space
    if (str[sl - 1] == ')')
        str[sl - 1] = ' ';
}

void pushDim(CEGUI::ExpressionDim_impl* ctx, CEGUI::BaseDim* dim)
{
    ctx->d_stack.push_back(
        std::make_pair(CEGUI::ExpressionDimParser::token::DIMVAL, dim));
}
