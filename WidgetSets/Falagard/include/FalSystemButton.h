/************************************************************************
    filename:   FalSystemButton.h
    created:    Sat Jul 9 2005
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
#ifndef _FalSystemButton_h_
#define _FalSystemButton_h_

#include "FalModule.h"
#include "FalButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        SystemButton class for the FalagardBase module.  This class should be used for 'system' buttons on
        a FrameWindow, such as the close button.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States (missing states will default to 'Normal'):
            - Normal    - Rendering for when the button is neither pushed or has the mouse hovering over it.
            - Hover     - Rendering for then the button has the mouse hovering over it.
            - Pushed    - Rendering for when the button is pushed and mouse is over it.
            - PushedOff - Rendering for when the button is pushed and mouse is not over it.
            - Disabled  - Rendering for when the button is disabled.
    */
    class FALAGARDBASE_API FalagardSystemButton : public FalagardButton
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardSystemButton(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardSystemButton();

        Rect getPixelRect(void) const;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _FalSystemSystemButton_h_
