/************************************************************************
	filename: 	TLAlternateProgressBar.cpp
	created:	23/5/2004
	author:		Paul D Turner
	
	purpose:	Interface to the alternate Taharez progress bar
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#ifndef _TLAlternateProgressBar_cpp_
#define _TLAlternateProgressBar_cpp_

#include "TLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIProgressBar.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// number of light images we have for this alternate progress bar
#define		TLAPB_LightImageCount		10


/*!
\brief
	Alternate progress bar for the Taharez Gui Scheme.
*/
class TAHAREZLOOK_API TLAlternateProgressBar : public ProgressBar
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// Progress bar image names
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	ContainerLeftImageName[];		//!< Name of image to use for left end of container.
	static const utf8	ContainerMiddleImageName[];		//!< Name of image to use for middle of container.
	static const utf8	ContainerRightImageName[];		//!< Name of image to use foe right of container.
	static const utf8	QuartersMarkImageName[];		//!< Name of image to use for quarter calibration marks.
	static const utf8	HalvesMarkImageName[];			//!< Name of image to use for halfs calibration mark.
	static const utf8	Light1ImageName[];				//!< Name of image for light 1.
	static const utf8	Light2ImageName[];				//!< Name of image for light 2.
	static const utf8	Light3ImageName[];				//!< Name of image for light 3.
	static const utf8	Light4ImageName[];				//!< Name of image for light 4.
	static const utf8	Light5ImageName[];				//!< Name of image for light 5.
	static const utf8	Light6ImageName[];				//!< Name of image for light 6.
	static const utf8	Light7ImageName[];				//!< Name of image for light 7.
	static const utf8	Light8ImageName[];				//!< Name of image for light 8.
	static const utf8	Light9ImageName[];				//!< Name of image for light 9.
	static const utf8	Light10ImageName[];				//!< Name of image for light 10.

	// some offsets
	static const float	FirstLightPaddingRatio;			//!< Value used to calculate required offset for first light.


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez alternate progress bar objects
	*/
	TLAlternateProgressBar(const String& type, const String& name);


	/*!
	\brief
		Destructor for Taharez alternate progress bar objects
	*/
	virtual ~TLAlternateProgressBar(void);


protected:
	/*************************************************************************
		Overridden Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		Perform rendering for this widget
	*/
	virtual void	drawSelf(float z);


	/*************************************************************************
		Overridden Event Handling Functions
	*************************************************************************/
	/*!
	\brief
		handler called when widget is sized
	*/
	virtual void	onSized(WindowEventArgs& e);


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		function to initialise data relating to lights that should be rendered.
	*/
	void	setupLightsInfo(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	int		d_lightCount;	//!< number of lights to render
	float	d_lightSpacing;	//!< space between lights for rendering
	int*	d_lights;		//!< holds pointer to array of light image indeces.
	int		d_lightsSize;	//!< size of d_lights array

	// images
	const Image*	d_left;				//!< Left end of container.
	const Image*	d_right;			//!< Right end of container.
	const Image*	d_middle;			//!< Middle of container.
	const Image*	d_quarterMark;		//!< Quarters calibration mark.
	const Image*	d_halvesMark;		//!< Halves calibration mark.
	const Image*	d_lightImages[TLAPB_LightImageCount];		//!< Light images.
};


/*!
\brief
	Factory class for producing TLAlternateProgressBar objects
*/
class TAHAREZLOOK_API TLAlternateProgressBarFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLAlternateProgressBarFactory(void) : WindowFactory(TLAlternateProgressBar::WidgetTypeName) { }
	~TLAlternateProgressBarFactory(void){}


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


#endif	// end of guard _TLAlternateProgressBar_cpp_
