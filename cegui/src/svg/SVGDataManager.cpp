/***********************************************************************
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
#include "CEGUI/SharedStringStream.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
// Singleton instance pointer
template<> SVGDataManager* Singleton<SVGDataManager>::ms_Singleton = nullptr;


//----------------------------------------------------------------------------//
SVGDataManager::SVGDataManager()
{

}

//----------------------------------------------------------------------------//
SVGDataManager::~SVGDataManager()
{
    destroyAll();
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
        throw AlreadyExistsException(
            "An SVGData object named " + name + " already exists.");

    SVGData* svg_data = new SVGData(name);
    d_svgDataMap[name] = svg_data;

    logSVGDataCreation(svg_data);

    return *svg_data;
}

//----------------------------------------------------------------------------//
SVGData& SVGDataManager::create(const String& name,
                                const String& filename,
                                const String& resourceGroup)
{
    if (d_svgDataMap.find(name) != d_svgDataMap.end())
        throw AlreadyExistsException(
            "An SVGData object named " + name + " already exists.");

    SVGData* svg_data = new SVGData(name, filename, resourceGroup);
    d_svgDataMap[name] = svg_data;

    logSVGDataCreation(svg_data);

    return *svg_data;
}

//----------------------------------------------------------------------------//
SVGData& SVGDataManager::getSVGData(const String& name) const
{
    SVGDataMap::const_iterator i = d_svgDataMap.find(name);

    if (i == d_svgDataMap.end())
        throw UnknownObjectException(
        "No SVGData named '" + name + "' is available.");

    return *i->second;
}

//----------------------------------------------------------------------------//
bool SVGDataManager::isSVGDataDefined(const String& name) const
{
    return d_svgDataMap.find(name) != d_svgDataMap.end();
}


//----------------------------------------------------------------------------//
void SVGDataManager::logSVGDataCreation(SVGData* svgData)
{
    String addressStr = SharedStringstream::GetPointerAddressAsString(this);

    Logger::getSingleton().logEvent(
        "[SVGDataManager] Created SVGData object: '" + svgData->getName() + "' (" + addressStr + 
        ")");
}

//----------------------------------------------------------------------------//
void SVGDataManager::destroy(SVGData& svgData)
{
    destroy(svgData.getName());
}

//----------------------------------------------------------------------------//
void SVGDataManager::destroy(const String& name)
{
    SVGDataIterator iter = d_svgDataMap.find(name);

    if (iter != d_svgDataMap.end())
    {
        delete iter->second;

        Logger::getSingleton().logEvent(
            "[SVGDataManager] Deleted SVGData object: " + iter->first);

        d_svgDataMap.erase(iter);
    }
}

//----------------------------------------------------------------------------//
void SVGDataManager::destroyAll()
{
    while (!d_svgDataMap.empty())
        destroy(d_svgDataMap.begin()->first);
}

//----------------------------------------------------------------------------//
}
