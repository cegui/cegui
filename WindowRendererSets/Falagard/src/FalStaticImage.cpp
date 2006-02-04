/************************************************************************
    filename:   FalStaticImage.cpp
    created:    Tue Jul 5 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
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
