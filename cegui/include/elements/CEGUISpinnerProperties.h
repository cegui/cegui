/***********************************************************************
	filename: 	CEGUISpinnerProperties.h
	created:	7/2/2005
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
#ifndef _CEGUISpinnerProperties_h_
#define _CEGUISpinnerProperties_h_

#include "../CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{
// Start of SpinnerProperties namespace section
namespace SpinnerProperties
{
    /*!
    \brief
	    Property to access the current value of the spinner.

	    \par Usage:
		    - Name: CurrentValue
		    - Format: "[float]".

	    \par Where:
		    - [float] represents the current value of the Spinner widget.
    */
    class CurrentValue : public Property
    {
    public:
	    CurrentValue() : Property(
		    "CurrentValue",
		    "Property to get/set the current value of the spinner.  Value is a float.",
		    "0.000000")
	    {}

	    String get(const PropertyReceiver* receiver) const;
	    void set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
	    Property to access the step size of the spinner.

	    \par Usage:
		    - Name: StepSize
		    - Format: "[float]".

	    \par Where:
		    - [float] represents the current value of the Spinner widget.
    */
    class StepSize : public Property
    {
    public:
	    StepSize() : Property(
		    "StepSize",
		    "Property to get/set the step size of the spinner.  Value is a float.",
		    "1.000000")
	    {}

	    String get(const PropertyReceiver* receiver) const;
	    void set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
	    Property to access the minimum value setting of the spinner.

	    \par Usage:
		    - Name: MinimumValue
		    - Format: "[float]".

	    \par Where:
		    - [float] represents the current minimum value of the Spinner widget.
    */
    class MinimumValue : public Property
    {
    public:
	    MinimumValue() : Property(
		    "MinimumValue",
		    "Property to get/set the minimum value setting of the spinner.  Value is a float.",
		    "-32768.000000")
	    {}

	    String get(const PropertyReceiver* receiver) const;
	    void set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
	    Property to access the maximum value setting of the spinner.

	    \par Usage:
		    - Name: MaximumValue
		    - Format: "[float]".

	    \par Where:
		    - [float] represents the current maximum value of the Spinner widget.
    */
    class MaximumValue : public Property
    {
    public:
	    MaximumValue() : Property(
		    "MaximumValue",
		    "Property to get/set the maximum value setting of the spinner.  Value is a float.",
		    "32767.000000")
	    {}

	    String get(const PropertyReceiver* receiver) const;
	    void set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
	    Property to access the TextInputMode setting.

	    This property offers access the text display and input mode for the spinner.

	    \par Usage:
		    - Name: TextInputMode
		    - Format: "[text]".

	    \par Where [text] is:
		    - "FloatingPoint" for floating point decimal numbers.
		    - "Integer" for integer decimal numbers.
            - "Hexadecimal" for hexadecimal numbers.
            - "Octal" for octal numbers.
    */
    class TextInputMode : public Property
    {
    public:
	    TextInputMode() : Property(
		    "TextInputMode",
		    "Property to get/set the TextInputMode setting for the spinner.  Value is \"FloatingPoint\", \"Integer\", \"Hexadecimal\", or \"Octal\".",
		    "Integer")
	    {}

	    String	get(const PropertyReceiver* receiver) const;
	    void	set(PropertyReceiver* receiver, const String& value);
    };

} // End of  SpinnerProperties namespace section
} // End of  CEGUI namespace section


#endif	// end of guard _CEGUISpinnerProperties_h_
