/************************************************************************
    filename:   Win32CEGuiRendererSelector.h
    created:    24/9/2004
    author:     Paul D Turner
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
#ifndef _Win32CEGuiRendererSelector_h_
#define _Win32CEGuiRendererSelector_h_

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define WIN32_LEAN_AND_MEAN
#   include "windows.h"
#endif

#include "CEGuiRendererSelector.h"


/*!
\brief
    Class representing a modal renderer selection dialog under Win32.
*/
class Win32CEGuiRendererSelector : public CEGuiRendererSelector
{
public:
    /*!
    \brief
        Constructor
    */
    Win32CEGuiRendererSelector();


    /*!
    \brief
        Destructor.
    */
    virtual ~Win32CEGuiRendererSelector();


    /*!
    \brief
        Displays a dialog allowing the user to select a renderer to be used.

    \return
        false if the user cancelled.
    */
    virtual bool    inkokeDialog();


protected:
    /*************************************************************************
        Static methods
    *************************************************************************/
    /*!
    \brief
        Create a Win32 dialog template for the requester.
    */
    static LPDLGTEMPLATE createDialogTemplate();


    /*!
    \brief
        Win32 dialog procedure function.
    */
    static INT_PTR CALLBACK dialogProcedure(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


    /*!
    \brief
        Take an input pointer, return closest pointer that is aligned on a DWORD (4 byte) boundary.

    \note
        This is taken from the DynDlg sample in the Win32 Platform SDK.
    */
    static LPBYTE alignPointer(LPBYTE buff);


    /*!
    \brief
        Converts the Ansi string in 'pAnsiIn' into wide characters and copies the result
        into the WORD array at 'pWCStr'.

    \note
        This is taken from the DynDlg sample in the Win32 Platform SDK.

    \param outBuff
        Pointer to the destination buffer.

    \param pAnsiIn
        Input ANSI string.

    \return
        The number of wide characters copied, including the trailing wide char NULL.
    */
    static int copyAnsiToWideChar(LPBYTE outBuff, PTSTR ansiString);


    /*************************************************************************
        Data fields
    *************************************************************************/
    LPDLGTEMPLATE   d_template;
};

#endif  // end of guard _Win32CEGuiRendererSelector_h_
