/***********************************************************************
    filename:   CEGUIBasicImage.h
    created:    Wed Feb 16 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIBasicImage_h_
#define _CEGUIBasicImage_h_

#include "CEGUIImage.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIEXPORT BasicImage : public Image
{
public:
    BasicImage(const String& name);

    BasicImage(const String& name, Texture* texture,
               const Rect& tex_area, const Vector2<>& offset,
               const bool autoscaled, const Size<>& native_res);

    void setTexture(Texture* texture);
    void setArea(const Rect& pixel_area);
    void setOffset(const Vector2<>& pixel_offset);
    void setAutoScaled(const bool autoscaled);
    void setNativeResolution(const Size<>& native_res);

    // Implement CEGUI::Image interface
    const String& getName() const;
    const Size<>& getRenderedSize() const;
    const Vector2<>& getRenderedOffset() const;
    void render(GeometryBuffer& buffer,
                        const Rect& dest_area,
                        const Rect* clip_area,
                        const ColourRect& colours) const;
    void notifyDisplaySizeChanged(const Size<>& size);
    Image& clone() const;

protected:
    //! name used when the BasicImage was created.
    String d_name;
    //! Texture used by this image.
    Texture* d_texture;
    //! Rect defining texture co-ords for this image.
    Rect d_area;
    //! Actual pixel size.
    Size<> d_pixelSize;
    //! Defined pixel offset
    Vector2<> d_pixelOffset;
    //! Whether image is auto-scaled or not.
    bool d_autoscaled;
    //! Native resolution used for autoscaling.
    Size<> d_nativeResolution;
    //! Size after having autoscaling applied.
    Size<> d_scaledSize;
    //! Offset after having autoscaling applied.
    Vector2<> d_scaledOffset;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIBasicImage_h_

