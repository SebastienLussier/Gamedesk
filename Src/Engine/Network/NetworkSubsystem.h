/**
 *  @file       NetworkSubsystem.h
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
#ifndef     _NETWORK_SUBSYSTEM_H_
#define     _NETWORK_SUBSYSTEM_H_


#include "Subsystem/Subsystem.h"


namespace Gamedesk {


class ENGINE_API NetworkSubsystem : public Subsystem
{
    DECLARE_ABSTRACT_CLASS(NetworkSubsystem, Subsystem);

};


} // namespace Gamedesk


#endif  //  _NETWORK_SUBSYSTEM_H_
