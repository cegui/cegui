/***********************************************************************
	filename: 	CEGUIWindowProperties.h
	created:	5/7/2004
	author:		Paul D Turner

	purpose:	Interface to available window base class properties
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
#ifndef _CEGUIWindowProperties_h_
#define _CEGUIWindowProperties_h_

#include "CEGUIProperty.h"
#include "CEGUIXMLSerializer.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of WindowProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Window base class
*/
namespace WindowProperties
{

/*!
\brief
    Property to access/change the assigned window renderer object.

    \par Usage:
        - Name: WindowRenderer
        - Format: "[windowRendererName]"

    \par Where [windowRendererName] is the factory name of the window renderer type you wish to assign.
*/
class WindowRenderer : public Property
{
    public:
        WindowRenderer() : Property(
        "WindowRenderer",
        "Property to get/set the windows assigned window renderer objects name.  Value is a string.",
        "")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);

        void writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const;
};


/*!
\brief
    Property to access/change the assigned look'n'feel.

    \par Usage:
        - Name: LookNFeel
        - Format: "[LookNFeelName]"

    \par Where [LookNFeelName] is the name of the look'n'feel you wish to assign.
*/
class LookNFeel : public Property
{
    public:
        LookNFeel() : Property(
        "LookNFeel",
        "Property to get/set the windows assigned look'n'feel.  Value is a string.",
        "")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);

        void writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const;
};


} // End of  WindowProperties namespace section


} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIWindowProperties_h_
