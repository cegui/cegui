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
#include "CEGUIImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String BasicRenderedStringParser::ColourTagName("colour");
const String BasicRenderedStringParser::FontTagName("font");
const String BasicRenderedStringParser::ImageTagName("image");
const String BasicRenderedStringParser::WindowTagName("window");
const String BasicRenderedStringParser::VertAlignmentTagName("vert-alignment");
const String BasicRenderedStringParser::PaddingTagName("padding");
const String BasicRenderedStringParser::TopPaddingTagName("top-padding");
const String BasicRenderedStringParser::BottomPaddingTagName("bottom-padding");
const String BasicRenderedStringParser::LeftPaddingTagName("left-padding");
const String BasicRenderedStringParser::RightPaddingTagName("right-padding");
const String BasicRenderedStringParser::AspectLockTagName("aspect-lock");
const String BasicRenderedStringParser::ImageSizeTagName("image-size");
const String BasicRenderedStringParser::ImageWidthTagName("image-width");
const String BasicRenderedStringParser::ImageHeightTagName("image-height");
const String BasicRenderedStringParser::TopAlignedValueName("top");
const String BasicRenderedStringParser::BottomAlignedValueName("bottom");
const String BasicRenderedStringParser::CentreAlignedValueName("centre");
const String BasicRenderedStringParser::StretchAlignedValueName("stretch");

//----------------------------------------------------------------------------//
BasicRenderedStringParser::BasicRenderedStringParser() :
    d_initialColours(0xFFFFFFFF),
    d_vertAlignment(VF_BOTTOM_ALIGNED),
    d_imageSize(0, 0),
    d_aspectLock(false),
    d_initialised(false)
{
    initialiseDefaultState();
}

//----------------------------------------------------------------------------//
BasicRenderedStringParser::BasicRenderedStringParser(const String& initial_font,
                                            const ColourRect& initial_colours) :
    d_initialFontName(initial_font),
    d_initialColours(initial_colours),
    d_vertAlignment(VF_BOTTOM_ALIGNED),
    d_imageSize(0, 0),
    d_aspectLock(false),
    d_initialised(false)
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
    // first-time initialisation (due to issues with static creation order)
    if (!d_initialised)
        initialiseTagHandlers();

    initialiseDefaultState();

    // maybe override initial font.
    if (initial_font)
        d_fontName = initial_font->getName();

    // maybe override initial colours.
    if (initial_colours)
        d_colours = *initial_colours;

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
            rtc.setVerticalFormatting(d_vertAlignment);
            rtc.setAspectLock(d_aspectLock);
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
            "sense of control string '" + ctrl_str + "'.  Ignoring!");
        
        return;
    }

    char var_buf[128];
    char val_buf[128];
    sscanf(ctrl_str.c_str(), " %127[^ =] = '%127[^']", var_buf, val_buf);

    const String var_str(var_buf);
    const String val_str(val_buf);

    // look up handler function
    TagHandlerMap::iterator i = d_tagHandlers.find(var_str);
    // despatch handler, or log error
    if (i != d_tagHandlers.end())
        (this->*(*i).second)(rs, val_str);
    else
        Logger::getSingleton().logEvent(
            "BasicRenderedStringParser::processControlString: unknown "
            "control variable '" + var_str + "'.  Ignoring!");
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::initialiseDefaultState()
{
    d_padding = Rect<>(0, 0, 0, 0);
    d_colours = d_initialColours;
    d_fontName = d_initialFontName;
    d_imageSize.d_width = d_imageSize.d_height = 0.0f;
    d_vertAlignment = VF_BOTTOM_ALIGNED;
    d_aspectLock = false;
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
void BasicRenderedStringParser::initialiseTagHandlers()
{
    d_tagHandlers[ColourTagName] = &BasicRenderedStringParser::handleColour;
    d_tagHandlers[FontTagName] = &BasicRenderedStringParser::handleFont;
    d_tagHandlers[ImageTagName] = &BasicRenderedStringParser::handleImage;
    d_tagHandlers[WindowTagName] = &BasicRenderedStringParser::handleWindow;
    d_tagHandlers[VertAlignmentTagName] = &BasicRenderedStringParser::handleVertAlignment;
    d_tagHandlers[PaddingTagName] = &BasicRenderedStringParser::handlePadding;
    d_tagHandlers[TopPaddingTagName] = &BasicRenderedStringParser::handleTopPadding;
    d_tagHandlers[BottomPaddingTagName] = &BasicRenderedStringParser::handleBottomPadding;
    d_tagHandlers[LeftPaddingTagName] = &BasicRenderedStringParser::handleLeftPadding;
    d_tagHandlers[RightPaddingTagName] = &BasicRenderedStringParser::handleRightPadding;
    d_tagHandlers[AspectLockTagName] = &BasicRenderedStringParser::handleAspectLock;
    d_tagHandlers[ImageSizeTagName] = &BasicRenderedStringParser::handleImageSize;
    d_tagHandlers[ImageWidthTagName] = &BasicRenderedStringParser::handleImageWidth;
    d_tagHandlers[ImageHeightTagName] = &BasicRenderedStringParser::handleImageHeight;

    d_initialised = true;
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleColour(RenderedString& rs, const String& value)
{
    d_colours.setColours(PropertyHelper<Colour>::fromString(value));
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleFont(RenderedString& rs, const String& value)
{
    d_fontName = value;
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleImage(RenderedString& rs, const String& value)
{
    RenderedStringImageComponent ric(
        PropertyHelper<Image*>::fromString(value));
    ric.setPadding(d_padding);
    ric.setColours(d_colours);
    ric.setVerticalFormatting(d_vertAlignment);
    ric.setSize(d_imageSize);
    ric.setAspectLock(d_aspectLock);
    rs.appendComponent(ric);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleWindow(RenderedString& rs, const String& value)
{
    RenderedStringWidgetComponent rwc(value);
    rwc.setPadding(d_padding);
    rwc.setVerticalFormatting(d_vertAlignment);
    rwc.setAspectLock(d_aspectLock);
    rs.appendComponent(rwc);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleVertAlignment(RenderedString& rs, const String& value)
{
    if (value == TopAlignedValueName)
        d_vertAlignment = VF_TOP_ALIGNED;
    else if (value == BottomAlignedValueName)
        d_vertAlignment = VF_BOTTOM_ALIGNED;
    else if (value == CentreAlignedValueName)
        d_vertAlignment = VF_CENTRE_ALIGNED;
    else if (value == StretchAlignedValueName)
        d_vertAlignment = VF_STRETCHED;
    else
        Logger::getSingleton().logEvent(
            "BasicRenderedStringParser::handleVertAlignment: unknown "
            "vertical alignment '" + value + "'.  Ignoring!");
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handlePadding(RenderedString& rs,
                                              const String& value)
{
    d_padding = PropertyHelper<Rect<> >::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleTopPadding(RenderedString& rs,
                                                 const String& value)
{
    d_padding.d_min.d_y = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleBottomPadding(RenderedString& rs,
                                                    const String& value)
{
    d_padding.d_max.d_y = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleLeftPadding(RenderedString& rs,
                                                  const String& value)
{
    d_padding.d_min.d_x = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleRightPadding(RenderedString& rs,
                                                   const String& value)
{
    d_padding.d_max.d_x = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleAspectLock(RenderedString& rs,
                                                 const String& value)
{
    d_aspectLock = PropertyHelper<bool>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleImageSize(RenderedString& rs,
                                                const String& value)
{
    d_imageSize = PropertyHelper<Size<> >::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleImageWidth(RenderedString& rs,
                                                 const String& value)
{
    d_imageSize.d_width = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleImageHeight(RenderedString& rs,
                                                  const String& value)
{
    d_imageSize.d_height = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
