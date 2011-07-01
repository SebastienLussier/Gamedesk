/**
 *  @file       Module.h
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
#ifndef     _MODULE_H_
#define     _MODULE_H_


#include "Containers/DList.h"
#include "Exception/Exception.h"


namespace Gamedesk {


class ModuleManager;


/**
 *  A module class wich is assigned to a dll or library when running the engine.
 *  Look at the IMPLEMENT_MODULE() macro to see how this is done.
 *  @brief  Used to manage library and dll at runtime.
 *  @author Sebastien Lussier.
 *  @date   21/04/02.
 */
class CORE_API Module
{
    DECLARE_DLIST(Module);
    friend class ModuleManager;

    static const UInt32 MAX_NAME_LEN = 256;

public:
    /**
     *  Constructor wich takes the module name and a flag used to tell if this
     *  is a static module.
     *  @brief  Constructor.
     *  @param  pName     the name of the module.
     */
    Module( const Char* pName );

    //! Destructor.
    ~Module();

    //! Get the name of this module.
    const Char* GetName() const
    {
        return mName;
    }

    //! Get the handle of this module.
    Handle GetHandle() const
    {
        return mHandle;
    }

protected:
    Handle      mHandle;        //!< Handle to the dll or library instance.
    const Char* mName;          //!< Name of the module.
    Bool        mDynamicLoad;   //!< True if module was loaded dynamically.
};


class CORE_API ModuleNotFoundException : public Exception
{
    DECLARE_EXCEPTION(ModuleNotFoundException);

public:
    ModuleNotFoundException( const Char* pModuleName, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = "Module not found : ";
        mMessage += pModuleName;
        mMessage += ".";
        DebugOut();
    }
};


class CORE_API ModuleNotRegisteredException : public Exception
{
    DECLARE_EXCEPTION(ModuleNotRegisteredException);

public:
    ModuleNotRegisteredException( const Char* pModuleName, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = "Module was loaded but not registered. Have you forgot the IMPLEMENT_MODULE() macro for the module \"";
        mMessage += pModuleName;
        mMessage += "\" ?";
        DebugOut();
    }
};


} // namespace Gamedesk


#endif  //  _MODULE_H_
