/**
 *  @file       Material.h
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
#ifndef     _MATERIAL_H_
#define     _MATERIAL_H_


#include "Graphic/Color4.h"


namespace Gamedesk {


class ENGINE_API Material
{
public:
    Material() :
        mDiffuse(0.8f,0.8f,0.8f,1.0f),
        mSpecular(0.0f,0.0f,0.0f,1.0f),
        mAmbient(0.2f,0.2f,0.2f,1.0f),
        mEmissive(0.0f,0.0f,0.0f,1.0f),
        mShininess(0.0f)
    {
    }

    Color4f     mDiffuse;
    Color4f     mSpecular;
    Color4f     mAmbient;
    Color4f     mEmissive;
    Float       mShininess;
};


} // namespace Gamedesk


#endif  //  _MATERIAL_H_
