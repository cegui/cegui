/***********************************************************************
	filename: 	CEGUIScrollbarProperties.h
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Interface for Scrollbar properties
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
#ifndef _CEGUIScrollbarProperties_h_
#define _CEGUIScrollbarProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ScrollbarProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Scrollbar class
*/
namespace ScrollbarProperties
{
/*!
\brief
	Property to access the document size for the Scrollbar.

	\par Usage:
		- Name: DocumentSize
		- Format: "[float]".

	\par Where:
		- [float] specifies the size of the document being scrolled (as defined by the client code).
*/
class DocumentSize : public Property
{
public:
	DocumentSize() : Property(
		"DocumentSize",
		"Property to get/set the document size for the Scrollbar.  Value is a float.",
		"1.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the page size for the Scrollbar.

	\par Usage:
		- Name: PageSize
		- Format: "[float]".

	\par Where:
		- [float] specifies the size of the visible page (as defined by the client code).
*/
class PageSize : public Property
{
public:
	PageSize() : Property(
		"PageSize",
		"Property to get/set the page size for the Scrollbar.  Value is a float.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the step size for the Scrollbar.

	\par Usage:
		- Name: StepSize
		- Format: "[float]".

	\par Where:
		- [float] specifies the size of the increase/decrease button step (as defined by the client code).
*/
class StepSize : public Property
{
public:
	StepSize() : Property(
		"StepSize",
		"Property to get/set the step size for the Scrollbar.  Value is a float.",
		"1.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the overlap size for the Scrollbar.

	\par Usage:
		- Name: OverlapSize
		- Format: "[float]".

	\par Where:
		- [float] specifies the size of the per-page overlap (as defined by the client code).
*/
class OverlapSize : public Property
{
public:
	OverlapSize() : Property(
		"OverlapSize",
		"Property to get/set the overlap size for the Scrollbar.  Value is a float.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
	Property to access the scroll position of the Scrollbar.

	\par Usage:
		- Name: ScrollPosition
		- Format: "[float]".

	\par Where:
		- [float] specifies the current scroll position / value of the Scrollbar.
*/
class ScrollPosition : public Property
{
public:
	ScrollPosition() : Property(
		"ScrollPosition",
		"Property to get/set the scroll position of the Scrollbar.  Value is a float.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  ScrollbarProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIScrollbarProperties_h_
