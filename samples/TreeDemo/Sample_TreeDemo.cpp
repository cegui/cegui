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
#include "Sample_TreeDemo.h"
#include "CEGUI/CEGUI.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <mmsystem.h>
#endif

#include <ctime>
#include <cstdlib>


const unsigned int TreeDemoSample::TreeID    = 1;
const unsigned int TreeDemoSample::EditBoxID = 2;

#define USE_VANILLA  0


#if USE_VANILLA
#define SCHEME_FILE_NAME   "VanillaSkin.scheme"
#define IMAGES_FILE_NAME   "Vanilla-Images"
#define STATICIMAGE_NAME   "Vanilla/StaticImage"
#define TOOLTIP_NAME       "Vanilla/Tooltip"
#define LAYOUT_FILE_NAME   "TreeDemo.layout"
#define BRUSH_NAME         "GenericBrush"
#else
#define SCHEME_FILE_NAME   "TaharezLook.scheme"
#define IMAGES_FILE_NAME   "TaharezLook"
#define STATICIMAGE_NAME   "TaharezLook/StaticImage"
#define TOOLTIP_NAME       "TaharezLook/Tooltip"
#define LAYOUT_FILE_NAME   "TreeDemoTaharez.layout"
#define BRUSH_NAME         "/TextSelectionBrush"
#endif


int randInt(int low, int high)
   {
   int   num;
   CEGUI::uint32 range;

   range = high - low + 1;
   if (range > RAND_MAX)
      num = (int)(((CEGUI::uint32)rand() * (CEGUI::uint32)rand()) % range);
   else
      num = (rand()) % range;

   return(num + low);
   }


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

    TreeDemoSample class

*************************************************************************/
//////////////////////////////////////////////////////////////////////////


/*************************************************************************
Sample specific initialisation goes here.
*************************************************************************/
bool TreeDemoSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    Tree *      theTree;
    TreeItem *  newTreeCtrlEntryLvl1;  // Level 1 TreeCtrlEntry (branch)
    TreeItem *  newTreeCtrlEntryLvl2;  // Level 2 TreeCtrlEntry (branch)
    TreeItem *  newTreeCtrlEntryLvl3;  // Level 3 TreeCtrlEntry (branch)
    TreeItem *  newTreeCtrlEntryParent;
    Image *     iconArray[9];

#if defined( __WIN32__ ) || defined( _WIN32 )
    // Windows specific code.
    srand(static_cast<unsigned int>(time(NULL)));
#endif

   // Get window manager which we will use for a few jobs here.
   WindowManager& winMgr = WindowManager::getSingleton();

   // load font and setup default if not loaded via scheme
   Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
   // Set default font for the gui context
   guiContext->setDefaultFont(&defaultFont);

   // Load the scheme to initialise the skin which we use in this sample
   SchemeManager::getSingleton().createFromFile(SCHEME_FILE_NAME);

   // set default mouse image
   guiContext->getMouseCursor().setDefaultImage(IMAGES_FILE_NAME "/MouseArrow");

   // load an image to use as a background
   if( !ImageManager::getSingleton().isDefined("SpaceBackgroundImage") )
       ImageManager::getSingleton().addFromImageFile("SpaceBackgroundImage", "SpaceBackground.jpg");

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

   TreeDemoWindow = winMgr.loadLayoutFromFile(LAYOUT_FILE_NAME);

   background->addChild(TreeDemoWindow);

   // listen for key presses on the root window.
   background->subscribeEvent(Window::EventKeyDown, Event::Subscriber(&TreeDemoSample::handleRootKeyDown, this));

   theTree = (Tree *)TreeDemoWindow->getChild(TreeID);
   theTree->initialise();
   theTree->subscribeEvent(Tree::EventSelectionChanged, Event::Subscriber(&TreeDemoSample::handleEventSelectionChanged, this));
   theTree->subscribeEvent(Tree::EventBranchOpened, Event::Subscriber(&TreeDemoSample::handleEventBranchOpened, this));
   theTree->subscribeEvent(Tree::EventBranchClosed, Event::Subscriber(&TreeDemoSample::handleEventBranchClosed, this));

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
   newTreeCtrlEntryLvl1 = new TreeItem("Tree Item Level 1a");
   newTreeCtrlEntryLvl1->setIcon(ImageManager::getSingleton().get("DriveIcons/Black"));
   newTreeCtrlEntryLvl1->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
//   newTreeCtrlEntryLvl1->setUserData((void *)someData);
   theTree->addItem(newTreeCtrlEntryLvl1);
   // Create a second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
   newTreeCtrlEntryLvl2 = new TreeItem("Tree Item Level 2a (1a)");
   newTreeCtrlEntryLvl2->setIcon(ImageManager::getSingleton().get("DriveIcons/Artic"));
   newTreeCtrlEntryLvl2->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);
   // Create a third-level TreeCtrlEntry and attach it to the above TreeCtrlEntry
   newTreeCtrlEntryLvl3 = new TreeItem("Tree Item Level 3a (2a)");
   newTreeCtrlEntryLvl3->setIcon(ImageManager::getSingleton().get("DriveIcons/Blue"));
   newTreeCtrlEntryLvl3->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   newTreeCtrlEntryLvl2->addItem(newTreeCtrlEntryLvl3);
   // Create another third-level TreeCtrlEntry and attach it to the above TreeCtrlEntry
   newTreeCtrlEntryLvl3 = new TreeItem("Tree Item Level 3b (2a)");
   newTreeCtrlEntryLvl3->setIcon(ImageManager::getSingleton().get("DriveIcons/Lime"));
   newTreeCtrlEntryLvl3->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   newTreeCtrlEntryLvl2->addItem(newTreeCtrlEntryLvl3);
   // Create another second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
   newTreeCtrlEntryLvl2 = new TreeItem("Tree Item Level 2b (1a)");
   newTreeCtrlEntryLvl2->setIcon(ImageManager::getSingleton().get("DriveIcons/Sunset"));
   newTreeCtrlEntryLvl2->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);
   // Create another second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
   newTreeCtrlEntryLvl2 = new TreeItem("Tree Item Level 2c (1a)");
   newTreeCtrlEntryLvl2->setIcon(ImageManager::getSingleton().get("DriveIcons/Silver"));
   newTreeCtrlEntryLvl2->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);

   // Create another top-most TreeCtrlEntry
   newTreeCtrlEntryLvl1 = new TreeItem("Tree Item Level 1b");
   newTreeCtrlEntryLvl1->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   newTreeCtrlEntryLvl1->setIcon(ImageManager::getSingleton().get("DriveIcons/DriveStack"));
   newTreeCtrlEntryLvl1->setDisabled(true); // Let's disable this one just to be sure it works
   theTree->addItem(newTreeCtrlEntryLvl1);
   // Create a second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
   newTreeCtrlEntryLvl2 = new TreeItem("Tree Item Level 2a (1b)");
   newTreeCtrlEntryLvl2->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);
   // Create another second-level TreeCtrlEntry and attach it to the top-most TreeCtrlEntry
   newTreeCtrlEntryLvl2 = new TreeItem("Tree Item Level 2b (1b)");
   newTreeCtrlEntryLvl2->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   newTreeCtrlEntryLvl1->addItem(newTreeCtrlEntryLvl2);

   newTreeCtrlEntryLvl1 = new TreeItem("Tree Item Level 1c");
   newTreeCtrlEntryLvl1->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
   theTree->addItem(newTreeCtrlEntryLvl1);

   // Now let's create a whole bunch of items automatically
   int levelIndex = 3;
   int idepthIndex;
   int childIndex;
   int childCount;
   unsigned int iconIndex;
   String itemText;
   while (levelIndex < 10)
      {
      idepthIndex = 0;
      itemText = "Tree Item Level " + PropertyHelper<int>::toString(levelIndex) + " Depth " + PropertyHelper<int>::toString(idepthIndex);
      newTreeCtrlEntryLvl1 = new TreeItem(itemText);
      // Set a random icon for the item.  Sometimes blank (on purpose).
      iconIndex = randInt(0, (sizeof(iconArray) / sizeof(iconArray[0])) + 2);
      if (iconIndex < sizeof(iconArray) / sizeof(iconArray[0]))
         newTreeCtrlEntryLvl1->setIcon(*iconArray[iconIndex]);
      newTreeCtrlEntryLvl1->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
      theTree->addItem(newTreeCtrlEntryLvl1);
      newTreeCtrlEntryParent = newTreeCtrlEntryLvl1;

#if 1
      childIndex = 0;
      childCount = randInt(0, 3);
      while (childIndex < childCount)
         {
         itemText = "Tree Item Level " + PropertyHelper<int>::toString(levelIndex) + " Depth " + PropertyHelper<int>::toString(idepthIndex + 1) + " Child " + PropertyHelper<int>::toString(childIndex + 1);
         newTreeCtrlEntryLvl2 = new TreeItem(itemText);
         // Set a random icon for the item.  Sometimes blank (on purpose).
         iconIndex = randInt(0, (sizeof(iconArray) / sizeof(iconArray[0]) + 2));
         if (iconIndex < sizeof(iconArray) / sizeof(iconArray[0]))
            newTreeCtrlEntryLvl2->setIcon(*iconArray[iconIndex]);
         newTreeCtrlEntryLvl2->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
         newTreeCtrlEntryParent->addItem(newTreeCtrlEntryLvl2);
         ++childIndex;
         }
#endif

      while (idepthIndex < 15)
         {
         itemText = "Tree Item Level " + PropertyHelper<int>::toString(levelIndex) + " Depth " + PropertyHelper<int>::toString(idepthIndex + 1);
         newTreeCtrlEntryLvl2 = new TreeItem(itemText);
         // Set a random icon for the item.  Sometimes blank (on purpose).
         iconIndex = randInt(0, (sizeof(iconArray) / sizeof(iconArray[0]) + 2));
         if (iconIndex < sizeof(iconArray) / sizeof(iconArray[0]))
            newTreeCtrlEntryLvl2->setIcon(*iconArray[iconIndex]);
         newTreeCtrlEntryLvl2->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
         newTreeCtrlEntryParent->addItem(newTreeCtrlEntryLvl2);
         newTreeCtrlEntryParent = newTreeCtrlEntryLvl2;

#if 1
         childIndex = 0;
         childCount = randInt(0, 3);
         while (childIndex < childCount)
            {
            itemText = "Tree Item Level " + PropertyHelper<int>::toString(levelIndex) + " Depth " + PropertyHelper<int>::toString(idepthIndex + 1) + " Child " + PropertyHelper<int>::toString(childIndex + 1);
            newTreeCtrlEntryLvl2 = new TreeItem(itemText);
            // Set a random icon for the item.  Sometimes blank (on purpose).
            iconIndex = randInt(0, (sizeof(iconArray) / sizeof(iconArray[0]) + 2));
            if (iconIndex < sizeof(iconArray) / sizeof(iconArray[0]))
               newTreeCtrlEntryLvl2->setIcon(*iconArray[iconIndex]);
            newTreeCtrlEntryLvl2->setSelectionBrushImage(IMAGES_FILE_NAME BRUSH_NAME);
            newTreeCtrlEntryParent->addItem(newTreeCtrlEntryLvl2);
            ++childIndex;
            }
#endif
         ++idepthIndex;
         }
      ++levelIndex;
      }

   return true;
   }


/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void TreeDemoSample::deinitialise()
   {
   }


bool TreeDemoSample::handleEventSelectionChanged(const CEGUI::EventArgs& args)
   {
   using namespace CEGUI;

   const TreeEventArgs& treeArgs = static_cast<const TreeEventArgs&>(args);
   Editbox *editBox = (Editbox *)TreeDemoWindow->getChild(EditBoxID);

   // Three different ways to get the item selected.
//   TreeCtrlEntry *selectedItem = theTree->getFirstSelectedItem();      // the first selection in the list (may be more)
//   TreeCtrlEntry *selectedItem = theTree->getLastSelectedItem();       // the last (time-wise) selected by the user
   TreeItem *selectedItem = treeArgs.treeItem;                    // the actual item that caused this event

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



bool TreeDemoSample::handleRootKeyDown(const CEGUI::EventArgs& args)
   {
   using namespace CEGUI;

   const KeyEventArgs& keyArgs = static_cast<const KeyEventArgs&>(args);

   switch (keyArgs.scancode)
      {
      case Key::F12:
         break;

      default:
         return false;
      }

   return true;
   }


bool TreeDemoSample::handleEventBranchOpened(const CEGUI::EventArgs& args)
   {
   using namespace CEGUI;

   const TreeEventArgs& treeArgs = static_cast<const TreeEventArgs&>(args);
   Editbox *editBox = (Editbox *)TreeDemoWindow->getChild(EditBoxID);
   editBox->setText("Opened: " + treeArgs.treeItem->getText());
   return true;
   }


bool TreeDemoSample::handleEventBranchClosed(const CEGUI::EventArgs& args)
   {
   using namespace CEGUI;

   const TreeEventArgs& treeArgs = static_cast<const TreeEventArgs&>(args);
   Editbox *editBox = (Editbox *)TreeDemoWindow->getChild(EditBoxID);
   editBox->setText("Closed: " + treeArgs.treeItem->getText());
   return true;
   }


/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static TreeDemoSample sample;
    return sample;
}