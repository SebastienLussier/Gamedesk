/**
 *  @file       Memory.h
 *  @brief	    Memory related functions and defines.
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
#ifndef     _MEMORY_H_
#define     _MEMORY_H_

 
#define GD_ALLOC(ObjType, Count, pParent, pId) \
            (ObjType*)malloc(sizeof(ObjType) * Count)

#define GD_FREE(pMemory) \
            free(pMemory)

#define GD_NEW(ObjType, pParent, pId) \
            new ObjType

#define GD_NEW_ARRAY(ObjType, Count, pParent, pId) \
            new ObjType[Count]

#define GD_DELETE(pObj) \
            delete pObj

#define GD_DELETE_ARRAY(pArrayObj) \
            delete[] pArrayObj


#endif  //  _MEMORY_H_
