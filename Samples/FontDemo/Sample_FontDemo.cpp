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

// NOTE: if you don't have Asian fonts installed, the Japanese and Korean
// sample texts might show up unreadable in this source file. The sample
// should look okay though when running.

#include "CEGuiSample.h"
#include "CEGUI.h"
#include "CEGUIPropertyHelper.h"

using namespace CEGUI;

static struct
{
    encoded_char* Language;
    encoded_char* Font;
	encoded_char* Text;
} LangList [] =
{
	// A list of strings in different languages
	// Feel free to add your own language here (UTF-8 ONLY!)...
    { (encoded_char*)"English",
      (encoded_char*)"DejaVuSans-10",
	  (encoded_char*)"THIS IS SOME TEXT IN UPPERCASE\n"
              "and this is lowercase...\n"
              "Try Catching The Brown Fox While It's Jumping Over The Lazy Dog" },
    { (encoded_char*)"Русский",
      (encoded_char*)"DejaVuSans-10",
	  (encoded_char*)"Всё ускоряющаяся эволюция компьютерных технологий предъявила жёсткие требования к производителям как собственно вычислительной техники, так и периферийных устройств.\n"
              "\nЗавершён ежегодный съезд эрудированных школьников, мечтающих глубоко проникнуть в тайны физических явлений и химических реакций.\n"
              "\nавтор панграмм -- Андрей Николаев\n" },
    { (encoded_char*)"Română",
      (encoded_char*)"DejaVuSans-10",
      (encoded_char*)"CEI PATRU APOSTOLI\n"
              "au fost trei:\n"
              "Luca şi Matfei\n" },
    { (encoded_char*)"Dansk",
      (encoded_char*)"DejaVuSans-10",
      (encoded_char*)"FARLIGE STORE BOGSTAVER\n"
              "og flere men små...\n"
              "Quizdeltagerne spiste jordbær med fløde, mens cirkusklovnen Walther spillede på xylofon\n" },
	{ (encoded_char*)"Japanese",
      (encoded_char*)"Sword-26",
      (encoded_char*)"日本語を選択\n"
              "トリガー検知\n"
              "鉱石備蓄不足\n" },
	{ (encoded_char*)"Korean",
      (encoded_char*)"Batang-26",
      (encoded_char*)"한국어를 선택\n"
              "트리거 검지\n"
              "광석 비축부족\n" },
    { (encoded_char*)"Việt",
      (encoded_char*)"DejaVuSans-10",
      (encoded_char*)"Chào CrazyEddie !\n"
              "Mình rất hạnh phúc khi nghe bạn nói điều đó\n"
              "Hy vọng sớm được thấy CEGUI hỗ trợ đầy đủ tiếng Việt\n"
              "Cám ơn bạn rất nhiều\n"
              "Chúc bạn sức khoẻ\n"
              "Tạm biệt !\n" }
};

#define MIN_POINT_SIZE 6.0f

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItem : public ListboxTextItem
{
public:
    MyListItem (const String& text, CEGUI::uint item_id = 0) :
        ListboxTextItem(text, item_id)
    {
        setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
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
        SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
        System::getSingleton().setDefaultMouseCursor ("TaharezLook/MouseArrow");

		// Create a custom font which we use to draw the list items. This custom
		// font won't get effected by the scaler and such.
		FontManager::getSingleton().createFreeTypeFont("DefaultFont", 10/*pt*/, true, "DejaVuSans.ttf");
		// Set it as the default
		System::getSingleton().setDefaultFont("DefaultFont");

        // load all the fonts (if they are not loaded yet)
        FontManager::getSingleton().createAll("*.font", "fonts");

        // load an image to use as a background
        ImageManager::getSingleton().addFromImageFile("BackgroundImage", "GPN-2000-001437.png");

        // here we will use a StaticImage as the root, then we can use it to place a background image
        Window* background = winMgr.createWindow ("TaharezLook/StaticImage");
        // set area rectangle
        background->setArea (URect (cegui_reldim (0), cegui_reldim (0),
                                          cegui_reldim (1), cegui_reldim (1)));
        // disable frame and standard background
        background->setProperty ("FrameEnabled", "false");
        background->setProperty ("BackgroundEnabled", "false");
        // set the background image
        background->setProperty ("Image", "BackgroundImage");
        // install this as the root GUI sheet
        System::getSingleton().setGUISheet (background);

        // set tooltip styles (by default there is none)
        System::getSingleton().setDefaultTooltip ("TaharezLook/Tooltip");

        // load some demo windows and attach to the background 'root'
        background->addChild(winMgr.loadLayoutFromFile("FontDemo.layout"));

        // Add the font names to the listbox
        Listbox *lbox = static_cast<Listbox *> (background->getChild("root/FontDemo/FontList"));
		lbox->setFont("DefaultFont");

        FontManager::FontIterator fi = FontManager::getSingleton().getIterator();    
        while (!fi.isAtEnd())
        {
            // exclude the special DefaultFont!
            if (fi.getCurrentKey() != String("DefaultFont"))
                lbox->addItem(new MyListItem(fi.getCurrentKey()));
            ++fi;
        }

        // set up the font listbox callback
        lbox->subscribeEvent (Listbox::EventSelectionChanged,
                              Event::Subscriber (&FontDemo::handleFontSelection, this));
        // select the first font
        lbox->setItemSelectState (size_t (0), true);

        // Add language list to the listbox
        lbox = static_cast<Listbox *> (background->getChild("root/FontDemo/LangList"));
		lbox->setFont("DefaultFont");
        for (size_t i = 0; i < (sizeof (LangList) / sizeof (LangList [0])); i++)
            // only add a language if 'preferred' font is available
            if (FontManager::getSingleton().isDefined(String(LangList[i].Font)))
                lbox->addItem (new MyListItem (LangList [i].Language, i));
        // set up the language listbox callback
        lbox->subscribeEvent (Listbox::EventSelectionChanged,
                              Event::Subscriber (&FontDemo::handleLangSelection, this));
        // select the first language
        lbox->setItemSelectState (size_t (0), true);

        background->getChild("root/FontDemo/AutoScaled")->subscribeEvent (
            Checkbox::EventCheckStateChanged,
            Event::Subscriber (&FontDemo::handleAutoScaled, this));
        background->getChild("root/FontDemo/Antialiased")->subscribeEvent (
            Checkbox::EventCheckStateChanged,
            Event::Subscriber (&FontDemo::handleAntialiased, this));
       background->getChild("root/FontDemo/PointSize")->subscribeEvent (
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber (&FontDemo::handlePointSize, this));

        return true;
	}

    // method to perform any required cleanup operations.
    void cleanupSample ()
    {
        // me? cleanup? what?
    }

	/** When a fonts get selected from the list, we update the name field. Of course,
	this can be done easier (by passing the selected font), but this demonstrates how 
	to query a widget's font. */
    void setFontDesc ()
    {
        Window* root = System::getSingleton().getGUISheet();

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (root->getChild("root/FontDemo/FontSample"));

		// Query the font from the textbox
        const Font *f = mle->getFont ();

		// Build up the font name...
        String s = f->getProperty ("Name");
        if (f->isPropertyPresent ("PointSize"))
            s += "." + f->getProperty ("PointSize");

		// ...and set it
        root->getChild("root/FontDemo/FontDesc")->setText (s);
    }

	/** Called when the used selects a different font from the font list.*/
    bool handleFontSelection (const EventArgs& e)
    {
        Window* root = System::getSingleton().getGUISheet();

		// Access the listbox which sent the event
        Listbox *lbox = static_cast<Listbox*> (
            static_cast<const WindowEventArgs&> (e).window);

        if (lbox->getFirstSelectedItem ())
		{	// Read the fontname and get the font by that name
            Font *font = &FontManager::getSingleton ().get(
                lbox->getFirstSelectedItem ()->getText ());

			// Tell the textbox to use the newly selected font
            root->getChild("root/FontDemo/FontSample")->setFont (font);

            bool b = font->isPropertyPresent ("AutoScaled");
            Checkbox *cb = static_cast<Checkbox *> (root->getChild("root/FontDemo/AutoScaled"));
            cb->setEnabled (b);
            if (b)
                cb->setSelected (PropertyHelper<bool>::fromString(font->getProperty ("AutoScaled")));

            b = font->isPropertyPresent ("Antialiased");
            cb = static_cast<Checkbox *> (root->getChild("root/FontDemo/Antialiased"));
            cb->setEnabled (b);
            if (b)
                cb->setSelected (PropertyHelper<bool>::fromString(font->getProperty ("Antialiased")));

            b = font->isPropertyPresent ("PointSize");
            Scrollbar *sb = static_cast<Scrollbar *> (
                root->getChild("root/FontDemo/PointSize"));
            sb->setEnabled (b);
			
			// Set the textbox' font to have the current scale
			if (font->isPropertyPresent("PointSize"))
				font->setProperty ("PointSize",
                        PropertyHelper<int>::toString (
                            int (MIN_POINT_SIZE + sb->getScrollPosition ())));

            setFontDesc ();
		}

        return true;
    }

    bool handleAutoScaled (const EventArgs& e)
    {
        Window* root = System::getSingleton().getGUISheet();

        Checkbox *cb = static_cast<Checkbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (root->getChild("root/FontDemo/FontSample"));

        Font *f = const_cast<Font*>(mle->getFont());
        f->setProperty ("AutoScaled",
                        PropertyHelper<bool>::toString (cb->isSelected ()));

        updateTextWindows();
        return true;
    }

    bool handleAntialiased (const EventArgs& e)
    {
        Window* root = System::getSingleton().getGUISheet();

        Checkbox *cb = static_cast<Checkbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        MultiLineEditbox *mle = static_cast<MultiLineEditbox *>
            (root->getChild("root/FontDemo/FontSample"));

        Font *f = const_cast<Font*>(mle->getFont ());
        f->setProperty ("Antialiased",
                        PropertyHelper<bool>::toString (cb->isSelected ()));

        updateTextWindows();
        return true;
    }

    bool handlePointSize (const EventArgs& e)
    {
        Window* root = System::getSingleton().getGUISheet();

        Scrollbar *sb = static_cast<Scrollbar *> (
            static_cast<const WindowEventArgs&> (e).window);

        Font *f = const_cast<Font*>(root->getChild("root/FontDemo/FontSample")->getFont ());

        f->setProperty ("PointSize",
                        PropertyHelper<int>::toString (
                            int (MIN_POINT_SIZE + sb->getScrollPosition ())));

        setFontDesc ();

        updateTextWindows();
        return true;
    }

	/** User selects a new language. Change the textbox content, and start with
	the recommended font. */
    bool handleLangSelection (const EventArgs& e)
    {
		// Access the listbox which sent the event
        Listbox *lbox = static_cast<Listbox *> (
            static_cast<const WindowEventArgs&> (e).window);

        if (lbox->getFirstSelectedItem ())
        {
            ListboxItem* sel_item = lbox->getFirstSelectedItem();
            size_t idx = sel_item ? sel_item->getID() : 0;
            const String fontName(LangList[idx].Font);

            Window* root = System::getSingleton().getGUISheet();
			// Access the font list
			Listbox *fontList = static_cast<Listbox*>(root->getChild("root/FontDemo/FontList"));
            ListboxItem* lbi = fontList->findItemWithText(fontName, 0);
			// Select correct font when not set already
			if (lbi && !lbi->isSelected())
			{	// This will cause 'handleFontSelection' to get called(!)
				fontList->setItemSelectState(lbi, true);
			}

			// Finally, set the sample text for the selected language
            root->getChild("root/FontDemo/FontSample")->setText((encoded_char*)LangList[idx].Text);
        }

        return true;
    }

    //! Ensure window content and layout is updated.
    void updateTextWindows()
    {
        Window* root = System::getSingleton().getGUISheet();
        MultiLineEditbox* eb = static_cast<MultiLineEditbox*>(
            root->getChild("root/FontDemo/FontSample"));
        // this is a hack to force the editbox to update it's state, and is
        // needed because no facility currently exists for a font to notify that
        // it's internal size or state has changed (ideally all affected windows
        // should receive EventFontChanged - this should be a TODO item!)
		eb->setWordWrapping(false);
		eb->setWordWrapping(true);
        // inform lists of updated data too
        Listbox* lb = static_cast<Listbox*>(root->getChild("root/FontDemo/LangList"));
		lb->handleUpdatedItemData();
        lb = static_cast<Listbox*>(root->getChild("root/FontDemo/FontList"));
		lb->handleUpdatedItemData();
    }
};

int main(int /*argc*/, char* /*argv*/[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    FontDemo app;
    return app.run ();
}
