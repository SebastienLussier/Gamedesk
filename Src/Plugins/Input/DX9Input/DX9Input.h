/**
 *  @file       DX9Input.h
 *  @brief      DX9 Input module for gamedesk.
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
#ifndef     _DX9INPUT_H_
#define     _DX9INPUT_H_


#ifndef DX9INPUT_API
#define DX9INPUT_API DLL_IMPORT
#endif


#define DIRECTINPUT_VERSION         0x0800

#include "Engine.h"

#include <dinput.h>
#include <dxerr.h>


namespace Gamedesk {


// DLL Instance is needed to initialize DirectInput
extern HINSTANCE gDLLInstance;


} // namespace Gamedesk


#endif  //  _DX9INPUT_H_
