/***********************************************************************
    filename:   SVGDataManager.cpp
    created:    30th July 2013
    author:     Lukas Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/svg/SVGDataManager.h"
#include "CEGUI/svg/SVGData.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
// Singleton instance pointer
template<> SVGDataManager* Singleton<SVGDataManager>::ms_Singleton = 0;


//----------------------------------------------------------------------------//
SVGDataManager::SVGDataManager()
{

}

//----------------------------------------------------------------------------//
SVGDataManager::~SVGDataManager()
{

}

//----------------------------------------------------------------------------//
SVGDataManager&	SVGDataManager::getSingleton(void)
{
	return Singleton<SVGDataManager>::getSingleton();
}

//----------------------------------------------------------------------------//
SVGDataManager*	SVGDataManager::getSingletonPtr(void)
{
	return Singleton<SVGDataManager>::getSingletonPtr();
}

//----------------------------------------------------------------------------//
SVGData& SVGDataManager::create(const String& name)
{
    if (d_svgDataMap.find(name) != d_svgDataMap.end())
        CEGUI_THROW(AlreadyExistsException(
        "SVGData already exists: " + name));

    SVGData* svg_data = CEGUI_NEW_AO SVGData();
    d_svgDataMap[name] = svg_data;

    char addr_buff[32];
    sprintf(addr_buff, "%p", static_cast<void*>(&svg_data));

    Logger::getSingleton().logEvent(
        "[SVGDataManager] Created SVGData: '" + name + "' (" + addr_buff + 
        ")");

    return *svg_data;
}

//----------------------------------------------------------------------------//
}
