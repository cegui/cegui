/***********************************************************************
    filename:   FalEditbox.h
    created:    Sat Jun 25 2005
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
