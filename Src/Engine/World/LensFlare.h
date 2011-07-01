/**
 *  @file       LensFlare.h
 *  @brief	    LensFlare effect.
 *  @author     Sébastien Lussier
 *  @date       04/05/04.
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
#ifndef     _LENS_FLARE_H_
#define     _LENS_FLARE_H_


#include "Maths/Vector3.h"
#include "Graphic/Texture/TextureHdl.h"


namespace Gamedesk {


class ENGINE_API LensFlare
{
public:
    LensFlare();
    void Render() const;

public:
    Vector3f        mPosition;
    
    HTexture2D      mFlare0;
    HTexture2D      mFlare1;
    HTexture2D      mFlare2;
    HTexture2D      mFlare3;
    HTexture2D      mFlare4;
};


} // namespace Gamedesk


#endif  //  _LENS_FLARE_H_
