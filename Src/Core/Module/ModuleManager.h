/**
 *  @file       ModuleManager.h
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
#ifndef     _MODULE_MANAGER_H_
#define     _MODULE_MANAGER_H_


#include "Containers/DList.h"
#include "Module/Module.h"


namespace Gamedesk {


/**
 *  This class is responsible for loading, freeing and querying modules.
 *  You make request for a certain module to this manager, and if it's
 *  not loaded yet, it will look for it and load it.
 *  @brief  Load and manage all the modules.
 *  @author Patrick Girard.
 *  @date   29/09/02.
 */
class CORE_API ModuleManager : public Object
{
    DECLARE_CLASS(ModuleManager, Object);

public:
    /**
     *  Destructor.
     *  @brief  Destructor.
     */
    virtual ~ModuleManager();

    /**
     *  Access the ModuleManager singleton.
     *  @brief  Singleton access method.
     *  @return the module manager singleton.
     */
    static ModuleManager* Instance()
    {
        return &mInstance;
    }

    /**
     *  Check if module is loaded.
     *  @brief  Check if module is loaded.
     *  @param  pModuleName     the module name.
     *  @return \b True if module is loaded, \b false otherwise.
     */
    Bool IsModuleLoaded( const Char* pModuleName ) const;

    /**
     *  Search for a module using its name.
     *  @param  pModuleName     the module name.
     *  @return a pointer to the module, or 0 if none was found.
     */
    const Module* FindModule( const Char* pModuleName ) const;

    /**
     *  Load a class, by specifing it's module and name.
     *  If the module is not loaded, the manager will try to find it
     *  and load it automatically.
     *  @brief  Load a class by it's module and name (module.name).
     *  @param  pClassName      the class name.
     *  @return The Class object if found,
     */
    Class* LoadClass( const Char* pClassName );

    /**
     *  Loads a Module.
     *  @brief  Loads a Module.
     *  @param  pModuleName         the module name.
     *  @param  pModuleDir      directory containing this module.
     *  @return \True if the module was loaded, false otherwise.
     */
    Bool LoadModule( const Char* pModuleName, const Char* pModuleDir = "" );

    /**
     *  Register a module to the module manager.
     *  @brief  Add a module to module list.
     *  @param  pModule         Pointer to the module.
     *  @return \b True if the module was registered succesfully, \b false otherwise.
     */
    Bool RegisterModule( Module* pModule );

    /**
     *  Unregister a module.
     *  @brief  Remove a module from the module list.
     *  @param  pModuleName     Pointer to the module.
     *  @return \b True if the module was unregistered succesfully, \b false otherwise.
     */
    Bool UnregisterModule( Module* pModule );

    /**
     *  Load all modules present in the given directory.
     *  @brief  Load all modules in a folder.
     *  @param  pModuleDir  Directory where to load the modules.
     */
    void LoadModulesInFolder( const Char* pModuleDir );

private:

    /**
     *  Constructor.
     *  @brief  Constructor.
     */
    ModuleManager();

private:
    static DList::Root<Module>   mModules;      //!< List of all loaded modules.
    static ModuleManager         mInstance;     //!< Singleton instance.
};


/**
 *  This macro should be used once per module, by preference in the
 *  cpp file that has the same name as the module.  It will register
 *  the module automatically when the DLL or library is loaded.
 *  @brief Automatically register a module when it is loaded.
 *  @param  moduleName  a string, the module name.
 */
#define IMPLEMENT_MODULE( moduleName ) \
            static Module s##moduleName##Instance(#moduleName)


} // namespace Gamedesk


#endif  //  _MODULE_MANAGER_H_
