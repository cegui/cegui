/************************************************************************
    created:	 10/17/2004
    author:		 David Durant (based on code by Paul D Turner)
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
#include "Tree.h"
#include "CEGUI/CEGUI.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include <ctime>
#include <cstdint>
#include <stddef.h>


const unsigned int TreeSample::TreeID    = 1;
const unsigned int TreeSample::EditBoxID = 2;

#define USE_VANILLA  0


#if USE_VANILLA
#define SCHEME_FILE_NAME   "VanillaSkin.scheme"
#define IMAGES_FILE_NAME   "Vanilla-Images"
#define STATICIMAGE_NAME   "Vanilla/StaticImage"
#define TOOLTIP_NAME       "Vanilla/Tooltip"
#define LAYOUT_FILE_NAME   "TreeSample.layout"
#define BRUSH_NAME         "GenericBrush"
#else
#define SCHEME_FILE_NAME   "TaharezLook.scheme"
#define IMAGES_FILE_NAME   "TaharezLook"
#define STATICIMAGE_NAME   "TaharezLook/StaticImage"
#define TOOLTIP_NAME       "TaharezLook/Tooltip"
#define LAYOUT_FILE_NAME   "TreeSampleTaharez.layout"
#define BRUSH_NAME         "/TextSelectionBrush"
#endif


int randInt(int low, int high)
   {
   int   num;
   std::uint32_t range;

   range = high - low + 1;
   if (range > RAND_MAX)
      num = static_cast<int>((static_cast<std::uint32_t>(rand()) * static_cast<std::uint32_t>(rand())) % range);
   else
      num = (rand()) % range;

   return(num + low);
   }


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

    TreeSample class

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

TreeSample::TreeSample()
{
    Sample::d_name = "TreeSample";
    Sample::d_credits = "David Durant";
    Sample::d_description =
        "The Tree sample contains a tree of items that can be expanded and collapsed.";
    Sample::d_summary =
        "The sample showcases the \"CEGUI/Tree\" functionality. "
        "The \"TaharezLook/Tree\" widget is used for this purpose.";
}

/*************************************************************************
Sample specific initialisation goes here.
*************************************************************************/
bool TreeSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = String(__FILE__);

    TreeWidget* theTree;
    StandardItem*  newTreeCtrlEntryLvl1;  // Level 1 TreeCtrlEntry (branch)
    StandardItem*  newTreeCtrlEntryLvl2;  // Level 2 TreeCtrlEntry (branch)
    StandardItem*  newTreeCtrlEntryLvl3;  // Level 3 TreeCtrlEntry (branch)
    StandardItem*  newTreeCtrlEntryParent;
    Image *     iconArray[9];

#if defined( __WIN32__ ) || defined( _WIN32 )
    // Windows specific code.
    srand(static_cast<unsigned int>(time(nullptr)));
#endif

    // Get window manager which we will use for a few jobs here.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load font and setup default if not loaded via scheme
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();
    // Set default font for the gui context
    guiContext->setDefaultFont(defaultFont);

    // Load the scheme to initialise the skin which we use in this sample
    SchemeManager::getSingleton().createFromFile(SCHEME_FILE_NAME);

    // set default cursor image
    guiContext->getCursor().setDefaultImage(IMAGES_FILE_NAME "/MouseArrow");

    // load an image to use as a background
    if (!ImageManager::getSingleton().isDefined("SpaceBackgroundImage"))
        ImageManager::getSingleton().addBitmapImageFromFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    // Load some icon images for our test tree
    ImageManager::getSingleton().loadImageset("DriveIcons.imageset");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow(STATICIMAGE_NAME);

    // set area rectangle
    background->setArea(URect(cegui_reldim(0), cegui_reldim(0), cegui_reldim(1), cegui_reldim(1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "SpaceBackgroundImage");
    // install this as the root GUI sheet
    guiContext->setRootWindow(background);

    FontManager::getSingleton().createFromFile("DejaVuSans-12.font");

    d_rootWindow = winMgr.loadLayoutFromFile(LAYOUT_FILE_NAME);

    background->addChild(d_rootWindow);

    theTree = static_cast<TreeWidget*>(d_rootWindow->getChild(TreeID));
    theTree->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
    theTree->subscribeEvent(TreeWidget::EventSelectionChanged, Event::Subscriber(&TreeSample::handleEventSelectionChanged, this));
    theTree->subscribeEvent(TreeWidget::EventSubtreeExpanded, Event::Subscriber(&TreeSample::handleEventSubtreeExpanded, this));
    theTree->subscribeEvent(TreeWidget::EventSubtreeCollapsed, Event::Subscriber(&TreeSample::handleEventSubtreeCollapsed, this));

    // activate the background window
    background->activate();

    iconArray[0] = &ImageManager::getSingleton().get("DriveIcons/Artic");
    iconArray[1] = &ImageManager::getSingleton().get("DriveIcons/Black");
    iconArray[2] = &ImageManager::getSingleton().get("DriveIcons/Sunset");
    iconArray[3] = &ImageManager::getSingleton().get("DriveIcons/DriveStack");
    iconArray[4] = &ImageManager::getSingleton().get("DriveIcons/GlobalDrive");
    iconArray[5] = &ImageManager::getSingleton().get("DriveIcons/Blue");
    iconArray[6] = &ImageManager::getSingleton().get("DriveIcons/Lime");
    iconArray[7] = &ImageManager::getSingleton().get("DriveIcons/Silver");
    iconArray[8] = &ImageManager::getSingleton().get("DriveIcons/GreenCandy");

    // Create a top-most TreeCtrlEntry
    newTreeCtrlEntryLvl1 = new StandardItem("Tree Item Level 1a", "DriveIcons/Black");
    theTree->addItem(newTreeCtrlEntryLvl1);

    // Create a second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
    newTreeCtrlEntryLvl2 = new StandardItem("Tree Item Level 2a (1a)");
    newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);
    // Create a third-level TreeCtrlEntry and attach it to the above TreeCtrlEntry
    newTreeCtrlEntryLvl3 = new StandardItem("Tree Item Level 3a (2a)", "DriveIcons/Blue");
    newTreeCtrlEntryLvl2->addItem(newTreeCtrlEntryLvl3);
    // Create another third-level TreeCtrlEntry and attach it to the above TreeCtrlEntry
    newTreeCtrlEntryLvl3 = new StandardItem("Tree Item Level 3b (2a)", "DriveIcons/Lime");
    newTreeCtrlEntryLvl2->addItem(newTreeCtrlEntryLvl3);
    // Create another second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
    newTreeCtrlEntryLvl2 = new StandardItem("Tree Item Level 2b (1a)", "DriveIcons/Sunset");
    newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);
    // Create another second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
    newTreeCtrlEntryLvl2 = new StandardItem("Tree Item Level 2c (1a)", "DriveIcons/Silver");
    newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);

    // Create another top-most TreeCtrlEntry
    newTreeCtrlEntryLvl1 = new StandardItem("Tree Item Level 1b", "DriveIcons/DriveStack");
    theTree->addItem(newTreeCtrlEntryLvl1);
    // Create a second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
    newTreeCtrlEntryLvl2 = new StandardItem("Tree Item Level 2a (1b)");
    newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);
    // Create another second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
    newTreeCtrlEntryLvl2 = new StandardItem("Tree Item Level 2b (1b)");
    newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);

    newTreeCtrlEntryLvl1 = new StandardItem("Tree Item Level 1c");
    theTree->addItem(newTreeCtrlEntryLvl1);

    // Now let's create a whole bunch of items automatically
    std::int32_t levelIndex = 3;
    std::int32_t idepthIndex;
    std::int32_t childIndex;
    std::int32_t childCount;
    unsigned int iconIndex;
    String itemText;
    while (levelIndex < 10)
    {
        idepthIndex = 0;
        itemText = "Tree Item Level " + PropertyHelper<std::int32_t>::toString(levelIndex) + " Depth " + PropertyHelper<std::int32_t>::toString(idepthIndex);
        newTreeCtrlEntryLvl1 = new StandardItem(itemText);
        // Set a random icon for the item.  Sometimes blank (on purpose).
        iconIndex = randInt(0, (sizeof(iconArray) / sizeof(iconArray[0])) + 2);
        if (iconIndex < sizeof(iconArray) / sizeof(iconArray[0]))
            newTreeCtrlEntryLvl1->setIcon((*iconArray[iconIndex]).getName());
        theTree->addItem(newTreeCtrlEntryLvl1);
        newTreeCtrlEntryParent = newTreeCtrlEntryLvl1;

        childIndex = 0;
        childCount = randInt(0, 3);
        while (childIndex < childCount)
        {
            itemText = "Tree Item Level " + PropertyHelper<std::int32_t>::toString(levelIndex) + " Depth " + PropertyHelper<std::int32_t>::toString(idepthIndex + 1) + " Child " + PropertyHelper<std::int32_t>::toString(childIndex + 1);
            newTreeCtrlEntryLvl2 = new StandardItem(itemText);
            // Set a random icon for the item.  Sometimes blank (on purpose).
            iconIndex = randInt(0, (sizeof(iconArray) / sizeof(iconArray[0]) + 2));
            if (iconIndex < sizeof(iconArray) / sizeof(iconArray[0]))
                newTreeCtrlEntryLvl2->setIcon((*iconArray[iconIndex]).getName());
            newTreeCtrlEntryParent->addItem(newTreeCtrlEntryLvl2);
            ++childIndex;
        }

        while (idepthIndex < 15)
        {
            itemText = "Tree Item Level " + PropertyHelper<std::int32_t>::toString(levelIndex) + " Depth " + PropertyHelper<std::int32_t>::toString(idepthIndex + 1);
            newTreeCtrlEntryLvl2 = new StandardItem(itemText);
            // Set a random icon for the item.  Sometimes blank (on purpose).
            iconIndex = randInt(0, (sizeof(iconArray) / sizeof(iconArray[0]) + 2));
            if (iconIndex < sizeof(iconArray) / sizeof(iconArray[0]))
                newTreeCtrlEntryLvl2->setIcon((*iconArray[iconIndex]).getName());
            newTreeCtrlEntryParent->addItem(newTreeCtrlEntryLvl2);
            newTreeCtrlEntryParent = newTreeCtrlEntryLvl2;

            childIndex = 0;
            childCount = randInt(0, 3);
            while (childIndex < childCount)
            {
                itemText = "Tree Item Level " + PropertyHelper<std::int32_t>::toString(levelIndex) + " Depth " + PropertyHelper<std::int32_t>::toString(idepthIndex + 1) + " Child " + PropertyHelper<std::int32_t>::toString(childIndex + 1);
                newTreeCtrlEntryLvl2 = new StandardItem(itemText);
                // Set a random icon for the item.  Sometimes blank (on purpose).
                iconIndex = randInt(0, (sizeof(iconArray) / sizeof(iconArray[0]) + 2));
                if (iconIndex < sizeof(iconArray) / sizeof(iconArray[0]))
                    newTreeCtrlEntryLvl2->setIcon((*iconArray[iconIndex]).getName());
                newTreeCtrlEntryParent->addItem(newTreeCtrlEntryLvl2);
                ++childIndex;
            }
            ++idepthIndex;
        }
        ++levelIndex;
    }

    return true;
}


/*************************************************************************
Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void TreeSample::deinitialise()
{
}


bool TreeSample::handleEventSelectionChanged(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    const ItemViewEventArgs& treeArgs = static_cast<const ItemViewEventArgs&>(args);
    Editbox *editBox = static_cast<Editbox *>(d_rootWindow->getChild(EditBoxID));

    TreeWidget* tree = static_cast<TreeWidget*>(treeArgs.window);
    // Three different ways to get the item selected.
    //   TreeCtrlEntry *selectedItem = theTree->getFirstSelectedItem();      // the first selection in the list (may be more)
    //   TreeCtrlEntry *selectedItem = theTree->getLastSelectedItem();       // the last (time-wise) selected by the user
    StandardItem* selectedItem = tree->getModel()->getItemForIndex(treeArgs.d_index);                    // the actual item that caused this event

    if (selectedItem)
    {
#if 0
        // A convoluted way to get the item text.
        // Just here to test findFirstItemWithText.
        selectedItem = theTree->findFirstItemWithText(selectedItem->getText());
        if (selectedItem)
            editBox->setText("Selected: " + selectedItem->getText());
        else
            editBox->setText("findItemWithText failed!");
#else
        // The simple way to do it.
        editBox->setText("Selected: " + selectedItem->getText());
#endif
    }
    else
        editBox->setText("None Selected");

    return true;
}

bool TreeSample::handleEventSubtreeExpanded(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    const ItemViewEventArgs& treeArgs = static_cast<const ItemViewEventArgs&>(args);
    Editbox *editBox = static_cast<Editbox *>(d_rootWindow->getChild(EditBoxID));
    TreeWidget* tree = static_cast<TreeWidget*>(treeArgs.window);
    StandardItem* expanded_item = tree->getModel()->getItemForIndex(treeArgs.d_index);
    editBox->setText("Opened: " + expanded_item->getText());
    return true;
}


bool TreeSample::handleEventSubtreeCollapsed(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    const ItemViewEventArgs& treeArgs = static_cast<const ItemViewEventArgs&>(args);
    Editbox *editBox = static_cast<Editbox *>(d_rootWindow->getChild(EditBoxID));
    TreeWidget* tree = static_cast<TreeWidget*>(treeArgs.window);
    StandardItem* collapsed_item = tree->getModel()->getItemForIndex(treeArgs.d_index);
    editBox->setText("Closed: " + collapsed_item->getText());
    return true;
}
