/************************************************************************
	filename: 	CEGUIDragContainer.h
	created:	14/2/2005
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
#ifndef _CEGUIDragContainer_h_
#define _CEGUIDragContainer_h_

#include "CEGUIWindow.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIDragContainerProperties.h"


// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Generic drag & drop enabled window class
    */
    class CEGUIBASE_API DragContainer : public Window
    {
    public:
        /*************************************************************************
            Constants
        *************************************************************************/
        static const String WidgetTypeName;     //!< Type name for DragContainer.
        static const String EventNamespace;     //!< Namespace for global events
        static const String EventDragStarted;   //!< Name of the event fired when the user begins dragging the thumb.
        static const String EventDragEnded;     //!< Name of the event fired when the user releases the thumb.
        static const String EventDragPositionChanged;   //!< Event fired when the drag position has changed.
        static const String EventDragEnabledChanged;    //!< Event fired when dragging is enabled or disabled.
        static const String EventDragAlphaChanged;      //!< Event fired when the alpha value used when dragging is changed.
        static const String EventDragMouseCursorChanged;//!< Event fired when the mouse cursor used when dragging is changed.
        static const String EventDragThresholdChanged;  //!< Event fired when the drag pixel threshold is changed.
        static const String EventDragDropTargetChanged; //!< Event fired when the drop target changes.

        /*************************************************************************
            Object Construction and Destruction
        *************************************************************************/
        /*!
        \brief
            Constructor for DragContainer objects
        */
        DragContainer(const String& type, const String& name);

        /*!
        \brief
            Destructor for DragContainer objects
        */
        virtual ~DragContainer(void);

        /*************************************************************************
        	Public Interface to DragContainer
        *************************************************************************/
        bool isDraggingEnabled(void) const;

        void setDraggingEnabled(bool setting);

        bool isBeingDragged(void) const;

        float getPixelDragThreshold(void) const;

        void setPixelDragThreshold(float pixels);

        float getDragAlpha(void) const;

        void setDragAlpha(float alpha);

        const Image* getDragCursorImage(void) const;

        void setDragCursorImage(const Image* image);

        void setDragCursorImage(MouseCursorImage image);

        void setDragCursorImage(const String& imageset, const String& image);

        Window* getCurrentDropTarget(void) const;

    protected:
        /*************************************************************************
        	Protected Implementation Methods
        *************************************************************************/
        /*!
        \brief
            Adds events specific to the DragContainer base class.
        
        \return
            Nothing.
        */
        void addDragContainerEvents(void);

        /*!
        \brief
            Return whether the required minimum movement threshold before initiating dragging
            has been exceeded.

        \param local_mouse
            Mouse position as a pixel offset from the top-left corner of this window.

        \return
            - true if the threshold has been exceeded and dragging should be initiated.
            - false if the threshold has not been exceeded.
        */		
        bool isDraggingThresholdExceeded(const Point& local_mouse);

	    /*!
	    \brief
		    Initialise the required states to put the window into dragging mode.

        \return
            Nothing.
	    */
        void initialiseDragging(void);

        /*!
	    \brief
		    Update state for window dragging.

	    \param local_mouse
		    Mouse position as a pixel offset from the top-left corner of this window.

	    \return
		    Nothing.
	    */
        void doDragging(const Point& local_mouse);

        /*!
        \brief
            Method to update mouse cursor image
        */
        void updateActiveMouseCursor(void) const;

        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
        void drawSelf(float z);

        /*************************************************************************
        	Overrides of methods in Window
        *************************************************************************/

        /*************************************************************************
        	Overrides for Event handler methods
        *************************************************************************/
        virtual void onMouseButtonDown(MouseEventArgs& e);
        virtual void onMouseButtonUp(MouseEventArgs& e);
        virtual void onMouseMove(MouseEventArgs& e);
        virtual void onCaptureLost(WindowEventArgs& e);
        virtual void onAlphaChanged(WindowEventArgs& e);
        virtual void onClippingChanged(WindowEventArgs& e);/*Window::drawSelf(z);*/

        /*************************************************************************
        	New Event handler methods
        *************************************************************************/
        /*!
        \brief
            Method called when dragging commences

        \param e
            WindowEventArgs object containing any relevant data.

        \return
            Nothing.
        */
        virtual void onDragStarted(WindowEventArgs& e);

        /*!
        \brief
            Method called when dragging ends.

        \param e
            WindowEventArgs object containing any relevant data.

        \return
            Nothing.
        */
        virtual void onDragEnded(WindowEventArgs& e);

        /*!
        \brief
            Method called when the dragged object position is changed.

        \param e
            WindowEventArgs object containing any relevant data.

        \return
            Nothing.
        */
        virtual void onDragPositionChanged(WindowEventArgs& e);

        /*!
        \brief
            Method called when the dragging state is enabled or disabled
        \param e
            WindowEventArgs object.
        \return
            Nothing.
        */
        virtual void onDragEnabledChanged(WindowEventArgs& e);

        /*!
        \brief
            Method called when the alpha value to use when dragging is changed.
        \param e
            WindowEventArgs object.
        \return
            Nothing.
        */
        virtual void onDragAlphaChanged(WindowEventArgs& e);

        /*!
        \brief
            Method called when the mouse cursor to use when dragging is changed.
        \param e
            WindowEventArgs object.
        \return
            Nothing.
        */
        virtual void onDragMouseCursorChanged(WindowEventArgs& e);

        /*!
        \brief
            Method called when the movement threshold required to trigger dragging is changed.
        \param e
            WindowEventArgs object.
        \return
            Nothing.
        */
        virtual void onDragThresholdChanged(WindowEventArgs& e);

        /*!
        \brief
            Method called when the current drop target of this DragContainer changes.
        \note
            This event fires just prior to the target field being changed.  The default implementation
            changes the drop target, you can examine the old and new targets before calling the default
            implementation to make the actual change (and fire appropriate events for the Window objects
            involved).
        \param e
            DragDropEventArgs object initialised as follows:
            - dragDropItem is initialised to the DragContainer triggering the event (typically 'this').
            - window is initialised to point to the Window which will be the new drop target.
        \return
            Nothing.
        */
        virtual void onDragDropTargetChanged(DragDropEventArgs& e);

        /*************************************************************************
        	Data
        *************************************************************************/
        bool    d_draggingEnabled;  //!< True when dragging is enabled.
        bool    d_leftMouseDown;    //!< True when left mouse button is down.
        bool    d_dragging;         //!< true when being dragged.
        Point   d_dragPoint;        //!< point we are being dragged at.
        Point   d_startPosition;    //!< position prior to dragging.
        float   d_dragThreshold;    //!< Pixels mouse must move before dragging commences.
        float   d_dragAlpha;        //!< Alpha value to set when dragging.
        float   d_storedAlpha;      //!< Alpha value to re-set when dragging ends.
        bool    d_storedClipState;  //!< Parent clip state to re-set.
        Window* d_dropTarget;       //!< Target window for possible drop operation.
        const Image* d_dragCursorImage; //!< Image to use for mouse cursor when dragging.

    private:
        /*************************************************************************
            Static properties for the Spinner widget
        *************************************************************************/
        static DragContainerProperties::DragAlpha       d_dragAlphaProperty;
        static DragContainerProperties::DragCursorImage d_dragCursorImageProperty;
        static DragContainerProperties::DraggingEnabled d_dragEnabledProperty;
        static DragContainerProperties::DragThreshold   d_dragThresholdProperty;

        /*************************************************************************
        	Implementation methods
        *************************************************************************/
        /*!
        \brief
            Adds properties specific to the DragContainer base class.
        
        \return
            Nothing.
        */
        void addDragContainerProperties(void);
    };

    /*!
    \brief
        Factory class for producing DragContainer windows
    */
    class DragContainerFactory : public WindowFactory
    {
    public:
        DragContainerFactory(void) : WindowFactory(DragContainer::WidgetTypeName) { }
        ~DragContainerFactory(void){}

        Window* createWindow(const String& name)
        {
            DragContainer* wnd = new DragContainer(d_type, name);
            return wnd;
        }

        void destroyWindow(Window* window)
        {
            if (window->getType() == d_type)
                delete window;
        }

    };


} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIDragContainer_h_
