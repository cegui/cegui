/***********************************************************************
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for Slider widget
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
#ifndef _CEGUISlider_h_
#define _CEGUISlider_h_

#include "../Window.h"
#include "../WindowRenderer.h"

namespace CEGUI
{

class CEGUIEXPORT SliderWindowRenderer : public WindowRenderer
{
public:

    // For easier changing or templating in the future
    using value_type = double;

    SliderWindowRenderer(const String& name);

    //! \brief update the size and location of the thumb to properly represent the current state of the slider
    virtual void updateThumb() = 0;

    //! \brief return value that best represents current slider value given the current location of the thumb.
    virtual value_type getValueFromThumb() const = 0;

    /*!
    \brief
        Given window location \a pt, return a value indicating what change should be 
        made to the slider.

    \param pt
        Point object describing a pixel position in window space.

    \return
        - -1 to indicate slider should be moved to a lower setting.
        -  0 to indicate slider should not be moved.
        - +1 to indicate slider should be moved to a higher setting.
    */
    virtual float getAdjustDirectionFromPoint(const glm::vec2& pt) const = 0;

    virtual bool isVertical() const { return false; }
};

/*!
\brief
	Base class for Slider widgets.

	The slider widget has a default range of 0.0f - 1.0f.  This enables use of the slider value to scale
	any value needed by way of a simple multiplication.
*/
class CEGUIEXPORT Slider : public Window
{
public:

    // For easier changing or templating in the future
    using value_type = double;

    static const String EventNamespace;				//!< Namespace for global events
    static const String WidgetTypeName;             //!< Window factory name

	/*************************************************************************
		Event name constants
	*************************************************************************/
    /** Event fired when the slider value changes.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Slider whose value has changed.
     */
	static const String EventValueChanged;
    /** Event fired when the minimum slider value is changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::windows set to the slider whose minimum value has
        * been changed.
        */
    static const String EventMinimumValueChanged;
    /** Event fired when the maximum slider value is changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the slider whose maximum value has
        * been changed.
        */
    static const String EventMaximumValueChanged;
    /** Event fired when the slider step value is changed.
        * Handlers area passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the slider whose step value has
        * changed.
        */
    static const String EventStepChanged;
    /** Event fired when the user begins dragging the thumb.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Slider whose thumb has started to
     * be dragged.
     */
	static const String EventThumbTrackStarted;
    /** Event fired when the user releases the thumb.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Slider whose thumb has been released.
     */
	static const String EventThumbTrackEnded;

    /*************************************************************************
        Child Widget name suffix constants
    *************************************************************************/
    static const String ThumbName;            //!< Widget name for the thumb component.


    Slider(const String& type, const String& name);

    void initialiseComponents() override;

    //! \brief Return the current slider value.
    value_type getCurrentValue() const { return d_currentValue; }

    //! \brief Return the current minimum limit value for this widget.
    value_type getMinimumValue() const { return d_minValue; }

    //! \brief Return the current maximum limit value for this widget.
    value_type getMaximumValue() const { return d_maxValue; }

    /*!
    \brief
        Return the current click step setting for the slider.

        The click step size is the amount the slider value will be adjusted when the widget
        is clicked wither side of the slider thumb.
    */
    value_type getStepSize() const { return d_stepSize; }

    //! \brief Return the fraction of the current value between min (0) and max (1).
    float getValueFraction() const { return static_cast<float>(d_currentValue - d_minValue) / static_cast<float>(d_maxValue - d_minValue); }

    //! \brief Round the value passed to the nearest step
    value_type roundToStep(value_type value) const;

    //! \brief Set the current slider value.
    void setCurrentValue(value_type value);

    //! \brief Set the minimum value for the slider.
    void setMinimumValue(value_type minValue);

    //! \brief Set the maximum value for the slider.
    void setMaximumValue(value_type maxValue);

	/*!
	\brief
		Set the current click step setting for the slider.

		The click step size is the amount the slider value will be adjusted when the widget
		is clicked wither side of the slider thumb.

	\param step
		value representing the click step setting to use.
	*/
	void setStepSize(value_type step) { d_stepSize = step; }

    /*!
    \brief
        Return whether this slider changes the value discretely between steps

    \see
        Slider::setStepSize
    */
    bool isDiscrete() const { return d_discrete; }

    /*!
    \brief
        Set true for discrete mode and false for continuous

    \see
        Slider::setStepSize
    */
    void setDiscrete(bool discrete);

    bool isVertical() const;

    /*!
    \brief
        Return a pointer to the Thumb component widget for this Slider.

    \return
        Pointer to a Thumb object.

    \exception UnknownObjectException
        Thrown if the Thumb component does not exist.
    */
    Thumb* getThumb() const;

protected:

    // Delegated to the window renderer
    virtual void updateThumb();
	virtual value_type getValueFromThumb() const;
    virtual float getAdjustDirectionFromPoint(const glm::vec2& pt) const;

    bool validateWindowRenderer(const WindowRenderer* renderer) const override;

	bool handleThumbMoved(const EventArgs& e);
	bool handleThumbTrackStarted(const EventArgs& e);
	bool handleThumbTrackEnded(const EventArgs& e);

	/*************************************************************************
		New event handlers for slider widget
	*************************************************************************/
	//! \brief Handler triggered when the slider value changes
	virtual void onValueChanged(WindowEventArgs& e);

    //! \brief Method called when the minimum value setting changes.
    virtual void onMinimumValueChanged(WindowEventArgs& e);

    //! \brief Method called when the maximum value setting changes.
    virtual void onMaximumValueChanged(WindowEventArgs& e);

    //! \brief Method called when the step value changes.
    virtual void onStepChanged(WindowEventArgs& e);

	//! \brief Handler triggered when the user begins to drag the slider thumb. 
	virtual void onThumbTrackStarted(WindowEventArgs& e);

	//! \brief Handler triggered when the slider thumb is released
	virtual void onThumbTrackEnded(WindowEventArgs& e);

	/*************************************************************************
		Overridden event handlers
	*************************************************************************/
    void onMouseButtonDown(MouseButtonEventArgs& e) override;
    void onScroll(ScrollEventArgs& e) override;
    void onKeyDown(KeyEventArgs& e) override;

	/*************************************************************************
		Implementation Data
	*************************************************************************/
    value_type d_currentValue = 0.0; //!< current slider value
    value_type d_minValue = 0.0;     //!< slider minimum value
    value_type d_maxValue = 1.0;	 //!< slider maximum value
    value_type d_stepSize = 0.01;    //!< amount to adjust slider by when clicked (and not dragged).
    bool       d_discrete = false;   //!< the value will change only by step

private:

	void addSliderProperties();
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUISlider_h_
