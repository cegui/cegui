/************************************************************************
	filename: 	irrlichtrenderer.cpp
	created:	20/7/2004
	author:		Thomas Suter
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "irrlichtrenderer.h"

#include <sstream>

namespace CEGUI
{
/************************************************************************/
	IrrlichtRenderer::IrrlichtRenderer(irr::IrrlichtDevice* dev):device(dev)
	{
		driver=device->getVideoDriver();
		resolution=driver->getScreenSize(); // @todo use active viewport!!
		screensize=device->getVideoModeList()->getDesktopResolution();
		bSorted=false;
		bQueuingEnabled=true;
		initCodes();
	}
/************************************************************************/
	void IrrlichtRenderer::addQuad(const Rect& dest_rect, 
		float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours)
	{

		irr::u32 tex_height=tex->getHeight();
		irr::u32 tex_width=tex->getWidth();

		irr::core::rect<irr::s32> source(
			texture_rect.d_left*tex_width,texture_rect.d_top*tex_height,
			texture_rect.d_right*tex_width,texture_rect.d_bottom*tex_height);

		irr::core::rect<irr::s32> target(
			dest_rect.d_left, dest_rect.d_top,
			dest_rect.d_right, dest_rect.d_bottom);

		RenderQuad quad(z,target,source,colours,tex);
		
		if(bQueuingEnabled)
		{
			
			renderlist.push_back(quad);
			bSorted=false;
		}
		else
		{
			doRender(quad);
		}
	}
/************************************************************************/
	void IrrlichtRenderer::print(RenderQuad& quad)
	{
		char buf[255];
		irr::video::ITexture* texture=((IrrlichtTexture*)quad.tex)->getTexture();
		sprintf(buf,"renderquad(dst(%d,%d-%d,%d) src(%d,%d-%d,%d) tex(%x-%d,%d))",
			quad.dst.UpperLeftCorner.X,quad.dst.UpperLeftCorner.Y,
			quad.dst.LowerRightCorner.X,quad.dst.LowerRightCorner.Y,
			quad.src.UpperLeftCorner.X,quad.src.UpperLeftCorner.Y,
			quad.src.LowerRightCorner.X,quad.src.LowerRightCorner.Y,
			texture,texture->getSize().Width,texture->getSize().Height);
		device->getLogger()->log(buf,irr::ELL_INFORMATION);
	}
/************************************************************************/
	void IrrlichtRenderer::sortQuads(void)
	{
		if (!bSorted)
		{
			std::stable_sort(renderlist.begin(),renderlist.end(), quadsorter());
			bSorted = true;
		}
	}
/************************************************************************/
	void IrrlichtRenderer::doRender(void)
	{
		sortQuads();
		std::vector<RenderQuad>::iterator iter=renderlist.begin();
		for(;iter!=renderlist.end();++iter) doRender( (*iter) );
	}
/************************************************************************/
	void IrrlichtRenderer::doRender(RenderQuad& quad)
	{
		irr::video::ITexture* texture=((IrrlichtTexture*)quad.tex)->getTexture();
		irr::video::SColor colors[4];
		colors[0]=toIrrlichtColor(quad.colours.d_top_left);
		colors[1]=toIrrlichtColor(quad.colours.d_bottom_left);
		colors[2]=toIrrlichtColor(quad.colours.d_bottom_right);
		colors[3]=toIrrlichtColor(quad.colours.d_top_right);
		driver->draw2DImage(texture,quad.dst,quad.src,0,colors,true);
	}
/************************************************************************/
	irr::video::SColor IrrlichtRenderer::toIrrlichtColor(CEGUI::colour cecolor)
	{
		return irr::video::SColor(
			((cecolor >> 24) ),
			(((cecolor & 0x00FF0000) >> 16) ),
			(((cecolor & 0x0000FF00) >> 8) ),
			((cecolor & 0x000000FF) ));
	}
/************************************************************************/
	void IrrlichtRenderer::clearRenderList(void)
	{
		renderlist.clear();
	}
/************************************************************************/
	void IrrlichtRenderer::setQueueingEnabled(bool setting)
	{
		bQueuingEnabled=setting;
	}
/************************************************************************/
	Texture* IrrlichtRenderer::createTexture(void)
	{
		IrrlichtTexture* t=new IrrlichtTexture(this,device);
		textures.push_back(t);
		return t;
	}
/************************************************************************/
	Texture* IrrlichtRenderer::createTexture(const String& filename)
	{
		IrrlichtTexture* t=(IrrlichtTexture*)createTexture();
		t->loadFromFile(filename);
		return t;
	}
/************************************************************************/	
	Texture* IrrlichtRenderer::createTexture(float size)
	{
		IrrlichtTexture* t=(IrrlichtTexture*)createTexture();
		return t;
	}
/************************************************************************/
	void IrrlichtRenderer::destroyTexture(Texture* texture)
	{
		std::vector<IrrlichtTexture*>::iterator iter=textures.begin();
		for(;iter!=textures.end();++iter)
		{
			IrrlichtTexture* t=(*iter);
			if(t==texture)
			{
				delete t;
				textures.erase(iter);
				return;
			}
		}
	}
/************************************************************************/
	void IrrlichtRenderer::destroyAllTextures(void)
	{
		std::vector<IrrlichtTexture*>::iterator iter=textures.begin();
		for(;iter!=textures.end();)
		{
			IrrlichtTexture* t=(*iter);
			delete t;
			iter=textures.erase(iter);
		}
	}
/************************************************************************/
	bool IrrlichtRenderer::isQueueingEnabled(void) const
	{
		return bQueuingEnabled;
	}
/************************************************************************/
	float IrrlichtRenderer::getWidth(void) const
	{
		return resolution.Width;
	}
/************************************************************************/
	float IrrlichtRenderer::getHeight(void) const
	{
		return resolution.Height;
	}
/************************************************************************/
	Size IrrlichtRenderer::getSize(void) const
	{
		return Size(resolution.Width,resolution.Height);
	}
/************************************************************************/
	Rect IrrlichtRenderer::getRect(void) const
	{
		return Rect(0,0,resolution.Width,resolution.Height);
	}
/************************************************************************/
	uint IrrlichtRenderer::getMaxTextureSize(void) const
	{
		return 2048;
	}
/************************************************************************/
	uint IrrlichtRenderer::getHorzScreenDPI(void) const
	{
		return 96;
	}
/************************************************************************/
	uint IrrlichtRenderer::getVertScreenDPI(void) const
	{
		return 96;
	}
/************************************************************************/
	void IrrlichtRenderer::initCodes()
	{
		using namespace irr;
		memset(irr2ceCODE,0,KEY_KEY_CODES_COUNT);

		irr2ceCODE[KEY_LBUTTON          ] = 0x01;  // Left mouse button  
		irr2ceCODE[KEY_RBUTTON          ] = 0x02;  // Right mouse button  
		irr2ceCODE[KEY_CANCEL           ] = 0x03;  // Control-break processing  
		irr2ceCODE[KEY_MBUTTON          ] = 0x04;  // Middle mouse button (three-button mouse)  
		irr2ceCODE[KEY_XBUTTON1         ] = 0x05;  // Windows 2000/XP: X1 mouse button 
		irr2ceCODE[KEY_XBUTTON2         ] = 0x06;  // Windows 2000/XP: X2 mouse button 
		irr2ceCODE[KEY_BACK             ] = Key::Backspace; //0x08;  // BACKSPACE key  
		irr2ceCODE[KEY_TAB              ] = Key::Tab; //0x09;  // TAB key  
		irr2ceCODE[KEY_CLEAR            ] = 0x0C;  // CLEAR key  
		irr2ceCODE[KEY_RETURN           ] = Key::Return; //0x0D;  // ENTER key  
		irr2ceCODE[KEY_SHIFT            ] = Key::LeftShift;//0x10;  // SHIFT key  
		irr2ceCODE[KEY_CONTROL          ] = Key::LeftControl; //0x11;  // CTRL key  
		irr2ceCODE[KEY_MENU             ] = Key::LeftAlt;  // ALT key  
		irr2ceCODE[KEY_PAUSE            ] = Key::Pause; //0x13;  // PAUSE key  
		irr2ceCODE[KEY_CAPITAL          ] = Key::Capital;//0x14;  // CAPS LOCK key  
		irr2ceCODE[KEY_KANA             ] = Key::Kana;//0x15;  // IME Kana mode 
		irr2ceCODE[KEY_HANGUEL          ] = KEY_HANGUEL;  // IME Hanguel mode 
		irr2ceCODE[KEY_HANGUL           ] = KEY_HANGUL;  // IME Hangul mode 
		irr2ceCODE[KEY_JUNJA            ] = 0x17;  // IME Junja mode 
		irr2ceCODE[KEY_FINAL            ] = 0x18;  // IME final mode 
		irr2ceCODE[KEY_HANJA            ] = 0x19;  // IME Hanja mode 
		irr2ceCODE[KEY_KANJI            ] = KEY_KANJI;//0x19;  // IME Kanji mode 
		irr2ceCODE[KEY_ESCAPE           ] = Key::Escape;  // ESC key  
		irr2ceCODE[KEY_CONVERT          ] = Key::Convert;//0x1C;  // IME convert 
		irr2ceCODE[KEY_NONCONVERT       ] = Key::NoConvert;//0x1D;  // IME nonconvert 
		irr2ceCODE[KEY_ACCEPT           ] = 0x1E;  // IME accept 
		irr2ceCODE[KEY_MODECHANGE       ] = 0x1F;  // IME mode change request 
		irr2ceCODE[KEY_SPACE            ] = 0x20;  // SPACEBAR  
		irr2ceCODE[KEY_PRIOR            ] = Key::PageUp;//0x21;  // PAGE UP key  
		irr2ceCODE[KEY_NEXT             ] = Key::PageDown;//0x22;  // PAGE DOWN key  
		irr2ceCODE[KEY_END              ] = Key::End;//0x23;  // END key  
		irr2ceCODE[KEY_HOME             ] = Key::Home;//0x24;  // HOME key  
		irr2ceCODE[KEY_LEFT             ] = Key::ArrowLeft;//0x25;  // LEFT ARROW key  
		irr2ceCODE[KEY_UP               ] = Key::ArrowUp;//0x26;  // UP ARROW key  
		irr2ceCODE[KEY_RIGHT            ] = Key::ArrowRight;//0x27;  // RIGHT ARROW key  
		irr2ceCODE[KEY_DOWN             ] = Key::ArrowDown;//0x28;  // DOWN ARROW key  
		irr2ceCODE[KEY_SELECT           ] = 0x29;  // SELECT key  
		irr2ceCODE[KEY_PRINT            ] = 0x2A;  // PRINT key
		irr2ceCODE[KEY_EXECUT           ] = 0x2B;  // EXECUTE key  
		irr2ceCODE[KEY_SNAPSHOT         ] = 0x2C;  // PRINT SCREEN key  
		irr2ceCODE[KEY_INSERT           ] = Key::Insert;//0x2D;  // INS key  
		irr2ceCODE[KEY_DELETE           ] = Key::Delete;//0x2E;  // DEL key  
		irr2ceCODE[KEY_HELP             ] = 0x2F;  // HELP key  
		irr2ceCODE[KEY_KEY_0            ] = 0x30;  // 0 key  
		irr2ceCODE[KEY_KEY_1            ] = 0x31;  // 1 key  
		irr2ceCODE[KEY_KEY_2            ] = 0x32;  // 2 key  
		irr2ceCODE[KEY_KEY_3            ] = 0x33;  // 3 key  
		irr2ceCODE[KEY_KEY_4            ] = 0x34;  // 4 key  
		irr2ceCODE[KEY_KEY_5            ] = 0x35;  // 5 key  
		irr2ceCODE[KEY_KEY_6            ] = 0x36;  // 6 key  
		irr2ceCODE[KEY_KEY_7            ] = 0x37;  // 7 key  
		irr2ceCODE[KEY_KEY_8            ] = 0x38;  // 8 key  
		irr2ceCODE[KEY_KEY_9            ] = 0x39;  // 9 key  
		irr2ceCODE[KEY_KEY_A            ] = 0x41;  // A key  
		irr2ceCODE[KEY_KEY_B            ] = 0x42;  // B key  
		irr2ceCODE[KEY_KEY_C            ] = 0x43;  // C key  
		irr2ceCODE[KEY_KEY_D            ] = 0x44;  // D key  
		irr2ceCODE[KEY_KEY_E            ] = 0x45;  // E key  
		irr2ceCODE[KEY_KEY_F            ] = 0x46;  // F key  
		irr2ceCODE[KEY_KEY_G            ] = 0x47;  // G key  
		irr2ceCODE[KEY_KEY_H            ] = 0x48;  // H key  
		irr2ceCODE[KEY_KEY_I            ] = 0x49;  // I key  
		irr2ceCODE[KEY_KEY_J            ] = 0x4A;  // J key  
		irr2ceCODE[KEY_KEY_K            ] = 0x4B;  // K key  
		irr2ceCODE[KEY_KEY_L            ] = 0x4C;  // L key  
		irr2ceCODE[KEY_KEY_M            ] = 0x4D;  // M key  
		irr2ceCODE[KEY_KEY_N            ] = 0x4E;  // N key  
		irr2ceCODE[KEY_KEY_O            ] = 0x4F;  // O key  
		irr2ceCODE[KEY_KEY_P            ] = 0x50;  // P key  
		irr2ceCODE[KEY_KEY_Q            ] = 0x51;  // Q key  
		irr2ceCODE[KEY_KEY_R            ] = 0x52;  // R key  
		irr2ceCODE[KEY_KEY_S            ] = 0x53;  // S key  
		irr2ceCODE[KEY_KEY_T            ] = 0x54;  // T key  
		irr2ceCODE[KEY_KEY_U            ] = 0x55;  // U key  
		irr2ceCODE[KEY_KEY_V            ] = 0x56;  // V key  
		irr2ceCODE[KEY_KEY_W            ] = 0x57;  // W key  
		irr2ceCODE[KEY_KEY_X            ] = 0x58;  // X key  
		irr2ceCODE[KEY_KEY_Y            ] = 0x59;  // Y key  
		irr2ceCODE[KEY_KEY_Z            ] = 0x5A;  // Z key 
		irr2ceCODE[KEY_LWIN             ] = Key::LeftWindows;//0x5B;  // Left Windows key (Microsoft® Natural® keyboard)  
		irr2ceCODE[KEY_RWIN             ] = Key::RightWindow;//0x5C;  // Right Windows key (Natural keyboard)  
		irr2ceCODE[KEY_APPS             ] = Key::AppMenu;0x5D;  //Applications key (Natural keyboard)  
		irr2ceCODE[KEY_SLEEP            ] = 0x5F;  // Computer Sleep key 
		irr2ceCODE[KEY_NUMPAD0          ] = 0x60;  // Numeric keypad 0 key  
		irr2ceCODE[KEY_NUMPAD1          ] = 0x61;  // Numeric keypad 1 key  
		irr2ceCODE[KEY_NUMPAD2          ] = 0x62;  // Numeric keypad 2 key  
		irr2ceCODE[KEY_NUMPAD3          ] = 0x63;  // Numeric keypad 3 key  
		irr2ceCODE[KEY_NUMPAD4          ] = 0x64;  // Numeric keypad 4 key  
		irr2ceCODE[KEY_NUMPAD5          ] = 0x65;  // Numeric keypad 5 key  
		irr2ceCODE[KEY_NUMPAD6          ] = 0x66;  // Numeric keypad 6 key  
		irr2ceCODE[KEY_NUMPAD7          ] = 0x67;  // Numeric keypad 7 key  
		irr2ceCODE[KEY_NUMPAD8          ] = 0x68;  // Numeric keypad 8 key  
		irr2ceCODE[KEY_NUMPAD9          ] = 0x69;  // Numeric keypad 9 key  
		irr2ceCODE[KEY_MULTIPLY         ] = 0x6A;  // Multiply key  
		irr2ceCODE[KEY_ADD              ] = 0x6B;  // Add key  
		irr2ceCODE[KEY_SEPARATOR        ] = 0x6C;  // Separator key  
		irr2ceCODE[KEY_SUBTRACT         ] = 0x6D;  // Subtract key  
		irr2ceCODE[KEY_DECIMAL          ] = 0x6E;  // Decimal key  
		irr2ceCODE[KEY_DIVIDE           ] = 0x6F;  // Divide key  
		irr2ceCODE[KEY_F1               ] = Key::F1;//0x70;  // F1 key  
		irr2ceCODE[KEY_F2               ] = Key::F2;//0x71;  // F2 key  
		irr2ceCODE[KEY_F3               ] = Key::F3;//0x72;  // F3 key  
		irr2ceCODE[KEY_F4               ] = Key::F4;//0x73;  // F4 key  
		irr2ceCODE[KEY_F5               ] = Key::F5;//0x74;  // F5 key  
		irr2ceCODE[KEY_F6               ] = Key::F6;//0x75;  // F6 key  
		irr2ceCODE[KEY_F7               ] = Key::F7;//0x76;  // F7 key  
		irr2ceCODE[KEY_F8               ] = Key::F8;//0x77;  // F8 key  
		irr2ceCODE[KEY_F9               ] = Key::F9;//0x78;  // F9 key  
		irr2ceCODE[KEY_F10              ] = Key::F10;//0x79;  // F10 key  
		irr2ceCODE[KEY_F11              ] = Key::F11;//0x7A;  // F11 key  
		irr2ceCODE[KEY_F12              ] = Key::F12;// 0x7B;  // F12 key  
		irr2ceCODE[KEY_F13              ] = 0x7C;  // F13 key  
		irr2ceCODE[KEY_F14              ] = 0x7D;  // F14 key  
		irr2ceCODE[KEY_F15              ] = 0x7E;  // F15 key  
		irr2ceCODE[KEY_F16              ] = 0x7F;  // F16 key  
		irr2ceCODE[KEY_F17              ] = 0x80;  // F17 key  
		irr2ceCODE[KEY_F18              ] = 0x81;  // F18 key  
		irr2ceCODE[KEY_F19              ] = 0x82;  // F19 key  
		irr2ceCODE[KEY_F20              ] = 0x83;  // F20 key  
		irr2ceCODE[KEY_F21              ] = 0x84;  // F21 key  
		irr2ceCODE[KEY_F22              ] = 0x85;  // F22 key  
		irr2ceCODE[KEY_F23              ] = 0x86;  // F23 key  
		irr2ceCODE[KEY_F24              ] = 0x87;  // F24 key  
		irr2ceCODE[KEY_NUMLOCK          ] = Key::NumLock;//0x90;  // NUM LOCK key  
		irr2ceCODE[KEY_SCROLL           ] = Key::ScrollLock;//0x91;  // SCROLL LOCK key  
		irr2ceCODE[KEY_LSHIFT           ] = Key::LeftShift;//0xA0;  // Left SHIFT key 
		irr2ceCODE[KEY_RSHIFT           ] = Key::RightShift;//0xA1;  // Right SHIFT key 
		irr2ceCODE[KEY_LCONTROL         ] = Key::LeftControl;//0xA2;  // Left CONTROL key 
		irr2ceCODE[KEY_RCONTROL         ] = Key::RightControl;//0xA3;  // Right CONTROL key 
		irr2ceCODE[KEY_LMENU            ] = 0xA4;  // Left MENU key 
		irr2ceCODE[KEY_RMENU            ] = 0xA5;  // Right MENU key 
		irr2ceCODE[KEY_COMMA            ] = Key::Comma;//0xBC;  // Comma Key  (;)
		irr2ceCODE[KEY_PLUS             ] = Key::Add;  // Plus Key   (+)
		irr2ceCODE[KEY_MINUS            ] = Key::Minus;  // Minus Key  (-)
		irr2ceCODE[KEY_PERIOD           ] = Key::Period;//0xBE;  // Period Key (.)
		irr2ceCODE[KEY_ATTN             ] = 0xF6;  // Attn key 
		irr2ceCODE[KEY_CRSEL            ] = 0xF7;  // CrSel key 
		irr2ceCODE[KEY_EXSEL            ] = 0xF8;  // ExSel key 
		irr2ceCODE[KEY_EREOF            ] = 0xF9;  // Erase EOF key 
		irr2ceCODE[KEY_PLAY             ] = 0xFA;  // Play key 
		irr2ceCODE[KEY_ZOOM             ] = 0xFB;  // Zoom key 
		irr2ceCODE[KEY_PA1              ] = 0xFD;  // PA1 key 
		irr2ceCODE[KEY_OEM_CLEAR        ] = 0xFE;   // Clear key 
	}
}
