/***********************************************************************
	filename: 	CEGUIGUILayout_xmlHandler.h
	created:	5/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to XML parser for GUILayout files
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
#ifndef _CEGUIGUILayout_xmlHandler_h_
#define _CEGUIGUILayout_xmlHandler_h_

#include "CEGUIWindowManager.h"
#include "CEGUIWindow.h"
#include "CEGUIXMLHandler.h"

#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Handler class used to parse the GUILayout XML files using SAX2
*/
class GUILayout_xmlHandler : public XMLHandler
{
	typedef WindowManager::PropertyCallback PropertyCallback;
public:
	/*************************************************************************
		Construction & Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for GUILayout_xmlHandler objects
	*/
	GUILayout_xmlHandler(const String& name_prefix, PropertyCallback* callback = 0, void* userdata = 0) :
	  d_root(0),
	  d_namingPrefix(name_prefix),
	  d_propertyCallback(callback),
	  d_userData(userdata)
	  {}

	/*!
	\brief
		Destructor for GUILayout_xmlHandler objects
	*/
	virtual ~GUILayout_xmlHandler(void) {}

	/*************************************************************************
		SAX2 Handler overrides
	*************************************************************************/ 
	/*!
	\brief
		document processing (only care about elements, schema validates format)
	*/
    virtual void elementStart(const String& element, const XMLAttributes& attributes);
    virtual void elementEnd(const String& element);
    virtual void text(const String& text);

	/*************************************************************************
		Functions used by our implementation
	*************************************************************************/
	/*!
	\brief
		Destroy all windows created so far.
	*/
	void	cleanupLoadedWindows(void);


	/*!
	\brief
		Return a pointer to the 'root' window created.
	*/
	Window*	getLayoutRootWindow(void) const;

private:
	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const String GUILayoutElement;				//!< Tag name for GUILayout elements.
	static const String WindowElement;				//!< Tag name for Window elements.
    static const String AutoWindowElement;              //!< Tag name for AutoWindow elements.
	static const String PropertyElement;				//!< Tag name for Property elements.
	static const String LayoutImportElement;			//!< Tag name for LayoutImport elements.
	static const String EventElement;					//!< Tag name for Event elements.
	static const String WindowTypeAttribute;			//!< Attribute name that stores the type of Window to create.
	static const String WindowNameAttribute;			//!< Attribute name that stores the name of the window to create.
    static const String AutoWindowNameSuffixAttribute;  //!< Attribute name that stores the name suffix of the auto window to get.
	static const String PropertyNameAttribute;		//!< Attribute name that stores the name of the property to set.
	static const String PropertyValueAttribute;		//!< Attribute name that stores the value to pass to the property.
	static const String LayoutParentAttribute;		//!< Attribute name that stores the name of the window to attach the layout to.
	static const String LayoutImportFilenameAttribute;//!< Attribute name that stores the file name of the layout to import.
	static const String LayoutImportPrefixAttribute;	//!< Attribute name that stores the prefix to use when loading the imported layout.
    static const String LayoutImportResourceGroupAttribute; //!< Attribute name that stores the resource group identifier used when loading imported file.
	static const String EventNameAttribute;			//!< Attribute name that stores the event name to be subscribed.
	static const String EventFunctionAttribute;		//!< Attribute name that stores the name of the scripted function to be bound.

    /*!
    \brief
        Method that handles the opening GUILayout XML element.
    */
    void elementGUILayoutStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the opening Window XML element.
    */
    void elementWindowStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the opening AutoWindow XML element.
    */
    void elementAutoWindowStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the Property XML element.
    */
    void elementPropertyStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the LayoutImport XML element.
    */
    void elementLayoutImportStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the Event XML element.
    */
    void elementEventStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the closing GUILayout XML element.
    */
    void elementGUILayoutEnd();

    /*!
    \brief
        Method that handles the closing Window XML element.
    */
    void elementWindowEnd();

    /*!
    \brief
        Method that handles the closing AutoWindow XML element.
    */
    void elementAutoWindowEnd();

    /*!
    \brief 
        Method that handles the closing of a property XML element.
    */
    void elementPropertyEnd();

    void operator=(const GUILayout_xmlHandler&) {}

    /*************************************************************************
		Implementation Data
	*************************************************************************/
    typedef std::pair<Window*,bool> WindowStackEntry; //!< Pair used as datatype for the window stack. second is false if the window is an autowindow.
	typedef std::vector<WindowStackEntry>	WindowStack;
	Window*	d_root;				//!< Will point to first window created.
	WindowStack	d_stack;		//!< Stack used to keep track of what we're doing to which window.
	String		d_layoutParent;	//!< Name of the parent window to attach the loaded layout to.
	const String&		d_namingPrefix;	//!< Prefix that is to prepend all names of created windows.
	PropertyCallback*	d_propertyCallback; //!< Callback for every property loaded
	void*				d_userData;			//!< User data for the property callback
  String d_propertyName; //!< Use for long property value 
  String d_propertyValue; //!< Use for long property value 
};


} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIGUILayout_xmlHandler_h_
