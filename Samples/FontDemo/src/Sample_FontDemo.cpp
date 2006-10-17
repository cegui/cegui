/***********************************************************************
    filename:   FontDemo.cpp
    created:    17/6/2006
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
#include "CEGuiSample.h"
#include "CEGUI.h"
#include "CEGUIPropertyHelper.h"

using namespace CEGUI;

static char *FontList [] =
{
    "DejaVuSans-10",
    "Commonwealth-10",
    "Iconified-12",
    "fkp-16",
    "FairChar-30"
};

static struct
{
    utf8 *Language;
	utf8 *Text;
} LangList [] =
{
	// A list of strings in different languages
	// Feel free to add your own language here (UTF-8 ONLY!)...
    { (utf8 *)"English",
	  (utf8 *)"THIS IS SOME TEXT IN UPPERCASE\n"
              "and this is lowercase...\n"
              "Try Catching The Brown Fox While It's Jumping Over The Lazy Dog" },
    { (utf8 *)"Русский",
	  (utf8 *)"Всё ускоряющаяся эволюция компьютерных технологий предъявила жёсткие требования к производителям как собственно вычислительной техники, так и периферийных устройств.\n"
              "\nЗавершён ежегодный съезд эрудированных школьников, мечтающих глубоко проникнуть в тайны физических явлений и химических реакций.\n"
              "\nавтор панграмм -- Андрей Николаев\n" },
    { (utf8 *)"Română",
      (utf8 *)"CEI PATRU APOSTOLI\n"
              "au fost trei:\n"
              "Luca şi Matfei\n" },
    { (utf8 *)"Dansk",
      (utf8 *)"FARLIGE STORE BOGSTAVER\n"
              "og flere men små...\n"
              "Quizdeltagerne spiste jordbær med fløde, mens cirkusklovnen Walther spillede på xylofon\n" }
};

#define MIN_POINT_SIZE 6.0f

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItem : public ListboxTextItem
{
public:
    MyListItem (const String& text) : ListboxTextItem(text)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};

// Sample class
class FontDemo : public CEGuiSample
{
public:
    // method to initialse the samples windows and events.
    bool initialiseSample ()
	{
        // we will use of the WindowManager.
        WindowManager& winMgr = WindowManager::getSingleton ();

        // load scheme and set up defaults
        SchemeManager::getSingleton().loadScheme ("TaharezLook.scheme");
        System::getSingleton().setDefaultMouseCursor ("TaharezLook", "MouseArrow");

        // load all the fonts except Commonwealth which has been already loaded
        for (size_t i = 0; i < (sizeof (FontList) / sizeof (FontList [0])); i++)
            FontManager::getSingleton().createFont (String (FontList [i]) + ".font");

        // load an image to use as a background
        ImagesetManager::getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga");

        // here we will use a StaticImage as the root, then we can use it to place a background image
        Window* background = winMgr.createWindow ("TaharezLook/StaticImage");
        // set area rectangle
        background->setArea (URect (cegui_reldim (0), cegui_reldim (0),
                                          cegui_reldim (1), cegui_reldim (1)));
        // disable frame and standard background
        background->setProperty ("FrameEnabled", "false");
        background->setProperty ("BackgroundEnabled", "false");
        // set the background image
        background->setProperty ("Image", "set:BackgroundImage image:full_image");
        // install this as the root GUI sheet
        System::getSingleton ().setGUISheet (background);

        // set tooltip styles (by default there is none)
        System::getSingleton ().setDefaultTooltip ("TaharezLook/Tooltip");

        // load some demo windows and attach to the background 'root'
        background->addChildWindow (winMgr.loadWindowLayout ("FontDemo.layout"));

        // Add the font names to the listbox
        Listbox *lbox = static_cast<Listbox *> (winMgr.getWindow ("FontDemo/FontList"));
        for (size_t i = 0; i < (sizeof (FontList) / sizeof (FontList [0])); i++)
            lbox->addItem (new MyListItem (FontList [i]));
        // set up the font listbox callback
        lbox->subscribeEvent (Listbox::EventSelectionChanged,
                              Event::Subscriber (&FontDemo::handleFontSelection, this));
        // select the first font
        lbox->setItemSelectState (size_t (0), true);

        // Add language list to the listbox
        lbox = static_cast<Listbox *> (winMgr.getWindow ("FontDemo/LangList"));
        for (size_t i = 0; i < (sizeof (LangList) / sizeof (LangList [0])); i++)
            lbox->addItem (new MyListItem (LangList [i].Language));
        // set up the language listbox callback
        lbox->subscribeEvent (Listbox::EventSelectionChanged,
                              Event::Subscriber (&FontDemo::handleLangSelection, this));
        // select the first language
        lbox->setItemSelectState (size_t (0), true);

        winMgr.getWindow("FontDemo/AutoScaled")->subscribeEvent (
            Checkbox::EventCheckStateChanged,
            Event::Subscriber (&FontDemo::handleAutoScaled, this));
        winMgr.getWindow("FontDemo/Antialiased")->subscribeEvent (
            Checkbox::EventCheckStateChanged,
            Event::Subscriber (&FontDemo::handleAntialiased, this));
        winMgr.getWindow("FontDemo/PointSize")->subscribeEvent (
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber (&FontDemo::handlePointSize, this));

        return true;
	}

    // method to perform any required cleanup operations.
    void cleanupSample ()
    {
        // me? cleanup? what?
    }

    void setFontDesc ()
    {
        WindowManager& winMgr = WindowManager::getSingleton ();

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (winMgr.getWindow("FontDemo/FontSample"));

        Font *f = mle->getFont ();

        String s = f->getProperty ("Name");
        if (f->isPropertyPresent ("PointSize"))
            s += "." + f->getProperty ("PointSize");

        winMgr.getWindow("FontDemo/FontDesc")->setText (s);
    }

    bool handleFontSelection (const EventArgs& e)
    {
        Listbox *lbox = static_cast<Listbox*> (
            static_cast<const WindowEventArgs&> (e).window);

        if (lbox->getFirstSelectedItem ())
		{
            Font *f = FontManager::getSingleton ().getFont (
                lbox->getFirstSelectedItem ()->getText ());

            WindowManager& winMgr = WindowManager::getSingleton ();
            winMgr.getWindow("FontDemo/FontSample")->setFont (f);

            bool b = f->isPropertyPresent ("AutoScaled");
            Checkbox *cb = static_cast<Checkbox *> (winMgr.getWindow("FontDemo/AutoScaled"));
            cb->setEnabled (b);
            if (b)
                cb->setSelected (PropertyHelper::stringToBool (f->getProperty ("AutoScaled")));

            b = f->isPropertyPresent ("Antialiased");
            cb = static_cast<Checkbox *> (winMgr.getWindow("FontDemo/Antialiased"));
            cb->setEnabled (b);
            if (b)
                cb->setSelected (PropertyHelper::stringToBool (f->getProperty ("Antialiased")));

            b = f->isPropertyPresent ("PointSize");
            Scrollbar *sb = static_cast<Scrollbar *> (
                winMgr.getWindow("FontDemo/PointSize"));
            sb->setEnabled (b);
            if (b)
                sb->setScrollPosition (
                    PropertyHelper::stringToFloat (f->getProperty ("PointSize")) - MIN_POINT_SIZE);

            setFontDesc ();
		}

        return true;
    }

    bool handleAutoScaled (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();

        Checkbox *cb = static_cast<Checkbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (winMgr.getWindow("FontDemo/FontSample"));

        Font *f = mle->getFont ();
        f->setProperty ("AutoScaled",
                        PropertyHelper::boolToString (cb->isSelected ()));

        return true;
    }

    bool handleAntialiased (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();

        Checkbox *cb = static_cast<Checkbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (winMgr.getWindow("FontDemo/FontSample"));

        Font *f = mle->getFont ();
        f->setProperty ("Antialiased",
                        PropertyHelper::boolToString (cb->isSelected ()));

        return true;
    }

    bool handlePointSize (const EventArgs& e)
    {
        WindowManager& winMgr = WindowManager::getSingleton ();

        Scrollbar *sb = static_cast<Scrollbar *> (
            static_cast<const WindowEventArgs&> (e).window);

        Font *f = winMgr.getWindow ("FontDemo/FontSample")->getFont ();

        f->setProperty ("PointSize",
                        PropertyHelper::intToString (
                            int (MIN_POINT_SIZE + sb->getScrollPosition ())));

        setFontDesc ();

        return true;
    }

    bool handleLangSelection (const EventArgs& e)
    {
        Listbox *lbox = static_cast<Listbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        if (lbox->getFirstSelectedItem ())
        {
            size_t idx = lbox->getItemIndex (lbox->getFirstSelectedItem ());
            WindowManager& winMgr = WindowManager::getSingleton ();
            winMgr.getWindow ("FontDemo/FontSample")->setText (LangList [idx].Text);
        }

        return true;
    }
};

int main(int argc, char *argv[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    FontDemo app;
    return app.run ();
}
