/***********************************************************************
	filename: 	CEGUIScheme.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements GUI Scheme class
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
#include "CEGUIScheme.h"
#include "CEGUIExceptions.h"
#include "CEGUISchemeManager.h"
#include "CEGUILogger.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIWindowRendererManager.h"
#include "CEGUIFactoryModule.h"
#include "CEGUIScheme_xmlHandler.h"
#include "CEGUIDataContainer.h"
#include "CEGUISystem.h"
#include "CEGUIXMLParser.h"
#include "falagard/CEGUIFalWidgetLookManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Static Data definitions
*************************************************************************/
// name of the xml schema for GUIScheme files
const char Scheme::GUISchemeSchemaName[]					= "GUIScheme.xsd";
// default resource group
String Scheme::d_defaultResourceGroup;


/*************************************************************************
	Constructor for scheme objects
*************************************************************************/
Scheme::Scheme(const String& filename, const String& resourceGroup)
{
	if (filename.empty())
	{
		throw InvalidRequestException("Scheme::Scheme - Filename supplied for Scheme loading must be valid");
	}

    // create handler object
    Scheme_xmlHandler handler(this);

    // do parse (which uses handler to create actual data)
	try
	{
        System::getSingleton().getXMLParser()->parseXMLFile(
            handler, filename, GUISchemeSchemaName,
            resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);
	}
	catch(...)
	{
        Logger::getSingleton().logEvent("Scheme::Scheme - loading of Scheme from file '" + filename +"' failed.", Errors);
        throw;
	}

	Logger::getSingleton().logEvent("Loaded GUI scheme '" + d_name + "' from data in file '" + filename + "'.", Informative);

	// attempt to load in resources
	loadResources();
}


/*************************************************************************
	Destructor for scheme objects
*************************************************************************/
Scheme::~Scheme(void)
{
	unloadResources();

	Logger::getSingleton().logEvent("GUI scheme '" + d_name + "' has been unloaded.", Informative);
}


/*************************************************************************
	Load all resources for this scheme
*************************************************************************/
void Scheme::loadResources(void)
{
    Logger::getSingleton().logEvent("---- Begining resource loading for GUI scheme '" + d_name + "' ----", Informative);

    // load all resources specified for this scheme.
    loadXMLImagesets();
    loadImageFileImagesets();
    loadFonts();
    loadLookNFeels();
    loadWindowRendererFactories();
    loadWindowFactories();
    loadFactoryAliases();
    loadFalagardMappings();

    Logger::getSingleton().logEvent("---- Resource loading for GUI scheme '" + d_name + "' completed ----", Informative);
}


/*************************************************************************
	Unload all resources for this scheme
*************************************************************************/
void Scheme::unloadResources(void)
{
    Logger::getSingleton().logEvent("---- Begining resource cleanup for GUI scheme '" + d_name + "' ----", Informative);

    // unload all resources specified for this scheme.
    unloadFonts();
    unloadXMLImagesets();
    unloadImageFileImagesets();
    unloadWindowFactories();
    unloadWindowRendererFactories();
    unloadFactoryAliases();
    unloadFalagardMappings();
    unloadLookNFeels();

    Logger::getSingleton().logEvent("---- Resource cleanup for GUI scheme '" + d_name + "' completed ----", Informative);
}


/*************************************************************************
	Check if all resources for this Scheme are loaded
*************************************************************************/
bool Scheme::resourcesLoaded(void) const
{
    // test state of all loadable resources for this scheme.
    if (areXMLImagesetsLoaded() &&
        areImageFileImagesetsLoaded() &&
        areFontsLoaded() &&
        areWindowRendererFactoriesLoaded() &&
        areWindowFactoriesLoaded() &&
        areFactoryAliasesLoaded() &&
        areFalagardMappingsLoaded())
    {
        return true;
    }

    return false;
}


/*************************************************************************
    Load all xml imagesets specified.
*************************************************************************/
void Scheme::loadXMLImagesets()
{
    ImagesetManager& ismgr = ImagesetManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // check all imagesets
    for (pos = d_imagesets.begin(); pos != d_imagesets.end(); ++pos)
    {
        // does such an imageset exist?
        if (!ismgr.isImagesetPresent((*pos).name))
        {
            // create imageset from specified file.
            Imageset* iset = ismgr.createImageset((*pos).filename, (*pos).resourceGroup);

            // check for wrong imageset for specified name
            String realname = iset->getName();

            if (realname != (*pos).name)
            {
                ismgr.destroyImageset(iset);
                throw InvalidRequestException("Scheme::loadResources - The Imageset created by file '" +
                    (*pos).filename + "' is named '" + realname + "', not '" + (*pos).name + "' as required by Scheme '" + d_name + "'.");
            }
        }
    }
}

/*************************************************************************
    Load all image file based imagesets specified.
*************************************************************************/
void Scheme::loadImageFileImagesets()
{
    ImagesetManager& ismgr = ImagesetManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // check imagesets that are created directly from image files
    for (pos = d_imagesetsFromImages.begin(); pos != d_imagesetsFromImages.end(); ++pos)
    {
        // see if imageset is present, and create it if not.
        if (!ismgr.isImagesetPresent((*pos).name))
            ismgr.createImagesetFromImageFile((*pos).name, (*pos).filename, (*pos).resourceGroup);
    }
}

/*************************************************************************
    Load all xml based fonts specified.
*************************************************************************/
void Scheme::loadFonts()
{
    FontManager& fntmgr = FontManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // check fonts
    for (pos = d_fonts.begin(); pos != d_fonts.end(); ++pos)
    {
        // is such a font already loaded?
        if (!fntmgr.isFontPresent((*pos).name))
        {
            // create font using specified xml file.
            Font* font = fntmgr.createFont((*pos).filename, (*pos).resourceGroup);

            // check for wrong font for specified name
            String realname = font->getProperty ("Name");

            if (realname != (*pos).name)
            {
                fntmgr.destroyFont(font);
                throw InvalidRequestException("Scheme::loadResources - The Font created by file '" +
                    (*pos).filename + "' is named '" + realname + "', not '" + (*pos).name + "' as required by Scheme '" + d_name + "'.");
            }
        }
    }
}

/*************************************************************************
    Load all xml LookNFeel files specified.
*************************************************************************/
void Scheme::loadLookNFeels()
{
    WidgetLookManager& wlfMgr   = WidgetLookManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // load look'n'feels
    // (we can't actually check these, at the moment, so we just re-parse data;
    // it does no harm except maybe waste a bit of time)
    for (pos = d_looknfeels.begin(); pos != d_looknfeels.end(); ++pos)
        wlfMgr.parseLookNFeelSpecification((*pos).filename, (*pos).resourceGroup);
}

/*************************************************************************
    Load all windowset modules specified.and register factories.
*************************************************************************/
void Scheme::loadWindowFactories()
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();

    // check factories
    std::vector<UIModule>::iterator cmod = d_widgetModules.begin();
    for (;cmod != d_widgetModules.end(); ++cmod)
    {
        // create and load dynamic module as required
        if (!(*cmod).module)
        {
            (*cmod).module = new FactoryModule((*cmod).name);
        }

        // see if we should just register all factories available in the module (i.e. No factories explicitly specified)
        if ((*cmod).factories.size() == 0)
        {
            Logger::getSingleton().logEvent("No window factories specified for module '" + (*cmod).name + "' - adding all available factories...");
            (*cmod).module->registerAllFactories();
        }
        // some names were explicitly given, so only register those.
        else
        {
            std::vector<UIElementFactory>::const_iterator   elem = (*cmod).factories.begin();
            for (; elem != (*cmod).factories.end(); ++elem)
            {
                if (!wfmgr.isFactoryPresent((*elem).name))
                {
                    (*cmod).module->registerFactory((*elem).name);
                }
            }
        }
    }
}

/*************************************************************************
    Load all windowrendererset modules specified.and register factories.
*************************************************************************/
void Scheme::loadWindowRendererFactories()
{
    WindowRendererManager& wfmgr = WindowRendererManager::getSingleton();

    // check factories
    std::vector<UIModule>::iterator cmod = d_windowRendererModules.begin();
    for (;cmod != d_windowRendererModules.end(); ++cmod)
    {
        // create and load dynamic module as required
        if (!(*cmod).module)
        {
            (*cmod).module = new FactoryModule((*cmod).name);
        }

        // see if we should just register all factories available in the module (i.e. No factories explicitly specified)
        if ((*cmod).factories.size() == 0)
        {
            Logger::getSingleton().logEvent("No window renderer factories specified for module '" + (*cmod).name + "' - adding all available factories...");
            (*cmod).module->registerAllFactories();
        }
        // some names were explicitly given, so only register those.
        else
        {
            std::vector<UIElementFactory>::const_iterator   elem = (*cmod).factories.begin();
            for (; elem != (*cmod).factories.end(); ++elem)
            {
                if (!wfmgr.isFactoryPresent((*elem).name))
                {
                    (*cmod).module->registerFactory((*elem).name);
                }
            }
        }
    }
}

/*************************************************************************
    Create all window factory aliases
*************************************************************************/
void Scheme::loadFactoryAliases()
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();

    // check aliases
    std::vector<AliasMapping>::iterator alias = d_aliasMappings.begin();
    for (;alias != d_aliasMappings.end(); ++alias)
    {
        // get iterator 
        WindowFactoryManager::TypeAliasIterator iter = wfmgr.getAliasIterator();

        // look for this alias
        while (!iter.isAtEnd() && (iter.getCurrentKey() != (*alias).aliasName))
            ++iter;

        // if the alias exists
        if (!iter.isAtEnd())
        {
            // if the current target type matches
            if (iter.getCurrentValue().getActiveTarget() == (*alias).targetName)
            {
                // assume this mapping is ours and skip to next alias
                continue;
            }
        }

        // create a new alias entry
        wfmgr.addWindowTypeAlias((*alias).aliasName, (*alias).targetName);
    }
}

/*************************************************************************
    Create all required falagard mappings
*************************************************************************/
void Scheme::loadFalagardMappings()
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();

    // check falagard window mappings.
    std::vector<FalagardMapping>::iterator falagard = d_falagardMappings.begin();
    for (;falagard != d_falagardMappings.end(); ++falagard)
    {
        // get iterator
        WindowFactoryManager::FalagardMappingIterator iter = wfmgr.getFalagardMappingIterator();

        // look for this mapping
        while (!iter.isAtEnd() && (iter.getCurrentKey() != (*falagard).windowName))
            ++iter;

        // if the mapping exists
        if (!iter.isAtEnd())
        {
            // check if the current target and looks and window renderer match
            if ((iter.getCurrentValue().d_baseType == (*falagard).targetName) &&
                (iter.getCurrentValue().d_rendererType == (*falagard).rendererName) &&
                (iter.getCurrentValue().d_lookName == (*falagard).lookName))
            {
                // assume this mapping is ours and skip to next
                continue;
            }
        }

        // create a new mapping entry
        wfmgr.addFalagardWindowMapping((*falagard).windowName, (*falagard).targetName, (*falagard).lookName, (*falagard).rendererName);
    }
}

/*************************************************************************
    Unload all xml based imagesets
*************************************************************************/
void Scheme::unloadXMLImagesets()
{
    ImagesetManager& ismgr      = ImagesetManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // unload all xml based Imagesets
    for (pos = d_imagesets.begin(); pos != d_imagesets.end(); ++pos)
        ismgr.destroyImageset((*pos).name);
}

/*************************************************************************
    Unload all image file based imagesets
*************************************************************************/
void Scheme::unloadImageFileImagesets()
{
    ImagesetManager& ismgr      = ImagesetManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // unload all imagesets that are created directly from image files
    for (pos = d_imagesetsFromImages.begin(); pos != d_imagesetsFromImages.end(); ++pos)
        ismgr.destroyImageset((*pos).name);
}

/*************************************************************************
    Unload all xml based fonts
*************************************************************************/
void Scheme::unloadFonts()
{
    FontManager& fntmgr         = FontManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // unload all fonts
    for (pos = d_fonts.begin(); pos != d_fonts.end(); ++pos)
        fntmgr.destroyFont((*pos).name);
}

/*************************************************************************
    Unload all xml based LookNfeels
*************************************************************************/
void Scheme::unloadLookNFeels()
{
    // While it's possible to remove WidgetLooks etc, currently we can't
    // extract which looks were loaded from a given xml file, so we just
    // do nothing for now.
}

/*************************************************************************
    Unregister all window factories and unload shared modules
*************************************************************************/
void Scheme::unloadWindowFactories()
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();
    std::vector<UIModule>::iterator cmod = d_widgetModules.begin();

    // for all widget modules loaded
    for (;cmod != d_widgetModules.end(); ++cmod)
    {
        // see if we should just unregister all factories available in the
        // module (i.e. No factories explicitly specified)
        if ((*cmod).factories.size() == 0)
        {
            // TODO: This is not supported yet!
        }
        // remove all window factories explicitly registered for this module
        else
        {
            std::vector<UIElementFactory>::const_iterator elem = (*cmod).factories.begin();
            for (; elem != (*cmod).factories.end(); ++elem)
                wfmgr.removeFactory((*elem).name);
        }

        // unload dynamic module as required
        if ((*cmod).module)
        {
            delete (*cmod).module;
            (*cmod).module = 0;
        }
    }
}

/*************************************************************************
    Unregister all window renderer factories and unload shared modules
*************************************************************************/
void Scheme::unloadWindowRendererFactories()
{
    WindowRendererManager& wfmgr = WindowRendererManager::getSingleton();
    std::vector<UIModule>::iterator cmod = d_windowRendererModules.begin();

    // for all widget modules loaded
    for (;cmod != d_windowRendererModules.end(); ++cmod)
    {
        // see if we should just unregister all factories available in the
        // module (i.e. No factories explicitly specified)
        if ((*cmod).factories.size() == 0)
        {
            // TODO: This is not supported yet!
        }
        // remove all window factories explicitly registered for this module
        else
        {
            std::vector<UIElementFactory>::const_iterator elem = (*cmod).factories.begin();
            for (; elem != (*cmod).factories.end(); ++elem)
                wfmgr.removeFactory((*elem).name);
        }

        // unload dynamic module as required
        if ((*cmod).module)
        {
            delete (*cmod).module;
            (*cmod).module = 0;
        }
    }
}

/*************************************************************************
    Unregister all factory aliases
*************************************************************************/
void Scheme::unloadFactoryAliases()
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();
    std::vector<AliasMapping>::iterator alias = d_aliasMappings.begin();

    // remove all factory aliases
    for (;alias != d_aliasMappings.end(); ++alias)
    {
        // get iterator 
        WindowFactoryManager::TypeAliasIterator iter = wfmgr.getAliasIterator();

        // look for this alias
        while (!iter.isAtEnd() && (iter.getCurrentKey() != (*alias).aliasName))
            ++iter;

        // if the alias exists
        if (!iter.isAtEnd())
            // remove this alias entry
            wfmgr.removeWindowTypeAlias((*alias).aliasName, (*alias).targetName);
    }
}

/*************************************************************************
    Unregister all Falagard window type mappings
*************************************************************************/
void Scheme::unloadFalagardMappings()
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();
    std::vector<FalagardMapping>::iterator falagard = d_falagardMappings.begin();

    // remove all falagard window mappings for this scheme.
    for (;falagard != d_falagardMappings.end(); ++falagard)
    {
        // get iterator
        WindowFactoryManager::FalagardMappingIterator iter = wfmgr.getFalagardMappingIterator();

        // look for this mapping
        while (!iter.isAtEnd() && (iter.getCurrentKey() != (*falagard).windowName))
            ++iter;

        // if the mapping exists
        if (!iter.isAtEnd())
        {
            // if the current target and looks match
            if ((iter.getCurrentValue().d_baseType == (*falagard).targetName) &&
                (iter.getCurrentValue().d_rendererType == (*falagard).rendererName) &&
                (iter.getCurrentValue().d_lookName == (*falagard).lookName))
            {
                // assume this mapping is ours and delete it
                wfmgr.removeFalagardWindowMapping((*falagard).targetName);
            }
        }
    }
}

/*************************************************************************
    Check if all XML based imagesets are loaded
*************************************************************************/
bool Scheme::areXMLImagesetsLoaded() const
{
    ImagesetManager& ismgr = ImagesetManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // check imagesets
    for (pos = d_imagesets.begin(); pos != d_imagesets.end(); ++pos)
    {
        if (!ismgr.isImagesetPresent((*pos).name))
            return false;
    }

    return true;
}

/*************************************************************************
    Check if all image file based imagesets are loaded
*************************************************************************/
bool Scheme::areImageFileImagesetsLoaded() const
{
    ImagesetManager& ismgr = ImagesetManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    for (pos = d_imagesetsFromImages.begin(); pos != d_imagesetsFromImages.end(); ++pos)
    {
        if (!ismgr.isImagesetPresent((*pos).name))
            return false;
    }

    return true;
}

/*************************************************************************
    Check if all xml based fonts are loaded
*************************************************************************/
bool Scheme::areFontsLoaded() const
{
    FontManager& fntmgr = FontManager::getSingleton();
    std::vector<LoadableUIElement>::const_iterator  pos;

    // check fonts
    for (pos = d_fonts.begin(); pos != d_fonts.end(); ++pos)
    {
        if (!fntmgr.isFontPresent((*pos).name))
            return false;
    }

    return true;
}

/*************************************************************************
    Check if xml looknfeel files are loaded
*************************************************************************/
bool Scheme::areLookNFeelsLoaded() const
{
    // can't check these at the moment, just say yes!
    return true;
}

/*************************************************************************
    Check if all required window factories are registered
*************************************************************************/
bool Scheme::areWindowFactoriesLoaded() const
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();
    std::vector<UIModule>::const_iterator   cmod = d_widgetModules.begin();

    // check factory modules
    for (;cmod != d_widgetModules.end(); ++cmod)
    {
        // see if we should just test all factories available in the
        // module (i.e. No factories explicitly specified)
        if ((*cmod).factories.size() == 0)
        {
            // TODO: This is not supported yet!
        }
        // check all window factories explicitly registered for this module
        else
        {
            std::vector<UIElementFactory>::const_iterator   elem = (*cmod).factories.begin();

            for (; elem != (*cmod).factories.end(); ++elem)
            {
                if (!wfmgr.isFactoryPresent((*elem).name))
                    return false;
            }
        }
    }

    return true;
}

/*************************************************************************
    Check if all required window renderer factories are registered
*************************************************************************/
bool Scheme::areWindowRendererFactoriesLoaded() const
{
    WindowRendererManager& wfmgr = WindowRendererManager::getSingleton();
    std::vector<UIModule>::const_iterator   cmod = d_windowRendererModules.begin();

    // check factory modules
    for (;cmod != d_windowRendererModules.end(); ++cmod)
    {
        // see if we should just test all factories available in the
        // module (i.e. No factories explicitly specified)
        if ((*cmod).factories.size() == 0)
        {
            // TODO: This is not supported yet!
        }
        // check all window factories explicitly registered for this module
        else
        {
            std::vector<UIElementFactory>::const_iterator   elem = (*cmod).factories.begin();

            for (; elem != (*cmod).factories.end(); ++elem)
            {
                if (!wfmgr.isFactoryPresent((*elem).name))
                    return false;
            }
        }
    }

    return true;
}

/*************************************************************************
    Check if all window factory aliases are registered
*************************************************************************/
bool Scheme::areFactoryAliasesLoaded() const
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();
    std::vector<AliasMapping>::const_iterator alias = d_aliasMappings.begin();

    // check aliases
    for (;alias != d_aliasMappings.end(); ++alias)
    {
        // get iterator 
        WindowFactoryManager::TypeAliasIterator iter = wfmgr.getAliasIterator();

        // look for this alias
        while (!iter.isAtEnd() && (iter.getCurrentKey() != (*alias).aliasName))
            ++iter;

        // if the alias exists
        if (!iter.isAtEnd())
        {
            // if the current target type matches
            if (iter.getCurrentValue().getActiveTarget() == (*alias).targetName)
                // target matches, assume we set it and continue to next alias
                continue;
        }

        // no alias or target type does not match
        return false;
    }

    return true;
}

/*************************************************************************
    Check if all falagard mappings are registered
*************************************************************************/
bool Scheme::areFalagardMappingsLoaded() const
{
    WindowFactoryManager& wfmgr = WindowFactoryManager::getSingleton();
    std::vector<FalagardMapping>::const_iterator falagard = d_falagardMappings.begin();

    // check falagard window mappings.
    for (;falagard != d_falagardMappings.end(); ++falagard)
    {
        // get iterator
        WindowFactoryManager::FalagardMappingIterator iter = wfmgr.getFalagardMappingIterator();

        // look for this mapping
        while (!iter.isAtEnd() && (iter.getCurrentKey() != (*falagard).windowName))
            ++iter;

        // if the mapping exists
        if (!iter.isAtEnd())
        {
            // if the current target and looks match
            if ((iter.getCurrentValue().d_baseType == (*falagard).targetName) &&
                (iter.getCurrentValue().d_rendererType == (*falagard).rendererName) &&
                (iter.getCurrentValue().d_lookName == (*falagard).lookName))
            {
                // assume this mapping is ours and skip to next
                continue;
            }
        }

        return false;
    }

    return true;
}

} // End of  CEGUI namespace section
