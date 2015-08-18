/***********************************************************************
    created:    1st August 2013
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
#ifndef _SVGDataManager_h_
#define _SVGDataManager_h_

#include "CEGUI/Base.h"
#include "CEGUI/Singleton.h"
#include "CEGUI/String.h"
#include "CEGUI/IteratorBase.h"

#include <map>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


namespace CEGUI
{
class SVGData;

class CEGUIEXPORT SVGDataManager :
        public Singleton<SVGDataManager>
{
public:
    SVGDataManager();
    ~SVGDataManager();

    /*!
    \brief
        Creates an instance of SVGData class registered using the name \a name.

    \param name
        String object describing the name that the newly created instance will
        be created with.  This name must be unique within the system. 

    \exception UnknownObjectException
        thrown if no Image subclass has been registered using identifier \a type.

    \exception AlreadyExistsException
        thrown if an Image instance named \a name already exists.
    */
    SVGData& create(const String& name);

    /*!
    \brief
        Creates an instance of SVGData class registered using the name \a name.

    \param name
        String object describing the name that the newly created instance will
        be created with.  This name must be unique within the system. 

    \param filename
        String object that specifies the path and filename of the image file to
        use when creating the texture.

    \param resourceGroup
        String object that specifies the resource group identifier to be passed
        to the resource provider when loading the texture file \a filename.

    \exception UnknownObjectException
        thrown if no Image subclass has been registered using identifier \a type.

    \exception AlreadyExistsException
        thrown if an Image instance named \a name already exists.
    */
    SVGData& create(const String& name,
                    const String& filename,
                    const String& resourceGroup);

    /*!
    \brief
        Destroys the SVGData object.

    \param svgData
        The object to destroy
    */
    void destroy(SVGData& svgData);

    /*!
    \brief
        Destroys the SVGData object registered using the name \a name.

    \param name
        The String holding the name of the SVGData object to be destroyed.
    */
    void destroy(const String& name);

    /*!
    \brief
        Destroys all of the SVGData objects created with this manager.
    */
    void destroyAll();

    /*!
    \brief
        Returns a SVGData object that was previously created by calling a
        create function of this class.

    \param name
        String holding the name of the SVGData object to be returned.

    \exceptions
        - UnknownObjectException - thrown if no SVGData object named \a name
          exists within the system.
    */
    SVGData& getSVGData(const String& name) const;

   
    /*!
    \brief
        Return whether an SVGData object with the given name exists.

    \param name
        String holding the name of the SVGData object to be checked.
    */
    bool isSVGDataDefined(const String& name) const;


    //! container type used to hold the SVGData objects.
    typedef std::map<String, SVGData*> SVGDataMap;

    //! ConstBaseIterator type definition.
    typedef SVGDataMap::iterator SVGDataIterator;

    static SVGDataManager&      getSingleton();
    static SVGDataManager*      getSingletonPtr();

protected:
    /*!
    \brief
        Logs the creation of the SVGData object.

    \param svgData
        The SVGData object that was created.
    */
    void logSVGDataCreation(SVGData* svgData);

    //! container holding the SVGData objects.
    SVGDataMap d_svgDataMap;
};
  


}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif

