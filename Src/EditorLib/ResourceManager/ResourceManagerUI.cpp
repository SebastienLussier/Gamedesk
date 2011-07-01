/**
 *  @file       ResourceManagerUI.cpp
 *  @brief      Manage resources visually.
 *  @author     Sebastien Lussier.
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
#include "EditorLib.h"
#include "ResourceManagerUI.h"
#include "ResourceManagerTool.h"

#include "World/Entity.h"

#include "..\EditorBase.h"
#include "..\WorldManager.h"


ResourceManagerUI::ResourceManagerUI( ResourceManagerTool* pTool )
    : mTool(pTool)
{
    setupUi(this);

    connect(mPackageTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnShowContextMenu(const QPoint&)));
}

ResourceManagerUI::~ResourceManagerUI()
{
}

void ResourceManagerUI::Init()
{
}

void ResourceManagerUI::Kill()
{
}

void ResourceManagerUI::OnShowContextMenu( const QPoint& pPoint )
{
    QMenu context(tr("Execution"), mPackageTree);
    QMenu menuAdd(tr("Add"), &context);
	
    menuAdd.addAction(tr("New Item..."));
    menuAdd.addAction(tr("Existing Item..."));
    menuAdd.addAction(tr("New Group..."));
    
    context.addMenu(&menuAdd);

    /*contextMenu.add
    contextMenu.addAction(tr("&New package"), this, SLOT(OnNewPackage()));
    contextMenu.addAction(tr("&Delete package"), this, SLOT(OnNewPackage()));
*/
    context.exec(mPackageTree->viewport()->mapToGlobal(pPoint));
}