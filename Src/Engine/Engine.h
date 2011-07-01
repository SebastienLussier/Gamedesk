 /**
 *  @file       Engine.h
 *  @brief	    Main engine module.
 *  @author     S�bastien Lussier.
 *  @date       25/08/02.
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
#ifndef     _ENGINE_H_
#define     _ENGINE_H_


#ifndef ENGINE_API
#define ENGINE_API DLL_IMPORT
#endif


#include "Core.h"


namespace Gamedesk 
{
    void StaticInitEngineClasses();
}


#endif  //  _ENGINE_H_
