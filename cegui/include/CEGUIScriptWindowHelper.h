/***********************************************************************
filename: CEGUIScriptWindowHelper.h
created:  5-13-07
author:   Jonathan Welch (Kokoro-Sama)

purpose:  Header that includes the information for the scriptwindowhelper
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
#ifndef _CEGUIScriptWindowHelper_h_
#define _CEGUIScriptWindowHelper_h_

#include "CEGUIBase.h"

namespace CEGUI
{

	/**
	 *
	 * \ingroup CEGUIBase
	 *
	 *
	 * \par requirements
	 * winXP or later\n
	 *
	 * \remarks
	 * Used by Scriptmodules to help access certain functions or special objects
	 *
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 05-13-2007
	 *
	 * \author jwelch
	 *
	 * \par license
	 * Copyright 2007 Jon Welch & CEGUI Team
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class CEGUIEXPORT ScriptWindowHelper
	{
	public:
		ScriptWindowHelper(Window* wnd);
		ScriptWindowHelper() {}
		~ScriptWindowHelper() {}

		/**
		 * Returns a pointer to the window that this helper was created for.
		 * \return Window object to helped window
		 */
		Window* getWindow();
		/**
		 * Returns a pointer to the window object specified using the prefix of the helped window.
		 * \param name name of the window you wish to retrieve
		 * \return Window object to found window, or NULL
		 */
		Window* getWindow(String& name);

	protected:
		Window*		d_window; //!< Window that is being aided by this helper
		
	};
}
#endif
