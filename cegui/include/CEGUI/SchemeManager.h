/***********************************************************************
    created:    Mon Jul 20 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUISchemeManager_h_
#define _CEGUISchemeManager_h_

#include "CEGUI/Singleton.h"
#include "CEGUI/XmlResourceExistsAction.h"
#include "CEGUI/ResourceEventSet.h"
#include <unordered_map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    A class that manages the creation of, access to, and destruction of GUI
    Scheme instances. Handled loading of Scheme files.
*/
class CEGUIEXPORT SchemeManager :
        public Singleton<SchemeManager>,
        public ResourceEventSet
{
public:
    //! Constructor.
    SchemeManager();

    //! Destructor.
    ~SchemeManager();

    //! type of collection used to store and manage instances
    typedef std::unordered_map<String, Scheme*> SchemeRegistry;

    /*!
    \brief
        Creates a new Scheme instance from a RawDataContainer and adds it to the collection.

        Use an instance of the XML resource loading class \a U to process the
        XML source thereby creating an instance of class \a Scheme and add it to the collection under
        the name specified in the XML file.

    \param source
        RawDataContainer holding the XML source to be used when creating the
        new Scheme instance.

    \param resourceExistsAction
        One of the XmlResourceExistsAction enumerated values indicating what
        action should be taken when a Scheme with the specified name
        already exists within the collection.
    */
    Scheme& createFromContainer(const RawDataContainer& source,
        XmlResourceExistsAction resourceExistsAction = XmlResourceExistsAction::Return);

    /*!
    \brief
        Creates a new Scheme instance from an XML file and adds it to the collection.

        Use an instance of the XML resource loading class \a U to process the
        XML file \a xml_filename from resource group \a resource_group thereby
        creating an instance of class \a T and add it to the collection under
        the name specified in the XML file.

    \param xml_filename
        String holding the filename of the XML file to be used when creating the
        new Scheme instance.

    \param resource_group
        String holding the name of the resource group identifier to be used
        when loading the XML file described by \a xml_filename.

    \param resourceExistsAction
        One of the XmlResourceExistsAction enumerated values indicating what
        action should be taken when a Scheme with the specified name
        already exists within the collection.
    */
    Scheme& createFromFile(const String& xml_filename, const String& resource_group = "",
        XmlResourceExistsAction resourceExistsAction = XmlResourceExistsAction::Return);

    /*!
    \brief
        Creates a new Scheme instance from a string and adds it to the collection.

        Use an instance of the XML resource loading class \a U to process the
        XML source thereby creating an instance of class \a T and add it to the collection under
        the name specified in the XML file.

    \param source
        String holding the XML source to be used when creating the
        new Scheme instance.

    \param resourceExistsAction
        One of the XmlResourceExistsAction enumerated values indicating what
        action should be taken when an Scheme with the specified name
        already exists within the collection.
    */
    Scheme& createFromString(const String& source,
        XmlResourceExistsAction resourceExistsAction = XmlResourceExistsAction::Return);

        /*!
    \brief
        Destroy the Scheme named \a scheme_name, or do nothing if such a
        Scheme does not exist in the collection.

    \param scheme_name
        String holding the name of the Scheme to be destroyed.
    */
    void destroy(const String& scheme_name);

    /*!
    \brief
        Destroy the Scheme \a scheme, or do nothing if such a
        Scheme does not exist in the collection.

    \param scheme
        The Scheme to be destroyed (beware of keeping references to this Scheme
        once it's been destroyed!)
    */
    void destroy(const Scheme& scheme);

    //! Destroy all Schemes.
    void destroyAll();

    /*!
    \brief
        Return a reference to the Scheme named \a scheme_name.

    \param scheme_name
        String holding the name of the Scheme to be returned.

    \exception UnknownObjectException
        thrown if no Scheme named \a scheme_name exists within the collection.
    */
    Scheme& get(const String& scheme_name) const;

    //! Return whether a Scheme named \a scheme_name exists.
    bool isDefined(const String& scheme_name) const;

    //! Create a new Scheme instance from files with names matching \a pattern in \a resource_group
    void createAll(const String& pattern, const String& resource_group);

    /*!
    \brief
        Returns a reference to the map of registered Schemes
    \return
        A map containing the names of and the pointers to the registered Schemes 
    */
    const SchemeRegistry& getRegisteredSchemes() const;
    
    /*!
    \brief
        If this is enabled, Schemas will immediately load their resources after they are created
        
    It's sometimes useful to turn this off when you want to load things more selectively.
    This is enabled by default.
    
    \param enabled
        If true, you will have to load resources from the Scheme yourself!
    
    \note
        Calling Scheme::loadResources after you create the Scheme is equivalent to this being enabled
        and creating the scheme.
    */
    void setAutoLoadResources(bool enabled);
    
    /*!
    \brief
        Checks whether resources are loaded immediately after schemes are created
        
    \see
        SchemeManager::setAutoLoadResources
    */
    bool getAutoLoadResources() const;

protected:
    //! implementation of object destruction.
    void destroyObject(SchemeRegistry::iterator ob);

    //! function to enforce XmlResourceExistsAction policy.
    Scheme& doExistingObjectAction(const String scheme_name, Scheme* objeschemect,
                                    const XmlResourceExistsAction resourceExistsAction);

    //! Function called each time a new object is added to the collection.
    void doPostObjectAdditionAction(Scheme& scheme);


    //! String holding the text for the resource type managed.
    const String d_resourceType;

    //! Map of registered Schemes, containing name and pointer to the instance
    SchemeRegistry d_registeredSchemes;

    //! If true, Scheme::loadResources is called after "create" is called for it
    bool d_autoLoadResources;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUISchemeManager_h_
