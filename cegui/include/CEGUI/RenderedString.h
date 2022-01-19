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
#ifndef _CEGUIRenderedString_h_
#define _CEGUIRenderedString_h_

#include "CEGUI/Base.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/DefaultParagraphDirection.h"
#include <glm/glm.hpp>
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class Rectf;
using RenderedStringComponentPtr = std::unique_ptr<class RenderedStringComponent>;

struct RenderedGlyph
{
    //pointer to component? store type to avoid dynamic cast / virtualization?
    // union FontGlyph / ImageElement / WidgetElement + type?

    size_t originalIndex;
    glm::vec2 offset;
    Sizef advance;
    // ColorRect - get from element?

    bool isJustifyable : 1;
    bool isBreakable : 1;
    bool isRightToLeft : 1;
};

struct RenderedParagraph
{
    std::vector<RenderedGlyph> glyphs;
    Sizef extents;
    uint16_t justifyableSpaceCount = 0;
    DefaultParagraphDirection bidiDir = DefaultParagraphDirection::Automatic;
};

/*!
\brief
    Class representing a rendered string of entities.

    Here 'string' does not refer solely to a text string, rather a string of
    any renderable items.
*/
class CEGUIEXPORT RenderedString
{
public:

    RenderedString();
    RenderedString(const RenderedString& other) = delete;
    RenderedString(RenderedString&&) noexcept;
    virtual ~RenderedString();

    RenderedString& operator =(const RenderedString& rhs) = delete; // Use clone()
    RenderedString& operator =(RenderedString&&) noexcept;

    bool renderText(const String& text, RenderedStringParser* parser = nullptr, Font* defaultFont = nullptr,
        DefaultParagraphDirection defaultParagraphDir = DefaultParagraphDirection::LeftToRight);

    /*!
    \brief
        Draw the string to a GeometryBuffer.

    \param refWnd
        A pointer to a reference Window used to retrieve certain attributes if needed. 

    \param line
        The line of the RenderedString to draw.

    \param position
        Vector2 describing the position where the RenderedString is to be drawn.
        Note that this is not the final onscreen position, but the position as
        offset from the top-left corner of the entity represented by the
        GeometryBuffer.

    \param mod_colours
        Pointer to a ColourRect describing colour values that are to be
        modulated with the any stored colour values to calculate the final
        colour values to be used.  This may be 0 if no modulated colours are
        required.  NB: Each specific component will decide if and how it will
        apply the modulated colours.

    \param clip_rect
        Pointer to a Rect object that describes a clipping rectangle that should
        be used when drawing the RenderedString.  This may be 0 if no clipping
        is required.

    \param space_extra
        float value indicating additional padding value to be applied to space
        characters in the string.

    \return
        Returns the created GeometryBuffer objects needed for rendering this object.

    \exception InvalidRequestException
        thrown if \a line is out of range.
    */
    void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const Window* refWnd, size_t line,
        const glm::vec2& position, const ColourRect* mod_colours,
        const Rectf* clip_rect, float space_extra) const;

    //! append \a component to the list of components drawn for this string.
    void appendComponent(const RenderedStringComponent& component);

    //! clear the list of components drawn for this string.
    void clearComponents();

    //! linebreak the rendered string at the present position.
    void appendLineBreak();

    //! return the number of components that make up this string.
    size_t getComponentCount() const { return d_components.size(); }

    //! return number of lines in this string.
    size_t getLineCount() const { return d_lines.size(); }

    //! return the total number of spacing characters in the specified line.
    size_t getSpaceCount(const size_t line) const;

    /*!
    \brief
        Return the pixel size of a specified line for the RenderedString.

    \param line
        The line number whose size is to be returned.

    \return
        Size object describing the size of the rendered output of the specified
        line of this RenderedString, in pixels.

    \exception InvalidRequestException
        thrown if \a line is out of range.
    */
    Sizef getLineExtent(const Window* refWnd, const size_t line) const;

    /*!
    \brief
        Return the pixel size of a whole text for the RenderedString.

    \return
        Size object describing the size of the rendered output of the whole
        text of this RenderedString, in pixels.
    */
    Sizef getExtent(const Window* refWnd) const;

    //! set selection highlight
    void setSelection(const Window* refWnd, float start, float end);

    /*!
    \brief
        split the string in line \a line as close to \a splitPoint as possible.

        The RenderedString \a left will receive the left portion of the split,
        while the right portion of the split will remain in this RenderedString.

    \param line
        The line number on which the split is to occur.

    \param splitPoint
        float value specifying the pixel location where the split should occur.
        The actual split will occur as close to this point as possible, though
        preferring a shorter 'left' portion when the split can not be made
        exactly at the requested point.

    \param left
        RenderedString object that will receieve the left portion of the split.
        Any existing content in the RenderedString is replaced.

    \return
        True if any word was split into 2 or more lines, because it couldn't fit
        in a single line?

    \exception InvalidRequestException
        thrown if \a line is out of range.
    */
    bool split(const Window* refWnd, const size_t line, float splitPoint, RenderedString& left);

    //! Explicit cloning method. Used instead of copy constructor and assignment operator.
    RenderedString clone() const;

protected:

    std::vector<RenderedParagraph> d_paragraphs;

    std::vector<RenderedStringComponentPtr> d_components;
    std::vector<std::pair<size_t, size_t>> d_lines; // first is component idx, second is component count.
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif
