/***********************************************************************
    filename:   CEGUITimer.h
    created:    08/08/2006
    author:     Paul D Turner

    purpose:    Interface to timer window 
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

#include "CEGUIWindow.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Window class intended to be used as a timer. 
    
    
    An application can contains several timer to generate events each count milliseconds. 
    The accuracy of the timer is not accurate enough to do any time counting and is 
    not guaranty either.
    
    This class does no rendering and so appears totally transparent.  This window defaults
    to position 0.0f, 0.0f with a size of 1.0f x 1.0f.

*/
class CEGUIEXPORT Timer : public Window
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
        Constructor for Timer windows.
    */
    Timer(const String& type, const String& name);


    /*!
    \brief
        Destructor for Timer windows.
    */
    virtual ~Timer(void) {}

    /*! 
    \brief 
        Set the delay between to event generation 
     */
    void setDelay(size_t seconds);

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
		if (class_name=="Timer")	
            return true;
		return Window::testClassName_impl(class_name);
	}
};


/*!
\brief
    typedef for DefaultWindow, which is the new name for GUISheet.
*/
typedef GUISheet DefaultWindow;


} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIGUISheet_h_
