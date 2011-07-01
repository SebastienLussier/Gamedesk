    /**
 *  @file       DX9InputKeyTable.cpp
 *  @brief      DX9 Input Key code to Gamedesk key code table.
 *  @author     Sébastien Lussier.
 *  @date       05/01/04.
 */
/*
 *  Copyright (C) 2004 Gamedesk
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  Gamedesk
 *  http://gamedesk.type-cast.com
 *
 */
#include "DX9Input.h"
#include "DX9InputKeyTable.h"

#include "Input/Keyboard.h"


namespace Gamedesk {
	
	
Int32 gDInputToGamedeskKeyTable[DI_KEY_ARRAY_SIZE];


void InitDInputToGamedeskKeyTable()
{
    memset( gDInputToGamedeskKeyTable, -1, sizeof(gDInputToGamedeskKeyTable) );

    gDInputToGamedeskKeyTable[DIK_0]            = Keyboard::Key_0;              
    gDInputToGamedeskKeyTable[DIK_1]            = Keyboard::Key_1;
    gDInputToGamedeskKeyTable[DIK_2]            = Keyboard::Key_2;
    gDInputToGamedeskKeyTable[DIK_3]            = Keyboard::Key_3;
    gDInputToGamedeskKeyTable[DIK_4]            = Keyboard::Key_4;
    gDInputToGamedeskKeyTable[DIK_5]            = Keyboard::Key_5;
    gDInputToGamedeskKeyTable[DIK_6]            = Keyboard::Key_6;
    gDInputToGamedeskKeyTable[DIK_7]            = Keyboard::Key_7;
    gDInputToGamedeskKeyTable[DIK_8]            = Keyboard::Key_8;
    gDInputToGamedeskKeyTable[DIK_9]            = Keyboard::Key_9;
    gDInputToGamedeskKeyTable[DIK_A]            = Keyboard::Key_A;
    gDInputToGamedeskKeyTable[DIK_ABNT_C1]      = -1;
    gDInputToGamedeskKeyTable[DIK_ABNT_C2]      = -1;
    gDInputToGamedeskKeyTable[DIK_ADD]          = Keyboard::Key_NumAdd;
    gDInputToGamedeskKeyTable[DIK_APOSTROPHE]   = Keyboard::Key_Apostrophe;
    gDInputToGamedeskKeyTable[DIK_APPS]         = Keyboard::Key_Apps;
    gDInputToGamedeskKeyTable[DIK_AT]           = -1;
    gDInputToGamedeskKeyTable[DIK_AX]           = -1;
    gDInputToGamedeskKeyTable[DIK_B]            = Keyboard::Key_B;
    gDInputToGamedeskKeyTable[DIK_BACK]         = Keyboard::Key_Backspace;
    gDInputToGamedeskKeyTable[DIK_BACKSLASH]    = Keyboard::Key_Backslash;
    gDInputToGamedeskKeyTable[DIK_C]            = Keyboard::Key_C;
    gDInputToGamedeskKeyTable[DIK_CALCULATOR]   = -1;
    gDInputToGamedeskKeyTable[DIK_CAPITAL]      = Keyboard::Key_CapsLock;
    gDInputToGamedeskKeyTable[DIK_COLON]        = -1;
    gDInputToGamedeskKeyTable[DIK_COMMA]        = Keyboard::Key_Comma;
    gDInputToGamedeskKeyTable[DIK_CONVERT]      = -1;
    gDInputToGamedeskKeyTable[DIK_D]            = Keyboard::Key_D;
    gDInputToGamedeskKeyTable[DIK_DECIMAL]      = Keyboard::Key_NumPoint;
    gDInputToGamedeskKeyTable[DIK_DELETE]       = Keyboard::Key_Delete;
    gDInputToGamedeskKeyTable[DIK_DIVIDE]       = Keyboard::Key_NumDivide;
    gDInputToGamedeskKeyTable[DIK_DOWN]         = Keyboard::Key_DownArrow;
    gDInputToGamedeskKeyTable[DIK_E]            = Keyboard::Key_E;
    gDInputToGamedeskKeyTable[DIK_END]          = Keyboard::Key_End;
    gDInputToGamedeskKeyTable[DIK_EQUALS]       = Keyboard::Key_Equals;
    gDInputToGamedeskKeyTable[DIK_ESCAPE]       = Keyboard::Key_Escape;
    gDInputToGamedeskKeyTable[DIK_F]            = Keyboard::Key_F;
    gDInputToGamedeskKeyTable[DIK_F1]           = Keyboard::Key_F1;
    gDInputToGamedeskKeyTable[DIK_F2]           = Keyboard::Key_F2;
    gDInputToGamedeskKeyTable[DIK_F3]           = Keyboard::Key_F3;
    gDInputToGamedeskKeyTable[DIK_F4]           = Keyboard::Key_F4;
    gDInputToGamedeskKeyTable[DIK_F5]           = Keyboard::Key_F5;
    gDInputToGamedeskKeyTable[DIK_F6]           = Keyboard::Key_F6;
    gDInputToGamedeskKeyTable[DIK_F7]           = Keyboard::Key_F7;
    gDInputToGamedeskKeyTable[DIK_F8]           = Keyboard::Key_F8;
    gDInputToGamedeskKeyTable[DIK_F9]           = Keyboard::Key_F9;
    gDInputToGamedeskKeyTable[DIK_F10]          = Keyboard::Key_F10;
    gDInputToGamedeskKeyTable[DIK_F11]          = Keyboard::Key_F11;
    gDInputToGamedeskKeyTable[DIK_F12]          = Keyboard::Key_F12;
    gDInputToGamedeskKeyTable[DIK_F13]          = Keyboard::Key_F13;
    gDInputToGamedeskKeyTable[DIK_F14]          = Keyboard::Key_F14;
    gDInputToGamedeskKeyTable[DIK_F15]          = Keyboard::Key_F15;
    gDInputToGamedeskKeyTable[DIK_G]            = Keyboard::Key_G;
    gDInputToGamedeskKeyTable[DIK_GRAVE]        = Keyboard::Key_Grave;
    gDInputToGamedeskKeyTable[DIK_H]            = Keyboard::Key_H;
    gDInputToGamedeskKeyTable[DIK_HOME]         = Keyboard::Key_Home;
    gDInputToGamedeskKeyTable[DIK_I]            = Keyboard::Key_I;
    gDInputToGamedeskKeyTable[DIK_INSERT]       = Keyboard::Key_Insert;
    gDInputToGamedeskKeyTable[DIK_J]            = Keyboard::Key_J;
    gDInputToGamedeskKeyTable[DIK_K]            = Keyboard::Key_K;
    gDInputToGamedeskKeyTable[DIK_KANA]         = -1;
    gDInputToGamedeskKeyTable[DIK_KANJI]        = -1;
    gDInputToGamedeskKeyTable[DIK_L]            = Keyboard::Key_L;
    gDInputToGamedeskKeyTable[DIK_LBRACKET]     = Keyboard::Key_LeftBracket;
    gDInputToGamedeskKeyTable[DIK_LCONTROL]     = Keyboard::Key_LeftCtrl;
    gDInputToGamedeskKeyTable[DIK_LEFT]         = Keyboard::Key_LeftArrow;
    gDInputToGamedeskKeyTable[DIK_LMENU]        = Keyboard::Key_LeftAlt;
    gDInputToGamedeskKeyTable[DIK_LSHIFT]       = Keyboard::Key_LeftShift;
    gDInputToGamedeskKeyTable[DIK_LWIN]         = Keyboard::Key_LeftWin;
    gDInputToGamedeskKeyTable[DIK_M]            = Keyboard::Key_M;
    gDInputToGamedeskKeyTable[DIK_MAIL]         = -1;
    gDInputToGamedeskKeyTable[DIK_MEDIASELECT]  = -1;
    gDInputToGamedeskKeyTable[DIK_MEDIASTOP]    = -1;
    gDInputToGamedeskKeyTable[DIK_MINUS]        = Keyboard::Key_Minus;
    gDInputToGamedeskKeyTable[DIK_MULTIPLY]     = Keyboard::Key_NumMultiply;
    gDInputToGamedeskKeyTable[DIK_MUTE]         = -1;
    gDInputToGamedeskKeyTable[DIK_MYCOMPUTER]   = -1;
    gDInputToGamedeskKeyTable[DIK_N]            = Keyboard::Key_N;
    gDInputToGamedeskKeyTable[DIK_NEXT]         = Keyboard::Key_PageDown;
    gDInputToGamedeskKeyTable[DIK_NEXTTRACK]    = -1;
    gDInputToGamedeskKeyTable[DIK_NOCONVERT]    = -1;
    gDInputToGamedeskKeyTable[DIK_NUMLOCK]      = Keyboard::Key_NumLock;
    gDInputToGamedeskKeyTable[DIK_NUMPAD0]      = Keyboard::Key_NumPad0;
    gDInputToGamedeskKeyTable[DIK_NUMPAD1]      = Keyboard::Key_NumPad1;
    gDInputToGamedeskKeyTable[DIK_NUMPAD2]      = Keyboard::Key_NumPad2;
    gDInputToGamedeskKeyTable[DIK_NUMPAD3]      = Keyboard::Key_NumPad3;
    gDInputToGamedeskKeyTable[DIK_NUMPAD4]      = Keyboard::Key_NumPad4;
    gDInputToGamedeskKeyTable[DIK_NUMPAD5]      = Keyboard::Key_NumPad5;
    gDInputToGamedeskKeyTable[DIK_NUMPAD6]      = Keyboard::Key_NumPad6;
    gDInputToGamedeskKeyTable[DIK_NUMPAD7]      = Keyboard::Key_NumPad7;
    gDInputToGamedeskKeyTable[DIK_NUMPAD8]      = Keyboard::Key_NumPad8;
    gDInputToGamedeskKeyTable[DIK_NUMPAD9]      = Keyboard::Key_NumPad9;
    gDInputToGamedeskKeyTable[DIK_NUMPADCOMMA]  = -1;
    gDInputToGamedeskKeyTable[DIK_NUMPADENTER]  = Keyboard::Key_NumPadEnter;
    gDInputToGamedeskKeyTable[DIK_NUMPADEQUALS] = -1;
    gDInputToGamedeskKeyTable[DIK_O]            = Keyboard::Key_O;
    gDInputToGamedeskKeyTable[DIK_OEM_102]      = -1;
    gDInputToGamedeskKeyTable[DIK_P]            = Keyboard::Key_P;
    gDInputToGamedeskKeyTable[DIK_PAUSE]        = Keyboard::Key_Pause;
    gDInputToGamedeskKeyTable[DIK_PERIOD]       = Keyboard::Key_Period;
    gDInputToGamedeskKeyTable[DIK_PLAYPAUSE]    = -1;
    gDInputToGamedeskKeyTable[DIK_POWER]        = -1;
    gDInputToGamedeskKeyTable[DIK_PREVTRACK]    = -1;
    gDInputToGamedeskKeyTable[DIK_PRIOR]        = Keyboard::Key_PageUp;
    gDInputToGamedeskKeyTable[DIK_Q]            = Keyboard::Key_Q;
    gDInputToGamedeskKeyTable[DIK_R]            = Keyboard::Key_R;
    gDInputToGamedeskKeyTable[DIK_RBRACKET]     = Keyboard::Key_RightBracket;
    gDInputToGamedeskKeyTable[DIK_RCONTROL]     = Keyboard::Key_RightCtrl;
    gDInputToGamedeskKeyTable[DIK_RETURN]       = Keyboard::Key_Enter;
    gDInputToGamedeskKeyTable[DIK_RIGHT]        = Keyboard::Key_RightArrow;
    gDInputToGamedeskKeyTable[DIK_RMENU]        = Keyboard::Key_RightAlt;
    gDInputToGamedeskKeyTable[DIK_RSHIFT]       = Keyboard::Key_RightShift;
    gDInputToGamedeskKeyTable[DIK_RWIN]         = Keyboard::Key_RightWin;
    gDInputToGamedeskKeyTable[DIK_S]            = Keyboard::Key_S;
    gDInputToGamedeskKeyTable[DIK_SCROLL]       = Keyboard::Key_ScrollLock;
    gDInputToGamedeskKeyTable[DIK_SEMICOLON]    = Keyboard::Key_Semicolon;
    gDInputToGamedeskKeyTable[DIK_SLASH]        = Keyboard::Key_Slash;
    gDInputToGamedeskKeyTable[DIK_SLEEP]        = -1;
    gDInputToGamedeskKeyTable[DIK_SPACE]        = Keyboard::Key_Spacebar;
    gDInputToGamedeskKeyTable[DIK_STOP]         = -1;
    gDInputToGamedeskKeyTable[DIK_SUBTRACT]     = Keyboard::Key_NumSubtract;
    gDInputToGamedeskKeyTable[DIK_SYSRQ]        = Keyboard::Key_PrintScreen;
    gDInputToGamedeskKeyTable[DIK_T]            = Keyboard::Key_T;
    gDInputToGamedeskKeyTable[DIK_TAB]          = Keyboard::Key_Tab;
    gDInputToGamedeskKeyTable[DIK_U]            = Keyboard::Key_U;
    gDInputToGamedeskKeyTable[DIK_UNDERLINE]    = -1;
    gDInputToGamedeskKeyTable[DIK_UNLABELED]    = -1;
    gDInputToGamedeskKeyTable[DIK_UP]           = Keyboard::Key_UpArrow;
    gDInputToGamedeskKeyTable[DIK_V]            = Keyboard::Key_V;
    gDInputToGamedeskKeyTable[DIK_VOLUMEDOWN]   = -1;
    gDInputToGamedeskKeyTable[DIK_VOLUMEUP]     = -1;
    gDInputToGamedeskKeyTable[DIK_W]            = Keyboard::Key_W;
    gDInputToGamedeskKeyTable[DIK_WAKE]         = -1;
    gDInputToGamedeskKeyTable[DIK_WEBBACK]      = -1;
    gDInputToGamedeskKeyTable[DIK_WEBFAVORITES] = -1;
    gDInputToGamedeskKeyTable[DIK_WEBFORWARD]   = -1;
    gDInputToGamedeskKeyTable[DIK_WEBHOME]      = -1;
    gDInputToGamedeskKeyTable[DIK_WEBREFRESH]   = -1;
    gDInputToGamedeskKeyTable[DIK_WEBSEARCH]    = -1;
    gDInputToGamedeskKeyTable[DIK_WEBSTOP]      = -1;
    gDInputToGamedeskKeyTable[DIK_X]            = Keyboard::Key_X;
    gDInputToGamedeskKeyTable[DIK_Y]            = Keyboard::Key_Y;
    gDInputToGamedeskKeyTable[DIK_YEN]          = -1;
    gDInputToGamedeskKeyTable[DIK_Z]            = Keyboard::Key_Z;
}


} // namespace Gamedesk
