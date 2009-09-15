/***********************************************************************
    filename:   CEGUIBasicRenderedStringParser.cpp
    created:    28/05/2009
    author:     Paul Turner
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
#include "CEGUIBasicRenderedStringParser.h"
#include "CEGUIRenderedStringTextComponent.h"
#include "CEGUIRenderedStringImageComponent.h"
#include "CEGUIRenderedStringWidgetComponent.h"
#include "CEGUILogger.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIFont.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
BasicRenderedStringParser::BasicRenderedStringParser() :
    d_initialColours(0xFFFFFFFF)
{
    initialiseDefaultState();
}

//----------------------------------------------------------------------------//
BasicRenderedStringParser::BasicRenderedStringParser(const String& initial_font,
                                            const ColourRect& initial_colours) :
    d_initialFontName(initial_font),
    d_initialColours(initial_colours)
{
    initialiseDefaultState();
}

//----------------------------------------------------------------------------//
BasicRenderedStringParser::~BasicRenderedStringParser()
{
}

//----------------------------------------------------------------------------//
RenderedString BasicRenderedStringParser::parse(const String& input_string,
                                                Font* initial_font,
                                                const ColourRect* initial_colours)
{
    // set up initial font.
    if (initial_font)
        d_fontName = initial_font->getName();
    else
        d_fontName = d_initialFontName;

    // set up initial colours.
    if (initial_colours)
        d_colours = *initial_colours;
    else
        d_colours = d_initialColours;

    RenderedString rs;
    String curr_section;

    size_t curr_pos = 0;

    while (curr_pos < input_string.length())
    {
        size_t cstart_pos = input_string.find_first_of('[', curr_pos);

        // if no control sequence start char, add remaining text verbatim.
        if (String::npos == cstart_pos)
        {
            curr_section += input_string.substr(curr_pos);
            curr_pos = input_string.length();
        }
        else if (cstart_pos == curr_pos || input_string[cstart_pos - 1] != '\\')
        {
            // append everything up to the control start to curr_section.
            curr_section += input_string.substr(curr_pos, cstart_pos - curr_pos);

            // scan forward for end of control sequence
            size_t cend_pos = input_string.find_first_of(']', cstart_pos);
            // if not found, treat as plain text
            if (String::npos == cend_pos)
            {
                curr_section += input_string.substr(curr_pos);
                curr_pos = input_string.length();
            }
            // extract control string
            else
            {
                appendRenderedText(rs, curr_section);
                curr_section.clear();

                String ctrl_string(
                    input_string.substr(cstart_pos + 1,
                                        cend_pos - cstart_pos - 1));
                curr_pos = cend_pos + 1;

                processControlString(rs, ctrl_string);
                continue;
            }
        }
        else
        {
            curr_section += input_string.substr(curr_pos,
                                                cstart_pos - curr_pos - 1);
            curr_section += '[';
            curr_pos = cstart_pos + 1;
            continue;
        }

        appendRenderedText(rs, curr_section);
        curr_section.clear();
    }

    return rs;
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::appendRenderedText(RenderedString& rs,
                                                   const String& text) const
{
    size_t cpos = 0;
    // split the given string into lines based upon the newline character
    while (text.length() > cpos)
    {
        // find next newline
        const size_t nlpos = text.find('\n', cpos);
        // calculate length of this substring
        const size_t len =
            ((nlpos != String::npos) ? nlpos : text.length()) - cpos;

        // construct new text component and append it.
        if (len > 0)
        {
            RenderedStringTextComponent rtc(text.substr(cpos, len), d_fontName);
            rtc.setPadding(d_padding);
            rtc.setColours(d_colours);
            rs.appendComponent(rtc);
        }

        // break line if needed
        if (nlpos != String::npos)
            rs.appendLineBreak();

        // advance current position.  +1 to skip the \n char
        cpos += len + 1;
    }
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::processControlString(RenderedString& rs,
                                                     const String& ctrl_str)
{
    // all our default strings are of the form <var> = <val>
    // so do a quick check for the = char and abort if it's not there.
    if (String::npos == ctrl_str.find('='))
    {
        Logger::getSingleton().logEvent(
            "BasicRenderedStringParser::processControlString: unable to make "
            "sense of control string '" + ctrl_str + "'.  Ingoring!");
        
        return;
    }

    char var_buf[128];
    char val_buf[128];
    sscanf(ctrl_str.c_str(), " %127[^ =] = '%127[^']", var_buf, val_buf);

    String var_str(var_buf);
    String val_str(val_buf);

    if (var_str == "colour")
        d_colours.setColours(PropertyHelper::stringToColour(val_str));
    else if (var_str == "font")
        d_fontName = val_str;
    else if (var_str == "image")
    {
        RenderedStringImageComponent ric(
            PropertyHelper::stringToImage(val_str));
        ric.setPadding(d_padding);
        ric.setColours(d_colours);
        rs.appendComponent(ric);
    }
    else if (var_str == "window")
    {
        RenderedStringWidgetComponent rwc(val_str);
        rwc.setPadding(d_padding);
        rs.appendComponent(rwc);
    }
    else
        Logger::getSingleton().logEvent(
            "BasicRenderedStringParser::processControlString: unknown "
            "control variable '" + var_str + "'.  Ingoring!");

    // TODO: Add support for parsing vert formatting specifications.
    // TODO: Add support for parsing padding specifications.
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::initialiseDefaultState()
{
    d_padding = Rect(0, 0, 0, 0);
    d_colours = d_initialColours;
    d_fontName = d_initialFontName;
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::setInitialFontName(const String& font_name)
{
    d_initialFontName = font_name;
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::setInitialColours(const ColourRect& colours)
{
    d_initialColours = colours;
}

//----------------------------------------------------------------------------//
const String& BasicRenderedStringParser::getInitialFontName() const
{
    return d_initialFontName;
}

//----------------------------------------------------------------------------//
const ColourRect& BasicRenderedStringParser::getInitialColours() const
{
    return d_initialColours;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
