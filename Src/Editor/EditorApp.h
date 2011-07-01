/**
 *  @file       EditorApp.h
 *  @brief      Gamedesk editor application.
 *  @author     Sébastien Lussier.
 *  @date       06/01/04.
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
#ifndef     _EDITOR_APP_H_
#define     _EDITOR_APP_H_


#include "Application/Application.h"


class GDEditor;


class EditorApp : public Application
{
    DECLARE_CLASS(EditorApp,Application);

public:
    EditorApp();

    virtual void Init();

    virtual void Kill();

    virtual void SetupWindows();
    virtual void SetupSubsystems( ConfigFile& pConfigFile );

    virtual void Run();
    
    virtual Handle GetWinHandle() const;

protected:
    QApplication*   mQtApp;  
    GDEditor*       mEditor; 
};


#endif  //  _EDITOR_APP_H_
