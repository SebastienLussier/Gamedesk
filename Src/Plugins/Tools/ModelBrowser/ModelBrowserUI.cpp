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
#include "ModelBrowserDll.h"
#include "ModelBrowserUI.h"
#include "ModelBrowserTool.h"

#include "QRenderWindow.h"
//Added by qt3to4:
#include <QGridLayout>

#include "EditorBase.h"
#include "WorldManager.h"

#include "World/Model3D.h"
#include "World/Camera.h"
#include "Properties/CameraProperties.h"


ModelBrowserUI::ModelBrowserUI( QWidget* pParent, ModelBrowserTool* pTool ) :
    QModelBrowser( pParent, 0, Qt::WStyle_Tool ),
    mTool(pTool),
    mRenderWindow(NULL)
{
    QGridLayout* layout = new QGridLayout( mViewerFrame );
    mRenderWindow = new QRenderWindow( mViewerFrame, mTool );
    
    layout->addWidget( mRenderWindow, 0, 0 );

    // Let the ratio between the height of row 0 (the viewer) and 3 (the tab panel) be 2:3.
    QModelBrowserLayout->setRowStretch( 0, 2 );
    QModelBrowserLayout->setRowStretch( 3, 3 );
    
    mRenderWindow->SetRefreshRate( 60 );

    QDir dir("Data/Meshes/");
    dir.setNameFilter("*.mdl *.lwo *.md5mesh *.proc");
    QStringList strList( dir.entryList(QDir::Files) );

    mModelList->clear();
    mModelList->insertStringList( strList );
}

void ModelBrowserUI::LoadSelectedModel()
{
    if( !mModelList->currentText().isEmpty() )
        mTool->SetViewedModel( String("Data/Meshes/") + String(mModelList->currentText().ascii()) );
}

void ModelBrowserUI::InsertModelInWorld( Q3ListBoxItem* pItem )
{
    Camera* camera = mTool->GetEditor()->GetWorldManager().GetCurrentCamera();
    Vector3f  pos = camera->GetPosition() + (camera->GetView() * 2.0f);

    Model3D* newModel = Cast<Model3D>( mTool->GetEditor()->GetWorldManager().SpawnEntity( Model3D::StaticClass(), pos ) );
    newModel->SetMesh( String("Data/Meshes/") + String(pItem->text().ascii()) );
}

QRenderWindow* ModelBrowserUI::GetRenderWindow()
{
    return mRenderWindow;
}
