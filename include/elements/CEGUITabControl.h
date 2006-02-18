/************************************************************************
	filename: 	CEGUITabControl.h
	created:	08/08/2004
	author:		Steve Streeting
	
	purpose:	Interface to base class for TabControl widget
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
#ifndef _CEGUITabControl_h_
#define _CEGUITabControl_h_

#include "CEGUIBase.h"
#include "CEGUIWindow.h"
#include "elements/CEGUITabControlProperties.h"
#include "elements/CEGUITabPane.h"
#include <vector>


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

    // Forward declaration
    class TabButton;

/*!
\brief
    Base class for TabControl window renderer objects.
*/
class CEGUIEXPORT TabControlWindowRenderer : public WindowRenderer
{
public:
    /*!
    \brief
        Constructor
    */
    TabControlWindowRenderer(const String& name);

    /*!
    \brief
        create and return a pointer to a TabButton widget for use as a clickable tab header
    \param name
        Button name
    \return
        Pointer to a TabButton to be used for changing tabs.
    */
    virtual TabButton*  createTabButton(const String& name) const       = 0;
};

/*!
\brief
	Base class for standard Tab Control widget.
*/
class CEGUIEXPORT TabControl : public Window
{
public:
	static const String EventNamespace;				//!< Namespace for global events
    static const String WidgetTypeName;             //!< Window factory name

	/*************************************************************************
		Constants
	*************************************************************************/
	// event names
	static const String EventSelectionChanged;			//!< Event triggered when there is a change to the currently selected tab.

    /*************************************************************************
        Child Widget name suffix constants
    *************************************************************************/
    static const String ContentPaneNameSuffix; //!< Widget name suffix for the tab content pane component.
    static const String TabButtonNameSuffix;   //!< Widget name suffix for the tab button components.
    static const String TabButtonPaneNameSuffix; //!< Widget name suffix for the tab button pane component.

	/*************************************************************************
		Accessor Methods
	*************************************************************************/
	/*!
	\brief
		Return number of tabs 

	\return
		the number of tabs currently present.
	*/
	size_t	getTabCount(void) const;


    /*!
    \brief
        Set the selected tab by the name of the root window within it.
    \exception	InvalidRequestException	thrown if \a index is out of range.
    */
    void    setSelectedTab(const String &name);

    /*!
    \brief
        Set the selected tab by the ID of the root window within it.
    \exception	InvalidRequestException	thrown if \a index is out of range.
    */
    void    setSelectedTab(uint ID);

    /*!
    \brief
        Set the selected tab by the index position in the tab control.
    \exception	InvalidRequestException	thrown if \a index is out of range.
    */
    void    setSelectedTabAtIndex(size_t index);

    /*!
	\brief
		Return the Window which is the first child of the tab at index position \a index.

	\param index
		Zero based index of the item to be returned.

	\return
		Pointer to the Window at index position \a index in the tab control.

	\exception	InvalidRequestException	thrown if \a index is out of range.
	*/
    Window*	getTabContentsAtIndex(size_t index) const;

    /*!
    \brief
        Return the Window which is the tab content with the given name.

    \param name
        Name of the Window which was attached as a tab content.

    \return
        Pointer to the named Window in the tab control.

    \exception	InvalidRequestException	thrown if content is not found.
    */
    Window*	getTabContents(const String& name) const;

    /*!
    \brief
        Return the Window which is the tab content with the given ID.

    \param ID
        ID of the Window which was attached as a tab content.

    \return
        Pointer to the Window with the given ID in the tab control.

    \exception	InvalidRequestException	thrown if content is not found.
    */
    Window*	getTabContents(uint ID) const;

    /*!
	\brief
		Return whether the tab contents window is currently selected.

    \param wnd
        The tab contents window to query.

	\return
		true if the tab is currently selected, false otherwise.

	\exception	InvalidRequestException	thrown if \a wnd is not a valid tab contents window.
	*/
    bool	isTabContentsSelected(Window* wnd) const;

    /*!
	\brief
		Return the index of the currently selected tab.

	\return
		index of the currently selected tab.
	*/
    size_t	getSelectedTabIndex() const;
	
    /*!
    \brief
        Return the height of the tabs
    */
    const UDim& getTabHeight(void) const { return d_tabHeight; }

    /*!
    \brief
        Return the amount of padding to add either side of the text in the tab
    */
    const UDim& getTabTextPadding(void) const { return d_tabPadding; }


    /*************************************************************************
		Manipulator Methods
	*************************************************************************/
	/*!
	\brief
		Initialise the Window based object ready for use.

	\note
		This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

	\return
		Nothing
	*/
	virtual void	initialiseComponents(void);

    /*!
    \brief
        Set the height of the tabs
    */
    void setTabHeight(const UDim& height);

    /*!
    \brief
        Set the amount of padding to add either side of the text in the tab
    */
    void setTabTextPadding(const UDim& padding);

    /*!
    \brief 
        Add a new tab to the tab control. 
    \par 
        The new tab will be added with the same text as the window passed in.
    \param wnd
        The Window which will be placed in the content area of this new tab.
    */
    void addTab(Window* wnd);
    /*!
    \brief 
        Remove the named tab from the tab control. 
    \par 
        The tab content will be destroyed. 
    */
    void removeTab(const String& name);
    /*!
    \brief 
    Remove the tab with the given ID from the tab control. 
    \par 
    The tab content will be destroyed. 
    */
    void removeTab(uint ID);


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for TabControl base class.
	*/
	TabControl(const String& type, const String& name);


	/*!
	\brief
		Destructor for Listbox base class.
	*/
	virtual ~TabControl(void);


protected:

	/*************************************************************************
		Implementation Functions
	*************************************************************************/
    /*!
    \brief
        Perform the actual rendering for this Window. 

    \param z
        float value specifying the base Z co-ordinate that should be used when rendering

    \return
        Nothing
    */
    virtual	void	drawSelf(float z) { /* do nothing; rendering handled by children */ }

    /*!
    \brief
        Add a TabButton for the specified child Window.
    */
    virtual void addButtonForTabContent(Window* wnd);
    /*!
    \brief
        Remove the TabButton for the specified child Window.
    */
    virtual void removeButtonForTabContent(Window* wnd);
    /*!
    \brief
        Return the TabButton associated with this Window.
    \exception	InvalidRequestException	thrown if content is not found.
    */
	TabButton* getButtonForTabContents(Window* wnd) const;
    /*!
    \brief
        Construct a button name to handle a window.
    */
    String makeButtonName(Window* wnd);
    
    /*!
    \brief
        Internal implementation of select tab.
    \param wnd
        Pointer to a Window which is the root of the tab content to select
    */
    virtual void selectTab_impl(Window* wnd);


	/*!
	\brief
		Return whether this window was inherited from the given class name at some point in the inheritance heirarchy.

	\param class_name
		The class name that is to be checked.

	\return
		true if this window was inherited from \a class_name. false if not.
	*/
	virtual bool	testClassName_impl(const String& class_name) const
	{
		if (class_name=="TabControl")	return true;
		return Window::testClassName_impl(class_name);
	}

    /*!
    \brief
        Return a pointer to the tab button pane (Window)for
        this TabControl.

    \return
        Pointer to a Window object.

    \exception UnknownObjectException
        Thrown if the component does not exist.
    */
    Window* getTabButtonPane() const;

    /*!
    \brief
        Return a pointer to the TabPane component widget for
        this TabControl.

    \return
        Pointer to a TabPane object.

    \exception UnknownObjectException
        Thrown if the component does not exist.
    */
    TabPane* getTabPane() const;

	void performChildWindowLayout();
    int writeChildWindowsXML(OutStream& out_stream) const;

    // validate window renderer
    virtual bool validateWindowRenderer(const String& name) const
    {
        return (name == "TabControl");
    }

    /*!
    \brief
        create and return a pointer to a TabButton widget for use as a clickable tab header
    \param name
        Button name
    \return
        Pointer to a TabButton to be used for changing tabs.
    */
    TabButton*  createTabButton(const String& name) const;

	/*************************************************************************
		New event handlers
	*************************************************************************/

	/*!
	\brief
		Handler called internally when the currently selected item or items changes.
	*/
	virtual	void	onSelectionChanged(WindowEventArgs& e);

	/*!
	\brief
		Handler called when the window's font is changed.

	\param e
		WindowEventArgs object whose 'window' pointer field is set to the window that triggered the event.  For this
		event the trigger window is always 'this'.
	*/
	virtual void	onFontChanged(WindowEventArgs& e);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
    UDim        d_tabHeight;        //!< The height of the tabs in pixels
    UDim        d_tabPadding;       //!< The padding of the tabs relative to parent
    size_t      d_nextTabIndex;     //!< The index to give the next tab 
    typedef std::map<size_t, TabButton*> TabButtonIndexMap; 
    TabButtonIndexMap d_tabButtonIndexMap;  //!< Sorting for tabs
    /*************************************************************************
    Abstract Implementation Functions (must be provided by derived class)
    *************************************************************************/
    /*!
    \brief
        create and return a pointer to a TabButton widget for use as a clickable tab header
    \param name
        Button name
    \return
        Pointer to a TabButton to be used for changing tabs.
    */
    //virtual TabButton*	createTabButton_impl(const String& name) const		= 0;

    /*!
    \brief
        Calculate the correct position and size of a tab button, based on the
        index it is due to be placed at.
    \param btn
        Pointer to a button to calculate the size and position for. This button
        will be updated in place with these metrics.
    \param targetIndex
        The index at which the tab is/will be placed. Tabs must exist for all the
        indexes before this.
    */
    void calculateTabButtonSizePosition(TabButton* btn, size_t targetIndex);

protected:
	/*************************************************************************
		Static Properties for this class
	*************************************************************************/
	static TabControlProperties::TabHeight			d_tabHeightProperty;
    static TabControlProperties::TabTextPadding			d_tabTextPaddingProperty;

    /*************************************************************************
		Private methods
	*************************************************************************/
	void	addTabControlProperties(void);

    void    addChild_impl(Window* wnd);
    void    removeChild_impl(Window* wnd);

    /*************************************************************************
    Event handlers
    *************************************************************************/
    bool handleContentWindowTextChanged(const EventArgs& args);
    bool handleTabButtonClicked(const EventArgs& args);
};


} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUITabControl_h_
