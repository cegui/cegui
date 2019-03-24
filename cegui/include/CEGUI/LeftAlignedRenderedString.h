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
#ifndef _CEGUILeftAlignedRenderedString_h_
#define _CEGUILeftAlignedRenderedString_h_

#include "CEGUI/FormattedRenderedString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    FormattedRenderedString implementation that renders the RenderedString with
    left aligned formatting.
*/
class CEGUIEXPORT LeftAlignedRenderedString : public FormattedRenderedString
{
public:
    //! Constructor.
    LeftAlignedRenderedString(const RenderedString& string);

    // implementation of base interface
    void format(const Window* ref_wnd, const Sizef& area_size) override;
    std::vector<GeometryBuffer*> createRenderGeometry(
        const Window* ref_wnd,
        const glm::vec2& position, const ColourRect* mod_colours,
        const Rectf* clip_rect) const override;
    size_t getFormattedLineCount() const override;
    float getHorizontalExtent(const Window* ref_wnd) const override;
    float getVerticalExtent(const Window* ref_wnd) const override;
};
    
} // End of  CEGUI namespace section

#endif // end of guard _CEGUILeftAlignedRenderedString_h_
