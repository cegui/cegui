/************************************************************************
	filename: 	CEGUIPropertyHelper.cpp
	created:	6/7/2004
	author:		Paul D Turner
	
	purpose:	Implementation of PropertyHelper methods
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
#include "CEGUIPropertyHelper.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIExceptions.h"

#include <cstdio>

// Start of CEGUI namespace section
namespace CEGUI
{
float PropertyHelper::stringToFloat(const String& str)
{
	float val = 0;
	std::sscanf(str.c_str(), " %f", &val);

	return val;
}


uint PropertyHelper::stringToUint(const String& str)
{
	uint val = 0;
	std::sscanf(str.c_str(), " %u", &val);

	return val;
}


bool PropertyHelper::stringToBool(const String& str)
{
	if (str == (utf8*)"True")
	{
		return true;
	}
	else
	{
		return false;
	}

}


Size PropertyHelper::stringToSize(const String& str)
{
	Size val(0,0);
	std::sscanf(str.c_str(), " w:%f h:%f", &val.d_width, &val.d_height);

	return val;
}


Point PropertyHelper::stringToPoint(const String& str)
{
	Point val(0,0) ;
	std::sscanf(str.c_str(), " x:%f y:%f", &val.d_x, &val.d_y);

	return val;
}


Rect PropertyHelper::stringToRect(const String& str)
{
	Rect val(0, 0, 0, 0);
	std::sscanf(str.c_str(), " l:%f t:%f r:%f b:%f", &val.d_left, &val.d_top, &val.d_right, &val.d_bottom);

	return val;
}


MetricsMode PropertyHelper::stringToMetricsMode(const String& str)
{
	if (str == (utf8*)"Relative")
	{
		return Relative;
	}
	else if (str == (utf8*)"Absolute")
	{
		return Absolute;
	}
	else
	{
		return Inherited;
	}

}


const Image* PropertyHelper::stringToImage(const String& str)
{
	char imageSet[128];
	char imageName[128];

	std::sscanf(str.c_str(), " set:%127s image:%127s", imageSet, imageName);

	const Image* image;

	try
	{
		image = &ImagesetManager::getSingleton().getImageset((utf8*)imageSet)->getImage((utf8*)imageName);
	}
	catch (UnknownObjectException)
	{
		image = NULL;
	}

	return image;
}


String PropertyHelper::floatToString(float val)
{
	char buff[64];
	std::sprintf(buff, "%f", val);

	return String((utf8*)buff);
}


String PropertyHelper::uintToString(uint val)
{
	char buff[64];
	std::sprintf(buff, "%u", val);

	return String((utf8*)buff);
}


String PropertyHelper::boolToString(bool val)
{
	if (val)
	{
		return String((utf8*)"True");
	}
	else
	{
		return String ((utf8*)"False");
	}

}


String PropertyHelper::sizeToString(const Size& val)
{
	char buff[128];
	std::sprintf(buff, "w:%f h:%f", val.d_width, val.d_height);

	return String((utf8*)buff);
}


String PropertyHelper::pointToString(const Point& val)
{
	char buff[128];
	std::sprintf(buff, "x:%f y:%f", val.d_x, val.d_y);

	return String((utf8*)buff);
}


String PropertyHelper::rectToString(const Rect& val)
{
	char buff[256];
	std::sprintf(buff, "l:%f t:%f r:%f b:%f", val.d_left, val.d_top, val.d_right, val.d_bottom);

	return String((utf8*)buff);
}


String PropertyHelper::metricsModeToString(MetricsMode val)
{
	if (val == Relative)
	{
		return String((utf8*)"Relative");
	}
	else if (val == Absolute)
	{
		return String((utf8*)"Absolute");
	}
	else
	{
		return String((utf8*)"Inherited");
	}

}


String PropertyHelper::imageToString(const Image* const val)
{
	if (val != NULL)
	{
		return String((utf8*)"set:" + val->getImagesetName() + (utf8*)" image:" + val->getName());
	}

	return String((utf8*)"");
}


String PropertyHelper::colourToString(colour val)
{
	char buff[16];
	std::sprintf(buff, "%.8X", val);

	return String((utf8*)buff);
}


colour PropertyHelper::stringToColour(const String& str)
{
	colour val = 0xFF000000;
	std::sscanf(str.c_str(), " %8X", &val);

	return val;

}


String PropertyHelper::colourRectToString(const ColourRect& val)
{
	char buff[64];
	std::sprintf(buff, "tl:%.8X tr:%.8X bl:%.8X br:%.8X", val.d_top_left, val.d_top_right, val.d_bottom_left, val.d_bottom_right);

	return String((utf8*)buff);
}


ColourRect PropertyHelper::stringtoColourRect(const String& str)
{
	ColourRect val(0xFF000000);

	std::sscanf(str.c_str(), "tl:%8X tr:%8X bl:%8X br:%8X", &val.d_top_left, &val.d_top_right, &val.d_bottom_left, &val.d_bottom_right);

	return val;
}

} // End of  CEGUI namespace section
