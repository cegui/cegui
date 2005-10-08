/************************************************************************
    filename:   CEGUIFalPropertyLinkDefinition.h
    created:    Sat Oct 8 2005
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
#ifndef _CEGUIFalPropertyLinkDefinition_h_
#define _CEGUIFalPropertyLinkDefinition_h_

#include "falagard/CEGUIFalPropertyDefinitionBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class representing a property that links to another property defined on
        an attached child widget.
    */
    class CEGUIEXPORT PropertyLinkDefinition : public PropertyDefinitionBase
    {
    public:
        PropertyLinkDefinition(const String& propertyName, const String& widgetNameSuffix, const String& targetProperty, const String& initialValue, bool redrawOnWrite, bool layoutOnWrite);

        // override members from PropertyDefinitionBase
        String get(const PropertyReceiver* receiver) const;
        void set(PropertyReceiver* receiver, const String& value);

    protected:
        void writeXMLElementType(OutStream& out_stream) const;
        void writeXMLAttributes(OutStream& out_stream) const;

        /*!
        \brief
            return a pointer to the window containing the target property to
            be accessed.

        \exception UnknownObjectException
            thrown if no such target window exists within the system.
        */
        const Window* getTargetWindow(const PropertyReceiver* receiver) const;
        Window* getTargetWindow(PropertyReceiver* receiver);

        String d_widgetNameSuffix;
        String d_targetProperty;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalPropertyLinkDefinition_h_
