/***********************************************************************
	filename: CEGUILua.h
	created:  16/3/2005
	author:   Tomas Lindquist Olsen
	
	purpose:  Defines interface for LuaScriptModule class
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
#ifndef _CEGUILua_h_
#define _CEGUILua_h_


/*************************************************************************
	Import / Export control macros
*************************************************************************/
#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef CEGUILUA_EXPORTS
#       define CEGUILUA_API __declspec(dllexport)
#   else
#       define CEGUILUA_API __declspec(dllimport)
#   endif
#else
#   define CEGUILUA_API
#endif


#include "CEGUIScriptModule.h"

struct lua_State;

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Interface for the LuaScriptModule class
*/
class CEGUILUA_API LuaScriptModule : public CEGUI::ScriptModule
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for LuaScriptModule class which create a lua_State
	*/
	LuaScriptModule();


	/*!
	\brief
		Constructor for LuaScriptModule class which takes a lua_State

	\param state
		Pointer to the lua_State that the script module should attach to.
	*/
	LuaScriptModule(lua_State* state);


	/*!
	\brief
		Destructor for LuaScriptModule class.
	*/
	~LuaScriptModule();


	/*************************************************************************
		Script Execution Functions
	*************************************************************************/
	/*!
	\brief
		Execute a script file.

	\param filename
		String object holding the filename of the script file that is to be executed
		
	\param resourceGroup
		Resource group idendifier to be passed to the ResourceProvider when loading the script file.
	*/
	void executeScriptFile(const String& filename, const String& resourceGroup);


	/*!
	\brief
		Execute a scripted global function.  The function should not take any parameters and should return an integer.

	\param function_name
		String object holding the name of the function, in the global script environment, that
		is to be executed.

	\return
		The integer value returned from the script function.
	*/
	int executeScriptGlobal(const String& function_name);


    /*!
    \brief
        Execute a scripted global 'event handler' function by looking it up by name.

    \param handler_name
        String object holding the name of the scripted handler function.
        If this string contains dots '.' it will be parsed as tables containing a function field.
        For example: 'mytable.subtable.func'

    \param e
        EventArgs based object that should be passed, by any appropriate means, to the scripted function.

    \return
        - true if the event was handled.
        - false if the event was not handled.
    */
    bool executeScriptedEventHandler(const String& handler_name, const EventArgs& e);


    /*!
    \brief
        Execute script code contained in the given CEGUI::String object.

    \param str
        String object holding the valid script code that should be executed.

    \return
        Nothing.
    */
    void executeString(const String& str);

    /*************************************************************************
        Event subscription
    *************************************************************************/
    /*!
    \brief
            Subscribes the named Event to a scripted funtion

    \param target
            The target EventSet for the subscription.

    \param name
            String object containing the name of the Event to subscribe to.

    \param subscriber_name
            String object containing the name of the script funtion that is to be subscribed to the Event.

    \return
            Connection object that can be used to check the status of the Event connection and to disconnect (unsubscribe) from the Event.

    \exception UnknownObjectException	Thrown if an Event named \a name is not in the EventSet
    */
    Event::Connection	subscribeEvent(EventSet* target, const String& name, const String& subscriber_name);

    /*!
    \brief
            Subscribes the specified group of the named Event to a scripted funtion.

    \param target
            The target EventSet for the subscription.

    \param name
            String object containing the name of the Event to subscribe to.

    \param group
            Group which is to be subscribed to.  Subscription groups are called in ascending order.

    \param subscriber_name
            String object containing the name of the script funtion that is to be subscribed to the Event.

    \return
            Connection object that can be used to check the status of the Event connection and to disconnect (unsubscribe) from the Event.

    \exception UnknownObjectException	Thrown if an Event named \a name is not in the EventSet
    */
    Event::Connection	subscribeEvent(EventSet* target, const String& name, Event::Group group, const String& subscriber_name);

    /*************************************************************************
        Bindings creation / destruction
    *************************************************************************/
    /*!
    \brief
        Method called during system initialisation, prior to running any scripts via the ScriptModule, to enable the ScriptModule
        to perform any operations required to complete initialisation or binding of the script language to the gui system objects.

    \return
        Nothing.
    */
    void createBindings(void);


    /*!
    \brief
        Method called during system destruction, after all scripts have been run via the ScriptModule, to enable the ScriptModule
        to perform any operations required to cleanup bindings of the script language to the gui system objects, as set-up in the
        earlier createBindings call.

    \return
        Nothing.
    */
    void destroyBindings(void);

    /*************************************************************************
        Accessor type functions
    *************************************************************************/
    /*!
    \brief
        Method used to get a pointer to the lua_State that the script module is attached to.

    \return
        A pointer to the lua_State that the script module is attached to.
    */
    lua_State* getLuaState(void) const {return d_state;}

private:
    /*************************************************************************
        Implementation Functions
    *************************************************************************/
    void setModuleIdentifierString();

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    bool d_ownsState;      //!< true when the attached lua_State was created by this script module
    lua_State* d_state;    //!< The lua_State that this script module uses.
};

} // namespace CEGUI

#endif // end of guard _CEGUILua_h_
