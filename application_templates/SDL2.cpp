/***********************************************************************
created:    Sep 11 2014
author:     Luca Ebach <bitbucket@lucebac.net>
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
* CEGUIBase
* CEGUIOpenGLRenderer
* CEGUICoreWindowRendererSet
* default CEGUI xml parser (and dependencies)
* SDL2main (windows only)
* SDL2
* OpengGL
* glm headers (as part of CEGUIBase)
***************************************************************************/

#include <iostream>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "sdl_scancode_to_dinput_mappings.h"

static SDL_Window* window;
static SDL_GLContext context;

CEGUI::Key::Scan toCEGUIKey(SDL_Scancode key)
{
    return static_cast<CEGUI::Key::Scan>(scanCodeToKeyNum[static_cast<int>(key)]);
}

void injectUTF8Text(const char* utf8str)
{
    static SDL_iconv_t cd = SDL_iconv_t(-1);

    if (cd == SDL_iconv_t(-1))
    {
        // note: just "UTF-32" doesn't work as toFormat, because then you get BOMs, which we don't want.
        const char* toFormat = "UTF-32LE"; // TODO: what does CEGUI expect on big endian machines?
        cd = SDL_iconv_open(toFormat, "UTF-8");
        if (cd == SDL_iconv_t(-1))
        {
            std::cerr << "Couldn't initialize SDL_iconv for UTF-8 to UTF-32!" << std::endl;
            return;
        }
    }

    // utf8str has at most SDL_TEXTINPUTEVENT_TEXT_SIZE (32) chars,
    // so we won't have have more utf32 chars than that
    Uint32 utf32buf[SDL_TEXTINPUTEVENT_TEXT_SIZE] = {0};

    // we'll convert utf8str to a utf32 string, saved in utf32buf.
    // the utf32 chars will be injected into cegui

    size_t len = strlen(utf8str);

    size_t inbytesleft = len;
    size_t outbytesleft = 4 * SDL_TEXTINPUTEVENT_TEXT_SIZE; // *4 because utf-32 needs 4x as much space as utf-8
    char* outbuf = (char*)utf32buf;
    size_t n = SDL_iconv(cd, &utf8str, &inbytesleft, &outbuf, &outbytesleft);

    if (n == size_t(-1)) // some error occured during iconv
    {
        std::cerr << "Converting UTF-8 string " << utf8str << " from SDL_TEXTINPUT to UTF-32 failed!" << std::endl;
    }

    for (int i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i)
    {
        if (utf32buf[i] == 0)
            break; // end of string

        CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(utf32buf[i]);
    }

    // reset cd so it can be used again
    SDL_iconv(cd, NULL, &inbytesleft, NULL, &outbytesleft);
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
    OpenGLRenderer& cegui_renderer = OpenGLRenderer::create(Sizef(800.f, 600.f));
    cegui_renderer.enableExtraStateSettings(true);

    // create CEGUI system object
    System::create(cegui_renderer);

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

    case SDL_BUTTON_X1:
        return X1Button;

    case SDL_BUTTON_X2:
        return X2Button;

    default:
        return NoButton;
    }
}

int main(int argc, char* argv[])
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

    OpenGLRenderer* renderer = static_cast<OpenGLRenderer*>(System::getSingleton().getRenderer());

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
                break;

            case SDL_KEYUP:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(toCEGUIKey(event.key.keysym.scancode));
                break;

            case SDL_TEXTINPUT:
                injectUTF8Text(event.text.text);
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    System::getSingleton().notifyDisplaySizeChanged(Sizef(static_cast<float>(event.window.data1),
                                                                          static_cast<float>(event.window.data2)));
                    glViewport(0, 0, event.window.data1, event.window.data2);
                }
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE)
                {
                    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseLeaves();
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
    OpenGLRenderer::destroy(*renderer);
    renderer = 0;

    // delete SDL GL context
    SDL_GL_DeleteContext(context);

    // destroy SDL window
    SDL_DestroyWindow(window);

    // cleanup SDL
    SDL_Quit();

    return 0;
}
