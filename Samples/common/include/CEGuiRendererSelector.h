/************************************************************************
    filename:   CEGuiRendererSelector.h
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
#ifndef _CEGuiRendererSelector_h_
#define _CEGuiRendererSelector_h_

/*!
\brief
    Enumeration of available renderer types.
*/
enum CEGuiRendererType
{
    OgreGuiRendererType,            //!< Renderer that uses the Ogre engine.
    Direct3D81GuiRendererType,      //!< Renderer that uses the Direct3D 8.1 API.
    Direct3D9GuiRendererType,       //!< Renderer that uses the Direct3D 9 API.
    OpenGLGuiRendererType,          //!< Renderer that uses the OpenGL API.
    IrrlichtGuiRendererType,        //!< Renderer that uses the Irrlicht engine.
    RendererTypeCount,              //!< Special value that equals the number of renderer types.
    InvalidGuiRendererType          //!< Special value used to represent an invalid selection.
};


/*!
\brief
    Base class for the renderer selection dialog class.
 
    This should be sub-classed to do something useful.  Initialisation should happen in the constructor and cleanup
    in the destructor.  The inkokeDialog should only display the dialog (and not return until the dialog is dismissed).
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
    virtual bool inkokeDialog()  = 0;


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
