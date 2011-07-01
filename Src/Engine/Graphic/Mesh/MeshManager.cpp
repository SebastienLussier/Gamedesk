/**
 *  @file       MeshManager.cpp
 *  @brief	    Manage access to Meshs.
 *  @author     Sébastien Lussier.
 *  @date       15/01/03.
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
#include "Engine.h"
#include "MeshManager.h"

#include "Mesh.h"
#include "Resource/ResourceManager.h"


namespace Gamedesk {
	
	
MeshManager MeshManager::mInstance;
MeshManager& MeshManager::Instance()
{
    return mInstance;
}


MeshManager::MeshManager()
{
}


Mesh* MeshManager::Create( const String& pMeshFile )
{
    std::map<String,Mesh*>::iterator  itFind = mLoadedMeshs.find( pMeshFile );
    if( itFind != mLoadedMeshs.end() )
    {
        itFind->second->AddRef();
        return itFind->second;
    }
    else
    {
        ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( pMeshFile, Mesh::StaticClass() );
        Mesh* mesh = Cast<Mesh>(importer->Import( pMeshFile ));
        mesh->Init();
        mesh->AddRef();
        mLoadedMeshs[pMeshFile] = mesh;
        return mesh;
    }        
}

void MeshManager::Release( const String& pFontFile )
{
    std::map<String,Mesh*>::iterator  itFind = mLoadedMeshs.find( pFontFile );
    GD_ASSERT( itFind != mLoadedMeshs.end() );
            
    if( itFind->second->RemoveRef() )
    {
        itFind->second->Kill();
        GD_DELETE(itFind->second);

        mLoadedMeshs.erase( itFind );
    }
}


} // namespace Gamedesk
