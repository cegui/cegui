/************************************************************************
	filename: 	CEGUIGlobalEventSet.cpp
	created:	16/1/2005
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
#include "CEGUIGlobalEventSet.h"
#include "CEGUILogger.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	/*************************************************************************
		Static Data Definitions
	*************************************************************************/
	// singleton instance pointer
	template<> GlobalEventSet* Singleton<GlobalEventSet>::ms_Singleton	= 0;

	/*************************************************************************
		GlobalEventSet constructor.
	*************************************************************************/
	GlobalEventSet::GlobalEventSet()
	{
		Logger::getSingleton().logEvent("CEGUI::GlobalEventSet singleton created.");
	}

	/*************************************************************************
		GlobalEventSet destructor.
	*************************************************************************/
	GlobalEventSet::~GlobalEventSet()
	{
		Logger::getSingleton().logEvent("CEGUI::GlobalEventSet singleton destroyed.");
	}

	/*************************************************************************
		Return singleton object
	*************************************************************************/
	GlobalEventSet&	GlobalEventSet::getSingleton(void)
	{
		return Singleton<GlobalEventSet>::getSingleton();
	}

	/*************************************************************************
		Return singleton pointer
	*************************************************************************/
	GlobalEventSet*	GlobalEventSet::getSingletonPtr(void)
	{
		return Singleton<GlobalEventSet>::getSingletonPtr();
	}

	/*************************************************************************
		Overridden fireEvent which always succeeds.
	*************************************************************************/
	void GlobalEventSet::fireEvent(const String& name, EventArgs& args, const String& eventNamespace)
	{
        fireEvent_impl(eventNamespace + "/" + name, args);
	}

} // End of  CEGUI namespace section
