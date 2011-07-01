/**
 *  @file       WorldUI.h
 *  @brief      UI for the World tool
 *  @author     Sebastien Lussier
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
#ifndef     _WORLD_UI_H_
#define     _WORLD_UI_H_


#include "ui_WorldUI.h"

class WorldTool;
class Class;

class EDITORLIB_API WorldUI : public QWidget, public Ui::WorldUI
{
    Q_OBJECT

public:
    WorldUI( WorldTool* pTool );
	virtual ~WorldUI();

    void Init();
    void Kill();

public slots:
    void SpawnClass(QTreeWidgetItem * item, int column);

private:
    WorldUI( const WorldUI& pOther );
    const WorldUI& operator = ( const WorldUI& pOther );

    void FillEntitiesTree(Class* pCurrentClass, QTreeWidgetItem* pParentItem = 0);

private:
	WorldTool*		mTool;
};


#endif  //  _WORLD_UI_H_
