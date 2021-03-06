/**
 *  @file       ResourceManagerTool.h
 *  @brief      Manage resources visually.
 *  @author     Sébastien Lussier.
 *  @date       23/10/06.
 */
/*
 *  Copyright (C) 2006 Gamedesk
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
#ifndef     _WORLD_TOOL_H_
#define     _WORLD_TOOL_H_


#include "..\EditorTool.h"


class EDITORLIB_API ResourceManagerTool : public EditorTool
{
    DECLARE_CLASS(ResourceManagerTool,EditorTool);

public:
    ResourceManagerTool();
    virtual ~ResourceManagerTool();

    virtual void Init();
    virtual void Kill();
    
    virtual void LoadSettings( ConfigSection& pSection );
    virtual void SaveSettings( ConfigSection& pSection ) const;

    virtual QWidget* CreateUI();
    virtual void DestroyUI();

private:
    class ResourceManagerUI* mManagerUI;
};


#endif  //  _WORLD_TOOL_H_
