/**
 *  @file       WorldUI.cpp
 *  @brief      UI for the World tool.
 *  @author     Sebastien Lussier.
 *  @date       17/10/06.
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
#include "EditorLib.h"
#include "WorldUI.h"
#include "WorldTool.h"

#include "World/Entity.h"

#include "..\EditorBase.h"
#include "..\WorldManager.h"


WorldUI::WorldUI( WorldTool* pTool )
    : mTool(pTool)
{
    setupUi(this);
}

WorldUI::~WorldUI()
{
}

void WorldUI::Init()
{
    FillEntitiesTree(Entity::StaticClass());
}

void WorldUI::Kill()
{
}

void WorldUI::SpawnClass(QTreeWidgetItem * item, int column)
{
    if( !item )
        return;

    Class* entityClass = Class::GetClassByName(item->text(0).toAscii().data());
    if(entityClass)
    {
        mTool->GetEditor()->GetWorldManager().SpawnEntity(entityClass);
    }
}

void WorldUI::FillEntitiesTree(Class* pCurrentClass, QTreeWidgetItem* pParentItem)
{
    QTreeWidgetItem* currentItem = 0;
    if( pParentItem ) 
        currentItem = new QTreeWidgetItem(pParentItem);
    else
        currentItem = new QTreeWidgetItem(mEntitiesTree);
    currentItem->setText(0, tr(pCurrentClass->GetName().c_str()));
    currentItem->setText(1, tr(pCurrentClass->GetName().c_str()));

    for(Class::Iterator itSubClass(pCurrentClass); itSubClass; ++itSubClass)
        FillEntitiesTree(*itSubClass, currentItem);
}
