/***********************************************************************
    created:    25/05/2009
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
#ifndef _CEGUIFormattedRenderedString_h_
#define _CEGUIFormattedRenderedString_h_

#include "CEGUI/RenderedString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Root of a class hierarchy that wrap RenderedString objects and render them
    with additional formatting.
*/
class CEGUIEXPORT FormattedRenderedString
{
public:
    //! Destructor.
    virtual ~FormattedRenderedString();

    virtual void format(const Window* ref_wnd, const Sizef& area_size) = 0;
    virtual std::vector<GeometryBuffer*> createRenderGeometry(const Window* ref_wnd, const glm::vec2& position, const ColourRect* mod_colours, const Rectf* clip_rect) const = 0;
    virtual size_t getFormattedLineCount() const = 0;
    virtual float getHorizontalExtent(const Window* ref_wnd) const = 0;
    virtual float getVerticalExtent(const Window* ref_wnd) const = 0;

    //! set the RenderedString.
    void setRenderedString(const RenderedString& string);

    const RenderedString& getRenderedString() const;

    /*
    \brief
        During the last call to "format", was any word split between 2 or more
        lines?

        This can happen e.g. if word wrapping is used, and the width of a word
        is more than that of the area of the string.

    \see setWasWordSplit
    */
    bool wasWordSplit() const;

    /*
    \brief
        Set a flag which indicates whether in the last call to "format", any
        word split between 2 or more lines.

        See the documentation for "wasWordSplit" for more details.

    \see wasWordSplit
    */
    void setWasWordSplit(bool value);

    /*
    \brief
        Get the number of text lines in the original (non-formatted) string
        (i.e. "d_renderedString");

    \see getNumOfFormattedTextLines
    */
    std::size_t getNumOfOriginalTextLines() const;

    /*
    \brief
        Get the number of text lines in the formatted string.

        That takes into account e.g. word-wrapping.

    \see getNumOfOriginalTextLines
    */
    virtual std::size_t getNumOfFormattedTextLines() const;

protected:
    //! Constructor.
    FormattedRenderedString(const RenderedString& string);

    //! RenderedString that we handle formatting for.
    const RenderedString* d_renderedString;

private:
    /*
    \brief
        During the last call to "format", was any word split between 2 or more
        lines?

        See the documentation for "wasWordSplit" for more details.

    \see wasWordSplit
    \see setWasWordSplit
    */
    bool d_wasWordSplit;
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIFormattedRenderedString_h_
