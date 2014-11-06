/***********************************************************************
created:    Sep 11 2014
author:     Luca Ebach <lucaebach@gmail.com>
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
* CEGUIBase
* CEGUIOpenGLRenderer
* CEGUICoreWindowRendererSet
* default CEGUI xml parser (and dependencies)
* SDL2main
* SDL2
* OpengGL
* glm headers (as part of CEGUIBase)
***************************************************************************/

#include <iostream>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <SDL.h>
#include <SDL_opengl.h>

static SDL_Window* window;
static SDL_GLContext context;

CEGUI::Key::Scan toCEGUIKey(SDL_Scancode key)
{
    switch (key)
    {
    case SDL_SCANCODE_ESCAPE: return CEGUI::Key::Escape;
    case SDL_SCANCODE_F1: return CEGUI::Key::F1;
    case SDL_SCANCODE_F2: return CEGUI::Key::F2;
    case SDL_SCANCODE_F3: return CEGUI::Key::F3;
    case SDL_SCANCODE_F4: return CEGUI::Key::F4;
    case SDL_SCANCODE_F5: return CEGUI::Key::F5;
    case SDL_SCANCODE_F6: return CEGUI::Key::F6;
    case SDL_SCANCODE_F7: return CEGUI::Key::F7;
    case SDL_SCANCODE_F8: return CEGUI::Key::F8;
    case SDL_SCANCODE_F9: return CEGUI::Key::F9;
    case SDL_SCANCODE_F10: return CEGUI::Key::F10;
    case SDL_SCANCODE_F11: return CEGUI::Key::F11;
    case SDL_SCANCODE_F12: return CEGUI::Key::F12;
    case SDL_SCANCODE_F13: return CEGUI::Key::F13;
    case SDL_SCANCODE_F14: return CEGUI::Key::F14;
    case SDL_SCANCODE_F15: return CEGUI::Key::F15;
    case SDL_SCANCODE_UP: return CEGUI::Key::ArrowUp;
    case SDL_SCANCODE_DOWN: return CEGUI::Key::ArrowDown;
    case SDL_SCANCODE_LEFT: return CEGUI::Key::ArrowLeft;
    case SDL_SCANCODE_RIGHT: return CEGUI::Key::ArrowRight;
    case SDL_SCANCODE_LSHIFT: return CEGUI::Key::LeftShift;
    case SDL_SCANCODE_RSHIFT: return CEGUI::Key::RightShift;
    case SDL_SCANCODE_LCTRL: return CEGUI::Key::LeftControl;
    case SDL_SCANCODE_RCTRL: return CEGUI::Key::RightControl;
    case SDL_SCANCODE_LALT: return CEGUI::Key::LeftAlt;
    case SDL_SCANCODE_RALT: return CEGUI::Key::RightAlt;
    case SDL_SCANCODE_TAB: return CEGUI::Key::Tab;
    case SDL_SCANCODE_RETURN: return CEGUI::Key::Return;
    case SDL_SCANCODE_BACKSPACE: return CEGUI::Key::Backspace;
    case SDL_SCANCODE_INSERT: return CEGUI::Key::Insert;
    case SDL_SCANCODE_DELETE: return CEGUI::Key::Delete;
    case SDL_SCANCODE_PAGEUP: return CEGUI::Key::PageUp;
    case SDL_SCANCODE_PAGEDOWN: return CEGUI::Key::PageDown;
    case SDL_SCANCODE_HOME: return CEGUI::Key::Home;
    case SDL_SCANCODE_END: return CEGUI::Key::End;
    case SDL_SCANCODE_KP_ENTER: return CEGUI::Key::NumpadEnter;
    case SDL_SCANCODE_SPACE: return CEGUI::Key::Space;
    case SDL_SCANCODE_A: return CEGUI::Key::A;
    case SDL_SCANCODE_B: return CEGUI::Key::B;
    case SDL_SCANCODE_C: return CEGUI::Key::C;
    case SDL_SCANCODE_D: return CEGUI::Key::D;
    case SDL_SCANCODE_E: return CEGUI::Key::E;
    case SDL_SCANCODE_F: return CEGUI::Key::F;
    case SDL_SCANCODE_G: return CEGUI::Key::G;
    case SDL_SCANCODE_H: return CEGUI::Key::H;
    case SDL_SCANCODE_I: return CEGUI::Key::I;
    case SDL_SCANCODE_J: return CEGUI::Key::J;
    case SDL_SCANCODE_K: return CEGUI::Key::K;
    case SDL_SCANCODE_L: return CEGUI::Key::L;
    case SDL_SCANCODE_M: return CEGUI::Key::M;
    case SDL_SCANCODE_N: return CEGUI::Key::N;
    case SDL_SCANCODE_O: return CEGUI::Key::O;
    case SDL_SCANCODE_P: return CEGUI::Key::P;
    case SDL_SCANCODE_Q: return CEGUI::Key::Q;
    case SDL_SCANCODE_R: return CEGUI::Key::R;
    case SDL_SCANCODE_S: return CEGUI::Key::S;
    case SDL_SCANCODE_T: return CEGUI::Key::T;
    case SDL_SCANCODE_U: return CEGUI::Key::U;
    case SDL_SCANCODE_V: return CEGUI::Key::V;
    case SDL_SCANCODE_W: return CEGUI::Key::W;
    case SDL_SCANCODE_X: return CEGUI::Key::X;
    case SDL_SCANCODE_Y: return CEGUI::Key::Y;
    case SDL_SCANCODE_Z: return CEGUI::Key::Z;
    case SDL_SCANCODE_1: return CEGUI::Key::One;
    case SDL_SCANCODE_2: return CEGUI::Key::Two;
    case SDL_SCANCODE_3: return CEGUI::Key::Three;
    case SDL_SCANCODE_4: return CEGUI::Key::Four;
    case SDL_SCANCODE_5: return CEGUI::Key::Five;
    case SDL_SCANCODE_6: return CEGUI::Key::Six;
    case SDL_SCANCODE_7: return CEGUI::Key::Seven;
    case SDL_SCANCODE_8: return CEGUI::Key::Eight;
    case SDL_SCANCODE_9: return CEGUI::Key::Nine;
    case SDL_SCANCODE_0: return CEGUI::Key::Zero;
    default: return CEGUI::Key::Unknown;
    }
}

void initSDL()
{
    // init everything from SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL could not be initialized!" << std::endl
            << "Error message: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // create opengl window with size of 800x600px
    window = SDL_CreateWindow("CEGUI + SDL2 window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "Could not create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    // disable native mouse cursor
    SDL_ShowCursor(0);

    // setup opengl rendering context
    context = SDL_GL_CreateContext(window);
}

void initCEGUI()
{
    using namespace CEGUI;

    // create renderer and enable extra states
    OpenGL3Renderer& cegui_renderer = OpenGL3Renderer::create(Sizef(800.f, 600.f));
    cegui_renderer.enableExtraStateSettings(true);

    // create CEGUI system object
    CEGUI::System::create(cegui_renderer);

    // setup resource directories
    DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", "datafiles/schemes/");
    rp->setResourceGroupDirectory("imagesets", "datafiles/imagesets/");
    rp->setResourceGroupDirectory("fonts", "datafiles/fonts/");
    rp->setResourceGroupDirectory("layouts", "datafiles/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "datafiles/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", "datafiles/lua_scripts/");
    rp->setResourceGroupDirectory("schemas", "datafiles/xml_schemas/");

    // set default resource groups
    ImageManager::setImagesetDefaultResourceGroup("imagesets");
    Font::setDefaultResourceGroup("fonts");
    Scheme::setDefaultResourceGroup("schemes");
    WidgetLookManager::setDefaultResourceGroup("looknfeels");
    WindowManager::setDefaultResourceGroup("layouts");
    ScriptModule::setDefaultResourceGroup("lua_scripts");

    XMLParser* parser = System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");

    // load TaharezLook scheme and DejaVuSans-10 font
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "schemes");
    FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

    // set default font and cursor image and tooltip type
    System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
    System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");
}

void initWindows()
{
    using namespace CEGUI;

    /////////////////////////////////////////////////////////////
    // Add your gui initialisation code in here.
    // You can use the following code as an inspiration for
    // creating your own windows.
    // But you should preferably use layout loading because you won't
    // have to recompile everytime you change the layout.
    /////////////////////////////////////////////////////////////

    // load layout
    Window* root = WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
    System::getSingleton().getDefaultGUIContext().setRootWindow(root);
}

// convert SDL mouse button to CEGUI mouse button
CEGUI::MouseButton SDLtoCEGUIMouseButton(const Uint8& button)
{
    using namespace CEGUI;

    switch (button)
    {
    case SDL_BUTTON_LEFT:
        return LeftButton;
        
    case SDL_BUTTON_MIDDLE:
        return MiddleButton;

    case SDL_BUTTON_RIGHT:
        return RightButton;

    default:
        return NoButton;
    }
}

int SDL_main(int argc, char* argv[])
{
    using namespace CEGUI;

    // init SDL
    initSDL();

    // init cegui
    initCEGUI();

    // notify system of the window size
    System::getSingleton().notifyDisplaySizeChanged(Sizef(800.f, 600.f));

    // initialise windows and setup layout
    initWindows();

    // set gl clear color
    glClearColor(0, 0, 0, 255);

    bool quit = false;
    SDL_Event event;
    float time = SDL_GetTicks() / 1000.f;

    OpenGL3Renderer* renderer = static_cast<OpenGL3Renderer*>(System::getSingleton().getRenderer());

    // repeat until a quit is requested
    while (!quit && !SDL_QuitRequested())
    {
        // query and process events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            
            case SDL_MOUSEMOTION:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(static_cast<float>(event.motion.x),
                                                                                         static_cast<float>(event.motion.y));
                break;

            case SDL_MOUSEBUTTONDOWN:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(SDLtoCEGUIMouseButton(event.button.button));
                break;

            case SDL_MOUSEBUTTONUP:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(SDLtoCEGUIMouseButton(event.button.button));
                break;

            case SDL_MOUSEWHEEL:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(static_cast<float>(event.wheel.y));
                break;

            case SDL_KEYDOWN:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(toCEGUIKey(event.key.keysym.scancode));
                CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(event.key.keysym.sym);
                break;

            case SDL_KEYUP:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(toCEGUIKey(event.key.keysym.scancode));
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    System::getSingleton().notifyDisplaySizeChanged(Sizef(static_cast<float>(event.window.data1),
                                                                          static_cast<float>(event.window.data2)));
                    glViewport(0, 0, event.window.data1, event.window.data2);
                }
                break;

            default:
                break;

            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // inject time pulses
        const float newtime = SDL_GetTicks() / 1000.f;
        const float time_elapsed = newtime - time;
        System::getSingleton().injectTimePulse(time_elapsed);
        System::getSingleton().getDefaultGUIContext().injectTimePulse(time_elapsed);
        time = newtime;

        // render gui
        renderer->beginRendering();
        System::getSingleton().renderAllGUIContexts();
        renderer->endRendering();

        // swap buffers
        SDL_GL_SwapWindow(window);
    }

    // destroy system and renderer
    System::destroy();
    OpenGL3Renderer::destroy(*renderer);
    renderer = 0;

    // delete SDL GL context
    SDL_GL_DeleteContext(context);

    // destroy SDL window
    SDL_DestroyWindow(window);

    // cleanup SDL
    SDL_Quit();

    return 0;
}