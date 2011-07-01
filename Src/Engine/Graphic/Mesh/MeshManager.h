/**
 *  @file       MeshManager.h
 *  @brief	    
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
#ifndef     _MESH_MANAGER_H_
#define     _MESH_MANAGER_H_


namespace Gamedesk {


class Mesh;


class ENGINE_API MeshManager
{
    CLASS_DISABLE_COPY(MeshManager);

public:
    static MeshManager& Instance();

    Mesh* Create( const String& pMeshFile );
    
    void Release( const String& pMeshFile );
    
protected:
    // Disable creation from outside.
    MeshManager();

private:
    std::map<String,Mesh*>    mLoadedMeshs;
    
    static MeshManager        mInstance;
};


} // namespace Gamedesk


#endif  //  _MESH_MANAGER_H_
