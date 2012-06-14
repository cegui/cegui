/***********************************************************************
    filename:   CEGUIFalExpressionDimScanner.l
    created:    Mon Dec 28 2009
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
%option header-file="ExpressionDimScanner.h"
%option outfile="ExpressionDimScanner.cpp"
%option prefix="CEGUI_yy"
%option noyywrap
%option batch
%option 8bit
%option never-interactive

%{
#include "./ExpressionDim_impl.h"
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/Logger.h"

#define YY_DECL int lexScan(CEGUI::ExpressionDim_impl* ctx)

void prepareString(char* str);
void pushDim(CEGUI::ExpressionDim_impl* ctx, CEGUI::BaseDim* dim);
char* parse_single_quoted_string(char* in, char* out, size_t out_size);
%}

SPACING     [[:space:]]*
DECIMAL     [[:space:]]*-?[0-9]+\.?[0-9]*[[:space:]]*
SQSTRING    [[:space:]]*'(\\.|[^'])*'[[:space:]]*
UQARG       [[:space:]]*[^'",)]+[[:space:]]*

%%
{DECIMAL} {
    CEGUI::AbsoluteDim* dim = new CEGUI::AbsoluteDim(atof(yytext));
    pushDim(ctx, dim);
}

"udim"\({DECIMAL},{DECIMAL},{UQARG}\) {
    float scale, offset;
    char type[32];

    prepareString(yytext);
    sscanf(&yytext[5], " %g %g %31s ", &scale, &offset, type);

    CEGUI::UnifiedDim* dim = new CEGUI::UnifiedDim(
        CEGUI::UDim(scale, offset),
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"imagedim"\({SQSTRING},{UQARG}\) {
    char image[256];
    char type[32];

    char* in = &yytext[9];
    in = parse_single_quoted_string(in, image, 256);

    prepareString(in);
    sscanf(in, " %31s ", type);

    CEGUI::ImageDim* dim = new CEGUI::ImageDim(
        image,
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"imagepropertydim"\({SQSTRING},{UQARG}\) {
    char property[256];
    char type[32];

    char* in = &yytext[9];
    in = parse_single_quoted_string(in, property, 256);

    prepareString(in);
    sscanf(in, " %31s ", type);

    CEGUI::ImagePropertyDim* dim =
        new CEGUI::ImagePropertyDim(
            property,
            CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"widgetdim"\({SQSTRING},{UQARG}\) {
    char widget[256];
    char type[32];

    char* in = &yytext[10];
    in = parse_single_quoted_string(in, widget, 256);

    prepareString(in);
    sscanf(in, " %31s ", type);

    CEGUI::WidgetDim* dim = new CEGUI::WidgetDim(
        widget,
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"widgetdim"\({UQARG}\) {
    char type[32];

    prepareString(yytext);
    sscanf(&yytext[10], " %31s ", type);

    CEGUI::WidgetDim* dim = new CEGUI::WidgetDim(
        CEGUI::String(""),
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"fontdim"\({SQSTRING},{SQSTRING},{UQARG},{DECIMAL}\) {
    char widget[256];
    char font[128];
    char type[32];
    float padding;

    char* in = &yytext[8];
    in = parse_single_quoted_string(in, widget, 256);
    in = parse_single_quoted_string(in, font, 128);

    prepareString(in);
    sscanf(in, " %31s %g ", type, &padding);

    CEGUI::FontDim* dim = new CEGUI::FontDim(
        widget,
        font,
        CEGUI::String(""),
        CEGUI::FalagardXMLHelper::stringToFontMetricType(type),
        padding);

    pushDim(ctx, dim);
}

"fontdim"\({UQARG},{DECIMAL}\) {
    char type[32];
    float padding;

    prepareString(yytext);
    sscanf(&yytext[8], " %31s %g ", type, &padding);

    CEGUI::FontDim* dim = new CEGUI::FontDim(
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::FalagardXMLHelper::stringToFontMetricType(type),
        padding);

    pushDim(ctx, dim);
}

"fontdim"\({UQARG}\) {
    char type[32];

    prepareString(yytext);
    sscanf(&yytext[8], " %31s ", type);

    CEGUI::FontDim* dim = new CEGUI::FontDim(
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::FalagardXMLHelper::stringToFontMetricType(type),
        0.0f);

    pushDim(ctx, dim);
}

"stringextent"\({SQSTRING},{SQSTRING}\) {
    char text_string[512];
    char font[128];

    char* in = &yytext[13];
    in = parse_single_quoted_string(in, text_string, 512);
    parse_single_quoted_string(in, font, 128);

    CEGUI::FontDim* dim = new CEGUI::FontDim(
        CEGUI::String(""),
        CEGUI::String(font),
        CEGUI::String(text_string),
        CEGUI::FMT_HORZ_EXTENT,
        0.0f);

    pushDim(ctx, dim);
}

"stringextent"\({SQSTRING}\) {
    char text_string[512];
    char* in = &yytext[13];
    parse_single_quoted_string(in, text_string, 512);

    CEGUI::FontDim* dim = new CEGUI::FontDim(
        CEGUI::String(""),
        CEGUI::String(""),
        CEGUI::String(text_string),
        CEGUI::FMT_HORZ_EXTENT,
        0.0f);

    pushDim(ctx, dim);
}

"propertydim"\({SQSTRING},{SQSTRING},{UQARG}\) {
    char widget[256];
    char property[256];
    char type[32];
    char* in = &yytext[12];

    in = parse_single_quoted_string(in, widget, 256);
    in = parse_single_quoted_string(in, property, 256);
    prepareString(in);
    sscanf(in, " %31s ", type);

    CEGUI::PropertyDim* dim = new CEGUI::PropertyDim(
        widget,
        property,
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"propertydim"\({SQSTRING},{UQARG}\) {
    char property[256];
    char type[32];
    char* in = &yytext[12];

    in = parse_single_quoted_string(in, property, 256);
    prepareString(in);
    sscanf(in, " %31s ", type);

    CEGUI::PropertyDim* dim = new CEGUI::PropertyDim(
        CEGUI::String(""),
        property,
        CEGUI::FalagardXMLHelper::stringToDimensionType(type));

    pushDim(ctx, dim);
}

"propertydim"\({SQSTRING}\) {
    char property[256];
    char* in = &yytext[12];
    parse_single_quoted_string(in, property, 256);

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

char* parse_single_quoted_string(char* in, char* out, size_t out_size)
{
    --out_size;
    size_t out_cnt = 0;

    char* const orig_in = in;
    in = strchr(in, '\'');

    if (!in)
    {
        *out = 0;
        return orig_in;
    }

    ++in;

    char c;
    while (out_cnt < out_size && (c = *in++))
    {
        if (c == '\'')
            break;

        if ((c == '\\') && !(c = *in++))
            break;

        *out++ = c;
        ++out_cnt;
    }

    *out = 0;

    return in;
}

