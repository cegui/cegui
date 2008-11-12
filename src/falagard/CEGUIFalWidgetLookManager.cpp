/***********************************************************************
    filename:   CEGUIFalWidgetLookManager.cpp
    created:    Mon Jun 13 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalagard_xmlHandler.h"
#include "CEGUIResourceProvider.h"
#include "CEGUIXMLParser.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    ////////////////////////////////////////////////////////////////////////////////
    // Static data definitions.
    template<> WidgetLookManager* Singleton<WidgetLookManager>::ms_Singleton = 0;
    const String WidgetLookManager::FalagardSchemaName("Falagard.xsd");
    String WidgetLookManager::d_defaultResourceGroup;
    ////////////////////////////////////////////////////////////////////////////////

    WidgetLookManager::WidgetLookManager()
    {
        char addr_buff[32];
        sprintf(addr_buff, "(%p)", static_cast<void*>(this));
        Logger::getSingleton().logEvent("CEGUI::WidgetLookManager singleton "
            "created. " + String(addr_buff));
    }

    WidgetLookManager::~ WidgetLookManager()
    {
        char addr_buff[32];
        sprintf(addr_buff, "(%p)", static_cast<void*>(this));
        Logger::getSingleton().logEvent("CEGUI::WidgetLookManager singleton "
            "destroyed. " + String(addr_buff));
    }

    /*************************************************************************
        Return singleton object
      *************************************************************************/
      WidgetLookManager&    WidgetLookManager::getSingleton(void)
      {
          return Singleton<WidgetLookManager>::getSingleton();
      }

      /*************************************************************************
          Return singleton pointer
      *************************************************************************/
      WidgetLookManager*    WidgetLookManager::getSingletonPtr(void)
      {
          return Singleton<WidgetLookManager>::getSingletonPtr();
    }

    void WidgetLookManager::parseLookNFeelSpecification(const String& filename, const String& resourceGroup)
    {
        // valid filenames are required!
        if (filename.empty())
        {
            throw InvalidRequestException("WidgetLookManager::parseLookNFeelSpecification - Filename supplied for look & feel file must be valid");
        }

        // create handler object
        Falagard_xmlHandler handler(this);

        // perform parse of XML data
        try
        {
            System::getSingleton().getXMLParser()->parseXMLFile(
                handler, filename, FalagardSchemaName,
                resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);
        }
        catch(...)
        {
            Logger::getSingleton().logEvent("WidgetLookManager::parseLookNFeelSpecification - loading of look and feel data from file '" + filename +"' has failed.", Errors);
            throw;
        }
    }

    bool WidgetLookManager::isWidgetLookAvailable(const String& widget) const
    {
        return d_widgetLooks.find(widget) != d_widgetLooks.end();
    }

    const WidgetLookFeel& WidgetLookManager::getWidgetLook(const String& widget) const
    {
        WidgetLookList::const_iterator wlf = d_widgetLooks.find(widget);

        if (wlf != d_widgetLooks.end())
        {
            return (*wlf).second;
        }

        throw UnknownObjectException("WidgetLookManager::getWidgetLook - Widget look and feel '" + widget + "' does not exist.");
    }

    void WidgetLookManager::eraseWidgetLook(const String& widget)
    {
        WidgetLookList::iterator wlf = d_widgetLooks.find(widget);
        if (wlf != d_widgetLooks.end())
        {
            d_widgetLooks.erase(wlf);
        }
        else
        {
            Logger::getSingleton().logEvent(
                "WidgetLookManager::eraseWidgetLook - Widget look and feel '" + widget + "' did not exist.");
        }
    }

    void WidgetLookManager::addWidgetLook(const WidgetLookFeel& look)
    {
        if (isWidgetLookAvailable(look.getName()))
        {
            Logger::getSingleton().logEvent(
                "WidgetLookManager::addWidgetLook - Widget look and feel '" + look.getName() + "' already exists.  Replacing previous definition.");
        }

        d_widgetLooks[look.getName()] = look;
    }

    void WidgetLookManager::writeWidgetLookToStream(const String& name, OutStream& out_stream) const
    {
        // start of file
        // output xml header
        XMLSerializer xml(out_stream);
        // output root element
        xml.openTag("Falagard");
        try
        {
            // output the desired widget look data
            getWidgetLook(name).writeXMLToStream(xml);
        }
        catch (UnknownObjectException&)
        {
            Logger::getSingleton().logEvent("WidgetLookManager::writeWidgetLookToStream - Failed to write widget look XML data to stream.", Errors);
        }

        // close the root tags to terminate the file
        xml.closeTag();
    }

    void WidgetLookManager::writeWidgetLookSeriesToStream(const String& prefix, OutStream& out_stream) const
    {
        // start of file
        // output xml header
        XMLSerializer xml(out_stream);
        // output root element
        xml.openTag("Falagard");
        for (WidgetLookList::const_iterator curr = d_widgetLooks.begin(); curr != d_widgetLooks.end(); ++curr)
        {
            if ((*curr).first.compare(0, prefix.length(), prefix) == 0)
                (*curr).second.writeXMLToStream(xml);
        }

        // close the root tags to terminate the file
        xml.closeTag();
    }

} // End of  CEGUI namespace section
