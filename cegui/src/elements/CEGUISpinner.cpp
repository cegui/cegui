/***********************************************************************
	filename: 	CEGUISpinner.cpp
	created:	3/2/2005
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
#include "elements/CEGUISpinner.h"
#include "elements/CEGUIPushButton.h"
#include "elements/CEGUIEditbox.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include <stdio.h>
#include <sstream>
#include <iomanip>

// Start of CEGUI namespace section
namespace CEGUI
{
    const String Spinner::WidgetTypeName("CEGUI/Spinner");

    //////////////////////////////////////////////////////////////////////////
    // event strings
    const String Spinner::EventNamespace("Spinner");
    const String Spinner::EventValueChanged("ValueChanged");
    const String Spinner::EventStepChanged("StepChanged");
    const String Spinner::EventMaximumValueChanged("MaximumValueChanged");
    const String Spinner::EventMinimumValueChanged("MinimumValueChanged");
    const String Spinner::EventTextInputModeChanged("TextInputModeChanged");
    // Validator strings
    const String Spinner::FloatValidator("-?\\d*\\.?\\d*");
    const String Spinner::IntegerValidator("-?\\d*");
    const String Spinner::HexValidator("[0-9a-fA-F]*");
    const String Spinner::OctalValidator("[0-7]*");
    // component widget name strings
    const String Spinner::EditboxName( "__auto_editbox__" );
    const String Spinner::IncreaseButtonName( "__auto_incbtn__" );
    const String Spinner::DecreaseButtonName( "__auto_decbtn__" );
    //////////////////////////////////////////////////////////////////////////
    // this is taken from stringencoders lib (and modified)
    // http://code.google.com/p/stringencoders/
    //
    // Thanks to FluXy for the heads-up on this one!
    String modp_dtoa(double value, int prec);


    Spinner::Spinner(const String& type, const String& name) :
        Window(type, name),
        d_stepSize(1.0f),
        d_currentValue(1.0f),
        d_maxValue(32767.0f),
        d_minValue(-32768.0f),
        d_inputMode((TextInputMode)-1)
    {
        addSpinnerProperties();
    }

    Spinner::~Spinner(void)
    {
        // Nothing to do here.
    }

    void Spinner::initialiseComponents(void)
    {
        // get all the component widgets
        PushButton* increaseButton = getIncreaseButton();
        PushButton* decreaseButton = getDecreaseButton();
        Editbox* editbox = getEditbox();

        // setup component controls
        increaseButton->setWantsMultiClickEvents(false);
        increaseButton->setMouseAutoRepeatEnabled(true);
        decreaseButton->setWantsMultiClickEvents(false);
        decreaseButton->setMouseAutoRepeatEnabled(true);

        // perform event subscriptions.
        increaseButton->subscribeEvent(Window::EventMouseButtonDown, Event::Subscriber(&Spinner::handleIncreaseButton, this));
        decreaseButton->subscribeEvent(Window::EventMouseButtonDown, Event::Subscriber(&Spinner::handleDecreaseButton, this));
        editbox->subscribeEvent(Window::EventTextChanged, Event::Subscriber(&Spinner::handleEditTextChange, this));

        // final initialisation
        setTextInputMode(Integer);
        setCurrentValue(0.0f);
        performChildWindowLayout();
    }

    double Spinner::getCurrentValue(void) const
    {
        return d_currentValue;
    }

    double Spinner::getStepSize(void) const
    {
        return d_stepSize;
    }

    double Spinner::getMaximumValue(void) const
    {
        return d_maxValue;
    }

    double Spinner::getMinimumValue(void) const
    {
        return d_minValue;
    }

    Spinner::TextInputMode Spinner::getTextInputMode(void) const
    {
        return d_inputMode;
    }

    void Spinner::setCurrentValue(double value)
    {
        if (value != d_currentValue)
        {
            // limit input value to within valid range for spinner
            value = ceguimax(ceguimin(value, d_maxValue), d_minValue);

            d_currentValue = value;

            WindowEventArgs args(this);
            onValueChanged(args);
        }
    }

    void Spinner::setStepSize(double step)
    {
        if (step != d_stepSize)
        {
            d_stepSize = step;

            WindowEventArgs args(this);
            onStepChanged(args);
        }
    }

    void Spinner::setMaximumValue(double maxValue)
    {
        if (maxValue != d_maxValue)
        {
            d_maxValue = maxValue;

            WindowEventArgs args(this);
            onMaximumValueChanged(args);
        }
    }

    void Spinner::setMinimumValue(double minVaue)
    {
        if (minVaue != d_minValue)
        {
            d_minValue = minVaue;

            WindowEventArgs args(this);
            onMinimumValueChanged(args);
        }
    }

    void Spinner::setTextInputMode(TextInputMode mode)
    {
        if (mode != d_inputMode)
        {
            switch (mode)
            {
            case FloatingPoint:
                getEditbox()->setValidationString(FloatValidator);
                break;
            case Integer:
                getEditbox()->setValidationString(IntegerValidator);
                break;
            case Hexadecimal:
                getEditbox()->setValidationString(HexValidator);
                break;
            case Octal:
                getEditbox()->setValidationString(OctalValidator);
                break;
            default:
                CEGUI_THROW(InvalidRequestException("Spinner::setTextInputMode - An unknown TextInputMode was specified."));
            }

            d_inputMode = mode;

            WindowEventArgs args(this);
            onTextInputModeChanged(args);
        }
    }

    void Spinner::addSpinnerProperties(void)
    {
        const String propertyOrigin("Spinner");

        CEGUI_DEFINE_PROPERTY(Spinner, double,
            "CurrentValue", "Property to get/set the current value of the spinner.  Value is a float.",
            &Spinner::setCurrentValue, &Spinner::getCurrentValue, 0.0f
        );
        CEGUI_DEFINE_PROPERTY(Spinner, double,
            "StepSize", "Property to get/set the step size of the spinner.  Value is a float.",
            &Spinner::setStepSize, &Spinner::getStepSize, 1.0f
        );
        CEGUI_DEFINE_PROPERTY(Spinner, double,
            "MinimumValue", "Property to get/set the minimum value setting of the spinner.  Value is a float.",
            &Spinner::setMinimumValue, &Spinner::getMinimumValue, -32768.000000f
        );
        CEGUI_DEFINE_PROPERTY(Spinner, double,
            "MaximumValue", "Property to get/set the maximum value setting of the spinner.  Value is a float.",
            &Spinner::setMaximumValue, &Spinner::getMaximumValue, 32767.000000f
        );
        CEGUI_DEFINE_PROPERTY(Spinner, Spinner::TextInputMode,
            "TextInputMode", "Property to get/set the TextInputMode setting for the spinner.  Value is \"FloatingPoint\", \"Integer\", \"Hexadecimal\", or \"Octal\".",
            &Spinner::setTextInputMode, &Spinner::getTextInputMode, Spinner::Integer
        );
    
    }

    double Spinner::getValueFromText(void) const
    {
        String tmpTxt(getEditbox()->getText());

        // handle empty and lone '-' or '.' cases
        if (tmpTxt.empty() || (tmpTxt == "-") || (tmpTxt == "."))
        {
            return 0.0f;
        }

        int res, tmp;
        uint utmp;
        double val;

        switch (d_inputMode)
        {
        case FloatingPoint:
            res = sscanf(tmpTxt.c_str(), "%lf", &val);
            break;
        case Integer:
            res = sscanf(tmpTxt.c_str(), "%d", &tmp);
            val = static_cast<double>(tmp);
            break;
        case Hexadecimal:
            res = sscanf(tmpTxt.c_str(), "%x", &utmp);
            val = static_cast<double>(utmp);
            break;
        case Octal:
            res = sscanf(tmpTxt.c_str(), "%o", &utmp);
            val = static_cast<double>(utmp);
            break;
        default:
            CEGUI_THROW(InvalidRequestException("Spinner::getValueFromText - An unknown TextInputMode was encountered."));
        }

        if (res)
        {
            return val;
        }

        CEGUI_THROW(InvalidRequestException("Spinner::getValueFromText - The string '" + getEditbox()->getText() + "' can not be converted to numerical representation."));
    }

    String Spinner::getTextFromValue(void) const
    {
        std::stringstream tmp;

        switch (d_inputMode)
        {
        case FloatingPoint:
            return modp_dtoa(d_currentValue, 9);
            break;
        case Integer:
            tmp << static_cast<int>(d_currentValue);
            break;
        case Hexadecimal:
            tmp << std::hex << std::uppercase << static_cast<int>(d_currentValue);
            break;
        case Octal:
            tmp << std::oct << static_cast<int>(d_currentValue);
            break;
        default:
            CEGUI_THROW(InvalidRequestException("Spinner::getValueFromText - An unknown TextInputMode was encountered."));
        }

        return String(tmp.str().c_str());
    }

    void Spinner::onFontChanged(WindowEventArgs& e)
    {
        // Propagate to children
        getEditbox()->setFont(getFont());
        // Call base class handler
        Window::onFontChanged(e);
    }

    void Spinner::onTextChanged(WindowEventArgs& e)
    {
        Editbox* editbox = getEditbox();

        // update only if needed
        if (editbox->getText() != getText())
        {
            // done before doing base class processing so event subscribers see
            // 'updated' version.
            editbox->setText(getText());
            ++e.handled;

            Window::onTextChanged(e);
        }
    }

    void Spinner::onActivated(ActivationEventArgs& e)
    {
        if (!isActive())
        {
            Window::onActivated(e);

            Editbox* editbox = getEditbox();

            if (!editbox->isActive())
            {
                editbox->activate();
            }
        }
    }

    void Spinner::onValueChanged(WindowEventArgs& e)
    {
        Editbox* editbox = getEditbox();

        // mute to save doing unnecessary events work.
        bool wasMuted = editbox->isMuted();
        editbox->setMutedState(true);

        // Update text with new value.
        // (allow empty and '-' cases to equal 0 with no text change required)
        if (!(d_currentValue == 0 &&
              (editbox->getText().empty() || editbox->getText() == "-")))
        {
            editbox->setText(getTextFromValue());
        }
        // restore previous mute state.
        editbox->setMutedState(wasMuted);

        fireEvent(EventValueChanged, e, EventNamespace);
    }

    void Spinner::onStepChanged(WindowEventArgs& e)
    {
        fireEvent(EventStepChanged, e, EventNamespace);
    }

    void Spinner::onMaximumValueChanged(WindowEventArgs& e)
    {
        fireEvent(EventMaximumValueChanged, e, EventNamespace);

        if (d_currentValue > d_maxValue)
        {
            setCurrentValue(d_maxValue);
        }
    }

    void Spinner::onMinimumValueChanged(WindowEventArgs& e)
    {
        fireEvent(EventMinimumValueChanged, e, EventNamespace);

        if (d_currentValue < d_minValue)
        {
            setCurrentValue(d_minValue);
        }
    }

    void Spinner::onTextInputModeChanged(WindowEventArgs& e)
    {
        Editbox* editbox = getEditbox();
        // update edit box text to reflect new mode.
        // mute to save doing unnecessary events work.
        bool wasMuted = editbox->isMuted();
        editbox->setMutedState(true);
        // Update text with new value.
        editbox->setText(getTextFromValue());
        // restore previous mute state.
        editbox->setMutedState(wasMuted);

        fireEvent(EventTextInputModeChanged, e, EventNamespace);
    }

    bool Spinner::handleIncreaseButton(const EventArgs& e)
    {
        if (((const MouseEventArgs&)e).button == LeftButton)
        {
            setCurrentValue(d_currentValue + d_stepSize);
            return true;
        }

        return false;
    }

    bool Spinner::handleDecreaseButton(const EventArgs& e)
    {
        if (((const MouseEventArgs&)e).button == LeftButton)
        {
            setCurrentValue(d_currentValue - d_stepSize);
            return true;
        }

        return false;
    }

    bool Spinner::handleEditTextChange(const EventArgs&)
    {
        // set this windows text to match
        setText(getEditbox()->getText());
        // update value
        setCurrentValue(getValueFromText());
        return true;
    }

    PushButton* Spinner::getIncreaseButton() const
    {
        return static_cast<PushButton*>(getChild(IncreaseButtonName));
    }

    PushButton* Spinner::getDecreaseButton() const
    {
        return static_cast<PushButton*>(getChild(DecreaseButtonName));
    }

    Editbox* Spinner::getEditbox() const
    {
        return static_cast<Editbox*>(getChild(EditboxName));
    }

//////////////////////////////////////////////////////////////////////////
// The following are slightly modified function(s) taken from the
// stringencoders library.
// http://code.google.com/p/stringencoders/
//
// Thanks to FluXy for the heads-up on this one!
/*
 * Copyright 2005, 2006, 2007
 * Nick Galbreath -- nickg [at] modp [dot] com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the modp.com nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This is the standard "new" BSD license:
 * http://www.opensource.org/licenses/bsd-license.php
 */

/**
 * Powers of 10
 * 10^0 to 10^9
 */
static const double pow10[] =
{ 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

void strreverse(char* begin, char* end)
{
    char aux;
    while (end > begin)
        aux = *end, *end-- = *begin, *begin++ = aux;
}

String modp_dtoa(double value, int prec)
{
    /* if input is larger than thres_max, revert to exponential */
    const double thres_max = (double)(0x7FFFFFFF);

    double diff = 0.0;
    char str[64];
    char* wstr = str;

    if (prec < 0) {
        prec = 0;
    } else if (prec > 9) {
        /* precision of >= 10 can lead to overflow errors */
        prec = 9;
    }

    /* we'll work in positive values and deal with the
       negative sign issue later */
    int neg = 0;
    if (value < 0) {
        neg = 1;
        value = -value;
    }

    int whole = (int) value;
    double tmp = (value - whole) * pow10[prec];
    uint32 frac = (uint32)(tmp);
    diff = tmp - frac;

    if (diff > 0.5) {
        ++frac;
        /* handle rollover, e.g.  case 0.99 with prec 1 is 1.0  */
        if (frac >= pow10[prec]) {
            frac = 0;
            ++whole;
        }
    } else if (diff == 0.5 && ((frac == 0) || (frac & 1))) {
        /* if halfway, round up if odd, OR
           if last digit is 0.  That last part is strange */
        ++frac;
    }

    /* for very large numbers switch back to native sprintf for exponentials.
       anyone want to write code to replace this? */
    /*
       normal printf behavior is to print EVERY whole number digit
       which can be 100s of characters overflowing your buffers == bad
    */
    if (value > thres_max) {
        sprintf(str, "%e", neg ? -value : value);
        return String(str);
    }

    if (prec == 0) {
        diff = value - whole;
        if (diff > 0.5) {
            /* greater than 0.5, round up, e.g. 1.6 -> 2 */
            ++whole;
        } else if (diff == 0.5 && (whole & 1)) {
            /* exactly 0.5 and ODD, then round up */
            /* 1.5 -> 2, but 2.5 -> 2 */
            ++whole;
        }
    } else {
        // This section has been modifed by the CEGUI project to eliminate
        // the output of trailing zeros in the fractional part.
        bool non_zero = false;
        int count = prec;
        uint32 digit = 0;
        // now do fractional part, as an unsigned number
        do {
            --count;
            digit = (frac % 10);
            if (non_zero || (digit != 0)) {
                *wstr++ = 48 + digit;
                non_zero = true;
            }
        } while (frac /= 10);
        // add extra 0s
        if (non_zero) {
            while (count-- > 0) *wstr++ = '0';
            // add decimal
            *wstr++ = '.';
        }
    }

    // do whole part
    // Take care of sign
    // Conversion. Number is reversed.
    do *wstr++ = 48 + (whole % 10); while (whole /= 10);
    if (neg) {
        *wstr++ = '-';
    }
    *wstr='\0';
    strreverse(str, wstr-1);

    return String(str);
}
// The above are slightly modified function(s) taken from the
// stringencoders library.
// http://code.google.com/p/stringencoders/
//////////////////////////////////////////////////////////////////////////

} // End of  CEGUI namespace section
