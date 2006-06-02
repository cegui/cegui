/***********************************************************************
	filename: 	CEGUITabPane.h
	created:	8/8/2004
	author:		Steve Streeting
	
	purpose:	Defines interface for the content area of a tab control
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
#ifndef _CEGUITabPane_h_
#define _CEGUITabPane_h_

#include "elements/CEGUIGUISheet.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Base class for a tab pane.
*/
class CEGUIEXPORT TabPane : public DefaultWindow
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for tab pane widgets.
	*/
    TabPane(const String& type, const String& name) : DefaultWindow(type, name) {}


	/*!
	\brief
		Destructor for tab pane widgets.
	*/
    virtual ~TabPane(void) {}


	/*************************************************************************
		Accessors
	*************************************************************************/

	/*************************************************************************
		Manipulators
	*************************************************************************/

protected:
	/*************************************************************************
		Overridden from base class
	*************************************************************************/

    /*************************************************************************
		Implementation methods
	*************************************************************************/
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
		if (class_name=="TabPane")	return true;
		return DefaultWindow::testClassName_impl(class_name);
	}

	/*************************************************************************
		Implementation Data
	*************************************************************************/

private:
	/*************************************************************************
		Static Properties for this class
	*************************************************************************/


	/*************************************************************************
		Private methods
	*************************************************************************/
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUITabPane_h_
