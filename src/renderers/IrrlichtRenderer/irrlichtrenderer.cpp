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
#include "renderers/IrrlichtRenderer/irrlichtrenderer.h"

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
		//char buf[255];
		//irr::video::ITexture* texture=((IrrlichtTexture*)quad.tex)->getTexture();
		//sprintf(buf,"renderquad(dst(%d,%d-%d,%d) src(%d,%d-%d,%d) tex(%x-%d,%d))",
		//	quad.dst.UpperLeftCorner.X,quad.dst.UpperLeftCorner.Y,
		//	quad.dst.LowerRightCorner.X,quad.dst.LowerRightCorner.Y,
		//	quad.src.UpperLeftCorner.X,quad.src.UpperLeftCorner.Y,
		//	quad.src.LowerRightCorner.X,quad.src.LowerRightCorner.Y,
		//	(unsigned long)texture,texture->getSize().Width,texture->getSize().Height);
		//device->getLogger()->log(buf,irr::ELL_INFORMATION);
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

		irr2ceCODE[KEY_LBUTTON          ] = 0;  // Left mouse button  
		irr2ceCODE[KEY_RBUTTON          ] = 0;  // Right mouse button  
		irr2ceCODE[KEY_CANCEL           ] = 0;  // Control-break processing  
		irr2ceCODE[KEY_MBUTTON          ] = 0;  // Middle mouse button (three-button mouse)  
		irr2ceCODE[KEY_XBUTTON1         ] = 0;  // Windows 2000/XP: X1 mouse button 
		irr2ceCODE[KEY_XBUTTON2         ] = 0;  // Windows 2000/XP: X2 mouse button 
		irr2ceCODE[KEY_BACK             ] = Key::Backspace; //0x08;  // BACKSPACE key  
		irr2ceCODE[KEY_TAB              ] = Key::Tab; //0x09;  // TAB key  
		irr2ceCODE[KEY_CLEAR            ] = 0;  // CLEAR key  
		irr2ceCODE[KEY_RETURN           ] = Key::Return; //0x0D;  // ENTER key  
		irr2ceCODE[KEY_SHIFT            ] = Key::LeftShift;  // SHIFT key  
		irr2ceCODE[KEY_CONTROL          ] = Key::LeftControl;   // CTRL key  
		irr2ceCODE[KEY_MENU             ] = Key::LeftAlt;  // ALT key  
		irr2ceCODE[KEY_PAUSE            ] = Key::Pause;   // PAUSE key  
		irr2ceCODE[KEY_CAPITAL          ] = Key::Capital;  // CAPS LOCK key  
		irr2ceCODE[KEY_KANA             ] = Key::Kana;  // IME Kana mode 
		irr2ceCODE[KEY_HANGUEL          ] = KEY_HANGUEL;  // IME Hanguel mode 
		irr2ceCODE[KEY_HANGUL           ] = KEY_HANGUL;  // IME Hangul mode 
		irr2ceCODE[KEY_JUNJA            ] = 0;  // IME Junja mode 
		irr2ceCODE[KEY_FINAL            ] = 0;  // IME final mode 
		irr2ceCODE[KEY_HANJA            ] = 0;  // IME Hanja mode 
		irr2ceCODE[KEY_KANJI            ] = 0;  // IME Kanji mode 
		irr2ceCODE[KEY_ESCAPE           ] = Key::Escape;  // ESC key  
		irr2ceCODE[KEY_CONVERT          ] = Key::Convert;  // IME convert 
		irr2ceCODE[KEY_NONCONVERT       ] = Key::NoConvert;  // IME nonconvert 
		irr2ceCODE[KEY_ACCEPT           ] = 0;  // IME accept 
		irr2ceCODE[KEY_MODECHANGE       ] = 0;  // IME mode change request 
		irr2ceCODE[KEY_SPACE            ] = Key::Space;  // SPACEBAR  
		irr2ceCODE[KEY_PRIOR            ] = Key::PageUp;  // PAGE UP key  
		irr2ceCODE[KEY_NEXT             ] = Key::PageDown;  // PAGE DOWN key  
		irr2ceCODE[KEY_END              ] = Key::End;  // END key  
		irr2ceCODE[KEY_HOME             ] = Key::Home;  // HOME key  
		irr2ceCODE[KEY_LEFT             ] = Key::ArrowLeft;  // LEFT ARROW key  
		irr2ceCODE[KEY_UP               ] = Key::ArrowUp;  // UP ARROW key  
		irr2ceCODE[KEY_RIGHT            ] = Key::ArrowRight;  // RIGHT ARROW key  
		irr2ceCODE[KEY_DOWN             ] = Key::ArrowDown;  // DOWN ARROW key  
		irr2ceCODE[KEY_SELECT           ] = 0;  // SELECT key  
		irr2ceCODE[KEY_PRINT            ] = Key::SysRq;  // PRINT key
		irr2ceCODE[KEY_EXECUT           ] = 0;  // EXECUTE key  
		irr2ceCODE[KEY_SNAPSHOT         ] = 0;  // PRINT SCREEN key  
		irr2ceCODE[KEY_INSERT           ] = Key::Insert;//0x2D;  // INS key  
		irr2ceCODE[KEY_DELETE           ] = Key::Delete;//0x2E;  // DEL key  
		irr2ceCODE[KEY_HELP             ] = 0;  // HELP key  
		irr2ceCODE[KEY_KEY_0            ] = Key::Zero;   // 0 key  
		irr2ceCODE[KEY_KEY_1            ] = Key::One;    // 1 key  
		irr2ceCODE[KEY_KEY_2            ] = Key::Two;    // 2 key  
		irr2ceCODE[KEY_KEY_3            ] = Key::Three;  // 3 key  
		irr2ceCODE[KEY_KEY_4            ] = Key::Four;   // 4 key  
		irr2ceCODE[KEY_KEY_5            ] = Key::Five;   // 5 key  
		irr2ceCODE[KEY_KEY_6            ] = Key::Six;    // 6 key  
		irr2ceCODE[KEY_KEY_7            ] = Key::Seven;  // 7 key  
		irr2ceCODE[KEY_KEY_8            ] = Key::Eight;  // 8 key  
		irr2ceCODE[KEY_KEY_9            ] = Key::Nine;   // 9 key  
		irr2ceCODE[KEY_KEY_A            ] = Key::A;  // A key  
		irr2ceCODE[KEY_KEY_B            ] = Key::B;  // B key  
		irr2ceCODE[KEY_KEY_C            ] = Key::C;  // C key  
		irr2ceCODE[KEY_KEY_D            ] = Key::D;  // D key  
		irr2ceCODE[KEY_KEY_E            ] = Key::E;  // E key  
		irr2ceCODE[KEY_KEY_F            ] = Key::F;  // F key  
		irr2ceCODE[KEY_KEY_G            ] = Key::G;  // G key  
		irr2ceCODE[KEY_KEY_H            ] = Key::H;  // H key  
		irr2ceCODE[KEY_KEY_I            ] = Key::I;  // I key  
		irr2ceCODE[KEY_KEY_J            ] = Key::J;  // J key  
		irr2ceCODE[KEY_KEY_K            ] = Key::K;  // K key  
		irr2ceCODE[KEY_KEY_L            ] = Key::L;  // L key  
		irr2ceCODE[KEY_KEY_M            ] = Key::M;  // M key  
		irr2ceCODE[KEY_KEY_N            ] = Key::N;  // N key  
		irr2ceCODE[KEY_KEY_O            ] = Key::O;  // O key  
		irr2ceCODE[KEY_KEY_P            ] = Key::P;  // P key  
		irr2ceCODE[KEY_KEY_Q            ] = Key::Q;  // Q key  
		irr2ceCODE[KEY_KEY_R            ] = Key::R;  // R key  
		irr2ceCODE[KEY_KEY_S            ] = Key::S;  // S key  
		irr2ceCODE[KEY_KEY_T            ] = Key::T;  // T key  
		irr2ceCODE[KEY_KEY_U            ] = Key::U;  // U key  
		irr2ceCODE[KEY_KEY_V            ] = Key::V;  // V key  
		irr2ceCODE[KEY_KEY_W            ] = Key::W;  // W key  
		irr2ceCODE[KEY_KEY_X            ] = Key::X;  // X key  
		irr2ceCODE[KEY_KEY_Y            ] = Key::Y;  // Y key  
		irr2ceCODE[KEY_KEY_Z            ] = Key::Z;  // Z key 
		irr2ceCODE[KEY_LWIN             ] = Key::LeftWindows;  // Left Windows key (Microsoft® Natural® keyboard)  
		irr2ceCODE[KEY_RWIN             ] = Key::RightWindow;  // Right Windows key (Natural keyboard)  
		irr2ceCODE[KEY_APPS             ] = Key::AppMenu;  //Applications key (Natural keyboard)  
		irr2ceCODE[KEY_SLEEP            ] = Key::Sleep;  // Computer Sleep key 
		irr2ceCODE[KEY_NUMPAD0          ] = Key::Numpad0;  // Numeric keypad 0 key  
		irr2ceCODE[KEY_NUMPAD1          ] = Key::Numpad1;  // Numeric keypad 1 key  
		irr2ceCODE[KEY_NUMPAD2          ] = Key::Numpad2;  // Numeric keypad 2 key  
		irr2ceCODE[KEY_NUMPAD3          ] = Key::Numpad3;  // Numeric keypad 3 key  
		irr2ceCODE[KEY_NUMPAD4          ] = Key::Numpad4;  // Numeric keypad 4 key  
		irr2ceCODE[KEY_NUMPAD5          ] = Key::Numpad5;  // Numeric keypad 5 key  
		irr2ceCODE[KEY_NUMPAD6          ] = Key::Numpad6;  // Numeric keypad 6 key  
		irr2ceCODE[KEY_NUMPAD7          ] = Key::Numpad7;  // Numeric keypad 7 key  
		irr2ceCODE[KEY_NUMPAD8          ] = Key::Numpad8;  // Numeric keypad 8 key  
		irr2ceCODE[KEY_NUMPAD9          ] = Key::Numpad9;  // Numeric keypad 9 key  
		irr2ceCODE[KEY_MULTIPLY         ] = Key::Multiply;  // Multiply key  
		irr2ceCODE[KEY_ADD              ] = Key::Add;  // Add key  
		irr2ceCODE[KEY_SEPARATOR        ] = 0;  // Separator key  
		irr2ceCODE[KEY_SUBTRACT         ] = Key::Subtract;  // Subtract key  
		irr2ceCODE[KEY_DECIMAL          ] = Key::Decimal;  // Decimal key  
		irr2ceCODE[KEY_DIVIDE           ] = Key::Divide;  // Divide key  
		irr2ceCODE[KEY_F1               ] = Key::F1;   // F1 key  
		irr2ceCODE[KEY_F2               ] = Key::F2;   // F2 key  
		irr2ceCODE[KEY_F3               ] = Key::F3;   // F3 key  
		irr2ceCODE[KEY_F4               ] = Key::F4;   // F4 key  
		irr2ceCODE[KEY_F5               ] = Key::F5;   // F5 key  
		irr2ceCODE[KEY_F6               ] = Key::F6;   // F6 key  
		irr2ceCODE[KEY_F7               ] = Key::F7;   // F7 key  
		irr2ceCODE[KEY_F8               ] = Key::F8;   // F8 key  
		irr2ceCODE[KEY_F9               ] = Key::F9;   // F9 key  
		irr2ceCODE[KEY_F10              ] = Key::F10;  // F10 key  
		irr2ceCODE[KEY_F11              ] = Key::F11;  // F11 key  
		irr2ceCODE[KEY_F12              ] = Key::F12;  // F12 key  
		irr2ceCODE[KEY_F13              ] = Key::F13;  // F13 key  
		irr2ceCODE[KEY_F14              ] = Key::F14;  // F14 key  
		irr2ceCODE[KEY_F15              ] = Key::F15;  // F15 key  
		irr2ceCODE[KEY_F16              ] = 0;  // F16 key  
		irr2ceCODE[KEY_F17              ] = 0;  // F17 key  
		irr2ceCODE[KEY_F18              ] = 0;  // F18 key  
		irr2ceCODE[KEY_F19              ] = 0;  // F19 key  
		irr2ceCODE[KEY_F20              ] = 0;  // F20 key  
		irr2ceCODE[KEY_F21              ] = 0;  // F21 key  
		irr2ceCODE[KEY_F22              ] = 0;  // F22 key  
		irr2ceCODE[KEY_F23              ] = 0;  // F23 key  
		irr2ceCODE[KEY_F24              ] = 0;  // F24 key  
		irr2ceCODE[KEY_NUMLOCK          ] = Key::NumLock;  // NUM LOCK key  
		irr2ceCODE[KEY_SCROLL           ] = Key::ScrollLock;  // SCROLL LOCK key  
		irr2ceCODE[KEY_LSHIFT           ] = Key::LeftShift;  // Left SHIFT key 
		irr2ceCODE[KEY_RSHIFT           ] = Key::RightShift;  // Right SHIFT key 
		irr2ceCODE[KEY_LCONTROL         ] = Key::LeftControl;  // Left CONTROL key 
		irr2ceCODE[KEY_RCONTROL         ] = Key::RightControl;  // Right CONTROL key 
		irr2ceCODE[KEY_LMENU            ] = Key::LeftAlt;  // Left MENU key 
		irr2ceCODE[KEY_RMENU            ] = Key::RightAlt;  // Right MENU key 
		irr2ceCODE[KEY_COMMA            ] = Key::Comma;//0xBC;  // Comma Key  (;)
		irr2ceCODE[KEY_PLUS             ] = Key::Add;  // Plus Key   (+)
		irr2ceCODE[KEY_MINUS            ] = Key::Minus;  // Minus Key  (-)
		irr2ceCODE[KEY_PERIOD           ] = Key::Period;//0xBE;  // Period Key (.)
		irr2ceCODE[KEY_ATTN             ] = 0;  // Attn key 
		irr2ceCODE[KEY_CRSEL            ] = 0;  // CrSel key 
		irr2ceCODE[KEY_EXSEL            ] = 0;  // ExSel key 
		irr2ceCODE[KEY_EREOF            ] = 0;  // Erase EOF key
		irr2ceCODE[KEY_PLAY             ] = 0;  // Play key 
		irr2ceCODE[KEY_ZOOM             ] = 0;  // Zoom key 
		irr2ceCODE[KEY_PA1              ] = 0;  // PA1 key 
		irr2ceCODE[KEY_OEM_CLEAR        ] = 0;  // Clear key 
	}
}
