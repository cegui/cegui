/************************************************************************
    filename:   TESTFalagardButton.h
    created:    Sat Jun 18 2005
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
#ifndef _TESTFalagardButton_h_
#define _TESTFalagardButton_h_

#include "CEGUIWindowFactory.h"
#include "elements/CEGUIPushButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    class CEGUIBASE_API FalagardButton : public PushButton
    {
    public:
        // type name for this widget
        static const utf8   WidgetTypeName[];

        FalagardButton(const String& type, const String& name);
        ~FalagardButton();

        // HACK: This was done to work around the fact that PushButton does not
        // HACK: currently know about the RenderCache, so we use this little hack
        // HACK: and some other bits elsewhere to tie this together.  Eventually
        // HACK: this kind of thing will be handled much cleaner.
        void drawSelf(float z)  { PushButton::drawSelf(z); Window::drawSelf(z); }

        void drawNormal(float z);
        void drawHover(float z);
        void drawPushed(float z);
    };


    class CEGUIBASE_API FalagardButtonFactory : public WindowFactory
    {
    public:
        FalagardButtonFactory(void) : WindowFactory(FalagardButton::WidgetTypeName) { }
        ~FalagardButtonFactory(void){}
        Window*	createWindow(const String& name);
        void	destroyWindow(Window* window)	 { if (window->getType() == d_type) delete window; }
    };


} // End of  CEGUI namespace section


#endif  // end of guard _TESTFalagardButton_h_
