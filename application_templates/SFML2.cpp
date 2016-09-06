/***********************************************************************
    created:    Sep 18 2016
    author:     Georger Araujo <georger_br@yahoo.com.br>
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2016 Paul D Turner & The CEGUI Development Team
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
* sfml-graphics
* sfml-main
* sfml-window
* sfml-system
* OpengGL
* glm headers (as part of CEGUIBase)
***************************************************************************/

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "sfml_keycodes_to_cegui_mappings.h"


const int WIDTH = 800;
const int HEIGHT = 600;


// Convert SFML2 keyboard code to CEGUI key code
CEGUI::Key::Scan toCEGUIKey(const sf::Keyboard::Key& key)
{
    return static_cast<CEGUI::Key::Scan>(sfKeyToCEGUIKey[static_cast<int>(key)]);
}


// Convert SFML2 mouse button to CEGUI mouse button
CEGUI::MouseButton toCEGUIButton(const sf::Mouse::Button& button)
{
    using namespace CEGUI;

    switch (button)
    {
    case sf::Mouse::Left:
        return LeftButton;

    case sf::Mouse::Middle:
        return MiddleButton;

    case sf::Mouse::Right:
        return RightButton;

    case sf::Mouse::XButton1:
        return X1Button;

    case sf::Mouse::XButton2:
        return X2Button;

    default:
        return NoButton;
    }
}


void initCEGUI()
{
    using namespace CEGUI;

    // create renderer and enable extra states
    OpenGL3Renderer& cegui_renderer = OpenGL3Renderer::create(Sizef(WIDTH, HEIGHT));
    cegui_renderer.enableExtraStateSettings(true);

    // create CEGUI system object
    CEGUI::System::create(cegui_renderer);

    // setup resource directories
    DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(
                System::getSingleton().getResourceProvider());
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


bool guiHandleEvent(const sf::Event& event, const sf::Window& window)
{
    switch (event.type)
    {
    case sf::Event::TextEntered:
        return CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(event.text.unicode);

    case sf::Event::KeyPressed:
        return CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(toCEGUIKey(event.key.code));

    case sf::Event::KeyReleased:
        return CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(toCEGUIKey(event.key.code));

    case sf::Event::MouseMoved:
        return CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(
                    static_cast<float>(sf::Mouse::getPosition(window).x),
                    static_cast<float>(sf::Mouse::getPosition(window).y));

    case sf::Event::MouseButtonPressed:
        return CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
                    toCEGUIButton(event.mouseButton.button));

    case sf::Event::MouseButtonReleased:
        return CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
                    toCEGUIButton(event.mouseButton.button));

    case sf::Event::MouseWheelMoved:
        return CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(
                    static_cast<float>(event.mouseWheel.delta));

    default:
        return false;
    }

    return false;
}


int main()
{
    // See http://www.sfml-dev.org/tutorials/2.4/window-opengl.php

    // Create the window
    sf::Window window(
        sf::VideoMode(WIDTH, HEIGHT),
        "SFML + OpenGL + CEGUI",
        sf::Style::Default,
        sf::ContextSettings(0, 0, 0, 3, 2, sf::ContextSettings::Core));
    window.setMouseCursorVisible(false);
    window.setVerticalSyncEnabled(false);

    // Set GL clear color
    glClearColor(0, 0, 0, 255);

    // Init CEGUI
    initCEGUI();

    // Initialise windows and setup layout
    initWindows();

    CEGUI::OpenGL3Renderer* renderer =
            static_cast<CEGUI::OpenGL3Renderer*>(CEGUI::System::getSingleton().getRenderer());

    // Run the main loop
    sf::Clock clock;
    bool running = true;

    while (running)
    {
        // Handle events
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (guiHandleEvent(event, window))
                continue;

            if (event.type == sf::Event::Closed)
            {
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                CEGUI::System::getSingleton().notifyDisplaySizeChanged(
                            CEGUI::Sizef(event.size.width, event.size.height));
            }
        }

        // Clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Inject time pulses
        const float time_elapsed = clock.restart().asSeconds();
        CEGUI::System::getSingleton().injectTimePulse(time_elapsed);
        CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(time_elapsed);

        // Draw...

        // Draw GUI
        CEGUI::System::getSingleton().renderAllGUIContexts();

        // End the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // Destroy system and renderer
    CEGUI::System::destroy();
    CEGUI::OpenGL3Renderer::destroy(*renderer);
    renderer = 0;

    return 0;
}
