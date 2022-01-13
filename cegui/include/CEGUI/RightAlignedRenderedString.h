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
#ifndef _CEGUIRightAlignedRenderedString_h_
#define _CEGUIRightAlignedRenderedString_h_

#include "CEGUI/FormattedRenderedString.h"
#include <vector>

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{
/*!
\brief
    FormattedRenderedString implementation that renders the RenderedString with
    right aligned formatting.
*/
class CEGUIEXPORT RightAlignedRenderedString : public FormattedRenderedString
{
public:

    HorizontalTextFormatting getCorrespondingFormatting() const override { return HorizontalTextFormatting::RightAligned; }

    void format(const RenderedString& rs, const Window* refWnd, const Sizef& areaSize) override;

    std::vector<GeometryBuffer*> createRenderGeometry(
        const Window* refWnd, const glm::vec2& position,
        const ColourRect* modColours, const Rectf* clipRect) const override;

protected:

    std::vector<float> d_offsets;
};
    
}

#if defined (_MSC_VER)
#	pragma warning(pop)
#endif

#endif
