/************************************************************************
    filename:   FalMultiLineEditbox.h
    created:    Thu Jul 7 2005
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
#ifndef _FalMultiLineEditbox_h_
#define _FalMultiLineEditbox_h_

#include "FalModule.h"
#include "elements/CEGUIMultiLineEditbox.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
   /*!
    \brief
        MultiLineEditbox class for the FalagardBase module.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States:
            - Enabled    - Rendering for when the editbox is in enabled and is in read-write mode.
            - ReadOnly  - Rendering for when the editbox is in enabled and is in read-only mode.
            - Disabled  - Rendering for when the editbox is disabled.

        NamedAreas:
            TextArea         - area where text, selection, and carat imagery will appear.
            TextAreaHScroll  - TextArea when only horizontal scrollbar is visible.
            TextAreaVScroll  - TextArea when only vertical scrollbar is visible.
            TextAreaHVScroll - TextArea when both horizontal and vertical scrollbar is visible.

        PropertyDefinitions (optional, defaults will be black):
            - NormalTextColour        - property that accesses a colour value to be used to render normal unselected text.
            - SelectedTextColour      - property that accesses a colour value to be used to render selected text.
            - ActiveSelectionColour   - property that accesses a colour value to be used to render active selection highlight.
            - InactiveSelectionColour - property that accesses a colour value to be used to render inactive selection highlight.

        Imagery Sections:
            - Carat

        Child Widgets:
            Scrollbar based widget with name suffix "__auto_vscrollbar__"
            Scrollbar based widget with name suffix "__auto_hscrollbar__"

    */
    class FALAGARDBASE_API FalagardMultiLineEditbox : public MultiLineEditboxWindowRenderer
    {
    public:
        static const utf8   TypeName[];     //! type name for this widget.
        //! Name of property to use to obtain unselected text rendering colour.
        static const String UnselectedTextColourPropertyName;
        //! Name of property to use to obtain selected text rendering colour.
        static const String SelectedTextColourPropertyName;
        //! Name of property to use to obtain active selection rendering colour.
        static const String ActiveSelectionColourPropertyName;
        //! Name of property to use to obtain inactive selection rendering colour.
        static const String InactiveSelectionColourPropertyName;

        /*!
        \brief
            Constructor
        */
        FalagardMultiLineEditbox(const String& type);

        // overridden from base classes.
        Rect getTextRenderArea(void) const;
        void render();

    protected:
        /*!
        \brief
            Perform rendering of the widget control frame and other 'static' areas.  This
            method should not render the actual text.  Note that the text will be rendered
            to layer 4 and the selection brush to layer 3, other layers can be used for
            rendering imagery behind and infront of the text & selection..

        \return
            Nothing.
        */
        void cacheEditboxBaseImagery();

        /*!
        \brief
            Render the carat.

        \return
            Nothing
        */
        void cacheCaratImagery(const Rect& textArea);

        /*!
        \brief
            Render text lines.
        */
        void cacheTextLines(const Rect& dest_area);

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
            return the colour to be used for rendering the selection highlight
            when the editbox is active.

        \return
            colour value describing the colour to be used.
        */
        colour getActiveSelectionColour() const;

        /*!
        \brief
            return the colour to be used for rendering the selection highlight
            when the editbox is inactive.

        \return
            colour value describing the colour to be used.
        */
        colour getInactiveSelectionColour() const;

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
    };

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _FalMultiLineEditbox_h_
