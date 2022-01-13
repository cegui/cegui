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

#include "./Sizef.h"
#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class RenderedString;

/*!
\brief
    Root of a class hierarchy that wrap RenderedString objects and render them
    with additional formatting.
*/
class CEGUIEXPORT FormattedRenderedString
{
public:

    FormattedRenderedString(const RenderedString& string);
    virtual ~FormattedRenderedString() = default;

    virtual void format(const Window* refWnd, const Sizef& area_size) = 0;
    virtual std::vector<GeometryBuffer*> createRenderGeometry(const Window* refWnd, const glm::vec2& position, const ColourRect* mod_colours, const Rectf* clip_rect) const = 0;

    /*
    \brief
        Get the number of text lines in the formatted string.
        That takes into account e.g. word-wrapping.
    */
    virtual size_t getFormattedLineCount() const;

    //! set the RenderedString.
    void setRenderedString(const RenderedString& string) { d_renderedString = &string; }

    const RenderedString& getRenderedString() const { return *d_renderedString; }

    //! Get pixel width and height of the formatted text
    const Sizef& getExtent() const { return d_extent; }

    /*
    \brief
        During the last call to "format", was any word split between 2 or more
        lines?

        This can happen e.g. if word wrapping is used, and the width of a word
        is more than that of the area of the string.
    */
    virtual bool wasWordSplit() const { return false; }

protected:

    const RenderedString* d_renderedString = nullptr; //!< RenderedString that we handle formatting for.
    Sizef d_extent;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
