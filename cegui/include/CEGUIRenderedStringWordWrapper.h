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
#include "CEGUIJustifiedRenderedString.h"
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
    //! Destructor.
    ~RenderedStringWordWrapper();

    // implementation of base interface
    void format(const Size& area_size);
    void draw(GeometryBuffer& buffer, const Vector2& position,
              const ColourRect* mod_colours, const Rect* clip_rect) const;
    size_t getFormattedLineCount() const;
    float getHorizontalExtent() const;
    float getVerticalExtent() const;

protected:
    //! Delete the current formatters and associated RenderedStrings
    void deleteFormatters();
    //! type of collection used to track the formatted lines.
    typedef std::vector<FormattedRenderedString*> LineList;
    //! collection of lines.
    LineList d_lines;
};

//! specialised version of format used with Justified text
template <> CEGUIEXPORT
void RenderedStringWordWrapper<JustifiedRenderedString>::format(const Size& area_size);

//----------------------------------------------------------------------------//
template <typename T>
RenderedStringWordWrapper<T>::RenderedStringWordWrapper(
        const RenderedString& string) :
    FormattedRenderedString(string)
{
}

//----------------------------------------------------------------------------//
template <typename T>
RenderedStringWordWrapper<T>::~RenderedStringWordWrapper()
{
    deleteFormatters();
}

//----------------------------------------------------------------------------//
template <typename T>
void RenderedStringWordWrapper<T>::format(const Size& area_size)
{
    deleteFormatters();

    RenderedString rstring, lstring;
    rstring = *d_renderedString;
    float rs_width;

    T* frs;

    for (size_t line = 0; line < rstring.getLineCount(); ++line)
    {
        while ((rs_width = rstring.getPixelSize(line).d_width) > 0)
        {
            // skip line if no wrapping occurs
            if (rs_width <= area_size.d_width)
                break;

            // split rstring at width into lstring and remaining rstring
            rstring.split(line, area_size.d_width, lstring);
            frs = new T(*new RenderedString(lstring));
            frs->format(area_size);
            d_lines.push_back(frs);
            line = 0;
        }
    }

    // last line.
    frs = new T(*new RenderedString(rstring));
    frs->format(area_size);
    d_lines.push_back(frs);
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
template <typename T>
void RenderedStringWordWrapper<T>::deleteFormatters()
{
    for (size_t i = 0; i < d_lines.size(); ++i)
    {
        // get the rendered string back from rthe formatter
        const RenderedString* rs = &d_lines[i]->getRenderedString();
        // delete the formatter
        delete d_lines[i];
        // delete the rendered string.
        delete rs;
    }

    d_lines.clear();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIRenderedStringWordWrapper_h_
