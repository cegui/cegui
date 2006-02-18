/************************************************************************
    filename:   FalToggleButton.h
    created:    Sat Jan 14 2006
    author:     Tomas Lindquist Olsen <tomas@famolsen.dk>
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
#ifndef _FalToggleButton_h_
#define _FalToggleButton_h_

#include "FalModule.h"
#include "FalButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        ToggleButton class for the FalagardBase module.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States (missing states will default to 'Normal' or 'SelectedNormal' if selected)
            - Normal            - Rendering for when the togglebutton is neither pushed or has the mouse hovering over it.
            - Hover             - Rendering for then the togglebutton has the mouse hovering over it.
            - Pushed            - Rendering for when the togglebutton is not selected, is pushed and has the mouse over it.
            - PushedOff         - Rendering for when the togglebutton is not selected, is pushed and the mouse is not over it.
            - Disabled          - Rendering for when the togglebutton is not selected and is disabled.
            - SelectedNormal    - Rendering for when the togglebutton is selected and is neither pushed or has the mouse hovering over it.
            - SelectedHover     - Rendering for then the togglebutton is selected and has the mouse hovering over it.
            - SelectedPushed    - Rendering for when the togglebutton is selected, is pushed and has the mouse over it.
            - SelectedPushedOff - Rendering for when the togglebutton is selected, is pushed and the mouse is not over it.
            - SelectedDisabled  - Rendering for when the togglebutton is selected and is disabled.
    */
    class FALAGARDBASE_API FalagardToggleButton : public FalagardButton
    {
    public:
        static const utf8   TypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardToggleButton(const String& type);

    protected:
        String actualStateName(const String& s) const;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _FalToggleButton_h_
