/**
 *  @file       Application.h
 *  @brief      Main application class.
 *  @author     Sébastien Lussier.
 *  @date       20/01/02.
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
#ifndef     _APPLICATION_H_
#define     _APPLICATION_H_


#include "Config/ConfigFile.h"
#include "Patterns/Singleton.h"
#include "Application/Window.h"


namespace Gamedesk {


class Window;
class RenderWindow;
class Subsystem;


/**
 *
 *  @brief
 *  @author Sébastien Lussier.
 *  @date   15/01/03.
 */
class ENGINE_API Application : public Object
{
    DECLARE_ABSTRACT_CLASS(Application, Object);
    DECLARE_ABSTRACT_SINGLETON(Application);

public:
    virtual ~Application();

    virtual void Launch( Int32 pArgCount, Char** pArgValues );
    Subsystem*   LoadSubsystem( Class* pSubsystemClass );
    
    Window*         GetWindow();
    RenderWindow*   GetRenderWindow();    

protected:
	Application(const Char* subsystemConfig);
    Application();

    virtual void Init();
    virtual void Run() = 0;
    virtual void Kill();

    virtual void SetupWindows() = 0;
    virtual void SetupSubsystems( ConfigFile& pConfigFile );
    
    void InitSingletons();
    void InitSubsystems();

    void KillSingletons();
    void KillSubsystems();

protected:
    Int32                   mArgCount;
    Char**                  mArgValues;

    ConfigFile              mSubsystemConfig;

    Window*                 mMainWindow;
    RenderWindow*           mRenderWindow;

    Vector<Subsystem*>      mSubsystemList;
};


} // namespace Gamedesk


#endif  //  _APPLICATION_H_
