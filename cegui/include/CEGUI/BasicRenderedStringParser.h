/***********************************************************************
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
#ifndef _CEGUIBasicRenderedStringParser_h_
#define _CEGUIBasicRenderedStringParser_h_

#include "CEGUI/RenderedStringParser.h"

#include "CEGUI/Rectf.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/String.h"
#include "CEGUI/falagard/Enums.h"
#include <unordered_map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Basic RenderedStringParser class that offers support for the following tags:
    - 'colour' value is a CEGUI colour property value.
    - 'font' value is the name of a font.
    - 'image' value is a CEGUI image property value.
    - 'window' value is the name of a window.
    - 'vert-alignment' value is either top, bottom, centre or stretch.
    - 'padding' value is a CEGUI Rect property value.
    - 'top-padding' value is a float.
    - 'bottom-padding' value is a float.
    - 'left-padding' value is a float.
    - 'right-padding' value is a float.
    - 'image-size' value is a CEGUI size property value.
    - 'image-width' value is a float.
    - 'image-height' value is a float.
    - 'aspect-lock' value is a boolean (NB: this currently has no effect).
*/
class CEGUIEXPORT BasicRenderedStringParser : public RenderedStringParser
{
public:
    // Strings for supported tags
    static const String ColourTagName;
    static const String FontTagName;
    static const String ImageTagName;
    static const String WindowTagName;
    static const String VertImageFormattingTagName;
    static const String VertTextFormattingTagName;
    static const String PaddingTagName;
    static const String TopPaddingTagName;
    static const String BottomPaddingTagName;
    static const String LeftPaddingTagName;
    static const String RightPaddingTagName;
    static const String ImageSizeTagName;
    static const String ImageWidthTagName;
    static const String ImageHeightTagName;

    //! Constructor.
    BasicRenderedStringParser();
    //! Destructor.
    virtual ~BasicRenderedStringParser();

    // implement required interface from RenderedStringParser
    RenderedString parse(const String& input_string,
                         const Font* active_font,
                         const ColourRect* active_colours) override;

protected:
    //! append the text string \a text to the RenderedString \a rs.
    virtual void appendRenderedText(RenderedString& rs, const String& text) const;

    //! Process the control string \a ctrl_str.
    virtual void processControlString(RenderedString& rs, const String& ctrl_str);

    //! initialise the default state
    virtual void initialiseDefaultState();

    //! initialise tag handlers
    void initialiseTagHandlers();

    //! handlers for the various tags supported
    void handleColour(RenderedString& rs, const String& value);
    void handleFont(RenderedString& rs, const String& value);
    void handleImage(RenderedString& rs, const String& value);
    void handleWindow(RenderedString& rs, const String& value);
    void handleVertImageFormatting(RenderedString& rs, const String& value);
    void handleVertTextFormatting(RenderedString& rs, const String& value);
    void handlePadding(RenderedString& rs, const String& value);
    void handleTopPadding(RenderedString& rs, const String& value);
    void handleBottomPadding(RenderedString& rs, const String& value);
    void handleLeftPadding(RenderedString& rs, const String& value);
    void handleRightPadding(RenderedString& rs, const String& value);
    void handleImageSize(RenderedString& rs, const String& value);
    void handleImageWidth(RenderedString& rs, const String& value);
    void handleImageHeight(RenderedString& rs, const String& value);

    //! active padding values.
    Rectf d_padding;
    //! active colour values.
    ColourRect d_colours;
    //! active font.
    String d_fontName;
    //! active vertical image formatting
    VerticalImageFormatting d_vertImageFormatting = VerticalImageFormatting::BottomAligned;
    //! active vertical image formatting
    VerticalTextFormatting d_vertTextFormatting = VerticalTextFormatting::BottomAligned;
    //! active image size
    Sizef d_imageSize;

    //! true if handlers have been registered
    bool d_initialised;
    //! definition of type used for handler functions
    typedef void (BasicRenderedStringParser::*TagHandler)(RenderedString&,
                                                          const String&);
    //! definition of type used to despatch tag handler functions
    typedef std::unordered_map<String, TagHandler> TagHandlerMap;
    //! Collection to map tag names to their handler functions.
    TagHandlerMap d_tagHandlers;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _CEGUIBasicRenderedStringParser_h_
