/**
 *  @file       ModelBrowserUI.cpp
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
#include "MainViewerDll.h"
#include "MainViewerUI.h"
#include "MainViewer.h"
#include "Viewer.h"
#include "AVIEncoder.h"


MainViewerUI::MainViewerUI( MainViewer* pTool ) :
	mTool(pTool),
    mViewer(NULL)
{
    setupUi(this);    

    QGridLayout* layout = new QGridLayout( mViewerFrame );
    mViewer = new Viewer( mViewerFrame, mTool );
        
    layout->addWidget( mViewer, 0, 0 );
    layout->setMargin(0);
    layout->setSpacing(0);

    // Connect the record button with OnRecord()...
    QObject::connect(mBtnRecord, SIGNAL(toggled(bool)), this, SLOT(OnRecord(bool)));
}

MainViewerUI::~MainViewerUI()
{
}

void MainViewerUI::Init()
{
    mViewer->Init();
}

void MainViewerUI::Kill()
{
    mViewer->Kill();
}

Viewer* MainViewerUI::GetViewer()
{
    return mViewer;
}

void MainViewerUI::OnRecord(bool bRecord)
{
    if( bRecord )
    {
        QString fileName = QFileDialog::getSaveFileName( this, "Choose a file", QString(), "AVI files (*.avi)" );
        if( !fileName.isNull() )
        {        
            Int32 iwidth  = mViewer->width();
            Int32 iheight = mViewer->height();
            Int32 diffW = 0;
            Int32 diffH = 0;

            // Make sure new resolution is a multiple of 4
            if( (iwidth-2) % 4 != 0 )
                diffW -= (iwidth-2) % 4;
            
            if( (iheight-2) % 4 != 0 )
                diffH -= (iheight-2) % 4;

            if( diffW != 0 || diffH != 0 )
            {
                // Smallest change possible
                if( diffW == -3 ) diffW = 1;
                if( diffH == -3 ) diffH = 1;

                resize( QSize(width()+diffW,height()+diffH) );
                mViewer->resize( QSize(iwidth+diffW,iheight+diffH) );
            }

            if( !mViewer->StartMovie( (const char*)fileName.toAscii() ) )
                mBtnRecord->setChecked( false );
        }
        else
        {
            mBtnRecord->setChecked( false );
        }
    }
    else
    {
        mViewer->StopMovie();
    }
}