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
#ifndef _CEGuiRendererSelector_h_
#define _CEGuiRendererSelector_h_

/*!
\brief
    Enumeration of available renderer types.
*/
enum CEGuiRendererType
{
    OgreGuiRendererType,            //!< Renderer that uses the Ogre engine.
    Direct3D9GuiRendererType,       //!< Renderer that uses the Direct3D 9 API.
    Direct3D10GuiRendererType,      //!< Renderer that uses the Direct3D 10 API.
    Direct3D11GuiRendererType,      //!< Renderer that uses the Direct3D 11 API.
    OpenGLGuiRendererType,          //!< Renderer that uses the desktop OpenGL 1.2 API.
    OpenGL3GuiRendererType,         //!< Renderer that uses the desktop OpenGL 3.2 API.
    OpenglEs2GuiRendererType,       //!< Renderer that uses the OpenGL ES 2.0 API.
    IrrlichtGuiRendererType,        //!< Renderer that uses the Irrlicht engine.
    DirectFBGuiRendererType,        //!< Renderer that uses the DirectFB engine.
    RendererTypeCount,              //!< Special value that equals the number of renderer types.
    InvalidGuiRendererType          //!< Special value used to represent an invalid selection.
};


/*!
\brief
    Base class for the renderer selection dialog class.
 
    This should be sub-classed to do something useful.  Initialisation should happen in the constructor and cleanup
    in the destructor.  The invokeDialog should only display the dialog (and not return until the dialog is dismissed).
*/
class CEGuiRendererSelector
{
public:
    /*!
    \brief
        Constructor.
    */
    CEGuiRendererSelector();


    /*!
    \brief
        Destructor.
    */
    virtual ~CEGuiRendererSelector();


    /*!
    \brief
        Displays a dialog allowing the user to select a renderer to be used.

    \return
        false if the user cancelled.
    */
    virtual bool invokeDialog()  = 0;


    /*!
    \brief
        Return the CEGuiRendererType value of the renderer most recently selected by the user.

    \return
        One of the CEGuiRendererType enumerated values.
    */
    CEGuiRendererType getSelectedRendererType();


    /*!
    \brief
        Set whether or not a specific renderer type will be available for selection from the dialog the next time it
        is displayed.

    \param rendererType
        One of the CEGuiRendererType enumerated values representing the renderer whos availability will be set.

    \param available
        - true if this renderer should be available to the user.
        - false if this renderer should not be available.
    */
    void setRendererAvailability(CEGuiRendererType rendererType, bool available = true);


protected:
    CEGuiRendererType   d_lastSelected;         //!< Holds the last selected renderer type.
    bool    d_rendererAvailability[RendererTypeCount];  //!< Holds availability of renderer types.
};

#endif  // end of guard _CEGuiRendererSelector_h_
