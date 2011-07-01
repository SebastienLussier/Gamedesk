/**
 *  @file       MDLMeshImporter.h
 *  @brief      Neverwinter Nights model importer (.mdl).
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
#ifndef     _MDLMESHIMPORTER_H_
#define     _MDLMESHIMPORTER_H_


#include "Resource/ResourceManager.h"
#include "MDLModel.h"


class Mesh;
class NWN::GeometryNode;
class NWN::TrimeshGeometryNode;


/**
 *  Neverwinter Nights mesh importer.
 *  Based on this project : http://sourceforge.net/projects/nwn-j3d/ 
 */
class MDLIMPORTER_API MDLMeshImporter : public ResourceImporter
{
    DECLARE_CLASS(MDLMeshImporter,ResourceImporter);
public:
    MDLMeshImporter();

    virtual Class*    GetResourceClass();

    /**
     *  Read the given MDL file and return a new Mesh object.
     */
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );

private:
    /**
     *  Create a new Mesh from a given geometry node (recursivly).
     */
    Mesh*   CreateMesh( NWN::GeometryNode* pNode, Mesh* pParent, Bool pMetallic );

    /**
     *  Create the geometry for a Mesh.
     */
    void    CreateGeometry( NWN::TrimeshGeometryNode* pTrimesh, Mesh* pMesh );

    /**
     *  Create the shader associated with a Mesh.
     */
    void    CreateShader( NWN::TrimeshGeometryNode* pTrimesh, Mesh* pMesh, Bool pMetallic );
};


#endif  //  _MDLMESHIMPORTER_H_
