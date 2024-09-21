/***********************************************************************
    created:    20/2/2004
    author:        Paul D Turner

    purpose:    Implementation of main system object
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
#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/DefaultLogger.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/WindowFactoryManager.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/RenderEffectManager.h"
#include "CEGUI/AnimationManager.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/Clipboard.h"
#include "CEGUI/ScriptModule.h"
#include "CEGUI/Config_xmlHandler.h"
#include "CEGUI/GlobalEventSet.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/WindowRendererManager.h"
#include "CEGUI/DynamicModule.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/text/LegacyTextParser.h"
#include "CEGUI/RenderingWindow.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/widgets/All.h"
#include "CEGUI/SharedStringStream.h"
#include "CEGUI/svg/SVGDataManager.h"
#if defined(__WIN32__) || defined(_WIN32)
#    include "CEGUI/Win32ClipboardProvider.h"
#endif

// declare create / destroy functions used for XMLParser and ImageCodec
// modules as extern when static linking
#if defined(CEGUI_STATIC)
extern "C"
{
// XML Parser
CEGUI::XMLParser* createParser(void);
void destroyParser(CEGUI::XMLParser* parser);
// Image codec
CEGUI::ImageCodec* createImageCodec(void);
void destroyImageCodec(CEGUI::ImageCodec* imageCodec);
}
#endif

#define S_(X) #X
#define STRINGIZE(X) S_(X)

// Start of CEGUI namespace section
namespace CEGUI
{
const String System::EventNamespace("System");

/*************************************************************************
    Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> System* Singleton<System>::ms_Singleton    = nullptr;
// instance of class that can convert string encodings
#if defined(__WIN32__) || defined(_WIN32)
const Win32StringTranscoder System::d_stringTranscoder;
#else
const IconvStringTranscoder System::d_stringTranscoder;
#endif

// event names
const String System::EventDisplaySizeChanged( "DisplaySizeChanged" );
const String System::EventTextParserChanged("TextParserChanged");

// Holds name of default XMLParser
String System::d_defaultXMLParserName(STRINGIZE(CEGUI_DEFAULT_XMLPARSER));
// Holds name of default ImageCodec
String System::d_defaultImageCodecName(STRINGIZE(CEGUI_DEFAULT_IMAGE_CODEC));


/*************************************************************************
    Constructor
*************************************************************************/
System::System(Renderer& renderer,
               ResourceProvider* resourceProvider,
               XMLParser* xmlParser,
               ImageCodec* imageCodec,
               ScriptModule* scriptModule,
               const String& configFile,
               const String& logFile):
    d_renderer(&renderer),
    d_resourceProvider(resourceProvider),
    d_ourResourceProvider(false),
    d_clipboard(new Clipboard()),
    d_nativeClipboardProvider(nullptr),
    d_scriptModule(scriptModule),
    d_xmlParser(xmlParser),
    d_ourXmlParser(false),
    d_parserModule(nullptr),
    d_imageCodec(imageCodec),
    d_ourImageCodec(false),
    d_imageCodecModule(nullptr),
    d_ourLogger(Logger::getSingletonPtr() == nullptr),
    d_fallbackTextParser(std::make_unique<LegacyTextParser>())
{
    d_defaultTextParser = d_fallbackTextParser.get();

    // Start out by fixing the numeric locale to C (we depend on this behaviour)
    // consider a UVector2 as a property {{0.5,0},{0.5,0}} could become {{0,5,0},{0,5,0}}
    setlocale(LC_NUMERIC, "C");

    // Instantiate logger first (we have no file at this point, but entries will
    // be cached until we do)
    //
    // NOTE: If the user already created a logger prior to calling this
    // constructor, we mark it as so and leave the logger untouched. This allows
    // the user to fully customize the logger as he sees fit without fear of
    // seeing its configuration overwritten by this.
#ifdef CEGUI_USE_DEFAULT_LOGGER
    if (d_ourLogger)
        new DefaultLogger();
#endif

#if defined(__WIN32__) || defined(_WIN32)
    d_nativeClipboardProvider = new Win32ClipboardProvider;
    // set the default win 32 clipboard provider
    d_clipboard->setNativeProvider(d_nativeClipboardProvider);
#endif

    Logger& logger(Logger::getSingleton());

    // create default resource provider, unless one was already provided
    if (!d_resourceProvider)
    {
        d_resourceProvider = new DefaultResourceProvider();
        d_ourResourceProvider = true;
    }

    // handle initialisation and setup of the XML parser
    setupXMLParser();

    // now XML is available, read the configuration file (if any)
    Config_xmlHandler config;
    if (!configFile.empty())
    {
        try
        {
            d_xmlParser->parseXMLFile(config, configFile,
                                      config.CEGUIConfigSchemaName,
                                      "");
        }
        catch (...)
        {
            // cleanup XML stuff
            d_xmlParser->cleanup();
            delete d_xmlParser;
            throw;
        }
    }

    // Initialise logger if the user didn't create a logger beforehand
    if (d_ourLogger)
        config.initialiseLogger(logFile);

    // if we created the resource provider we know it's DefaultResourceProvider
    // so can auto-initialise the resource group directories via the config
    if (d_ourResourceProvider)
        config.initialiseResourceGroupDirectories();

    // get config to update XML parser if it needs to.
    config.initialiseXMLParser();

    // set up ImageCodec
    config.initialiseImageCodec();
    if (!d_imageCodec)
        setupImageCodec("");

    // initialise any default resource groups specified in the config.
    config.initialiseDefaultResourceGroups();

    outputLogHeader();

    // beginning main init
    logger.logEvent("---- Beginning CEGUI System initialisation ----");

    // create the core system singleton objects
    createSingletons();

    // add the window factories for the core window types
    addStandardWindowFactories();

    String addressStr = SharedStringstream::GetPointerAddressAsString(this);
    logger.logEvent("CEGUI::System Singleton created. (" + addressStr + ")");
    logger.logEvent("---- CEGUI System initialisation completed ----");
    logger.logEvent("");

    // autoload resources specified in config
    config.loadAutoResources();

    // set up defaults
    config.initialiseDefaultFont();
    config.initialiseDefaultCursor();
    config.initialiseDefaulTooltip();

    // scripting available?
    if (d_scriptModule)
    {
        d_scriptModule->createBindings();
        config.executeInitScript();
        d_termScriptName = config.getTerminateScriptName();
    }
}


/*************************************************************************
    Destructor
*************************************************************************/
System::~System(void)
{
    Logger::getSingleton().logEvent("---- Beginning CEGUI System destruction ----");

    // execute shut-down script
    if (!d_termScriptName.empty())
    {
        try
        {
            executeScriptFile(d_termScriptName);
        }
        catch (...) {}  // catch all exceptions and continue system shutdown

    }

    if (d_nativeClipboardProvider != nullptr)
        delete d_nativeClipboardProvider;

    cleanupImageCodec();

    // cleanup XML stuff
    cleanupXMLParser();

    //
    // perform cleanup in correct sequence
    //
    // ensure no windows get created during destruction.  NB: I'm allowing the
    // potential exception to escape here so as to make it obvious that client
    // code should really be adjusted to not create windows during cleanup.
    WindowManager::getSingleton().lock();
    // destroy windows so it's safe to destroy factories
    for(auto* i : d_guiContexts)
    {
       i->destroyTooltips();
    }
    WindowManager::getSingleton().destroyAllWindows();
    WindowManager::getSingleton().cleanDeadPool();

    // remove factories so it's safe to unload GUI modules
    WindowFactoryManager::getSingleton().removeAllFactories();

    // Cleanup script module bindings
    if (d_scriptModule)
        d_scriptModule->destroyBindings();

    // cleanup singletons
    destroySingletons();

    // delete all the GUIContexts
    for (GUIContextCollection::iterator i = d_guiContexts.begin();
         i != d_guiContexts.end();
         ++i)
    {
        delete *i;
    }

    // cleanup resource provider if we own it
    if (d_ourResourceProvider)
        delete d_resourceProvider;

    String addressStr = SharedStringstream::GetPointerAddressAsString(this);
    Logger::getSingleton().logEvent("CEGUI::System singleton destroyed. " + addressStr);
    Logger::getSingleton().logEvent("---- CEGUI System destruction completed ----");

#ifdef CEGUI_USE_DEFAULT_LOGGER
    // delete the Logger object only if we created it.
    if (d_ourLogger)
        delete Logger::getSingletonPtr();
#endif

    delete d_clipboard;
}

//---------------------------------------------------------------------------//
unsigned int System::getMajorVersion()
{
    return CEGUI_VERSION_MAJOR;
}

//---------------------------------------------------------------------------//
unsigned int System::getMinorVersion()
{
    return CEGUI_VERSION_MINOR;
}

//---------------------------------------------------------------------------//
unsigned int System::getPatchVersion()
{
    return CEGUI_VERSION_PATCH;
}

//---------------------------------------------------------------------------//
const String& System::getVersion()
{
    static String ret(STRINGIZE(CEGUI_VERSION_MAJOR) "." STRINGIZE(CEGUI_VERSION_MINOR) "." STRINGIZE(CEGUI_VERSION_PATCH));
    return ret;
}

//---------------------------------------------------------------------------//
const String& System::getVerboseVersion()
{
    static String ret("");

    if (ret.empty())
    {
        ret = PropertyHelper<std::uint32_t>::toString(CEGUI_VERSION_MAJOR) + "." +
              PropertyHelper<std::uint32_t>::toString(CEGUI_VERSION_MINOR) + "." +
              PropertyHelper<std::uint32_t>::toString(CEGUI_VERSION_PATCH);

        ret += " (Build: " __DATE__;

#if defined(CEGUI_STATIC)
        ret += " Static";
#endif

#if defined(DEBUG) || defined(_DEBUG)
        ret += " Debug";
#endif

#if defined(__linux__)
        ret += " GNU/Linux";
#elif defined (__FreeBSD__)
        ret += " FreeBSD";
#elif defined (__APPLE__)
        ret += " Apple Mac";
#elif defined (_WIN32) || defined (__WIN32__)
        ret += " Microsoft Windows";
#endif

#ifdef __GNUG__
        ret += " g++ " __VERSION__;

#ifdef _LP64
        ret += " 64 bit";
#else
        ret += " 32 bit";
#endif

#elif defined(_MSC_VER)
        ret += " MSVC++ ";
#if _MSC_VER < 1600
        ret += "(Note: Compiler version is old and not officially supported)";
#elif _MSC_VER == 1600
        ret += "10.0 (2010)";
#elif _MSC_VER == 1700
        ret += "11.0 (2012)";
#elif _MSC_VER == 1800
        ret += "12.0 (2013)";
#elif _MSC_VER == 1900
        ret += "14.0 (2015)";
#elif _MSC_VER > 1900
        ret += "Unknown MSVC++ version";
#endif

#ifdef _WIN64
        ret += " 64 bit";
#else
        ret += " 32 bit";
#endif

#endif

        ret += ")";
    }

    return ret;
}

//----------------------------------------------------------------------------//
void System::renderAllGUIContexts()
{
    d_renderer->beginRendering();

    for (GUIContextCollection::iterator i = d_guiContexts.begin();
         i != d_guiContexts.end();
         ++i)
    {
        (*i)->draw();
    }

    d_renderer->endRendering();

    // do final destruction on dead-pool windows
    WindowManager::getSingleton().cleanDeadPool();
}

void System::renderAllGUIContextsOnTarget(Renderer* /*contained_in*/)
{
    d_renderer->beginRendering();

    for (GUIContextCollection::iterator i = d_guiContexts.begin();
        i != d_guiContexts.end();
        ++i)
    {
        // Add a check here to see if the renderer of context matches
        // contained_in
        (*i)->draw();
    }

    d_renderer->endRendering();

    // do final destruction on dead-pool windows
    WindowManager::getSingleton().cleanDeadPool();
}

/*************************************************************************
    Return a pointer to the ScriptModule being used for scripting within
    the GUI system.
*************************************************************************/
ScriptModule* System::getScriptingModule(void) const
{
    return d_scriptModule;
}

/*************************************************************************
    Set the scripting module
*************************************************************************/
void System::setScriptingModule(ScriptModule* scriptModule)
{
    // cleanup old bindings
    if (d_scriptModule)
        d_scriptModule->destroyBindings();
    // set new scriptModule
    d_scriptModule = scriptModule;

    if (d_scriptModule)
    {
        // log the new ScriptModule ID string
        Logger::getSingleton().logEvent("---- Scripting module is now: "
            + d_scriptModule->getIdentifierString() + " ----");

        // create bindings on the new scriptModule
        d_scriptModule->createBindings();
    }
}

/*************************************************************************
    Return a pointer to the ResourceProvider being used for within the GUI
    system.
*************************************************************************/
ResourceProvider* System::getResourceProvider(void) const
{
    return d_resourceProvider;
}

/*************************************************************************
    Execute a script file if possible.
*************************************************************************/
void System::executeScriptFile(const String& filename, const String& resourceGroup) const
{
    if (d_scriptModule)
    {
        try
        {
            d_scriptModule->executeScriptFile(filename, resourceGroup);
        }
        // Forward script exceptions with line number and file info
        catch (ScriptException&)
        {
            throw;
        }
        catch (...)
        {
            throw GenericException(
                "An exception was thrown during the execution of the script file.");
        }

    }
    else
    {
        Logger::getSingleton().logEvent("System::executeScriptFile - the script named '" + filename +"' could not be executed as no ScriptModule is available.", LoggingLevel::Error);
    }

}


/*************************************************************************
    Execute a scripted global function if possible.  The function should
    not take any parameters and should return an integer.
*************************************************************************/
int    System::executeScriptGlobal(const String& function_name) const
{
    if (d_scriptModule)
    {
        try
        {
            return d_scriptModule->executeScriptGlobal(function_name);
        }
        // Forward script exceptions with line number and file info
        catch (ScriptException&)
        {
            throw;
        }
        catch (...)
        {
            throw GenericException(
                "An exception was thrown during execution of the scripted function.");
        }

    }
    else
    {
        Logger::getSingleton().logEvent("System::executeScriptGlobal - the global script function named '" + function_name +"' could not be executed as no ScriptModule is available.", LoggingLevel::Error);
    }

    return 0;
}


/*************************************************************************
    If possible, execute script code contained in the given
    CEGUI::String object.
*************************************************************************/
void System::executeScriptString(const String& str) const
{
    if (d_scriptModule)
    {
        try
        {
            d_scriptModule->executeString(str);
        }
        // Forward script exceptions with line number and file info
        catch (ScriptException&)
        {
            throw;
        }
        catch (...)
        {
            throw GenericException(
                "An exception was thrown during execution of the script code.");
        }

    }
    else
    {
        Logger::getSingleton().logEvent("System::executeScriptString - the script code could not be executed as no ScriptModule is available.", LoggingLevel::Error);
    }
}

/*************************************************************************
    Method to inject time pulses into the system.
*************************************************************************/
bool System::injectTimePulse(float timeElapsed)
{
    AnimationManager::getSingleton().autoStepInstances(timeElapsed);
    return true;
}

System&    System::getSingleton(void)
{
    return Singleton<System>::getSingleton();
}


System*    System::getSingletonPtr(void)
{
    return Singleton<System>::getSingletonPtr();
}

/*************************************************************************
    Handler method for display size change notifications
*************************************************************************/
void System::notifyDisplaySizeChanged(const Sizef& new_size)
{
    // notify other components of the display size change
    ImageManager::getSingleton().notifyDisplaySizeChanged(new_size);
    FontManager::getSingleton().notifyDisplaySizeChanged(new_size);
    d_renderer->setDisplaySize(new_size);

    invalidateAllWindows();

    // Fire event
    DisplayEventArgs args(new_size);
    fireEvent(EventDisplaySizeChanged, args, EventNamespace);

    Logger::getSingleton().logEvent(
        "Display resize:"
        " w=" + PropertyHelper<float>::toString(new_size.d_width) +
        " h=" + PropertyHelper<float>::toString(new_size.d_height));
}

//----------------------------------------------------------------------------//

void System::outputLogHeader()
{
    Logger& l(Logger::getSingleton());
    l.logEvent("");
    l.logEvent("********************************************************************************");
    l.logEvent("* Important:                                                                   *");
    l.logEvent("*     To get support at the CEGUI forums, you must post _at least_ the section *");
    l.logEvent("*     of this log file indicated below.  Failure to do this will result in no  *");
    l.logEvent("*     support being given; please do not waste our time.                       *");
    l.logEvent("********************************************************************************");
    l.logEvent("********************************************************************************");
    l.logEvent("* -------- START OF ESSENTIAL SECTION TO BE POSTED ON THE FORUM       -------- *");
    l.logEvent("********************************************************************************");
    l.logEvent("---- Version: " + getVerboseVersion() + " ----");
    l.logEvent("---- Renderer module is: " + d_renderer->getIdentifierString() + " ----");
    l.logEvent("---- XML Parser module is: " + d_xmlParser->getIdentifierString() + " ----");
    l.logEvent("---- Image Codec module is: " + d_imageCodec->getIdentifierString() + " ----");
    l.logEvent(d_scriptModule ? "---- Scripting module is: " + d_scriptModule->getIdentifierString() + " ----" : "---- Scripting module is: None ----");
    l.logEvent("********************************************************************************");
    l.logEvent("* -------- END OF ESSENTIAL SECTION TO BE POSTED ON THE FORUM         -------- *");
    l.logEvent("********************************************************************************");
    l.logEvent("");
}

void System::addStandardWindowFactories()
{
    // Add types all base elements
    WindowFactoryManager::addWindowType<DefaultWindow>();
    WindowFactoryManager::addWindowType<DragContainer>();
    WindowFactoryManager::addWindowType<ScrolledContainer>();
    WindowFactoryManager::addWindowType<PushButton>();
    WindowFactoryManager::addWindowType<RadioButton>();
    WindowFactoryManager::addWindowType<Combobox>();
    WindowFactoryManager::addWindowType<ComboDropList>();
    WindowFactoryManager::addWindowType<Editbox>();
    WindowFactoryManager::addWindowType<FrameWindow>();
    WindowFactoryManager::addWindowType<ItemEntry>();
    WindowFactoryManager::addWindowType<ListHeader>();
    WindowFactoryManager::addWindowType<ListHeaderSegment>();
    WindowFactoryManager::addWindowType<ListWidget>();
    WindowFactoryManager::addWindowType<Menubar>();
    WindowFactoryManager::addWindowType<PopupMenu>();
    WindowFactoryManager::addWindowType<MenuItem>();
    WindowFactoryManager::addWindowType<MultiColumnList>();
    WindowFactoryManager::addWindowType<MultiLineEditbox>();
    WindowFactoryManager::addWindowType<ProgressBar>();
    WindowFactoryManager::addWindowType<ScrollablePane>();
    WindowFactoryManager::addWindowType<Scrollbar>();
    WindowFactoryManager::addWindowType<Slider>();
    WindowFactoryManager::addWindowType<Spinner>();
    WindowFactoryManager::addWindowType<TabButton>();
    WindowFactoryManager::addWindowType<TabControl>();
    WindowFactoryManager::addWindowType<Thumb>();
    WindowFactoryManager::addWindowType<Titlebar>();
    WindowFactoryManager::addWindowType<ToggleButton>();
    WindowFactoryManager::addWindowType<Tooltip>();
    WindowFactoryManager::addWindowType<TreeWidget>();
    WindowFactoryManager::addWindowType<HorizontalLayoutContainer>();
    WindowFactoryManager::addWindowType<VerticalLayoutContainer>();
    WindowFactoryManager::addWindowType<GridLayoutContainer>();

    // views
    WindowFactoryManager::addWindowType<ListView>();
    WindowFactoryManager::addWindowType<TreeView>();
}

void System::createSingletons()
{
    // cause creation of other singleton objects
    new ImageManager();
    new FontManager();
    new WindowFactoryManager();
    new WindowManager();
    new SchemeManager();
    new GlobalEventSet();
    new AnimationManager();
    new WidgetLookManager();
    new WindowRendererManager();
    new RenderEffectManager();
    new SVGDataManager();
}

void System::destroySingletons()
{
    delete SchemeManager::getSingletonPtr();
    delete WindowManager::getSingletonPtr();
    delete WindowFactoryManager::getSingletonPtr();
    delete WidgetLookManager::getSingletonPtr();
    delete WindowRendererManager::getSingletonPtr();
    delete AnimationManager::getSingletonPtr();
    delete RenderEffectManager::getSingletonPtr();
    delete FontManager::getSingletonPtr();
    delete ImageManager::getSingletonPtr();
    delete GlobalEventSet::getSingletonPtr();
    delete SVGDataManager::getSingletonPtr();
}

//----------------------------------------------------------------------------//
void System::setupXMLParser()
{
    // handle creation / initialisation of XMLParser
    if (!d_xmlParser)
    {
#ifndef CEGUI_STATIC
        setXMLParser(d_defaultXMLParserName);
#else
        //Static Linking Call
        d_xmlParser = createParser();
        // make sure we know to cleanup afterwards.
        d_ourXmlParser = true;
        d_xmlParser->initialise();
#endif
    }
    // parser object already set, just initialise it.
    else
        d_xmlParser->initialise();
}

//----------------------------------------------------------------------------//
void System::cleanupXMLParser()
{
    // bail out if no parser
    if (!d_xmlParser)
        return;

    // get parser object to do whatever cleanup it needs to
    d_xmlParser->cleanup();

    // exit if we did not create this parser object
    if (!d_ourXmlParser)
        return;

    // if parser module loaded, destroy the parser object & cleanup module
    if (d_parserModule)
    {
        // get pointer to parser deletion function
        void(*deleteFunc)(XMLParser*) = reinterpret_cast<void(*)(XMLParser*)>(d_parserModule->
            getSymbolAddress("destroyParser"));
        // cleanup the xml parser object
        deleteFunc(d_xmlParser);

        // delete the dynamic module for the xml parser
        delete d_parserModule;
        d_parserModule = nullptr;
    }
#ifdef CEGUI_STATIC
    else
        //Static Linking Call
        destroyParser(d_xmlParser);
#endif

    d_xmlParser = nullptr;
}

//----------------------------------------------------------------------------//
void System::setXMLParser(const String& parserName)
{
#ifndef CEGUI_STATIC
    cleanupXMLParser();
    // load dynamic module
    d_parserModule = new DynamicModule(String("CEGUI") + parserName);
    // get pointer to parser creation function
    XMLParser* (*createFunc)(void) =
        reinterpret_cast<XMLParser* (*)(void)>(d_parserModule->getSymbolAddress("createParser"));
    // create the parser object
    d_xmlParser = createFunc();
    // make sure we know to cleanup afterwards.
    d_ourXmlParser = true;
    // perform initialisation of XML parser.
    d_xmlParser->initialise();
#else
    CEGUI_UNUSED(parserName);
    Logger::getSingleton().logEvent(
        "System::setXMLParser(const String& parserName) called from statically "
        "linked CEGUI library - unable to load dynamic module!", LoggingLevel::Error);
#endif
}

//----------------------------------------------------------------------------//
void System::setXMLParser(XMLParser* parser)
{
    cleanupXMLParser();
    d_xmlParser = parser;
    d_ourXmlParser = false;
    setupXMLParser();
}

//----------------------------------------------------------------------------//
void System::setDefaultXMLParserName(const String& parserName)
{
    d_defaultXMLParserName = parserName;
}

String System::getDefaultXMLParserName()
{
    return d_defaultXMLParserName;
}

//----------------------------------------------------------------------------//
ImageCodec& System::getImageCodec() const
{
    return *d_imageCodec;
}

//----------------------------------------------------------------------------//
void System::setImageCodec(const String& codecName)
{
    setupImageCodec(codecName);
}

//----------------------------------------------------------------------------//
void System::setImageCodec(ImageCodec& codec)
{
    cleanupImageCodec();
    d_imageCodec = &codec;
    d_ourImageCodec = false;
    d_imageCodecModule = nullptr;
}

//----------------------------------------------------------------------------//
void System::setupImageCodec(const String& codecName)
{
    // Cleanup the old image codec
    cleanupImageCodec();

#    if defined(CEGUI_STATIC)
        // for static build use static createImageCodec to create codec object
        d_imageCodec = createImageCodec();
        CEGUI_UNUSED(codecName);
#    else
        // load the appropriate image codec module
        d_imageCodecModule = codecName.empty() ?
            new DynamicModule(String("CEGUI") + d_defaultImageCodecName) :
            new DynamicModule(String("CEGUI") + codecName);

        // use function from module to create the codec object.
        d_imageCodec = reinterpret_cast<ImageCodec*(*)(void)>(d_imageCodecModule->
            getSymbolAddress("createImageCodec"))();
#    endif

    // make sure we mark this as our own object so we can clean it up later.
    d_ourImageCodec = true;
}

//----------------------------------------------------------------------------//
void System::cleanupImageCodec()
{
    // bail out if no codec, or if we did not create it.
    if (!d_imageCodec || !d_ourImageCodec)
        return;

    if (d_imageCodecModule)
    {
        reinterpret_cast<void(*)(ImageCodec*)>(d_imageCodecModule->
            getSymbolAddress("destroyImageCodec"))(d_imageCodec);

        delete d_imageCodecModule;
        d_imageCodecModule = nullptr;
    }
#if defined(CEGUI_STATIC)
    else
        destroyImageCodec(d_imageCodec);
#endif

    d_imageCodec = nullptr;
}

//----------------------------------------------------------------------------//
void System::setDefaultImageCodecName(const String& codecName)
{
    d_defaultImageCodecName = codecName;
}

//----------------------------------------------------------------------------//
const String& System::getDefaultImageCodecName()
{
    return d_defaultImageCodecName;
}

//----------------------------------------------------------------------------//
System& System::create(Renderer& renderer, ResourceProvider* resourceProvider,
                       XMLParser* xmlParser, ImageCodec* imageCodec,
                       ScriptModule* scriptModule, const String& configFile,
                       const String& logFile, const int abi)
{
    performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new System(renderer, resourceProvider, xmlParser, imageCodec,
                       scriptModule, configFile, logFile);
}

//----------------------------------------------------------------------------//
void System::performVersionTest(const int expected, const int received,
                                const String& func)
{
    if (expected != received)
        throw InvalidRequestException("Version mismatch detected! "
            "Called from function: " + func +
            " Expected abi: " + PropertyHelper<std::int32_t>::toString(expected) +
            " received abi: " + PropertyHelper<std::int32_t>::toString(received) +
            ". This means that the code calling the function was compiled "
            "against a CEGUI version that is incompatible with the library "
            "containing the function. Usually this means that you have "
            "old binary library versions that have been used by mistake.");
}

//----------------------------------------------------------------------------//
void System::destroy()
{
    delete System::getSingletonPtr();
}

//----------------------------------------------------------------------------//
TextParser* System::getDefaultTextParser() const
{
    return d_defaultTextParser;
}

//----------------------------------------------------------------------------//
void System::setDefaultTextParser(TextParser* parser)
{
    if (parser == d_defaultTextParser)
        return;

    d_defaultTextParser = parser ? parser : d_fallbackTextParser.get();

    EventArgs args;
    fireEvent(EventTextParserChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void System::invalidateAllCachedRendering()
{
    invalidateAllWindows();
}

//----------------------------------------------------------------------------//
void System::invalidateAllWindows()
{
    WindowManager::WindowIterator wi(
        WindowManager::getSingleton().getIterator());

    for ( ; !wi.isAtEnd(); ++wi)
    {
        Window* const wnd(wi.getCurrentValue());
        // invalidate window itself
        wnd->invalidate();
        // if window has rendering window surface, invalidate it's geometry
        RenderingSurface* rs = wnd->getRenderingSurface();
        if (rs != nullptr && rs->isRenderingWindow())
            static_cast<RenderingWindow*>(rs)->invalidateGeometry();
    }
}

//----------------------------------------------------------------------------//
void System::setDefaultFontName(const String& name)
{
    d_defaultFontName = name;
}

//----------------------------------------------------------------------------//
void System::setDefaultCursorName(const String& name)
{
    d_defaultCursorName = name;
}

//----------------------------------------------------------------------------//
void System::setDefaultTooltipType(const String& tooltip_type)
{
    d_defaultTooltipType = tooltip_type;
}

//----------------------------------------------------------------------------//
GUIContext& System::createGUIContext(RenderTarget& rt)
{
    GUIContext* c = new GUIContext(rt);
    d_guiContexts.push_back(c);

    if (!d_defaultFontName.empty())
        c->setDefaultFont(d_defaultFontName);
    if (!d_defaultCursorName.empty())
        c->setDefaultCursorImage(&ImageManager::getSingleton().get(d_defaultCursorName));
    if (!d_defaultTooltipType.empty())
        c->setDefaultTooltipType(d_defaultTooltipType);

    return *c;
}

//----------------------------------------------------------------------------//
void System::destroyGUIContext(GUIContext& context)
{
    for (GUIContextCollection::iterator i = d_guiContexts.begin();
         i != d_guiContexts.end();
         ++i)
    {
        if (*i == &context)
        {
            delete *i;
            d_guiContexts.erase(i);
            return;
        }
    }
}

//----------------------------------------------------------------------------//
const StringTranscoder& System::getStringTranscoder()
{
    return d_stringTranscoder;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

