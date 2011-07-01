/**
 *  @file       Subsystem.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       15/01/03.
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
#ifndef     _SUBSYSTEM_H_
#define     _SUBSYSTEM_H_


namespace Gamedesk {

class Manager;
class Resource;


class ENGINE_API Subsystem : public Object
{
    DECLARE_ABSTRACT_CLASS(Subsystem,Object);

public:
    Subsystem();
    virtual ~Subsystem();
    
    //! Subsystems act as factories.
    virtual Object* Create( Class* pResourceClass );
};


class ENGINE_API SubsystemNotLoadedException : public Exception
{
    DECLARE_EXCEPTION(SubsystemNotLoadedException);

public:
    SubsystemNotLoadedException( Class* pInClass, Class* pSubsystemClass, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = "Class \"";
        mMessage += pInClass->GetName();
        mMessage += "\" need to use \"";
        mMessage += pSubsystemClass->GetName();
        mMessage += "\" but it is not loaded!";
        DebugOut();
    }
};


} // namespace Gamedesk


#endif  //  _SUBSYSTEM_H_
