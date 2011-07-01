/**
 *  @file       MeshHdl.h
 *  @brief	    Managed handle to a mesh.
 *  @author     Sébastien Lussier.
 *  @date       29/01/04.
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
#ifndef     _MESH_HDL_H_
#define     _MESH_HDL_H_

#include "Graphic/Mesh/MeshManager.h"


namespace Gamedesk {


class Mesh;


class ENGINE_API MeshHdl
{
public:
    MeshHdl();
    MeshHdl( const String& mMeshFile );
    MeshHdl( const MeshHdl& pOther );

    ~MeshHdl();

    INLINE MeshHdl& operator = ( const MeshHdl& pOther );
    INLINE Bool operator == ( const MeshHdl& pOther );
    INLINE Bool operator != ( const MeshHdl& pOther );
    INLINE Mesh* operator -> () const;
    INLINE Mesh* operator * () const;
    INLINE operator Bool () const;

    INLINE void GetMesh( const String& mMeshFile );

private:
    String      mMeshFile;
    Mesh*       mMesh;
};

INLINE MeshHdl::MeshHdl() : mMeshFile(), mMesh(NULL)
{
}

INLINE MeshHdl::MeshHdl( const String& pMeshFile ) : mMesh(NULL)
{
    GetMesh( pMeshFile );
}

INLINE MeshHdl::MeshHdl( const MeshHdl& pOther ) : mMesh(NULL)
{
    if( pOther )
        GetMesh( pOther.mMeshFile );
}

INLINE MeshHdl::~MeshHdl()
{
    if( mMesh )
        MeshManager::Instance().Release( mMeshFile );
}

INLINE MeshHdl& MeshHdl::operator = ( const MeshHdl& pOther )
{
    if( *this == pOther || mMesh == pOther.mMesh )
        return *this;

    GetMesh( pOther.mMeshFile );
    return *this;
}

INLINE Bool MeshHdl::operator == ( const MeshHdl& pOther )
{
    return mMesh == pOther.mMesh;
}

INLINE Bool MeshHdl::operator != ( const MeshHdl& pOther )
{
    return mMesh != pOther.mMesh;
}

INLINE Mesh* MeshHdl::operator -> () const
{
    GD_ASSERT( mMesh != NULL );
    return mMesh;
}

Mesh* MeshHdl::operator * () const
{
    GD_ASSERT( mMesh != NULL );
    return mMesh;
}

INLINE MeshHdl::operator Bool () const
{
    return mMesh != NULL;
}

INLINE void MeshHdl::GetMesh( const String& pMeshFile )
{
    try
    {
        if( mMesh )
        {
            MeshManager::Instance().Release( mMeshFile );
            mMesh = NULL;
        }

        mMeshFile = pMeshFile;
        mMesh = MeshManager::Instance().Create( mMeshFile );
    }
    catch( Exception& /*e*/ )
    {
        mMesh = NULL;
    }
}


} // namespace Gamedesk


#endif  //  _MESH_HDL_H_
