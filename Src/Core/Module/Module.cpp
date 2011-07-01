/**
 *  @file       Module.cpp
 *  @brief      Class used to handle dynamic pluggable modules.
 *  @author     Sebastien Lussier.
 *  @date       21/04/02.
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
#include "Core.h"
#include "Module.h"
#include "ModuleManager.h"


namespace Gamedesk {
	
	
Module::Module( const Char* pName )
    : mHandle(0)
    , mName(pName)
    , mDynamicLoad(false)
{
    ModuleManager::Instance()->RegisterModule( this );
}

Module::~Module()
{
    ModuleManager::Instance()->UnregisterModule( this );
    
    if( mDynamicLoad && mHandle != 0 )
        Core::CloseLibrary( mHandle );
}


} // namespace Gamedesk
