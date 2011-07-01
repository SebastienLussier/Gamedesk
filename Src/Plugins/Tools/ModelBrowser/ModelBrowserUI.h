/**
 *  @file       ModelBrowserUI.h
 *  @brief      UI for the Model Browser.
 *  @author     Sébastien Lussier.
 *  @date       10/02/04.
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
#ifndef     _MODELBROWSER_UI_H_
#define     _MODELBROWSER_UI_H_


#include "Qt/QModelBrowser.h"
#include "Graphic/Mesh.h"


class QRenderWindow;
class RenderListener;
class ModelBrowserTool;


class ModelBrowserUI : public QModelBrowser
{
public:
    ModelBrowserUI( QWidget* pParent, ModelBrowserTool* pTool );

    QRenderWindow*      GetRenderWindow();

private:
    void                LoadSelectedModel();
    void                InsertModelInWorld(Q3ListBoxItem* pItem);
    
private:
    ModelBrowserTool*   mTool;
    QRenderWindow*      mRenderWindow;
};


#endif  //  _MODELBROWSER_UI_H_
