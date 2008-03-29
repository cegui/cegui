/***********************************************************************
	filename: 	CEGUISliderProperties.h
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to properties for Slider class
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
#ifndef _CEGUISliderProperties_h_
#define _CEGUISliderProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of SliderProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Slider class
*/
namespace SliderProperties
{
/*!
\brief
	Property to access the current value of the slider.

	\par Usage:
		- Name: CurrentValue
		- Format: "[float]".

	\par Where:
		- [float] represents the current value of the slider.
*/
class CurrentValue : public Property
{
public:
	CurrentValue() : Property(
		"CurrentValue",
		"Property to get/set the current value of the slider.  Value is a float.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the maximum value of the slider.

	\par Usage:
		- Name: MaximumValue
		- Format: "[float]".

	\par Where:
		- [float] represents the maximum value of the slider.
*/
class MaximumValue : public Property
{
public:
	MaximumValue() : Property(
		"MaximumValue",
		"Property to get/set the maximum value of the slider.  Value is a float.",
		"1.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the click-step size for the slider.

	\par Usage:
		- Name: ClickStepSize
		- Format: "[float]".

	\par Where:
		- [float] represents the click-step size slider (this is how much the value changes when the slider container is clicked).
*/
class ClickStepSize : public Property
{
public:
	ClickStepSize() : Property(
		"ClickStepSize",
		"Property to get/set the click-step size for the slider.  Value is a float.",
		"0.010000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

} // End of  SliderProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUISliderProperties_h_
