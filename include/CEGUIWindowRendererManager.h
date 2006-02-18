#ifndef _CEGUIWindowRendererManager_h_
#define _CEGUIWindowRendererManager_h_

#include "CEGUIWindowRenderer.h"
#include "CEGUISingleton.h"
//#include "CEGUIIteratorBase.h"
#include <map>

// Start of CEGUI namespace section
namespace CEGUI
{

class CEGUIEXPORT WindowRendererManager : public Singleton<WindowRendererManager>
{
public:
    /*************************************************************************
        Contructor / Destructor
    *************************************************************************/
    WindowRendererManager();
    ~WindowRendererManager();

    /*************************************************************************
        Singleton
    *************************************************************************/
    static WindowRendererManager& getSingleton();
    static WindowRendererManager* getSingletonPtr();

    /*************************************************************************
        Accessors
    *************************************************************************/
    bool isFactoryPresent(const String& name) const;
    WindowRendererFactory* getFactory(const String& name) const;

    /*************************************************************************
        Manipulators
    *************************************************************************/
    void addFactory(WindowRendererFactory* wr);
    void removeFactory(const String& name);

    /*************************************************************************
        Factory usage
    *************************************************************************/
    WindowRenderer* createWindowRenderer(const String& name);
    void destroyWindowRenderer(WindowRenderer* wr);

private:
    /*************************************************************************
        Private implementation
    *************************************************************************/

    /*************************************************************************
        Implementation data
    *************************************************************************/
    typedef std::map<String, WindowRendererFactory*, String::FastLessCompare> WR_Registry;
    WR_Registry d_wrReg;
};

} // End of CEGUI namespace

#endif // _CEGUIWindowRendererManager_h_
