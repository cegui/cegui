/***********************************************************************
created:    Sep 15 2014
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
* GLFW3
* OpengGL
* glm headers (as part of CEGUIBase)
***************************************************************************/

#include <iostream>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>

#include <GLFW/glfw3.h>

static GLFWwindow* window;
static CEGUI::InputAggregator* G_inputAggregator;
static CEGUI::GUIContext* G_ctx;

CEGUI::MouseButton toCEGUIButton(int button)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT: 
        return CEGUI::MouseButton::Left;

    case GLFW_MOUSE_BUTTON_MIDDLE:
        return CEGUI::MouseButton::Middle;

    case GLFW_MOUSE_BUTTON_RIGHT:
        return CEGUI::MouseButton::Right;

    default:
        return CEGUI::MouseButton::Count;
    }
}

CEGUI::Key::Scan toCEGUIKey(int glfwKey)
{
    switch (glfwKey)
    {
    case GLFW_KEY_ESCAPE: return CEGUI::Key::Scan::Esc;
    case GLFW_KEY_F1: return CEGUI::Key::Scan::F1;
    case GLFW_KEY_F2: return CEGUI::Key::Scan::F2;
    case GLFW_KEY_F3: return CEGUI::Key::Scan::F3;
    case GLFW_KEY_F4: return CEGUI::Key::Scan::F4;
    case GLFW_KEY_F5: return CEGUI::Key::Scan::F5;
    case GLFW_KEY_F6: return CEGUI::Key::Scan::F6;
    case GLFW_KEY_F7: return CEGUI::Key::Scan::F7;
    case GLFW_KEY_F8: return CEGUI::Key::Scan::F8;
    case GLFW_KEY_F9: return CEGUI::Key::Scan::F9;
    case GLFW_KEY_F10: return CEGUI::Key::Scan::F10;
    case GLFW_KEY_F11: return CEGUI::Key::Scan::F11;
    case GLFW_KEY_F12: return CEGUI::Key::Scan::F12;
    case GLFW_KEY_F13: return CEGUI::Key::Scan::F13;
    case GLFW_KEY_F14: return CEGUI::Key::Scan::F14;
    case GLFW_KEY_F15: return CEGUI::Key::Scan::F15;
    case GLFW_KEY_UP: return CEGUI::Key::Scan::ArrowUp;
    case GLFW_KEY_DOWN: return CEGUI::Key::Scan::ArrowDown;
    case GLFW_KEY_LEFT: return CEGUI::Key::Scan::ArrowLeft;
    case GLFW_KEY_RIGHT: return CEGUI::Key::Scan::ArrowRight;
    case GLFW_KEY_LEFT_SHIFT: return CEGUI::Key::Scan::LeftShift;
    case GLFW_KEY_RIGHT_SHIFT: return CEGUI::Key::Scan::RightShift;
    case GLFW_KEY_LEFT_CONTROL: return CEGUI::Key::Scan::LeftControl;
    case GLFW_KEY_RIGHT_CONTROL: return CEGUI::Key::Scan::RightControl;
    case GLFW_KEY_LEFT_ALT: return CEGUI::Key::Scan::LeftAlt;
    case GLFW_KEY_RIGHT_ALT: return CEGUI::Key::Scan::RightAlt;
    case GLFW_KEY_TAB: return CEGUI::Key::Scan::Tab;
    case GLFW_KEY_ENTER: return CEGUI::Key::Scan::Return;
    case GLFW_KEY_BACKSPACE: return CEGUI::Key::Scan::Backspace;
    case GLFW_KEY_INSERT: return CEGUI::Key::Scan::Insert;
    case GLFW_KEY_DELETE: return CEGUI::Key::Scan::DeleteKey;
    case GLFW_KEY_PAGE_UP: return CEGUI::Key::Scan::PageUp;
    case GLFW_KEY_PAGE_DOWN: return CEGUI::Key::Scan::PageDown;
    case GLFW_KEY_HOME: return CEGUI::Key::Scan::Home;
    case GLFW_KEY_END: return CEGUI::Key::Scan::End;
    case GLFW_KEY_KP_ENTER: return CEGUI::Key::Scan::NumpadEnter;
    case GLFW_KEY_SPACE: return CEGUI::Key::Scan::Space;
    case 'A': return CEGUI::Key::Scan::A;
    case 'B': return CEGUI::Key::Scan::B;
    case 'C': return CEGUI::Key::Scan::C;
    case 'D': return CEGUI::Key::Scan::D;
    case 'E': return CEGUI::Key::Scan::E;
    case 'F': return CEGUI::Key::Scan::F;
    case 'G': return CEGUI::Key::Scan::G;
    case 'H': return CEGUI::Key::Scan::H;
    case 'I': return CEGUI::Key::Scan::I;
    case 'J': return CEGUI::Key::Scan::J;
    case 'K': return CEGUI::Key::Scan::K;
    case 'L': return CEGUI::Key::Scan::L;
    case 'M': return CEGUI::Key::Scan::M;
    case 'N': return CEGUI::Key::Scan::N;
    case 'O': return CEGUI::Key::Scan::O;
    case 'P': return CEGUI::Key::Scan::P;
    case 'Q': return CEGUI::Key::Scan::Q;
    case 'R': return CEGUI::Key::Scan::R;
    case 'S': return CEGUI::Key::Scan::S;
    case 'T': return CEGUI::Key::Scan::T;
    case 'U': return CEGUI::Key::Scan::U;
    case 'V': return CEGUI::Key::Scan::V;
    case 'W': return CEGUI::Key::Scan::W;
    case 'X': return CEGUI::Key::Scan::X;
    case 'Y': return CEGUI::Key::Scan::Y;
    case 'Z': return CEGUI::Key::Scan::Z;
    default: return CEGUI::Key::Scan::Unknown;
    }
}

void charCallback(GLFWwindow*, unsigned int char_pressed)
{
    G_inputAggregator->injectChar(char_pressed);
}

void cursorPosCallback(GLFWwindow*, double x, double y)
{
    G_inputAggregator->injectMousePosition(x, y);
}

void keyCallback(GLFWwindow*, int key, int /*scan*/, int action, int /*mod*/)
{
    CEGUI::Key::Scan cegui_key = toCEGUIKey(key);
    if (action == GLFW_PRESS)
    {
        G_inputAggregator->injectKeyDown(cegui_key);
    }
    else
    {
        G_inputAggregator->injectKeyUp(cegui_key);
    }
}

void mouseButtonCallback(GLFWwindow*, int button, int state, int /*mod*/)
{
    if (state == GLFW_PRESS)
    {
        G_inputAggregator->injectMouseButtonDown(toCEGUIButton(button));
    }
    else
    {
        G_inputAggregator->injectMouseButtonUp(toCEGUIButton(button));
    }
}

void mouseWheelCallback(GLFWwindow*, double /*x*/, double y)
{
    if (y < 0.f)
        G_inputAggregator->injectMouseWheelChange(-1.f);
    else
        G_inputAggregator->injectMouseWheelChange(+1.f);
}

void windowResizedCallback(GLFWwindow*, int width, int height)
{
    CEGUI::System::getSingleton().notifyDisplaySizeChanged(
        CEGUI::Sizef(static_cast<float>(width), static_cast<float>(height)));
    glViewport(0, 0, width, height);
}

void errorCallback(int /*error*/, const char* message)
{
    CEGUI::Logger::getSingleton().logEvent(message, CEGUI::LoggingLevel::Error);
}

void setupCallbacks()
{
    // input callbacks
    glfwSetCharCallback(window, charCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseWheelCallback);

    // window callback
    glfwSetWindowSizeCallback(window, windowResizedCallback);

    // error callback
    glfwSetErrorCallback(errorCallback);
}

void initGLFW()
{
    // init everything from glfw
    if (glfwInit() != GL_TRUE)
    {
        std::cerr << "glfw could not be initialized!" << std::endl;
        exit(1);
    }

    // create glfw window with size of 800x600px
    window = glfwCreateWindow(800, 600, "CEGUI + glfw3 window", NULL, NULL);
    if (!window)
    {
        std::cerr << "Could not create glfw window!" << std::endl;
        glfwTerminate();
        exit(1);
    }

    // makes this window's gl context the current one
    glfwMakeContextCurrent(window);

    // hide native mouse cursor when it is over the window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // disable VSYNC
    glfwSwapInterval(0);

    // clear error messages
    glGetError();
}

void initCEGUI()
{
    using namespace CEGUI;

    // create renderer and enable extra states
    OpenGLRenderer& cegui_renderer = OpenGLRenderer::create(Sizef(800.f, 600.f));

    // create CEGUI system object
    CEGUI::System::create(cegui_renderer);

    G_ctx = &CEGUI::System::getSingleton().createGUIContext(cegui_renderer.getDefaultRenderTarget());

    G_inputAggregator = new InputAggregator(G_ctx);
    G_inputAggregator->initialise();

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
    G_ctx->setDefaultFont("DejaVuSans-10");
    G_ctx->getCursor().setDefaultImage("TaharezLook/MouseArrow");
    G_ctx->setDefaultTooltipType("TaharezLook/Tooltip");
}

void initWindows()
{
    using namespace CEGUI;

    /////////////////////////////////////////////////////////////
    // Add your gui initialisation code in here.
    // You should preferably use layout loading because you won't
    // have to recompile everytime you change the layout. But you
    // can also use static window creation code here, of course.
    /////////////////////////////////////////////////////////////

    // load layout
    Window* root = WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
    G_ctx->setRootWindow(root);
}

#ifdef _MSC_VER
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
    using namespace CEGUI;

    // init glfw
    initGLFW();

    // init cegui
    initCEGUI();

    // setup glfw callbacks
    setupCallbacks();

    // notify system of the window size
    System::getSingleton().notifyDisplaySizeChanged(Sizef(800.f, 600.f));
    glViewport(0, 0, 800, 600);

    // initialise windows and setup layout
    initWindows();

    // set gl clear color
    glClearColor(0, 0, 0, 255);

    float time = glfwGetTime();

    OpenGLRenderer* renderer = static_cast<OpenGLRenderer*>(System::getSingleton().getRenderer());

    // repeat until a quit is requested
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        // clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // inject time pulses
        const float newtime = glfwGetTime();
        const float time_elapsed = newtime - time;
        System::getSingleton().injectTimePulse(time_elapsed);
        G_ctx->injectTimePulse(time_elapsed);
        time = newtime;

        // render gui
        renderer->beginRendering();
        System::getSingleton().renderAllGUIContexts();
        renderer->endRendering();

        // swap buffers
        glfwSwapBuffers(window);

        // poll events
        glfwPollEvents();
    }

    delete G_inputAggregator;
    G_inputAggregator = 0;
    System::destroy();
    OpenGLRenderer::destroy(*renderer);
    renderer = 0;

    // destroy glfw window
    glfwDestroyWindow(window);

    // cleanup glfw
    glfwTerminate();

    return 0;
}
