/***********************************************************************
    filename:   CEGUIGUISheet.h
    created:    5/6/2004
    author:     Paul D Turner

    purpose:    Interface to a default window
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
#ifndef _CEGUIGUISheet_h_
#define _CEGUIGUISheet_h_

#include "../CEGUIWindow.h"
#include "../CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Window class intended to be used as a simple, generic Window.

    This class does no rendering and so appears totally transparent.  This window defaults
    to position 0.0f, 0.0f with a size of 1.0f x 1.0f.

    /note
    The C++ name of this class has been retained for backward compatibility reasons.  The intended usage of
    this window type has now been extended beyond that of a gui-sheet or root window.
*/
class CEGUIEXPORT GUISheet : public Window
{
public:
    /*************************************************************************
        Constants
    *************************************************************************/
    // type name for this widget
    static const String WidgetTypeName;             //!< The unique typename of this widget


    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    /*!
    \brief
        Constructor for GUISheet windows.
    */
    GUISheet(const String& type, const String& name);


    /*!
    \brief
        Destructor for GUISheet windows.
    */
    virtual ~GUISheet(void) {}


protected:
	/*!
	\brief
		Return whether this window was inherited from the given class name at some point in the inheritance hierarchy.

	\param class_name
		The class name that is to be checked.

	\return
		true if this window was inherited from \a class_name. false if not.
	*/
	virtual bool	testClassName_impl(const String& class_name) const
	{
		if (class_name=="DefaultWindow" || class_name=="GUISheet")	return true;
		return Window::testClassName_impl(class_name);
	}

    //! helper to update mouse input handled state
    void updateMouseEventHandled(MouseEventArgs& e) const;

    // overridden functions from Window base class
    bool moveToFront_impl(bool wasClicked);

    // override the mouse event handlers
    void onMouseMove(MouseEventArgs& e);
    void onMouseWheel(MouseEventArgs& e);
    void onMouseButtonDown(MouseEventArgs& e);
    void onMouseButtonUp(MouseEventArgs& e);
    void onMouseClicked(MouseEventArgs& e);
    void onMouseDoubleClicked(MouseEventArgs& e);
    void onMouseTripleClicked(MouseEventArgs& e);
};


/*!
\brief
    typedef for DefaultWindow, which is the new name for GUISheet.
*/
typedef GUISheet DefaultWindow;


} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIGUISheet_h_
