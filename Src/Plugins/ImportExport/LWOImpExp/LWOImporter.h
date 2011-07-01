/**
 *  @file       LWOImporter.h
 *  @brief      Lightwave model importer for Gamedesk.
 *  @author     Sébastien Lussier.
 *  @date       07/01/04.
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
#ifndef     _LWOIMPORTER_H_
#define     _LWOIMPORTER_H_


#include "Resource/ResourceManager.h"
#include "LWObject.h"


class LWOIMPEXP_API LWOImporter : public ResourceImporter
{
    DECLARE_CLASS(LWOImporter,ResourceImporter);

public:
    LWOImporter();

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );

private:
    void    ResolveVertexUV( LWObject& pObject );
    void    DuplicatePoints( LWObject& pObject, Vector<LWVertex>& pNewVertices );
    void    Triangulate( LWObject& pObject, Vector<LWTriangle>& pTriangles );
};


#endif  //  _LWOIMPORTER_H_
