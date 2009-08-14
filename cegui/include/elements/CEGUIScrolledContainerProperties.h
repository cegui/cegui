/***********************************************************************
	filename: 	CEGUIScrolledContainerProperties.h
	created:	3/3/2005
	author:		Paul D Turner
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
#ifndef _CEGUIScrolledContainerProperties_h_
#define _CEGUIScrolledContainerProperties_h_

#include "../CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ScrolledContainerProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the ScrolledContainer class
*/
namespace ScrolledContainerProperties
{
    /*!
    \brief
        Property to access the setting which controls whether the content pane is automatically
        resized according to the size and position of attached content.

    \par Usage:
        - Name: ContentPaneAutoSized
        - Format: "[text]"

    \par Where [Text] is:
        - "True" to indicate the pane should automatically resize itself.
        - "False" to indicate the pane should not automatically resize itself.
    */
    class ContentPaneAutoSized : public Property
    {
    public:
        ContentPaneAutoSized() : Property(
            "ContentPaneAutoSized",
            "Property to get/set the setting which controls whether the content pane will auto-size itself.  Value is either \"True\" or \"False\".",
            "True")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
       Property to access the current content pane area rectangle (as window relative pixels).

    \par Usage:
        - Name: ContentArea
        - Format: "l:[float] t:[float] r:[float] b:[float]".

    \par Where:
        - l:[float]	specifies the position of the left edge of the area as a floating point number.
        - t:[float]	specifies the position of the top edge of the area as a floating point number.
        - r:[float]	specifies the position of the right edge of the area as a floating point number.
        - b:[float]	specifies the position of the bottom edge of the area as a floating point number.
    */
    class ContentArea : public Property
    {
    public:
        ContentArea() : Property(
            "ContentArea",
            "Property to get/set the current content area rectangle of the content pane.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom).",
            "l:0.000000 t:0.000000 r:0.000000 b:0.000000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property offering read-only access to the current content extents rectangle (as window relative pixels).

    \par Usage:
        - Name: ChildExtentsArea
        - Format: "l:[float] t:[float] r:[float] b:[float]".

    \par Where:
        - l:[float]	specifies the position of the left edge of the area as a floating point number.
        - t:[float]	specifies the position of the top edge of the area as a floating point number.
        - r:[float]	specifies the position of the right edge of the area as a floating point number.
        - b:[float]	specifies the position of the bottom edge of the area as a floating point number.
    */
    class ChildExtentsArea : public Property
    {
    public:
        ChildExtentsArea() : Property(
            "ChildExtentsArea",
            "Property to get the current content extents rectangle.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom).",
            "l:0.000000 t:0.000000 r:0.000000 b:0.000000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

} // End of  ScrolledContainerProperties namespace section
} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIScrolledContainerProperties_h_
