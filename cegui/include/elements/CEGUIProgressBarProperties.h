/***********************************************************************
	filename: 	CEGUIProgressBarProperties.h
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to properties for the ProgressBar class
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
#ifndef _CEGUIProgressBarProperties_h_
#define _CEGUIProgressBarProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ProgressBarProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the ProgressBar class
*/
namespace ProgressBarProperties
{
/*!
\brief
	Property to access the current progress of the progress bar.

	\par Usage:
		- Name: CurrentProgress
		- Format: "[float]".

	\par Where:
		- [float] is the current progress of the bar expressed as a value between 0 and 1.
*/
class CurrentProgress : public Property
{
public:
	CurrentProgress() : Property(
		"CurrentProgress",
		"Property to get/set the current progress of the progress bar.  Value is a float  value between 0.0 and 1.0 specifying the progress.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the step size setting for the progress bar.

	\par Usage:
		- Name: StepSize
		- Format: "[float]".

	\par Where:
		- [float] is the size of the invisible sizing border in screen pixels.
*/
class StepSize : public Property
{
public:
	StepSize() : Property(
		"StepSize",
		"Property to get/set the step size setting for the progress bar.  Value is a float value.",
		"0.010000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


} // End of  ProgressBarProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIProgressBarProperties_h_
