/**
 *  @file       MainViewerUI.h
 *  @brief      UI for the MainViewer.
 *  @author     Marco Arsenault.
 *  @date       30/03/04.
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
#ifndef     _MAINVIEWER_UI_H_
#define     _MAINVIEWER_UI_H_


#include "Qt/QMainViewer.h"


class MainViewer;
class Viewer;


class MainViewerUI : public QWidget, public Ui::QMainViewer
{
    Q_OBJECT

public:
    MainViewerUI( MainViewer* pTool );
	virtual ~MainViewerUI();

    void Init();
    void Kill();

	Viewer* GetViewer();
    
public slots:
    void OnRecord(bool bRecord);

private:
    MainViewerUI( const MainViewerUI& pOther );
    const MainViewerUI& operator = ( const MainViewerUI& pOther );

private:
	MainViewer*		mTool;
	Viewer*			mViewer;
};


#endif  //  _MAINVIEWER_UI_H_
