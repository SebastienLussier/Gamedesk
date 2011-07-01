 /**
 *  @file       EntityProperties.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       14/04/04.
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
#include "EditorLib.h"
#include "EntityProperties.h"
#include "World/Entity.h"


ImplementProperty( EntityProperties, mPosition, Vector3f, GetPosition, SetPosition );
ImplementProperty( EntityProperties, mOrientation, Quaternionf, GetOrientation, SetOrientation );


EntityProperties::EntityProperties()
{    
}

EntityProperties::~EntityProperties()
{
}

void EntityProperties::InitProperties()
{
    Super::InitProperties();

    RegisterProperty( EntityProperties, "Position", mPosition );
    RegisterProperty( EntityProperties, "Orientation", mOrientation );
}

void EntityProperties::SetPosition(const Vector3f& pPosition)
{
	Cast<Entity>( mObject )->SetPosition(pPosition);
    FirePropertyChanged(&mPosition);
}

const Vector3f& EntityProperties::GetPosition() const
{
    return Cast<Entity>( mObject )->GetPosition();
}

void EntityProperties::SetOrientation(const Quaternionf& pOrientation)
{
	Cast<Entity>( mObject )->SetOrientation(pOrientation);
    FirePropertyChanged(&mOrientation);
}

const Quaternionf& EntityProperties::GetOrientation() const
{
    return Cast<Entity>( mObject )->GetOrientation();
}


IMPLEMENT_CLASS(EntityProperties);
