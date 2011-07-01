/**
 *  @file       PropertyEditorTool.h
 *  @brief      Property editor used to edit the selection properties.
 *  @author     Sébastien Lussier.
 *  @date       29/02/04.
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
#ifndef     _PROPERTYEDITOR_TOOL_H_
#define     _PROPERTYEDITOR_TOOL_H_


#include "EditorTool.h"
#include "SelectionListener.h"


class PropertyEditorUI;


/**
 *  
 *  @author Sébastien Lussier.
 *  @date   29/02/04
 */
class PropertyEditorTool : public EditorTool, 
                           public SelectionListener
{
    DECLARE_CLASS(PropertyEditorTool,EditorTool);

public:
    PropertyEditorTool();
    virtual ~PropertyEditorTool();
    
    virtual void LoadSettings( ConfigSection& pSection );
    virtual void SaveSettings( ConfigSection& pSection ) const;

    virtual QWidget* CreateUI();
    virtual void DestroyUI();

    virtual void OnSelectionChanged(const std::list<Entity*>& pSelection);

private:
    PropertyEditorUI*   mPropertyEditorWindow;
};


#endif  //  _PROPERTYEDITOR_TOOL_H_
