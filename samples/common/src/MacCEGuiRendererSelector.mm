/***********************************************************************
    filename:   MacCEGuiRendererSelector.cpp
    created:    Tue Mar 17 2009
    author:     Paul D Turner
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
#import "MacCEGuiRendererSelector.h"
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>

#import "CEGUISamplesConfig.h"

//----------------------------------------------------------------------------//
@interface RendererSelectionDialog : NSObject
{
    NSWindow* window;
    NSButton* cancel;
    NSButton* okay;
    NSPopUpButton* rendererList;
}

- (id)init;
- (BOOL)run;

- (void)addRendererName:(NSString*)theName;
- (NSInteger)selectedRendererIndex;

- (void)handleCancel;
- (void)handleOkay;
@end

//----------------------------------------------------------------------------//

@implementation RendererSelectionDialog
//----------------------------------------------------------------------------//
- (id)init
{
    self = [super init];
    if (self)
    {
        [NSApplication sharedApplication];
        window = [[NSWindow alloc] initWithContentRect:NSMakeRect(600, 400, 291, 100)
                                             styleMask:NSTitledWindowMask|NSMiniaturizableWindowMask
                                               backing:NSBackingStoreBuffered
                                                 defer:NO];
        
        [window setTitle:@"CEGUI Sample Application"];
        
        cancel = [[NSButton alloc] initWithFrame:NSMakeRect(14, 18, 117, 32)];
        [cancel setTitle:@"Cancel"];
        [cancel setBezelStyle:NSRoundedBezelStyle];
        [cancel setTarget:self];
        [cancel setAction:@selector(handleCancel)];
        
        okay = [[NSButton alloc] initWithFrame:NSMakeRect(160, 18, 117, 32)];
        [okay setTitle:@"OK"];
        [okay setBezelStyle:NSRoundedBezelStyle];
        [okay setTarget:self];
        [okay setAction:@selector(handleOkay)];
        
        rendererList = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(17, 57, 257, 26)];
        
        [[window contentView] addSubview:cancel];
        [[window contentView] addSubview:okay];
        [[window contentView] addSubview:rendererList];
    }
    
    return self;
}

//----------------------------------------------------------------------------//
- (void)dealloc
{
    [window close];
    
    [rendererList release];
    [okay release];
    [cancel release];

    [super dealloc];
}

//----------------------------------------------------------------------------//
- (BOOL)run
{
    [window makeKeyAndOrderFront:NSApp];
    
    if ([NSApp runModalForWindow:window])
    {
        return YES;
    }
    
    return NO;
}

//----------------------------------------------------------------------------//
- (void)addRendererName:(NSString*)theName
{
    [rendererList addItemWithTitle:theName];
}

//----------------------------------------------------------------------------//
- (NSInteger)selectedRendererIndex
{
    return [rendererList indexOfSelectedItem];
}

//----------------------------------------------------------------------------//
- (void)handleCancel
{
    [NSApp stopModalWithCode:0];
}

//----------------------------------------------------------------------------//
- (void)handleOkay
{
    [NSApp stopModalWithCode:1];
}

//----------------------------------------------------------------------------//

@end


//----------------------------------------------------------------------------//
class MacCEGuiRendererSelector_pimpl
{
public:
    MacCEGuiRendererSelector_pimpl();
    ~MacCEGuiRendererSelector_pimpl();

    //! array of CEGuiRendererTypes that map to entries in the pop up.
    CEGuiRendererType rendererTypes[4];
    //! true if user cancelled the dialog
    bool cancelled;
    // The dialog
    RendererSelectionDialog* dialog;

    NSAutoreleasePool* pool;
};

//----------------------------------------------------------------------------//
MacCEGuiRendererSelector_pimpl::MacCEGuiRendererSelector_pimpl() :
    cancelled(false),
    dialog(nil)
{
    pool = [[NSAutoreleasePool alloc] init];
}

//----------------------------------------------------------------------------//
MacCEGuiRendererSelector_pimpl::~MacCEGuiRendererSelector_pimpl()
{
    [dialog release];
    [pool release];
}

//----------------------------------------------------------------------------//
MacCEGuiRendererSelector::MacCEGuiRendererSelector() :
    d_pimpl(new MacCEGuiRendererSelector_pimpl())
{
}

//----------------------------------------------------------------------------//
MacCEGuiRendererSelector::~MacCEGuiRendererSelector()
{
    delete d_pimpl;
}

//----------------------------------------------------------------------------//
bool MacCEGuiRendererSelector::invokeDialog()
{
    d_pimpl->dialog = [[RendererSelectionDialog alloc] init];
    
    int rendererCount = populateRendererMenu();

    // 'cancel' if there are no renderers available
    if (rendererCount == 0)
        d_pimpl->cancelled = true;
    // only bother with the dialog if there is a choice ;)
    else if (rendererCount > 0)
    {
        d_pimpl->cancelled = ![d_pimpl->dialog run];
    }

    NSInteger idx = [d_pimpl->dialog selectedRendererIndex];

    [d_pimpl->dialog release];
    d_pimpl->dialog = nil;

    // bail out if user cancelled dialog or if selected index is invalid
    if (d_pimpl->cancelled || (idx == -1))
        return false;

    // set the last selected renderer - i.e. the one we want to use.
    d_lastSelected = d_pimpl->rendererTypes[idx];

    return true;
}

//----------------------------------------------------------------------------//
int MacCEGuiRendererSelector::populateRendererMenu()
{
    int idx = 0;
    // Put items in the combobox for enabled renderers.
    if (d_rendererAvailability[OpenGLGuiRendererType])
    {
        d_pimpl->rendererTypes[idx++] = OpenGLGuiRendererType;
        [d_pimpl->dialog addRendererName:@"OpenGL Renderer"];
    }
    if (d_rendererAvailability[OpenGL3GuiRendererType])
    {
        d_pimpl->rendererTypes[idx++] = OpenGL3GuiRendererType;
        [d_pimpl->dialog addRendererName:@"OpenGL 3 Renderer"];
    }
    if (d_rendererAvailability[IrrlichtGuiRendererType])
    {
        d_pimpl->rendererTypes[idx++] = IrrlichtGuiRendererType;
        [d_pimpl->dialog addRendererName:@"Irrlicht Renderer"];
    }
    if (d_rendererAvailability[OgreGuiRendererType])
    {
        d_pimpl->rendererTypes[idx++] = OgreGuiRendererType;
        [d_pimpl->dialog addRendererName:@"Ogre Renderer"];
    }
    if (d_rendererAvailability[DirectFBGuiRendererType])
    {
        d_pimpl->rendererTypes[idx++] = DirectFBGuiRendererType;
        [d_pimpl->dialog addRendererName:@"DirectFB Renderer"];
    }

    return idx;
}

//----------------------------------------------------------------------------//
