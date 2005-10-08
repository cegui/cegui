/************************************************************************
    filename:   CEGUIFalPropertyDefinitionBase.h
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
#ifndef _CEGUIFalPropertyDefinitionBase_h_
#define _CEGUIFalPropertyDefinitionBase_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        common base class used for types representing a new property to be
        available on all widgets that use the WidgetLook that the property
        definition is a part of.
    */
    class CEGUIEXPORT PropertyDefinitionBase : public Property
    {
    public:
        PropertyDefinitionBase(const String& name, const String& help, const String& initialValue, bool redrawOnWrite, bool layoutOnWrite);

        /*!
        \brief
            Sets the value of the property.

        \note
            When overriding the set() member of PropertyDefinitionBase, you MUST
            call the base class implementation after you have set the property
            value (i.e. you must call PropertyDefinitionBase::set()).

        \param receiver
            Pointer to the target object.

        \param value
            A String object that contains a textual representation of the new value to assign to the Property.

        \return
            Nothing.

        \exception InvalidRequestException  Thrown when the Property was unable to interpret the content of \a value.
        */
        void set(PropertyReceiver* receiver, const String& value);

        /*!
        \brief
            Writes an xml representation of the PropertyDefinitionBase based
            object to \a out_stream.

        \param out_stream
            Stream where xml data should be output.

        \return
            Nothing.
        */
        virtual void writeXMLToStream(OutStream& out_stream) const;

    protected:
        /*!
        \brief
            Write out the text of the XML element type.  Note that you should
            not write the opening '<' character, nor any other information such
            as attributes in this function.  The writeExtraAttributes function
            can be used for writing attributes.

        \param out_stream
            Stream where xml data should be output.
        */
        virtual void writeXMLElementType(OutStream& out_stream) const = 0;

        /*!
        \brief
            Write out any xml attributes added in a sub-class.  Note that you
            should not write the closing '/>' character sequence, nor any other
            information in this function.  You should always call the base class
            implementation of this function when overriding.

        \param out_stream
            Stream where xml data should be output.
        */
        virtual void writeXMLAttributes(OutStream& out_stream) const;

        bool d_writeCausesRedraw;
        bool d_writeCausesLayout;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalPropertyDefinitionBase_h_
