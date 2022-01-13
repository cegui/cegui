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
#include "./falagard/Enums.h"
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

    virtual ~FormattedRenderedString() = default;

    virtual HorizontalTextFormatting getCorrespondingFormatting() const = 0;

    //! \brief Calculate formatting data for the given string.
    virtual void format(const RenderedString& rs, const Window* refWnd, const Sizef& areaSize) = 0;

    /*
    \brief
        Create render geometry for the previously formatted string.

    \see format
    */
    virtual void createRenderGeometry(std::vector<GeometryBuffer*>& out, const Window* refWnd,
        const glm::vec2& position, const ColourRect* modColours, const Rectf* clipRect) const = 0;

    /*
    \brief
        Get the number of text lines in the formatted string.
        That takes into account e.g. word-wrapping.
    */
    virtual size_t getFormattedLineCount() const;

    /*
    \brief
        During the last call to "format", was any word split between lines?

        This can happen if word wrapping is used, and the width of a word
        is more than that of the area of the string.
    */
    virtual bool wasWordSplit() const { return false; }

    const RenderedString* getRenderedString() const { return d_renderedString; }

    //! Get pixel width and height of the formatted text
    const Sizef& getExtent() const { return d_extent; }

protected:

    const RenderedString* d_renderedString = nullptr; //!< RenderedString that we handle formatting for.
    Sizef d_extent;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
