/***********************************************************************
	filename: 	CEGUIProgressBarProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements the ProgressBar property classes
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
#include "elements/CEGUIProgressBarProperties.h"
#include "elements/CEGUIProgressBar.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ProgressBarProperties namespace section
namespace ProgressBarProperties
{
String CurrentProgress::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const ProgressBar*>(receiver)->getProgress());
}


void CurrentProgress::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<ProgressBar*>(receiver)->setProgress(PropertyHelper::stringToFloat(value));
}


String StepSize::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const ProgressBar*>(receiver)->getStep());
}


void StepSize::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<ProgressBar*>(receiver)->setStepSize(PropertyHelper::stringToFloat(value));
}

} // End of  ProgressBarProperties namespace section

} // End of  CEGUI namespace section
