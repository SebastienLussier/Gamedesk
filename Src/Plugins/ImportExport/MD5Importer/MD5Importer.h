/**
 *  @file       MD5Importer.h
 *  @brief      Doom 3 MD5 mesh/anim importer.
 *  @author     Sébastien Lussier.
 *  @date       12/08/04.
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
#ifndef     _MD5IMPORTER_H_
#define     _MD5IMPORTER_H_


#include "Resource/ResourceManager.h"

#include "MD5Mesh.h"

class SkeletalMesh;
class SkeletalAnim;


class MD5IMPORTER_API MD5Importer : public ResourceImporter
{
    DECLARE_CLASS(MD5Importer,ResourceImporter);
    
public:
    MD5Importer();

    SkeletalMesh* CreateSkeletalMesh( MD5::MeshFile& meshFile );
    SkeletalAnim* CreateAnim( MD5::AnimFile& animFile );

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );

    String mDoomFolder;
};


class MD5IMPORTER_API ProcImporter : public ResourceImporter
{
    DECLARE_CLASS(ProcImporter,ResourceImporter);
    
public:
    ProcImporter();

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );

    String mDoomFolder;
};


#endif  //  _MD5IMPORTER_H_
