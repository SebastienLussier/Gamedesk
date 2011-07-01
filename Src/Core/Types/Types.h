/**
 *  @file       Types.h
 *  @brief      Type definitions.
 */
/*
 *  Copyright (C) 2008 Gamedesk
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
#ifndef     _TYPES_SWITCH_H_
#define     _TYPES_SWITCH_H_


#if GD_PLATFORM == GD_PLATFORM_WIN32
    #include "Types/Win32/Types.h"                  // Win32
#elif GD_PLATFORM == GD_PLATFORM_PSP
    #include "Types/PSP/Types.h"                    // PSP
#elif GD_PLATFORM == GD_PLATFORM_LINUX
    #include "Types/Linux/Types.h"                  // Linux
#else
    #error "Unsupported platform!"
#endif


#endif  //  _TYPES_SWITCH_H_
