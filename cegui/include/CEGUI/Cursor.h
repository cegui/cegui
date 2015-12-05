/***********************************************************************
	created:	21/2/2004
	author:		Paul D Turner

    purpose:    Defines interface for the Cursor class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUICursor_h_
#define _CEGUICursor_h_

#include "CEGUI/Base.h"
#include "CEGUI/String.h"
#include "CEGUI/UVector.h"
#include "CEGUI/Rectf.h"
#include "CEGUI/URect.h"
#include "CEGUI/EventSet.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/UDim.h"

#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//!	Class that provides cursor support.
class CEGUIEXPORT Cursor : public EventSet
{
public:
    //! Namespace for global events.
	static const String EventNamespace;

    /** Name of Event fired when the cursor image is changed.
     * Handlers are passed a const CursorEventArgs reference with
     * CursorEventArgs::d_cursor set to the Cursor that has
     * had it's image changed, and CursorEventArgs::image set to the
     * Image that is now set for the Cursor (may be 0).
     */
	static const String EventImageChanged;

    /** Name of Event fired when the Image to be used as a default cursor
     * image is changed.
     * Handlers are passed a const CursorEventArgs reference with
     * CursorEventArgs::Cursor set to the Cursor that has
     * had it's default image changed, and CursorEventArgs::image set to
     * the Image that is now set as the default (may be 0).
     */
	static const String EventDefaultImageChanged;

	/*!
	\brief
        Constructor for Cursor objects
	*/
	Cursor(void);


	/*!
	\brief
        Destructor for Cursor objects
	*/
	~Cursor(void);

	/*!
	\brief
        Set the current cursor image

	\param name
		String object holding the name of the desired Image.

	\return
		Nothing.

	\exception UnknownObjectException	thrown if Image \a name is not known.
	*/
	void	setImage(const String& name);


	/*!
	\brief
        Set the current cursor image
	*/
	void	setImage(const Image* image);


	/*!
	\brief
        Get the current cursor image
	\return
        The current image used to draw cursor.
	*/
	const Image*	getImage(void) const	{return d_indicatorImage;}

    /*!
    \brief
        Set the image to be used as the default cursor.

    \param image
        Pointer to an image object that is to be used as the default cursor
        indicator. To have no indicator rendered by default, you can specify 0 here.
    */
    void setDefaultImage(const Image* image);

    /*!
    \brief
        Set the image to be used as the default cursor.

    \param name
        String object that contains the name of the Image that is to be used.

    \exception
        UnknownObjectException thrown if no Image named \a name exists.
    */
    void setDefaultImage(const String& name);

    /*!
    \brief
        Return the currently set default cursor image

    \return
        Pointer to the current default image used for the cursor.  May
        return 0 if default cursor has not been set, or has intentionally
        been set to 0 - which results in a blank default cursor.
    */
    const Image* getDefaultImage() const;


	/*!
	\brief
        Makes the indicator draw itself

	*/
	void draw(void);


	/*!
	\brief
        Set the current cursor position

	\param position
        Point object describing the new location for the cursor. This will
        be clipped to within the renderer screen area.
	*/
    void setPosition(const glm::vec2& position);


	/*!
	\brief
        Offset the cursor position by the deltas specified in \a offset.

	\param offset
        Point object which describes the amount to move the indicator in each axis.

	\return
		Nothing.
	*/
    void offsetPosition(const glm::vec2& offset);


	/*!
	\brief
        Set the area that the cursor is constrained to.

	\param area
        Pointer to a Rect object that describes the area of the display that
        the cursor is allowed to occupy. The given area will be clipped to
        the current Renderer screen area - it is never possible for the cursor
        to leave this area. If this parameter is NULL, the constraint is set
        to the size of the current Renderer screen area.

	\return
		Nothing.
	*/
	void	setConstraintArea(const Rectf* area);


	/*!
	\brief
        Set the area that the cursor is constrained to.

	\param area
        Pointer to a URect object that describes the area of the display that
        the cursor is allowed to occupy. The given area will be clipped to the
        current Renderer screen area - it is never possible for the cursor to
        leave this area. If this parameter is NULL, the constraint is set to
        the size of the current Renderer screen area.

	\return
		Nothing.
	*/
	void	setUnifiedConstraintArea(const URect* area);


	/*!
	\brief
        Hides the cursor.

	\return
		Nothing.
	*/
	void	hide(void)		{d_visible = false;}


	/*!
	\brief
        Shows the cursor.

	\return
		Nothing.
	*/
	void	show(void)		{d_visible = true;}


    /*!
    \brief
        Set the visibility of the cursor.

    \param visible
        'true' to show the cursor, 'false' to hide it.

    \return
        Nothing.
    */
    void    setVisible(bool visible)    {d_visible = visible;}


	/*!
	\brief
        return whether the cursor is visible.

	\return
		true if the cursor is visible, false if the cursor is hidden.
	*/
	bool	isVisible(void) const	{return d_visible;}


	/*!
	\brief
        Return the current cursor position as a pixel offset from
        the top-left corner of the display.

	\return
		Point object describing the cursor position in screen pixels.
	*/
    inline glm::vec2 getPosition() const
    {
        return d_position;
    }

	/*!
	\brief
        Return the current constraint area of the cursor.

	\return
        Rect object describing the active area that the cursor is constrained to.
	*/
	Rectf getConstraintArea(void) const;


	/*!
	\brief
        Return the current constraint area of the cursor.

	\return
        URect object describing the active area that the cursor is constrained to.
	*/
	const URect& getUnifiedConstraintArea(void) const;


	/*!
	\brief
        Return the current cursor position as display resolution
        independent values.

	\return
        Point object describing the current cursor position as
        resolution independent values that anges from 0.0f to 1.0f, where 0.0f
        represents the left-most and top-most positions, and 1.0f represents
        the right-most and bottom-most positions.
	*/
    glm::vec2 getDisplayIndependantPosition(void) const;

    /*!
    \brief
        Function used to notify the Cursor of changes in the display size.

        You normally would not call this directly; rather you would call the
        function System::notifyDisplaySizeChanged and that will then call this
        function for you.

    \param new_size
        Size object describing the new display size in pixels.
    */
    void notifyDisplaySizeChanged(const Sizef& new_size);

    /*!
    \brief
        Set an explicit size for the cursor image to be drawn at.

        This will override the size that is usually obtained directly from the
        cursor image and will stay in effect across changes to the
        cursor image.

        Setting this size to (0, 0) will revert back to using the size as
        obtained from the Image itself.

    \param size
        Reference to a Size object that describes the size at which the indicator
        image should be drawn in pixels.
    */
    void setExplicitRenderSize(const Sizef& size);

    /*!
    \brief
        Return the explicit render size currently set.  A return size of (0, 0)
        indicates that the real image size will be used.
    */
    const Sizef& getExplicitRenderSize() const;

    /*!
    \brief
        Static function to pre-initialise the cursor position (prior to
        Cursor instantiation).

        Calling this function prior to instantiating Cursor will prevent
        the cursor having it's position set to the middle of the initial view.
        Calling this function after the Cursor is instantiated will have
        no effect.

    \param position
        vec2 object describing the initial pixel position to
        be used for the cursor.
    */
    static void setInitialPointerPosition(const glm::vec2& position);

    /*!
    \brief
        Mark the cached geometry as invalid so it will be recached next time the
        cursor is drawn.
    */
    void invalidate();

protected:
	/*************************************************************************
		New event handlers
	*************************************************************************/
    //! Event triggered internally when cursor image is changed.
    virtual void onImageChanged(CursorEventArgs& e);
    //! Event triggered internally when cursor default image is changed.
    virtual void onDefaultImageChanged(CursorEventArgs& e);

private:
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
        Checks the cursor position is within the current 'constrain'
        Rect and adjusts as required.
	*/
	void constrainPosition(void);

    //! updates the cached geometry.
    void cacheGeometry();

    //! calculate offset for custom image size so 'hot spot' is maintained.
    void calculateCustomOffset() const;

    /*!
    \brief
        Destroys the geometry buffers of this Cursor.
    */
    void destroyGeometryBuffers();

    /*!
    \brief
        Updates the translation of the geometry buffers of this Cursor.
    */
    void updateGeometryBuffersTranslation();

    /*!
    \brief
        Updates the clipping area of the geometry buffers of this Cursor.

    \param clipping_area
        The clipping area that will be applied to the geometry buffers of this Cursor.
    */
    void updateGeometryBuffersClipping(const Rectf& clipping_area);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
    //! Image that is currently set as the cursor.
	const Image* d_indicatorImage;
    //! Image that will be used as the default image for this cursor.
	const Image* d_defaultIndicatorImage;
    //! Current location of the indicator
    glm::vec2 d_position;
    //! true if the indicator will be drawn, else false.
    bool    d_visible;
    //! Specifies the area (in screen pixels) that the indicator can move around in.
    URect   d_constraints;
    //! buffer to hold geometry for cursor imagery.
    std::vector<GeometryBuffer*> d_geometryBuffers;
    //! custom explicit size to render the indicator image at
    Sizef d_customSize;
    //! correctly scaled offset used when using custom image size.
    mutable glm::vec2 d_customOffset;
    //! true if the cursor initial position has been pre-set
    static bool s_initialPositionSet;
    //! value set as initial position (if any)
    static glm::vec2 s_initialPosition;
    //! boolean indicating whether cached cursor geometry is valid.
    mutable bool d_cachedGeometryValid;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUICursor_h_
