/***********************************************************************
filename: 	CEGUITask.h
created:	12-5-07
author:		Jonathan Welch (Kokoro-Sama)
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

#ifndef _CEGUITask_h_
#define _CEGUITask_h_

#include "CEGUIBase.h"

class CEGUIEXPORT Task
{
public:
	Task(bool destroy = true):d_destroyOnComplete(destroy);
	~Task();

	/**
	 * Actual code to be run when the task is called.
	 * \param elapsed Amount of time since last frame update
	 * \return 
	 */
	virtual bool run(float elapsed) = 0;

protected:
	bool d_destroyOnComplete; //!< Does this task destory itself once it completes?

};

class CEGUIEXPORT WidgetFadeTask : public Task
{
public:

	enum eFadeDirection
	{
		FADEDIR_IN = 0,
		FADEDIR_OUT = 1
	}

	WidgetFadeTask(Window* widget,eFadeDirection fadedir, float targetalpha, bool destroy = true):Task(destroy){};

	/**
	 * Overridden from base task class -- Does the actual fading on the window.
	 * \param elapsed Amount of time since the last frame update
	 * \return True on success
	 */
	virtual bool run(float elapsed);

protected:
	eFadeDirection	d_fadeDirection; //!< Dictates which direction we are fading (in or out)
	float			d_targetAlpha; //!< What alpha are we fading to/from?
	Window*			d_widget;//!< what widget are we affecting?
};

#endif

