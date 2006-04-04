/************************************************************************
	filename: 	CEGUIItemEntry.h
	created:	31/3/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
	
	purpose:	Interface to base class for ItemEntry widget
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
#ifndef _CEGUIItemEntry_h_
#define _CEGUIItemEntry_h_

#include "CEGUIBase.h"
#include "CEGUIWindow.h"
#include "elements/CEGUIItemEntryProperties.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Base class for ItemEntry window renderer objects.
*/
class CEGUIEXPORT ItemEntryWindowRenderer : public WindowRenderer
{
public:
    /*!
    \brief
        Constructor
    */
    ItemEntryWindowRenderer(const String& name);

    /*!
    \brief
        Return the "optimal" size for the item

    \return
        Size describing the size in pixel that this ItemEntry's content requires
        for non-clipped rendering
    */
    virtual Size getItemPixelSize(void) const = 0;
};

/*!
\brief
	Base class for item type widgets.

\todo
    Fire events on selection / deselection.
    (Maybe selectable mode changed as well?)
*/
class CEGUIEXPORT ItemEntry : public Window
{
public:
    /*************************************************************************
        Constants
    *************************************************************************/
    static const String WidgetTypeName;             //!< Window factory name
    static const String EventSelectionChanged;    //!< Event fired when selection state changes.

	/*************************************************************************
		Accessors
	*************************************************************************/
	/*!
	\brief
		Return the "optimal" size for the item
	
	\return
		Size describing the size in pixel that this ItemEntry's content requires
		for non-clipped rendering
	*/
	Size getItemPixelSize(void) const;

    /*!
    \brief
        Returns a pointer to the owner ItemListBase.
        0 if there is none.
    */
    ItemListBase* getOwnerList(void) const  {return d_ownerList;}

    /*!
    \brief
        Returns whether this item is selected or not.
    */
    bool isSelected(void) const             {return d_selected;}

    /*!
    \brief
        Returns whether this item is selectable or not.
    */
    bool isSelectable(void) const           {return d_selectable;}

    /*************************************************************************
		Set methods
	*************************************************************************/
    /*!
    \brief
        Sets the selection state of this item (on/off).
        If this item is not selectable this function does nothing.

    \param setting
        'true' to select the item.
        'false' to deselect the item.
    */
    void setSelected(bool setting)  {setSelected_impl(setting, true);}
    
    /*!
    \brief
        Selects the item.
    */
    void select(void)               {setSelected_impl(true, true);}

    /*!
    \brief
        Deselects the item.
    */
    void deselect(void)             {setSelected_impl(false, true);}

    /*!
    \brief
        Set the selection state for this ListItem.
        Internal version. Should NOT be used by client code.
    */
    void setSelected_impl(bool state, bool notify);

    /*!
    \brief
        Sets whether this item will be selectable.

    \param setting
        'true' to allow this item to be selected.
        'false' to disallow this item from ever being selected.

    \note
        If the item is currently selectable and selected, calling this
        function with \a setting as 'false' will first deselect the item
        and then disable selectability.
    */
    void setSelectable(bool setting);

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for ItemEntry objects
	*/
	ItemEntry(const String& type, const String& name);

	/*!
	\brief
		Destructor for ItemEntry objects
	*/
	virtual ~ItemEntry(void) {}

protected:
    /*************************************************************************
        Abstract Implementation Functions
    *************************************************************************/
    /*!
    \brief
        Return the "optimal" size for the item

    \return
        Size describing the size in pixel that this ItemEntry's content requires
        for non-clipped rendering
    */
    //virtual Size getItemPixelSize_impl(void) const = 0;

	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Return whether this window was inherited from the given class name at
		some point in the inheritance heirarchy.

	\param class_name
		The class name that is to be checked.

	\return
		true if this window was inherited from \a class_name. false if not.
	*/
	virtual bool testClassName_impl(const String& class_name) const
	{
		if (class_name=="ItemEntry")	return true;
		return Window::testClassName_impl(class_name);
	}

    // validate window renderer
    virtual bool validateWindowRenderer(const String& name) const
    {
        return (name == "ItemEntry");
    }

    /*************************************************************************
        New Event Handlers
    *************************************************************************/
    /*!
    \brief
        Handles selection state changes.
    */
    virtual void onSelectionChanged(WindowEventArgs& e);

    /*************************************************************************
		Overridden Event Handlers
	*************************************************************************/
	virtual void onMouseClicked(MouseEventArgs& e);

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    
    //!< pointer to the owner ItemListBase. 0 if there is none.
    ItemListBase* d_ownerList;
    
    //!< 'true' when the item is in the selected state, 'false' if not.
    bool d_selected;

    //!< 'true' when the item is selectable.
    bool d_selectable;
    
    // make the ItemListBase a friend
    friend class ItemListBase;

private:
    /************************************************************************
        Static Properties for this class
    ************************************************************************/
    static ItemEntryProperties::Selectable d_selectableProperty;
    static ItemEntryProperties::Selected d_selectedProperty;
    
    void addItemEntryProperties(void);
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIItemEntry_h_
