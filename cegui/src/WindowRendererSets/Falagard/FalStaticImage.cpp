/***********************************************************************
    filename:   FalStaticImage.cpp
    created:    Tue Jul 5 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "FalStaticImage.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardStaticImage::TypeName[] = "Falagard/StaticImage";
    
    FalagardStaticImageProperties::Image    FalagardStaticImage::d_imageProperty;

    FalagardStaticImage::FalagardStaticImage(const String& type) :
        FalagardStatic(type),
        d_image(0)
    {
        registerProperty(&d_imageProperty);
    }

    void FalagardStaticImage::render()
    {
        // base class rendering
        FalagardStatic::render();

        // render image if there is one
        if (d_image!=0)
        {
            // get WidgetLookFeel for the assigned look.
            const WidgetLookFeel& wlf = getLookNFeel();
            String imagery_name = (!d_frameEnabled && wlf.isStateImageryPresent("NoFrameImage")) ? "NoFrameImage" : "WithFrameImage";
            wlf.getStateImagery(imagery_name).render(*d_window);
        }
    }

    void FalagardStaticImage::setImage(const Image* img)
    {
        d_image = img;
        d_window->requestRedraw();
    }

} // End of  CEGUI namespace section
