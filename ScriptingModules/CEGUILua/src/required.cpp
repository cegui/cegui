/************************************************************************
    filename:   required.cpp
    created:    16/3/2005
    author:     Tomas Lindquist Olsen
*************************************************************************/
#include "CEGUI.h"

namespace CEGUI
{

const MouseCursorEventArgs& toMouseCursorEventArgs(const EventArgs& e)
	{
	return (const MouseCursorEventArgs&)e;
	}


const WindowEventArgs& toWindowEventArgs(const EventArgs& e)
	{
	return (const WindowEventArgs&)e;
	}


const ActivationEventArgs& toActivationEventArgs(const EventArgs& e)
	{
	return (const ActivationEventArgs&)e;
	}


const HeaderSequenceEventArgs& toHeaderSequenceEventArgs(const EventArgs& e)
	{
	return (const HeaderSequenceEventArgs&)e;
	}


const MouseEventArgs& toMouseEventArgs(const EventArgs& e)
	{
	return (const MouseEventArgs&)e;
	}

}
