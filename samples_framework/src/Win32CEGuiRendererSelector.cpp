/***********************************************************************
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "Win32CEGuiRendererSelector.h"
#include <tchar.h>


/*************************************************************************
    Constructor
*************************************************************************/
Win32CEGuiRendererSelector::Win32CEGuiRendererSelector() :
        d_template(createDialogTemplate())
{}


/*************************************************************************
    Destructor
*************************************************************************/
Win32CEGuiRendererSelector::~Win32CEGuiRendererSelector()
{
    if (d_template)
    {
        LocalFree(d_template);
    }
}


/*************************************************************************
    Display the dialog and wait for user
*************************************************************************/
bool Win32CEGuiRendererSelector::invokeDialog()
{
    // dialog template was not created so abort.
    if (!d_template)
        return false;

    int renderer_count = 0;
    CEGuiRendererType first_available = InvalidGuiRendererType;
    
    // Check number of renderer modules available
    for (int i = 0; i < RendererTypeCount; ++i)
    {
        if (d_rendererAvailability[i])
        {
            ++renderer_count;

            if (first_available == InvalidGuiRendererType)
                first_available = static_cast<CEGuiRendererType>(i);
        }
    }

    // if there is only one renderer, select that one, but do not show dialog
    if (renderer_count == 1)
    {
        d_lastSelected = first_available;
        return true;
    }

    // multiple renderer modules available, so show dialog & return result
    return (1 == DialogBoxIndirectParam(GetModuleHandle(0), d_template, 0, Win32CEGuiRendererSelector::dialogProcedure, reinterpret_cast<LPARAM>(this)));
}


/*************************************************************************
    Create Win32 dialog template
*************************************************************************/
LPDLGTEMPLATE Win32CEGuiRendererSelector::createDialogTemplate()
{
    SIZE_T  templateBufferSize = 1024;

    // allocate memory to hold the template we're going to construct
    LPDLGTEMPLATE dialogTemplate = static_cast<LPDLGTEMPLATE>(LocalAlloc(LPTR, templateBufferSize));

    // if allocation was successful
    if (dialogTemplate)
    {
        LPDLGITEMTEMPLATE item;
        LPBYTE buffer = reinterpret_cast<LPBYTE>(dialogTemplate);

        //
        // build template header
        //
        LPDLGTEMPLATE header    = reinterpret_cast<LPDLGTEMPLATE>(buffer);
        header->style           = DS_MODALFRAME|WS_CAPTION|WS_VISIBLE;
        header->dwExtendedStyle = 0;
        header->cdit            = 6;
        header->x               = (short)0x8000;
        header->y               = (short)0x8000;
        header->cx              = 150;
        header->cy              = 75;

        // advance buffer pointer
        buffer += sizeof(DLGTEMPLATE);

        //
        // Write null menu and class names
        //
        *reinterpret_cast<LPWORD>(buffer) = 0;
        buffer += sizeof(WORD);

        *reinterpret_cast<LPWORD>(buffer) = 0;
        buffer += sizeof(WORD);

        //
        // Write dialog title
        //
        int charCount = copyAnsiToWideChar(buffer, TEXT("CEGui - Renderer Selection"));
        buffer += charCount * sizeof(WORD);

        // align pointer for first item
        buffer = alignPointer(buffer);

        //
        // Buttons area static frame
        //
        item = reinterpret_cast<LPDLGITEMTEMPLATE>(buffer);
        item->style             = BS_GROUPBOX|WS_VISIBLE|WS_CHILD;
        item->dwExtendedStyle   = 0;
        item->x                 = 5;
        item->y                 = 48;
        item->cx                = 140;
        item->cy                = 22;
        item->id                = 0;

        // advance buffer pointer
        buffer += sizeof(DLGITEMTEMPLATE);

        // write class information
        *reinterpret_cast<LPWORD>(buffer) = 0xFFFF;
        buffer += sizeof(WORD);

        *reinterpret_cast<LPWORD>(buffer) = 0x0080;
        buffer += sizeof(WORD);

        // write caption
        charCount = copyAnsiToWideChar(buffer, TEXT(""));
        buffer += charCount * sizeof(WORD);

        // no creation data
        *reinterpret_cast<LPWORD>(buffer) = 0x0000;
        buffer += sizeof(WORD);

        // align pointer for next item
        buffer = alignPointer(buffer);

        //
        // Selection area static frame
        //
        item = reinterpret_cast<LPDLGITEMTEMPLATE>(buffer);
        item->style             = BS_GROUPBOX|WS_VISIBLE|WS_CHILD;
        item->dwExtendedStyle   = 0;
        item->x                 = 5;
        item->y                 = 0;
        item->cx                = 140;
        item->cy                = 50;
        item->id                = 0;

        // advance buffer pointer
        buffer += sizeof(DLGITEMTEMPLATE);

        // write class information
        *reinterpret_cast<LPWORD>(buffer) = 0xFFFF;
        buffer += sizeof(WORD);

        *reinterpret_cast<LPWORD>(buffer) = 0x0080;
        buffer += sizeof(WORD);

        // write caption
        charCount = copyAnsiToWideChar(buffer, TEXT(""));
        buffer += charCount * sizeof(WORD);

        // no creation data
        *reinterpret_cast<LPWORD>(buffer) = 0x0000;
        buffer += sizeof(WORD);

        // align pointer for next item
        buffer = alignPointer(buffer);

        //
        // Okay button
        //
        item = reinterpret_cast<LPDLGITEMTEMPLATE>(buffer);
        item->style             = BS_DEFPUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_TABSTOP;
        item->dwExtendedStyle   = 0;
        item->x                 = 9;
        item->y                 = 55;
        item->cx                = 40;
        item->cy                = 12;
        item->id                = IDOK;

        // advance buffer pointer
        buffer += sizeof(DLGITEMTEMPLATE);

        // write class information
        *reinterpret_cast<LPWORD>(buffer) = 0xFFFF;
        buffer += sizeof(WORD);

        *reinterpret_cast<LPWORD>(buffer) = 0x0080;
        buffer += sizeof(WORD);

        // write caption
        charCount = copyAnsiToWideChar(buffer, TEXT("Go!"));
        buffer += charCount * sizeof(WORD);

        // no creation data
        *reinterpret_cast<LPWORD>(buffer) = 0x0000;
        buffer += sizeof(WORD);

        // align pointer for next item
        buffer = alignPointer(buffer);

        //
        // Cancel button
        //
        item = reinterpret_cast<LPDLGITEMTEMPLATE>(buffer);
        item->style             = BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_TABSTOP;
        item->dwExtendedStyle   = 0;
        item->x                 = 101;
        item->y                 = 55;
        item->cx                = 40;
        item->cy                = 12;
        item->id                = IDCANCEL;

        // advance buffer pointer
        buffer += sizeof(DLGITEMTEMPLATE);

        // write class information
        *reinterpret_cast<LPWORD>(buffer) = 0xFFFF;
        buffer += sizeof(WORD);

        *reinterpret_cast<LPWORD>(buffer) = 0x0080;
        buffer += sizeof(WORD);

        // write caption
        charCount = copyAnsiToWideChar(buffer, TEXT("Cancel"));
        buffer += charCount * sizeof(WORD);

        // no creation data
        *reinterpret_cast<LPWORD>(buffer) = 0x0000;
        buffer += sizeof(WORD);

        // align pointer for next item
        buffer = alignPointer(buffer);

        //
        // Combo label
        //
        item = reinterpret_cast<LPDLGITEMTEMPLATE>(buffer);
        item->style             = SS_LEFT|WS_VISIBLE|WS_CHILD|WS_TABSTOP;
        item->dwExtendedStyle   = 0;
        item->x                 = 8;
        item->y                 = 7;
        item->cx                = 130;
        item->cy                = 12;
        item->id                = 0;

        // advance buffer pointer
        buffer += sizeof(DLGITEMTEMPLATE);

        // write class information
        *reinterpret_cast<LPWORD>(buffer) = 0xFFFF;
        buffer += sizeof(WORD);

        *reinterpret_cast<LPWORD>(buffer) = 0x0082;
        buffer += sizeof(WORD);

        // write caption
        charCount = copyAnsiToWideChar(buffer, TEXT("Select Renderer to Use:"));
        buffer += charCount * sizeof(WORD);

        // no creation data
        *reinterpret_cast<LPWORD>(buffer) = 0x0000;
        buffer += sizeof(WORD);

        // align pointer for next item
        buffer = alignPointer(buffer);

        //
        // Combobox
        //
        item = reinterpret_cast<LPDLGITEMTEMPLATE>(buffer);
        item->style             = CBS_DROPDOWNLIST|WS_VISIBLE|WS_CHILD;
        item->dwExtendedStyle   = 0;
        item->x                 = 8;
        item->y                 = 19;
        item->cx                = 130;
        item->cy                = 100;
        item->id                = 1000;

        // advance buffer pointer
        buffer += sizeof(DLGITEMTEMPLATE);

        // write class information
        *reinterpret_cast<LPWORD>(buffer) = 0xFFFF;
        buffer += sizeof(WORD);

        *reinterpret_cast<LPWORD>(buffer) = 0x0085;
        buffer += sizeof(WORD);

        // write caption
        charCount = copyAnsiToWideChar(buffer, TEXT(""));
        buffer += charCount * sizeof(WORD);

        // no creation data
        *reinterpret_cast<LPWORD>(buffer) = 0x0000;
        buffer += sizeof(WORD);

        // align pointer for next item
        buffer = alignPointer(buffer);

        return dialogTemplate;
    }

    return 0;
}

/*************************************************************************
    Checks availability of a renderer type and adds an entry to the
    combo box if it's available.  Returns true if the item was added.
*************************************************************************/
bool Win32CEGuiRendererSelector::addComboboxOption(HWND combo, const char* name, CEGuiRendererType rendererType)
{
    if (d_rendererAvailability[rendererType])
    {
        int idx = static_cast<int>(SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(name)));
        SendMessage(combo, CB_SETITEMDATA, idx, rendererType);

        // pre-select this item if it's the first one added.
        if (idx == 0)
        {
            SendMessage(combo, CB_SETCURSEL, 0, 0);
            d_lastSelected = rendererType;
        }
    }

    return d_rendererAvailability[rendererType];
}

/*************************************************************************
    Win32 dialog procedure function
*************************************************************************/
INT_PTR CALLBACK Win32CEGuiRendererSelector::dialogProcedure(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_INITDIALOG:
        {
            // get the 'this' ptr we were passed
            Win32CEGuiRendererSelector* obj = reinterpret_cast<Win32CEGuiRendererSelector*>(lParam);

            // set as window long for future use
            SetWindowLongPtr(hDlg, DWLP_USER, static_cast<LONG_PTR>(lParam));

            //
            // Set-up combo box list
            //
            // get combo control
            HWND combo = GetDlgItem(hDlg, 1000);

            if (combo)
            {
                // clear old data
                SendMessage(combo, CB_RESETCONTENT, 0, 0);

                // add new stings according to if item is enabled or not
                obj->addComboboxOption(combo, "OpenGL 3.2 Core Renderer", OpenGL3GuiRendererType);
                obj->addComboboxOption(combo, "OpenGL ES 2.0 Renderer", OpenglEs2GuiRendererType);
                obj->addComboboxOption(combo, "OpenGL Renderer", OpenGLGuiRendererType);
                obj->addComboboxOption(combo, "Ogre Engine Renderer", OgreGuiRendererType);
                obj->addComboboxOption(combo, "Microsoft Direct3D 9 Renderer", Direct3D9GuiRendererType);
                obj->addComboboxOption(combo, "Microsoft Direct3D 10 Renderer", Direct3D10GuiRendererType);
                obj->addComboboxOption(combo, "Microsoft Direct3D 11 Renderer", Direct3D11GuiRendererType);
                obj->addComboboxOption(combo, "Irrlicht Engine Renderer", IrrlichtGuiRendererType);
            }

            return FALSE;
        }

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hDlg, 1);
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, 0);
            return TRUE;

            // Combo-box
        case 1000:
            switch(HIWORD(wParam))
            {
            case CBN_SELENDOK:
                {
                    HWND combo = reinterpret_cast<HWND>(lParam);

                    // get the 'this' ptr for the object we were created by
                    Win32CEGuiRendererSelector* obj = reinterpret_cast<Win32CEGuiRendererSelector*>(GetWindowLongPtr(hDlg, DWLP_USER));

                    int idx = static_cast<int>(SendMessage(combo, CB_GETCURSEL, 0, 0));

                    if (idx != CB_ERR)
                    {
                        // set last selected renderer type
                        obj->d_lastSelected = static_cast<CEGuiRendererType>(SendMessage(combo, CB_GETITEMDATA, idx, 0));
                    }

                    return TRUE;
                }
            }
        }

        break;
    }

    return FALSE;
}


/*************************************************************************
    Take an input pointer, return closest pointer that is aligned on a
    DWORD (4 byte) boundary.
*************************************************************************/
LPBYTE Win32CEGuiRendererSelector::alignPointer(LPBYTE buff)
{
    DWORD_PTR ul = reinterpret_cast<DWORD_PTR>(buff);
    ul +=3;
    ul >>=2;
    ul <<=2;
    return reinterpret_cast<LPBYTE>(ul);
}


/*************************************************************************
    Converts the Ansi string in 'pAnsiIn' into wide characters and
    copies the result into the WORD array at 'pWCStr'.
*************************************************************************/
int Win32CEGuiRendererSelector::copyAnsiToWideChar(LPBYTE outBuff, LPCTSTR ansiString)
{
    LPWSTR pWCStr = reinterpret_cast<LPWSTR>(outBuff);

#   ifdef UNICODE
    return lstrlen(lstrcpy(pWCStr, ansiString)) + 1;
#   else
    int cchAnsi = lstrlen(ansiString);
    return MultiByteToWideChar(GetACP(), MB_PRECOMPOSED, ansiString, cchAnsi, pWCStr, cchAnsi) + 1;
#   endif
}
