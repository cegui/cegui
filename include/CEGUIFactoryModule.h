/************************************************************************
	filename: 	CEGUIFactoryModule.h
	created:	12/4/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for object that controls a loadable
				module (.dll/.so/ whatever) that contains concrete
				window / widget implementations and their factories.
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#ifndef _CEGUIFactoryModule_h_
#define _CEGUIFactoryModule_h_

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Forward refs
*************************************************************************/
class FactoryModuleImplDat;


/*!
\brief
	Class that encapsulates access to a dynamic loadable module containing implementations of Windows, Widgets, and their factories.
*/
class FactoryModule
{
public:
	/*!
	\brief
		Construct the FactoryModule object by loading the dynamic loadable module specified.

	\param filename
		String object holding the filename of a loadable module.

	\return
		Nothing
	*/
	FactoryModule(const String& filename);


	/*!
	\brief
		Destroys the FactoryModule object and unloads any loadable module.

	\return
		Nothing
	*/
	~FactoryModule(void);


	/*!
	\brief
		Register a WindowFactory for \a type Windows.

	\param type
		String object holding the name of the Window type a factory is to be registered for.

	\return
		Nothing.
	*/
	void	registerFactory(const String& type) const;

private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	FactoryModuleImplDat*	d_data;		//!< Pointer to a ImplDat derived class that can hold any required implementation data
};


/*************************************************************************
	class that can be derived from to hold implementation specific data
*************************************************************************/
class FactoryModuleImplDat
{
public:
	FactoryModuleImplDat(void) {}
	virtual ~FactoryModuleImplDat(void) {}
};





} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIFactoryModule_h_
