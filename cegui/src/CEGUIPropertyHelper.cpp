/***********************************************************************
	filename: 	CEGUIPropertyHelper.cpp
	created:	6/7/2004
	author:		Paul D Turner
	
	purpose:	Implementation of PropertyHelper methods
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
#include "CEGUIPropertyHelper.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIExceptions.h"

#include <cstdio>
#include <sstream>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
float PropertyHelper::stringToFloat(const String& str)
{
	using namespace std;

	float val = 0;
	sscanf(str.c_str(), " %g", &val);

	return val;
}


uint PropertyHelper::stringToUint(const String& str)
{
	using namespace std;

	uint val = 0;
	sscanf(str.c_str(), " %u", &val);

	return val;
}


int PropertyHelper::stringToInt(const String& str)
{
	using namespace std;

	uint val = 0;
	sscanf(str.c_str(), " %d", &val);

	return val;
}


bool PropertyHelper::stringToBool(const String& str)
{
	if ((str == "True") || (str == "true"))
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
	using namespace std;

	Size val(0,0);
	sscanf(str.c_str(), " w:%g h:%g", &val.d_width, &val.d_height);

	return val;
}


Point PropertyHelper::stringToPoint(const String& str)
{
	using namespace std;

	Point val(0,0) ;
	sscanf(str.c_str(), " x:%g y:%g", &val.d_x, &val.d_y);

	return val;
}


Rect PropertyHelper::stringToRect(const String& str)
{
	using namespace std;

	Rect val(0, 0, 0, 0);
	sscanf(str.c_str(), " l:%g t:%g r:%g b:%g", &val.d_left, &val.d_top, &val.d_right, &val.d_bottom);

	return val;
}


const Image* PropertyHelper::stringToImage(const String& str)
{
	using namespace std;

    // handle empty string case
    if (str.empty())
        return 0;

	char imageSet[128];
	char imageName[128];

	sscanf(str.c_str(), " set:%127s image:%127s", imageSet, imageName);

	const Image* image;

	try
	{
		image = &ImagesetManager::getSingleton().getImageset(imageSet)->getImage(imageName);
	}
	catch (UnknownObjectException&)
	{
		image = 0;
	}

	return image;
}


UDim PropertyHelper::stringToUDim(const String& str)
{
	using namespace std;

	UDim ud;
	sscanf(str.c_str()," {%g,%g}", &ud.d_scale, &ud.d_offset);

	return ud;
}


UVector2 PropertyHelper::stringToUVector2(const String& str)
{
	using namespace std;

	UVector2 uv;
	sscanf(str.c_str(), " {{%g,%g},{%g,%g}}", &uv.d_x.d_scale,&uv.d_x.d_offset, &uv.d_y.d_scale,&uv.d_y.d_offset);

	return uv;
}


URect PropertyHelper::stringToURect(const String& str)
{
	using namespace std;

	URect ur;
	sscanf(
		str.c_str(),
		" {{%g,%g},{%g,%g},{%g,%g},{%g,%g}}",
		&ur.d_min.d_x.d_scale, &ur.d_min.d_x.d_offset,
		&ur.d_min.d_y.d_scale, &ur.d_min.d_y.d_offset,
		&ur.d_max.d_x.d_scale, &ur.d_max.d_x.d_offset,
		&ur.d_max.d_y.d_scale, &ur.d_max.d_y.d_offset
	);

	return ur;
}


String PropertyHelper::floatToString(float val)
{
	using namespace std;

	char buff[64];
	snprintf(buff, sizeof (buff), "%g", val);

	return String(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << val;
    return String(iss.str());
#endif
}


String PropertyHelper::uintToString(uint val)
{
	using namespace std;

	char buff[64];
	snprintf(buff, sizeof (buff), "%u", val);

	return String(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << val;
    return String(iss.str());
#endif
}


String PropertyHelper::intToString(int val)
{
	using namespace std;

	char buff[64];
	snprintf(buff, sizeof (buff), "%d", val);

	return String(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << val;
    return String(iss.str());
#endif
}


String PropertyHelper::boolToString(bool val)
{
	if (val)
	{
		return String("True");
	}
	else
	{
		return String ("False");
	}

}


String PropertyHelper::sizeToString(const Size& val)
{
	using namespace std;

	char buff[128];
	snprintf(buff, sizeof (buff), "w:%g h:%g", val.d_width, val.d_height);

	return String(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << "w:" << val.d_width << " h:" << val.d_height;
    return String(iss.str());
#endif
}


String PropertyHelper::pointToString(const Point& val)
{
	using namespace std;

	char buff[128];
	snprintf(buff, sizeof (buff), "x:%g y:%g", val.d_x, val.d_y);

	return String(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << "x:" << val.d_x << " y:" << val.d_y;
    return String(iss.str());
#endif
}


String PropertyHelper::rectToString(const Rect& val)
{
	using namespace std;

	char buff[256];
	snprintf(buff, sizeof (buff), "l:%g t:%g r:%g b:%g",
		     val.d_left, val.d_top, val.d_right, val.d_bottom);

	return String(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << "l:" << val.d_left << " t:" << val.d_top << " r:" << val.d_right << " b:" << val.d_bottom;
    return String(iss.str());
#endif
}


String PropertyHelper::imageToString(const Image* const val)
{
	if (val)
	{
		return String("set:" + val->getImagesetName() + " image:" + val->getName());
	}

	return String("");
}


String PropertyHelper::udimToString(const UDim& val)
{
	using namespace std;

	char buff[128];
	snprintf(buff, sizeof (buff), "{%g,%g}", val.d_scale, val.d_offset);

	return String(buff);

#if 0
    std::ostringstream iss;
    iss << '{' << val.d_scale << ',' << val.d_offset << '}';
    return String(iss.str());
#endif
}


String PropertyHelper::uvector2ToString(const UVector2& val)
{
	using namespace std;

	char buff[256];
	snprintf(buff, sizeof (buff), "{{%g,%g},{%g,%g}}",
	         val.d_x.d_scale, val.d_x.d_offset, val.d_y.d_scale, val.d_y.d_offset);

    return String(buff);

#if 0
    std::ostringstream iss;

    iss << "{{"
        << val.d_x.d_scale << ',' << val.d_x.d_offset << "},{"
        << val.d_y.d_scale << ',' << val.d_y.d_offset << "}}";

	return String(iss.str());
#endif
}


String PropertyHelper::urectToString(const URect& val)
{
	using namespace std;

	char buff[512];
	snprintf(buff, sizeof (buff), "{{%g,%g},{%g,%g},{%g,%g},{%g,%g}}",
             val.d_min.d_x.d_scale,val.d_min.d_x.d_offset,
             val.d_min.d_y.d_scale,val.d_min.d_y.d_offset,
             val.d_max.d_x.d_scale,val.d_max.d_x.d_offset,
             val.d_max.d_y.d_scale,val.d_max.d_y.d_offset);

    return String(buff);

#if 0
    std::ostringstream iss;

    iss << "{{"
        << val.d_min.d_x.d_scale << ',' << val.d_min.d_x.d_offset << "},{"
        << val.d_min.d_y.d_scale << ',' << val.d_min.d_y.d_offset << "},{"
        << val.d_max.d_x.d_scale<< ',' << val.d_max.d_x.d_offset << "},{"
        << val.d_max.d_y.d_scale << ',' << val.d_max.d_y.d_offset << "}}";

	return String(iss.str());
#endif
}


String PropertyHelper::colourToString(const colour& val)
{
	using namespace std;

	char buff[16];
	sprintf(buff, "%.8X", val.getARGB());

	return String(buff);
}


colour PropertyHelper::stringToColour(const String& str)
{
	using namespace std;

	argb_t val = 0xFF000000;
	sscanf(str.c_str(), " %8X", &val);

	return colour(val);

}


String PropertyHelper::colourRectToString(const ColourRect& val)
{
	using namespace std;

	char buff[64];
	sprintf(buff, "tl:%.8X tr:%.8X bl:%.8X br:%.8X", val.d_top_left.getARGB(), val.d_top_right.getARGB(), val.d_bottom_left.getARGB(), val.d_bottom_right.getARGB());

	return String(buff);
}


ColourRect PropertyHelper::stringToColourRect(const String& str)
{
	using namespace std;

    if (str.length() == 8)
    {
        argb_t all = 0xFF000000;
        sscanf(str.c_str(), "%8X", &all);
        return ColourRect(all);
    }

	argb_t topLeft = 0xFF000000, topRight = 0xFF000000, bottomLeft = 0xFF000000, bottomRight = 0xFF000000;
	sscanf(str.c_str(), "tl:%8X tr:%8X bl:%8X br:%8X", &topLeft, &topRight, &bottomLeft, &bottomRight);

	return ColourRect(topLeft, topRight, bottomLeft, bottomRight);
}

} // End of  CEGUI namespace section
