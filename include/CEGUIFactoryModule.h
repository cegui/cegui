/***********************************************************************
	filename: 	CEGUIFactoryModule.h
	created:	12/4/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for object that controls a loadable
				module (.dll/.so/ whatever) that contains concrete
				window / widget implementations and their factories.
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
#ifndef _CEGUIFactoryModule_h_
#define _CEGUIFactoryModule_h_

#include "CEGUIDynamicModule.h"

// Start of CEGUI namespace section
namespace CEGUI
{

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
	virtual ~FactoryModule(void);


	/*!
	\brief
		Register a WindowFactory for \a type Windows.

	\param type
		String object holding the name of the Window type a factory is to be registered for.

	\return
		Nothing.
	*/
	void	registerFactory(const String& type) const;


    /*!
    \brief
        Register all factories available in this module.

    \return
        uint value indicating the number of factories registered.
    */
    uint registerAllFactories() const;

private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	static const char	RegisterFactoryFunctionName[];
    static const char   RegisterAllFunctionName[];

	typedef void (*FactoryRegisterFunction)(const String&); 
    typedef uint (*RegisterAllFunction)(void);

	FactoryRegisterFunction	d_regFunc;	//!< Pointer to the function called to register factories.
    RegisterAllFunction d_regAllFunc;   //!< Pointer to a function called to register all factories in a module.
    DynamicModule* d_module;
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIFactoryModule_h_
