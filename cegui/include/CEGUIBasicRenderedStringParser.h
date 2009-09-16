/***********************************************************************
    filename:   CEGUIBasicRenderedStringParser.h
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

#include "CEGUIRenderedStringParser.h"

#include "CEGUIRect.h"
#include "CEGUIColourRect.h"
#include "CEGUIString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Basic RenderedStringParser class that offers support for 'colour', 'font',
    'image' and 'window' control tags.
*/
class CEGUIEXPORT BasicRenderedStringParser : public RenderedStringParser
{
public:
    //! Constructor.
    BasicRenderedStringParser();
    /*!
    \brief
        Initialising constructor.

    \param initial_font
        Reference to a String holding the name of the initial font to be used.

    \param initial_colours
        Reference to a ColourRect describing the initial colours to be used.
    */
    BasicRenderedStringParser(const String& initial_font,
                              const ColourRect& initial_colours);
    //! Destructor.
    virtual ~BasicRenderedStringParser();

    /*!
    \brief
        set the initial font name to be used on subsequent calls to parse.

    \param font_name
        String object holding the name of the font.
    */
    void setInitialFontName(const String& font_name);

    /*!
    \brief
        Set the initial colours to be used on subsequent calls to parse.

    \param colours
        ColourRect object holding the colours.
    */
    void setInitialColours(const ColourRect& colours);

    /*!
    \brief
        Return the name of the initial font used in each parse.
    */
    const String& getInitialFontName() const;

    /*!
    \brief
        Return a ColourRect describing the initial colours used in each parse.
    */
    const ColourRect& getInitialColours() const;

    // implement required interface from RenderedStringParser
    RenderedString parse(const String& input_string,
                         Font* initial_font,
                         const ColourRect* initial_colours);

protected:
    //! append the text string \a text to the RenderedString \a rs.
    virtual void appendRenderedText(RenderedString& rs, const String& text) const;

    //! Process the control string \a ctrl_str.
    virtual void processControlString(RenderedString& rs, const String& ctrl_str);

    //! initialise the default state
    virtual void initialiseDefaultState();

    //! initial font name
    String d_initialFontName;
    //! initial colours
    ColourRect d_initialColours;
    //! active padding values.
    Rect d_padding;
    //! active colour values.
    ColourRect d_colours;
    //! active font.
    String d_fontName;

};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIBasicRenderedStringParser_h_
