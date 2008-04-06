/***********************************************************************
filename: 	CEGUIExceptions.cpp
created:	20/2/2004
author:		Paul D Turner, Frederico Jeronimo (fjeronimo)

purpose:	Implements the exception classes used within the system
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2007 Paul D Turner & The CEGUI Development Team
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

//////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////

#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIPropertyHelper.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////
// CEGUI NAMESPACE
//////////////////////////////////////////////////////////////////////////

// Start of CEGUI namespace section
namespace CEGUI
{
    //////////////////////////////////////////////////////////////////////////
    // EXCEPTION FUNCTIONS
    //////////////////////////////////////////////////////////////////////////

    //------------------------------------------------------------------------
    Exception::Exception(const String& message, const String& name, const String& filename, int line)
        : d_message(message), d_filename(filename), d_name(name), d_line(line)
    {
        // Log exception or send it to error stream (if logger not available)
        Logger* logger = Logger::getSingletonPtr();
        if (logger)
        {
            logger->logEvent(name + " in file " + filename  + "(" + PropertyHelper::intToString(line) + ") : " + message, Errors);
        }
        else
        {
            std::cerr << name << " in file " << filename.c_str() << "(" << line << ") : " << message.c_str() << std::endl;
        }
    }

    //------------------------------------------------------------------------
    Exception::~Exception(void)
    {
    }

} // End of  CEGUI namespace section
