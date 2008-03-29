/***********************************************************************
	filename: 	CEGUISpinnerProperties.cpp
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
#include "elements/CEGUISpinnerProperties.h"
#include "elements/CEGUISpinner.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of SpinnerProperties namespace section
namespace SpinnerProperties
{
    String CurrentValue::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::floatToString(static_cast<const Spinner*>(receiver)->getCurrentValue());
    }

    void CurrentValue::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Spinner*>(receiver)->setCurrentValue(PropertyHelper::stringToFloat(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String StepSize::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::floatToString(static_cast<const Spinner*>(receiver)->getStepSize());
    }

    void StepSize::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Spinner*>(receiver)->setStepSize(PropertyHelper::stringToFloat(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String MinimumValue::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::floatToString(static_cast<const Spinner*>(receiver)->getMinimumValue());
    }

    void MinimumValue::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Spinner*>(receiver)->setMinimumValue(PropertyHelper::stringToFloat(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String MaximumValue::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::floatToString(static_cast<const Spinner*>(receiver)->getMaximumValue());
    }

    void MaximumValue::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Spinner*>(receiver)->setMaximumValue(PropertyHelper::stringToFloat(value));
    }

    //////////////////////////////////////////////////////////////////////////
    
    String TextInputMode::get(const PropertyReceiver* receiver) const
    {
        switch(static_cast<const Spinner*>(receiver)->getTextInputMode())
        {
        case Spinner::FloatingPoint:
            return String("FloatingPoint");
            break;
        case Spinner::Hexadecimal:
            return String("Hexadecimal");
            break;
        case Spinner::Octal:
            return String("Octal");
            break;
        default:
            return String("Integer");
        }
    }

    void TextInputMode::set(PropertyReceiver* receiver, const String& value)
    {
        Spinner::TextInputMode mode;

        if (value == "FloatingPoint")
        {
            mode = Spinner::FloatingPoint;
        }
        else if (value == "Hexadecimal")
        {
            mode = Spinner::Hexadecimal;
        }
        else if (value == "Octal")
        {
            mode = Spinner::Octal;
        }
        else
        {
            mode = Spinner::Integer;
        }

        static_cast<Spinner*>(receiver)->setTextInputMode(mode);
    }

} // End of  SpinnerProperties namespace section
} // End of  CEGUI namespace section
