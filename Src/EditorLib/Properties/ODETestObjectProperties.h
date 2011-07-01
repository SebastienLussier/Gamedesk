/**
 *  @file       ODETestObjectProperties.h
 *  @brief	    
 *  @author     S�bastien Lussier.
 *  @date       11/04/04.
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
#ifndef     _ODE_TEST_OBJECT_PROPERTIES_H_
#define     _ODE_TEST_OBJECT_PROPERTIES_H_


#include "EntityProperties.h"


class ODETestObject;


class EDITORLIB_API ODETestObjectProperties : public EntityProperties
{
    DECLARE_CLASS(ODETestObjectProperties, EntityProperties);

public:
    //! Constructor.
	ODETestObjectProperties();

    //! Destructor.
    virtual ~ODETestObjectProperties();

    //! Initialize all properties.
    virtual void InitProperties();
};


#endif  //  _ODE_TEST_OBJECT_PROPERTIES_H_