/************************************************************************
	filename: 	CEGUIProgressBarProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements the ProgressBar property classes
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
#include "elements/CEGUIProgressBarProperties.h"
#include "elements/CEGUIProgressBar.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ProgressBarProperties namespace section
namespace ProgressBarProperties
{
String CurrentProgress::get(const void* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const ProgressBar*>(receiver)->getProgress());
}


void CurrentProgress::set(void* receiver, const String& value)
{
	static_cast<ProgressBar*>(receiver)->setProgress(PropertyHelper::stringToFloat(value));
}


String StepSize::get(const void* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const ProgressBar*>(receiver)->getStep());
}


void StepSize::set(void* receiver, const String& value)
{
	static_cast<ProgressBar*>(receiver)->setStepSize(PropertyHelper::stringToFloat(value));
}

} // End of  ProgressBarProperties namespace section

} // End of  CEGUI namespace section
