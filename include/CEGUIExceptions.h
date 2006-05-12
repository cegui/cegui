/***********************************************************************
	filename: 	CEGUIExceptions.h
	created:	20/2/2004
	author:		Paul D Turner
	
	purpose:	Defines exceptions used within the system
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
#ifndef _CEGUIExceptions_h_
#define _CEGUIExceptions_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Root exception class used within the GUI system.
*/
class  CEGUIEXPORT Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Exception(const String& message);
	virtual ~Exception(void);

	/*!
	\brief
		Return a reference to the String object describing the reason for the exception being thrown.

	\return
		String object containing a message describing the reason for the exception.
	*/
	const String&	getMessage(void) const		{return d_message;}


protected:
	String	d_message;
};

/*!
\brief
	Exception class used when none of the other classes are applicable
*/
class CEGUIEXPORT GenericException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	GenericException(const String& message) : Exception(message) {}
};

/*!
\brief
	Exception class used when a request was made using a name of an unknown object
*/
class CEGUIEXPORT UnknownObjectException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	UnknownObjectException(const String& message) : Exception(message) {}
};

/*!
\brief
	Exception class used when some impossible request was made for the current system state
*/
class CEGUIEXPORT InvalidRequestException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	InvalidRequestException(const String& message) : Exception(message) {}
};

/*!
\brief
	Exception class used when a file handling problem occurs
*/
class CEGUIEXPORT FileIOException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	FileIOException(const String& message) : Exception(message) {}
};

/*!
\brief
	Exception class used when an problem is detected within the Renderer or related objects
*/
class CEGUIEXPORT RendererException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	RendererException(const String& message) : Exception(message) {}
};

/*!
\brief
	Exception class used when an attempt is made to use an object name that is already in use within the system
*/
class CEGUIEXPORT AlreadyExistsException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	AlreadyExistsException(const String& message) : Exception(message) {}
};

/*!
\brief
	Exception class used when a memory handling error is detected
*/
class CEGUIEXPORT MemoryException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	MemoryException(const String& message) : Exception(message) {}
};

/*!
\brief
	Exception class used when some required object or parameter is null
*/
class CEGUIEXPORT NullObjectException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	NullObjectException(const String& message) : Exception(message) {}
};

/*!
\brief
	Exception class used when some attempt to delete, remove, or otherwise invalidate some object that is still in use occurs.
*/
class CEGUIEXPORT ObjectInUseException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	ObjectInUseException(const String& message) : Exception(message) {}
};

/*!
\brief
    Exception class used when a scripting error occurs
*/
class CEGUIEXPORT ScriptException : public Exception
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	ScriptException(const String& message) : Exception(message) {}
};


} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIExceptions_h_
