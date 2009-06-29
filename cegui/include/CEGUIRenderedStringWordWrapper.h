/***********************************************************************
    filename:   CEGUIRenderedStringWordWrapper.h
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
#ifndef _CEGUIRenderedStringWordWrapper_h_
#define _CEGUIRenderedStringWordWrapper_h_

#include "CEGUIFormattedRenderedString.h"
#include "CEGUIVector.h"
#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class that handles wrapping of a rendered string into sub-strings.  Each
    sub-string is rendered using the FormattedRenderedString based class 'T'.
*/
template <typename T>
class RenderedStringWordWrapper : public FormattedRenderedString
{
public:
    //! Constructor.
    RenderedStringWordWrapper(const RenderedString& string);

    // implementation of base interface
    void format(const Size& area_size);
    void draw(GeometryBuffer& buffer, const Vector2& position,
              const ColourRect* mod_colours, const Rect* clip_rect) const;
    size_t getFormattedLineCount() const;
    float getHorizontalExtent() const;
    float getVerticalExtent() const;

protected:
    //! type of collection used to track the formatted lines.
    typedef std::vector<T*> LineList;
    //! collection of lines.
    LineList d_lines;
};

//----------------------------------------------------------------------------//
template <typename T>
RenderedStringWordWrapper<T>::RenderedStringWordWrapper(
        const RenderedString& string) :
    FormattedRenderedString(string)
{
}

//----------------------------------------------------------------------------//
template <typename T>
void RenderedStringWordWrapper<T>::format(const Size& area_size)
{
    // delete existing formatters
    for (size_t i = 0; i < d_lines.size(); ++i)
        delete d_lines[i];
    d_lines.clear();

    RenderedString rstring, lstring;
    rstring = d_renderedString;
    float rs_width;
    
    while ((rs_width = rstring.getPixelSize().d_width) > 0)
    {
        T* frs;

        // case where no wrapping occurs
        if (rs_width <= area_size.d_width)
        {
            frs = new T(rstring);
            frs->format(area_size);
            d_lines.push_back(frs);
            return;
        }

        // split rstring at width into lstring and remaining rstring
        rstring.split(area_size.d_width, lstring);
        frs = new T(lstring);
        frs->format(area_size);
        d_lines.push_back(frs);
    }
}

//----------------------------------------------------------------------------//
template <typename T>
void RenderedStringWordWrapper<T>::draw(GeometryBuffer& buffer,
                                     const Vector2& position,
                                     const ColourRect* mod_colours,
                                     const Rect* clip_rect) const
{
    Vector2 line_pos(position);
    typename LineList::const_iterator i = d_lines.begin();
    for (; i != d_lines.end(); ++i)
    {
        (*i)->draw(buffer, line_pos, mod_colours, clip_rect);
        line_pos.d_y += (*i)->getVerticalExtent();
    }
}

//----------------------------------------------------------------------------//
template <typename T>
size_t RenderedStringWordWrapper<T>::getFormattedLineCount() const
{
    return d_lines.size();
}

//----------------------------------------------------------------------------//
template <typename T>
float RenderedStringWordWrapper<T>::getHorizontalExtent() const
{
    // TODO: Cache at format time.

    float w = 0;
    typename LineList::const_iterator i = d_lines.begin();
    for (; i != d_lines.end(); ++i)
    {
        const float cur_width = (*i)->getHorizontalExtent();
        if (cur_width > w)
            w = cur_width;
    }
    
    return w;
}

//----------------------------------------------------------------------------//
template <typename T>
float RenderedStringWordWrapper<T>::getVerticalExtent() const
{
    // TODO: Cache at format time.

    float h = 0;
    typename LineList::const_iterator i = d_lines.begin();
    for (; i != d_lines.end(); ++i)
        h += (*i)->getVerticalExtent();

    return h;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIRenderedStringWordWrapper_h_
