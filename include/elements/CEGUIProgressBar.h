/************************************************************************
	filename: 	CEGUIProgressBar.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for ProgressBar widget
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
#ifndef _CEGUIProgressBar_h_
#define _CEGUIProgressBar_h_

#include "CEGUIBase.h"
#include "CEGUIWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Base class for progress bars.
*/
class CEGUIBASE_API ProgressBar : public Window
{
public:
	/*************************************************************************
		Event name constants
	*************************************************************************/
	static const utf8	ProgressChanged[];		//!< Event fired whenever the progress value changes.
	static const utf8	ProgressDone[];			//!< Event fired when the progress bar reaches 100%.	


	/************************************************************************
		Accessor Functions
	************************************************************************/
	/*!
	\brief
		return the current progress value
	*/
	float	getProgress(void) const		{return d_progress;}

	/*!
	\brief
		return the current step size
	*/
	float	getStep(void) const			{return d_step;}


	/*************************************************************************
		Manipulator Functions
	*************************************************************************/
	/*!
	\brief
		set the current progress.

	\param progress
		The level of progress to set.  If this value is >1.0f (100%) progress will be limited to 1.0f.

	\return
		Nothing.
	*/
	void	setProgress(float progress);


	/*!
	\brief
		set the size of the 'step' in percentage points (default is 0.01f or 1%).

	\param step
		Amount to increase the progress by each time the step method is called.

	\return
		Nothing.
	*/
	void	setStepSize(float step)		{d_step = step;}


	/*!
	\brief
		cause the progress to step

		The amount the progress bar will step can be changed by calling the setStepSize method.  The
		default step size is 0.01f which is equal to 1%.
	
	\return
		Nothing.
	*/
	void	step(void)			{setProgress(d_progress + d_step);}


	/*!
	\brief
		Modify the progress level by a specified delta.

	\param delta
		amount to adjust the progress by.  Whatever this value is, the progress of the bar will be kept
		within the range: 0.0f <= progress <= 1.0f.

	\return
		Nothing.
	*/
	void	adjustProgress(float delta)		{setProgress(d_progress + delta);}


protected:
	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for ProgressBar class
	*/
	ProgressBar(const String& type, const String& name);


	/*!
	\brief
		Destructor for ProgressBar
	*/
	virtual ~ProgressBar(void);


	/*************************************************************************
		Implementation methods
	*************************************************************************/
	/*!
	\brief
		Add progress bar specific events to the window
	*/
	void	addProgressBarEvents(void);


	/*************************************************************************
		Event handlers for progress bar specific events
	*************************************************************************/
	/*!
	\brief
		event triggered when progress changes
	*/
	virtual	void	onProgressChanged(WindowEventArgs& e);


	/*!
	\brief
		event triggered when progress reaches 100%
	*/
	virtual void	onProgressDone(WindowEventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	float	d_progress;		//!< current progress (from 0.0f to 1.0f)
	float	d_step;			//!< amount to 'step' progress by on a call to step()
};


} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIProgressBar_h_
