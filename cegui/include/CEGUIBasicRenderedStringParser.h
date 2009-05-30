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
    A middle level string parser that is intended to be subclassed in order to
    implement actual creation of the RenderedStringComponent objects based upon
    control constructs extracted from the input string.
*/
class CEGUIEXPORT BasicRenderedStringParser : public RenderedStringParser
{
public:
    //! Constructor.
    BasicRenderedStringParser();
    //! Destructor.
    virtual ~BasicRenderedStringParser();
    
    // implement required interface from RenderedStringParser
    RenderedString parse(const String& input_string);

protected:
    //! append the text string \a text to the RenderedString \a rs.
    virtual void appendRenderedText(RenderedString& rs, const String& text) const;

    //! Process the control string \a ctrl_str.
    virtual void processControlString(RenderedString& rs, const String& ctrl_str);

    //! initialise the default state
    virtual void initialiseDefaultState();

    //! active padding values.
    Rect d_padding;
    //! active colour values.
    ColourRect d_colours;
    //! active font.
    String d_fontName;
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIBasicRenderedStringParser_h_
