/************************************************************************
    filename: 	TLSpinner.h
    created:	3/2/2005
    author:		Paul D Turner
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
#ifndef _TLSpinner_h_
#define _TLSpinner_h_

#include "TLModule.h"
#include "elements/CEGUISpinner.h"
#include "CEGUIWindowFactory.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Spinner widget class for the Taharez look Gui Scheme
    */
    class TAHAREZLOOK_API TLSpinner : public Spinner
    {
    public:
        /*************************************************************************
            Constants
        *************************************************************************/
        // type name for this widget
        static const utf8 WidgetTypeName[];   //!< The unique typename of this widget

        // image name constants
        static const utf8 ImagesetName[];                   //!< Name of the Imageset containing the imagery to use.
        static const utf8 UpButtonNormalImageName[];        //!< Name of image to use for the up button in normal state.
        static const utf8 UpButtonHighlightImageName[];	    //!< Name of image to use for the up button in highlighted state.
        static const utf8 DownButtonNormalImageName[];	    //!< Name of image to use for the down button in normal state.
        static const utf8 DownButtonHighlightImageName[];	//!< Name of image to use for the down button in the highlighted state.

        /*************************************************************************
           Object Construction and Destruction
        *************************************************************************/
        /*!
        \brief
            Constructor for Taharez spinner widgets
        */
        TLSpinner(const String& type, const String& name);

        /*!
        \brief
            Destructor for Taharez spinner widgets
        */
        virtual ~TLSpinner(void);

    protected:
        /*************************************************************************
            Constants
        *************************************************************************/
        // type names for the component widgets
        static const utf8* IncreaseButtonWidgetType;    //!< Type of widget to create for the increase button (up arrow).
        static const utf8* DecreaseButtonWidgetType;    //!< Type of widget to create for the decrease button (down arrow).
        static const utf8* EditboxWidgetType;           //!< Type of widget to create for the editbox.

        /*************************************************************************
        	Implementation of Spinner abstract methods
        *************************************************************************/
        PushButton* createIncreaseButton(const String& name) const;
        PushButton* createDecreaseButton(const String& name) const;
        Editbox* createEditbox(const String& name) const;
        void performChildWindowLayout();

        /*************************************************************************
        	Implementation of Window abstract methods
        *************************************************************************/
        void drawSelf(float z) { }

        /*************************************************************************
        	Data
        *************************************************************************/
        float d_buttonsWidth;
    };


    /*!
    \brief
        Factory class for producing TLSpinner objects
    */
    class TAHAREZLOOK_API TLSpinnerFactory : public WindowFactory
    {
    public:
        /*************************************************************************
            Object Construction and Destruction
        *************************************************************************/
        TLSpinnerFactory(void) : WindowFactory(TLSpinner::WidgetTypeName) { }
        ~TLSpinnerFactory(void){}


        /*!
        \brief
            Create a new Window object of whatever type this WindowFactory produces.

        \param name
            A unique name that is to be assigned to the newly created Window object

        \return
            Pointer to the new Window object.
        */
        Window* createWindow(const String& name);


        /*!
        \brief
            Destroys the given Window object.

        \param window
            Pointer to the Window object to be destroyed.

        \return
            Nothing.
        */
        virtual void destroyWindow(Window* window)	 { if (window->getType() == d_type) delete window; }
    };


} // End of  CEGUI namespace section


#endif	// end of guard _TLSpinner_h_
