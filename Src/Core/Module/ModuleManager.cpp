  /**
 *  @file       ModuleManager.cpp
 *  @brief      Manage access to dynamic modules.
 *  @author     Patrick Girard.
 *  @date       29/09/02.
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
#include "ModuleManager.h"
#include "Module.h"
#include "FileManager/FileManager.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(ModuleManager);

// Statics
ModuleManager        ModuleManager::mInstance;
DList::Root<Module>  ModuleManager::mModules;


ModuleManager::ModuleManager()
{
}

ModuleManager::~ModuleManager()
{
}

Bool ModuleManager::IsModuleLoaded( const Char* pModuleName ) const
{
    return FindModule(pModuleName) != 0;
}

const Module* ModuleManager::FindModule( const Char* pModuleName ) const
{
    GD_ASSERT(pModuleName);

    DList::Root<Module>::Iterator itModule(mModules);

    for( ; itModule.IsValid(); ++itModule )
    {
        if( strcmp((*itModule)->GetName(), pModuleName) == 0 )
            return *itModule;
    }

    return 0;
}

Class* ModuleManager::LoadClass( const Char* pClassName )
{
    GD_ASSERT(pClassName);

    Char          moduleName[Module::MAX_NAME_LEN];
    const Char*   className;

    const Char* posPoint = strchr(pClassName,'.');
    if( posPoint == 0 )
    {
        className = pClassName;
    }
    else
    {
        strncpy(moduleName, pClassName, posPoint - pClassName);
		moduleName[posPoint - pClassName] = 0;
        className = posPoint + 1;

        LoadModule(moduleName);
    }

    return Class::GetClassByName( className );
}

///////////////////////////////////////////////////////////////////////////////
//  RegisterModule
Bool ModuleManager::RegisterModule( Module* pModule )
{
    GD_ASSERT(pModule);

    if( IsModuleLoaded(pModule->GetName()) )
        return false;

    mModules.AddTail(*pModule);

    return true;
}

Bool ModuleManager::UnregisterModule( Module* pModule )
{
    GD_ASSERT(pModule);

    if( !IsModuleLoaded(pModule->GetName()) )
        return false;

    mModules.RemoveElement(*pModule);
    return true;
}

Bool ModuleManager::LoadModule( const Char* pModuleName, const Char* pModuleDir )
{
    GD_ASSERT(pModuleName);
    GD_ASSERT(pModuleDir);

    Char fullPath[260];

    if( IsModuleLoaded(pModuleName) )
        return true;

    strcpy(fullPath, pModuleDir);
	strcat(fullPath, pModuleName);

    Handle libHandle = Core::OpenLibrary( fullPath );
    if( !libHandle )
        throw ModuleNotFoundException( pModuleName, Here );

    Module* module = const_cast<Module*>(FindModule(pModuleName));
    if( !module )
        throw ModuleNotRegisteredException( pModuleName, Here );

    module->mHandle = libHandle;
    module->mDynamicLoad = true;

    return true;
}

void ModuleManager::LoadModulesInFolder( const Char* pModuleDir )
{
    Vector<String>              moduleNames;
    Vector<String>::iterator    itList;
    String                      moduleName;
    Char                        searchFor[256];
    
    sprintf( searchFor, "*%s", LIBRARY_EXTENSION );
    FileManager::FindFiles( pModuleDir, searchFor, moduleNames );

// If we're in release mode on windows, ignore debug dlls
#if !defined(GD_DEBUG) && defined(GD_WINDOWS)
    for( itList = moduleNames.begin(); itList != moduleNames.end(); )
    {
        if( (*itList).substr( (*itList).length() - strlen(LIBRARY_EXTENSION_DEBUG) ) == LIBRARY_EXTENSION_DEBUG )
            moduleNames.erase( itList );
        else
            ++itList;
    }
#endif

    // Remove LIBRARY_EXTENSION from the end
    // Call LoadModule on each item.
    for( itList = moduleNames.begin(); itList != moduleNames.end(); ++itList )
    {
        moduleName = (*itList).substr( 0, (*itList).length() - strlen(LIBRARY_EXTENSION) );
        LoadModule( moduleName.c_str(), pModuleDir );
    }
}


} // namespace Gamedesk





