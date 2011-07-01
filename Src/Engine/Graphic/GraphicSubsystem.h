/**
 *  @file       GraphicSubsystem.h
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
#ifndef     _GRAPHIC_SUBSYSTEM_H_
#define     _GRAPHIC_SUBSYSTEM_H_


#include "Subsystem/Subsystem.h"
#include "Exception/Exception.h"
#include "Patterns/Singleton.h"


namespace Gamedesk {


class Renderer;
class Resource;
class RenderWindow;
class Model;
class Texture;


class ENGINE_API GraphicSubsystem : public Subsystem
{
    DECLARE_ABSTRACT_CLASS(GraphicSubsystem, Subsystem);
    DECLARE_ABSTRACT_SINGLETON(GraphicSubsystem);

public:    
    virtual ~GraphicSubsystem();
       
    virtual void Init();
    virtual void Kill();

    Renderer* GetRenderer() const;
    
protected:
    GraphicSubsystem();

protected:
    Renderer*               mRenderer;
};


class ENGINE_API GraphicException : public Exception
{
    DECLARE_EXCEPTION(GraphicException);

public:
    GraphicException( const Char* pMessage, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = pMessage;
        mMessage += ".";
        DebugOut();
    }
};


} // namespace Gamedesk


#endif  //  _GRAPHIC_SUBSYSTEM_H_
