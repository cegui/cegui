/************************************************************************
	filename: 	TLTabControl.h
	created:	10/08/2004
	author:		Steve Streeting
	
	purpose:	Defines interface for Taharez Look Tab Control class.
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _TLTabControl_h_
#define _TLTabControl_h_

#include "TLModule.h"
#include "elements/CEGUITabControl.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Concrete TabControl class for the Taharez Look widget set.	
*/
class TAHAREZLOOK_API TLTabControl : public TabControl
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

    static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
    static const utf8	FillerImageName[];			    //!< Name of the image to use for filling the top edge where there are no buttons

	// window type stuff
	static const utf8*	TabContentPaneType;				//!< Window type to create for the tab pane.
    static const utf8*	TabButtonType;				    //!< Window type to create for the tab button.

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Look tab control objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	TLTabControl(const String& type, const String& name);


	/*!
	\brief
		Destructor for TLTabControl objects.
	*/
	virtual ~TLTabControl(void);

protected:
	/*************************************************************************
		Overridden event handlers
	*************************************************************************/

	/*************************************************************************
		Implementation Functions
	*************************************************************************/
    /*!
    \brief
    create and return a pointer to a TabPane widget for use as content area

    \return
    Pointer to a TabPane to be used for holding the main content.
    */
    TabPane*	createTabContentPane(void) const;

    /*!
    \brief
        create and return a pointer to a TabButton widget

    */
    TabButton* createTabButton(const String& name) const;

    /*!
    \brief
        Setup size and position for the component widgets attached to this TabControl

    \return
        Nothing.
    */
    void layoutComponentWidgets();

    /*!
    \brief
        Perform the actual rendering for this Window. 

    \param z
        float value specifying the base Z co-ordinate that should be used when rendering

    \return
        Nothing
    */
    void drawSelf(float z);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
    // standard button rendering images
    const Image*	d_fillerImage;			//!< Image to use when rendering the filler section
    Vector3         d_fillerPos;
    Size            d_fillerSize;

};


/*!
\brief
	Factory class for producing TLFrameWindow objects
*/
class TAHAREZLOOK_API TLTabControlFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Frame Window factory class.
	*/
	TLTabControlFactory(void) : WindowFactory(TLTabControl::WidgetTypeName) { }


	/*
	\brief
		Destructor for Taharez Frame Window factory class.
	*/
	~TLTabControlFactory(void){}


	/*!
	\brief
		Create a new Window object of whatever type this WindowFactory produces.

	\param name
		A unique name that is to be assigned to the newly created Window object

	\return
		Pointer to the new Window object.
	*/
	Window*	createWindow(const String& name);


	/*!
	\brief
		Destroys the given Window object.

	\param window
		Pointer to the Window object to be destroyed.

	\return
		Nothing.
	*/
	virtual void	destroyWindow(Window* window)	 { if (window->getType() == d_type) delete window; }
};


} // End of  CEGUI namespace section

#endif	// end of guard _TLFrameWindow_h_
