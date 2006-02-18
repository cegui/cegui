#include "CEGUIWindowRendererManager.h"
#include "CEGUILogger.h"
#include "CEGUIExceptions.h"

// Start CEGUI namespace
namespace CEGUI
{

/*************************************************************************
    Static data
*************************************************************************/
template<> WindowRendererManager* Singleton<WindowRendererManager>::ms_Singleton = 0;

/*************************************************************************
    Singleton functions
*************************************************************************/
WindowRendererManager& WindowRendererManager::getSingleton(void)
{
    return Singleton<WindowRendererManager>::getSingleton();
}
WindowRendererManager* WindowRendererManager::getSingletonPtr(void)
{
    return Singleton<WindowRendererManager>::getSingletonPtr();
}

/*************************************************************************
    Constructor / Destructor
*************************************************************************/
WindowRendererManager::WindowRendererManager()
{
    Logger::getSingleton().logEvent("CEGUI::WindowRendererManager singleton created");
}
WindowRendererManager::~WindowRendererManager()
{
    Logger::getSingleton().logEvent("CEGUI::WindowRendererManager singleton destroyed");
}

/*************************************************************************
    Is there a WindowRenderer by this name?
*************************************************************************/
bool WindowRendererManager::isFactoryPresent(const String& name) const
{
    return (d_wrReg.find(name) != d_wrReg.end());
}

/*************************************************************************
    Get the named WindowRenderer
*************************************************************************/
WindowRendererFactory* WindowRendererManager::getFactory(const String& name) const
{
    WR_Registry::const_iterator i = d_wrReg.find(name);
    if (i != d_wrReg.end())
    {
        return (*i).second;
    }
    throw UnknownObjectException("There is no WindowRendererFactory named '"+name+"' available");
}

/*************************************************************************
    Add a new WindowRenderer factory
*************************************************************************/
void WindowRendererManager::addFactory(WindowRendererFactory* wr)
{
    if (wr == 0)
    {
        return;
    }
    if (d_wrReg.insert(std::make_pair(wr->getName(), wr)).second == false)
    {
        throw AlreadyExistsException("A WindowRendererFactory named '"+wr->getName()+"' already exist");
    }
    Logger::getSingleton().logEvent("WindowRendererFactory '"+wr->getName()+"' added.");
}

/*************************************************************************
    Remove a factory by name
*************************************************************************/
void WindowRendererManager::removeFactory(const String& name)
{
    d_wrReg.erase(name);
}

/*************************************************************************
    Create a WindowRenderer instance by factory name
*************************************************************************/
WindowRenderer* WindowRendererManager::createWindowRenderer(const String& name)
{
    WindowRendererFactory* factory = getFactory(name);
    return factory->create();
}

/*************************************************************************
    Destroy a WindowRenderer using its factory
*************************************************************************/
void WindowRendererManager::destroyWindowRenderer(WindowRenderer* wr)
{
    WindowRendererFactory* factory = getFactory(wr->getName());
    factory->destroy(wr);
}

} // CEGUI
