/**
 *  @file       PSPTexture.h
 *  @brief	    PSP Texture.
 *  @author     Sébastien Lussier.
 *  @date       12/09/2008.
 */
/*
 *  Copyright (C) 2008 Gamedesk
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
#ifndef     _PSP_TEXTURE_H_
#define     _PSP_TEXTURE_H_


#include "Graphic/Texture/Texture.h"


class PSPGRAPHIC_API PSPTexture2D : public Texture2D
{
    friend class PSPGraphicSubsystem;
    DECLARE_CLASS(PSPTexture2D,Texture2D);

public:
    void SetWrapMode( WrapAxis pWrapAxis, WrapMode pWrapMode );
    void SetMagFilter( MagFilter pFilter );
    void SetMinFilter( MinFilter pFilter );
    void SetAnisotropy( Float pAnisotropy );

private:
    PSPTexture2D();    
    virtual void Init();
    virtual void Kill();
};


#endif  //  _PSP_TEXTURE_H_
