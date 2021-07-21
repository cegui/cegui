/***********************************************************************
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
#include "CEGUI/widgets/Spinner.h"
#include "CEGUI/widgets/PushButton.h"
#include "CEGUI/widgets/Editbox.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/SharedStringStream.h"

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


    Spinner::Spinner(const String& type, const String& name) :
        Window(type, name),
        d_stepSize(1.0f),
        d_currentValue(1.0f),
        d_maxValue(32767.0f),
        d_minValue(-32768.0f),
        d_precision(6),
        d_inputMode(static_cast<TextInputMode>(-1))
    {
        addSpinnerProperties();
    }

    Spinner::~Spinner(void)
    {
        // Nothing to do here.
    }

    void Spinner::initialiseComponents()
    {
        // get all the component widgets
        PushButton* increaseButton = getIncreaseButton();
        PushButton* decreaseButton = getDecreaseButton();
        Editbox* editbox = getEditbox();

        // ban properties forwarded from here
        editbox->banPropertyFromXML(Window::TextPropertyName);
        editbox->banPropertyFromXML("ValidationString");
        increaseButton->banPropertyFromXML(Window::CursorAutoRepeatEnabledPropertyName);
        decreaseButton->banPropertyFromXML(Window::CursorAutoRepeatEnabledPropertyName);

        // setup component controls
        increaseButton->setCursorAutoRepeatEnabled(true);
        decreaseButton->setCursorAutoRepeatEnabled(true);

        // perform event subscriptions.
        increaseButton->subscribeEvent(Window::EventCursorPressHold, Event::Subscriber(&Spinner::handleIncreaseButton, this));
        decreaseButton->subscribeEvent(Window::EventCursorPressHold, Event::Subscriber(&Spinner::handleDecreaseButton, this));
        editbox->subscribeEvent(Window::EventTextChanged, Event::Subscriber(&Spinner::handleEditTextChange, this));

        // final initialisation
        setTextInputMode(TextInputMode::Integer);
        setCurrentValue(0.0f);
        Window::initialiseComponents();
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

    int Spinner::getPrecision(void) const
    {
        return d_precision;
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
            value = std::max(std::min(value, d_maxValue), d_minValue);

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

    void Spinner::setPrecision(int val)
    {
        d_precision = val;
    }

    void Spinner::setTextInputMode(TextInputMode mode)
    {
        if (mode != d_inputMode)
        {
            switch (mode)
            {
            case TextInputMode::FloatingPoint:
                getEditbox()->setValidationString(FloatValidator);
                break;
            case TextInputMode::Integer:
                getEditbox()->setValidationString(IntegerValidator);
                break;
            case TextInputMode::Hexadecimal:
                getEditbox()->setValidationString(HexValidator);
                break;
            case TextInputMode::Octal:
                getEditbox()->setValidationString(OctalValidator);
                break;
            default:
                throw InvalidRequestException(
                    "An unknown TextInputMode was specified.");
            }

            d_inputMode = mode;

            WindowEventArgs args(this);
            onTextInputModeChanged(args);
        }
    }

    void Spinner::addSpinnerProperties(void)
    {
        const String& propertyOrigin = WidgetTypeName;

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
            &Spinner::setTextInputMode, &Spinner::getTextInputMode, Spinner::TextInputMode::Integer
        );

        CEGUI_DEFINE_PROPERTY(Spinner, int,
            "Precision", "Property to get/set the precision of display the floating point values.  Value is a int.",
            &Spinner::setPrecision, &Spinner::getPrecision, 6
        );
    }

    double Spinner::getValueFromText(void) const
    {
        const String& text = getEditbox()->getText();

        switch (d_inputMode)
        {
        case TextInputMode::FloatingPoint:
            return static_cast<double>(PropertyHelper<float>::fromString(text));
        case TextInputMode::Integer:
            return static_cast<double>(PropertyHelper<int>::fromString(text));
        case TextInputMode::Hexadecimal:
        {
            unsigned int tempUint;
            std::stringstream& sstream = SharedStringstream::GetPreparedStream();
            sstream << std::hex << text;
            sstream >> tempUint;
            sstream << std::dec;
            if (sstream.fail())
            {
                throw InvalidRequestException("The string '" + getEditbox()->getText() +
                    "' could not be converted to numerical representation.");
            }
            
            return static_cast<double>(tempUint);
        }
        case TextInputMode::Octal:
        {
            unsigned int tempUint;
            std::stringstream& sstream = SharedStringstream::GetPreparedStream();
            sstream << std::oct << text;
            sstream >> tempUint;
            sstream << std::dec;
            if (sstream.fail())
            {
                throw InvalidRequestException("The string '" + getEditbox()->getText() +
                    "' could not be converted to numerical representation.");
            }
            
            return static_cast<double>(tempUint);
        }
        default:
            throw InvalidRequestException(
                "An unknown TextInputMode was encountered.");
        }
    }

    String Spinner::getTextFromValue(void) const
    {
        std::stringstream& tmp = SharedStringstream::GetPreparedStream();

        switch (d_inputMode)
        {
        case TextInputMode::FloatingPoint:
        {
            std::streamsize defPrec = tmp.precision();
            tmp << std::fixed << std::setprecision(d_precision) << d_currentValue << std::setprecision(defPrec) << std::defaultfloat;
            break;
        }
        case TextInputMode::Integer:
            tmp << static_cast<int>(d_currentValue);
            break;
        case TextInputMode::Hexadecimal:
            tmp << std::hex << std::uppercase << static_cast<int>(d_currentValue) << std::dec;
            break;
        case TextInputMode::Octal:
            tmp << std::oct << static_cast<int>(d_currentValue) << std::dec;
            break;
        default:
            throw InvalidRequestException(
                "An unknown TextInputMode was encountered.");
        }

        return String(tmp.str().c_str());
    }

    void Spinner::onFontChanged(WindowEventArgs& e)
    {
        // Propagate to children
        getEditbox()->setFont(getActualFont());
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

        // Update editbox and spinner text with new value.
        // (allow empty and '-'/'+' cases to equal 0 with no text change required)
        if(d_currentValue != 0 || !ParserHelper::IsEmptyOrContainingSign(editbox->getText()))
        {
            const CEGUI::String& valueString = getTextFromValue();
            editbox->setText(valueString);
            setText(valueString);
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
        if (static_cast<const CursorInputEventArgs&>(e).source == CursorInputSource::Left)
        {
            setCurrentValue(d_currentValue + d_stepSize);
            return true;
        }

        return false;
    }

    bool Spinner::handleDecreaseButton(const EventArgs& e)
    {
        if (static_cast<const CursorInputEventArgs&>(e).source == CursorInputSource::Left)
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

} // End of  CEGUI namespace section
