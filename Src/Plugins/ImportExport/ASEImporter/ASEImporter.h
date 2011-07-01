/**
 *  @file       ASEImporter.h
 *  @brief      ASE importer.
 *  @author     Sébastien Lussier.
 *  @date       03/12/10.
 */
/*
 *  Copyright (C) 2010 Gamedesk
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
#ifndef     _ASEIMPORTER_H_
#define     _ASEIMPORTER_H_


#include "Resource/ResourceManager.h"
#include "ASEFile.h"

class ASEIMPORTER_API ASEImporter : public ResourceImporter
{
    DECLARE_CLASS(ASEImporter, ResourceImporter);
    
public:
    ASEImporter();

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );

private:
    struct ASEVertex
    {
        Bool operator == ( const ASEVertex& pOther ) const
        {
            return mPosition.EqualEps( pOther.mPosition ) && mNormal.EqualEps( pOther.mNormal ) && mUV.EqualEps( pOther.mUV );
        }

        Vector3f mPosition;
        Vector3f mNormal;
        Vector2f mUV;
    };
    
    struct ASESubMesh
    {
        Vector<ASEVertex> mVertices;
        Vector<UInt32>    mIndices;
    };

private:
    void GenerateSubMeshes(const ASEFile& pASEFile, Map<const ASEFile::Material*, ASESubMesh>& pSubMeshes);
};


#endif  //  _ASEIMPORTER_H_
