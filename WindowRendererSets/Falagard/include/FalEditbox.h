/************************************************************************
    filename:   FalEditbox.h
    created:    Sat Jun 25 2005
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
#ifndef _FalEditbox_h_
#define _FalEditbox_h_

#include "FalModule.h"
#include "elements/CEGUIEditbox.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Editbox class for the FalagardBase module.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States:
            - Enabled    - Rendering for when the editbox is in enabled and is in read-write mode.
            - ReadOnly  - Rendering for when the editbox is in enabled and is in read-only mode.
            - Disabled  - Rendering for when the editbox is disabled.
            - ActiveSelection   - additional state rendered for text selection (the imagery in this section is rendered within the selection area.)
            - InactiveSelection - additional state rendered for text selection (the imagery in this section is rendered within the selection area.)

        NamedAreas:
            TextArea    - area where text, selection, and carat imagery will appear.

        PropertyDefinitions (optional, defaults will be black):
            - NormalTextColour   - property that accesses a colour value to be used to render normal unselected text.
            - SelectedTextColour - property that accesses a colour value to be used to render selected text.

        Imagery Sections:
            - Carat
    */
    class FALAGARDBASE_API FalagardEditbox : public EditboxWindowRenderer
    {
    public:
        static const utf8   TypeName[];       //!< type name for this widget.
        static const String UnselectedTextColourPropertyName;   //!< Name of property to access for unselected text colour.
        static const String SelectedTextColourPropertyName;     //!< Name of property to access for selected text colour.

        /*!
        \brief
            Constructor
        */
        FalagardEditbox(const String& type);

        /*!
        \brief
            return the colour to be used for rendering Editbox text oustside of the
            selected region.

        \return
            colour value describing the colour to be used.
        */
        colour getUnselectedTextColour() const;

        /*!
        \brief
            return the colour to be used for rendering Editbox text falling within
            the selected region.

        \return
            colour value describing the colour to be used.
        */
        colour getSelectedTextColour() const;

        /*!
        \brief
            Return a colour object fetched from the named property if it exists,
            else a default colour (black).

        \param propertyName
            String object holding the name of the property to be accessed if it
            exists.
        */
        colour getOptionalPropertyColour(const String& propertyName) const;

        void render();

        // overridden from EditboxWindowRenderer base class.
        size_t getTextIndexFromPosition(const Point& pt) const;

        // rendering internal vars
        float	d_lastTextOffset;		//!< x rendering offset used last time we drew the widget.
    };

} // End of  CEGUI namespace section


#endif  // end of guard _FalEditbox_h_
