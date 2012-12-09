/***********************************************************************
    filename:   CEGUIFalPropertyDefinitionBase.h
    created:    Sat Oct 8 2005
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
#ifndef _CEGUIFalPropertyDefinitionBase_h_
#define _CEGUIFalPropertyDefinitionBase_h_

#include "../CEGUIProperty.h"

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

        \param xml_stream
            Stream where xml data should be output.


        \return
            Nothing.
        */
        virtual void writeXMLToStream(XMLSerializer& xml_stream) const;

    protected:
        /*!
        \brief
            Write out the text of the XML element type.  Note that you should
            not write the opening '<' character, nor any other information such
            as attributes in this function.  The writeExtraAttributes function
            can be used for writing attributes.

        \param xml_stream
            Stream where xml data should be output.
        */
        virtual void writeXMLElementType(XMLSerializer& xml_stream) const = 0;

        /*!
        \brief
            Write out any xml attributes added in a sub-class.  Note that you
            should not write the closing '/>' character sequence, nor any other
            information in this function.  You should always call the base class
            implementation of this function when overriding.

        \param xml_stream
            Stream where xml data should be output.
        */
        virtual void writeXMLAttributes(XMLSerializer& xml_stream) const;

        bool d_writeCausesRedraw;
        bool d_writeCausesLayout;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalPropertyDefinitionBase_h_
