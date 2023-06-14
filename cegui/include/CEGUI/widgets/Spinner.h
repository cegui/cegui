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
#ifndef _CEGUISpinner_h_
#define _CEGUISpinner_h_

#include "../Window.h"

namespace CEGUI
{
/*!
\brief
    Base class for the Spinner widget.

    The spinner widget has a text area where numbers may be entered
    and two buttons which may be used to increase or decrease the
    value in the text area by a user specified amount.
*/
class CEGUIEXPORT Spinner : public Window
{
public:

    // For easier changing or templating in the future
    using value_type = double;

    //! \brief Enumerated type specifying possible input and/or display modes for the spinner.
    enum class TextInputMode : int
    {
        FloatingPoint,  //!< Floating point decimal.
        Integer,        //!< Integer decimal.
        Hexadecimal,    //!< Hexadecimal.
        Octal,          //!< Octal.
        Invalid = -1    //!< Used internally for correct initial setup.
    };

    /*************************************************************************
        Events system constants
    *************************************************************************/
    static const String WidgetTypeName;                 //!< Window factory name
    static const String EventNamespace;                 //!< Namespace for global events
    /** Event fired when the spinner current value changes.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Spinner whose current value has
        * changed.
        */
    static const String EventValueChanged;
    /** Event fired when the minimum spinner value is changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::windows set to the Spinner whose minimum value has
        * been changed.
        */
    static const String EventMinimumValueChanged;
    /** Event fired when the maximum spinner value is changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Spinner whose maximum value has
        * been changed.
        */
    static const String EventMaximumValueChanged;
    /** Event fired when the spinner step value is changed.
        * Handlers area passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Spinner whose step value has
        * changed.
        */
    static const String EventStepChanged;
    /** Event fired when the spinner text input & display mode is changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Spinner whose text mode has been
        * changed.
        */
    static const String EventTextInputModeChanged;

    /*************************************************************************
        Component widget name strings
    *************************************************************************/
    static const String EditboxName;          //!< Widget name for the editbox thumb component.
    static const String IncreaseButtonName;   //!< Widget name for the increase button component.
    static const String DecreaseButtonName;   //!< Widget name for the decrease button component.

    Spinner(const String& type, const String& name);

    //! \copydoc Window::initialiseComponents
    void initialiseComponents() override;

    //! \brief Return the current spinner value.
    value_type getCurrentValue() const { return d_currentValue; }

    //! \brief Return the current minimum limit value for the Spinner.
    value_type getMinimumValue() const { return d_minValue; }

    //! \brief Return the current maximum limit value for the Spinner.
    value_type getMaximumValue() const { return d_maxValue; }

    //! \brief Return the value added to the spinner vaue when the up / down buttons are clicked.
    value_type getStepSize() const { return d_stepSize; }

    //! \brief Return the number of decimal places for the string representation.
    int getPrecision() const { return d_precision; }

    //! \brief Return the current text input / display mode setting.
    TextInputMode getTextInputMode() const { return d_inputMode; }

    //! \brief Return the fraction of the current value between min (0) and max (1).
    float getValueFraction() const { return static_cast<float>(d_currentValue - d_minValue) / static_cast<float>(d_maxValue - d_minValue); }

    /*!
    \brief
        Set the current spinner value.

    \param value
        value to be assigned to the Spinner.
    */
    void setCurrentValue(value_type value);

    /*!
    \brief
        Set the spinner minimum value.

    \param minValue
        The minimum value to be allowed by the spinner.
    */
    void setMinimumValue(value_type minValue);

    /*!
    \brief
        Set the spinner maximum value.

    \param maxValue
        The maximum value to be allowed by the spinner.
    */
    void setMaximumValue(value_type maxValue);

    /*!
    \brief
        Set the current step value.

    \param step
        The value added to be the spinner value when the
        up / down buttons are clicked.
    */
    void setStepSize(value_type step);

    /*!
    \brief
        Set precision (number of decimal places)

    \param val
        The floating point precision using by the spinner.
    */
    void setPrecision(int val);

    /*!
    \brief
        Set the spinner input / display mode.

    \param mode
        One of the TextInputMode enumerated values indicating the text
        input / display mode to be used by the spinner.
    */
    void setTextInputMode(TextInputMode mode);

protected:
    /*************************************************************************
        Constants
    *************************************************************************/
    static const String FloatValidator;      //!< Validator regex used for floating point mode.
    static const String IntegerValidator;    //!< Validator regex used for decimal integer mode.
    static const String HexValidator;        //!< Validator regex used for hexadecimal mode.
    static const String OctalValidator;      //!< Validator regex used for octal mode.

    /*************************************************************************
        Protected Implementation Methods
    *************************************************************************/
    /*!
    \brief
        Returns the numerical representation of the current editbox text.

    \return
        value that is the numerical equivalent of the editbox text.

    \exception InvalidRequestException  thrown if the text can not be converted.
    */
    virtual value_type getValueFromText() const;

    /*!
    \brief
        Returns the textual representation of the current spinner value.

    \return
        String object that is equivalent to the the numerical value of the spinner.
    */
    virtual String getTextFromValue() const;

    /*!
    \brief
        Return a pointer to the 'increase' PushButtoncomponent widget for
        this Spinner.

    \return
        Pointer to a PushButton object.

    \exception UnknownObjectException
        Thrown if the increase PushButton component does not exist.
    */
    PushButton* getIncreaseButton() const;

    /*!
    \brief
        Return a pointer to the 'decrease' PushButton component widget for
        this Spinner.

    \return
        Pointer to a PushButton object.

    \exception UnknownObjectException
        Thrown if the 'decrease' PushButton component does not exist.
    */
    PushButton* getDecreaseButton() const;

    /*!
    \brief
        Return a pointer to the Editbox component widget for this Spinner.

    \return
        Pointer to a Editbox object.

    \exception UnknownObjectException
        Thrown if the Editbox component does not exist.
    */
    Editbox* getEditbox() const;

    /*************************************************************************
        Overrides for Event handler methods
    *************************************************************************/
    void onFontChanged(WindowEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;
    void onActivated(ActivationEventArgs& e) override;
    void onScroll(ScrollEventArgs& e) override;
    void onKeyDown(KeyEventArgs& e) override;

    /*************************************************************************
        New Event handler methods
    *************************************************************************/
    /*!
    \brief
        Method called when the spinner value changes.

    \param e
        WindowEventArgs object containing any relevant data.
    */
    virtual void onValueChanged(WindowEventArgs& e);

    /*!
    \brief
        Method called when the minimum value setting changes.

    \param e
        WindowEventArgs object containing any relevant data.
    */
    virtual void onMinimumValueChanged(WindowEventArgs& e);

    /*!
    \brief
        Method called when the maximum value setting changes.

    \param e
        WindowEventArgs object containing any relevant data.
    */
    virtual void onMaximumValueChanged(WindowEventArgs& e);

    /*!
    \brief
        Method called when the step value changes.

    \param e
        WindowEventArgs object containing any relevant data.
    */
    virtual void onStepChanged(WindowEventArgs& e);

    /*!
    \brief
        Method called when the text input/display mode is changed.

    \param e
        WindowEventArgs object containing any relevant data.
    */
    virtual void onTextInputModeChanged(WindowEventArgs& e);

    /*************************************************************************
        Internal event listener methods
    *************************************************************************/
    bool handleIncreaseButton(const EventArgs& e);
    bool handleDecreaseButton(const EventArgs& e);
    bool handleEditTextChange(const EventArgs& e);

    /*************************************************************************
        Data Fields
    *************************************************************************/
    value_type    d_currentValue = 1.0;                 //!< Numerical copy of the text in d_editbox.
    value_type    d_minValue = -32768.0;                //!< Minimum value for spinner.
    value_type    d_maxValue = 32767.0;                 //!< Maximum value for spinner.
    value_type    d_stepSize = 1.0;                     //!< Step size value used y the increase & decrease buttons.
    int           d_precision = 6;                      //!< precision (number of decimal places)
    TextInputMode d_inputMode = TextInputMode::Invalid; //!< Current text display/input mode.

private:

    void addSpinnerProperties();
    void updateEditboxText();
};

template<>
class PropertyHelper<Spinner::TextInputMode>
{
public:
    typedef Spinner::TextInputMode return_type;
    typedef return_type safe_method_return_type;
    typedef Spinner::TextInputMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static const String type("TextInputMode");
        return type;
    }

    static return_type fromString(const String& str)
    {
        if (str == "FloatingPoint")
            return Spinner::TextInputMode::FloatingPoint;
        else if (str == "Hexadecimal")
            return Spinner::TextInputMode::Hexadecimal;
        else if (str == "Octal")
            return Spinner::TextInputMode::Octal;
        else if (str == "Invalid")
            return Spinner::TextInputMode::Invalid;
        else
            return Spinner::TextInputMode::Integer;
    }

    static string_return_type toString(pass_type val)
    {
        switch (val)
        {
            case Spinner::TextInputMode::FloatingPoint: return "FloatingPoint";
            case Spinner::TextInputMode::Integer: return "Integer";
            case Spinner::TextInputMode::Hexadecimal: return "Hexadecimal";
            case Spinner::TextInputMode::Octal: return "Octal";
            case Spinner::TextInputMode::Invalid: return "Invalid";
            default:
            {
                assert(false && "Unknown Text Input Mode");
                return "FloatingPoint";
            }
        }
    }
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUISpinner_h_
