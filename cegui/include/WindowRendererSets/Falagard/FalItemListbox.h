/************************************************************************
    filename:   FalItemListbox.h
    created:    Mon Mar 20 2006
    author:     Tomas Lindquist Olsen
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)
 
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
#ifndef _FalItemListbox_h_
#define _FalItemListbox_h_

#include "FalModule.h"
#include "elements/CEGUIItemListBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        ItemListbox class for the FalagardBase module.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States:
            - Enabled
            - Disabled

        Named Areas:
            ItemRenderArea
            ItemRenderAreaHScroll
            ItemRenderAreaVScroll
            ItemRenderAreaHVScroll
    */
    class FALAGARDBASE_API FalagardItemListbox : public ItemListBaseWindowRenderer
    {
    public:
        static const utf8   TypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardItemListbox(const String& type);

        // overridden from ItemListBaseWindowRenderer base class.
        void render();
        Rect getItemRenderArea(void) const;
    };

} // End of  CEGUI namespace section

#endif  // end of guard _FalItemListbox_h_
