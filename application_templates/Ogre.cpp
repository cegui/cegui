/***********************************************************************
created:    Feb 27 2016
author:     Petskull
*************************************************************************/

/***************************************************************************
*   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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

/**************************************************************************
* The following libs (and corresponding headers) are needed to compile and to link:
* OgreMain
* OIS
* CEGUIBase-0
* CEGUIOgreRenderer-0
***************************************************************************/

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

// Ogre3D 1.10 cloned on 160226 https://bitbucket.org/sinbad/ogre
#include <Ogre.h>
#include <OIS.h>

// CEGUI 0.8.4 cloned on 160226 from https://bitbucket.org/cegui/cegui
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

typedef struct{	/* For moving data between App and AppListener */
	Ogre::Root* ogre_root;
	Ogre::RenderWindow *ogre_win;
	OIS::ParamList *ois_pl;
	CEGUI::OgreRenderer *cegui_renderer;
} AppPak;

/* This AppListener class will house all callback from Ogre's rendering and window events, as well as OIS's Keyboard and Mouse events. */
class AppListener : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener{
public:
	AppListener(AppPak *new_app_pak) :keep_running(true){
		app_pak = new_app_pak;

		ois_input_man = OIS::InputManager::createInputSystem(*app_pak->ois_pl);
		ois_kb = static_cast<OIS::Keyboard*>(ois_input_man->createInputObject(OIS::OISKeyboard, true));
		ois_mouse = static_cast<OIS::Mouse*>(ois_input_man->createInputObject(OIS::OISMouse, true));
		ois_kb->setEventCallback(this);
		ois_mouse->setEventCallback(this);

		windowResized(app_pak->ogre_win);

		MAX_BUTTON_NUM = 0;
	}
	~AppListener(){
		OIS::InputManager::destroyInputSystem(ois_input_man);
	}

	// --- Ogre::FrameListener Callbacks ---------------------------//
	bool frameRenderingQueued(const Ogre::FrameEvent& evt){
		Ogre::WindowEventUtilities::messagePump();

		// For some reason, injectTimePulse(delta_time) currently must be done both
		// on the GUIContext and also on CEGUI::System itself everytime.
		CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse((float)evt.timeSinceLastFrame);
		CEGUI::System::getSingleton().injectTimePulse((float)evt.timeSinceLastFrame);

		// If our OIS::Keyboard or our OIS::Mouse has been destroyed (say, by closing the window), end the rendering
		if (ois_kb && ois_mouse){
			ois_kb->capture();
			ois_mouse->capture();
		} else {
			return false;
		}

		//CEGUI::System::getSingleton().renderAllGUIContexts();	// By bootstrapping Ogre directly, we don't have to call this.  Ogre does it automatically.

		return keep_running;	// Return false ends Ogre's rendering
	}

	// --- OIS::KeyListener and OIS::MouseListener Callbacks -------//
	bool keyPressed(const OIS::KeyEvent &arg){
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)arg.key);
		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar((CEGUI::Key::Scan)arg.text);

		if (arg.key == OIS::KC_ESCAPE){ keep_running = false; }
		return true;
	}
	bool keyReleased(const OIS::KeyEvent &arg){
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
		return true;
	}
	bool mouseMoved(const OIS::MouseEvent &arg){
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove((float)arg.state.X.rel, (float)arg.state.Y.rel);
		//CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(200.f, 200.f);	// Another way to control the mouse cursor
		return true;
	}
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
		return true;
	}
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
		return true;
	}

	// --- Ogre::WindowEventListener Callbacks ---------------------//
	void windowResized(Ogre::RenderWindow* rw){
		unsigned int width, height, depth;
		int left, top;
		app_pak->ogre_win->getMetrics(width, height, depth, left, top);

		const OIS::MouseState &ms = ois_mouse->getMouseState();
		ms.width = width;
		ms.height = height;

		CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef((float)width, (float)height));
	}
	void windowClosed(Ogre::RenderWindow* rw){
		if (rw == app_pak->ogre_win){
			if (ois_input_man){
				ois_input_man->destroyInputObject(ois_mouse);
				ois_input_man->destroyInputObject(ois_kb);
				ois_mouse = NULL;
				ois_kb = NULL;

				OIS::InputManager::destroyInputSystem(ois_input_man);
				ois_input_man = 0;
			}
		}
	}

	// Convert OIS MouseButton event to CEGUI MouseButton event
	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) {
		switch (buttonID) {
		case OIS::MB_Left:
			return CEGUI::LeftButton;
		case OIS::MB_Right:
			return CEGUI::RightButton;
		case OIS::MB_Middle:
			return CEGUI::MiddleButton;
		default:
			return CEGUI::LeftButton;
		}
	}

private:
	AppPak *app_pak;
	bool keep_running;

	OIS::InputManager *ois_input_man;
	OIS::Mouse *ois_mouse;
	OIS::Keyboard *ois_kb;

	unsigned int MAX_BUTTON_NUM;
};

class App{
public:
	void go(){	/* Entry Point Called by main() */
		Setup();
		CreateGUIContext();
		CreateGUIRootWindow();
		app_pak.ogre_root->startRendering();
		DestroyGUIRootWindow();
		Shutdown();
	}
private:
	void Setup(){	/* Here we set up the basic libs of Ogre, OIS, and CEGUI */
		// --- Init Ogre --- //
		app_pak.ogre_root = new Ogre::Root("", "", "Ogre.log");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media", "FileSystem", "General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/CEGUI/imagesets/", "FileSystem", "imagesets");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/CEGUI/schemes/", "FileSystem", "schemes");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/CEGUI/fonts/", "FileSystem", "fonts");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/CEGUI/looknfeel/", "FileSystem", "looknfeel");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/CEGUI/layouts/", "FileSystem", "layouts");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/CEGUI/lua_scripts/", "FileSystem", "lua_scripts");

#ifdef _DEBUG
		app_pak.ogre_root->loadPlugin(Ogre::String("RenderSystem_GL_d"));
		app_pak.ogre_root->loadPlugin(Ogre::String("Plugin_OctreeSceneManager_d"));
#else
		app_pak.ogre_root->loadPlugin("RenderSystem_GL");
		app_pak.ogre_root->loadPlugin("Plugin_OctreeSceneManager");
#endif;

		Ogre::RenderSystem* rs = app_pak.ogre_root->getRenderSystemByName("OpenGL Rendering Subsystem");
		if (!(rs->getName() == "OpenGL Rendering Subsystem")){ return; }
		rs->setConfigOption("Full Screen", "No");
		rs->setConfigOption("VSync", "No");
		rs->setConfigOption("Video Mode", "1024 x 768 @ 32-bit");
		app_pak.ogre_root->setRenderSystem(rs);

		app_pak.ogre_win = app_pak.ogre_root->initialise(true, "Clean CEGUI Window");

		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		//---------[ CEGUI works fine without these, but the black background does not redraw. ]---------//
		// --- Ogre Scene Manager --- //
		Ogre::SceneManager *scene_man = app_pak.ogre_root->createSceneManager("OctreeSceneManager", "DefaultSceneManager");

		// --- Camera --- //
		Ogre::Camera *main_cam = scene_man->createCamera("MainCam");
		main_cam->setPosition(Ogre::Vector3(40, 20, 80));
		main_cam->lookAt(Ogre::Vector3(0, 0, 0));
		main_cam->setNearClipDistance(5);

		// --- Viewport --- //
		Ogre::Viewport* vp = app_pak.ogre_win->addViewport(main_cam);
		vp->setBackgroundColour(Ogre::ColourValue(17.f / 255.f, 17.f / 255.f, 17.f / 255.f));
		main_cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
		//----------------------------------------------[|]----------------------------------------------//

		// --- Init OIS --- //
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
		app_pak.ogre_win->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		app_pak.ois_pl = new OIS::ParamList();
		app_pak.ois_pl->insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		// --- Init CEGUI --- //
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing CEGUI ***");
		app_pak.cegui_renderer = &CEGUI::OgreRenderer::bootstrapSystem();
		CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(1024.f, 768.f));
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

		// setup CEGUI default resource groups
		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

		// --- Init Ogre/OIS Listeners --- //
		app_listener = new AppListener(&app_pak);
		app_pak.ogre_root->addFrameListener(app_listener);
		Ogre::WindowEventUtilities::addWindowEventListener(app_pak.ogre_win, app_listener);

	}
	void Shutdown(){
		// --- Shutdown Ogre, OIS, and CEGUI --- //
		app_pak.ogre_root->getAutoCreatedWindow()->removeAllViewports();
		Ogre::WindowEventUtilities::removeWindowEventListener(app_pak.ogre_win, app_listener);
		app_listener->windowClosed(app_pak.ogre_win);
		app_pak.ogre_root->removeFrameListener( app_listener );
		delete app_listener;

		delete app_pak.ogre_root;
		delete app_pak.ois_pl;
	}
	void CreateGUIContext(){
		// --- Create resources we need --- //
		CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
		CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-12.font");

		// --- Create our GUIContext and set its defaults --- //
		CEGUI::System::getSingleton().createGUIContext(app_pak.cegui_renderer->getDefaultRenderTarget());
		CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-12");
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");
		//CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();		// Only important to bring cursor back from hide()
	}
	void CreateGUIRootWindow(){
		ui_root_win = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
		CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(ui_root_win);
	}
	void DestroyGUIRootWindow(){
		CEGUI::WindowManager::getSingleton().destroyWindow(ui_root_win);
	}

	AppPak app_pak;
	AppListener *app_listener;

	CEGUI::Window *ui_root_win;
};

//-------------------------------------------------------------------[ Init ]---//
std::wstring s2ws(const std::string& s){
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT){
#else
int main(int argc, char *argv[]){
#endif
	App my_app;

	try {
		my_app.go();
	}
	catch (Ogre::Exception& e){
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		std::wstring stemp1 = s2ws("Stoopid Ogre exception has occurred!");
		std::wstring stemp2 = s2ws(e.getFullDescription().c_str());

		LPCWSTR title = stemp1.c_str();
		LPCWSTR error_desc = stemp2.c_str();
		MessageBox(NULL, error_desc, title, MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "Stoopid Ogre exception has occurred: " << e.getFullDescription().c_str() << std::endl;
#endif
	}

	return 0;
}