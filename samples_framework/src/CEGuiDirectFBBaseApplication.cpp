/***********************************************************************
    created:    4/10/2008
    author:     Keith Mok
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUISamplesConfig.h"
#ifdef CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE

#include "CEGuiDirectFBBaseApplication.h"
#include "CEGUI/RendererModules/DirectFB/Renderer.h"
#include "CEGuiSample.h"
#include "CEGUI/CEGUI.h"

#include <directfb.h>

/*************************************************************************
    Simple key-map used to translate glut special keys to CEGUI keys.
    See: CEGuiOpenGLBaseApplication::keySpecial method.
*************************************************************************/
struct DFBKeyMapping
{
    int dfbKey;
    CEGUI::Key::Scan  ceguiKey;
};

static DFBKeyMapping specialKeyMap[] =
{
    {DIKS_F1, CEGUI::Key::F1},
    {DIKS_F2, CEGUI::Key::F2},
    {DIKS_F3, CEGUI::Key::F3},
    {DIKS_F4, CEGUI::Key::F4},
    {DIKS_F5, CEGUI::Key::F5},
    {DIKS_F6, CEGUI::Key::F6},
    {DIKS_F7, CEGUI::Key::F7},
    {DIKS_F8, CEGUI::Key::F8},
    {DIKS_F9, CEGUI::Key::F9},
    {DIKS_F10, CEGUI::Key::F10},
    {DIKS_F11, CEGUI::Key::F11},
    {DIKS_F12, CEGUI::Key::F12},
    {DIKS_CURSOR_LEFT, CEGUI::Key::ArrowLeft},
    {DIKS_CURSOR_UP, CEGUI::Key::ArrowUp},
    {DIKS_CURSOR_RIGHT, CEGUI::Key::ArrowRight},
    {DIKS_CURSOR_DOWN, CEGUI::Key::ArrowDown},
    {DIKS_PAGE_UP, CEGUI::Key::PageUp},
    {DIKS_PAGE_DOWN, CEGUI::Key::PageDown},
    {DIKS_HOME, CEGUI::Key::Home},
    {DIKS_END, CEGUI::Key::End},
    {DIKS_INSERT, CEGUI::Key::Insert},
    {DIKS_DELETE, CEGUI::Key::Delete},
    {DIKS_ENTER, CEGUI::Key::Return},
    {DIKS_BACKSPACE, CEGUI::Key::Backspace},
    {-1, CEGUI::Key::Unknown}
};

//----------------------------------------------------------------------------//
struct CEGuiBaseApplicationImpl
{
    IDirectFB* d_dfb;
    IDirectFBDisplayLayer* d_layer;
    IDirectFBWindow* d_window;
    IDirectFBSurface* d_surface;
    IDirectFBEventBuffer* d_event_buffer;
};

//----------------------------------------------------------------------------//
CEGuiDirectFBBaseApplication::CEGuiDirectFBBaseApplication() :
    pimpl(new CEGuiBaseApplicationImpl)
{
    DFBResult ret;

    setenv("DFBARGS", "no-cursor-updates,layer-bg-none", 1);

    ret = DirectFBInit(0, 0);
    if (ret)
    {
        CEGUI_THROW(std::runtime_error("DirectFB application failed to "
            "initialise. [DirectFBInit]"));
    }

    ret = DirectFBCreate(&pimpl->d_dfb);
    if (ret)
    {
        pimpl->d_dfb = 0;
        CEGUI_THROW(std::runtime_error("DirectFB application failed to "
            "initialise. [DirectFBCreate]"));
    }

    ret = pimpl->d_dfb->GetDisplayLayer(pimpl->d_dfb, DLID_PRIMARY, &pimpl->d_layer);
    if (ret)
    {
        pimpl->d_layer = 0;
        pimpl->d_dfb->Release(pimpl->d_dfb);
        pimpl->d_dfb = 0;
        CEGUI_THROW(std::runtime_error("DirectFB application failed to "
            "initialise. [GetDisplayLayer]"));
    }

    if(initialiseDirectFB(800, 600, true))
    {
        CEGUI_THROW(std::runtime_error("DirectFB application failed to "
            "initialise. [initialiseDirectFB]"));
    }

    ret = pimpl->d_window->CreateEventBuffer(pimpl->d_window, &pimpl->d_event_buffer);
    if (ret)
    {
        pimpl->d_layer->Release(pimpl->d_layer);
        pimpl->d_layer = 0;
        pimpl->d_dfb->Release(pimpl->d_dfb);
        pimpl->d_dfb = 0;
        CEGUI_THROW(std::runtime_error("DirectFB application failed to "
            "initialise. [CreateEventBuffer]"));
    }

    pimpl->d_window->GetSurface(pimpl->d_window, &pimpl->d_surface);

    d_renderer =
        &CEGUI::DirectFBRenderer::create(*pimpl->d_dfb, *pimpl->d_surface);

    return;
}

//----------------------------------------------------------------------------//
CEGuiDirectFBBaseApplication::~CEGuiDirectFBBaseApplication()
{
    cleanupDirectFB();

    delete pimpl;
}

//----------------------------------------------------------------------------//
void CEGuiDirectFBBaseApplication::handleKeyUpEvent(DFBWindowEvent *evt)
{
    DFBKeyMapping* mapping = specialKeyMap;

    while (mapping->dfbKey != -1)
    {
        if (mapping->dfbKey == evt->key_symbol)
        {
            CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(mapping->ceguiKey);
            return;
        }

        ++mapping;
    }
}
//----------------------------------------------------------------------------//
void CEGuiDirectFBBaseApplication::handleKeyDownEvent(DFBWindowEvent *evt)
{
    DFBKeyMapping* mapping = specialKeyMap;

    if(evt->key_symbol < 128)
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(static_cast<CEGUI::utf32>(evt->key_symbol));
    }
    else
    {
        while (mapping->dfbKey != -1)
        {
            if (mapping->dfbKey == evt->key_symbol)
            {
                CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(mapping->ceguiKey);
                return;
            }

            ++mapping;
        }
    }
}

//----------------------------------------------------------------------------//
void CEGuiDirectFBBaseApplication::handleWindowEvent(DFBWindowEvent *evt)
{
    switch(evt->type)
    {
    case DWET_ENTER:
        break;
    case DWET_LEAVE:
        break;
    case DWET_MOTION:
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(evt->x, evt->y);
        break;
    case DWET_BUTTONDOWN:
        if(evt->button == DIBI_LEFT)
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
        break;
    case DWET_BUTTONUP:
        if(evt->button == DIBI_LEFT)
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
        break;
    case DWET_KEYUP:
        handleKeyUpEvent(evt);
        break;
    case DWET_KEYDOWN:
        handleKeyDownEvent(evt);
        break;
    case DWET_WHEEL:
        break;
    default:
        break;
    }
}

//----------------------------------------------------------------------------//
bool CEGuiDirectFBBaseApplication::execute_impl(CEGuiSample* sampleApp)
{
    sampleApp->initialiseSample();
    DFBEvent    evt;
    DFBResult   ret;

    CEGUI::System& guiSystem = CEGUI::System::getSingleton();

    while(!isQuitting())
    {
        if ((DFB_OK == pimpl->d_event_buffer->HasEvent(pimpl->d_event_buffer)))
        {
            ret = pimpl->d_event_buffer->GetEvent(pimpl->d_event_buffer, DFB_EVENT(&evt));
            if (ret != DFB_OK || evt.clazz != DFEC_WINDOW)
                continue;

            DFBWindowEvent *win_event = (DFBWindowEvent*)(&evt);
            handleWindowEvent(win_event);
        }
        else
        {
            renderSingleFrame(0.0f);
        }
    }

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiDirectFBBaseApplication::cleanup_impl()
{
    // nothing to do here.
}

//----------------------------------------------------------------------------//
void CEGuiDirectFBBaseApplication::beginRendering(const float elapsed)
{
    // clear display
    pimpl->d_surface->Clear( pimpl->d_surface, 0x0, 0x0, 0x0, 0xFF );
}

//----------------------------------------------------------------------------//
void CEGuiDirectFBBaseApplication::endRendering()
{
    pimpl->d_surface->Flip(pimpl->d_surface, 0, (DFBSurfaceFlipFlags)0);
}

//----------------------------------------------------------------------------//
bool CEGuiDirectFBBaseApplication::initialiseDirectFB(unsigned int width,
    unsigned int height, bool /*windowed*/)
{
    DFBWindowDescription desc_window;
    desc_window.flags = (DFBWindowDescriptionFlags)(DWDESC_POSX | DWDESC_POSY |
                        DWDESC_WIDTH | DWDESC_HEIGHT);
    desc_window.posx  = 100;
    desc_window.posy  = 100;

    desc_window.width = width;
    desc_window.height= height;

    DFBResult ret;

    ret = pimpl->d_layer->CreateWindow( pimpl->d_layer, &desc_window, &pimpl->d_window);
    if(ret)
        return true;

    pimpl->d_window->SetOpacity( pimpl->d_window, 0xff );

    return false;
}

//----------------------------------------------------------------------------//
void CEGuiDirectFBBaseApplication::cleanupDirectFB()
{
    if(pimpl->d_event_buffer) {
        pimpl->d_event_buffer->Release(pimpl->d_event_buffer);
    }
    if(pimpl->d_surface) {
        pimpl->d_surface->Release(pimpl->d_surface);
    }
    if(pimpl->d_window) {
        pimpl->d_window->Release(pimpl->d_window);
    }
    if(pimpl->d_layer) {
        pimpl->d_layer->Release(pimpl->d_layer);
    }
    if(pimpl->d_dfb) {
        pimpl->d_dfb->Release(pimpl->d_dfb);
    }
    pimpl->d_dfb = 0;
    pimpl->d_layer = 0;
    pimpl->d_window = 0;
    pimpl->d_surface = 0;
    pimpl->d_event_buffer = 0;
}

#endif
