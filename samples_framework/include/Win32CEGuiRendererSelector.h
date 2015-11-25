/***********************************************************************
    created:    24/9/2004
    author:     Paul D Turner
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
    virtual bool    invokeDialog();


protected:
    /*!
    \brief
        Helper method to add entries for renderers to the Combobox and pre-select
        the first item entered.
    */
    bool addComboboxOption(HWND combo, const char* name, CEGuiRendererType rendererType);

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
    static int copyAnsiToWideChar(LPBYTE outBuff, LPCTSTR ansiString);


    /*************************************************************************
        Data fields
    *************************************************************************/
    LPDLGTEMPLATE   d_template;
};

#endif  // end of guard _Win32CEGuiRendererSelector_h_
